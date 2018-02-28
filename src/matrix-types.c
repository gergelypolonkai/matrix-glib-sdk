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
#include "matrix-types.h"

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
 * @MATRIX_RECEIPT_TYPE_UNKNOWN: the receipt type is unknown to this SDK
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

/**
 * MatrixAudioInfo: (ref-func matrix_audio_info_ref) (unref-func matrix_audio_info_unref)
 *
 * Information about an audio file referred to by an URL.
 */

struct _MatrixAudioInfo {
    gssize size;
    gchar *mimetype;
    gint duration;

    volatile int refcount;
};
G_DEFINE_BOXED_TYPE(MatrixAudioInfo, matrix_audio_info, (GBoxedCopyFunc)matrix_audio_info_ref, (GBoxedFreeFunc)matrix_audio_info_unref);

/**
 * matrix_audio_info_new:
 *
 * Creates a new #MatrixAudioInfo object with a reference count of 1.
 *
 * Returns: (transfer full): A new #MatrixAudioInfo object
 */
MatrixAudioInfo *
matrix_audio_info_new(void)
{
    MatrixAudioInfo *ret = g_new0(MatrixAudioInfo, 1);

    ret->refcount = 1;
    ret->size = -1;
    ret->duration = -1;

    return ret;
}

/**
 * matrix_audio_info_ref:
 * @audio_info: (nullable): A #MatrixAudioInfo object
 *
 * Increment reference count on @audio_info by one.
 *
 * Returns: (transfer full): the same object
 */
MatrixAudioInfo *
matrix_audio_info_ref(MatrixAudioInfo *matrix_audio_info)
{
    g_return_val_if_fail(matrix_audio_info != NULL, NULL);

    matrix_audio_info->refcount++;

    return matrix_audio_info;
}

static void
matrix_audio_info_destroy(MatrixAudioInfo *audio_info)
{
    g_free(audio_info->mimetype);
}

/**
 * matrix_audio_info_unref:
 * @audio_info: (nullable): A #MatrixAudioInfo object
 *
 * Decrement reference count on @audio_info by one.
 *
 * If reference count reaches zero, the whole object is freed.
 */
void
matrix_audio_info_unref(MatrixAudioInfo *matrix_audio_info)
{
    g_return_if_fail(matrix_audio_info != NULL);

    if (--(matrix_audio_info->refcount) == 0) {
        matrix_audio_info_destroy(matrix_audio_info);
        g_free(matrix_audio_info);
    }
}

/**
 * matrix_audio_info_set_size:
 * @audio_info: A #MatrixAudioInfo object
 * @size: the size of the audio file, in bytes
 *
 * Set the size of the audio file described by @audio_info.
 *
 * -1 is considered an invalid value by matrix_audio_info_get_json_node(), but can be set to
 * indicate incompleteness.
 */
void
matrix_audio_info_set_size(MatrixAudioInfo *matrix_audio_info, gssize size)
{
    g_return_if_fail(matrix_audio_info != NULL);

    matrix_audio_info->size = size;
}

/**
 * matrix_audio_info_get_size:
 * @audio_info: A #MatrixAudioInfo object
 *
 * Get the size of the audio file described by @audio_info.
 *
 * Returns: the size of the file, or -1 if unset.
 */
gssize
matrix_audio_info_get_size(MatrixAudioInfo *matrix_audio_info)
{
    g_return_val_if_fail(matrix_audio_info != NULL, -1);

    return matrix_audio_info->size;
}

/**
 * matrix_audio_info_set_mimetype:
 * @audio_info: a #MatrixAudioInfo object
 * @mimetype: (nullable) (transfer none): a MIME type
 *
 * Set the MIME type of the audio file described by @audio_info.
 *
 * NULL is considered an invalid value by matrix_audio_info_get_json_node(), but can be set to
 * indicate incompleteness.
 */
