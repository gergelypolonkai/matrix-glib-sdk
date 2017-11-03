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
 * Class to hold a m.room.guest_access event
 *
 * This event controls whether guest users are allowed to join
 * rooms. If this event is absent, servers should act as if it is
 * present and has the guest_access value `forbidden`.
 */

/**
 * Class to hold a m.room.guest_access event
 */
public class Matrix.Event.RoomGuestAccess : Matrix.Event.State {
    /**
     * Whether guests can join the room.
     */
    public GuestAccess guest_access { get; set; default = GuestAccess.UNKNOWN; }

    protected override void
    from_json(Json.Node json_data)
        throws Matrix.Error
    {
        var root = json_data.get_object();
        var content_root = root.get_member("content").get_object();
        Json.Node? node = null;

        if (Config.DEBUG && ((node = root.get_member("state_key")) != null)) {
            if (node.get_string() != "") {
                warning("state_key of a m.room.guest_access is non-empty");
            }
        }

        if ((node = content_root.get_member("guest_access")) != null) {
            GuestAccess? rules = (GuestAccess?)_g_enum_nick_to_value(
                    typeof(GuestAccess), node.get_string());

            if (rules != null) {
                _guest_access = rules;
            } else {
                _guest_access = GuestAccess.UNKNOWN;

                if (Config.DEBUG) {
                    warning("Unknown value '%s' in a m.room.guest_access event",
                            node.get_string());
                }
            }
        } else {
            warning("content.guest_access is missing from a m.room.guest_access event");
        }

        base.from_json(json_data);
    }

    protected override void
    to_json(Json.Node json_data)
        throws Matrix.Error
    {
        if (_state_key != "") {
            throw new Matrix.Error.INCOMPLETE(
                    "Won't generate a m.room.guest_access event with a non-empty state key");
        }

        if ((_guest_access == GuestAccess.UNKNOWN)) {
            throw new Matrix.Error.INCOMPLETE(
                    "Won't generate a m.room.guest_access event with an unknown content.guest_access key");
        }

        var content_root = json_data.get_object()
            .get_member("content").get_object();

        content_root.set_string_member(
                "guest_access",
                _g_enum_value_to_nick(typeof(GuestAccess), _guest_access));

        base.to_json(json_data);
    }
}
