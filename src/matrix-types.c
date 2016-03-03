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

#include "matrix-types.h"
#include "matrix-enumtypes.h"
#include "matrix-glib.h"
#include "utils.h"

/**
 * SECTION:matrix-types
 * @title: Generic types
 * @short_description: Generic types for Matrix GLib SDK calls
 *
 * These are the generic types used by many SDK calls for
 * communication with the homeserver.
 */

/**
 * MatrixError:
 * @MATRIX_ERROR_NONE: no error
 * @MATRIX_ERROR_COMMUNICATION_ERROR: there was a problem in
 *                                    communication (e.g. connection
 *                                    error)
 * @MATRIX_ERROR_INCOMPLETE: the passed/generated data is incomplete
 * @MATRIX_ERROR_BAD_REQUEST: the request is invalid
 * @MATRIX_ERROR_BAD_RESPONSE: malformed response, or the response is
 *                             not a JSON object
 * @MATRIX_ERROR_INVALID_ROOM_ID: the provided string doesn’t contain
 *                                a valid room ID
 * @MATRIX_ERROR_INVALID_TYPE: the provided type is invalid
 * @MATRIX_ERROR_UNSUPPORTED: the operation is unsupported
 * @MATRIX_ERROR_INVALID_FORMAT: the format of the JSON node is
 *                               invalid (e.g. it is an array instead
 *                               of an object)
 * @MATRIX_ERROR_UNKNOWN_VALUE: the response from the Matrix.org
 *                              server contains a value unknown to
 *                              this library. These should be reported
 *                              to the Matrix GLib SDK developers
 * @MATRIX_ERROR_M_MISSING_TOKEN: authorization token is missing from
 *                                the request
 * @MATRIX_ERROR_M_FORBIDDEN: access was forbidden (e.g. due to a
 *                            missing/invalid token, or using a bad
 *                            password during login)
 * @MATRIX_ERROR_M_UNKNOWN: an error unknown to the Matrix homeserver
 * @MATRIX_ERROR_M_UNKNOWN_TOKEN: the token provided is not known for
 *                                the homeserver
 * @MATRIX_ERROR_M_NOT_JSON: illegal request, the content is not valid
 *                           JSON
 * @MATRIX_ERROR_M_UNRECOGNIZED: the homeserver didn't understand the
 *                               request
 * @MATRIX_ERROR_M_UNAUTHORIZED: the request is unauthorized
 * @MATRIX_ERROR_M_BAD_JSON: the JSON data is not in the required
 *                           format
 * @MATRIX_ERROR_M_USER_IN_USE: the specified username is in use
 * @MATRIX_ERROR_M_ROOM_IN_USE: the specified room is in use
 * @MATRIX_ERROR_M_BAD_PAGINATION: invalid pagination parameters
 * @MATRIX_ERROR_M_BAD_STATE: invalid state event
 * @MATRIX_ERROR_M_NOT_FOUND: the requested resource is not found
 * @MATRIX_ERROR_M_GUEST_ACCESS_FORBIDDEN: guest access was requested,
 *                                         but it is forbidden
 * @MATRIX_ERROR_M_LIMIT_EXCEEDED: the request was rate limited
 * @MATRIX_ERROR_M_CAPTCHA_NEEDED: a captcha is needed to continue
 * @MATRIX_ERROR_M_CAPTCHA_INVALID: the provided captcha is invalid
 * @MATRIX_ERROR_M_MISSING_PARAM: a parameter is missing from the
 *                                request
 * @MATRIX_ERROR_M_TOO_LARGE: the request data is too large
 * @MATRIX_ERROR_M_EXCLUSIVE: the desired user ID is in an exclusive
 *                            namespace claimed by an application
 *                            server
 * @MATRIX_ERROR_M_THREEPID_AUTH_FAILED: 3rd party authentication
 *                                       failed
 * @MATRIX_ERROR_M_THREEPID_IN_USE: the provided 3rd party ID is
 *                                  already in use
 * @MATRIX_ERROR_M_INVALID_USERNAME: the given username is invalid
 * @MATRIX_ERROR_UNSPECIFIED: no error code was sent by the
 *                            homeserver. If you see this error, that
 *                            usually indicates a homeserver bug
 * @MATRIX_ERROR_UNKNOWN_ERROR: an error unknown to this library
 *
 * Value mappings from Matrix.org API error codes
 * (e.g. <code>M_MISSING_TOKEN</code>). They should be set
 * automatically by API calls, if the response contains an error code.
 */

/**
 * MATRIX_ERROR:
 *
 * Error domain for Matrix GLib SDK. See #GError for more information
 * on error domains.
 */

/**
 * matrix_error_quark:
 *
 * Gets the Matrix error #GQuark
 */
G_DEFINE_QUARK(matrix-error-quark, matrix_error);

/**
 * MatrixRoomPreset:
 * @MATRIX_ROOM_PRESET_NONE: no preset
 * @MATRIX_ROOM_PRESET_PRIVATE: preset for private rooms
 * @MATRIX_ROOM_PRESET_TRUSTED_PRIVATE: same as private rooms, but all
 *                                      users get the same power level
 *                                      as the room creator
 * @MATRIX_ROOM_PRESET_PUBLIC: preset for public rooms
 *
 * Preset values for matrix_api_create_room() calls.
 */

/**
 * MatrixRoomVisibility:
 * @MATRIX_ROOM_VISIBILITY_DEFAULT: use a server-assigned value
 *                                  (usually <code>private</code>
 * @MATRIX_ROOM_VISIBILITY_PUBLIC: make the room visible in the public
 *                                 room list
 * @MATRIX_ROOM_VISIBILITY_PRIVATE: hide the room from the public room
 *                                  list
 *
 * Visibility values for room creation. Not to be confused with join
 * rules.
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
 * MatrixPresence:
 * @MATRIX_PRESENCE_UNKNOWN: user's presence is unknown
 * @MATRIX_PRESENCE_ONLINE: user is online
 * @MATRIX_PRESENCE_OFFLINE: user is offline
 * @MATRIX_PRESENCE_UNAVAILABLE: user is unavailable (i.e. busy)
 * @MATRIX_PRESENCE_FREE_FOR_CHAT: user is free for chat
 *
 * Presence values for matrix_api_set_user_presence() and other
 * presence related queries.
 */

/**
 * MatrixPusherKind:
 * @MATRIX_PUSHER_KIND_OVERRIDE: highest priority rules
 * @MATRIX_PUSHER_KIND_SENDER: for (unencrypted) messages that match
 *                             certain patterns
 * @MATRIX_PUSHER_KIND_ROOM: for all messages for a given room. The
 *                           rule ID of a room rule is always the ID
 *                           of the room that it affects
 * @MATRIX_PUSHER_KIND_CONTENT: for messages from a specific Matrix
 *                              user ID. The rule ID of such rules is
 *                              always the Matrix ID of the user whose
 *                              messages they'd apply to
 * @MATRIX_PUSHER_KIND_UNDERRIDE: lowest priority rules
 *
 * Pusher types.
 */

/**
 * MatrixPusherConditionKind:
 * @MATRIX_PUSHER_CONDITION_KIND_EVENT_MATCH: glob pattern match on a
 *                                            field of the
 *                                            event. Requires a
 *                                            <code>key</code> and a
 *                                            <code>pattern</code>
 *                                            parameter
 * @MATRIX_PUSHER_CONDITION_KIND_PROFILE_TAG: matches the profile tag
 *                                            of the device that the
 *                                            notification would be
 *                                            delivered to. Requires a
 *                                            <code>profile_tag</code>
 *                                            parameter
 * @MATRIX_PUSHER_CONDITION_KIND_CONTAINS_DISPLAY_NAME: matches
 *                                                      unencrypted
 *                                                      messages where
 *                                                      the content's
 *                                                      body contains
 *                                                      the owner's
 *                                                      display name
 *                                                      in that room.
 * @MATRIX_PUSHER_CONDITION_KIND_ROOM_MEMBER_COUNT: matches the
 *                                                  current number of
 *                                                  members in the
 *                                                  room. Requires an
 *                                                  <code>is</code>
 *                                                  parameter, which
 *                                                  must be an
 *                                                  integer,
 *                                                  optionally
 *                                                  prefixed by
 *                                                  <code>==</code>,
 *                                                  <code>&lt;</code>,
 *                                                  <code>&gt;</code>,
 *                                                  <code>&lt;=</code>
 *                                                  or
 *                                                  <code>&gt;=</code>. If
 *                                                  the prefix is
 *                                                  omitted, it
 *                                                  defaults to
 *                                                  <code>==</code>
 *
 * Condition types for pushers.
 */

/**
 * MatrixAccountKind:
 * @MATRIX_ACCOUNT_KIND_DEFAULT: use the server default (usually
 *                               %MATRIX_ACCOUNT_KIND_USER)
 * @MATRIX_ACCOUNT_KIND_USER: normal user
 * @MATRIX_ACCOUNT_KIND_GUEST: guest user
 *
 * User account types.
 */

/**
 * MatrixEventDirection:
 * @MATRIX_EVENT_DIRECTION_FORWARD: List events after the specified
 *                                  one
 * @MATRIX_EVENT_DIRECTION_BACKWARD: List events before the specified
 *                                   one
 *
 * Direction of events when requesting an event context.
 */

