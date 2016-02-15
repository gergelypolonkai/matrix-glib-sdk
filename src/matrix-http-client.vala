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
 * An event-driven client class to communicate with HTTP based
 * Matrix.org servers.
 */
public class Matrix.HTTPClient : Matrix.HTTPAPI, Matrix.Client {
    private bool _polling = false;
    private ulong _event_timeout = 30000;
    private string? _last_sync_token;

    public
    HTTPClient(string base_url)
    {
        Object(base_url : base_url);
    }

    public void
    login_with_password(string username, string password)
        throws Matrix.Error
    {
        var builder = new Json.Builder();

        builder.begin_object();

        builder.set_member_name("user");
        builder.add_string_value(username);

        builder.set_member_name("password");
        builder.add_string_value(password);

        builder.end_object();

        login((i, content_type, json_content, raw_content, error) => {
                login_finished((error == null) || (error is Matrix.Error.NONE));
            },
            "m.login.password",
            builder.get_root());
    }

    public void
    register_with_password(string? username, string password)
        throws Matrix.Error
    {
        register_account(
                (i, content_type, json_content, raw_content, error) => {
                    login_finished((error is Matrix.Error.NONE));
                },
                Matrix.AccountKind.USER,
                false, username, password);
    }

    public void
    logout()
        throws Matrix.Error
    {
        token = null;
        refresh_token = null;
        abort_pending();
    }

    private void
    process_event(Json.Node event_node, string? room_id)
    {
        Json.Object root_obj;
        Json.Node node;
        string event_type;
        Matrix.Event evt;

        if (event_node.get_node_type() != Json.NodeType.OBJECT) {
            warning("Received event that is not an object.");

            return;
        }

        root_obj = event_node.get_object();

        if ((node = root_obj.get_member("type")) == null) {
            warning("Received event without type.");

            return;
        }

        event_type = node.get_string();

        try {
            evt = Matrix.Event.new_from_json(event_type, room_id, event_node);
        } catch (Matrix.Error e) {
            warning("Error during event creation: %s", e.message);

            return;
        } catch (GLib.Error e) {
            warning("Error during event creation: %s", e.message);

            return;
        }

        incoming_event(room_id, event_node, evt);
    }

    private void
    _process_event_list_obj(Json.Node node, string? room_id)
        requires(node.get_node_type() == Json.NodeType.OBJECT)
    {
        Json.Object node_obj;
        Json.Node events_node;

        node_obj = node.get_object();

        if ((events_node = node_obj.get_member("events")) != null) {
            if (events_node.get_node_type() == Json.NodeType.ARRAY) {
                events_node.get_array().foreach_element(
                        (array, idx, event_node) => {
                            process_event(event_node, room_id);
                        });
            }
        }
    }

    private void
    cb_sync(string content_type,
            Json.Node? json_content,
            ByteArray? raw_content,
            Matrix.Error? error)
    {
        if (error == null) {
            var root_obj = json_content.get_object();
            Json.Node? node;

            debug("Processing account data");
            _process_event_list_obj(root_obj.get_member("account_data"),
                                    null);

            debug("Processing presence");
            _process_event_list_obj(root_obj.get_member("presence"),
                                    null);

            if ((node = root_obj.get_member("rooms")) != null) {
                if (node.get_node_type() == Json.NodeType.OBJECT) {
                    Json.Object rooms_object = node.get_object();
                    Json.Node rooms_node;

                    debug("Processing rooms");

                    if ((rooms_node = rooms_object.get_member(
                                 "invite")) != null) {
                        rooms_node.get_object().foreach_member(
                                (obj, room_id, room_node) => {
                                    if (room_node.get_node_type() != Json.NodeType.OBJECT) {
                                        return;
                                    }

                                    _process_event_list_obj(
                                            room_node
                                                .get_object()
                                                .get_member("invite_state"),
                                            room_id);
                                });
                    }

                    if ((rooms_node = rooms_object.get_member(
                                 "join")) != null) {
                        rooms_node.get_object().foreach_member(
                                (obj, room_id, room_node) => {
                                    if (room_node.get_node_type() != Json.NodeType.OBJECT) {
                                        return;
                                    }

                                    _process_event_list_obj(
                                            room_node
                                                .get_object()
                                                .get_member("timeline"),
                                            room_id);
                                    _process_event_list_obj(
                                            room_node
                                                .get_object()
                                                .get_member("state"),
                                            room_id);
                                    _process_event_list_obj(
                                            room_node
                                                .get_object()
                                                .get_member("account_data"),
                                            room_id);
                                    _process_event_list_obj(
                                            room_node
                                                .get_object()
                                                .get_member("ephemeral"),
                                            room_id);
                                });
                    }

                    if ((rooms_node = rooms_object.get_member(
                                 "leave")) != null) {
                        rooms_node.get_object().foreach_member(
                                (obj, room_id, room_node) => {
                                    if (room_node.get_node_type() != Json.NodeType.OBJECT) {
                                        return;
                                    }

                                    _process_event_list_obj(
                                            room_node
                                                .get_object()
                                                .get_member("timeline"),
                                            room_id);
                                    _process_event_list_obj(
                                            room_node
                                                .get_object()
                                                .get_member("state"),
                                            room_id);
                                });
                    }
                }
            }

            if ((node = root_obj.get_member("next_batch")) != null) {
                _last_sync_token = node.get_string();
            }
        }

        // It is possible that polling has been disabled while we were
        // processing events. Don’t continue polling if that is the
        // case.
        if (_polling) {
            // This `500` should be changed to M_MISSING_TOKEN somehow
            try {
                if ((error == null) || (error.code < 500)) {
                    begin_polling();
                } else if ((error != null) && error.code >= 500) {
                    stop_polling(false);
                }
            } catch (Matrix.Error e) {}
        }
    }

    public void
    begin_polling()
        throws Matrix.Error
    {
        try {
            sync((API.Callback)cb_sync,
                 null, null, _last_sync_token,
                 false, false, _event_timeout);

            _polling = true;
        } catch (Matrix.Error e) {
            throw e;
        }

        _polling = true;
    }

    public void
    stop_polling(bool cancel_ongoing)
        throws Matrix.Error
    {
        _polling = false;

        if (cancel_ongoing) {
            abort_pending();
        }
    }
}
