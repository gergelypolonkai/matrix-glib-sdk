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
 * Class to hold a m.room.message.feedback event.
 *
 * Usage of this event is discouraged in favour of the receipts
 * module. Most clients will not recognise this event.
 *
 * Feedback events are events sent to acknowledge a message in some
 * way. There are two supported acknowledgements: `delivered` (sent
 * when the event has been received) and `read` (sent when the event
 * has been observed by the end-user). The `target_event_id` should
 * reference the `m.room.message` event being acknowledged.
 */
public class Matrix.Event.RoomMessageFeedback : Matrix.Event.Room {
    /**
     * The type of the feedback. As the use of this event type is
     * discouraged, Matrix GLib SDK doesn’t implement this as an
     * actual enum.
     */
    public string? feedback_type { get; set; default = null; }

    /**
     * The event that this feedback is related to.
     */
    public string? target_event_id { get; set; default = null; }

    protected override void
    from_json(Json.Node json_data)
        throws Matrix.Error
    {
        var content_root = json_data.get_object()
            .get_member("content").get_object();
        Json.Node? node;

        if ((node = content_root.get_member("type")) != null) {
            _feedback_type = node.get_string();
        } else {
            warning("content.type is missing from a m.room.message.feedback event");
        }

        if ((node = content_root.get_member("target_event_id")) != null) {
            _target_event_id = node.get_string();
        } else {
            warning("content.target_event_id is missing from a m.room.message.feedback event");
        }

        base.from_json(json_data);
    }

    protected override void
    to_json(Json.Node json_data)
        throws Matrix.Error
    {
        if ((_feedback_type == null) || (_target_event_id == null)) {
            throw new Matrix.Error.INCOMPLETE(
                    "Won't generate a m.room.message.feedback without all fields set");
        }

        var content_root = json_data.get_object()
            .get_member("content").get_object();

        content_root.set_string_member("type", _feedback_type);
        content_root.set_string_member("target_event_id", _target_event_id);

        base.to_json(json_data);
    }
}