/**
 * MatrixReceiptType:
 * @MATRIX_RECEIPT_TYPE_READ: indicate that the message has been read
 *
 * Receipt types of acknowledgment.
 */

/**
 * MatrixFilterRules: (ref-func matrix_filter_rules_ref) (unref-func matrix_filter_rules_unref)
 *
 * An opaque structure to hold filter rules that can be used to filter
 * events in the event stream. It is possible to filter by event type,
 * room and sender ID.
 */

struct _MatrixFilterRules {
    guint limit;
    GList *rooms;
    GList *excluded_rooms;
    GList *senders;
    GList *excluded_senders;
    GList *types;
    GList *excluded_types;
    guint refcount;
};

G_DEFINE_BOXED_TYPE(MatrixFilterRules, matrix_filter_rules,
                    (GBoxedCopyFunc)matrix_filter_rules_ref,
                    (GBoxedFreeFunc)matrix_filter_rules_unref);

/**
 * matrix_filter_rules_new:
 *
 * Create a new #MatrixFilterRules object with reference count of
 * 1.
 *
 * Returns: (transfer full): a new #MatrixFilterRules
 */
MatrixFilterRules *
matrix_filter_rules_new(void)
{
    MatrixFilterRules *rules;

    rules = g_new0(MatrixFilterRules, 1);
    rules->refcount = 1;

    return rules;
}

static void
matrix_filter_rules_free(MatrixFilterRules *rules)
{
    g_list_free_full(rules->rooms, g_free);
    g_list_free_full(rules->excluded_rooms, g_free);
    g_list_free_full(rules->senders, g_free);
    g_list_free_full(rules->excluded_senders, g_free);
    g_list_free_full(rules->types, g_free);
    g_list_free_full(rules->excluded_types, g_free);

    g_free(rules);
}

/**
 * matrix_filter_rules_ref:
 * @rules: a #MatrixFilterRules
 *
 * Increase reference count of @rules by one.
 *
 * Returns: (transfer none): the same #MatrixFilterRules
 */
MatrixFilterRules *
matrix_filter_rules_ref(MatrixFilterRules *rules)
{
    rules->refcount++;

    return rules;
}

/**
 * matrix_filter_rules_unref:
 * @rules: a #MatrixFilterRules
 *
 * Decrease reference count of @rules by one. If reference count
 * reaches zero, @rules is freed.
 */
void
matrix_filter_rules_unref(MatrixFilterRules *rules)
{
    if (--rules->refcount == 0) {
        matrix_filter_rules_free(rules);
    }
}

/**
 * matrix_filter_rules_set_limit:
 * @rules: a #MatrixFilterRules
 * @limit: (in): the maximum number of events to return.
 *
 * Set the maximum number of events to return by the filter. If @limit
 * is <code>0</code>, no limit will be applied.
 */
void
matrix_filter_rules_set_limit(MatrixFilterRules *rules, guint limit)
{
    rules->limit = limit;
}

/**
 * matrix_filter_rules_get_limit:
 * @rules: a #MatrixFilterRules
 *
 * Get the current limit set in @rules.
 *
 * Returns: the limit currently set
 */
guint
matrix_filter_rules_get_limit(MatrixFilterRules *rules)
{
    return rules->limit;
}

/**
 * matrix_filter_rules_set_senders:
 * @rules: a #MatrixFilterRules
 * @senders: (in) (element-type utf8) (transfer full) (allow-none):
 *           a list of Matrix user IDs. Events from these users will
 *           be included in the filtered event list.If %NULL then all
 *           senders are included. See
 *           matrix_filter_rules_add_sender() for wildcarding
 *           possibilities
 *
 * Set the list of user IDs to include in the filtered events. @rules
 * takes ownership of @senders, so it should not be freed nor modified
 * directly after this call.
 */
void
matrix_filter_rules_set_senders(MatrixFilterRules *rules, GList *senders)
{
    g_list_free_full(rules->senders, g_free);
    rules->senders = senders;
}

/**
 * matrix_filter_rules_add_sender:
 * @rules: a #MatrixFilterRules
 * @sender: (in): a Matrix user ID to add to the included senders
 *          list. A <code>*</code> can be used as a wildcard to match
 *          any sequence of characters
 *
 * Add @sender to the list of user IDs to include in the filtered
 * event list. If @sender is already included in the senders list,
 * nothing happens.
 */
void
matrix_filter_rules_add_sender(MatrixFilterRules *rules, const gchar *sender)
{
    g_return_if_fail(sender != NULL);

    if (!g_list_find_custom(rules->senders, sender, (GCompareFunc)g_strcmp0)) {
        rules->senders = g_list_prepend(rules->senders, g_strdup(sender));
    }
}

/**
 * matrix_filter_rules_delete_sender:
 * @rules: a #MatrixFilterRules
 * @sender: (in): the user ID to remove from the senders list
 *
 * Remove @sender from the list of user IDs to include in the filtered
 * event list.
 */
void
matrix_filter_rules_delete_sender(MatrixFilterRules *rules, const gchar *sender)
{
    GList *sender_element;

    g_return_if_fail(sender != NULL);

    while ((sender_element = g_list_find_custom(rules->senders, sender,
                                                (GCompareFunc)g_strcmp0))) {
        rules->senders = g_list_remove_link(rules->senders, sender_element);
        g_list_free_full(sender_element, g_free);
    }
}

/**
 * matrix_filter_rules_get_senders:
 * @rules: a #MatrixFilterRules
 *
 * Get the list of user IDs that will be included in the filtered
 * events.
 *
 * Returns: (element-type utf8) (allow-none) (transfer none): the
 *          list of user IDs. The returned value is owned by @rules
 *          and should not be freed nor modified
 */
const GList *
matrix_filter_rules_get_senders(MatrixFilterRules *rules)
{
    return rules->senders;
}

/**
 * matrix_filter_rules_set_excluded_senders:
 * @rules: a #MatrixFilterRules
 * @senders: (in) (element-type utf8) (transfer full) (allow-none): a
 *           list of Matrix user IDs. Events from these users will be
 *           included in the filtered event list.If %NULL then all
 *           senders are included. See
 *           matrix_filter_rules_add_sender() for wildcarding
 *           possibilities
 *
 * Set the list of Matrix user IDs to exclude from the filtered
 * events. A matching sender will be excluded even if it is listed in
 * the senders list (specified by
 * e.g. matrix_filter_rules_set_senders()). @rules takes ownership of
 * @senders, so it should not be freed nor modified directly after
 * this call.
 */
void
matrix_filter_rules_set_excluded_senders(MatrixFilterRules *rules,
                                         GList *senders)
{
    g_list_free_full(rules->excluded_senders, g_free);
    rules->excluded_senders = senders;
}

/**
 * matrix_filter_rules_add_excluded_sender:
 * @rules: a #MatrixFilterRules
 * @sender: (in): a Matrix user ID to add to the excluded senders
 *          list. See matrix_filter_rules_add_sender() for wildcarding
 *          possibilities
 *
 * Add @sender to the list of user IDs to exclude from the filtered
 * event list. If @sender is already in the excluded senders list,
 * nothing happens.
 */
void
matrix_filter_rules_add_excluded_sender(MatrixFilterRules *rules,
                                        const gchar *sender)
{
    g_return_if_fail(sender != NULL);

    if (!g_list_find_custom(rules->excluded_senders, sender,
                            (GCompareFunc)g_strcmp0)) {
        rules->excluded_senders = g_list_prepend(rules->excluded_senders,
                                                 g_strdup(sender));
    }
}

/**
 * matrix_filter_rules_delete_excluded_sender:
 * @rules: a #MatrixFilterRules
 * @sender: (in): the Matrix user ID to remove from the excluded
 *          senders list
 *
 * Remove @sender from the list of user IDs to exclude from the
 * filtered event list.
 */
void
matrix_filter_rules_delete_excluded_sender(MatrixFilterRules *rules,
                                           const gchar *sender)
{
    GList *sender_element;

    g_return_if_fail(sender != NULL);

    while ((sender_element = g_list_find_custom(rules->excluded_senders, sender,
                                                (GCompareFunc)g_strcmp0))) {
        rules->excluded_senders = g_list_remove_link(rules->excluded_senders,
                                                     sender_element);
        g_list_free_full(sender_element, g_free);
    }
}

/**
 * matrix_filter_rules_get_excluded_senders:
 * @rules: a #MatrixFilterRules
 *
 * Get the list of user IDs that will be excluded in the filtered
 * events.
 *
 * Returns: (element-type utf8) (allow-none) (transfer none): the
 *          list of user IDs to be excluded. The returned value is
 *          owned by @rules and should not be freed nor modified.
 */
const GList *
matrix_filter_rules_get_excluded_senders(MatrixFilterRules *rules)
{
    return rules->excluded_senders;
}

/**
 * matrix_filter_rules_set_rooms:
 * @rules: a #MatrixFilterRules
 * @rooms: (in) (element-type utf8) (transfer full) (allow-none): a
 *         list of room IDs. Events from these rooms will be included
 *         in the filtered event list.If %NULL then all rooms are
 *         included. See matrix_filter_rules_add_sender() for
 *         wildcarding possibilities
 *
 * Set the list of room IDs to include in the filtered events. @rules
 * takes ownership of @rooms, so it should not be freed nor modified
 * directly after this call.
 */
