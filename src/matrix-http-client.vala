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
 * An event-driven client class to communicate with HTTP based
 * Matrix.org servers.
 */
public class Matrix.HTTPClient : Matrix.HTTPAPI, Matrix.Client {
    private bool _polling = false;
    private ulong _event_timeout = 30000;
    private string? _last_sync_token;
    private Gee.HashMap<string, Profile> _user_global_profiles =
        new Gee.HashMap<string, Profile>();
    private Gee.HashMap<string, Presence> _user_global_presence =
        new Gee.HashMap<string, Presence>();
    private Gee.HashMap<string, Room> _rooms =
        new Gee.HashMap<string, Room>();
    private ulong _last_txn_id = 0;

    public
    HTTPClient(string base_url)
    {
        Object(base_url : base_url);
    }

    public void
    login_with_password(string username, string password)
        throws Matrix.Error
    {
        var builder = new Json.Builder();

        builder.begin_object();

        builder.set_member_name("user");
        builder.add_string_value(username);

        builder.set_member_name("password");
        builder.add_string_value(password);

        builder.end_object();

        login((i, content_type, json_content, raw_content, error) => {
                login_finished((error == null) || (error is Matrix.Error.NONE));
            },
            "m.login.password",
            builder.get_root());
    }

    public void
    register_with_password(string? username, string password)
        throws Matrix.Error
    {
        register_account(
                (i, content_type, json_content, raw_content, error) => {
                    login_finished((error is Matrix.Error.NONE));
                },
                Matrix.AccountKind.USER,
                false, username, password);
    }

    public void
    logout()
        throws Matrix.Error
    {
        token = null;
        refresh_token = null;
        abort_pending();
    }

    private void
    process_event(Json.Node event_node, string? room_id)
    {
        Json.Object root_obj;
        Json.Node node;
        string event_type;
        Matrix.Event.Base? evt = null;

        if (event_node.get_node_type() != Json.NodeType.OBJECT) {
            if (Config.DEBUG) {
                warning("Received event that is not an object.");
            }

            return;
        }

        root_obj = event_node.get_object();

        if ((node = root_obj.get_member("type")) == null) {
            if (Config.DEBUG) {
                warning("Received event without type.");
            }

            return;
        }

        event_type = node.get_string();

        try {
            evt = Matrix.Event.Base.new_from_json(event_type, event_node);
        } catch (GLib.Error e) {
            evt = null;
        }

        if (evt != null) {
            // Make sure Room events have room_id set, even if it was
            // stripped by the HS
            if (evt is Matrix.Event.Room) {
                Matrix.Event.Room revt = (Matrix.Event.Room)evt;

                if (revt.room_id == null) {
                    revt.room_id = room_id;
                }
            }

            if (evt is Matrix.Event.Presence) {
                var pevt = (Matrix.Event.Presence)evt;
                var user_id = pevt.user_id;

                _user_global_presence[user_id] = pevt.presence;

                Profile? profile = _user_global_profiles[user_id];

                if (profile == null) {
                    profile = new Profile();
                    _user_global_profiles[user_id] = profile;
                }

                profile.avatar_url = pevt.avatar_url;
                profile.display_name = pevt.display_name;
            } else if (evt is Matrix.Event.Room) {
                var revt = (Matrix.Event.Room)evt;
                var room = _get_or_create_room(revt.room_id);

                if (evt is Matrix.Event.RoomMember) {
                    var mevt = (Matrix.Event.RoomMember)evt;
                    var user_id = mevt.user_id;
                    Profile? profile = null;

                    try {
                        profile = room.get_or_add_member(
                                user_id,
                                (mevt.tpi_display_name != null));
                    } catch (Matrix.Error e) {}

                    profile.avatar_url = mevt.avatar_url;
                    profile.display_name = mevt.display_name;
                } else if (evt is Matrix.Event.RoomAliases) {
                    var aevt = (Matrix.Event.RoomAliases)evt;

                    room.aliases = aevt.aliases;
                } else if (evt is Matrix.Event.RoomAvatar) {
                    var aevt = (Matrix.Event.RoomAvatar)evt;

                    room.avatar_url = aevt.url;
                    room.avatar_info = aevt.info;
                    room.avatar_thumbnail_url = aevt.thumbnail_url;
                    room.avatar_thumbnail_info = aevt.thumbnail_info;
                } else if (evt is Matrix.Event.RoomCanonicalAlias) {
                    var cevt = (Matrix.Event.RoomCanonicalAlias)evt;

                    room.canonical_alias = cevt.canonical_alias;
                } else if (evt is Matrix.Event.RoomCreate) {
                    var cevt = (Matrix.Event.RoomCreate)evt;

                    room.creator = cevt.creator;
                    room.federate = cevt.federate;
                } else if (evt is Matrix.Event.RoomGuestAccess) {
                    var gevt = (Matrix.Event.RoomGuestAccess)evt;

                    room.guest_access = gevt.guest_access;
                } else if (evt is Matrix.Event.RoomHistoryVisibility) {
                    var hevt = (Matrix.Event.RoomHistoryVisibility)evt;

                    room.history_visibility = hevt.visibility;
                } else if (evt is Matrix.Event.RoomJoinRules) {
                    var jevt = (Matrix.Event.RoomJoinRules)evt;

                    room.join_rules = jevt.join_rules;
                } else if (evt is Matrix.Event.RoomName) {
                    var nevt = (Matrix.Event.RoomName)evt;

                    room.name = nevt.name;
                } else if (evt is Matrix.Event.RoomPowerLevels) {
                    var levt = (Matrix.Event.RoomPowerLevels)evt;

                    room.default_power_level = levt.users_default;
                    room.default_event_level = levt.events_default;
                    room.default_state_level = levt.state_default;
                    room.ban_level = levt.ban;
                    room.kick_level = levt.kick;
                    room.redact_level = levt.redact;
                    room.invite_level = levt.invite;
                    room.clear_user_levels();
                    room.clear_event_levels();

                    foreach (var entry in levt.user_levels.entries) {
                        room.set_user_level(entry.key, entry.value);
                    }

                    foreach (var entry in levt.event_levels.entries) {
                        room.set_event_level(entry.key, entry.value);
                    }
                } else if (evt is Matrix.Event.RoomTopic) {
                    var tevt = (Matrix.Event.RoomTopic)evt;

                    room.topic = tevt.topic;
                }
            }
        }

        incoming_event(room_id, event_node, evt);
    }

