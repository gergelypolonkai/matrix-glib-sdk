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
 * Event type to hold the m.room.topic event.
 *
 * A topic is a short message detailing what is currently being
 * discussed in the room. It can also be used as a way to display
 * extra information about the room, which may not be suitable for the
 * room name.
 */
public class Matrix.Event.RoomTopic : Matrix.Event.State {
    /**
     * The topic text.
     */
    public string? topic { get; set; default = null; }

    protected override void
    from_json(Json.Node json_data)
        throws Matrix.Error
    {
        var root = json_data.get_object();
        var content_root = root.get_member("content").get_object();
        Json.Node? node = null;

        if (Config.DEBUG && ((node = root.get_member("state_key")) != null)) {
            if (node.get_string() != "") {
                warning("state_key of a m.room.topic event is non-empty");
            }
        }

        if ((node = content_root.get_member("topic")) != null) {
            _topic = node.get_string();
        } else if (Config.DEBUG) {
            warning("content.topic is missing from an m.room.topic event");
        }

        base.from_json(json_data);
    }

    protected override void
    to_json(Json.Node json_data)
        throws Matrix.Error
    {
        var root = json_data.get_object();
        var content_root = root.get_member("content").get_object();

        if (_state_key != "") {
            throw new Matrix.Error.INCOMPLETE(
                    "Won't generate a m.room.topic event with a non-empty state_key");
        }

        if (_topic != null) {
            content_root.set_string_member("topic", _topic);
        }

        base.to_json(json_data);
    }
}