void
matrix_filter_rules_set_rooms(MatrixFilterRules *rules, GList *rooms)
{
    g_list_free_full(rules->rooms, g_free);
    rules->rooms = rooms;
}

/**
 * matrix_filter_rules_add_room:
 * @rules: a #MatrixFilterRules
 * @room: (in): a room ID to add to the included rooms list. See
 *        matrix_filter_rules_add_sender() for wildcarding
 *        possibilities
 *
 * Add @room to the list of room IDs to include in the filtered
 * event list. If @room is already included in the rooms list,
 * nothing happens.
 */
void
matrix_filter_rules_add_room(MatrixFilterRules *rules, const gchar *room)
{
    g_return_if_fail(room != NULL);

    if (!g_list_find_custom(rules->rooms, room, (GCompareFunc)g_strcmp0)) {
        rules->rooms = g_list_prepend(rules->rooms, g_strdup(room));
    }
}

/**
 * matrix_filter_rules_delete_room:
 * @rules: a #MatrixFilterRules
 * @room: (in): the room ID to remove from the rooms list
 *
 * Remove @room from the list of room IDs to include in the filtered
 * event list.
 */
void
matrix_filter_rules_delete_room(MatrixFilterRules *rules, const gchar *room)
{
    GList *room_element;

    g_return_if_fail(room != NULL);

    while ((room_element = g_list_find_custom(rules->rooms, room,
                                              (GCompareFunc)g_strcmp0))) {
        rules->rooms = g_list_remove_link(rules->rooms, room_element);
        g_list_free_full(room_element, g_free);
    }
}

/**
 * matrix_filter_rules_get_rooms:
 * @rules: a #MatrixFilterRules
 *
 * Get the list of room IDs that will be included in the filtered
 * events.
 *
 * Returns: (element-type utf8) (allow-none) (transfer none): the
 *          list of room IDs. The returned value is owned by @rules
 *          and should not be freed nor modified
 */
const GList *
matrix_filter_rules_get_rooms(MatrixFilterRules *rules)
{
    return rules->rooms;
}

/**
 * matrix_filter_rules_set_excluded_rooms:
 * @rules: a #MatrixFilterRules
 * @rooms: (in) (element-type utf8) (transfer full) (allow-none): a
 *         list of room IDs. Events from these rooms will be included
 *         in the filtered event list.If %NULL, then all rooms are
 *         included. See matrix_filter_rules_add_sender() for
 *         wildcarding possibilities
 *
 * Set the list of room IDs to exclude from the filtered events. A
 * matching room will be excluded even if it is listed in the rooms
 * list (specified by e.g. matrix_filter_rules_set_rooms()). @rules
 * takes ownership of @rooms, so it should not be freed nor modified
 * directly after this call.
 */
void
matrix_filter_rules_set_excluded_rooms(MatrixFilterRules *rules, GList *rooms)
{
    g_list_free_full(rules->excluded_rooms, g_free);
    rules->excluded_rooms = rooms;
}

/**
 * matrix_filter_rules_add_excluded_room:
 * @rules: a #MatrixFilterRules
 * @room: (in): a room ID to add to the excluded rooms list. See
 *        matrix_filter_rules_add_sender() for wildcarding
 *        possibilities
 *
 * Add @room to the list of room IDs to exclude from the filtered
 * event list. If @room is already in the excluded rooms list, nothing
 * happens.
 */
void
matrix_filter_rules_add_excluded_room(MatrixFilterRules *rules,
                                      const gchar *room)
{
    g_return_if_fail(room != NULL);

    if (!g_list_find_custom(rules->excluded_rooms, room,
                            (GCompareFunc)g_strcmp0)) {
        rules->excluded_rooms = g_list_prepend(rules->excluded_rooms,
                                               g_strdup(room));
    }
}

/**
 * matrix_filter_rules_delete_excluded_room:
 * @rules: a #MatrixFilterRules
 * @room: (in): the room ID to remove from the excluded rooms list
 *
 * Remove @room from the list of room IDs to exclude from the filtered
 * event list.
 */
void
matrix_filter_rules_delete_excluded_room(MatrixFilterRules *rules,
                                         const gchar *room)
{
    GList *room_element;

    g_return_if_fail(room != NULL);

    while ((room_element = g_list_find_custom(rules->excluded_rooms, room,
                                              (GCompareFunc)g_strcmp0))) {
        rules->excluded_rooms = g_list_remove_link(rules->excluded_rooms,
                                                   room_element);
        g_list_free_full(room_element, g_free);
    }
}

/**
 * matrix_filter_rules_get_excluded_rooms:
 * @rules: a #MatrixFilterRules
 *
 * Get the list of room IDs that will be excluded in the filtered
 * events.
 *
 * Returns: (element-type utf8) (allow-none) (transfer none): the
 *          list of room IDs to be excluded. The returned value is
 *          owned by @rules and should not be freed nor modified.
 */
const GList *
matrix_filter_rules_get_excluded_rooms(MatrixFilterRules *rules)
{
    return rules->excluded_rooms;
}

/**
 * matrix_filter_rules_set_types:
 * @rules: a #MatrixFilterRules
 * @types: (in) (element-type utf8) (transfer full) (allow-none): a
 *         list of event types to include. If %NULL then all event
 *         types are included. See
 *         matrix_filter_rules_add_sender() for wildcarding
 *         possibilities
 *
 * Set the list of event types to be included in the filtered events.
 */
void
matrix_filter_rules_set_types(MatrixFilterRules *rules, GList *types)
{
    g_list_free_full(rules->types, g_free);
    rules->types = types;
}

/**
 * matrix_filter_rules_add_type:
 * @rules: a #MatrixFilterRules
 * @type: (in): an event type to add to the list of included
 *        events. See matrix_filter_rules_add_sender() for wildcarding
 *        possibilities
 *
 * Add @type to the list of event types to include in the filtered
 * event list. If @type is already included in the types list, nothing
 * happens.
 */
void
matrix_filter_rules_add_type(MatrixFilterRules *rules, const gchar *event_type)
{
    g_return_if_fail(event_type != NULL);

    if (g_list_find_custom(rules->types, event_type, (GCompareFunc)g_strcmp0)) {
        rules->types = g_list_prepend(rules->types, g_strdup(event_type));
    }
}

/**
 * matrix_filter_rules_delete_type:
 * @rules: a #MatrixFilterRules
 * @type: (in): a type to remove from the list of included event
 *        types. See matrix_filter_rules_add_sender() for
 *        wildcarding possibilities
 *
 * Remove @type from the list of excluded event type list.
 */
void
matrix_filter_rules_delete_type(MatrixFilterRules *rules,
                                const gchar *event_type)
{
    GList *type_element;

    g_return_if_fail(event_type != NULL);

    while ((type_element = g_list_find_custom(rules->types, event_type,
                                              (GCompareFunc)g_strcmp0))) {
        rules->types = g_list_remove_link(rules->types, type_element);
        g_list_free_full(type_element, g_free);
    }
}

/**
 * matrix_filter_rules_get_types:
 * @rules: a #MatrixFilterRules
 *
 * Get the list of event types that will be included in the filtered
 * events.
 *
 * Returns: (element-type utf8) (allow-none) (transfer none): the
 *          list of event types. The returned values is owned by
 *          @rules and should not be freed nor modified
 */
const GList *
matrix_filter_rules_get_types(MatrixFilterRules *rules)
{
    return rules->types;
}

/**
 * matrix_filter_rules_set_excluded_types:
 * @rules: a #MatrixFilterRules
 * @types: (in) (element-type utf8) (transfer full) (allow-none): a
 *         list of event types to exclude. If %NULL then no event
 *         types are excluded. A matching type will be excluded even
 *         if it is listed in the included types. See
 *         matrix_filter_rules_add_sender() for wildcarding
 *         possibilities
 *
 * Set the list of event types to be excluded from the filtered
 * events. A matching type will be excluded even if it is listed in
 * the types list (specified by
 * e.g. matrix_filter_rules_set_types()).
 */
void
matrix_filter_rules_set_excluded_types(MatrixFilterRules *rules, GList *types)
{
    g_list_free_full(rules->excluded_types, g_free);
    rules->excluded_types = types;
}

/**
 * matrix_filter_rules_add_excluded_type:
 * @rules: a #MatrixFilterRules
 * @type: (in): an event type to add to the excluded event type
 *        list. See matrix_filter_rules_add_sender() for wildcarding
 *        possibilities
 *
 * Add @type to the list of excluded event types.
 */
void
matrix_filter_rules_add_excluded_type(MatrixFilterRules *rules,
                                      const gchar *event_type)
{
    g_return_if_fail(event_type != NULL);

    if (!g_list_find_custom(rules->excluded_types, event_type,
                            (GCompareFunc)g_strcmp0)) {
        rules->excluded_types = g_list_prepend(rules->excluded_types,
                                               g_strdup(event_type));
    }
}

/**
 * matrix_filter_rules_delete_excluded_type:
 * @rules: a #MatrixFilterRules
 * @type: (in): the event type to be removed from the excluded types
 *        list
 *
 * Remove @type from the list of event types to be excluded from the
 * filtered event list.
 */
