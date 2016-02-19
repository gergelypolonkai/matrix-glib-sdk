/*
 * This file is part of matrix-glib-sdk
 *
 * matrix-glib-sdk is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version.
 *
 * matrix-glib-sdk is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with matrix-glib-sdk. If not, see
 * <http://www.gnu.org/licenses/>.
 */

/**
 * This is a class for low level communication with a Matrix.org
 * server via HTTP.
 */
[CCode (lower_case_csuffix = "http_api")]
public class Matrix.HTTPAPI : GLib.Object, Matrix.API {
    private enum CallType {
        API,
        MEDIA;
    }

    private const string API_ENDPOINT = "/_matrix/client/r0/";
    private const string MEDIA_ENDPOINT = "/_matrix/media/r0/";

    private Soup.Session _soup_session = new Soup.Session();
    private string? _base_url = null;
    private Soup.URI? _api_uri = null;
    private Soup.URI? _media_uri = null;

    public string base_url {
        get {
            return _base_url;
        }

        set {
            Soup.URI? api_uri;
            Soup.URI? media_uri;

            if (!value.is_ascii()) {
                warning("URL specified(%s) is not ASCII", value);

                return;
            }

            if (value.last_index_of(API_ENDPOINT) != -1) {
                warning("Provided URL (%s) already contains the API endpoint. Please use an URL without it!",
                        base_url);

                return;
            }

            _set_url(out api_uri, value, API_ENDPOINT);
            _set_url(out media_uri, value, MEDIA_ENDPOINT);

            if ((api_uri != null) && (media_uri != null)) {
                _api_uri = api_uri;
                _media_uri = media_uri;

                _token = null;
                _refresh_token = null;
                _homeserver = null;
                _user_id = null;

                debug("API URL: %s", api_uri.to_string(false));
                debug("Media URL: %s", media_uri.to_string(false));
            } else {
                warning("Invalid base URL: %s", value);
            }
        }
    }
    public bool validate_certificate {
        get {
            return _soup_session.ssl_strict;
        }

        set {
            _soup_session.ssl_strict = value;
        }
    }
    private string? _user_id;
    public string? user_id {
        get {
            return _user_id;
        }

        default = null;
    }
    public string? token { get; set; default = null; }
    public string? refresh_token { get; set; default = null; }
    private string? _homeserver;
    public string? homeserver {
        get {
            return _homeserver;
        }

        default = null;
    }

    private void
    _set_url(out Soup.URI? uri, string base_url, string endpoint)
    {
        string url;

        if (base_url[base_url.length - 1] == '/') {
            url = "%s%s".printf(base_url, endpoint.offset(1));
        } else {
            url = "%s%s".printf(base_url, endpoint);
        }

        var new_uri = new Soup.URI(url);

        if (SOUP_URI_VALID_FOR_HTTP(new_uri)) {
            uri = new_uri;
        } else {
            uri = null;
        }
    }

    protected
    HTTPAPI(string base_url, string? token = null)
    {
        Object(base_url : base_url, token : token);
        _soup_session.ssl_strict = true;
    }

    // This macro is not available in Vala
    private static bool
    SOUP_URI_VALID_FOR_HTTP(Soup.URI? uri)
    {
        return ((uri != null)
                && ((uri.scheme == "http")
                    || (uri.scheme == "https"))
                && (uri.host != null)
                && (uri.path != null));
    }

    private void
    _send(API.Callback? cb,
          CallType call_type,
          string method,
          string path,
          owned GLib.HashTable<string, string>? parms,
          string? content_type,
          Json.Node? json_content,
          ByteArray? raw_content,
          bool accept_non_json)
        throws Matrix.Error
    {
        if ((_api_uri == null) || (_media_uri == null)) {
            throw new Matrix.Error.COMMUNICATION_ERROR("No valid base URL");
        }

        if ((json_content != null) && (raw_content != null)) {
            critical("json_content and raw_content cannot be used together. This is a bug.");
        }

        if (!method.is_ascii()) {
            critical("Method must be ASCII. This is a bug.");
        }

        if ((method.ascii_casecmp("GET") != 0)
            && (method.ascii_casecmp("POST") != 0)
            && (method.ascii_casecmp("PUT") != 0)
            && (method.ascii_casecmp("DELETE") != 0)) {
            critical("Method %s is invalid. This is a bug.", method);
        }

        Soup.URI? request_path = null;

        if (call_type == CallType.MEDIA) {
            request_path = new Soup.URI.with_base(_media_uri, path);
        } else {
            request_path = new Soup.URI.with_base(_api_uri, path);
        }

        if (parms == null) {
            parms = _create_query_params();
        }

        if (token != null) {
            debug("Adding access token '%s'", token);

            parms.replace("access_token", token);
        }

        request_path.set_query_from_form(parms);

        var message = new Soup.Message.from_uri(method, request_path);

        uint8[] request_data;

        if (json_content != null) {
            var generator = new Json.Generator();
            generator.set_root(json_content);
            var json_str = generator.to_data(null);
            request_data = json_str.data;
        } else if (raw_content != null) {
            request_data = raw_content.data;
        } else {
            request_data = "{}".data;
        }

        debug("Sending %d bytes (%s %s): %s",
              request_data.length,
              method,
              request_path.to_string(false),
              (raw_content != null)
                  ? "<Binary data>"
                  : (string)request_data);

        message.set_flags(Soup.MessageFlags.NO_REDIRECT);
        message.set_request(
                (content_type == null)
                    ? "application/json"
                    : content_type,
                Soup.MemoryUse.COPY,
                request_data);

        _soup_session.queue_message(
                message,
                (session, msg) => _response_callback(msg,
                                                     call_type,
                                                     accept_non_json,
                                                     cb));
    }