void
matrix_audio_info_set_mimetype(MatrixAudioInfo *matrix_audio_info, const gchar *mimetype)
{
    g_return_if_fail(matrix_audio_info != NULL);

    g_free(matrix_audio_info->mimetype);
    matrix_audio_info->mimetype = g_strdup(mimetype);
}

/**
 * matrix_audio_info_get_mimetype:
 * @audio_info: a #MatrixAudioInfo object
 *
 * Get the MIME type of the audio file described by @audio_info.
 *
 * The returned value is owned by @audio_info, and should not be freed nor modified.
 *
 * Returns: (transfer none) (nullable): the MIME type, or NULL if not set
 */
const gchar *
matrix_audio_info_get_mimetype(MatrixAudioInfo *matrix_audio_info)
{
    g_return_val_if_fail(matrix_audio_info != NULL, NULL);

    return matrix_audio_info->mimetype;
}

/**
 * matrix_audio_info_set_duration:
 * @audio_info: a #MatrixAudioInfo object
 * @duration: a duration in seconds
 *
 * Set the duration of the audio file described by @audio_info.
 *
 * -1 is considered an invalid value by matrix_audio_info_get_json_node(), but can be set to
 * indicate incompleteness.
 */
void
matrix_audio_info_set_duration(MatrixAudioInfo *matrix_audio_info, gint duration)
{
    g_return_if_fail(matrix_audio_info != NULL);

    matrix_audio_info->duration = duration;
}

/**
 * matrix_audio_info_get_duration:
 * @audio_info: a #MatrixAudioInfo object
 *
 * Get the duration of the audio file described by @audio_info, in seconds.
 *
 * Returns: the duration
 */
gint
matrix_audio_info_get_duration(MatrixAudioInfo *matrix_audio_info)
{
    g_return_val_if_fail(matrix_audio_info != NULL, -1);

    return matrix_audio_info->duration;
}

/**
 * matrix_audio_info_set_from_json:
 * @audio_info: A #MatrixAudioInfo object
 * @json_data: a #JsonNode object
 *
 * Load the data in @json_data into the fields of @audio_info.  @json_data must hold a valid Matrix
 * audio info object with a size, duration, and mimetype fields.
 */
void
matrix_audio_info_set_from_json(MatrixAudioInfo *audio_info, JsonNode *json_data)
{
    JsonNode *node;
    JsonObject *root;

    root = json_node_get_object(json_data);

    if ((node = json_object_get_member(root, "size"))) {
        audio_info->size = json_node_get_int(node);
    } else if (DEBUG) {
        g_warning("size is missing from an ImageInfo");
    }

    if ((node = json_object_get_member(root, "mimetype"))) {
        audio_info->mimetype = g_strdup(json_node_get_string(node));
    } else if (DEBUG) {
        g_warning("mimetype is missing from an ImageInfo");
    }

    if ((node = json_object_get_member(root, "duration"))) {
        audio_info->duration = json_node_get_int(node);
    } else if (DEBUG) {
        g_warning("duration is missing from an ImageInfo");
    }
}

/**
 * matrix_audio_info_get_json_node:
 * @audio_info: a #MatrixAudioInfo object
 * @error: (nullable): a #GError, or NULL to ignore
 *
 * Convert @audio_info to a #JsonNode.  If the file size or the duration is negative, or the MIME
 * type is not set, this function returns NULL and @error is set to #MATRIX_ERROR_INCOMPLETE.
 *
 * Returns: (transfer full) (nullable): a #JsonNode with a valid Matrix audio info object
 */
JsonNode *
matrix_audio_info_get_json_node(MatrixAudioInfo *audio_info, GError **error)
{
    JsonNode * node;
    JsonObject *obj;

    if ((audio_info->size == -1)
        || (audio_info->mimetype == NULL)
        || (audio_info->duration == -1)) {
        g_set_error(error, MATRIX_ERROR, MATRIX_ERROR_INCOMPLETE,
                    "Won't generate an ImageInfo without all fields set.");
        return NULL;
    }

    node = json_node_new(JSON_NODE_OBJECT);
    obj = json_object_new();
    json_node_set_object(node, obj);

    json_object_set_int_member(obj, "size", audio_info->size);
    json_object_set_string_member(obj, "mimetype", audio_info->mimetype);
    json_object_set_int_member(obj, "duration", audio_info->duration);

    return node;
}