    private Room
    _get_or_create_room(string room_id)
    {
        Room? room = null;

        if ((room = _rooms[room_id]) == null) {
            room = new Room(room_id);
            _rooms[room_id] = room;
        }

        return room;
    }

    private void
    _process_event_list_obj(Json.Node node, string? room_id)
        requires(node.get_node_type() == Json.NodeType.OBJECT)
    {
        Json.Object node_obj;
        Json.Node events_node;

        node_obj = node.get_object();

        if ((events_node = node_obj.get_member("events")) != null) {
            if (events_node.get_node_type() == Json.NodeType.ARRAY) {
                events_node.get_array().foreach_element(
                        (array, idx, event_node) => {
                            process_event(event_node, room_id);
                        });
            }
        }
    }

    private void
    cb_sync(string content_type,
            Json.Node? json_content,
            ByteArray? raw_content,
            Matrix.Error? error)
    {
        if (error == null) {
            var root_obj = json_content.get_object();
            Json.Node? node;

            if (Config.DEBUG) {
                debug("Processing account data");
            }

            _process_event_list_obj(root_obj.get_member("account_data"),
                                    null);

            if (Config.DEBUG) {
                debug("Processing presence");
            }

            _process_event_list_obj(root_obj.get_member("presence"),
                                    null);

            if ((node = root_obj.get_member("rooms")) != null) {
                if (node.get_node_type() == Json.NodeType.OBJECT) {
                    Json.Object rooms_object = node.get_object();
                    Json.Node rooms_node;

                    if (Config.DEBUG) {
                        debug("Processing rooms");
                    }

                    if ((rooms_node = rooms_object.get_member(
                                 "invite")) != null) {
                        rooms_node.get_object().foreach_member(
                                (obj, room_id, room_node) => {
                                    if (room_node.get_node_type() != Json.NodeType.OBJECT) {
                                        return;
                                    }

                                    _process_event_list_obj(
                                            room_node
                                                .get_object()
                                                .get_member("invite_state"),
                                            room_id);
                                });
                    }

                    if ((rooms_node = rooms_object.get_member(
                                 "join")) != null) {
                        rooms_node.get_object().foreach_member(
                                (obj, room_id, room_node) => {
                                    if (room_node.get_node_type() != Json.NodeType.OBJECT) {
                                        return;
                                    }

                                    _process_event_list_obj(
                                            room_node
                                                .get_object()
                                                .get_member("timeline"),
                                            room_id);
                                    _process_event_list_obj(
                                            room_node
                                                .get_object()
                                                .get_member("state"),
                                            room_id);
                                    _process_event_list_obj(
                                            room_node
                                                .get_object()
                                                .get_member("account_data"),
                                            room_id);
                                    _process_event_list_obj(
                                            room_node
                                                .get_object()
                                                .get_member("ephemeral"),
                                            room_id);
                                });
                    }

                    if ((rooms_node = rooms_object.get_member(
                                 "leave")) != null) {
                        rooms_node.get_object().foreach_member(
                                (obj, room_id, room_node) => {
                                    if (room_node.get_node_type() != Json.NodeType.OBJECT) {
                                        return;
                                    }

                                    _process_event_list_obj(
                                            room_node
                                                .get_object()
                                                .get_member("timeline"),
                                            room_id);
                                    _process_event_list_obj(
                                            room_node
                                                .get_object()
                                                .get_member("state"),
                                            room_id);
                                });
                    }
                }
            }

            if ((node = root_obj.get_member("next_batch")) != null) {
                _last_sync_token = node.get_string();
            }
        } else if ((error is Matrix.Error.M_FORBIDDEN)
                   || (error is Matrix.Error.M_UNKNOWN_TOKEN)
                   || (error is Matrix.Error.M_UNAUTHORIZED)) {
            try {
                token_refresh((i, ct, jc, rc, err) => {
                        login_finished((error == null)
                                       || (error is Matrix.Error.NONE));
                    } , null);
            } catch (Matrix.Error e) {}
        }

        // It is possible that polling has been disabled while we were
        // processing events. Don’t continue polling if that is the
        // case.
        if (_polling) {
            // This `500` should be changed to M_MISSING_TOKEN somehow
            try {
                if ((error == null) || (error.code < 500)) {
                    begin_polling();
                } else if ((error != null) && error.code >= 500) {
                    stop_polling(false);
                }
            } catch (Matrix.Error e) {}
        }
    }