    private void
    _response_callback(Soup.Message msg,
                       CallType call_type,
                       bool accept_non_json,
                       API.Callback? cb)
    {
        string request_url = msg.get_uri().get_path();
        Matrix.Error? err = null;
        ByteArray? raw_content = null;
        Json.Node? content = null;

        switch (call_type) {
            case CallType.API:
                request_url = request_url .substring(API_ENDPOINT.length);

                break;

            case CallType.MEDIA:
                request_url = request_url.substring(MEDIA_ENDPOINT.length);

                break;
        }

        if ((msg.status_code < 100)
            || (msg.status_code >= 400)) {
            err = new Matrix.Error.COMMUNICATION_ERROR(
                    "%s %u: %s",
                    (msg.status_code < 100) ? "Network error" : "HTTP",
                    msg.status_code,
                    msg.reason_phrase);
        } else {
            var buffer = msg.response_body.flatten();
            string data = (string)buffer.data;
            var datalen = buffer.length;
            var parser = new Json.Parser();
            bool is_json;

            try {
                is_json = parser.load_from_data(data, (ssize_t)datalen);
            } catch (GLib.Error e) {
                is_json = false;
            }

            if (is_json) {
                debug("Response (%s): %s", request_url, data);

                content = parser.get_root();

                if (content.get_node_type() == Json.NodeType.OBJECT) {
                    var root = content.get_object();
                    Json.Node node;

                    /* Check if the response holds an access token; if it
                     * does, set it as our new token */
                    if ((node = root.get_member("access_token")) != null) {
                        string? access_token;

                        if ((access_token = node.get_string()) != null) {
                            debug("Got new access token: %s", access_token);
                            token = access_token;
                        }
                    }

                    /* Check if the response holds a refresh token; if it
                     * does, set it as our new refresh token */
                    if ((node = root.get_member("refresh_token")) != null) {
                        string? refresh_token;

                        if ((refresh_token = node.get_string()) != null) {
                            debug("Got new refresh token: %s",
                                  refresh_token);
                            this.refresh_token = refresh_token;
                        }
                    }

                    /* Check if the response holds a homeserver name */
                    if ((node = root.get_member("home_server")) != null) {
                        string homeserver = node.get_string();

                        debug("Our home server calls itself %s", homeserver);
                        this._homeserver = homeserver;
                    }

                    /* Check if the response holds a user ID; if it does,
                     * set this as our user ID */
                    if ((node = root.get_member("user_id")) != null) {
                        string user_id = node.get_string();

                        debug("We are reported to be logged in as %s", user_id);
                        this._user_id = user_id;
                    }

                    /* Check if the response holds an error */
                    var errcode_node = root.get_member("errcode");
                    var error_node = root.get_member("error");
                    string? error = null;
                    string? errcode = null;

                    if ((errcode_node != null) || (error_node != null)) {
                        err = new Matrix.Error.UNKNOWN_ERROR(
                                "The error is not known to this library");

                        if (error_node != null) {
                            error = error_node.get_string();
                        }

                        if (errcode_node != null) {
                            errcode = errcode_node.get_string();

                            if (errcode.ascii_ncasecmp("M_", 2) == 0) {
                                // This is an ugly hack until Vala
                                // registers errordomains as an
                                // EnumClass
                                switch (errcode) {
                                    case "M_MISSING_TOKEN":
                                        err = new Matrix.Error.M_MISSING_TOKEN("");

                                        break;

                                    case "M_FORBIDDEN":
                                        err = new Matrix.Error.M_FORBIDDEN("");

                                        break;

                                    case "M_UNKNOWN":
                                        err = new Matrix.Error.M_UNKNOWN("");

                                        break;

                                    case "M_UNKNOWN_TOKEN":
                                        err = new Matrix.Error.M_UNKNOWN_TOKEN("");

                                        break;

                                    case "M_NOT_JSON":
                                        err = new Matrix.Error.M_NOT_JSON("");

                                        break;

                                    case "M_UNRECOGNIZED":
                                        err = new Matrix.Error.M_UNRECOGNIZED("");

                                        break;

                                    default:
                                        warning("An unknown error code '%s' was sent by the homeserver. You may want to report it to the Matrix GLib developers", errcode);

                                        break;
                                }
                            }
                        } else {
                            warning("An error was sent by the homeserver, but no error code was specified. You may want to report this to the homeserver admins.");
                            err = new Matrix.Error.UNSPECIFIED("No error code was sent by the server");
                        }

                        if ((errcode_node != null) && (error_node != null)) {
                            err.message = "%s: %s".printf(errcode, error);
                        } else if (errcode_node != null) {
                            err.message = errcode;
                        } else {
                            err.message = "(No errcode given) %s".printf(error);
                        }
                    }
                } else if (content.get_node_type() != Json.NodeType.ARRAY) {
                    err = new Matrix.Error.BAD_RESPONSE(
                            "Bad response: not a JSON object, nor an array.");
                }
            } else {
                if (accept_non_json) {
                    raw_content = new ByteArray.sized((uint)datalen);
                    raw_content.append(buffer.data);
                    debug("Binary data (%s): %u bytes", request_url, (uint)datalen);
                } else {
                    err = new Matrix.Error.BAD_RESPONSE(
                            "Malformed response (invalid JSON)");
                    debug("Malformed response (%s): %s", request_url, data);
                }
            }
        }

        /* Call the assigned function, if any */
        if (cb != null) {
            cb(this,
               msg.response_headers.get_content_type(null),
               content,
               raw_content,
               err);
        }
    }

