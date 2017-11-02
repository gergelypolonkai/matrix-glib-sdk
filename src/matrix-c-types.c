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

#include "matrix-c-types.h"

/**
 * SECTION:matrix-types
 * @title: Generic types
 * @short_description: Generic types for Matrix GLib SDK calls
 *
 * These are the generic types used by many SDK calls for communication with the homeserver.
 */

/**
 * MatrixError:
 * @MATRIX_ERROR_NONE: no error. You should never see this.
 * @MATRIX_ERROR_COMMUNICATION_ERROR: there was a problem in communication (e.g. connection error)
 * @MATRIX_ERROR_INCOMPLETE: the passed/generated data is incomplete
 * @MATRIX_ERROR_BAD_REQUEST: the request is invalid
 * @MATRIX_ERROR_BAD_RESPONSE: malformed response, or the response is not a JSON object
 * @MATRIX_ERROR_INVALID_ROOM_ID: the provided string doesn’t contain a valid room ID
 * @MATRIX_ERROR_UNKNOWN_VALUE: the response from the Matrix.org server contains a value unknown
 *     to this library. These should be reported to the Matrix GLib SDK developers
 * @MATRIX_ERROR_INVALID_TYPE: the provided type is invalid
 * @MATRIX_ERROR_UNSUPPORTED: the operation is unsupported
 * @MATRIX_ERROR_INVALID_FORMAT: the format of the JSON node is invalid (e.g. it is an array instead of an object)
 * @MATRIX_ERROR_UNAVAILABLE: the requested data is not cached yet. Clients getting this message
 *     may go online by some means to get the data
 * @MATRIX_ERROR_NOT_FOUND: the requested data (e.g. member of a room) can not be found
 * @MATRIX_ERROR_ALREADY_EXISTS: the data to create (e.g. when adding a new member to a Room
 *     object) already exists
 * @MATRIX_ERROR_M_MISSING_TOKEN: authorization token is missing from the request
 * @MATRIX_ERROR_M_FORBIDDEN: access was forbidden (e.g. due to a missing/invalid token, or using
 *     a bad password during login)
 * @MATRIX_ERROR_M_UNKNOWN: an error unknown to the Matrix homeserver
 * @MATRIX_ERROR_M_UNKNOWN_TOKEN: the token provided is not known for the homeserver
 * @MATRIX_ERROR_M_NOT_JSON: illegal request, the content is not valid JSON
 * @MATRIX_ERROR_M_UNRECOGNIZED: the homeserver didn't understand the request
 * @MATRIX_ERROR_M_UNAUTHORIZED: the request is unauthorized
 * @MATRIX_ERROR_M_BAD_JSON: the JSON data is not in the required format
 * @MATRIX_ERROR_M_USER_IN_USE: the specified username is in use
 * @MATRIX_ERROR_M_ROOM_IN_USE: the specified room is in use
 * @MATRIX_ERROR_M_BAD_PAGINATION: invalid pagination parameters
 * @MATRIX_ERROR_M_BAD_STATE: invalid state event
 * @MATRIX_ERROR_M_NOT_FOUND: the requested resource is not found
 * @MATRIX_ERROR_M_GUEST_ACCESS_FORBIDDEN: guest access was requested, but ( it is forbidden
 * @MATRIX_ERROR_M_LIMIT_EXCEEDED: the request was rate limited
 * @MATRIX_ERROR_M_CAPTCHA_NEEDED: a captcha is needed to continue
 * @MATRIX_ERROR_M_CAPTCHA_INVALID: the provided captcha is invalid
 * @MATRIX_ERROR_M_MISSING_PARAM: a parameter is missing from the request
 * @MATRIX_ERROR_M_TOO_LARGE: the request data is too large
 * @MATRIX_ERROR_M_EXCLUSIVE: the desired user ID is in an exclusive namespace claimed by an
 *     application server
 * @MATRIX_ERROR_M_THREEPID_AUTH_FAILED: 3rd party authentication failed
 * @MATRIX_ERROR_M_THREEPID_IN_USE: the provided 3rd party ID is already in use
 * @MATRIX_ERROR_M_INVALID_USERNAME: the given username is invalid
 * @MATRIX_ERROR_UNSPECIFIED: no error code was sent by the homeserver. If you see this error,
 *     that usually indicates a homeserver bug
 * @MATRIX_ERROR_UNKNOWN_ERROR: an error unknown to this library
 *
 * Matrix SDK error codes.  The SDK maps most known error codes from homeservers, too; these are
 * the MATRIX_ERROR_M_* codes.
 */

