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
}
