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
}
