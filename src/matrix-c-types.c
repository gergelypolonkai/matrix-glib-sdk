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

#include "config.h"
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

/**
 * MatrixSearchGroupBy:
 * @MATRIX_SEARCH_GROUP_BY_NONE: no grouping
 * @MATRIX_SEARCH_GROUP_BY_ROOM_ID: group by room ID
 * @MATRIX_SEARCH_GROUP_BY_SENDER: group by sender
 *
 * Search grouping
 *
 * The client can request that the results are returned along with
 * grouping information, e.g. grouped by room_id. In this case the
 * response will contain a group entry for each distinct value of
 * room_id. Each group entry contains at least a list of the
 * event_ids that are in that group, as well as potentially other
 * metadata about the group.
 */

/**
 * MatrixHistoryVisibility:
 * @MATRIX_HISTORY_VISIBILITY_UNKNOWN: represents a value unknown to this library
 * @MATRIX_HISTORY_VISIBILITY_INVITED: only room members can see the room history, and only what
 *     happened after they got an invitation
 * @MATRIX_HISTORY_VISIBILITY_JOINED: only room members can see the room history, and only what
 *     happened after they joined
 * @MATRIX_HISTORY_VISIBILITY_SHARED: only room members can see the room history, but they see all
 *     of it
 * @MATRIX_HISTORY_VISIBILITY_WORLD_READABLE: anyone can see the room history
 *
 * Room history visibility
 */

/**
 * MatrixGuestAccess:
 * @MATRIX_GUEST_ACCESS_UNKNOWN: represents a value unknown to this library
 * @MATRIX_GUEST_ACCESS_CAN_JOIN: guest users are allowed to access the room
 * @MATRIX_GUEST_ACCESS_FORBIDDEN: guest users are not allowed to access the room
 *
 * Room guest access
 */

/**
 * MatrixCallOfferType:
 * @MATRIX_CALL_OFFER_TYPE_UNKNOWN: represents a value unknown to this library
 * @MATRIX_CALL_OFFER_TYPE_OFFER: call offer
 *
 * Call offer types
 */

/**
 * MatrixCallAnswerType:
 * @MATRIX_CALL_ANSWER_TYPE_UNKNOWN: represents a value unknown to this library
 * @MATRIX_CALL_ANSWER_TYPE_ANSWER: call answer
 *
 * Call answer types
 */

/**
 * MatrixFileInfo: (ref-func matrix_file_info_ref) (unref-func matrix_file_info_unref)
 *
 * Information about the file referred to in a URL.
 */
struct _MatrixFileInfo {
    gssize size;
    gchar *mimetype;

    volatile int refcount;
};
G_DEFINE_BOXED_TYPE(MatrixFileInfo, matrix_file_info, (GBoxedCopyFunc)matrix_file_info_ref, (GBoxedFreeFunc)matrix_file_info_unref);

/**
 * matrix_file_info_new:
 *
 * Create a new #MatrixFileInfo object with a reference count of 1.
 *
 * Returns: (transfer full): A new #MatrixFileInfo object.
 */
MatrixFileInfo *
matrix_file_info_new(void)
{
    MatrixFileInfo *ret = g_new0(MatrixFileInfo, 1);

    ret->refcount = 1;
    ret->size = -1;

    return ret;
}

static void
matrix_file_info_destroy(MatrixFileInfo *file_info)
{
    g_free(file_info->mimetype);
}

/**
 * matrix_file_info_ref:
 * @file_info: (nullable): a #MatrixFileInfo object
 *
 * Increment reference count on @file_info.
 *
 * Returns: (transfer full): @file_info
 */
MatrixFileInfo *
matrix_file_info_ref(MatrixFileInfo *file_info)
{
    g_return_val_if_fail(file_info != NULL, NULL);

    file_info->refcount++;

    return file_info;
}

/**
 * matrix_file_info_unref:
 * @file_info: (nullable) (transfer full): a #MatrixFileInfo object
 *
 * Decrement reference count on @file_info.  If reference count reaches zero, destroy the whole
 * object.
 */
void
matrix_file_info_unref(MatrixFileInfo *matrix_file_info)
{
    g_return_if_fail(matrix_file_info != NULL);

    if (--(matrix_file_info->refcount) == 0) {
        matrix_file_info_destroy(matrix_file_info);
        g_free(matrix_file_info);
    }
}

