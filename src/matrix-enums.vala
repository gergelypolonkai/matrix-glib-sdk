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
    public errordomain Error {
        NONE,                  /// no error
        COMMUNICATION_ERROR,   /// there was a problem in communication (e.g. connection error)
        INCOMPLETE,            /// the passed/generated data is incomplete
        BAD_REQUEST,           /// the request is invalid
        BAD_RESPONSE,          /// malformed response, or the response is not a JSON object
        INVALID_ROOM_ID,       /// the provided string doesn’t contain a valid room ID
        UNKNOWN_VALUE,         /// the response from the Matrix.org server contains a value unknown to this library. These should be reported to the Matrix GLib SDK developers
        INVALID_TYPE,          /// the provided type is invalid
        UNSUPPORTED,           ///the operation is unsupported
        INVALID_FORMAT,        /// the format of the JSON node is invalid (e.g. it is an array instead of an object)

        /* Add Matrix-defined error codes under here, prefixing them with
         * `MATRIX_ERROR_`, i.e. `M_FORBIDDEN` =>
         * `MATRIX_ERROR_M_FORBIDDEN` */
        M_MISSING_TOKEN = 500,    /// authorization token is missing from the request
        M_FORBIDDEN,              /// access was forbidden (e.g. due to a missing/invalid token, or using a bad password during login)
        M_UNKNOWN,                /// an error unknown to the Matrix homeserver
        M_UNKNOWN_TOKEN,          /// the token provided is not known for the homeserver
        M_NOT_JSON,               /// illegal request, the content is not valid JSON
        M_UNRECOGNIZED,           /// the homeserver didn't understand the request
        M_UNAUTHORIZED,           /// the request is unauthorized
        M_BAD_JSON,               /// the JSON data is not in the required format
        M_USER_IN_USE,            /// the specified username is in use
        M_ROOM_IN_USE,            /// the specified room is in use
        M_BAD_PAGINATION,         /// invalid pagination parameters
        M_BAD_STATE,              /// invalid state event
        M_NOT_FOUND,              /// the requested resource is not found
        M_GUEST_ACCESS_FORBIDDEN, /// guest access was requested, but it is forbidden
        M_LIMIT_EXCEEDED,         /// the request was rate limited
        M_CAPTCHA_NEEDED,         /// a captcha is needed to continue
        M_CAPTCHA_INVALID,        /// the provided captcha is invalid
        M_MISSING_PARAM,          /// a parameter is missing from the request
        M_TOO_LARGE,              /// the request data is too large
        M_EXCLUSIVE,              /// the desired user ID is in an exclusive namespace claimed by an application server
        M_THREEPID_AUTH_FAILED,   /// 3rd party authentication failed
        M_THREEPID_IN_USE,        /// the provided 3rd party ID is already in use
        M_INVALID_USERNAME,       /// the given username is invalid

        /* Allow for a lot of Matrix.org defined codes
         * Do not define error codes after this! */
        UNSPECIFIED = 16383,      /// no error code was sent by the homeserver. If you see this error, that usually indicates a homeserver bug
        UNKNOWN_ERROR;            /// an error unknown to this library

        public static GLib.Quark
        quark ()
        {
            return Quark.from_string("matrix-error-quark");
        }
    }

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
     * Pusher types.
     */
    public enum PusherKind {
        OVERRIDE,  /// highest priority rules
        SENDER,    /// for (unencrypted) messages that match certain patterns
        ROOM,      /// for all messages for a given room. The rule ID of a room rule is always the ID of the room that it affects
        CONTENT,   /// for messages from a specific Matrix user ID. The rule ID of such rules is always the Matrix ID of the user whose messages they'd apply to
        UNDERRIDE; /// lowest priority rules
    }

    /**
     * Receipt types of acknowledgment.
     */
    public enum ReceiptType {
        READ; /// indicate that the message has been read
    }

    /**
     * Resizing methods for matrix_api_media_thumbnail().
     */
    public enum ResizeMethod {
        DEFAULT, /// use the server default value
        CROP,    /// crop thumbnail to the requested size
        SCALE;   /// scale thumbnail to the requested size
    }

    /**
     * Room membership types.
     */
    public enum RoomMembership {
        UNKNOWN, /// the membership sent by the server is unknown to this SDK
        INVITE,  /// the user has been invited to join a room, but has not yet joined it. They may not participate in the room until they join
        JOIN,    /// the user has joined the room (possibly after accepting an invite), and may participate in it
        LEAVE,   /// the user was once joined to the room, but has since left (possibly by choice, or possibly by being kicked)
        BAN,     /// the user has been banned from the room, and is no longer allowed to join it until they are un-banned from the room (by having their membership state set to a value other than {{{BAN}}})
        KNOCK;   /// this is a reserved word, which currently has no meaning
    }

    /**
     * Preset values for matrix_api_create_room() calls.
     */
    public enum RoomPreset {
        NONE,            /// no preset
        PRIVATE,         /// preset for private rooms
        TRUSTED_PRIVATE, /// same as private rooms, but all users get the same power level as the room creator
        PUBLIC;          /// preset for public rooms
    }

    /**
     * Visibility values for room creation. Not to be confused with
     * join rules.
     */
    public enum RoomVisibility {
        DEFAULT, /// use a server-assigned value (usually {{{private}}}
        PUBLIC,  /// make the room visible in the public room list
        PRIVATE; /// hide the room from the public room list
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
