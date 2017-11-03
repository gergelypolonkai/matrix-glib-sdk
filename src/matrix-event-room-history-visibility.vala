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
 * Class to hold a m.room.history_visibility event.
 *
 * This event controls whether a user can see the events that happened
 * in a room from before they joined.
 */
public class Matrix.Event.RoomHistoryVisibility : Matrix.Event.State {
    /**
     * Who can see the room history.
     */
    public Matrix.HistoryVisibility visibility {
        get; set;

        default = Matrix.HistoryVisibility.UNKNOWN;
    }

    protected override void
    from_json(Json.Node json_data)
        throws Matrix.Error
    {
        var root = json_data.get_object();
        var content_root = root.get_member("content").get_object();
        Json.Node? node = null;

        if (Config.DEBUG && ((node = root.get_member("state_key")) != null)) {
            if (node.get_string() != "") {
                warning("state_key of a m.room.history_visibility event is non-empty");
            }
        }

        if ((node = content_root.get_member("history_visibility")) != null) {
            Matrix.HistoryVisibility? vis = (Matrix.HistoryVisibility?)_g_enum_nick_to_value(
                    typeof(Matrix.HistoryVisibility),
                    node.get_string());

            if (vis != null) {
                _visibility = vis;
            } else {
                _visibility = Matrix.HistoryVisibility.UNKNOWN;

                if (Config.DEBUG) {
                    warning("Unknown history_visibility value %s", node.get_string());
                }
            }
        }

        base.from_json(json_data);
    }

    protected override void
    to_json(Json.Node json_data)
        throws Matrix.Error
    {
        var content_root = json_data.get_object()
            .get_member("content").get_object();

        if (_state_key != "") {
            throw new Matrix.Error.INCOMPLETE(
                    "Won't generate a m.room.history_visibility event with a non-empty state key");
        }

        if (visibility == Matrix.HistoryVisibility.UNKNOWN) {
            throw new Matrix.Error.UNKNOWN_VALUE(
                    "Won't send m.room.history_visibility event with unknown visibility value");
        }

        content_root.set_string_member("history_visibility",
                                       _g_enum_value_to_nick(
                                               typeof(Matrix.HistoryVisibility),
                                               visibility));

        base.to_json(json_data);
    }
}
