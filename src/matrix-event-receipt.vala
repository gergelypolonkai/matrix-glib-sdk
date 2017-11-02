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
 * Class to hold m.receipt events.
 *
 * Informs the client of new receipts.
 */
public class Matrix.Event.Receipt : Matrix.Event.Base {
    public string? room_id { get; set; default = null; }

    private struct ReceiptData {
        string event_id;
        string typ;
        string user;
    }

    private HashTable<ReceiptData?, ulong?> _receipt_data = null;

    private static bool
    _rd_equal(ReceiptData k1, ReceiptData k2)
    {
        return ((k1.event_id == k2.event_id)
                && (k1.typ == k2.typ)
                && (k1.user == k2.user));
    }

    private void
    _init_receipt_data()
    {
        _receipt_data = new HashTable<ReceiptData?, ulong?>(null, (EqualFunc)_rd_equal);
    }

    protected override void
    from_json(Json.Node json_data)
        throws Matrix.Error
    {
        var root = json_data.get_object();
        var content_root = root.get_member("content").get_object();
        Json.Node? node;

        if ((node = root.get_member("room_id")) != null) {
            _room_id = node.get_string();
        } else if (Config.DEBUG) {
            warning("room_id is missing from a m.receipt event");
        }

        content_root.foreach_member((obj, event_id, event_content) => {
                if ((node = event_content.get_object()
                     .get_member("m.read")) != null) {
                    var read_obj = node.get_object();
                    read_obj.foreach_member((robj, r_user_id, r_content) => {
                            if (_receipt_data == null) {
                                _init_receipt_data();
                            }

                            ReceiptData rd_key = ReceiptData() {
                                event_id = event_id,
                                typ = "m.read",
                                user = r_user_id
                            };

                            _receipt_data[rd_key] =
                                (ulong)r_content.get_object().get_member("ts").get_int();
                        });
                } else {
                    warning("content.$event-id.m.read is missing from a m.presence event");
                }
            });

        base.from_json(json_data);
    }

    protected override void
    to_json(Json.Node json_data)
        throws Matrix.Error
    {
        Json.Node? node = null;

        if (_room_id == null) {
            throw new Matrix.Error.INCOMPLETE(
                    "Won't generate a m.receipt without room_id");
        }

        var root = json_data.get_object();
        var content_root = root.get_member("content").get_object();
        int i = 0;
        Matrix.Error? error = null;

        _receipt_data.foreach(
            (key, value) => {
                Json.Object event_object;
                Json.Object type_object;
                Json.Object user_object;

                if (key.event_id == null) {
                    error = new Matrix.Error.INCOMPLETE(
                            "Won't generate a m.receipt event with an empty event ID");
                }

                if (key.typ == null) {
                    error = new Matrix.Error.INCOMPLETE(
                            "Won't generate a m.receipt event with an empty receipt type");
                }

                if (key.user == null) {
                    error = new Matrix.Error.INCOMPLETE(
                            "Won't generate a m.receipt event with an empty user ID");
                }

                i++;

                if ((node = content_root.get_member(key.event_id)) == null) {
                    event_object = new Json.Object();
                    node = new Json.Node(Json.NodeType.OBJECT);
                    node.set_object(event_object);
                    content_root.set_member(key.event_id, node);
                } else {
                    event_object = node.get_object();
                }

                if ((node = event_object.get_member(key.typ)) == null) {
                    type_object = new Json.Object();
                    node = new Json.Node(Json.NodeType.OBJECT);
                    node.set_object(type_object);
                    event_object.set_member(key.typ, node);
                } else {
                    type_object = node.get_object();
                }

                if ((node = type_object.get_member(key.user)) == null) {
                    user_object = new Json.Object();
                    node = new Json.Node(Json.NodeType.OBJECT);
                    node.set_object(user_object);
                    type_object.set_member(key.user, node);
                } else {
                    user_object = node.get_object();
                }

                user_object.set_int_member("ts", value);
            });

        if (error != null) {
            throw error;
        }

        if (i == 0) {
            throw new Matrix.Error.INCOMPLETE(
                    "Won't generate a m.receipt event with no receipts");
        }

        root.set_string_member("room_id", _room_id);

        base.to_json(json_data);
    }
}
