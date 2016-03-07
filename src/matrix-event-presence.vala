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
 * Class for representing presence events
 *
 * The presence event class.
 */
public class Matrix.Event.Presence : Matrix.Event.Base {
    public string? avatar_url { get; set; }
    public string? display_name { get; set; }
    public ulong? last_active_ago { get; set; }
    public string? user_id { get; set; default = null; }
    public Matrix.Presence presence {
        get;
        set;
        default = Matrix.Presence.UNKNOWN;
    }

    protected override void
    from_json(Json.Node json_data)
        throws Matrix.Error
    {
        Json.Object content_root = json_data.get_object().get_member("content").get_object();
        Json.Node? node;

        if ((node = content_root.get_member("user_id")) != null) {
            _user_id = node.get_string();
        } else if (Config.DEBUG) {
            warning("user_id is missing from the m.presence event");
        }

        if ((node = content_root.get_member("last_active_ago")) != null) {
            _last_active_ago = (ulong)node.get_int();
        } else {
            _last_active_ago = null;
        }

        if ((node = content_root.get_member("avatar_url")) != null) {
            _avatar_url = node.get_string();
        }

        if ((node = content_root.get_member("displayname")) != null) {
            _display_name = node.get_string();
        }

        if ((node = content_root.get_member("presence")) != null) {
            Matrix.Presence? pres = (Matrix.Presence?)_g_enum_nick_to_value(
                    typeof(Matrix.Presence), node.get_string());

            if (pres != null) {
                _presence = pres;
            } else {
                _presence = Matrix.Presence.UNKNOWN;
            }
        } else if (Config.DEBUG) {
            warning("presence is missing from the m.presence event");
        }

        base.from_json(json_data);
    }

    protected override void
    to_json(Json.Node json_data)
        throws Matrix.Error
    {
        Json.Object content_root;
        string? pres;

        if (presence == Matrix.Presence.UNKNOWN) {
            throw new Matrix.Error.UNKNOWN_VALUE (
                    "unkwnown presence cannot be added to a presence event");
        }

        if (_user_id == null) {
            throw new Matrix.Error.INCOMPLETE (
                    "sender must be set for presence events!");
        }

        content_root = json_data
            .get_object()
            .get_member("content")
            .get_object();

        content_root.set_string_member("user_id", _user_id);

        if (last_active_ago != null) {
            content_root.set_int_member("last_active_ago", last_active_ago);
        }

        if (avatar_url != null) {
            content_root.set_string_member("avatar_url", avatar_url);
        }

        if (display_name != null) {
            content_root.set_string_member("displayname", display_name);
        }

        pres = _g_enum_value_to_nick(typeof(Matrix.Presence), presence);

        if (pres != null) {
            content_root.set_string_member("presence", pres);
        }

        base.to_json(json_data);
    }
}