void
matrix_filter_rules_delete_excluded_type(MatrixFilterRules *rules,
                                         const gchar *event_type)
{
    GList *type_element;

    g_return_if_fail(event_type != NULL);

    while ((type_element = g_list_find_custom(rules->excluded_types, event_type,
                                              (GCompareFunc)g_strcmp0))) {
        rules->excluded_types = g_list_remove_link(rules->excluded_types,
                                                   type_element);
        g_list_free_full(type_element, g_free);
    }
}

/**
 * matrix_filter_rules_get_excluded_types:
 * @rules: a #MatrixFilterRules
 *
 * Get the list of event types that will be excluded from the filtered
 * events.
 *
 * Returns: (element-type utf8) (allow-none) (transfer none): the
 *          list of event types. The returned value is owned by
 *          @rules and should not be freed nor modified
 */
const GList *
matrix_filter_rules_get_excluded_types(MatrixFilterRules *rules)
{
    return rules->excluded_types;
}

static void
json_add_string(gchar *str, JsonBuilder *builder)
{
    json_builder_add_string_value(builder, str);
}

/**
 * matrix_filter_rules_get_json_node:
 * @rules: a #MatrixFilterRules
 *
 * Gets the #JsonNode representation of this filtering ruleset.
 *
 * Returns: (transfer full): the JSON representation of the filtering
 *          data as a #JsonNode
 */
JsonNode *
matrix_filter_rules_get_json_node(MatrixFilterRules *rules)
{
    JsonBuilder *builder;
    JsonNode *node;

    builder = json_builder_new();
    json_builder_begin_object(builder);

    json_builder_set_member_name(builder, "limit");
    json_builder_add_int_value(builder, rules->limit);

    if (rules->rooms) {
        json_builder_set_member_name(builder, "rooms");
        json_builder_begin_array(builder);
        g_list_foreach(rules->rooms, (GFunc)json_add_string, builder);
        json_builder_end_array(builder);
    }

    if (rules->excluded_rooms) {
        json_builder_set_member_name(builder, "not_rooms");
        json_builder_begin_array(builder);
        g_list_foreach(rules->excluded_rooms, (GFunc)json_add_string, builder);
        json_builder_end_array(builder);
    }

    if (rules->senders) {
        json_builder_set_member_name(builder, "senders");
        json_builder_begin_array(builder);
        g_list_foreach(rules->senders, (GFunc)json_add_string, builder);
        json_builder_end_array(builder);
    }

    if (rules->excluded_senders) {
        json_builder_set_member_name(builder, "not_senders");
        json_builder_begin_array(builder);
        g_list_foreach(rules->excluded_senders,
                       (GFunc)json_add_string, builder);
        json_builder_end_array(builder);
    }

    if (rules->types) {
        json_builder_set_member_name(builder, "types");
        json_builder_begin_array(builder);
        g_list_foreach(rules->types, (GFunc)json_add_string, builder);
        json_builder_end_array(builder);
    }

    if (rules->excluded_types) {
        json_builder_set_member_name(builder, "not_types");
        json_builder_begin_array(builder);
        g_list_foreach(rules->excluded_types, (GFunc)json_add_string, builder);
        json_builder_end_array(builder);
    }

    json_builder_end_object(builder);

    node = json_builder_get_root(builder);
    g_object_unref(builder);

    return node;
}

/**
 * matrix_filter_rules_get_json_data:
 * @rules: a #MatrixFilterRules
 * @datalen: (out): storage for the the length of the JSON data or
 *           %NULL
 *
 * Gets the string representation of these filtering rules, as a JSON
 * object.
 *
 * Returns: (transfer full): the JSON representation of the filtering
 *          rule data as a string
 */
gchar *
matrix_filter_rules_get_json_data(MatrixFilterRules *rules, gsize *datalen)
{
    JsonGenerator *generator;
    JsonNode *node = matrix_filter_rules_get_json_node(rules);
    gchar *data;

    generator = json_generator_new();
    json_generator_set_root(generator, node);
    json_node_free(node);

    data = json_generator_to_data(generator, datalen);
    g_object_unref(generator);

    return data;
}

/**
 * MatrixRoomFilter:
 *
 * An opaque structure to hold room filters.
 */
struct _MatrixRoomFilter {
    gboolean include_leave;
    MatrixFilterRules *ephemeral;
    MatrixFilterRules *state;
    MatrixFilterRules *timeline;
    guint refcount;
};

G_DEFINE_BOXED_TYPE(MatrixRoomFilter, matrix_room_filter,
                    (GBoxedCopyFunc)matrix_room_filter_ref,
                    (GBoxedFreeFunc)matrix_room_filter_unref);

/**
 * matrix_room_filter_new:
 *
 * Create a new #MatrixRoomFilter object with reference count of 1.
 *
 * Returns: (transfer full): a new #MatrixRoomFilter
 */
MatrixRoomFilter *
matrix_room_filter_new(void)
{
    MatrixRoomFilter *filter;

    filter = g_new0(MatrixRoomFilter, 1);
    filter->refcount = 1;

    return filter;
}

static void
matrix_room_filter_free(MatrixRoomFilter *filter)
{
    if (filter->ephemeral) {
        matrix_filter_rules_unref(filter->ephemeral);
    }

    if (filter->state) {
        matrix_filter_rules_unref(filter->state);
    }

    if (filter->timeline) {
        matrix_filter_rules_unref(filter->timeline);
    }

    g_free(filter);
}

/**
 * matrix_room_filter_ref:
 * @filter: a #MatrixRoomFilter
 *
 * Increase reference count of @filter by one.
 *
 * Returns: (transfer none): the same #MatrixRoomFilter
 */
MatrixRoomFilter *
matrix_room_filter_ref(MatrixRoomFilter *filter)
{
    filter->refcount++;

    return filter;
}

/**
 * matrix_room_filter_unref:
 * @filter: a #MatrixRoomFilter
 *
 * Decrease reference count of @filter by one. If reference count
 * reaches zero, @filter is freed.
 */
void
matrix_room_filter_unref(MatrixRoomFilter *filter)
{
    if (--filter->refcount == 0) {
        matrix_room_filter_free(filter);
    }
}

/**
 * matrix_room_filter_set_include_leave:
 * @filter: a #MatrixRoomFilter
 * @include_leave: %TRUE if events from left rooms should be included
 *
 * Sets if events for rooms that the user has left should be included
 * in the filtered event list.
 */
void
matrix_room_filter_set_include_leave(MatrixRoomFilter *filter,
                                         gboolean include_leave)
{
    filter->include_leave = include_leave;
}

/**
 * matrix_room_filter_get_include_leave:
 * @filter: a #MatrixRoomFilter
 *
 * If %TRUE, events from rooms that the user has left will be included
 * in the filtered event list.
 *
 * Returns: the value of <code>include_leave</code>
 */
gboolean
matrix_room_filter_get_include_leave(MatrixRoomFilter *filter)
{
    return filter->include_leave;
}

/**
 * matrix_room_filter_set_ephemeral:
 * @filter: a #MatrixRoomFilter
 * @rules: (transfer none): filtering rules for ephemeral events
 *
 * Set filtering rules for ephemeral events, i.e. events that are not
 * recorded in the room history (typing notifications, receipts, etc.)
 * @filter gets a reference on @rules, so the caller may unref it.
 */
void
matrix_room_filter_set_ephemeral(MatrixRoomFilter *filter,
                                 MatrixFilterRules *rules)
{
    if (filter->ephemeral) {
        matrix_filter_rules_unref(filter->ephemeral);
    }

    filter->ephemeral = matrix_filter_rules_ref(rules);
}

/**
 * matrix_room_filter_get_ephemeral:
 * @filter: a #MatrixRoomFilter
 *
 * Gets the filtering rules for ephemeral events. See
 * matrix_room_filter_set_ephemeral() for details.
 *
 * Returns: (transfer none): the filtering rules for ephemeral events.
 */
MatrixFilterRules *
matrix_room_filter_get_ephemeral(MatrixRoomFilter *filter)
{
    return filter->ephemeral;
}

/**
 * matrix_room_filter_set_state:
 * @filter: a #MatrixRoomFilter
 * @rules: (transfer none): filtering rules for state events
 *
 * Sets filtering rules for state events. @filter obtains a reference
 * on @rules, so the caller may unref it safely.
 */
void
matrix_room_filter_set_state(MatrixRoomFilter *filter, MatrixFilterRules *rules)
{
    if (filter->state) {
        matrix_filter_rules_unref(filter->state);
    }

    filter->state = matrix_filter_rules_ref(rules);
}

/**
 * matrix_room_filter_get_state:
 * @filter: a #MatrixRoomFilter
 *
 * Gets the filtering rules for state events.
 *
 * Returns: (transfer none): the filtering rules for state events
 */
MatrixFilterRules *
matrix_room_filter_get_state(MatrixRoomFilter *filter)
{
    return filter->state;
}

/**
 * matrix_room_filter_set_timeline:
 * @filter: a #MatrixRoomFilter
 * @rules: (transfer none): filtering rules for timeline events
 *         (messages and state events from rooms)
 *
 * Set filtering rules for timeline events.
 */
void
matrix_room_filter_set_timeline(MatrixRoomFilter *filter,
                                MatrixFilterRules *rules)
{
    if (filter->timeline) {
        matrix_filter_rules_unref(filter->timeline);
    }

    filter->timeline = matrix_filter_rules_ref(rules);
}

