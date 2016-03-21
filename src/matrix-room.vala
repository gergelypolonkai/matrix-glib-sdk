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
    /**
     * The ID of the room this object belongs to.
     */
    public string room_id { get; construct; }

    /**
     * All the known room aliases.
     */
    public string[] aliases { get; set; }

    /**
     * The URL of the room’s avatar.
     */
    public string? avatar_url { get; set; default = null; }

    /**
     * ImageInfo relevant to the room avatar.
     */
    public ImageInfo? avatar_info { get; set; default = null; }

    /**
     * The URL of the room avatar’s thumbnail.
     */
    public string? avatar_thumbnail_url { get; set; default = null; }

    /**
     * ImageInfo relevant to the room avatar’s thumbnail.
     */
    public ImageInfo? avatar_thumbnail_info { get; set; default = null; }

    /**
     * The canonical alias of the room.
     */
    public string? canonical_alias { get; set; default = null; }

    /**
     * The Matrix ID of the room’s creator.
     */
    public string? creator { get; set; default = null; }

    /**
     * If false, the room is not federated.
     */
    public bool federate { get; set; default = false; }

    /**
     * Wether guests are allowed to join the room.
     */
    public GuestAccess guest_access { get; set; default = GuestAccess.UNKNOWN; }

    /**
     * This value controls the visibility of the room’s history.
     */
    public HistoryVisibility history_visibility {
        get; set;
        default = HistoryVisibility.UNKNOWN;
    }

    /**
     * Controls who can join the room.
     */
    public JoinRules join_rules { get; set; default = JoinRules.UNKNOWN; }

    /**
     * The room’s name.
     */
    public string? name { get; set; default = null; }

    /**
     * The default power level users get upon joining the room.
     */
    public int default_power_level { get; set; default = 0; }

    /**
     * The power level required to send non-state events to the
     * room. This gets applied if the event type doesn’t have an
     * explicit level requirement (see set_event_level() and
     * get_event_level()).
     */
    public int default_event_level { get; set; default = 0; }

    /**
     * The power level required to send state events to the room. This
     * get applied if the event type doesn’t have an explicit level
     * requirement (see set_event_level() and get_event_level()).
     */
    public int default_state_level { get; set; default = 10; }

    /**
     * The power level required to ban other users from the room.
     */
    public int ban_level { get; set; default = 5; }

    /**
     * The power level required to kick other users from the room.
     */
    public int kick_level { get; set; default = 5; }

    /**
     * The power level required to redact events in the room.
     */
    public int redact_level { get; set; default = 20; }

    /**
     * The power level required to invite users to the room.
     */
    public int invite_level { get; set; default = 0; }

    /**
     * The room’s topic.
     */
    public string? topic { get; set; default = null; }

    /**
     * The users currently typing in the room.
     */
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

    /**
     * Create a new Room object.
     */
    public
    Room(string room_id)
    {
        Object(room_id : room_id);
    }

    /**
     * Add a member to the room member list. If a member with the
     * same @user_id exists, {@link Matrix.Error.ALREADY_EXISTS} is
     * thrown.
     *
     * @param user_id the Matrix ID of the user to add
     * @param third_party if true, the member is marked as a pending
     *                    3rd party invitation
     */
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

    /**
     * Gets the profile of the room member specified in @user_id. If
     * that user is not added to the room yet, it gets added with an
     * empty profile and that profile is returned.
     *
     * @param user_id the Matrix ID of the user to add
     * @param third_party if true, the member is marked as a pending
     *                    3rd party invitation
     * @return the {@link Matrix.Profile} of the user
     */
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

    /**
     * Gets the profile of the room member specified in @user_id. If
     * that user is not added to the room yet,
     * {@link Matrix.Error.NOT_FOUND} is thrown.
     *
     * @param user_id the Matrix ID of the user to find
     * @param third_party gets a true value if the member is actually
     *                    a pending 3rd party invitation
     * @return the profile of the user
     */
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

    /**
     * Removes a member from the member list.
     */
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
