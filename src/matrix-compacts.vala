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

namespace Matrix {
    /**
     * Abstract parent class for classes that can be saved to JSON.
     */
    public abstract class JsonCompact {
        public abstract Json.Node?
        get_json_node()
            throws Matrix.Error;

        public string
        get_json_data(out size_t datalen)
            throws Matrix.Error
        {
            var generator = new Json.Generator();

            generator.set_root(get_json_node());

            return generator.to_data(out datalen);
        }
    }

    /**
     * Class to hold a filter.
     */
    public class Filter : JsonCompact {
        private List<string>? _event_fields;

        /**
         * The event fields to include in the filtered events.
         */
        public List<string>? event_fields {
            get {
                return _event_fields;
            }

            set {
                _event_fields = value.copy();
            }

            default = null;
        }

        /**
         * The desired event format for the filtered events (e.g. for
         * matrix_api_sync())
         */
        public EventFormat event_format {
            get; set;
            default = Matrix.EventFormat.CLIENT;
        }

        /**
         * A filtering ruleset for presence events.
         */
        public FilterRules? presence_filter { get; set; default = null; }

        /**
         * A filtering ruleset for room events.
         */
        public RoomFilter? room_filter { get; set; default = null; }

        /**
         * Get the filter as a JSON node.
         */
        public override Json.Node?
        get_json_node()
            throws Matrix.Error
        {
            var builder = new Json.Builder();

            builder.begin_object();

            builder.set_member_name("event_fields");
            builder.begin_array();

            foreach (var entry in event_fields) {
                builder.add_string_value(entry);
            }

            builder.end_array();

            builder.set_member_name("event_format");
            builder.add_string_value(
                    _g_enum_value_to_nick(typeof(EventFormat), event_format));

            builder.set_member_name("presence");
            builder.add_value(presence_filter.get_json_node());

            builder.set_member_name("room");
            builder.add_value(room_filter.get_json_node());

            builder.end_object();

            return builder.get_root();
        }
    }

    /**
     * Class to hold room filters.
     */
    public class RoomFilter : JsonCompact {
        /**
         * If {{{true}}}, events for rooms that the user has left will
         * be included in the filtered event list.
         */
        public bool include_leave { get; set; default=true; }

        /**
         * Filtering rules for ephemeral events, i.e. events that are
         * not recorded in the room history (typing notifications,
         * receipts, etc.)
         */
        public FilterRules? ephemeral { get; set; default = null; }

        /**
         * Filtering rules for state events.
         */
        public FilterRules? state { get; set; default = null; }

        /**
         * Filtering rules for timeline events.
         */
        public FilterRules? timeline { get; set; default = null; }

        /**
         * Get the room filters as a JSON node.
         */
        public override Json.Node?
        get_json_node()
            throws Matrix.Error
        {
            var builder = new Json.Builder();

            builder.begin_object();

            builder.set_member_name("include_leave");
            builder.add_boolean_value(include_leave);

            if (ephemeral != null) {
                builder.set_member_name("ephemeral");
                builder.add_value(ephemeral.get_json_node());
            }

            if (state != null) {
                builder.set_member_name("state");
                builder.add_value(state.get_json_node());
            }

            if (timeline != null) {
                builder.set_member_name("timeline");
                builder.add_value(timeline.get_json_node());
            }

            builder.end_object();

            return builder.get_root();
        }
    }

    /**
     * Class to hold filtering rules.
     */
    public class FilterRules : JsonCompact {
        private List<string>? _types;
        private List<string>? _excluded_types;
        private List<string>? _senders;
        private List<string>? _excluded_senders;
        private List<string>? _rooms;
        private List<string>? _excluded_rooms;

        /**
         * The limit of the count of returned events.
         */
        public uint limit { get; set; }

        /**
         * List of message types to include in the filtered result.
         */
        public List<string>? types {
            get {
                return _types;
            }

            set {
                _types = value.copy();
            }

            default = null;
        }

        /**
         * List of message types to exclude from the filtered
         * result. A matching type will be excluded from the result
         * even if it is listed in the types to include.
         */
        public List<string>? excluded_types {
            get {
                return _excluded_types;
            }

            set {
                _excluded_types = value.copy();
            }

            default = null;
        }

        /**
         * List of senders to include in the filtered results.
         */
        public List<string>? senders {
            get {
                return _senders;
            }

            set {
                _senders = value.copy();
            }

            default = null;
        }

