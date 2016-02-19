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
}