    private static HashTable<string, string>
    _create_query_params()
    {
        return new HashTable<string, string>(str_hash, str_equal);
    }

    /* Media */

    public void
    media_download(API.Callback? cb,
                   string server_name,
                   string media_id)
        throws Matrix.Error
    {
        string path = "download/"
            + Soup.URI.encode(server_name, null)
            + "/"
            + Soup.URI.encode(media_id, null);

        _send(cb,
              CallType.MEDIA, "GET", path,
              null, null, null, null, true);
    }

    public void
    media_thumbnail(API.Callback? cb,
                    string server_name,
                    string media_id,
                    uint width,
                    uint height,
                    ResizeMethod method)
        throws Matrix.Error
    {
        string path = "download/"
            + Soup.URI.encode(server_name, null)
            + "/"
            + Soup.URI.encode(media_id, null);
        HashTable<string, string> parms;

        parms = _create_query_params();

        if (width > 0) {
            parms.replace("width", "%u".printf(width));
        }

        if (height > 0) {
            parms.replace("height", "%u".printf(height));
        }

        if (method != ResizeMethod.DEFAULT) {
            switch (method) {
                case ResizeMethod.CROP:
                    parms.replace("method", "crop");

                    break;

                case ResizeMethod.SCALE:
                    parms.replace("method", "scale");

                    break;

                // This is here to prevent compiler warnings
                case ResizeMethod.DEFAULT: break;
            }
        }

        _send(cb,
              CallType.MEDIA, "GET", path,
              parms, null, null, null, true);
    }

    public void
    media_upload(API.Callback? cb,
                 string? content_type,
                 owned ByteArray content)
        throws Matrix.Error
    {
        _send(cb,
              CallType.MEDIA, "POST", "upload",
              null, content_type, null, content, false);
    }

    /* Presence */

    public void
    get_presence_list(API.Callback? cb, string user_id)
        throws Matrix.Error
    {
        string path = "presence/list/" + Soup.URI.encode(user_id, null);

        _send(cb,
              CallType.API, "GET", path,
              null, null, null, null, false);
    }

    public void
    update_presence_list(API.Callback? cb,
                         string user_id,
                         List<string> drop_ids,
                         List<string> invite_ids)
        throws Matrix.Error
    {
        Json.Builder builder;
        string path = "presence/"
            + Soup.URI.encode(user_id, null)
            + "/status";

        builder = new Json.Builder();
        builder.begin_object();

        if (drop_ids != null) {
            builder.set_member_name("drop");
            builder.begin_array();
            drop_ids.foreach(
                    (entry) => {
                        builder.add_string_value(entry);
                    });
            builder.end_array();
        }

        if (invite_ids != null) {
            builder.set_member_name("invite");
            builder.begin_array();
            invite_ids.foreach(
                    (entry) => {
                        builder.add_string_value(entry);
                    });
            builder.end_array();
        }

        builder.end_object();

        _send(cb,
              CallType.API, "POST", path,
              null, null, builder.get_root(), null, false);
    }

    public void
    get_user_presence(API.Callback? cb,
                      string user_id)
        throws Matrix.Error
    {
        string path = "presence/"
            + Soup.URI.encode(user_id, null)
            + "/status";

        _send(cb,
              CallType.API, "GET", path,
              null, null, null, null, false);
    }

    public void
    set_user_presence(API.Callback? cb,
                      string user_id,
                      Presence presence,
                      string? status_message)
        throws Matrix.Error
    {
        Json.Builder builder;
        string path = "presence/"
            + Soup.URI.encode(user_id, null)
            + "/status";

        builder = new Json.Builder();
        builder.begin_object();

        builder.set_member_name("presence");
        builder.add_string_value(
                _g_enum_value_to_nick(typeof(Matrix.Presence),
                                      presence));

        if (status_message != null) {
            builder.set_member_name("status_msg");
            builder.add_string_value(status_message);
        }

        _send(cb,
              CallType.API, "POST", path,
              null, null, builder.get_root(), null, false);
    }

    /* Push notifications */

    public void
    update_pusher(API.Callback? cb,
                  Matrix.Pusher pusher)
        throws Matrix.Error
    {
        Json.Node? pusher_node;

        if ((pusher_node = pusher.get_json_node()) != null) {
            return;
        }

        _send(cb,
              CallType.API, "POST", "pushers/set",
              null, null, pusher_node, null, false);
    }