/**
 * MatrixImageInfo: (ref-func matrix_image_info_ref) (unref-func matrix_image_info_unref)
 *
 * Information about an image referred to in an URL.
 */
struct _MatrixImageInfo {
    gssize size;
    gint height;
    gint width;
    gchar *mimetype;

    volatile int refcount;
};
G_DEFINE_BOXED_TYPE(MatrixImageInfo, matrix_image_info, (GBoxedCopyFunc)matrix_image_info_ref, (GBoxedFreeFunc)matrix_image_info_unref);

/**
 * matrix_image_info_new:
 *
 * Create a new #MatrixImageInfo object with a reference count of 1.
 *
 * Returns: (transfer full): A new #MatrixImageInfo object.
 */
MatrixImageInfo *
matrix_image_info_new()
{
    MatrixImageInfo *image_info = g_new0(MatrixImageInfo, 1);

    image_info->refcount = 1;
    image_info->size = -1;
    image_info->height = -1;
    image_info->width = -1;

    return image_info;
}

/**
 * matrix_image_info_ref:
 * @image_info: (nullable): A #MatrixImageInfo object
 *
 * Increment reference count on @image_info.
 *
 * Returns: (transfer full): @image_info
 */
MatrixImageInfo *
matrix_image_info_ref(MatrixImageInfo *matrix_image_info)
{
    g_return_val_if_fail(matrix_image_info != NULL, NULL);

    matrix_image_info->refcount++;

    return matrix_image_info;
}

static void
matrix_image_info_destroy(MatrixImageInfo *image_info)
{
    g_free(image_info->mimetype);
}

/**
 * matrix_image_info_unref:
 * @image_info: (nullable) (transfer full): a #MatrixImageInfo object
 *
 * Decrement reference count on @image_info.  If reference count reaches zero, destroy the whole
 * object.
 */
void
matrix_image_info_unref(MatrixImageInfo *matrix_image_info)
{
    g_return_if_fail(matrix_image_info != NULL);

    if (--(matrix_image_info->refcount) == 0) {
        matrix_image_info_destroy(matrix_image_info);
        g_free(matrix_image_info);
    }
}

/**
 * matrix_image_info_set_size:
 * @image_info: a #MatrixImageInfo object
 * @size: the size to set
 *
 * Set the size of the image described by @image_info.
 *
 * -1 is considered an invalid value by matrix_image_info_get_json_node(), but can be set to
 * indicate incompleteness.
 */
void
matrix_image_info_set_size(MatrixImageInfo *matrix_image_info, gssize size)
{
    g_return_if_fail(matrix_image_info != NULL);

    matrix_image_info->size = size;
}

/**
 * matrix_image_info_get_size:
 * @image_info: A #MatrixImageInfo object
 *
 * Get the size of the image described by @image_info.
 *
 * Returns: the image size or -1 if not set
 */
gssize
matrix_image_info_get_size(MatrixImageInfo *matrix_image_info)
{
    g_return_val_if_fail(matrix_image_info != NULL, -1);

    return matrix_image_info->size;
}

/**
 * matrix_image_info_set_height:
 * @image_info: A #MatrixImageInfo object
 * @height: The height of the image, in pixels
 *
 * Set the height of the image described by @image_info.
 *
 * -1 is considered an invalid value by matrix_image_info_get_json_node(), but can be set to
 * indicate incompleteness.
 */
void
matrix_image_info_set_height(MatrixImageInfo *matrix_image_info, gint height)
{
    g_return_if_fail(matrix_image_info != NULL);

    matrix_image_info->height = height;
}

/**
 * matrix_image_info_get_heiht:
 * @image_info: A #MatrixImageInfo object
 *
 * Get the height of the image described by @image_info.
 *
 * Returns: the height, or -1 if not set.
 */
