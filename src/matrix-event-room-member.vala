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
 * Class for representing a room membership events
 *
 * Adjusts the membership state for a user in a room. It is preferable
 * to use the membership APIs (`/rooms/<room id>/invite` etc) when
 * performing membership actions rather than adjusting the state
 * directly as there are a restricted set of valid
 * transformations. For example, user A cannot force user B to join a
 * room, and trying to force this state change directly will
 * fail.
 *
 * The following membership states are specified:
 *
 * - invite - The user has been invited to join a room, but has not
 * yet joined it. They may not participate in the room until they
 * join.
 *
 * - join - The user has joined the room (possibly after accepting an
 * invite), and may participate in it.
 *
 * - leave - The user was once joined to the room, but has since left
 * (possibly by choice, or possibly by being kicked).
 *
 * - ban - The user has been banned from the room, and is no longer
 * allowed to join it until they are un-banned from the room (by
 * having their membership state set to a value other than ban).
 *
 * - knock - This is a reserved word, which currently has no meaning.
 *
 * The third_party_invite property will be set if this invite is an
 * invite event and is the successor of an m.room.third_party_invite
 * event, and absent otherwise.
 *
 * This event may also include an invite_room_state key outside the
 * content key. If present, this contains an array of stripped state
 * events. These events provide information on a few select state
 * events such as the room name.
 */
public class Matrix.Event.RoomMember : Matrix.Event.State {
    private List<Matrix.Event.State>? _invite_room_state = null;

    /**
     * The membership state of the user.
     */
    public RoomMembership membership {
        get;

        set;

        default = RoomMembership.UNKNOWN;
    }

    /**
     * The avatar URL for this user, if any. This is added by the
     * homeserver.
     */
    public string? avatar_url { get; set; default = null; }

    /**
     * The display name for this user, if any. This is added by the
     * homeserver.
     */
    public string? display_name { get; set; default = null; }

    /**
     * A name which can be displayed to represent the user instead of
     * their third party identifier
     */
    public string? tpi_display_name { get; set; default = null; }

    /**
     * The invited matrix user ID. Must be equal to the user_id
     * property of the event.
     */
    public string? tpi_signed_mxid { get; set; default = null; }

    /**
     * The token property of the containing third_party_invite object.
     */
    public string? tpi_signed_token { get; set; default = null; }

    /**
     * A single signature from the verifying server, in the format
     * specified by the Signing Events section of the server-server
     * API.
     */
    public Json.Node? tpi_signature { get; set; default = null; }

    /**
     * A subset of the state of the room at the time of the invite, if
     * membership is invite.
     */
    public List<Matrix.Event.State>? invite_room_state {
        get {
            return _invite_room_state;
        }

        set {
            _invite_room_state = value.copy();
        }

        default = null;
    }

    /**
     * The user ID whom this event relates to.
     */
    public string? user_id { get; set; default = null; }

    protected override void
    from_json(Json.Node json_data)
        throws Matrix.Error
    {
        Json.Object root = json_data.get_object();
        Json.Object content_root = root.get_member("content").get_object();
        Json.Node? node;

        // Even though the state_key is handled by the parent class,
        // in this event type this actually means the sender
        if ((node = root.get_member("state_key")) != null) {
            _user_id = node.get_string();
        } else {
            warning("state_key (thus, a user ID) is missing from a m.room.member event");
        }

        if ((node = content_root.get_member("membership")) != null) {
            Matrix.RoomMembership? mship = (Matrix.RoomMembership?)_g_enum_nick_to_value(
                    typeof(Matrix.RoomMembership),
                    node.get_string());

            if (mship != null) {
                _membership = mship;
            }
        } else if (Config.DEBUG) {
            warning("membership key is missing from the m.room.member event");
        }

        if ((node = content_root.get_member("avatar_url")) != null) {
            _avatar_url = node.get_string();
        }

        if ((node = content_root.get_member("displayname")) != null) {
            _display_name = node.get_string();
        }

        if ((node = content_root.get_member("third_party_invite")) != null) {
            var tpi_root = node.get_object();

            if ((node = tpi_root.get_member("display_name")) != null) {
                _tpi_display_name = node.get_string();
            } else {
                warning("content.third_party_invite.display_name is missing from a m.room.member event");
            }

            if ((node = tpi_root.get_member("signed")) != null) {
                var signed_root = node.get_object();

                if ((node = signed_root.get_member("mxid")) != null) {
                    tpi_signed_mxid = node.get_string();
                } else {
                    warning("content.third_party_invit.signed.mxid is missing from a m.room.member event");
                }

                if ((node = signed_root.get_member("token")) != null) {
                    tpi_signed_token = node.get_string();
                } else {
                    warning("content.third_party_invite.signed.token is missing from a m.room.member event");
                }

                if ((node = signed_root.get_member("signatures")) != null) {
                    tpi_signature = node;
                } else {
                    warning("content.third_party_invite.signed.signatures is missing from a m.room.member event");
                }
            } else {
                warning("content.third_party_invite.signed is missing from a m.room.member event");
            }
        }

        if ((node = root.get_member("invite_room_state")) != null) {
            var events = node.get_array();

            if (events.get_length() > 0) {
                _invite_room_state = null;

                events.foreach_element((ary, idx, member_node) => {
                        try {
                            var evt = Matrix.Event.Base.new_from_json(
                                    null, member_node);

                            _invite_room_state.prepend((Matrix.Event.State)evt);
                        } catch (GLib.Error e) {}
                    });
            }
        }

        // Chain up
        base.from_json(json_data);
    }

