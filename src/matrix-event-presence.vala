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
 * Informs the client of a user's presence state change.
 */
public class Matrix.Event.Presence : Matrix.Event.Base {
    /**
     * The current avatar URL for this user, if any.
     */
    public string? avatar_url { get; set; }

    /**
     * The current display name for this user, if any.
     */
    public string? display_name { get; set; }

    /**
     * The last time since this used performed some action, in
     * milliseconds.
     *
     * This won’t get into the generated event JSON if negative.
     */
    public long last_active_ago { get; set; default = -1; }

    /**
     * The user's ID.
     */
    public string? user_id { get; set; default = null; }

    public string? event_id { get; set; default = null; }

    /**
     * The presence state for this user.
     */
    public Matrix.Presence presence {
        get; set;

        default = Matrix.Presence.UNKNOWN;
    }

    protected override void
    from_json(Json.Node json_data)
        throws Matrix.Error
    {
        var root = json_data.get_object();
        var content_root = root.get_member("content").get_object();
        Json.Node? node;

        if ((node = root.get_member("event_id")) != null) {
            _event_id = node.get_string();
        } else if (Config.DEBUG) {
            warning("event_id is missing from a m.presence event");
        }

        if ((node = content_root.get_member("user_id")) != null) {
            _user_id = node.get_string();
        } else if (Config.DEBUG) {
            warning("content.user_id is missing from the m.presence event");

            // Workaround for having sender instead of content.user_id
            // in most (room-dependent) presence events
            if ((node = root.get_member("sender")) != null) {
                _user_id = node.get_string();
            }
        }

        if ((node = content_root.get_member("last_active_ago")) != null) {
            _last_active_ago = (long)node.get_int();
        }

        if ((node = content_root.get_member("avatar_url")) != null) {
            _avatar_url = node.get_string();
        }

        if ((node = content_root.get_member("displayname")) != null) {
            _display_name = node.get_string();
        }

        if ((node = content_root.get_member("presence")) != null) {
            try {
                _presence = (Matrix.Presence)_g_enum_nick_to_value(
                        typeof(Matrix.Presence), node.get_string());
            } catch (Matrix.Error e) {
                _presence = Matrix.Presence.UNKNOWN;

                if (Config.DEBUG) {
                    warning("Unknown value %s for content.presence in a m.presence event",
                            node.get_string());
                }
            }
        } else if (Config.DEBUG) {
            warning("content.presence is missing from the m.presence event");
        }

        base.from_json(json_data);
    }

    protected override void
    to_json(Json.Node json_data)
        throws Matrix.Error
    {
        if (_presence == Matrix.Presence.UNKNOWN) {
            throw new Matrix.Error.UNKNOWN_VALUE(
                    "Won't generate a m.presence event with an unkwnown presence");
        }

        if (_user_id == null) {
            throw new Matrix.Error.INCOMPLETE(
                    "Won't generate a m.presence event without sender");
        }

        if (_event_id == null) {
            throw new Matrix.Error.INCOMPLETE(
                    "Won't generate a m.presence event without event_id");
        }

        var root = json_data.get_object();
        var content_root = root.get_member("content").get_object();

        root.set_string_member("event_id", _event_id);

        content_root.set_string_member("user_id", _user_id);
        content_root.set_string_member("presence",
                                       _g_enum_value_to_nick(typeof(Presence),
                                                             _presence));

        if (last_active_ago >= 0) {
            content_root.set_int_member("last_active_ago", last_active_ago);
        }

        if (avatar_url != null) {
            content_root.set_string_member("avatar_url", avatar_url);
        }

        if (display_name != null) {
            content_root.set_string_member("displayname", display_name);
        }

        base.to_json(json_data);
    }
}
