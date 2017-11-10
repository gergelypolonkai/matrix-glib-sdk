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