/**
 * matrix_room_filter_get_timeline:
 * @filter: a #MatrixRoomFilter
 *
 * Gets filtering rules for timeline events (see
 * matrix_room_filter_set_timeline() for details.
 *
 * Returns: (transfer none): the filtering rules for timeline events
 */
MatrixFilterRules *
matrix_room_filter_get_timeline(MatrixRoomFilter *filter)
{
    return filter->timeline;
}

/**
 * matrix_room_filter_get_json_node:
 * @filter: a #MatrixRoomFilter
 *
 * Gets the #JsonNode representation of the filtering ruleset.
 *
 * Returns: (transfer full): the JSON representation of the filtering
 *          ruleset as a #JsonNode
 */
JsonNode *
matrix_room_filter_get_json_node(MatrixRoomFilter *filter)
{
    JsonBuilder *builder;
    JsonNode *node;

    builder = json_builder_new();
    json_builder_begin_object(builder);

    json_builder_set_member_name(builder, "include_leave");
    json_builder_add_boolean_value(builder, filter->include_leave);

    if (filter->ephemeral) {
        json_builder_set_member_name(builder, "ephemeral");
        json_builder_add_value(builder,
                               matrix_filter_rules_get_json_node(
                                       filter->ephemeral));
    }

    if (filter->state) {
        json_builder_set_member_name(builder, "state");
        json_builder_add_value(builder,
                               matrix_filter_rules_get_json_node(
                                       filter->state));
    }

    if (filter->timeline) {
        json_builder_set_member_name(builder, "timeline");
        json_builder_add_value(builder,
                               matrix_filter_rules_get_json_node(
                                       filter->timeline));
    }

    json_builder_end_object(builder);

    node = json_builder_get_root(builder);
    g_object_unref(builder);

    return node;
}

/**
 * matrix_room_filter_get_json_data:
 * @filter: a #MatrixRoomFilter
 * @datalen: (out) (allow-none): storage for the length of the JSON
 *           data, or %NULL
 *
 * Get the string representation of the filtering ruleset as a JSON
 * object.
 *
 * Returns: the JSON object representation of the filtering ruleset
 *          data as a string
 */
gchar *
matrix_room_filter_get_json_data(MatrixRoomFilter *filter, gsize *datalen)
{
    JsonGenerator *generator;
    JsonNode *node = matrix_room_filter_get_json_node(filter);
    gchar *data;

    generator = json_generator_new();
    json_generator_set_root(generator, node);
    json_node_free(node);

    data = json_generator_to_data(generator, datalen);
    g_object_unref(generator);

    return data;
}

/**
 * Matrix3PidCredential:
 *
 * An opaque structure to store credentials to use with Identity
 * Server communication.
 */
struct _Matrix3PidCredential {
    gchar *id_server;
    gchar *session_id;
    gchar *client_secret;
    guint refcount;
};

G_DEFINE_BOXED_TYPE(Matrix3PidCredential, matrix_3pid_credential,
                    (GBoxedCopyFunc)matrix_3pid_credential_ref,
                    (GBoxedFreeFunc)matrix_3pid_credential_unref);

/**
 * matrix_3pid_credential_new:
 *
 * Create a new #Matrix3PidCredential object with reference count
 * of 1.
 *
 * Returns: (transfer full): a new #Matrix3PidCredential
 */
Matrix3PidCredential *
matrix_3pid_credential_new(void)
{
    Matrix3PidCredential *credential;

    credential = g_new0(Matrix3PidCredential, 1);
    credential->refcount = 1;

    return credential;
}

static void
matrix_3pid_credential_free(Matrix3PidCredential *credential)
{
    g_free(credential->id_server);
    g_free(credential->session_id);
    g_free(credential->client_secret);
    g_free(credential);
}

/**
 * matrix_3pid_credential_ref:
 * @credential: a #Matrix3PidCredential
 *
 * Increase reference count of @credential by one.
 *
 * Returns: (transfer none): the same #Matrix3PidCredential
 */
Matrix3PidCredential *
matrix_3pid_credential_ref(Matrix3PidCredential *credential)
{
    credential->refcount++;

    return credential;
}

/**
 * matrix_3pid_credential_unref:
 * @credential: a #Matrix3PidCredential
 *
 * Decrease reference count of @credential by one. If reference count
 * reaches zero, @credential gets freed.
 */
void
matrix_3pid_credential_unref(Matrix3PidCredential *credential)
{
    if (--credential->refcount == 0) {
        matrix_3pid_credential_free(credential);
    }
}

/**
 * matrix_3pid_credential_set_id_server:
 * @credential: a #Matrix3PidCredential
 * @id_server: the Identity Server to use
 *
 * Set the Identity Server to use for this credential.
 */
void
matrix_3pid_credential_set_id_server(Matrix3PidCredential *credential,
                                     const gchar *id_server)
{
    g_free(credential->id_server);
    credential->id_server = g_strdup(id_server);
}

/**
 * matrix_3pid_credential_get_id_server:
 * @credential: a #Matrix3PidCredential
 *
 * Get the Identity Server used for this credential.
 *
 * Returns: the Identity Server's name. The returned value is owned by
 *          @credential and should not be freed nor modified
 */
const gchar *
matrix_3pid_credential_get_id_server(Matrix3PidCredential *credential)
{
    return credential->id_server;
}

/**
 * matrix_3pid_credential_set_session_id:
 * @credential: a #Matrix3PidCredential
 * @session_id: the session identifier given by the Identity Server
 *
 * Set the session identifier got from the Identity Server.
 */
void
matrix_3pid_credential_set_session_id(Matrix3PidCredential *credential,
                                      const gchar *session_id)
{
    g_free(credential->session_id);
    credential->session_id = g_strdup(session_id);
}

/**
 * matrix_3pid_credential_get_session_id:
 * @credential: a #Matrix3PidCredential
 *
 * Get the session identifier for this credential.
 *
 * Returns: the session identifier. The returned value is owned by
 *          @credential and should not be freed nor modified.
 */
const gchar *
matrix_3pid_credential_get_session_id(Matrix3PidCredential *credential)
{
    return credential->session_id;
}

/**
 * matrix_3pid_credential_set_client_secret:
 * @credential: a #Matrix3PidCredential
 * @client_secret: the client secret used in the session with the
 *                 Identity Server
 *
 * Set the client secret that was used in the session with the
 * Identity Server.
 */
void
matrix_3pid_credential_set_client_secret(Matrix3PidCredential *credential,
                                         const gchar *client_secret)
{
    g_free(credential->client_secret);
    credential->client_secret = g_strdup(client_secret);
}

/**
 * matrix_3pid_credential_get_client_secret:
 * @credential: a #Matrix3PidCredential
 *
 * Get the client secret that was used in the session with the
 * Identity Server.
 *
 * Returns: the client secret. The returned value is owned by
 *          @credential and should not be freed nor modified.
 */
const gchar *
matrix_3pid_credential_get_client_secret(Matrix3PidCredential *credential)
{
    return credential->client_secret;
}

/**
 * matrix_3pid_credential_get_json_node:
 * @credential: a #Matrix3PidCredential
 * @error: return location for a #GError, or %NULL
 *
 * Get the JSON representation of @credential as a #JsonNode. If any
 * fields of @credential are empty, it will yield an error, as all
 * fields are mandatory. In this case, the return value is %NULL.
 *
 * Returns: (transfer full) (allow-none): the #JsonNode representation
 *          of @credential
 */
JsonNode *
matrix_3pid_credential_get_json_node(Matrix3PidCredential *credential,
                                     GError **error)
{
    JsonBuilder *builder;
    JsonNode *node;

    if (!credential->id_server
        || !credential->session_id
        || !credential->client_secret) {
        g_set_error(error,
                    MATRIX_ERROR, MATRIX_ERROR_INCOMPLETE,
                    "All fields of the 3PID credential must be filled!");

        return NULL;
    }

    builder = json_builder_new();
    json_builder_begin_object(builder);

    json_builder_set_member_name(builder, "id_server");
    json_builder_add_string_value(builder, credential->id_server);

    json_builder_set_member_name(builder, "session_id");
    json_builder_add_string_value(builder, credential->session_id);

    json_builder_set_member_name(builder, "client_secret");
    json_builder_add_string_value(builder, credential->client_secret);

    json_builder_end_object(builder);
    node = json_builder_get_root(builder);
    g_object_unref(builder);

    return node;
}

/**
 * matrix_3pid_credential_get_json_data:
 * @credential: a #Matrix3PidCredential
 * @datalen: (out): storage location for the length of the JSON data,
 *           or %NULL
 * @error: a #GError
 *
 * Get the JSON representation of @credential as a string. If any
 * fields of @credential is %NULL, this function returns %NULL and
 * fills @error wich %MATRIX_ERROR_INCOMPLETE.
 *
 * Returns: (transfer full) (allow-none): the JSON representation of
 *          @credential, or %NULL
 */
gchar *
matrix_3pid_credential_get_json_data(Matrix3PidCredential *credential,
                                     gsize *datalen,
                                     GError **error)
{
    JsonGenerator *generator;
    JsonNode *node;
    gchar *data;

    if ((node = matrix_3pid_credential_get_json_node(
                 credential, error)) == NULL) {
        return NULL;
    }

    generator = json_generator_new();
    json_generator_set_root(generator, node);
    json_node_free(node);

    data = json_generator_to_data(generator, datalen);
    g_object_unref(generator);

    return data;
}

