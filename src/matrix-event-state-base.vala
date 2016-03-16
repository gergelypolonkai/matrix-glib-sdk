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

public abstract class Matrix.Event.State : Matrix.Event.Room {
    protected string? _state_key;

    public string? state_key {
        get {
            return _state_key;
        }

        set {
            _state_key = value;
        }

        default = null;
    }
    public Json.Node? prev_content { get; set; default = null; }

    protected override void
    from_json(Json.Node json_data)
        throws Matrix.Error
    {
        var root = json_data.get_object();
        Json.Node? node;

        if ((node = root.get_member("state_key")) != null) {
            _state_key = node.get_string();
        } else if (Config.DEBUG) {
            warning("state_key is not present in a State event");
        }

        if ((node = root.get_member("prev_content")) != null) {
            _prev_content = node;
        }

        base.from_json(json_data);
    }

    protected override void
    to_json(Json.Node json_node)
        throws Matrix.Error
    {
        if (_state_key == null) {
            throw new Matrix.Error.INCOMPLETE(
                    "Won't generate state events without state_key");
        }

        var root = json_node.get_object();

        root.set_string_member("state_key", state_key);

        if (_prev_content != null) {
            root.set_member("prev_content", prev_content);
        }

        base.to_json(json_node);
    }

    /**
     * Get a stripped state event.
     *
     * @return `null` if the event is not allowed to be stripped, or
     *         the full JSON node otherwise
     */
    public Json.Node?
    get_stripped_node()
    {
        if ((_event_type != "m.room.join_rules")
            && (_event_type != "m.room.canonical_alias")
            && (_event_type != "m.room.avatar")
            && (_event_type != "m.room.name")) {
            warning("Trying to strip down event that is not allowed to be stripped.");

            return null;
        }

        return json;
    }
}
