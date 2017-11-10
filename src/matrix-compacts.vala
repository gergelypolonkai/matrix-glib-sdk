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
    public class EventContext : JsonCompact {
        public int before_limit { get; set; default = -1; }
        public int after_limit { get; set; default = -1; }
        public bool include_profile { get; set; default = false; }

        public override Json.Node?
        get_json_node()
            throws Matrix.Error
        {
            var builder = new Json.Builder();

            builder.begin_object();

            if (before_limit >= 0) {
                builder.set_member_name("before_limit");
                builder.add_int_value(before_limit);
            }

            if (after_limit >= 0) {
                builder.set_member_name("after_limit");
                builder.add_int_value(after_limit);
            }

            builder.set_member_name("include_profile");
            builder.add_boolean_value(include_profile);

            builder.end_object();

            return builder.get_root();
        }
     }

    public class SearchGrouping : JsonCompact {
        public SearchGroupBy key { get; set; default = SearchGroupBy.NONE; }

        public override Json.Node?
        get_json_node()
            throws Matrix.Error
        {
            if (key == SearchGroupBy.NONE) {
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
        public SearchGrouping[] group_by { get; set; }

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

            int count = 0;

            foreach (var entry in group_by) {
                var node = entry.get_json_node();

                if (node != null) {
                    count++;
                    builder.add_value(node);
                }
            }

            if (count == 0) {
                return null;
            }

            builder.end_array();

            builder.end_object();

            return builder.get_root();
        }
    }

    public class SearchRoomEvents : JsonCompact {
        public SearchOrder order_by { get; set; default = SearchOrder.RECENT; }
        public SearchKey[] keys { get; set; }
        public EventContext? event_context { get; set; default = null; }
        public bool include_state { get; set; default = false; }
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

            builder.set_member_name("order_by");
            builder.add_string_value(
                    _g_enum_value_to_nick(typeof(SearchOrder), order_by));

            if (keys.length > 0) {
                EnumClass key_class = (EnumClass)(typeof(SearchKey).class_ref());
                var key_array = new Json.Array();

                foreach (var entry in keys) {
                    unowned EnumValue? key_value = key_class.get_value(entry);

                    if (key_value != null) {
                        key_array.add_string_element(
                                key_value.value_nick.replace("-", "."));
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

            builder.set_member_name("include_state");
            builder.add_boolean_value(include_state);

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
