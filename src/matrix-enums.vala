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

namespace Matrix {
    /**
     * User account types.
     */
    public enum AccountKind {
        DEFAULT, /// use the server default (usually {{{USER}}})
        USER,    /// normal user
        GUEST;   /// guest user
    }

    /**
     * Direction of events when requesting an event context.
     */
    public enum EventDirection {
        FORWARD,  /// List events after the specified one
        BACKWARD; /// List events before the specified one
    }

    /**
     * Event format received when synchronizing.
     */
    public enum EventFormat {
        DEFAULT,    /// event format will be omitted from the filter, so the server will use its default (usually {{{FEDERATION}}})
        CLIENT,     /// return the events in a format suitable for clients
        FEDERATION; /// return the raw event as receieved over federation
    }

    /**
     * Presence values for matrix_api_set_user_presence() and other
     * presence related queries.
     */
    public enum Presence {
        UNKNOWN,       /// user's presence is unknown
        ONLINE,        /// user is online
        OFFLINE,       /// user is offline
        UNAVAILABLE,   /// user is unavailable (i.e. busy)
        FREE_FOR_CHAT; /// user is free for chat
    }

    /**
     * Condition types for pushers.
     */
    public enum PusherConditionKind {
        EVENT_MATCH,           /// glob pattern match on a field of the event. Requires a {{{key}}} and a {{{pattern}}} parameter
        PROFILE_TAG,           /// matches the profile tag of the device that the notification would be delivered to. Requires a {{{profile_tag}}} parameter
        CONTAINS_DISPLAY_NAME, /// matches unencrypted messages where the content's body contains the owner's display name in that room.
        ROOM_MEMBER_COUNT;     /// matches the current number of members in the room. Requires an {{{is}}} parameter, which must be an integer, optionally prefixed by {{==}}}, {{{&lt;}}}, {{{&gt;}}}, {{{&lt;=}}} or {{{&gt;=}}}. If the prefix is omitted, it defaults to {{{==}}}
    }

    /**
     * Resizing methods for matrix_api_media_thumbnail().
     */
    public enum ResizeMethod {
        DEFAULT, /// use the server default value
        CROP,    /// crop thumbnail to the requested size
        SCALE;   /// scale thumbnail to the requested size
    }

    private int?
    _g_enum_nick_to_value(Type enum_type, string nick)
    {
        EnumClass enum_class = (EnumClass)enum_type.class_ref();
        unowned EnumValue? enum_val = enum_class.get_value_by_nick(nick);

        if (enum_val != null) {
            return enum_val.value;
        } else {
            return null;
        }
    }

    private string?
    _g_enum_value_to_nick(Type enum_type,
                          int value,
                          bool convert_dashes = true)
    {
        EnumClass enum_class = (EnumClass)enum_type.class_ref();
        unowned EnumValue? enum_val = enum_class.get_value(value);

        if (enum_val != null) {
            var nick = enum_val.value_nick;

            if (convert_dashes) {
                return nick.replace("-", "_");
            }

            return nick;
        } else {
            return null;
        }
    }
}