    public void
    get_pushers(API.Callback? cb)
        throws Matrix.Error
    {
        _send(cb,
              CallType.API, "GET", "pushrules",
              null, null, null, null, false);
    }

    private static string
    _pusher_url(string scope,
                PusherKind kind,
                string rule_id)
    {
        return "pushrules/"
            + Soup.URI.encode(scope, null)
            + "/"
            + Soup.URI.encode(_g_enum_value_to_nick(
                                      typeof(Matrix.PusherKind),
                                      kind),
                              null)
            + "/"
            + Soup.URI.encode(rule_id, null);
    }

    private void
    _pusher_modif(API.Callback? cb,
                  string method,
                  string scope,
                  PusherKind kind,
                  string rule_id)
        throws Matrix.Error
    {
        _send(cb,
              CallType.API, method,
              _pusher_url(scope, kind, rule_id),
              null, null, null, null, false);
    }

    public void
    delete_pusher(API.Callback? cb,
                  string scope,
                  PusherKind kind,
                  string rule_id)
        throws Matrix.Error
    {
        _pusher_modif(cb, "DELETE", scope, kind, rule_id);
    }

    public void
    get_pusher(API.Callback? cb,
               string scope,
               PusherKind kind,
               string rule_id)
        throws Matrix.Error
    {
        _pusher_modif(cb, "GET", scope, kind, rule_id);
    }

    public void
    add_pusher(API.Callback? cb,
               string scope,
               PusherKind kind,
               string rule_id,
               string? before,
               string? after,
               List<string> actions,
               List<PusherConditionKind?>? conditions)
        throws Matrix.Error
    {
        Json.Builder builder;
        HashTable<string, string> parms = _create_query_params();

        if (before != null) {
            parms.replace("before", before);
        }

        if (after != null) {
            parms.replace("after", after);
        }

        builder = new Json.Builder();
        builder.begin_object();

        builder.set_member_name("actions");
        builder.begin_array();
        actions.foreach(
                (entry) => {
                    builder.add_string_value(entry);
                });
        builder.end_array();

        if (conditions != null) {
            builder.set_member_name("conditions");
            builder.begin_array();
            conditions.foreach(
                    (entry) => {
                        string? kind_string = _g_enum_value_to_nick(
                                typeof(Matrix.PusherConditionKind),
                                entry);

                        if (kind_string == null) {
                            warning("Invalid condition kind");

                            return;
                        }

                        builder.begin_object();
                        builder.set_member_name("kind");
                        builder.add_string_value(kind_string);
                        builder.end_object();
                    });
            builder.end_array();
        }

        builder.end_object();

        _send(cb,
              CallType.API, "GET",
              _pusher_url(scope, kind, rule_id),
              parms, null, builder.get_root(), null, false);
    }

    public void
    toggle_pusher(API.Callback? cb,
                  string scope,
                  PusherKind kind,
                  string rule_id,
                  bool enabled)
        throws Matrix.Error
    {
        Json.Builder builder;

        builder = new Json.Builder();
        builder.begin_object();

        builder.set_member_name("enabled");
        builder.add_boolean_value(enabled);

        builder.end_object();

        _send(cb,
              CallType.API, "GET",
              _pusher_url(scope, kind, rule_id),
              null, null, builder.get_root(), null, false);
    }

    /* Room creation */

    public void
    create_room(API.Callback? cb,
                RoomPreset preset,
                string? room_name,
                string? room_alias,
                string? topic,
                RoomVisibility visibility,
                Json.Node? creation_content,
                List<StateEvent>? initial_state,
                List<string>? invitees,
                List<3PidCredential>? invite_3pids)
        throws Matrix.Error
    {
        Json.Builder builder = new Json.Builder();

        builder.begin_object();

        if (creation_content != null) {
            builder.set_member_name("creation_content");
            builder.add_value(creation_content);
        }

        if (initial_state != null) {
            builder.set_member_name("initial_state");
            builder.begin_array();
            initial_state.foreach(
                    (entry) => {
                        builder.add_value(entry.json);
                    });
            builder.end_array();
        }

        if (invitees != null) {
            builder.set_member_name("invite");
            builder.begin_array();
            invitees.foreach(
                    (entry) => {
                        builder.add_string_value(entry);
                    });
            builder.end_array();
        }

        if (invite_3pids != null) {
            builder.set_member_name("invite_3pid");
            builder.begin_array();
            invite_3pids.foreach(
                    (entry) => {
                        try {
                            builder.add_value(entry.get_json_node());
                            // TODO exceptions should be handled
                            // here somehow
                        } catch (Matrix.Error e) {}
                    });
            builder.end_array();
        }

        if (room_name != null) {
            builder.set_member_name("name");
            builder.add_string_value(room_name);
        }

        if (preset != RoomPreset.NONE) {
            string? preset_string = _g_enum_value_to_nick(
                    typeof(RoomPreset),
                    preset);

            if (preset_string != null) {
                builder.set_member_name("preset");
                builder.add_string_value(preset_string);
            } else {
                warning("Invalid room preset type");
            }
        }

        if (room_alias != null) {
            builder.set_member_name("room_alias_name");
            builder.add_string_value(room_alias);
        }

        if (topic != null) {
            builder.set_member_name("topic");
            builder.add_string_value(topic);
        }

        if (visibility != RoomVisibility.DEFAULT) {
            string? visibility_string = _g_enum_value_to_nick(
                    typeof(RoomVisibility), visibility);

            if (visibility_string != null) {
                builder.set_member_name("visibility");
                builder.add_string_value(visibility_string);
            } else {
                warning("Invalid room visibility type");
            }
        }

        _send(cb,
              CallType.API, "POST", "createRoom",
              null, null, builder.get_root(), null, false);
    }

