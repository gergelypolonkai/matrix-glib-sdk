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
 * Class to hold data about rooms
 *
 * The Room class is there to hold different parameters of a room like
 * its known aliases, its members, etc.
 */
public class Matrix.Room : GLib.Object {
    public string room_id { get; construct; }

    public string[] aliases { get; set; }

    public string? avatar_url { get; set; default = null; }

    public ImageInfo? avatar_info { get; set; default = null; }

    public string? avatar_thumbnail_url { get; set; default = null; }

    public ImageInfo? avatar_thumbnail_info { get; set; default = null; }

    public string? canonical_alias { get; set; default = null; }

    public string? creator { get; set; default = null; }

    public bool federate { get; set; default = false; }

    public GuestAccess guest_access { get; set; default = GuestAccess.UNKNOWN; }

    public HistoryVisibility history_visibility {
        get; set;
        default = HistoryVisibility.UNKNOWN;
    }

    public JoinRules join_rules { get; set; default = JoinRules.UNKNOWN; }

    public string? name { get; set; default = null; }

    public int default_power_level { get; set; default = 0; }

    public int default_event_level { get; set; default = 0; }

    public int default_state_level { get; set; default = 10; }

    public int ban_level { get; set; default = 5; }

    public int kick_level { get; set; default = 5; }

    public int redact_level { get; set; default = 20; }

    public int invite_level { get; set; default = 0; }

    public string? topic { get; set; default = null; }

    public string[] typing_users { get; set; }

    private Gee.HashMap<string, int?> _event_levels =
        new Gee.HashMap<string, int?>();

    private Gee.HashMap<string, int?> _user_levels =
        new Gee.HashMap<string, int?>();

    private struct MemberData {
        Profile profile;
        bool thirdparty;
    }

    private Gee.HashMap<string, MemberData?> _members =
        new Gee.HashMap<string, MemberData?>();

    public
    Room(string room_id)
    {
        Object(room_id : room_id);
    }

    public void
    add_member(string user_id, Profile? profile, bool third_party)
        throws Matrix.Error
    {
        MemberData? data = null;

        if ((data = _members[user_id]) != null) {
            throw new Matrix.Error.ALREADY_EXISTS(
                    "User already exists in that room");
        }

        data = MemberData();

        if (profile == null) {
            data.profile = new Profile();
        } else {
            data.profile = profile;
        }

        data.thirdparty = third_party;

        _members[user_id] = data;
    }

    public Profile
    get_or_add_member(string user_id, bool third_party = false)
        throws Matrix.Error
    {
        try {
            return get_member(user_id, null);
        } catch (Matrix.Error e) {
            add_member(user_id, null, third_party);

            return get_member(user_id, null);
        }
    }

    public Profile
    get_member(string user_id, out bool third_party)
        throws Matrix.Error
    {
        MemberData? data = null;

        if ((data = _members[user_id]) == null) {
            throw new Matrix.Error.NOT_FOUND(
                    "No such room member");
        }

        third_party = data.thirdparty;

        return data.profile;
    }

    public void
    remove_member(string user_id)
        throws Matrix.Error
    {
        MemberData? data = null;

        if ((data = _members[user_id]) == null) {
            throw new Matrix.Error.NOT_FOUND(
                    "No such room member");
        }

        _members.unset(user_id);
    }

    /**
     * Clear the stored individual user levels. This should be called
     * e.g. when receiving a new m.room.power_levels event.
     */
    public void
    clear_user_levels()
    {
        _user_levels.clear();
    }

    /**
     * Set an individual power level for a user.
     *
     * @param user_id a fully qualified Matrix ID
     * @param level the new power level
     */
    public void
    set_user_level(string user_id, int level)
    {
        _user_levels[user_id] = level;
    }

    /**
     * Get the power level of a user.
     *
     * @param user_id a fully qualified Matrix ID
     * @return the level of the user. If the user doesn’t have an
     *         individually set power level, the default value is
     *         returned
     */
    public int
    get_user_level(string user_id)
    {
        int? level = _user_levels[user_id];

        if (level == null) {
            level = _default_power_level;
        }

        return level;
    }

    /**
     * Clear the stored event level requirements. This should be
     * called e.g. when receiving a new m.room.power_levels event.
     */
    public void
    clear_event_levels()
    {
        _event_levels.clear();
    }

    /**
     * Set the required level to send an event of
     * type @param event_type.
     *
     * @param event_type the event type to restrict
     * @param level the desired level for the event type
     */
    public void
    set_event_level(string event_type, int level)
    {
        _event_levels[event_type] = level;
    }

    /**
     * Get the required level to send an event of
     * type @param event_type.
     *
     * @param event_type the event type to query
     * @return the level required to send a specific event. If there
     *         is no level requirement is set for this event type,
     *         this function returns null as there is no way to decide
     *         if {@link Matrix.Room.default_state_level} or
     *         {@link Matrix.Room.default_event_level} should be used
     */
    public int?
    get_event_level(string event_type)
    {
        return _event_levels[event_type];
    }
}
