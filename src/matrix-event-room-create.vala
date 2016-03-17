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
 * Class to hold a m.room.create event
 *
 * This is the first event in a room and cannot be changed. It acts as
 * the root of all other events.
 */
public class Matrix.Event.RoomCreate : Matrix.Event.State {
    /**
     * The user_id of the room creator. This is set by the homeserver.
     */
    public string? creator { get; set; default = null; }

    /**
     * Whether users on other servers can join this room. Defaults to
     * true if key does not exist.
     */
    public bool federate { get; set; default = false; }

    protected override void
    from_json(Json.Node json_data)
        throws Matrix.Error
    {
        var root = json_data.get_object();
        var content_root = root.get_member("content").get_object();
        Json.Node? node = null;

        if (Config.DEBUG && ((node = root.get_member("state_key")) != null)) {
            if (node.get_string() != "") {
                warning("state_key of a m.room.create event is non-empty");
            }
        }

        if ((node = content_root.get_member("creator")) != null) {
            _creator = node.get_string();
        } else {
            warning("content.creator is missing from a m.room.create event");
        }

        if ((node = content_root.get_member("m.federate")) != null) {
            _federate = node.get_boolean();
        }

        base.from_json(json_data);
    }

    protected override void
    to_json(Json.Node json_data)
        throws Matrix.Error
    {
        if (_creator == null) {
            throw new Matrix.Error.INCOMPLETE(
                    "Won't generate a m.room.create event without a creator key");
        }

        if (state_key != "") {
            throw new Matrix.Error.INCOMPLETE(
                    "Won't generater a m.root.create event with a non-empty state_key");
        }

        var content_root = json_data.get_object()
            .get_member("content").get_object();

        content_root.set_string_member("creator", _creator);
        content_root.set_boolean_member("m.federate", _federate);

        base.to_json(json_data);
    }
}
