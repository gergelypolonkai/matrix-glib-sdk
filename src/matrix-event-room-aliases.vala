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
 * Class to hold a m.room.aliases event.
 *
 * This event is sent by a homeserver directly to inform of changes to
 * the list of aliases it knows about for that room.
 *
 * The state_key for this event is set to the homeserver which owns
 * the room alias.
 *
 * The entire set of known aliases for the room is the union of all
 * the m.room.aliases events, one for each homeserver. Clients should
 * check the validity of any room alias given in this list before
 * presenting it to the user as trusted fact. The lists given by this
 * event should be considered simply as advice on which aliases might
 * exist, for which the client can perform the lookup to confirm
 * whether it receives the correct room ID.
 */
public class Matrix.Event.RoomAliases : Matrix.Event.State {
    private List<string>? _aliases = null;

    /**
     * A list of room aliases.
     */
    public List<string>? aliases {
        get {
            return _aliases;
        }

        set {
            _aliases = value.copy();
        }

        default = null;
    }

    protected override void
    from_json(Json.Node json_data)
        throws Matrix.Error
    {
        var content_root = json_data.get_object()
            .get_member("content").get_object();
        Json.Node? node;

        if ((node = content_root.get_member("aliases")) != null) {
            _aliases = null;

            node.get_array().foreach_element((ary, idx, member_node) => {
                    _aliases.prepend(member_node.get_string());
                });
        } else if (Config.DEBUG) {
            warning("content.aliases is missing from a m.room.aliases event");
        }

        base.from_json(json_data);
    }

    protected override void
    to_json(Json.Node json_data)
        throws Matrix.Error
    {
        if ((_aliases == null) || (_aliases.length() == 0)) {
            throw new Matrix.Error.INCOMPLETE(
                    "Won't generate a m.room.aliases event without aliases");
        }

        var content_root = json_data.get_object()
            .get_member("content").get_object();

        var aliases_ary = new Json.Array();

        foreach (var entry in _aliases) {
            aliases_ary.add_string_element(entry);
        }

        var aliases_node = new Json.Node(Json.NodeType.ARRAY);
        aliases_node.set_array(aliases_ary);
        content_root.set_member("aliases", aliases_node);

        base.to_json(json_data);
    }
}