    /* Room directory */

    public void
    delete_room_alias(API.Callback? cb,
                      string room_alias)
        throws Matrix.Error
    {
        string path = "room/" + Soup.URI.encode(room_alias, null);

        _send(cb,
              CallType.API, "DELETE", path,
              null, null, null, null, false);
    }

    public void
    get_room_id(API.Callback? cb,
                string room_alias)
        throws Matrix.Error
    {
        string path = "room/" + Soup.URI.encode(room_alias, null);

        _send(cb,
              CallType.API, "GET", path,
              null, null, null, null, false);
    }

    public void
    create_room_alias(API.Callback? cb,
                      string room_id,
                      string room_alias)
        throws Matrix.Error
    {
        Json.Builder builder;
        string path = "room/" + Soup.URI.encode(room_alias, null);

        builder = new Json.Builder();
        builder.begin_object();

        builder.set_member_name("room_id");
        builder.add_string_value(room_id);

        builder.end_object();

        _send(cb,
              CallType.API, "PUT", path,
              null, null, builder.get_root(), null, false);
    }

    /* Room discovery */

    public void
    list_public_rooms(API.Callback? cb)
        throws Matrix.Error
    {
        _send(cb,
              CallType.API, "GET", "publicRooms",
              null, null, null, null, false);
    }

    /* Room membership */

    public void
    ban_user(API.Callback? cb,
             string room_id,
             string user_id,
             string? reason)
        throws Matrix.Error
    {
        string path = "rooms/"
            + Soup.URI.encode(room_id, null)
            + "/ban";
        var builder = new Json.Builder();

        builder.begin_object();

        builder.set_member_name("user_id");
        builder.add_string_value(user_id);

        if (reason != null) {
            builder.set_member_name("reason");
            builder.add_string_value(reason);
        }

        builder.end_object();

        _send(cb,
              CallType.API, "POST", path,
              null, null, builder.get_root(), null, false);
    }

    public void
    forget_room(API.Callback? cb,
                string room_id)
        throws Matrix.Error
    {
        string path = "rooms/"
            + Soup.URI.encode(room_id, null)
            + "/forget";

        _send(cb,
              CallType.API, "POST", path,
              null, null, null, null, false);
    }

    public void
    invite_user_3rdparty(API.Callback? cb,
                         string room_id,
                         Matrix.3PidCredential credential)
        throws Matrix.Error
    {
        string path = "rooms/"
            + Soup.URI.encode(room_id, null)
            + "/invite";
        Json.Node? body;

        if ((body = credential.get_json_node()) == null) {
            return;
        }

        _send(cb,
              CallType.API, "POST", path,
              null, null, body, null, false);
    }

    public void
    invite_user(API.Callback? cb,
                string room_id,
                string user_id)
        throws Matrix.Error
    {
        var builder = new Json.Builder();
        string path = "rooms/"
            + Soup.URI.encode(room_id, null)
            + "/invite";

        builder.begin_object();

        builder.set_member_name("user_id");
        builder.add_string_value(user_id);

        builder.end_object();

        _send(cb,
              CallType.API, "POST", path,
              null, null, builder.get_root(), null, false);
    }

    public void
    join_room(API.Callback? cb,
              string room_id)
        throws Matrix.Error
    {
        string path = "rooms/"
            + Soup.URI.encode(room_id, null)
            + "/join";

        _send(cb,
              CallType.API, "POST", path,
              null, null, null, null, false);
    }

    public void
    leave_room(API.Callback? cb,
               string room_id)
        throws Matrix.Error
    {
        string path = "rooms/"
            + Soup.URI.encode(room_id, null)
            + "/leave";

        _send(cb,
              CallType.API, "POST", path,
              null, null, null, null, false);
    }

    /* Room participation */

    public void
    event_stream(API.Callback? cb,
                 string? from_token,
                 ulong timeout)
        throws Matrix.Error
    {
        HashTable<string, string> parms = _create_query_params();

        if (from_token != null) {
            parms.replace("from", from_token);
        }

        if (timeout > 0) {
            parms.replace("timeout", "%lu".printf(timeout));
        }

        _send(cb,
              CallType.API, "GET", "events",
              parms, null, null, null, false);
    }

    public void
    get_event(API.Callback? cb,
              string event_id)
        throws Matrix.Error
    {
        string path = "events/" + Soup.URI.encode(event_id, null);

        _send(cb,
              CallType.API, "GET", path,
              null, null, null, null, false);
    }

