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
 * Class for representing a room membership events
 *
 * The room membership event class.
 */
public class Matrix.RoomMemberEvent : Matrix.RoomEvent {
    public string? state_key { get; set; }
    public RoomMembership membership {
        get; set;
        default = RoomMembership.UNKNOWN;
    }
    public string? avatar_url { get; set; }
    public string? display_name { get; set; }

    protected override void
    from_json(Json.Node json_data)
        throws Matrix.Error
    {
        Json.Object root = json_data.get_object();
        Json.Object content_root = root.get_member("content").get_object();
        Json.Node? node;

        if ((node = root.get_member("state_key")) != null) {
            _state_key = node.get_string();
        } else if (Config.DEBUG) {
            warning("state_key is missing from the m.room.member event");
        }

        if ((node = root.get_member("room_id")) != null) {
            _room_id = node.get_string();
        } else if (Config.DEBUG) {
            warning("room_id is missing from the m.room.member event");
        }

        if ((node = content_root.get_member("membership")) != null) {
            Matrix.RoomMembership? mship = (Matrix.RoomMembership?)_g_enum_nick_to_value(
                    typeof(Matrix.RoomMembership),
                    node.get_string());

            if (mship != null) {
                _membership = mship;
            }
        } else if (Config.DEBUG) {
            warning("membership key is missing from the m.room.member event");
        }

        if ((node = content_root.get_member("avatar_url")) != null) {
            _avatar_url = node.get_string();
        }

        if ((node = content_root.get_member("displayname")) != null) {
            _display_name = node.get_string();
        }

        // Chain up
        base.from_json(json_data);
    }

    protected override void
    to_json(Json.Node json_data)
        throws Matrix.Error
    {
        Json.Object root, content_root;
        string? mship;

        if (membership == RoomMembership.UNKNOWN) {
            throw new Matrix.Error.UNKNOWN_VALUE(
                    "Unknown membership value cannot be added to a room member event");
        }

        root = json_data.get_object();

        content_root = _json_object_node_ensure_field(json_data,
                                                      "content",
                                                      Json.NodeType.OBJECT)
            .get_object();

        root.set_string_member("state_key", state_key);

        if (room_id != null) {
            root.set_string_member("room_id", room_id);
        }

        mship = _g_enum_value_to_nick(typeof(Matrix.RoomMembership),
                                      membership);

       if (mship != null) {
            content_root.set_string_member("membership", mship);
        }

        if (avatar_url != null) {
            content_root.set_string_member("avatar_url", avatar_url);
        }

        if (display_name != null) {
            content_root.set_string_member("displayname", display_name);
        }

        base.to_json(json_data);
    }
}