gint
matrix_image_info_get_height(MatrixImageInfo *matrix_image_info)
{
    g_return_val_if_fail(matrix_image_info != NULL, -1);

    return matrix_image_info->height;
}

/**
 * matrix_image_info_set_width:
 * @image_info: A #MatrixImageInfo object
 * @width: the width of the image, in pixels
 *
 * Set the width of the image described by @image_info.
 *
 * -1 is considered an invalid value by matrix_image_info_get_json_node(), but can be set to
 * indicate incompleteness.
 */
void
matrix_image_info_set_width(MatrixImageInfo *matrix_image_info, gint width)
{
    g_return_if_fail(matrix_image_info != NULL);

    matrix_image_info->width = width;
}

/**
 * matrix_image_info_get_width:
 * @image_info: A #MatrixImageInfo object
 *
 * Get the width of the image described by @image_info.
 *
 * Returns: the width, or -1 if not set.
 */
gint
matrix_image_info_get_width(MatrixImageInfo *matrix_image_info)
{
    g_return_val_if_fail(matrix_image_info != NULL, -1);

    return matrix_image_info->height;
}

/**
 * matrix_image_info_set_mimetype:
 * @image_info: A #MatrixImageInfo object
 * @mimetype: (transfer none) (nullable): the MIME type of the image
 *
 * Set the MIME type of the image described by @image_info.
 *
 * NULL is considered an invalid value by matrix_image_info_get_json_node(), but can be set to
 * indicate incompleteness.
 */
void
matrix_image_info_set_mimetype(MatrixImageInfo *matrix_image_info, const gchar *mimetype)
{
    g_return_if_fail(matrix_image_info != NULL);

    g_free(matrix_image_info->mimetype);
    matrix_image_info->mimetype = g_strdup(mimetype);
}

/**
 * matrix_image_info_get_mimetype:
 * @image_info: A #MatrixImageInfo object
 *
 * Get the MIME type of the image described by @image_info.
 *
 * Returns: (transfer none) (nullable): the MIME type, or NULL if not set.
 */
const gchar *
matrix_image_info_get_mimetype(MatrixImageInfo *matrix_image_info)
{
    g_return_val_if_fail(matrix_image_info != NULL, NULL);

    return matrix_image_info->mimetype;
}

/**
 * matrix_image_info_set_from_json:
 * @image_info: A #MatrixImageInfo object
 * @json_data: (nullable): a #JsonNode object
 *
 * Load the data in @json_data into the fields of @image_info_info.  @json_data must hold a valid Matrix
 * image info object with a size and mimetype fields.
 */
void
matrix_image_info_set_from_json(MatrixImageInfo *image_info, JsonNode *json_data)
{
    JsonObject *root = NULL;
    JsonNode * node;

    root = json_node_get_object(json_data);

    if ((node = json_object_get_member(root, "w"))) {
        image_info->width = json_node_get_int(node);
    } else if (DEBUG) {
        g_warning("w is missing from an ImageInfo");
    }

    if ((node = json_object_get_member(root, "h"))) {
        image_info->height = json_node_get_int(node);
    } else if (DEBUG) {
        g_warning("h is missing from an ImageInfo");
    }

    if ((node = json_object_get_member(root, "size"))) {
        image_info->size = json_node_get_int(node);
    } else if (DEBUG) {
        g_warning("size is missing from an ImageInfo");
    }

    if ((node = json_object_get_member(root, "mimetype"))) {
        g_free(image_info->mimetype);
        image_info->mimetype = g_strdup(json_node_get_string(node));
    } else if (DEBUG) {
        g_warning("mimetype is missing from an ImageInfo");
    }
}

/**
 * matrix_image_info_get_json_node:
 * @image_info: a #MatrixImageInfo object
 * @error: (nullable): a #GError, or NULL to ignore
 *
 * Convert @image_info to a #JsonNode.  If the file size is negative or the MIME type is not set,
 * this function returns NULL and @error is set to #MATRIX_ERROR_INCOMPLETE.
 *
 * Returns: (transfer full) (nullable): a #JsonNode with a valid Matrix image info object, or NULL
 *     if any of the fields are invalid.
 */