    public void
    initial_sync(API.Callback? cb,
                 uint limit,
                 bool archived)
        throws Matrix.Error
    {
        HashTable<string, string> parms = _create_query_params();

        if (limit != 0) {
            parms.replace("limit", "%u".printf(limit));
        }

        if (archived) {
            parms.replace("archived", "true");
        }

        _send(cb,
              CallType.API, "GET", "initialSync",
              parms, null, null, null, false);
    }

    public void
    get_event_context(API.Callback? cb,
                      string room_id,
                      string event_id,
                      uint limit)
        throws Matrix.Error
    {
        string path = "rooms/"
            + Soup.URI.encode(room_id, null)
            + "/"
            + Soup.URI.encode(event_id, null);
        HashTable<string, string>? parms = null;

        if (limit > 0) {
            parms = _create_query_params();

            parms.replace("limit", "%u".printf(limit));
        }

        _send(cb,
              CallType.API, "GET", path,
              parms, null, null, null, false);
    }

    public void
    initial_sync_room(API.Callback? cb,
                      string room_id)
        throws Matrix.Error
    {
        string path = "rooms/"
            + Soup.URI.encode(room_id, null)
            + "/initialSync";

        _send(cb,
              CallType.API, "GET", path,
              null, null, null, null, false);
    }

    public void
    list_room_members(API.Callback? cb,
                      string room_id)
        throws Matrix.Error
    {
        string path = "rooms/"
            + Soup.URI.encode(room_id, null)
            + "/members";

        _send(cb,
              CallType.API, "GET", path,
              null, null, null, null, false);
    }

    public void
    list_room_messages(API.Callback? cb,
                       string room_id,
                       string from_token,
                       EventDirection direction,
                       uint limit)
        throws Matrix.Error
    {
        string path = "rooms/"
            + Soup.URI.encode(room_id, null)
            + "/messages";
        HashTable<string, string> parms = _create_query_params();

        parms.replace("from", from_token);

        switch (direction) {
            case EventDirection.BACKWARD:
                parms.replace("dir", "b");

                break;

            case EventDirection.FORWARD:
                parms.replace("dir", "f");

                break;
        }

        if (limit > 0) {
            parms.replace("limit", "%u".printf(limit));
        }

        _send(cb,
              CallType.API, "GET", path,
              parms, null, null, null, false);
    }

    public void
    send_event_receipt(API.Callback? cb,
                       string room_id,
                       ReceiptType receipt_type,
                       string event_id,
                       Json.Node receipt)
        throws Matrix.Error
    {
        string path = "rooms/"
            + Soup.URI.encode(room_id, null)
            + "/receipt/"
            + Soup.URI.encode(_g_enum_value_to_nick(
                                      typeof(ReceiptType),
                                      receipt_type),
                              null)
            + "/"
            + Soup.URI.encode(event_id, null);

        _send(cb,
              CallType.API, "POST", path,
              null, null, receipt, null, false);
    }

    public void
    redact_event(API.Callback? cb,
                 string room_id,
                 string event_id,
                 string txn_id,
                 string? reason)
        throws Matrix.Error
    {
        Json.Node? body = null;
        string path = "rooms/%s/redact/%s/%s".printf(
                Soup.URI.encode(room_id, null),
                Soup.URI.encode(event_id, null),
                Soup.URI.encode(txn_id, null));

        if (reason != null) {
            var builder = new Json.Builder();
            builder.begin_object();

            builder.set_member_name("reason");
            builder.add_string_value(reason);

            builder.end_object();
            body = builder.get_root();
        }

        _send(cb,
              CallType.API, "PUT", path,
              null, null, body, null, false);
    }

    public void
    send_message_event(API.Callback? cb,
                       string room_id,
                       string event_type,
                       string txn_id,
                       owned Json.Node content)
        throws Matrix.Error
    {
        string path = "rooms/%s/send/%s/%s".printf(
                Soup.URI.encode(room_id, null),
                Soup.URI.encode(event_type, null),
                Soup.URI.encode(txn_id, null));

        _send(cb,
              CallType.API, "PUT", path,
              null, null, content, null, false);
    }

    public void
    get_room_state(API.Callback? cb,
                   string room_id,
                   string? event_type,
                   string? state_key)
        throws Matrix.Error
    {
        string path = "rooms/%s/state".printf(
                Soup.URI.encode(room_id, null));

        if (event_type != null) {
            path += "/%s".printf(Soup.URI.encode(event_type, null));
        }

        if (state_key != null) {
            path += "/%s".printf(Soup.URI.encode(state_key, null));
        }

        _send(cb,
              CallType.API, "GET", path,
              null, null, null, null, false);
    }

    public void
    send_room_event(API.Callback? cb,
                    string room_id,
                    string event_type,
                    string? state_key,
                    owned Json.Node content)
        throws Matrix.Error
    {
        string path = "rooms/%s/state/%s".printf(
                Soup.URI.encode(room_id, null),
                Soup.URI.encode(event_type, null));

        if (state_key != null) {
            path += "/%s".printf(Soup.URI.encode(state_key, null));
        }

        _send(cb,
              CallType.API, "PUT", path,
              null, null, content, null, false);
    }

