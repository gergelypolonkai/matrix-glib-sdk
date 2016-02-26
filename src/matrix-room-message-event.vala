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

public class Matrix.RoomMessageEvent : Matrix.RoomEvent {
    public string msg_type { get; set; }
    public string body { get; set; }

    protected override void
    from_json(Json.Node json_data)
        throws Matrix.Error
    {
        var root = json_data.get_object();

        if (root.get_member("content") == null) {
            throw new Matrix.Error.INCOMPLETE(
                    "Message event without a content!");
        }

        var content_root = root.get_member("content").get_object();
        Json.Node? node;

        if ((node = content_root.get_member("msgtype")) == null) {
            throw new Matrix.Error.INCOMPLETE(
                    "Message event without a message type!");
        }

        _msg_type = node.get_string();

        if ((node = content_root.get_member("body")) == null) {
            throw new Matrix.Error.INCOMPLETE(
                    "Message event without a body!");
        }

        _body = node.get_string();

        base.from_json(json_data);
    }
}