JsonNode *
matrix_image_info_get_json_node(MatrixImageInfo *image_info, GError **error)
{
    JsonNode *node;
    JsonObject *obj;

    if ((image_info->size == -1)
        || (image_info->height == -1)
        || (image_info->width == -1)
        || (image_info->mimetype == NULL)) {
        g_set_error(error, MATRIX_ERROR, MATRIX_ERROR_INCOMPLETE,
                    "Won't generate an ImageInfo without all fields set.");

        return NULL;
    }

    node = json_node_new(JSON_NODE_OBJECT);
    obj = json_object_new();
    json_node_set_object(node, obj);

    json_object_set_int_member(obj, "size", image_info->size);
    json_object_set_int_member(obj, "h", image_info->height);
    json_object_set_int_member(obj, "w", image_info->width);
    json_object_set_string_member(obj, "mimetype", image_info->mimetype);

    return node;
}

/**
 * matrix_image_info_differs:
 * @image_info: A #MatrixImageInfo object
 * @other: A #MatrixImageInfo object to compare @image_info with
 *
 * Check if the two ImageInfo objects are identical (ie. hold the same data).
 *
 * Returns: TRUE if the two objects hold the same values, FALSE otherwise
 */
gboolean
matrix_image_info_differs(MatrixImageInfo *image_info, MatrixImageInfo *other)
{
    // If we compare image_info with itself, they will obviously not differ
    if (G_UNLIKELY(image_info == other)) {
        return FALSE;
    }

    // If we compare NULL and non-NULL values, it obviously differs
    if ((image_info == NULL) || (other == NULL)) {
        return TRUE;
    }

    // At this point, we are definitely comparing two different structs,
    // let’s compare field by field

    return ((image_info->size == other->size) ||
            (image_info->width == other->width) ||
            (image_info->height == other->height) ||
            (g_strcmp0(image_info->mimetype, other->mimetype) != 0));
}

struct _MatrixVideoInfo {
    gssize size;
    gchar *mimetype;
    gint duration;
    gint width;
    gint height;
    gchar *thumbnail_url;
    MatrixImageInfo *thumbnail_info;

    volatile int refcount;
};

/**
 * MatrixVideoInfo: (ref-func matrix_video_info_ref) (unref-func matrix_video_info_unref)
 *
 * Information about a video file referred to by an URL.
 */
G_DEFINE_BOXED_TYPE(MatrixVideoInfo, matrix_video_info, (GBoxedCopyFunc)matrix_video_info_ref, (GBoxedFreeFunc)matrix_video_info_unref);

/**
 * matrix_video_info_new:
 *
 * Create a new #MatrixVideoInfo object with a reference count of 1.
 */
MatrixVideoInfo *
matrix_video_info_new(void)
{
    MatrixVideoInfo *ret = g_new0(MatrixVideoInfo, 1);

    ret->refcount = 1;
    ret->size = -1;
    ret->duration = -1;
    ret->width = -1;
    ret->height = -1;

    return ret;
}

/**
 * matrix_video_info_ref:
 * @video_info: a #MatrixVideoInfo object
 *
 * Increment reference count of @video_info by 1.
 *
 * Returns: (transfer full): @video_info
 */
MatrixVideoInfo *
matrix_video_info_ref(MatrixVideoInfo *matrix_video_info)
{
    g_return_val_if_fail(matrix_video_info != NULL, NULL);

    matrix_video_info->refcount++;

    return matrix_video_info;
}

static void
matrix_video_info_destroy(MatrixVideoInfo *video_info)
{
    g_free(video_info->mimetype);
    g_free(video_info->thumbnail_url);
    matrix_image_info_unref(video_info->thumbnail_info);
}

/**
 * matrix_video_info_unref:
 * @video_info: (transfer full): a #MatrixVideoInfo object
 *
 * Decrement reference count on @video_info by 1.
 *
 * If reference count reaches 0, the object is freed.
 */