    public void
    begin_polling()
        throws Matrix.Error
    {
        try {
            sync((API.Callback)cb_sync,
                 null, null, _last_sync_token,
                 false, false, _event_timeout);

            _polling = true;
        } catch (Matrix.Error e) {
            throw e;
        }

        _polling = true;
    }

    public void
    stop_polling(bool cancel_ongoing)
        throws Matrix.Error
    {
        _polling = false;

        if (cancel_ongoing) {
            abort_pending();
        }
    }

    public Profile?
    get_user_profile(string user_id, string? room_id = null)
        throws Matrix.Error
    {
        if (room_id == null) {
            var profile = _user_global_profiles[user_id];

            if (profile == null) {
                throw new Matrix.Error.UNAVAILABLE(
                        "Global profile for %s is not cached yet.",
                        user_id);
            }

            return profile;
        }

        var room = _rooms[room_id];

        if (room == null) {
            throw new Matrix.Error.UNAVAILABLE(
                    "Room data for %s is not cached yet.", room_id);
        }

        return room.get_member(user_id, null);
    }

    public Presence
    get_user_presence(string user_id, string? room_id = null)
        throws Matrix.Error
    {
        if (room_id == null) {
            Presence? presence = _user_global_presence[user_id];

            if (presence == null) {
                throw new Matrix.Error.UNAVAILABLE(
                        "Global presence for %s is not cached yet.",
                        user_id);
            }

            return presence;
        }

        throw new Matrix.Error.UNSUPPORTED(
                "Per-room presences are not supported yet.");
    }

    public Room
    get_room_by_id(string room_id)
        throws Matrix.Error
    {
        Room? room;

        if ((room = _rooms[room_id]) == null) {
            throw new Matrix.Error.UNAVAILABLE(
                    "Room data for %s is not cached yet.", room_id);
        }

        return room;
    }

    public Room
    get_room_by_alias(string room_alias)
        throws Matrix.Error
    {
        foreach (var entry in _rooms.entries) {
            var room = entry.value;

            if (room.canonical_alias == room_alias) {
                return room;
            }

            if (room_alias in room.aliases) {
                return room;
            }
        }

        throw new Matrix.Error.UNAVAILABLE(
                "Noo room data found for alias %s", room_alias);
    }

