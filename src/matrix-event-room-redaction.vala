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
 * Class to hold a m.room.redaction event
 *
 * Events can be redacted by either room or server admins. Redacting
 * an event means that all keys not required by the protocol are
 * stripped off, allowing admins to remove offensive or illegal
 * content that may have been attached to any event. This cannot be
 * undone, allowing server owners to physically delete the offending
 * data. There is also a concept of a moderator hiding a message
 * event, which can be undone, but cannot be applied to state
 * events. The event that has been redacted is specified in the
 * redacts event level key.
 */
public class Matrix.Event.RoomRedaction : Matrix.Event.Room {
    /**
     * The reason for the redaction, if any.
     */
    public string? reason { get; set; default = null; }

    /**
     * The event ID that was redacted.
     */
    public string? redacted_event_id { get; set; default = null; }

    protected override void
    from_json(Json.Node json_data)
        throws Matrix.Error
    {
        var root = json_data.get_object();
        var content_root = root.get_member("content").get_object();
        Json.Node? node;

        if ((node = content_root.get_member("reason")) != null) {
            _reason = node.get_string();
        }

        if ((node = root.get_member("redacts")) != null) {
            _redacted_event_id = node.get_string();
        } else {
            warning("redacts is missing from a m.room.redaction event");
        }

        base.from_json(json_data);
    }

    protected override void
    to_json(Json.Node json_data)
        throws Matrix.Error
    {
        if (_redacted_event_id == null) {
            throw new Matrix.Error.INCOMPLETE(
                    "Won't generate a m.room.redaction event without the redacts field");
        }

        var root = json_data.get_object();

        root.set_string_member("redacts", _redacted_event_id);

        if (_reason != null) {
            var content_root = root.get_member("content").get_object();

            content_root.set_string_member("reason", _reason);
        }

        base.to_json(json_data);
    }
}