    public void
    notify_room_typing(API.Callback? cb,
                       string user_id,
                       string room_id,
                       uint timeout,
                       bool typing)
        throws Matrix.Error
    {
        string path = "rooms/%s/typing/%s".printf(
                Soup.URI.encode(room_id, null),
                Soup.URI.encode(user_id, null));
        var builder = new Json.Builder();

        builder.begin_object();

        if (timeout > 0) {
            builder.set_member_name("timeout");
            builder.add_int_value(timeout);
        }

        builder.set_member_name("typing");
        builder.add_boolean_value(typing);

        _send(cb,
              CallType.API, "PUT", path,
              null, null, builder.get_root(), null, false);
    }

    public void
    sync(API.Callback? cb,
         string? filter_id,
         Filter? filter,
         string? since,
         bool full_state,
         bool set_presence,
         ulong timeout)
        throws Matrix.Error
    {
        HashTable<string, string> parms = _create_query_params();

        if ((filter_id != null) && (filter != null)) {
            throw new Matrix.Error.BAD_REQUEST(
                    "Cannot set both filter_id and filter");
        }

        if (filter_id != null) {
            parms.replace("filter", filter_id);
        }

        if (filter != null) {
            parms.replace("filter", filter.get_json_data(null));
        }

        if (since != null) {
            parms.replace("since", since);
        }

        parms.replace("full_state", (full_state) ? "true" : "false");

        if (!set_presence) {
            parms.replace("set_presence", "offline");
        }

        if (timeout != 0) {
            parms.replace("timeout", "%lu".printf(timeout));
        }

        _send(cb,
              CallType.API, "GET", "sync",
              parms, null, null, null, false);
    }

    public void
    create_filter(API.Callback? cb,
                  string user_id,
                  Filter filter)
        throws Matrix.Error
    {
        string path = "user/%s/filter".printf(
                Soup.URI.encode(user_id, null));

        _send(cb,
              CallType.API, "POST", path,
              null, null, filter.get_json_node(), null, false);
    }

    public void
    download_filter(API.Callback? cb,
                    string user_id,
                    string filter_id)
        throws Matrix.Error
    {
        string path = "user/%s/filter/%s".printf(
                Soup.URI.encode(user_id, null),
                Soup.URI.encode(filter_id, null));

        _send(cb,
              CallType.API, "GET", path,
              null, null, null, null, false);
    }

    /* Search */

    public void
    search(Matrix.API.Callback? cb,
           string? next_batch,
           SearchCategories search_categories)
    throws Matrix.Error
    {
        HashTable<string, string>? parms = null;

        if (next_batch == null) {
            parms = _create_query_params();

            parms.replace("next_batch", next_batch);
        }

        _send(cb,
              CallType.API, "POST", "search",
              parms, null, search_categories.get_json_node(), null, false);
    }

    /* Server administration */

    public void
    whois(API.Callback? cb, string user_id)
        throws Matrix.Error
    {
        string path = "admin/whois/" + Soup.URI.encode(user_id, null);

        _send(cb,
              CallType.API, "GET", path,
              null, null, null, null, false);
    }

    public void
    versions(API.Callback? cb)
        throws Matrix.Error
    {
        _send(cb,
              CallType.API, "GET", "versions",
              null, null, null, null, false);
    }

    /* Session management */

    public void
    login(API.Callback? cb,
          string login_type,
          Json.Node? content)
        throws Matrix.Error
    {
        Json.Node body = _json_node_deep_copy(content);

        body.get_object().set_string_member("type", login_type);

        _send(cb,
              CallType.API, "POST", "login",
              null, null, body, null, false);
    }

    public void
    token_refresh(API.Callback? cb,
                  string? refresh_token)
        throws Matrix.Error
    {
        if ((refresh_token == null) && (this.refresh_token == null)) {
            throw new Matrix.Error.M_MISSING_TOKEN("No token available");
        }

        var builder = new Json.Builder();

        builder.begin_object();

        builder.set_member_name("refresh_token");
        builder.add_string_value((refresh_token != null)
                                 ? refresh_token
                                 : this.refresh_token);

        builder.end_object();

        _send(cb,
              CallType.API, "POST", "tokenrefresh",
              null, null, builder.get_root(), null, false);
    }

    /* User data */

    public void
    get_3pids(API.Callback? cb)
        throws Matrix.Error
    {
        _send(cb,
              CallType.API, "GET", "account/3pid",
              null, null, null, null, false);
    }

    public void
    add_3pid(API.Callback? cb,
             bool bind_creds,
             Matrix.3PidCredential creds)
        throws Matrix.Error
    {
        var id_node = creds.get_json_node();
        var builder = new Json.Builder();

        builder.begin_object();

        builder.set_member_name("bind");
        builder.add_boolean_value(bind_creds);

        builder.set_member_name("threePidCreds");
        builder.add_value(id_node);

        _send(cb,
              CallType.API, "POST", "account/3pid",
              null, null, builder.get_root(), null, false);
    }

    public void
    change_password(API.Callback? cb,
                    string new_password)
        throws Matrix.Error
    {
        var builder = new Json.Builder();

        builder.begin_object();

        builder.set_member_name("new_password");
        builder.add_string_value(new_password);

        builder.end_object();

        _send(cb,
              CallType.API, "POST", "account/password",
              null, null, builder.get_root(), null, false);
    }