void
matrix_video_info_unref(MatrixVideoInfo *matrix_video_info)
{
    g_return_if_fail(matrix_video_info != NULL);

    if (--(matrix_video_info->refcount) == 0) {
        matrix_video_info_destroy(matrix_video_info);
        g_free(matrix_video_info);
    }
}

/**
 * matrix_video_info_set_size:
 * @video_info: a #MatrixVideoInfo object
 * @size: the size of the file, in bytes
 *
 * Set the size of the video file described by @video_info.
 *
 * -1 is considered an invalid value by matrix_video_info_get_json_node(), but can be set to
 * indicate incompleteness.
 */
void
matrix_video_info_set_size(MatrixVideoInfo *matrix_video_info, gsize size)
{
    g_return_if_fail(matrix_video_info != NULL);

    matrix_video_info->size = size;
}

/**
 * matrix_video_info_get_size:
 * @video_info: a #MatrixVideoInfo object
 *
 * Get the size of the video file described by @video_info, in bytes.
 *
 * Returns: the file size
 */
gsize
matrix_video_info_get_size(MatrixVideoInfo *matrix_video_info)
{
    g_return_val_if_fail(matrix_video_info != NULL, -1);

    return matrix_video_info->size;
}

/**
 * matrix_video_info_set_mimetype:
 * @video_info: a #MatrixVideoInfo object
 * @mimetype: (transfer none): a MIME type
 *
 * Set the MIME type of the video file described by @video_info.
 *
 * NULL is considered an invalid value by matrix_video_info_get_json_node(), but can be set to
 * indicate incompleteness.
 */
void
matrix_video_info_set_mimetype(MatrixVideoInfo *matrix_video_info, const gchar *mimetype)
{
    g_return_if_fail(matrix_video_info != NULL);

    g_free(matrix_video_info->mimetype);
    matrix_video_info->mimetype = g_strdup(mimetype);
}

/**
 * matrix_video_info_get_mimetype:
 * @video_info: a #MatrixVideoInfo object
 *
 * Get the MIME type of the video file described by @video_info.
 *
 * Returns: (transfer none) (nullable): the MIME type, or %NULL if not set
 */
const gchar *
matrix_video_info_get_mimetype(MatrixVideoInfo *matrix_video_info)
{
    g_return_val_if_fail(matrix_video_info != NULL, NULL);

    return matrix_video_info->mimetype;
}

/**
 * matrix_video_info_set_duration:
 * @video_info: a #MatrixVideoInfo object
 * @duration: the duration of the video, in seconds
 *
 * Set the duration of the video file described by @video_info.
 *
 * -1 is considered an invalid value by matrix_video_info_get_json_node(), but can be set to
 * indicate incompleteness.
 */
void
matrix_video_info_set_duration(MatrixVideoInfo *matrix_video_info, gint duration)
{
    g_return_if_fail(matrix_video_info != NULL);

    matrix_video_info->duration = duration;
}

/**
 * matrix_video_info_get_duration:
 * @video_info: a #MatrixVideoInfo object
 *
 * Get the duration of the video file described by @video_info, in seconds.
 *
 * Returns: the duration
 */
gint
matrix_video_info_get_duration(MatrixVideoInfo *matrix_video_info)
{
    g_return_val_if_fail(matrix_video_info != NULL, -1);

    return matrix_video_info->duration;
}

/**
 * matrix_video_info_set_width:
 * @video_info: a #MatrixVideoInfo object
 * @width: the width of the video, in pixels
 *
 * Set the width of the video file described by @video_info.
 *
 * -1 is considered an invalid value by matrix_video_info_get_json_node(), but can be set to
 * indicate incompleteness.
 */
void
matrix_video_info_set_width(MatrixVideoInfo *matrix_video_info, gint width)
{
    g_return_if_fail(matrix_video_info != NULL);

    matrix_video_info->width = width;
}

