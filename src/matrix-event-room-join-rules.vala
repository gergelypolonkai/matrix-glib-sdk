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
 * Class to hold a m.room.join_rules event.
 *
 * A room may be public meaning anyone can join the room without any
 * prior action. Alternatively, it can be invite meaning that a user
 * who wishes to join the room must first receive an invite to the
 * room from someone already inside of the room. Currently, knock and
 * private are reserved keywords which are not implemented.
 */
public class Matrix.Event.RoomJoinRules : Matrix.Event.State {
    public Matrix.JoinRules? join_rules { get; set; default = null; }

    protected override void
    from_json(Json.Node json_data)
        throws Matrix.Error
    {
        var content_root = json_data.get_object()
            .get_member("content").get_object();
        Json.Node? node;

        if ((node = content_root.get_member("join_rule")) != null) {
            Matrix.JoinRules? rules = (Matrix.JoinRules)_g_enum_nick_to_value(
                    typeof(Matrix.JoinRules), node.get_string());

            if (rules != null) {
                _join_rules = rules;
            } else {
                _join_rules = Matrix.JoinRules.UNKNOWN;

                warning("Unknown value %s in a m.room.join_rules event",
                        node.get_string());
            }
        } else {
            _join_rules = Matrix.JoinRules.UNKNOWN;

            warning("content.join_rules is missing from a m.room.join_rules event.");
        }

        base.from_json(json_data);
    }

    protected override void
    to_json(Json.Node json_data)
        throws Matrix.Error
    {
        var content_root = json_data.get_object().
            get_member("content").get_object();

        if (_join_rules == null) {
            throw new Matrix.Error.INCOMPLETE(
                    "Won't send a m.room.join_rules event without content.join_rule");
        }

        if (_join_rules == Matrix.JoinRules.UNKNOWN) {
            throw new Matrix.Error.INCOMPLETE(
                    "Won't send a m.room.join_rules event with an unknown rule");
        }

        content_root.set_string_member(
                "join_rule",
                _g_enum_value_to_nick(typeof(Matrix.JoinRules), _join_rules));

        base.to_json(json_data);
    }
}
