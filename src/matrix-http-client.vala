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
    private Gee.HashMap<string, Profile> _user_global_profiles =
        new Gee.HashMap<string, Profile>();
    private Gee.HashMap<string, Presence> _user_global_presence =
        new Gee.HashMap<string, Presence>();

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
        Matrix.Event.Base? evt = null;

        if (event_node.get_node_type() != Json.NodeType.OBJECT) {
            if (Config.DEBUG) {
                warning("Received event that is not an object.");
            }

            return;
        }

        root_obj = event_node.get_object();

        if ((node = root_obj.get_member("type")) == null) {
            if (Config.DEBUG) {
                warning("Received event without type.");
            }

            return;
        }

        event_type = node.get_string();

        try {
            evt = Matrix.Event.Base.new_from_json(event_type, event_node);
        } catch (GLib.Error e) {
            evt = null;
        }

        if (evt != null) {
            string? user_id = null;

            if (evt.get_type().is_a(typeof(Matrix.Event.Presence))) {
                var pevt = (Matrix.Event.Presence)evt;
                user_id = pevt.user_id;

                _user_global_presence[user_id] = pevt.presence;

                Profile? profile = _user_global_profiles[user_id];

                if (profile == null) {
                    profile = new Profile();
                    _user_global_profiles[user_id] = profile;
                }

                profile.avatar_url = pevt.avatar_url;
                profile.display_name = pevt.display_name;
            } else if (evt.get_type().is_a(typeof(Matrix.Event.RoomMember))) {
                // The following is a temporary hack until per-room
                // profiles get implemented in HSes.
                var mevt = (Matrix.Event.RoomMember)evt;
                user_id = mevt.user_id;

                Profile? profile = _user_global_profiles[user_id];

                if (profile == null) {
                    profile = new Profile();
                    _user_global_profiles[user_id] = profile;
                }

                profile.avatar_url = mevt.avatar_url;
                profile.display_name = mevt.display_name;
            }
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

            if (Config.DEBUG) {
                debug("Processing account data");
            }

            _process_event_list_obj(root_obj.get_member("account_data"),
                                    null);

            if (Config.DEBUG) {
                debug("Processing presence");
            }

            _process_event_list_obj(root_obj.get_member("presence"),
                                    null);

            if ((node = root_obj.get_member("rooms")) != null) {
                if (node.get_node_type() == Json.NodeType.OBJECT) {
                    Json.Object rooms_object = node.get_object();
                    Json.Node rooms_node;

                    if (Config.DEBUG) {
                        debug("Processing rooms");
                    }

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

    public Profile?
    get_user_profile(string user_id, string? room_id = null)
        throws Matrix.Error
    {
        if (room_id == null) {
            var profile = _user_global_profiles[user_id];

            if (profile == null) {
                throw new Matrix.Error.UNAVAILABLE(
                        "Global profile for %s is not cached yet.",
                        user_id);
            }

            return profile;
        }

        throw new Matrix.Error.UNSUPPORTED(
                "Per-room profiles are not supported yet.");
    }

    public Presence?
    get_user_presence(string user_id, string? room_id = null)
        throws Matrix.Error
    {
        if (room_id == null) {
            Presence? presence = _user_global_presence[user_id];

            if (presence == null) {
                throw new Matrix.Error.UNAVAILABLE(
                        "Global presence for %s is not cached yet.",
                        user_id);
            }

            return presence;
        }

        throw new Matrix.Error.UNSUPPORTED(
                "Per-room presences are not supported yet.");
    }
}
