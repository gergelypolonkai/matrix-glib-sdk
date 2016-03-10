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
 * Class to hold an m.room.message event
 *
 * This event is used when sending messages in a room. Messages are
 * not limited to be text. The `msgtype` key outlines the type of
 * message, e.g. text, audio, image, video, etc. The `body` key is
 * text and MUST be used with every kind of `msgtype` as a fallback
 * mechanism for when a client cannot render a message. This allows
 * clients to display *something* even if it is just plain text.
 */
public class Matrix.Event.RoomMessage : Matrix.Event.Room {
    /**
     * The message as a Matrix.Message object.
     */
    public Matrix.Message.Base? message { get; set; default = null; }

    /**
     * The message as a JSON object. This gets set by
     * Matrix.Message.Base.new_from_json if no handler is installed
     * for the given message type.
     */
    public Json.Node? fallback_content {
        get {
            return _fallback_content;
        }
    }

    private Json.Node? _fallback_content;

    protected override void
    from_json(Json.Node json_data)
        throws Matrix.Error
    {
        var content_node = json_data.get_object().get_member("content");

        try {
            _message = Matrix.Message.Base.new_from_json(content_node);

            // We don't want to fail on unknown message types, so
            // let's save the JSON content instead. Silent
            // (ie. without exception) null is only returned if there
            // is no handler class installed
            if (_message == null) {
                _fallback_content = content_node;
            } else {
                _fallback_content = null;
            }
        } catch (GLib.Error e) {
            throw new Matrix.Error.INVALID_TYPE(
                    "Error during message initialization:%s",
                    e.message);
        }

        base.from_json(json_data);
    }

    protected override void
    to_json(Json.Node json_data)
        throws Matrix.Error
    {
        if (_message == null) {
            throw new Matrix.Error.INCOMPLETE(
                    "Won't generate a m.room.message event without content");
        }

        var root = json_data.get_object();

        root.set_member("content", _message.json);

        base.to_json(json_data);
    }
}