/**
 * MatrixPusher:
 *
 * An opaque structure for pusher rulesets.
 */
struct _MatrixPusher {
    gchar *device_display_name;
    gchar *app_display_name;
    gchar *app_id;
    gboolean append;
    gchar *kind;
    gchar *lang;
    gchar *profile_tag;
    gchar *pushkey;
    JsonNode *data;
    guint refcount;
};

G_DEFINE_BOXED_TYPE(MatrixPusher, matrix_pusher,
                    (GBoxedCopyFunc)matrix_pusher_ref,
                    (GBoxedFreeFunc)matrix_pusher_unref);

/**
 * matrix_pusher_new:
 *
 * Create a new #MatrixPusher object with reference count of 1.
 *
 * Returns: (transfer full): a new #MatrixPusher
 */
MatrixPusher *
matrix_pusher_new(void)
{
    MatrixPusher *pusher;

    pusher = g_new0(MatrixPusher, 1);
    pusher->refcount = 1;

    return pusher;
}

static void
matrix_pusher_free(MatrixPusher *pusher)
{
    g_free(pusher->device_display_name);
    g_free(pusher->app_display_name);
    g_free(pusher->app_id);
    g_free(pusher->kind);
    g_free(pusher->lang);
    g_free(pusher->profile_tag);
    g_free(pusher->pushkey);
    json_node_free(pusher->data);

    g_free(pusher);
}

/**
 * matrix_pusher_ref:
 * @pusher: a #MatrixPusher
 *
 * Increase reference count of @pusher by one.
 *
 * Returns: (transfer none): the same #MatrixPusher
 */
MatrixPusher *
matrix_pusher_ref(MatrixPusher *pusher)
{
    pusher->refcount++;

    return pusher;
}

/**
 * matrix_pusher_unref:
 * @pusher: a #MatrixPusher
 *
 * Decrease reference count of @pusher by one. If reference count
 * reaches zero, @pusher is freed.
 */
void
matrix_pusher_unref(MatrixPusher *pusher)
{
    if (--pusher->refcount == 0) {
        matrix_pusher_free(pusher);
    }
}

/**
 * matrix_pusher_set_device_display_name:
 * @pusher: a #MatrixPusher
 * @device_display_name: a string that will allow the user to identify
 *                       what device owns this pusher
 *
 * Set the device display name for @pusher.
 */
void
matrix_pusher_set_device_display_name(MatrixPusher *pusher,
                                      const gchar *device_display_name)
{
    g_free(pusher->device_display_name);
    pusher->device_display_name = g_strdup(device_display_name);
}

/**
 * matrix_pusher_get_device_display_name:
 * @pusher: a #MatrixPusher
 *
 * Get the device display name from pusher.
 *
 * Returns: the device display name. The returned value is owned by
 *          @pusher and should not be freed nor modified.
 */
const gchar *
matrix_pusher_get_device_display_name(MatrixPusher *pusher)
{
    return pusher->device_display_name;
}

/**
 * matrix_pusher_set_app_display_name:
 * @pusher: a #MatrixPusher
 * @app_display_name: a string that will allow the user to identify
 *                    what application owns the pusher
 *
 * Sets the app display name for the pusher.
 */
void
matrix_pusher_set_app_display_name(MatrixPusher *pusher,
                                   const gchar *app_display_name)
{
    g_free(pusher->app_display_name);
    pusher->app_display_name = g_strdup(app_display_name);
}

/**
 * matrix_pusher_get_app_display_name:
 * @pusher: a #MatrixPusher
 *
 * Get the app display name for this pusher.
 *
 * Returns: the app display name. The returned value is owned by
 *          @pusher and should not be freed nor modified.
 */
const gchar *
matrix_pusher_get_app_display_name(MatrixPusher *pusher)
{
    return pusher->app_display_name;
}

/**
 * matrix_pusher_set_app_id:
 * @pusher: a #MatrixPusher
 * @app_id: a reverse DNS style identifier for the application. It is
 *          recommended that this ends with the platform, such that
 *          different platform versions get different app
 *          identifiers. Maximum length is 64 characters. If @app_id
 *          is longer than this, it will be truncated.
 *
 * Sets the application ID for this pusher.
 */
void
matrix_pusher_set_app_id(MatrixPusher *pusher, const gchar *app_id)
{
    g_free(pusher->app_id);
    pusher->app_id = g_strndup(app_id, 64);
}

/**
 * matrix_pusher_get_app_id:
 * @pusher: a #MatrixPusher
 *
 * Get the application ID for this pusher.
 *
 * Returns: the application ID. The returned value is owned by @pusher
 *          and should not be freed nor modified.
 */
const gchar *
matrix_pusher_get_app_id(MatrixPusher *pusher)
{
    return pusher->app_id;
}

/**
 * matrix_pusher_set_append:
 * @pusher: a #MatrixPusher
 * @append: if %TRUE, the homeserver should add another pusher with
 *          the given push key and app ID in addition to any others
 *          with different user IDs. Otherwise, the homeserver must
 *          remove any other pushers with the same App ID and pushkey
 *          for different users
 *
 * Set the appending rule for this pusher.
 */
void
matrix_pusher_set_append(MatrixPusher *pusher, gboolean append)
{
    pusher->append = append;
}

/**
 * matrix_pusher_get_append:
 * @pusher: a #MatrixPusher
 *
 * Get the appending rule for this pusher. See
 * matrix_pusher_set_append() for details.
 *
 * Returns: the append rule
 */
gboolean
matrix_pusher_get_append(MatrixPusher *pusher)
{
    return pusher->append;
}

/**
 * matrix_pusher_set_kind:
 * @pusher: a #MatrixPusher
 * @kind: the kind of pusher to configure. "http" makes a pusher that
 *        sends HTTP pokes. %NULL deletes the pusher.
 *
 * Set the kind of pusher to configure.
 */
void
matrix_pusher_set_kind(MatrixPusher *pusher, const gchar *kind)
{
    g_free(pusher->kind);
    pusher->kind = g_strdup(kind);
}

/**
 * matrix_pusher_get_kind:
 * @pusher: a #MatrixPusher
 *
 * Get the kind of pusher in @pusher.
 *
 * Returns: the kind of this pusher. The returned value is owned by
 *          @pusher and should not be freed nor modified
 */
const gchar *
matrix_pusher_get_kind(MatrixPusher *pusher)
{
    return pusher->kind;
}

/**
 * matrix_pusher_set_lang:
 * @pusher: a #MatrixPusher
 * @lang: the preferred language for receiving notifications,
 *        e.g. "en" or "en-US"
 *
 * Set the preferred language for receiving notifications.
 */
void
matrix_pusher_set_lang(MatrixPusher *pusher, const gchar *lang)
{
    g_free(pusher->lang);
    pusher->lang = g_strdup(lang);
}

/**
 * matrix_pusher_get_lang:
 * @pusher: a #MatrixPusher
 *
 * Get the preferred language for receiving notifications.
 *
 * Returns: the preferred language. The returned value is owned by
 *          @pusher and should not be modified nor freed
 */
const gchar *
matrix_pusher_get_lang(MatrixPusher *pusher)
{
    return pusher->lang;
}

/**
 * matrix_pusher_set_profile_tag:
 * @pusher: a #MatrixPusher
 * @profile_tag: a string that determines what set of device rules
 *               will be matched when evaluating push rules for this
 *               pusher. It is an arbitrary string. Multiple devices
 *               may use the same <code>profile_tag</code>. It is
 *               advised that when an app's data is copied or restored
 *               to a different device, this value remain the
 *               same. Client apps should offer ways to change the
 *               profile_tag, optionally copying rules from the old
 *               profile tag. Maximum length is 32 bytes. If
 *               @profile_tag is longer than this, it will be
 *               truncated
 *
 * Set the profile tag of this pusher.
 */
void
matrix_pusher_set_profile_tag(MatrixPusher *pusher, const gchar *profile_tag)
{
    g_free(pusher->profile_tag);
    pusher->profile_tag = g_strndup(profile_tag, 32);
}

/**
 * matrix_pusher_get_profile_tag:
 * @pusher: a #MatrixPusher
 *
 * Get the profile tag of this pusher.
 *
 * Returns: the profile tag. The returned value is owned by @pusher
 *          and should not be freed nor modified
 */
const gchar *
matrix_pusher_get_profile_tag(MatrixPusher *pusher)
{
    return pusher->profile_tag;
}

/**
 * matrix_pusher_set_pushkey:
 * @pusher: a #MatrixPusher
 * @pushkey: a unique identifier for this pusher. The value you should
 *           use for this is the routing or destination address
 *           information for the notification, for example, the APNS
 *           token for APNS or the Registration ID for GCM. If your
 *           notification client has no such concept, use any unique
 *           identifier. Maximum length is 512 bytes. If @pushkey is
 *           longer than this, it will be truncated
 *
 * Set the pushkey for this pusher.
 */
void
matrix_pusher_set_pushkey(MatrixPusher *pusher, const gchar *pushkey)
{
    g_free(pusher->pushkey);
    pusher->pushkey = g_strndup(pushkey, 512);
}