        /**
         * List of senders to exclude from the filtered result. A
         * matching sender will be excluded from the result even if it
         * is listed in the senders to include.
         */
        public List<string>? excluded_senders {
            get {
                return _excluded_senders;
            }

            set {
                _excluded_senders = value.copy();
            }

            default = null;
        }

        /**
         * List of rooms to include in the filtered results.
         */
        public List<string>? rooms {
            get {
                return _rooms;
            }

            set {
                _rooms = value.copy();
            }

            default = null;
        }

        /**
         * List of rooms to exclude from the filtered result. A
         * matching room will be excluded from the result even if it
         * is listed in the rooms to include.
         */
        public List<string>? excluded_rooms {
            get {
                return _excluded_rooms;
            }

            set {
                _excluded_rooms = value.copy();
            }

            default = null;
        }

        /**
         * Get the filtering rules as a JSON node.
         */
        public override Json.Node?
        get_json_node()
            throws Matrix.Error
        {
            var builder = new Json.Builder();
            builder.begin_object();

            builder.set_member_name("limit");
            builder.add_int_value(limit);

            if (rooms != null) {
                builder.set_member_name("rooms");
                builder.begin_array();
                foreach (var entry in rooms) {
                    builder.add_string_value(entry);
                }
                builder.end_array();
            }

            if (excluded_rooms != null) {
                builder.set_member_name("not_rooms");
                builder.begin_array();
                foreach (var entry in excluded_rooms) {
                    builder.add_string_value(entry);
                }
                builder.end_array();
            }

            if (senders != null) {
                builder.set_member_name("senders");
                builder.begin_array();
                foreach (var entry in senders) {
                    builder.add_string_value(entry);
                }
                builder.end_array();
            }

            if (excluded_senders != null) {
                builder.set_member_name("not_senders");
                builder.begin_array();
                foreach (var entry in excluded_senders) {
                    builder.add_string_value(entry);
                }
                builder.end_array();
            }

            if (types != null) {
                builder.set_member_name("types");
                builder.begin_array();
                foreach (var entry in types) {
                    builder.add_string_value(entry);
                }
                builder.end_array();
            }

            if (excluded_types != null) {
                builder.set_member_name("not_types");
                builder.begin_array();
                foreach(var entry in types) {
                    builder.add_string_value(entry);
                }
                builder.end_array();
            }

            builder.end_object();

            return builder.get_root();
        }
    }

    /**
     * Class to hold 3rd party credential related data.
     */
    public class @3PidCredential : JsonCompact {
        /**
         * The Identity Server used for this credential.
         */
        public string? id_server { get; set; default = null; }

        /**
         * The session identifier got from the Identity Server.
         */
        public string? session_id { get; set; default = null; }

        /**
         * The client secret that was used in the session with the
         * Identity Server.
         */
        public string? client_secret { get; set; default = null; }

        /**
         * Get 3rd party credential related data as a JSON node.
         */
        public override Json.Node?
        get_json_node()
            throws Matrix.Error
        {
            if ((id_server == null)
                || (session_id == null)
                || (client_secret == null))
            {
                throw new Matrix.Error.INCOMPLETE(
                        "All fields of a 3PID credential must be filled!");
            }

            var builder = new Json.Builder();

            builder.begin_object();

            builder.set_member_name("id_server");
            builder.add_string_value(id_server);

            builder.set_member_name("session_id");
            builder.add_string_value(session_id);

            builder.set_member_name("client_secret");
            builder.add_string_value(client_secret);

            builder.end_object();

            return builder.get_root();
        }
    }

    /**
     * Class to hold pusher related data.
     */
    public class Pusher : JsonCompact {
        /**
         * A device display name.
         */
        public string? device_display_name { get; set; default = null; }

        /**
         * An application display name.
         */
        public string? app_display_name { get; set; default = null; }

        /**
         * An application ID.
         */
        public string? app_id { get; set; default = null; }

        /**
         * If {{{true}}}, the homeserver should add another pusher
         * with the given push key and app ID in addition to any
         * others with different user IDs. Otherwise, the homeserver
         * must remove any other pushers with the same App ID and
         * pushkey for different users.
         */
        public bool append { get; set; default = true; }

        /**
         * The kind of the pusher. {{{http}}} makes a pusher that
         * sends HTTP pokes. {{{null}}} deletes the pusher.
         */
        public string? kind { get; set; default = null; }

