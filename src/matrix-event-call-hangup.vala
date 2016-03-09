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
 * Sent by either party to signal their termination of the call. This
 * can be sent either once the call has has been established or before
 * to abort the call.
 */
public class Matrix.Event.CallHangup : Matrix.Event.Room {
    /**
     * The ID of the call this event relates to.
     */
    public string? call_id { get; set; default = null; }

    /**
     * The version of the VoIP specification this message adheres to.
     */
    public int? version { get; set; default = null; }

    protected override void
    from_json(Json.Node json_data)
        throws Matrix.Error
    {
        var content_root = json_data.get_object()
            .get_member("content").get_object();
        Json.Node? node;

        if ((node = content_root.get_member("call_id")) != null) {
            _call_id = node.get_string();
        } else {
            warning("content.call_id is missing from a m.call.hangup event");
        }

        if ((node = content_root.get_member("version")) != null) {
            _version = (int)node.get_int();
        } else {
            warning("content.version is missing from a m.call.hangup event");
        }

        base.from_json(json_data);
    }

    protected override void
    to_json(Json.Node json_data)
        throws Matrix.Error
    {
        if (_call_id == null) {
            throw new Matrix.Error.INCOMPLETE(
                    "Won't generate a m.call.hangup event without call_id");
        }

        if (_version == null) {
            throw new Matrix.Error.INCOMPLETE(
                    "Won't generate a m.call.hangup event without version");
        }

        var content_root = json_data.get_object()
            .get_member("content").get_object();

        content_root.set_string_member("call_id", _call_id);
        content_root.set_int_member("version", version);

        base.to_json(json_data);
    }
}