    protected override void
    to_json(Json.Node json_data)
        throws Matrix.Error
    {
        Json.Object root, content_root;
        string? mship;

        root = json_data.get_object();
        content_root = _json_object_node_ensure_field(json_data,
                                                      "content",
                                                      Json.NodeType.OBJECT)
            .get_object();

        // I’m not sure if it is a good idea to set it here.
        if (user_id != null) {
            root.set_string_member("state_key", user_id);
        }

        if (membership == RoomMembership.UNKNOWN) {
            throw new Matrix.Error.UNKNOWN_VALUE(
                    "Unknown membership value cannot be added to a room member event");
        }

        mship = _g_enum_value_to_nick(typeof(Matrix.RoomMembership),
                                      membership);

        if (mship != null) {
            content_root.set_string_member("membership", mship);
        }

        if (avatar_url != null) {
            content_root.set_string_member("avatar_url", avatar_url);
        }

        if (display_name != null) {
            content_root.set_string_member("displayname", display_name);
        }

        var tpi_root = new Json.Object();

        if (_tpi_display_name != null) {
            tpi_root.set_string_member("display_name", tpi_display_name);
        }

        var tpi_signed_root = new Json.Object();

        if (_tpi_signed_mxid != null) {
            tpi_signed_root.set_string_member("mxid", tpi_signed_mxid);
        }

        if (_tpi_signed_token != null) {
            tpi_signed_root.set_string_member("token", tpi_signed_token);
        }

        if (_tpi_signature != null) {
            tpi_signed_root.set_member("signature", tpi_signature);
        }

        if ((tpi_signed_root.get_size() != 3)
            && (tpi_signed_root.get_size() != 0)) {
            warning("3rd party invite data is not filled; ignoring");

            tpi_signed_root = null;
        }

        if ((_tpi_display_name != null) && (tpi_signed_root.get_size() == 3)) {
            var tpi_signed_node = new Json.Node(Json.NodeType.OBJECT);
            tpi_signed_node.set_object(tpi_signed_root);
            tpi_root.set_member("signed", tpi_signed_node);
        }

        if (tpi_root.get_size() == 2) {
            var tpi_node = new Json.Node(Json.NodeType.OBJECT);
            tpi_node.set_object(tpi_root);
            content_root.set_member("third_party_invite", tpi_node);
        } else if (tpi_root.get_size() != 0) {
            warning("3rd party invite data is incomplete; ignoring");
        }

        if (_invite_room_state != null) {
            var state_ary = new Json.Array();

            foreach (var entry in _invite_room_state) {
                var state_node = entry.get_stripped_node();

                if (state_node != null) {
                    state_ary.add_element(state_node);
                }
            }

            if (state_ary.get_length() > 0) {
                var state_node = new Json.Node(Json.NodeType.ARRAY);
                state_node.set_array(state_ary);
                root.set_member("invite_room_state", state_node);
            }
        }

        base.to_json(json_data);
    }
}