        /**
         * The preferred language for receiving notifications,
         * e.g. {{{en}}} or {{{en-US}}}.
         */
        public string? lang { get; set; default = null; }

        /**
         * A string that determines what set of device rules will be
         * matched when evaluating push rules for this pusher. It is
         * an arbitrary string. Multiple devices may use the same
         * profile tag. It is advised that when an app's data is
         * copied or restored to a different device, this value remain
         * the same. Client apps should offer ways to change the
         * profile tag, optionally copying rules from the old profile
         * tag. Maximum length is 32 bytes. If the profile tag is
         * longer than this, it will be truncated
         */
        public string? profile_tag { get; set; default = null; }

        /**
         * A unique identifier for this pusher. The value you should
         * use for this is the routing or destination address
         * information for the notification, for example, the APNS
         * token for APNS or the Registration ID for GCM. If your
         * notification client has no such concept, use any unique
         * identifier. Maximum length is 512 bytes. If pushkey is
         * longer than this, it will be truncated
         */
        public string? pushkey { get; set; default = null; }

        /**
         * a dictionary of information for the pusher implementation
         * itself. For example, if kind is {{{http}}}, this should
         * contain an {{{url}}} member, which is the URL to use to
         * send notifications to. This function creates a deep copy of
         * the data, so it can be freed after this call.
         */
        public Json.Node? data { get; set; default = null; }

        /**
         * Get the pusher data as a JSON node.
         */
        public override Json.Node?
        get_json_node()
            throws Matrix.Error
        {
            if ((device_display_name == null)
                || (app_display_name == null)
                || (app_id == null)
                || (data == null)
                || (kind == null)
                || (lang == null)
                || (profile_tag == null)
                || (pushkey == null)) {
                throw new Matrix.Error.INCOMPLETE("Pusher data incomplete");
            }

            var builder = new Json.Builder();

            builder.begin_object();

            builder.set_member_name("device_display_name");
            builder.add_string_value(device_display_name);

            builder.set_member_name("app_display_name");
            builder.add_string_value(app_display_name);

            builder.set_member_name("app_id");
            builder.add_string_value(app_id);

            builder.set_member_name("append");
            builder.add_boolean_value(append);

            builder.set_member_name("kind");
            builder.add_string_value(kind);

            builder.set_member_name("lang");
            builder.add_string_value(lang);

            builder.set_member_name("profile_tag");
            builder.add_string_value(profile_tag);

            builder.set_member_name("pushkey");
            builder.add_string_value(pushkey);

            builder.set_member_name("data");
            builder.add_value(data);

            builder.end_object();

            return builder.get_root();
        }
    }

    public class EventContext : JsonCompact {
        public int? before_limit { get; set; default = null; }
        public int? after_limit { get; set; default = null; }
        public bool? include_profile { get; set; default = null; }

        public override Json.Node?
        get_json_node()
            throws Matrix.Error
        {
            if ((before_limit == null)
                && (after_limit == null)
                && (include_profile == null))
            {
                return null;
            }

            var builder = new Json.Builder();

            builder.begin_object();

            if (before_limit != null) {
                builder.set_member_name("before_limit");
                builder.add_int_value(before_limit);
            }

            if (after_limit != null) {
                builder.set_member_name("after_limit");
                builder.add_int_value(after_limit);
            }

            if (include_profile != null) {
                builder.set_member_name("include_profile");
                builder.add_boolean_value(include_profile);
            }

            builder.end_object();

            return builder.get_root();
        }
     }

    public class SearchGrouping : JsonCompact {
        public SearchGroupBy? key { get; set; default = null; }

        public override Json.Node?
        get_json_node()
            throws Matrix.Error
        {
            if (key == null) {
                return null;
            }

            var builder = new Json.Builder();

            builder.begin_object();

            builder.set_member_name("key");
            builder.add_string_value(
                    _g_enum_value_to_nick(typeof(SearchGroupBy), key, true));

            builder.end_object();

            return builder.get_root();
        }
    }

    public class SearchGroupings : JsonCompact {
        private List<SearchGrouping>? _group_by = null;
        public List<SearchGrouping>? group_by {
            get {
                return _group_by;
            }

            set {
                _group_by = value.copy();
            }

            default = null;
        }

