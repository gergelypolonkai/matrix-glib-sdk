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
 * Class to handle m.room.power_levels event
 *
 * This event specifies the minimum level a user must have in order to
 * perform a certain action. It also specifies the levels of each user
 * in the room. If a user_id is in the users list, then that user_id
 * has the associated power level. Otherwise they have the default
 * level users_default. If users_default is not supplied, it is
 * assumed to be 0. The level required to send a certain event is
 * governed by events, state_default and events_default. If an event
 * type is specified in events, then the user must have at least the
 * level specified in order to send that event. If the event type is
 * not supplied, it defaults to events_default for Message Events and
 * state_default for State Events.
 */
public class Matrix.Event.RoomPowerLevels : Matrix.Event.State {
    /**
     * The default power level for every user in the room, unless
     * their user_id is mentioned in the users key.
     */
    public int users_default { get; set; default = 0; }

    /**
     * The default level required to send message events. Can be
     * overridden by the events key.
     */
    public int events_default { get; set; default = 0; }

    /**
     * The default level required to send state events. Can be
     * overridden by the events key.
     */
    public int state_default { get; set; default = 10; }

    /**
     * The level required to ban a user.
     */
    public int ban { get; set; default = 5; }

    /**
     * The level required to kick a user.
     */
    public int kick { get; set; default = 5; }

    /**
     * The level required to redact an event.
     */
    public int redact { get; set; default = 20; }

    /**
     * The level required to invite someone.
     */
    public int invite { get; set; default = 0; }

    /**
     * A hash map to store the required level to send specific events.
     */
    public HashTable<string, int?> event_levels {
        get {
            return _event_levels;
        }
    }

    /**
     * A hash map to store current level for individual users.
     */
    public HashTable<string, int?> user_levels {
        get {
            return _user_levels;
        }
    }

    private HashTable<string, int?> _event_levels =
        new HashTable<string, int?>(str_hash, str_equal);

    private HashTable<string, int?> _user_levels =
        new HashTable<string, int?>(str_hash, str_equal);

    protected override void
    from_json(Json.Node json_data)
        throws Matrix.Error
    {
        var root = json_data.get_object();
        var content_root = root.get_member("content").get_object();
        Json.Node? node = null;

        if (Config.DEBUG && ((node = root.get_member("content")) != null)) {
            if (node.get_string() != "") {
                warning("state_key of a m.room.power_levels event is non-empty");
            }
        }

        if ((node = content_root.get_member("ban")) != null) {
            _ban = (int)node.get_int();
        } else {
            warning("content.ban is missing from a m.room.power_levels event");
        }

        if ((node = content_root.get_member("kick")) != null) {
            _kick = (int)node.get_int();
        } else {
            warning("content.kick is missing from a m.room.power_levels event");
        }

        if ((node = content_root.get_member("redact")) != null) {
            _redact = (int)node.get_int();
        } else {
            warning("content.redact is missing from a m.room.power_levels event");
        }

        if ((node = content_root.get_member("events_default")) != null) {
            _events_default = (int)node.get_int();
        } else {
            warning("content.events_default is missing from a m.room.power_levels event");
        }

        if ((node = content_root.get_member("state_default")) != null) {
            _state_default = (int)node.get_int();
        } else {
            warning("content.state_default is missing from a m.room.power_levels event");
        }

        if ((node = content_root.get_member("users_default")) != null) {
            _users_default = (int)node.get_int();
        }

        if ((node = content_root.get_member("invite")) != null) {
            _invite = (int)node.get_int();
        }

        if ((node = content_root.get_member("events")) != null) {
            _event_levels.remove_all();

            node.get_object().foreach_member((obj, event_name, event_node) => {
                    _event_levels[event_name] = (int)event_node.get_int();
                });
        }

        if ((node = content_root.get_member("users")) != null) {
            _user_levels.remove_all();

            node.get_object().foreach_member((obj, user_id, user_node) => {
                    _user_levels[user_id] = (int)user_node.get_int();
                });
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
                    "Won't generate a m.room.power_levels event with a non-empty state_key");
        }

        if (_user_levels == null) {
            throw new Matrix.Error.INCOMPLETE(
                    "Won't create an m.room.power_levels event without a content.users key");
        }

        if (_event_levels == null) {
            throw new Matrix.Error.INCOMPLETE(
                    "Won't create an m.room.power_levels event without a content.events key");
        }

        content_root.set_int_member("ban", _ban);
        content_root.set_int_member("kick", _kick);
        content_root.set_int_member("redact", _redact);
        content_root.set_int_member("state_default", _state_default);
        content_root.set_int_member("events_default", _events_default);

        var user_obj = new Json.Object();
        var user_node = new Json.Node(Json.NodeType.OBJECT);
        user_node.set_object(user_obj);

        _user_levels.foreach(
            (key, value) => {
                user_obj.set_int_member(key, value);
            });

        content_root.set_member("users", user_node);

        var events_obj = new Json.Object();
        var events_node = new Json.Node(Json.NodeType.OBJECT);
        events_node.set_object(events_obj);

        _event_levels.foreach(
            (key, value) => {
                events_obj.set_int_member(key, value);
            });

        content_root.set_member("users", events_node);

        base.to_json(json_data);
    }

    public void
    set_user_level(string user_id, int level)
    {
        _user_levels[user_id] = level;
    }

    public void
    set_event_level(string event_type, int level)
    {
        _event_levels[event_type] = level;
    }
}