/**
 * MATRIX_ERROR:
 *
 * Error domain for Matrix GLib SDK.  See #GError for more information on error domains.
 */

/**
 * matrix_error_quark:
 *
 * Gets the Matrix error #GQuark
 */
G_DEFINE_QUARK(matrix-error-quark, matrix_error);

/**
 * MatrixAccountKind:
 * @MATRIX_ACCOUNT_KIND_DEFAULT: use the server default (usually #MATRIX_ACCOUNT_KIND_USER)
 * @MATRIX_ACCOUNT_KIND_USER: normal user
 * @MATRIX_ACCOUNT_KIND_GUEST: guest user
 *
 * User account types.
 */

/**
 * MatrixEventDirection:
 * @MATRIX_EVENT_DIRECTION_FORWARD: list events after the specified one
 * @MATRIX_EVENT_DIRECTION_BACKWARD: list events before the specified one
 *
 * Direction of events when requesting an event context.
 */

/**
 * MatrixEventFormat:
 * @MATRIX_EVENT_FORMAT_DEFAULT: event format will be omitted from the filter, so the server
 *     will use its default (usually #MATRIX_EVENT_FORMAT_FEDERATION)
 * @MATRIX_EVENT_FORMAT_CLIENT: return the events in a format suitable for clients
 * @MATRIX_EVENT_FORMAT_FEDERATION: return the raw event as receieved over federation
 *
 * Event format received when synchronizing.
 */

/**
 * MatrixPresence:
 * @MATRIX_PRESENCE_UNKNOWN: user's presence is unknown
 * @MATRIX_PRESENCE_ONLINE: user is online
 * @MATRIX_PRESENCE_OFFLINE: user is offline
 * @MATRIX_PRESENCE_UNAVAILABLE: user is unavailable (i.e. busy)
 * @MATRIX_PRESENCE_FREE_FOR_CHAT: user is free for chat
 *
 * Presence values for matrix_api_set_presence() and other presence related queries.
 */

/**
 * MatrixPusherConditionKind:
 * @MATRIX_PUSHER_CONDITION_KIND_EVENT_MATCH: glob pattern match on a field of the event.
 *     Requires a `key` and a `pattern` parameter
 * @MATRIX_PUSHER_CONDITION_KIND_PROFILE_TAG: matches the profile tag of the device that the
 *     notification would be delivered to.  Requires a `profile_tag` parameter
 * @MATRIX_PUSHER_CONDITION_KIND_CONTAINS_DISPLAY_NAME: matches unencrypted messages where the
 *     content's body contains the owner's display name in that room.
 * @MATRIX_PUSHER_CONDITION_KIND_ROOM_MEMBER_COUNT: matches the current number of members in the
 *     room.  Requires an `is` parameter, which must be an integer, optionally prefixed by `==`,
 *     `<`, `>`, `<=` or `>=`.  If the prefix is omitted, it defaults to `==`
 *
 * Condition types for pushers.
 */

/**
 * MatrixPusherKind:
 * @MATRIX_PUSHER_KIND_OVERRIDE: highest priority rules
 * @MATRIX_PUSHER_KIND_SENDER: for (unencrypted) messages that match certain patterns
 * @MATRIX_PUSHER_KIND_ROOM: for all messages for a given room. The rule ID of a room rule is
 *     always the ID of the room that it affects
 * @MATRIX_PUSHER_KIND_CONTENT: for messages from a specific Matrix user ID. The rule ID of such
 *     rules is always the Matrix ID of the user whose messages they'd apply to
 * @MATRIX_PUSHER_KIND_UNDERRIDE: lowest priority rules
 *
 * Pusher types.
 */