/**
 * matrix_pusher_get_pushkey:
 * @pusher: a #MatrixPusher
 *
 * Get the pushkey for this pusher.
 *
 * Returns: the pushkey. The returned value is owned by @pusher and
 *          should not be freed nor modified
 */
const gchar *
matrix_pusher_get_pushkey(MatrixPusher *pusher)
{
    return pusher->pushkey;
}

/**
 * matrix_pusher_set_data:
 * @pusher: a #MatrixPusher
 * @data: (transfer none): a dictionary of information for the pusher
 *        implementation itself. For example, if kind is "http", this
 *        should contain an "url" member, which is the URL to use to
 *        send notifications to. This function creates a deep copy of
 *        @data, so it can be freed after this call.
 *
 * Set some extra data for the pusher.
 */
void
matrix_pusher_set_data(MatrixPusher *pusher, const JsonNode *data)
{
    if (pusher->data) {
        json_node_free(pusher->data);
    }

    pusher->data = json_node_copy((JsonNode *)data);
}

/**
 * matrix_pusher_take_data:
 * @pusher: a #MatrixPusher
 * @data: (transfer full): extra data for the pusher. See
 *        matrix_pusher_set_data() for details.
 *
 * Set some extra data for the pusher. It differs from
 * matrix_pusher_set_data() that this call assumes ownership over
 * @data, so it should not be freed by the caller.
 */
void
matrix_pusher_take_data(MatrixPusher *pusher, JsonNode *data)
{
    if (pusher->data) {
        json_node_free(pusher->data);
    }

    pusher->data = data;
}

/**
 * matrix_pusher_get_data:
 * @pusher: a #MatrixPusher
 *
 * Get the extra data of this pusher.
 *
 * Returns: (transfer none): the extra data. The returned value is
 *          owned by @pusher and should not be freed nor modified
 */
const JsonNode *
matrix_pusher_get_data(MatrixPusher *pusher)
{
    return pusher->data;
}

/**
 * matrix_pusher_get_json_node:
 * @pusher: a #MatrixPusher
 * @err: a #GError
 *
 * Get the JSON representation of the pusher data as a #JsonNode
 * object. As in the pusher object all fields are mandatory, if any of
 * them is %NULL, this function returns %NULL, and fills @err with
 * %MATRIX_ERROR_INCOMPLETE.
 *
 * Returns: (transfer full): the JSON representation of the pusher
 *          data.
 */
JsonNode *
matrix_pusher_get_json_node(MatrixPusher *pusher, GError **err)
{
    JsonBuilder *builder;
    JsonNode *node;

    if (!pusher->device_display_name
        || !pusher->app_display_name
        || !pusher->app_id
        || !pusher->data
        || !pusher->kind
        || !pusher->lang
        || !pusher->profile_tag
        || !pusher->pushkey) {
        g_set_error(err,
                    MATRIX_ERROR, MATRIX_ERROR_INCOMPLETE,
                    "Pusher data incomplete");

        return NULL;
    }

    builder = json_builder_new();
    json_builder_begin_object(builder);

    json_builder_set_member_name(builder, "device_display_name");
    json_builder_add_string_value(builder, pusher->device_display_name);

    json_builder_set_member_name(builder, "app_display_name");
    json_builder_add_string_value(builder, pusher->app_display_name);

    json_builder_set_member_name(builder, "app_id");
    json_builder_add_string_value(builder, pusher->app_id);

    json_builder_set_member_name(builder, "append");
    json_builder_add_boolean_value(builder, pusher->append);

    json_builder_set_member_name(builder, "kind");
    json_builder_add_string_value(builder, pusher->kind);

    json_builder_set_member_name(builder, "lang");
    json_builder_add_string_value(builder, pusher->lang);

    json_builder_set_member_name(builder, "profile_tag");
    json_builder_add_string_value(builder, pusher->profile_tag);

    json_builder_set_member_name(builder, "pushkey");
    json_builder_add_string_value(builder, pusher->pushkey);

    json_builder_set_member_name(builder, "data");
    json_builder_add_value(builder, pusher->data);

    json_builder_end_object(builder);

    node = json_builder_get_root(builder);
    g_object_unref(builder);

    return node;
}

/**
 * matrix_pusher_get_json_data:
 * @pusher: a #MatrixPusher
 * @datalen: (out): storage for the the length of the JSON data or
 *           %NULL
 * @err: a #GError
 *
 * Get the JSON representation of the pusher data as a string. As in
 * the pusher object all fields are mandatory, if any of them is
 * %NULL, this function returns %NULL, and fills @err with
 * %MATRIX_ERROR_INCOMPLETE.
 *
 * Returns: (transfer full): the JSON representation of the pusher
 *          data.
 */
gchar *matrix_pusher_get_json_data(MatrixPusher *pusher,
                                   gsize *datalen,
                                   GError **err)
{
    JsonGenerator *generator;
    JsonNode *node;
    gchar *data;

    if ((node = matrix_pusher_get_json_node(pusher, err)) == NULL) {
        return NULL;
    }

    generator = json_generator_new();
    json_generator_set_root(generator, node);
    json_node_free(node);

    data = json_generator_to_data(generator, datalen);
    g_object_unref(generator);

    return data;
}

/**
 * MatrixStateEvent:
 *
 * An opaque structure to hold a state event filter.
 */
struct _MatrixStateEvent {
    gchar *type;
    gchar *state_key;
    JsonNode *content;
    guint refcount;
};

G_DEFINE_BOXED_TYPE(MatrixStateEvent, matrix_state_event,
                    (GBoxedCopyFunc)matrix_state_event_ref,
                    (GBoxedFreeFunc)matrix_state_event_unref);

/**
 * matrix_state_event_new:
 *
 * Create a new #MatrixStateEvent object with reference count of 1.
 *
 * Returns: (transfer full): a new #MatrixStateEvent
 */
MatrixStateEvent *
matrix_state_event_new(void)
{
    MatrixStateEvent *event;

    event = g_new0(MatrixStateEvent, 1);
    event->refcount = 1;

    return event;
}

static void
matrix_state_event_free(MatrixStateEvent *event)
{
    g_free(event->type);
    g_free(event->state_key);

    if (event->content) {
        json_node_free(event->content);
    }

    g_free(event);
}

/**
 * matrix_state_event_ref:
 * @event: a #MatrixStateEvent
 *
 * Increase reference count of @event by one.
 *
 * Returns: (transfer none): the same #MatrixStateEvent
 */
MatrixStateEvent *
matrix_state_event_ref(MatrixStateEvent *event)
{
    event->refcount++;

    return event;
}

/**
 * matrix_state_event_unref:
 * @event: a #MatrixStateEvent
 *
 * Decrease reference count of @event by one. If reference count
 * reaches zero, @event is freed.
 */
void
matrix_state_event_unref(MatrixStateEvent *event)
{
    if (--event->refcount == 0) {
        matrix_state_event_free(event);
    }
}

/**
 * matrix_state_event_set_event_type:
 * @event: a #MatrixStateEvent
 * @event_type: the type of the state event
 *
 * Set the type of the state event in @event.
 */
void
matrix_state_event_set_event_type(MatrixStateEvent *event,
                                  const gchar *event_type)
{
    g_free(event->type);
    event->type = g_strdup(event_type);
}

/**
 * matrix_state_event_get_event_type:
 * @event: a #MatrixStateEvent
 *
 * Get the type of the state event in @event.
 *
 * Returns: (transfer none): the event type. The returned value is
 *          owned by @event and should not be freed nor modified.
 */
const gchar *
matrix_state_event_get_event_type(MatrixStateEvent *event)
{
    return event->type;
}

/**
 * matrix_state_event_set_state_key:
 * @event: a #MatrixStateEvent
 * @state_key: the key of the state event
 *
 * Set the state key for the state event @event.
 */
void
matrix_state_event_set_state_key(MatrixStateEvent *event,
                                 const gchar *state_key)
{
    g_free(event->state_key);
    event->state_key = g_strdup(state_key);
}

/**
 * matrix_state_event_get_state_key:
 * @event: a #MatrixStateEvent
 *
 * Get the state key of @event.
 *
 * Returns: (transfer none): the state key. The returned value is
 *          owned by @event and should not be freed nor modified
 */
const gchar *
matrix_state_event_get_state_key(MatrixStateEvent *event)
{
    return event->state_key;
}

/**
 * matrix_state_event_set_content:
 * @event: a #MatrixStateEvent
 * @content: the desired content of the state event
 *
 * Set the content of the state event.
 */
void
matrix_state_event_set_content(MatrixStateEvent *event, const JsonNode *content)
{
    if (event->content) {
        json_node_free(event->content);
    }

    event->content = json_node_copy((JsonNode *)content);
}

/**
 * matrix_state_event_get_content:
 * @event: a #MatrixStateEvent
 *
 * Get the contents of the state event.
 *
 * Returns: (transfer none): the contents of the state event. The
 *          returned value is owned by @event and should not be freed
 *          nor modified
 */
const JsonNode *
matrix_state_event_get_content(MatrixStateEvent *event)
{
    return event->content;
}