        public override Json.Node?
        get_json_node()
            throws Matrix.Error
        {
            if (group_by == null) {
                return null;
            }

            var builder = new Json.Builder();

            builder.begin_object();

            builder.set_member_name("group_by");
            builder.begin_array();

            foreach (var entry in group_by) {
                var node = entry.get_json_node();

                if (node != null) {
                    builder.add_value(node);
                }
            }

            builder.end_array();

            builder.end_object();

            return builder.get_root();
        }
    }

    public class SearchRoomEvents : JsonCompact {
        private List<SearchKey?>? _keys = null;

        public SearchOrder? order_by { get; set; default = SearchOrder.RECENT; }
        public List<SearchKey?>? keys {
            get {
                return _keys;
            }

            set {
                _keys = value.copy();
            }

            default = null; }
        public EventContext? event_context { get; set; default = null; }
        public bool? include_state { get; set; default = false; }
        public string? filter_id { get; set; default = null; }
        public Filter? filter { get; set; default = null; }
        public string search_term { get; set; }
        public SearchGroupings? groupings { get; set; default = null; }

        public override Json.Node?
        get_json_node()
            throws Matrix.Error
        {
            Json.Node? node = null;

            var builder = new Json.Builder();

            if ((filter_id != null) && (filter != null)) {
                throw new Matrix.Error.INCOMPLETE(
                        "filter and filter_id is exclusive to each other");
            }

            builder.begin_object();

            if (order_by != null) {
                builder.set_member_name("order_by");
                builder.add_string_value(
                        _g_enum_value_to_nick(typeof(SearchOrder), order_by));
            }

            if (keys != null) {
                EnumClass key_class = (EnumClass)(typeof(SearchKey).class_ref());
                var key_array = new Json.Array();

                foreach (var entry in keys) {
                    if (entry != null) {
                        unowned EnumValue? key_value = key_class.get_value(entry);

                        if (key_value != null) {
                            key_array.add_string_element(
                                    key_value.value_nick.replace("-", "."));
                        }
                    }
                }

                if (key_array.get_length() > 0) {
                    node = new Json.Node(Json.NodeType.ARRAY);
                    node.set_array(key_array);

                    builder.set_member_name("keys");
                    builder.add_value(node);
                }
            }

            if ((event_context != null)
                && ((node = event_context.get_json_node()) != null)) {
                builder.set_member_name("event_context");
                builder.add_value(node);
            }

            if (include_state != null) {
                builder.set_member_name("include_state");
                builder.add_boolean_value(include_state);
            }

            if ((filter != null)
                && ((node = filter.get_json_node()) != null)) {
                builder.set_member_name("filter");
                builder.add_value(node);
            }

            if (filter_id != null) {
                builder.set_member_name("filter");
                builder.add_string_value(filter_id);
            }

            builder.set_member_name("search_term");
            builder.add_string_value(search_term);

            if ((groupings != null)
                && ((node = groupings.get_json_node()) != null)) {
                builder.set_member_name("groupings");
                builder.add_value(node);
            }

            builder.end_object();

            return builder.get_root();
        }
    }

    public class SearchCategories : JsonCompact {
        public SearchRoomEvents? room_events { get; set; default = null; }

        public override Json.Node?
        get_json_node()
            throws Matrix.Error
        {
            Json.Node? node = null;

            if ((room_events == null)
                && ((node = room_events.get_json_node()) != null)) {
                return null;
            }

            var builder = new Json.Builder();

            builder.begin_object();

            builder.set_member_name("room_events");
            builder.add_value(node);

            builder.end_object();

            return builder.get_root();
        }
    }

    public Json.Node?
    _json_node_deep_copy(Json.Node? node)
    {
        Json.Node ret;

        if (node == null) {
            return null;
        }

        ret = new Json.Node(node.get_node_type());

        switch (node.get_node_type()) {
            case Json.NodeType.OBJECT:
                var new_obj = new Json.Object();

                node.get_object().foreach_member(
                        (old_obj, member_name, member_node) => {
                            new_obj.set_member(
                                    member_name,
                                    _json_node_deep_copy(member_node));
                        });

                ret.set_object(new_obj);

                break;

            case Json.NodeType.ARRAY:
                var new_ary = new Json.Array();

                node.get_array().foreach_element(
                        (old_ary, idx, element_node) => {
                            new_ary.add_element(
                                    _json_node_deep_copy(element_node));
                        });

                break;

            case Json.NodeType.VALUE:
                ret.set_value(node.get_value());

                break;

            case Json.NodeType.NULL:
                break;
        }

        return ret;
    }
}