    public void
    get_profile(API.Callback? cb,
                string user_id)
        throws Matrix.Error
    {
        string path = "profile/%s".printf(Soup.URI.encode(user_id, null));

        _send(cb,
              CallType.API, "GET", path,
              null, null, null, null, false);
    }

    public void
    get_avatar_url(API.Callback? cb,
                   string user_id)
        throws Matrix.Error
    {
        string path = "profile/%s/avatar_url".printf(
                Soup.URI.encode(user_id, null));

        _send(cb,
              CallType.API, "GET", path,
              null, null, null, null, false);
    }

    public void
    set_avatar_url(API.Callback? cb,
                   string user_id,
                   string avatar_url)
        throws Matrix.Error
    {
        string path = "profile/%s/avatar_url".printf(
                Soup.URI.encode(user_id, null));
        var builder = new Json.Builder();

        builder.begin_object();

        builder.set_member_name("avatar_url");
        builder.add_string_value(avatar_url);

        builder.end_object();

        _send(cb,
              CallType.API, "PUT", path,
              null, null, builder.get_root(), null, false);
    }

    public void
    get_display_name(API.Callback? cb,
                     string user_id)
        throws Matrix.Error
    {
        string path = "profile/%s/displayname".printf(
                Soup.URI.encode(user_id, null));

        _send(cb,
              CallType.API, "GET", path,
              null, null, null, null, false);
    }

    public void
    set_display_name(API.Callback? cb,
                     string user_name,
                     string display_name)
        throws Matrix.Error
    {
        string path = "profile/%s/displayname".printf(
                Soup.URI.encode(user_id, null));
        var builder = new Json.Builder();

        builder.begin_object();

        builder.set_member_name("displayname");
        builder.add_string_value(display_name);

        builder.end_object();

        _send(cb,
              CallType.API, "PUT", path,
              null, null, builder.get_root(), null, false);
    }

    public void
    register_account(API.Callback? cb,
                     AccountKind account_kind,
                     bool bind_email,
                     string? username,
                     string password)
        throws Matrix.Error
    {
        HashTable<string, string>? parms = null;
        var builder = new Json.Builder();

        builder.begin_object();

        builder.set_member_name("bind_email");
        builder.add_boolean_value(bind_email);

        if (username != null) {
            builder.set_member_name("username");
            builder.add_string_value(username);
        }

        builder.set_member_name("password");
        builder.add_string_value(password);

        builder.end_object();

        if (account_kind != AccountKind.DEFAULT) {
            string? kind_string = _g_enum_value_to_nick(
                    typeof(Matrix.AccountKind), account_kind);

            if (kind_string != null) {
                parms = _create_query_params();

                parms.replace("kind", kind_string);
            }
        }

        _send(cb,
              CallType.API, "POST", "register",
              parms, null, builder.get_root(), null, false);
    }

    public void
    set_account_data(API.Callback? cb,
                     string user_id,
                     string? room_id,
                     string event_type,
                     owned Json.Node content)
        throws Matrix.Error
    {
        string path;

        if (room_id != null) {
            path = "user/%s/rooms/%s/account_data/%s".printf(
                    Soup.URI.encode(user_id, null),
                    Soup.URI.encode(room_id, null),
                    Soup.URI.encode(event_type, null));
        } else {
            path = "user/%s/account_data/%s".printf(
                    Soup.URI.encode(user_id, null),
                    Soup.URI.encode(event_type, null));
        }

        _send(cb,
              CallType.API, "PUT", path,
              null, null, content, null, false);
    }

    public void
    get_room_tags(API.Callback? cb,
                  string user_id,
                  string room_id)
        throws Matrix.Error
    {
        string path = "user/%s/rooms/%s/tags".printf(
                Soup.URI.encode(user_id, null),
                Soup.URI.encode(room_id, null));

        _send(cb,
              CallType.API, "GET", path,
              null, null, null, null, false);
    }

    public void
    delete_room_tag(API.Callback? cb,
                    string user_id,
                    string room_id,
                    string tag)
        throws Matrix.Error
    {
        string path = "user/%s/rooms/%s/tags/%s".printf(
                Soup.URI.encode(user_id, null),
                Soup.URI.encode(room_id, null),
                Soup.URI.encode(tag, null));

        _send(cb,
              CallType.API, "DELETE", path,
              null, null, null, null, false);
    }

    public void
    add_room_tag(API.Callback? cb,
                 string user_id,
                 string room_id,
                 string tag,
                 owned Json.Node? content)
        throws Matrix.Error
    {
        string path = "user/%s/rooms/%s/tags/%s".printf(
                Soup.URI.encode(user_id, null),
                Soup.URI.encode(room_id, null),
                Soup.URI.encode(tag, null));

        _send(cb,
              CallType.API, "PUT", path,
              null, null, content, null, false);
    }

    /* VoIP */

    public void
    get_turn_server(API.Callback? cb)
        throws Matrix.Error
    {
        _send(cb,
              CallType.API, "GET", "voip/turnServer",
              null, null, null, null, false);
    }

    /* Non-spec methods */

    public void
    abort_pending()
    {
        _soup_session.abort();
    }
}
