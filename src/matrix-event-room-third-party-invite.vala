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
 * Class to hold a m.room.third_party_invite event
 *
 * Acts as an m.room.member invite event, where there isn't a target
 * user_id to invite. This event contains a token and a public key
 * whose private key must be used to sign the token. Any user who can
 * present that signature may use this invitation to join the target
 * room.
 */
public class Matrix.Event.RoomThirdPartyInvite : Matrix.Event.State {
    public struct PublicKey {
        string? key;
        string? validity_url;
    }

    /**
     * A user-readable string which represents the user who has been
     * invited. This should not contain the user's third party ID, as
     * otherwise when the invite is accepted it would leak the
     * association between the matrix ID and the third party ID.
     */
    public string? display_name { get; set; default = null; }

    /**
     * A URL which can be fetched, with querystring
     * public_key=public_key, to validate whether the key has been
     * revoked. The URL must return a JSON object containing a boolean
     * property named valid.
     */
    public string? key_validity_url { get; set; default = null; }

    /**
     * A base64-encoded ed25519 key with which token must be signed
     * (though a signature from any entry in public_keys is also
     * sufficient). This exists for backwards compatibility.
     */
    public string? public_key { get; set; default = null;}

    /**
     * Keys with which the token may be signed.
     */
    public PublicKey[] public_keys { get; set; }

    /**
     * The token, of which a signature must be produced in order to
     * join the room.
     */
    public string? token {
         get {
             return _state_key;
         }
         set {
             _state_key = value;
         }

         default = null;
     }

    protected override void
    from_json(Json.Node json_data)
        throws Matrix.Error
    {
        var root = json_data.get_object();
        var content_root = root.get_member("content").get_object();
        Json.Node? node;

        if ((node = root.get_member("state_key")) != null) {
            _state_key = node.get_string();
        } else if (Config.DEBUG) {
            warning("state_key is missing from a m.room.third_party_invite_event");
        }

        if ((node = content_root.get_member("display_name")) != null) {
            _display_name = node.get_string();
        } else {
            warning("content.display_name is missing from a m.room.third_party_invite event");
        }

        if ((node = content_root.get_member("public_key")) != null) {
            _public_key = node.get_string();
        } else {
            warning("content.public_key is missing from a m.room.third_party_invite event");
        }

        if ((node = content_root.get_member("key_validity_url")) != null) {
            _key_validity_url = node.get_string();
        } else {
            warning("content.key_validity_url is missing from a m.room.third_party_invite event");
        }

        if ((node = content_root.get_member("public_keys")) != null) {
        }

        base.from_json(json_data);
    }

    protected override void
    to_json(Json.Node json_data)
        throws Matrix.Error
    {
        if (_display_name == null) {
            throw new Matrix.Error.INCOMPLETE(
                    "Won't generate a m.room.third_party_invite event without display_name");
        }

        if (_public_key == null) {
            throw new Matrix.Error.INCOMPLETE(
                    "Won't generate a m.room.third_party_invite without public_key");
        }

        if (_key_validity_url == null) {
            throw new Matrix.Error.INCOMPLETE(
                    "Won't generate a m.room.third_party_invite without key_validity_url");
        }

        if (_state_key == null) {
            throw new Matrix.Error.INCOMPLETE(
                    "Won't generate a m.room.third_party_invite without token");
        }

        var content_root = json_data.get_object()
            .get_member("content").get_object();

        // We don't need to set the state key here, our base class
        // will do it for us

        content_root.set_string_member("display_name", _display_name);
        content_root.set_string_member("public_key", _public_key);
        content_root.set_string_member("key_validity_url", _key_validity_url);

        var key_list = new Json.Array();

        foreach (var entry in _public_keys) {
            if (entry.key == null) {
                throw new Matrix.Error.INCOMPLETE(
                        "Won't generate a m.room.third_party_invite with a missing key under public_keys");
            }

            var key_obj = new Json.Object();
            var key_node = new Json.Node(Json.NodeType.OBJECT);
            key_node.set_object(key_obj);
            key_obj.set_string_member("public_key", entry.key);

            if (entry.validity_url != null) {
                key_obj.set_string_member("key_validity_url", entry.validity_url);
            }

            key_list.add_element(key_node);
        }

        if (key_list.get_length() > 0) {
            var keys_node = new Json.Node(Json.NodeType.ARRAY);
            keys_node.set_array(key_list);
            content_root.set_member("public_keys", keys_node);
        }

        base.to_json(json_data);
    }
}
