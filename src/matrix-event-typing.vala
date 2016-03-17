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
 * Class to hold an m.typing event.
 *
 * Informs the client of the list of users currently typing.
 */
public class Matrix.Event.Typing : Matrix.Event.Base {
    /**
     * The ID of the room this event belongs to.
     */
    public string? room_id { get; set; default = null; }

    /**
     * The list of user IDs typing in this room, if any.
     */
    public string[] user_ids { get; set; }

    protected override void
    from_json(Json.Node json_data)
        throws Matrix.Error
    {
        var root = json_data.get_object();
        var content_root = root.get_member("content").get_object();
        Json.Node? node;

        if ((node = root.get_member("room_id")) != null) {
            _room_id = node.get_string();
        } else if (Config.DEBUG) {
            warning("room_id is missing from a m.typing event");
        }

        if ((node = content_root.get_member("user_ids")) != null) {
            _user_ids = new string[node.get_array().get_length()];

            node.get_array().foreach_element((ary, idx, user_node) => {
                    _user_ids[idx] = user_node.get_string();
                });
        } else if (Config.DEBUG) {
            warning("content.user_ids is missing from a m.typing event");
        }

        base.from_json(json_data);
    }

    protected override void
    to_json(Json.Node json_data)
        throws Matrix.Error
    {
        if (_room_id == null) {
            throw new Matrix.Error.INCOMPLETE(
                    "Won't generate a m.typing event without room_id");
        }

        var root = json_data.get_object();
        var content_root = root.get_member("content").get_object();

        root.set_string_member("room_id", _room_id);

        var users_ary = new Json.Array();
        var users_node = new Json.Node(Json.NodeType.ARRAY);
        content_root.set_member("user_ids", users_node);

        foreach (var entry in _user_ids) {
            users_ary.add_string_element(entry);
        }

        base.to_json(json_data);
    }
}