/**
 * MatrixReceiptType:
 * @MATRIX_RECEIPT_TYPE_READ: indicate that the message has been read
 *
 * Receipt types of acknowledgment.
 */

/**
 * MatrixResizeMethod:
 * @MATRIX_RESIZE_METHOD_DEFAULT: use the server default value
 * @MATRIX_RESIZE_METHOD_CROP: crop thumbnail to the requested size
 * @MATRIX_RESIZE_METHOD_SCALE: scale thumbnail to the requested size
 *
 * Resizing methods for matrix_api_media_thumbnail().
 */

/**
 * MatrixRoomMembership:
 * @MATRIX_ROOM_MEMBERSHIP_UNKNOWN: the membership sent by the server is unknown to this SDK
 * @MATRIX_ROOM_MEMBERSHIP_INVITE: the user has been invited to join a room, but has not yet
 *     joined it. They may not participate in the room until they join
 * @MATRIX_ROOM_MEMBERSHIP_JOIN: the user has joined the room (possibly after accepting an invite),
 *     and may participate in it
 * @MATRIX_ROOM_MEMBERSHIP_LEAVE: the user was once joined to the room, but has since left
 *     (possibly by choice, or possibly by being kicked)
 * @MATRIX_ROOM_MEMBERSHIP_BAN: the user has been banned from the room, and is no longer allowed
 *     to join it until they are un-banned from the room (by having their membership state set
 *     to a value other than MATRIX_ROOM_MEMBERSHIP_BAN)
 * @MATRIX_ROOM_MEMBERSHIP_KNOCK: this is a reserved word, which currently has no meaning
 *
 * Room membership types.
 */

/**
 * MatrixRoomPreset:
 * @MATRIX_ROOM_PRESET_NONE: no preset
 * @MATRIX_ROOM_PRESET_PRIVATE: preset for private rooms
 * @MATRIX_ROOM_PRESET_TRUSTED_PRIVATE: same as private rooms, but all users get the same power
 *     level as the room creator
 * @MATRIX_ROOM_PRESET_PUBLIC: preset for public rooms
 *
 * Preset values for matrix_api_create_room() calls.
 */

/**
 * MatrixRoomVisibility:
 * @MATRIX_ROOM_VISIBILITY_DEFAULT: use a server-assigned value (usually
 *     #MATRIX_ROOM_VISIBILITY_PRIVATE)
 * @MATRIX_ROOM_VISIBILITY_PUBLIC: make the room visible in the public room list
 * @MATRIX_ROOM_VISIBILITY_PRIVATE: hide the room from the public room list
 *
 * Visibility values for room creation. Not to be confused with join rules.
 */

/**
 * MatrixJoinRules:
 * @MATRIX_JOIN_RULES_UNKNOWN: a value unknown to this library
 * @MATRIX_JOIN_RULES_PUBLIC: anyone can join
 * @MATRIX_JOIN_RULES_INVITE: users may join upon invite
 * @MATRIX_JOIN_RULES_PRIVATE: reserved word, not usable yet
 * @MATRIX_JOIN_RULES_KNOCK: reserved word, not usable yet
 *
 * Room join rules.
 */

/**
 * MatrixSearchOrder:
 * @MATRIX_SEARCH_ORDER_RECENT: order messages as they arrived
 * @MATRIX_SEARCH_ORDER_RANK: order messages by relevance
 *
 * Search ordering.
 */

/**
 * MatrixSearchKey:
 * @MATRIX_SEARCH_KEY_CONTENT_BODY: search in the body of a message
 * @MATRIX_SEARCH_KEY_CONTENT_NAME: search in the name of rooms
 * @MATRIX_SEARCH_KEY_CONTENT_TOPIC: search in the topic of rooms
 *
 * Search keys.
 */