/**
 * matrix_video_info_get_width:
 * @video_info: a #MatrixVideoInfo object
 *
 * Get the width of the video file described by @video_info, in pixels.
 *
 * Returns: the video width
 */
gint
matrix_video_info_get_width(MatrixVideoInfo *matrix_video_info)
{
    g_return_val_if_fail(matrix_video_info != NULL, -1);

    return matrix_video_info->width;
}

/**
 * matrix_video_info_set_height:
 * @video_info: a #MatrixVideoInfo object
 * @height: the height of the video, in pixels
 *
 * Set the width of the video file described by @video_info.
 *
 * -1 is considered an invalid value by matrix_video_info_get_json_node(), but can be set to
 * indicate incompleteness.
 */
void
matrix_video_info_set_height(MatrixVideoInfo *matrix_video_info, gint height)
{
    g_return_if_fail(matrix_video_info != NULL);

    matrix_video_info->height = height;
}

/**
 * matrix_video_info_get_height:
 * @video_info: a #MatrixVideoInfo object
 *
 * Get the height of the video file described by @video_info, in pixels.
 *
 * Returns: the video height
 */
gint
matrix_video_info_get_height(MatrixVideoInfo *matrix_video_info)
{
    g_return_val_if_fail(matrix_video_info != NULL, -1);

    return matrix_video_info->height;
}

/**
 * matrix_video_info_set_thumbnail_url:
 * @video_info: a #MatrixVideoInfo object
 * @thumbnail_url: (transfer none): the URL of the thumbnail image for the video
 *
 * Set the thumbnail image URL for the video described by @video_info.
 */
void
matrix_video_info_set_thumbnail_url(MatrixVideoInfo *matrix_video_info, const gchar *thumbnail_url)
{
    g_return_if_fail(matrix_video_info != NULL);

    g_free(matrix_video_info->thumbnail_url);
    matrix_video_info->thumbnail_url = g_strdup(thumbnail_url);
}

/**
 * matrix_video_info_get_thumbnail_url:
 * @video_info: a #MatrixVideoInfo object
 *
 * Get the thumbnail URL for the video described by @video_info.
 *
 * Returns: (transfer none) (nullable): the thumbnail URL
 */
const gchar *
matrix_video_info_get_thumbnail_url(MatrixVideoInfo *matrix_video_info)
{
    g_return_val_if_fail(matrix_video_info != NULL, NULL);

    return matrix_video_info->thumbnail_url;
}

/**
 * matrix_video_info_set_thumbnail_info:
 * @video_info: a #MatrixVideoInfo object
 * @thumbnail_info: (transfer none): a #MatrixImageInfo object describing the video’s thumbnail
 *     image
 *
 * Set the image info for the video’s thumbnail image, referred to by the URL set with
 * matrix_video_info_set_thumbnail_url().
 */
void
matrix_video_info_set_thumbnail_info(MatrixVideoInfo *matrix_video_info, const MatrixImageInfo *thumbnail_info)
{
    g_return_if_fail(matrix_video_info != NULL);

    matrix_image_info_unref(matrix_video_info->thumbnail_info);
    matrix_video_info->thumbnail_info = matrix_image_info_ref((MatrixImageInfo *)thumbnail_info);
}

/**
 * matrix_video_info_get_thumbnail_info:
 * @video_info: a #MatrixVideoInfo object
 *
 * Get the image info for the video’s thumbnail image, referred to by the URL that can be
 * retrieved with matrix_video_info_get_thumbnail_url().
 *
 * Returns: (transfer none): a #MatrixImageInfo object, or %NULL if not set
 */
const MatrixImageInfo *
matrix_video_info_get_thumbnail_info(MatrixVideoInfo *matrix_video_info)
{
    g_return_val_if_fail(matrix_video_info != NULL, NULL);

    return matrix_video_info->thumbnail_info;
}

/**
 * matrix_video_info_set_from_json:
 * @video_info: A #MatrixVideoInfo object
 * @json_data: (nullable): a #JsonNode object
 *
 * Load the data in @json_data into the fields of @video_info.  @json_data must hold a valid
 * Matrix video info object with a size, duration, width, height, and mimetype fields.
 */