/**
 * matrix_file_info_set_size:
 * @file_info: (nullable): a #MatrixFileInfo object
 * @size: the size to set
 *
 * Set the size of the file described by @file_info.
 */
void
matrix_file_info_set_size(MatrixFileInfo *matrix_file_info, gssize size)
{
    g_return_if_fail(matrix_file_info != NULL);

    matrix_file_info->size = size;
}

/**
 * matrix_file_info_get_size:
 * @file_info: (nullable): A #MatrixFileInfo object
 *
 * Get the size of the file described by @file_info.
 *
 * Returns: the file size.
 */
gssize
matrix_file_info_get_size(MatrixFileInfo *matrix_file_info)
{
    g_return_val_if_fail(matrix_file_info != NULL, -1);

    return matrix_file_info->size;
}

/**
 * matrix_file_info_set_mimetype:
 * @file_info: (nullable): A #MatrixFileInfo object
 * @mimetype: the MIME type to set
 *
 * Set the MIME type of the file described by @file_info.
 */
void
matrix_file_info_set_mimetype(MatrixFileInfo *matrix_file_info, const gchar *mimetype)
{
    g_return_if_fail(matrix_file_info != NULL);

    g_free(matrix_file_info->mimetype);
    matrix_file_info->mimetype = g_strdup(mimetype);
}

/**
 * matrix_file_info_get_mimetype:
 * @file_info: (nullable): A #MatrixFileInfo object
 *
 * Get the MIME type of the file described by @file_info.
 *
 * Returns: (nullable) (transfer none): the MIME type.  The value is owned by @file_info and
 * shouldn’t be freed.
 */
const gchar *
matrix_file_info_get_mimetype(MatrixFileInfo *matrix_file_info)
{
    g_return_val_if_fail(matrix_file_info != NULL, NULL);

    return matrix_file_info->mimetype;
}

/**
 * matrix_file_info_set_from_json:
 * @file_info: (nullable): A #MatrixFileInfo object
 * @json_data: (nullable): a #JsonNode object
 *
 * Load the data in @json_data into the fields of @file_info.  @json_data must hold a valid Matrix
 * file info object with a size and mimetype fields.
 */
void
matrix_file_info_set_from_json(MatrixFileInfo *file_info, JsonNode *json_data)
{
    JsonNode *node;
    JsonObject *root;

    g_return_if_fail(file_info != NULL);
    g_return_if_fail(json_data != NULL);

    root = json_node_get_object(json_data);

    if ((node = json_object_get_member(root, "size"))) {
        file_info->size = json_node_get_int(node);
    } else if (DEBUG) {
        g_warning("size is missing from a FileInfo");
    }

    if ((node = json_object_get_member(root, "mimetype"))) {
        g_free(file_info->mimetype);
        file_info->mimetype = g_strdup(json_node_get_string(node));
    } else if (DEBUG) {
        g_warning("mimetype is missing from a FileInfo");
    }
}

/**
 * matrix_file_info_get_json_node:
 * @file_info: (nullable): a #MatrixFileInfo object
 * @error: (nullable): a #GError, or NULL to ignore
 *
 * Convert @file_info to a #JsonNode.  If the file size is negative or the MIME type is not set,
 * this function returns NULL and @error is set to #MATRIX_ERROR_INCOMPLETE.
 *
 * Returns: (transfer full) (nullable): a #JsonNode with a valid Matrix file info object
 */
JsonNode *
matrix_file_info_get_json_node(MatrixFileInfo *file_info, GError **error)
{
    JsonNode *node;
    JsonObject *obj;

    if ((file_info->size == -1) || (file_info->mimetype == NULL)) {
        g_set_error(error, MATRIX_ERROR, MATRIX_ERROR_INCOMPLETE,
                    "Won't generate a FileInfo without all fields set.");
        return NULL;
    }

    node = json_node_new(JSON_NODE_OBJECT);
    obj = json_object_new();
    json_node_set_object(node, obj);

    json_object_set_int_member(obj, "size", file_info->size);
    json_object_set_string_member(obj, "mimetype", file_info->mimetype);

    return node;
}