/**
 * matrix_state_event_get_json_node:
 * @event: a #MatrixStateEvent
 *
 * Get the JSON representation of the state event as a #JsonNode
 * object.
 *
 * Returns: (transfer full): the JSON representation of the state
 *          event
 */
JsonNode *
matrix_state_event_get_json_node(MatrixStateEvent *event)
{
    JsonBuilder *builder;
    JsonNode *node;

    builder = json_builder_new();
    json_builder_begin_object(builder);

    json_builder_set_member_name(builder, "state_key");
    json_builder_add_string_value(builder, event->state_key);

    json_builder_set_member_name(builder, "type");
    json_builder_add_string_value(builder, event->type);

    json_builder_set_member_name(builder, "content");
    json_builder_add_value(builder, event->content);

    json_builder_end_object(builder);

    node = json_builder_get_root(builder);
    g_object_unref(builder);

    return node;
}

/**
 * matrix_state_event_get_json_data:
 * @event: a #MatrixStateEvent
 * @datalen: (out): storage for the the length of the JSON data or
 *           %NULL
 *
 * Get the JSON representation of the state event as a string.
 *
 * Returns: (transfer full): the JSON representation of the state
 *          event
 */
gchar *
matrix_state_event_get_json_data(MatrixStateEvent *event, gsize *datalen)
{
    JsonGenerator *generator;
    JsonNode *node = matrix_state_event_get_json_node(event);
    gchar *data;

    generator = json_generator_new();
    json_generator_set_root(generator, node);
    json_node_free(node);

    data = json_generator_to_data(generator, datalen);
    g_object_unref(generator);

    return data;
}

/**
 * MatrixPresenceEvent:
 *
 * An opaque structure to hold a presence event.
 */
struct _MatrixPresenceEvent {
    gchar *user_id;
    gchar *display_name;
    gchar *avatar_url;
    gulong last_active_ago;
    MatrixPresence presence;
    guint refcount;
};

G_DEFINE_BOXED_TYPE(MatrixPresenceEvent, matrix_presence_event,
                    (GBoxedCopyFunc)matrix_presence_event_ref,
                    (GBoxedFreeFunc)matrix_presence_event_unref);

/**
 * matrix_presence_event_new:
 *
 * Create a new #MatrixPresenceEvent object with reference count of 1.
 *
 * Returns: (transfer full): a new #MatrixPresenceEvent
 */
MatrixPresenceEvent *
matrix_presence_event_new(void)
{
    MatrixPresenceEvent *event;

    event = g_new0(MatrixPresenceEvent, 1);
    event->refcount = 1;

    return event;
}

#define try_get_string_member(evt, nd, obj, m_n, s_m)           \
    if ((nd = json_object_get_member(obj, (m_n))) != NULL) {    \
        g_free(evt->s_m);                                       \
        evt->s_m = g_strdup(json_node_get_string (nd));         \
        json_object_remove_member(obj, m_n);                    \
    }

#define try_get_member(evt, nd, obj, m_n, typ, s_m)             \
    if ((nd = json_object_get_member(obj, (m_n))) != NULL) {    \
        evt->s_m = json_node_get_##typ (nd);                    \
        json_object_remove_member(obj, m_n);                    \
    }

/**
 * matrix_presence_event_new_from_json: (constructor)
 * @json_data: a #JsonNode to convert to #MatrixPresenceEvent
 *
 * Converts a #JsonNode (usually from the response from a homeserver)
 * to a #MatrixPresenceEvent.
 *
 * Returns: (transfer full): a new #MatrixPresenceEvent with all
 *          values from #JsonNode
 */
MatrixPresenceEvent *
matrix_presence_event_new_from_json(JsonNode *json_data)
{
    JsonNode *node, *copied_data;
    JsonObject *root;
    MatrixPresenceEvent *event;
    GError *err = NULL;

    g_return_val_if_fail(JSON_NODE_HOLDS_OBJECT(json_data), NULL);

    copied_data = _json_node_deep_copy(json_data);

    event = matrix_presence_event_new();
    root = json_node_get_object(copied_data);

    try_get_string_member(event, node, root, "user_id", user_id);
    try_get_string_member(event, node, root, "displayname", display_name);
    try_get_string_member(event, node, root, "avatar_url", avatar_url);
    try_get_member(event, node, root, "last_active_ago", int, last_active_ago);

    if ((node = json_object_get_member(root, "presence")) != NULL) {
        int val;
        const gchar *value = json_node_get_string(node);

        val = _g_enum_nick_to_value(MATRIX_TYPE_PRESENCE, value, &err);

        if (!err) {
            event->presence = val;
        } else {
            g_warning("Unknown presence value '%s'. Please report it to the Matrix.org GLib SDK project.", value);
        }

        json_object_remove_member(root, "presence");
    }

    // By now, root_object should be empty.
    if (json_object_get_size(root) > 0) {
        g_warning("Some members remained in the received presence event. This may be a bug in Matrix GLib");
    }

    return event;
}

static void
matrix_presence_event_free(MatrixPresenceEvent *event)
{
    g_free(event);
}

/**
 * matrix_presence_event_ref:
 * @event: a #MatrixPresenceEvent
 *
 * Increase reference count of @event by one.
 *
 * Returns: (transfer none): the same #MatrixPresenceEvent
 */
MatrixPresenceEvent *
matrix_presence_event_ref(MatrixPresenceEvent *event)
{
    event->refcount++;

    return event;
}

/**
 * matrix_presence_event_unref:
 * @event: a #MatrixPresenceEvent
 *
 * Decrease reference count of @event by one. If reference count drops
 * to zero, @event is freed.
 */
void
matrix_presence_event_unref(MatrixPresenceEvent *event)
{
    if (--event->refcount == 0) {
        matrix_presence_event_free(event);
    }
}

/**
 * matrix_presence_event_set_user_id:
 * @event: a #MatrixPresenceEvent
 * @user_id: (transfer none): a Matrix.org user ID
 *
 * Set the user ID in the presence event.
 */
void
matrix_presence_event_set_user_id(MatrixPresenceEvent *event,
                                  const gchar *user_id)
{
    g_free(event->user_id);
    event->user_id = g_strdup(user_id);
}

/**
 * matrix_presence_event_get_user_id:
 * @event: a #MatrixPresenceEvent
 *
 * Get the user ID from the presence event.
 *
 * Returns: (transfer none): a user ID
 */
const gchar *
matrix_presence_event_get_user_id(MatrixPresenceEvent *event)
{
    return event->user_id;
}

/**
 * matrix_presence_event_set_display_name:
 * @event: a #MatrixPresenceEvent
 * @display_name: a display name to set
 *
 * Set the display name in the presence event.
 */
void
matrix_presence_event_set_display_name(MatrixPresenceEvent *event,
                                       const gchar *display_name)
{
    g_free(event->display_name);
    event->display_name = g_strdup(display_name);
}

/**
 * matrix_presence_event_get_display_name:
 * @event: a #MatrixPresenceEvent
 *
 * Get the display name from the presence event.
 *
 * Returns: (transfer none) (allow-none): the display name
 */
const gchar *
matrix_presence_event_get_display_name(MatrixPresenceEvent *event)
{
    return event->display_name;
}

/**
 * matrix_presence_event_set_avatar_url:
 * @event: a #MatrixPresenceEvent
 * @avatar_url: (allow-none): the avatar URL to set
 *
 * Set the avatar URL in the presence event.
 */
void
matrix_presence_event_set_avatar_url(MatrixPresenceEvent *event,
                                     const gchar *avatar_url)
{
    g_free(event->avatar_url);
    event->avatar_url = g_strdup(avatar_url);
}

/**
 * matrix_presence_event_get_avatar_url:
 * @event: a #MatrixPresenceEvent
 *
 * Gets the avatar URL from the presence event.
 *
 * Returns: (transfer none) (allow-none): the avatar URL
 */
const gchar *
matrix_presence_event_get_avatar_url(MatrixPresenceEvent *event)
{
    return event->avatar_url;
}

/**
 * matrix_presence_event_set_last_active_ago:
 * @event: a #MatrixPresenceEvent
 * @last_active_ago: the amount of time, in seconds
 *
 * Set the amount of time when the user in the presence event was last
 * active.
 */
void
matrix_presence_event_set_last_active_ago(MatrixPresenceEvent *event,
                                          gulong last_active_ago)
{
    event->last_active_ago = last_active_ago;
}

/**
 * matrix_presence_event_get_last_active_ago:
 * @event: a #MatrixPresenceEvent
 *
 * Get the amount of time since the user was last active, in seconds.
 *
 * Returns: count of seconds
 */
gulong
matrix_presence_event_get_last_active_ago(MatrixPresenceEvent *event)
{
    return event->last_active_ago;
}

/**
 * matrix_presence_event_set_presence:
 * @event: a #MatrixPresenceEvent
 * @presence: the new presence
 *
 * Set the new presence state of the user in the presence event.
 */
void
matrix_presence_event_set_presence(MatrixPresenceEvent *event,
                                   MatrixPresence presence)
{
    event->presence = presence;
}

/**
 * matrix_presence_event_get_presence:
 * @event: a #MatrixPresenceEvent
 *
 * Get the new presence of the user from the presence event.
 *
 * Returns: the new presence state of the user
 */
MatrixPresence
matrix_presence_event_get_presence(MatrixPresenceEvent *event)
{
    return event->presence;
}
