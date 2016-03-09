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
 * Abstract base class for room events.
 */
public abstract class Matrix.Event.Room : Matrix.Event.Base {
    /**
     * A globally unique event ID. Required.
     */
    public string? event_id { get; set; default = null; }

    /**
     * The ID of the room associated with this event. Required.
     */
    public string? room_id { get; set; default = null; }

    /**
     * The fully qualified Matrix ID of the user who sent the
     * event. Required.
     */
    public string? sender { get; set; default = null; }

    /**
     * The time, in milliseconds, that has elapsed since the event was
     * sent. This is part of the unsigned event data.
     */
    public ulong? age { get; set; default = null; }

    /**
     * The reason this event was redacted, if it was redacted.
     */
    public string? redacted_because { get; set; default = null; }

    /**
     * The client-supplied transaction ID. This should only be set if
     * the client being given the event is the same one which sent it.
     */
    public string? transaction_id { get; set; default = null; }

    protected override void
    from_json(Json.Node json_data)
        throws Matrix.Error
    {
        Json.Node? node;
        var root = json_data.get_object();

        if ((node = root.get_member("event_id")) != null) {
            _event_id = node.get_string();
        } else if (Config.DEBUG) {
            warning("event_id is missing from a Room event");
        }

        if ((node = root.get_member("room_id")) != null) {
            _room_id = node.get_string();
        } else if (Config.DEBUG) {
            warning("room_id is missing from a Room event");
        }

        if ((node = root.get_member("sender")) != null) {
            _sender = node.get_string();
        } else if (Config.DEBUG) {
            warning("sender is missing from a Room event");
        }

        if ((node = root.get_member("unsigned")) != null) {
            var unsigned_root = node.get_object();

            if ((node = unsigned_root.get_member("age")) != null) {
                _age = (int)node.get_int();
            }

            if ((node = unsigned_root.get_member("redacted_because")) != null) {
                _redacted_because = node.get_string();
            }

            if ((node = unsigned_root.get_member("transaction_id")) != null) {
                _transaction_id = node.get_string();
            }
        }

        base.from_json(json_data);
    }

    protected override void
    to_json(Json.Node json_data)
        throws Matrix.Error
    {
        var root_obj = json_data.get_object();

        if (event_id != null) {
            root_obj.set_string_member("event_id", _event_id);
        }

        if (room_id != null) {
            root_obj.set_string_member("room_id", _room_id);
        }

        if (sender != null) {
            root_obj.set_string_member("sender", _sender);
        }

        var unsigned_obj = new Json.Object();

        if (age != null) {
            unsigned_obj.set_int_member("age", _age);
        }

        if (redacted_because != null) {
            unsigned_obj.set_string_member("redacted_because",
                                           _redacted_because);
        }

        if (transaction_id != null) {
            unsigned_obj.set_string_member("transaction_id",
                                           _transaction_id);
        }

        if (unsigned_obj.get_size() > 0) {
            var unsigned_node = new Json.Node(Json.NodeType.OBJECT);
            unsigned_node.set_object(unsigned_obj);
            root_obj.set_member("unsigned", unsigned_node);
        }

        base.to_json(json_data);
    }
}
