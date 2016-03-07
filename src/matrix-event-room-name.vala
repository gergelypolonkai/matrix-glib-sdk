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
 * Class to hold a m.room.name event.
 *
 * A room has an opaque room ID which is not human-friendly to read. A
 * room alias is human-friendly, but not all rooms have room
 * aliases. The room name is a human-friendly string designed to be
 * displayed to the end-user. The room name is not unique, as multiple
 * rooms can have the same room name set. The room name can also be
 * set when creating a room using createRoom with the name key.
 */
public class Matrix.Event.RoomName : Matrix.Event.State {
    public string? name { get; set; default = null; }

    protected override void
    from_json(Json.Node json_data)
        throws Matrix.Error
    {
        var content_root = json_data.get_object()
            .get_member("content").get_object();
        Json.Node? node;

        if ((node = content_root.get_member("name")) != null) {
            _name = node.get_string();
        } else {
            warning("content.name is missing from a m.room.name event");
        }

        base.from_json(json_data);
    }

    protected override void
    to_json(Json.Node json_data)
        throws Matrix.Error
    {
        var content_root = json_data.get_object()
            .get_member("content").get_object();

        if (_name == null) {
            throw new Matrix.Error.INCOMPLETE(
                    "Won't send a m.room.name event without a name");
        }

        content_root.set_string_member("name", _name);

        base.to_json(json_data);
    }
}