    /**
     * Get the next transaction ID to use. It increments the
     * internally stored value and returns that, so it is guaranteed
     * to be unique until we run out of ulong boundaries.
     *
     * It is called internally by send().
     */
    public ulong
    next_txn_id()
    {
        return ++_last_txn_id;
    }

    private void
    send_callback(Json.Node? json_content,
                  GLib.Error? err,
                  Matrix.Client.SendCallback? cb)
    {
        string? event_id = null;
        GLib.Error? new_err = err;

        // If there is no callback, there is no point to continue
        if (cb == null) {
            return;
        }

        if (err == null) {
            var root = json_content.get_object();

            if (root.has_member("event_id")) {
                event_id = root.get_string_member("event_id");
            } else {
                new_err = new Matrix.Error.BAD_RESPONSE(
                        "event_id is missing from an event response");
            }
        }

        cb(event_id, new_err);
    }

    public void
    send(string room_id,
         Matrix.Event.Base evt,
         Matrix.Client.SendCallback? cb,
         out ulong txn_id)
        throws Matrix.Error
    {
        var evt_node = evt.json;
        var evt_root = evt_node.get_object();
        string? state_key = null;

        if (evt_root.has_member("state_key")) {
            state_key = evt_root.get_string_member("state_key");
        }

        if (state_key != null) {
            txn_id = 0;
            send_state_event(
                    (i, ct, json_node, rc, err) =>
                        send_callback(json_node, err, cb),
                    room_id,
                    evt.event_type,
                    (state_key == "") ? null : state_key,
                    evt_root.get_member("content"));
        } else {
            txn_id = next_txn_id();
            send_event(
                    (i, ct, json_node, rc, err) =>
                        send_callback(json_node, err, cb),
                    room_id,
                    evt.event_type,
                    "%lu".printf(txn_id),
                    evt_root.get_member("content"));
        }
    }

    public void
    save_state(string filename)
        throws Matrix.Error, GLib.Error
    {
        var root = new Json.Object();

        root.set_string_member("base_url", base_url);

        root.set_boolean_member("validate_certificate", validate_certificate);

        if (user_id != null) {
            root.set_string_member("user_id", user_id);
        }

        if (homeserver != null) {
            root.set_string_member("homeserver_name", homeserver);
        }

        if (token != null) {
            root.set_string_member("access_token", token);
        }

        if (refresh_token != null) {
            root.set_string_member("refresh_token", refresh_token);
        }

        var node = new Json.Node(Json.NodeType.OBJECT);
        node.set_object(root);

        if (Config.DEBUG) {
            debug("Saving state to %s\n", filename);
        }

        var generator = new Json.Generator();
        generator.set_root(node);
        generator.to_file(filename);
    }

    public void
    load_state(string filename)
        throws Matrix.Error, GLib.Error
    {
        var parser = new Json.Parser();

        if (Config.DEBUG) {
            debug("Loading state from %s\n", filename);
        }

        parser.load_from_file(filename);
        Json.Node? node = parser.get_root();

        if (node.get_node_type() != Json.NodeType.OBJECT) {
            throw new Matrix.Error.INVALID_FORMAT(
                    "Save data must be a JSON object.");
        }

        var root = node.get_object();

        if ((node = root.get_member("base_url")) == null) {
            throw new Matrix.Error.INVALID_FORMAT(
                    "Save data has no base_url key");
        }

        base_url = node.get_string();

        if (Config.DEBUG) {
            debug("Loaded base URL %s", base_url);
        }

        if ((node = root.get_member("validate_certificate")) == null) {
            throw new Matrix.Error.INVALID_FORMAT(
                    "Save data has no validate_certificate key");
        }

        validate_certificate = node.get_boolean();

        if ((node = root.get_member("user_id")) != null) {
            _user_id = node.get_string();

            if (Config.DEBUG) {
                debug("Loaded user ID %s", user_id);
            }
        }

        if ((node = root.get_member("homeserver_name")) != null) {
            _homeserver = node.get_string();

            if (Config.DEBUG) {
                debug("Loaded homeserver name %s", homeserver);
            }
        }

        if ((node = root.get_member("access_token")) != null) {
            token = node.get_string();

            if (Config.DEBUG) {
                debug("Loaded access token %s", token);
            }
        }

        if ((node = root.get_member("refresh_token")) != null) {
            refresh_token = node.get_string();

            if (Config.DEBUG) {
                debug("Loaded refresh token %s", refresh_token);
            }
        }
    }
}