void
matrix_video_info_set_from_json(MatrixVideoInfo *video_info, JsonNode *json_data)
{
    JsonObject *root = NULL;
    JsonNode *node;

    root = json_node_get_object(json_data);

    if ((node = json_object_get_member(root, "size"))) {
        video_info->size = json_node_get_int(node);
    } else if (DEBUG) {
        g_warning("size is missing from an VideoInfo");
    }

    if ((node = json_object_get_member(root, "mimetype"))) {
        g_free(video_info->mimetype);
        video_info->mimetype = g_strdup(json_node_get_string(node));
    } else if (DEBUG) {
        g_warning("mimetype is missing from an VideoInfo");
    }

    if ((node = json_object_get_member(root, "duration"))) {
        video_info->duration = json_node_get_int(node);
    } else if (DEBUG) {
        g_warning("duration is missing from an VideoInfo");
    }

    if ((node = json_object_get_member(root, "w"))) {
        video_info->width = json_node_get_int(node);
    } else if (DEBUG) {
        g_warning("w is missing from a VideoInfo");
    }

    if ((node = json_object_get_member(root, "h"))) {
        video_info->height = json_node_get_int(node);
    } else if (DEBUG) {
        g_warning("h is missing from a VideoInfo");
    }

    if ((node = json_object_get_member(root, "thumbnail_url"))) {
        g_free(video_info->thumbnail_url);
        video_info->thumbnail_url = g_strdup(json_node_get_string(node));
    }

    if ((node = json_object_get_member(root, "thumbnail_info"))) {
        matrix_image_info_unref(video_info->thumbnail_info);
        video_info->thumbnail_info = matrix_image_info_new();
        matrix_image_info_set_from_json(video_info->thumbnail_info, node);
    }
}

/**
 * matrix_video_info_get_json_node:
 * @video_info: a #MatrixImageInfo object
 * @error: (nullable): a #GError, or NULL to ignore
 *
 * Convert @video_info to a #JsonNode.  If the file size, duration, width, or height is negative,
 * or the MIME type is not set, this function returns NULL and @error is set to
 * #MATRIX_ERROR_INCOMPLETE.
 *
 * Returns: (transfer full) (nullable): a #JsonNode with a valid Matrix video info object, or NULL
 *     if any of the fields are invalid.
 */
JsonNode *
matrix_video_info_get_json_node(MatrixVideoInfo *video_info, GError **error)
{
    JsonNode *node;
    JsonObject *obj;

    if ((video_info->size == -1)
        || (video_info->mimetype == NULL)
        || (video_info->duration == -1)
        || (video_info->width == -1)
        || (video_info->height == -1)) {
        g_set_error(error, MATRIX_ERROR, MATRIX_ERROR_INCOMPLETE,
                    "Won't generate a VideoInfo without all the fields set.");
        return NULL;
    }

    node = json_node_new(JSON_NODE_OBJECT);
    obj = json_object_new();
    json_node_set_object(node, obj);

    json_object_set_int_member(obj, "size", video_info->size);
    json_object_set_string_member(obj, "mimetype", video_info->mimetype);
    json_object_set_int_member(obj, "duration", (video_info->duration));
    json_object_set_int_member(obj, "w", video_info->width);
    json_object_set_int_member(obj, "h", video_info->height);

    if(video_info->thumbnail_url && video_info->thumbnail_info) {
        GError *here_error = NULL;
        JsonNode *thumbnail_info = matrix_image_info_get_json_node(video_info->thumbnail_info, &here_error);

        if (here_error) {
            g_propagate_error(error, here_error);

            json_node_free(node);

            return NULL;
        }

        json_object_set_string_member(obj, "thumbnail_url", video_info->thumbnail_url);
        json_object_set_member(obj, "thumbnail_info", thumbnail_info);

        json_node_unref(thumbnail_info);
    }

    return node;
}
