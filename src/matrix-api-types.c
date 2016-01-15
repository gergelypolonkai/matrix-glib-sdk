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

#include "matrix-api-types.h"
#include "matrix-enumtypes.h"
#include "utils.h"

/**
 * SECTION:matrix-api-types
 * @title: Generic types
 * @short_description: Generic types for all #MatrixAPI
 *                     implementations
 *
 * These are the types used by all #MatrixAPI implementations for
 * communication with the homeserver.
 */

/**
 * MatrixAPIError:
 * @MATRIX_API_ERROR_NONE: no error
 * @MATRIX_API_ERROR_COMMUNICATION_ERROR: there was a problem in
 *                                        communication
 *                                        (e.g. connection error)
 * @MATRIX_API_ERROR_INCOMPLETE: the passed/generated data is
 *                               incomplete
 * @MATRIX_API_ERROR_BAD_REQUEST: the request is invalid
 * @MATRIX_API_ERROR_BAD_RESPONSE: malformed response, or the response
 *                                 is not a JSON object
 * @MATRIX_API_ERROR_INVALID_ROOM_ID: the provided string doesn’t
 *                                    contain a valid room ID
 * @MATRIX_API_ERROR_MISSING_TOKEN: authorization token is missing
 *                                  from the request
 * @MATRIX_API_ERROR_FORBIDDEN: access was forbidden (e.g. due to a
 *                              missing/invalid token, or using a bad
 *                              password during login)
 * @MATRIX_API_ERROR_UNKNOWN: an error unknown to the Matrix server
 * @MATRIX_API_ERROR_UNKNOWN_TOKEN: the token provided is not known
 *                                  for the homeserver
 * @MATRIX_API_ERROR_NOT_JSON: illegal request, the content is not
 *                             valid JSON
 * @MATRIX_API_ERROR_UNRECOGNIZED: the homeserver didn't understand
 *                                 the request
 * @MATRIX_API_ERROR_UNSPECIFIED: no error code was sent by the
 *                                homeserver
 * @MATRIX_API_ERROR_UNKNOWN_ERROR: an error unknown to this library
 *
 * Value mappings from Matrix.org API error codes
 * (e.g. <code>M_MISSING_TOKEN</code>). They should be set
 * automatically by API calls, if the response contains an error code.
 */

/**
 * MATRIX_API_ERROR:
 *
 * Error domain for Matrix GLib SDK API. See #GError for more
 * information on error domains.
 */

/**
 * matrix_api_error_quark:
 *
 * Gets the Matrix API error #GQuark
 */
G_DEFINE_QUARK(matrix-api-error-quark, matrix_api_error);

/**
 * MatrixAPIEventFormat:
 * @MATRIX_API_EVENT_FORMAT_DEFAULT: event format will be omitted from
 *                                   the filter, so the server will
 *                                   use its default (usually
 *                                   @MATRIX_API_EVENT_FORMAT_FEDERATION)
 * @MATRIX_API_EVENT_FORMAT_CLIENT: return the events in a format
 *                                  suitable for clients
 * @MATRIX_API_EVENT_FORMAT_FEDERATION: return the raw event as
 *                                      receieved over federation
 *
 * Event format received when synchronizing.
 */

/**
 * MatrixAPIRoomPreset:
 * @MATRIX_API_ROOM_PRESET_NONE: no preset
 * @MATRIX_API_ROOM_PRESET_PRIVATE: preset for private rooms
 * @MATRIX_API_ROOM_PRESET_TRUSTED_PRIVATE: same as private rooms, but
 *                                          all users get the same
 *                                          power level as the room
 *                                          creator
 * @MATRIX_API_ROOM_PRESET_PUBLIC: preset for public rooms
 *
 * Preset values for matrix_api_create_room() calls.
 */

/**
 * MatrixAPIRoomVisibility:
 * @MATRIX_API_ROOM_VISIBILITY_DEFAULT: use a server-assigned value
 *                                      (usually <code>private</code>
 * @MATRIX_API_ROOM_VISIBILITY_PUBLIC: make the room visible in the
 *                                     public room list
 * @MATRIX_API_ROOM_VISIBILITY_PRIVATE: hide the room from the public
 *                                      room list
 *
 * Visibility values for room creation. Not to be confused with join
 * rules.
 */

/**
 * MatrixAPIResizeMethod:
 * @MATRIX_API_RESIZE_METHOD_DEFAULT: use the server default value
 * @MATRIX_API_RESIZE_METHOD_CROP: crop thumbnail to the requested
 *                                 size
 * @MATRIX_API_RESIZE_METHOD_SCALE: scale thumbnail to the requested
 *                                  size
 *
 * Resizing methods for matrix_api_media_thumbnail().
 */

/**
 * MatrixAPIPresence:
 * @MATRIX_API_PRESENCE_ONLINE: user is online
 * @MATRIX_API_PRESENCE_OFFLINE: user is offline
 * @MATRIX_API_PRESENCE_UNAVAILABLE: user is unavailable (i.e. busy)
 * @MATRIX_API_PRESENCE_FREE_FOR_CHAT: user is free for chat
 *
 * Presence values for matrix_api_set_user_presence() and other
 * presence related queries.
 */

/**
 * MatrixAPIPusherKind:
 * @MATRIX_API_PUSHER_KIND_OVERRIDE: highest priority rules
 * @MATRIX_API_PUSHER_KIND_SENDER: for (unencrypted) messages that
 *                                 match certain patterns
 * @MATRIX_API_PUSHER_KIND_ROOM: for all messages for a given
 *                               room. The rule ID of a room rule is
 *                               always the ID of the room that it
 *                               affects
 * @MATRIX_API_PUSHER_KIND_CONTENT: for messages from a specific
 *                                  Matrix user ID. The rule ID of
 *                                  such rules is always the Matrix ID
 *                                  of the user whose messages they'd
 *                                  apply to
 * @MATRIX_API_PUSHER_KIND_UNDERRIDE: lowest priority rules
 *
 * Pusher types.
 */

/**
 * MatrixAPIPusherConditionKind:
 * @MATRIX_API_PUSHER_CONDITION_KIND_EVENT_MATCH: glob pattern match
 *                                                on a field of the
 *                                                event. Requires a
 *                                                <code>key</code> and
 *                                                a
 *                                                <code>pattern</code>
 *                                                parameter
 * @MATRIX_API_PUSHER_CONDITION_KIND_PROFILE_TAG: matches the profile
 *                                                tag of the device
 *                                                that the
 *                                                notification would
 *                                                be delivered
 *                                                to. Requires a
 *                                                <code>profile_tag</code>
 *                                                parameter
 * @MATRIX_API_PUSHER_CONDITION_KIND_CONTAINS_DISPLAY_NAME: matches
 *                                                          unencrypted
 *                                                          messages
 *                                                          where the
 *                                                          content's
 *                                                          body
 *                                                          contains
 *                                                          the
 *                                                          owner's
 *                                                          display
 *                                                          name in
 *                                                          that room.
 * @MATRIX_API_PUSHER_CONDITION_KIND_ROOM_MEMBER_COUNT: matches the
 *                                                      current number
 *                                                      of members in
 *                                                      the
 *                                                      room. Requires
 *                                                      an
 *                                                      <code>is</code>
 *                                                      parameter,
 *                                                      which must be
 *                                                      an integer,
 *                                                      optionally
 *                                                      prefixed by
 *                                                      <code>==</code>,
 *                                                      <code>&lt;</code>,
 *                                                      <code>&gt;</code>,
 *                                                      <code>&lt;=</code>
 *                                                      or
 *                                                      <code>&gt;=</code>. If
 *                                                      the prefix is
 *                                                      omitted, it
 *                                                      defaults to
 *                                                      <code>==</code>
 *
 * Condition types for pushers.
 */

/**
 * MatrixAPIEventDirection:
 * @MATRIX_API_EVENT_DIRECTION_FORWARD: List events after the
 *                                      specified one
 * @MATRIX_API_EVENT_DIRECTION_BACKWARD: List events before the
 *                                       specified one
 *
 * Direction of events when requesting an event context.
 */

/**
 * MatrixAPIReceiptType:
 * @MATRIX_API_RECEIPT_TYPE_READ: indicate that the message has been
 *                                read
 *
 * Receipt types of acknowledgment.
 */

/**
 * MatrixAPIFilterRules: (ref-func matrix_api_filter_rules_ref) (unref-func matrix_api_filter_rules_unref)
 *
 * An opaque structure to hold filter rules that can be used to filter
 * events in the event stream. It is possible to filter by event type,
 * room and sender ID.
 */

struct _MatrixAPIFilterRules {
    guint limit;
    GList *rooms;
    GList *excluded_rooms;
    GList *senders;
    GList *excluded_senders;
    GList *types;
    GList *excluded_types;
    guint refcount;
};

G_DEFINE_BOXED_TYPE(MatrixAPIFilterRules, matrix_api_filter_rules,
                    (GBoxedCopyFunc)matrix_api_filter_rules_ref,
                    (GBoxedFreeFunc)matrix_api_filter_rules_unref);

/**
 * matrix_api_filter_rules_new:
 *
 * Create a new #MatrixAPIFilterRules object with reference count of
 * 1.
 *
 * Returns: (transfer full): a new #MatrixAPIFilterRules
 */
MatrixAPIFilterRules *
matrix_api_filter_rules_new(void)
{
    MatrixAPIFilterRules *rules;

    rules = g_new0(MatrixAPIFilterRules, 1);
    rules->refcount = 1;

    return rules;
}

static void
matrix_api_filter_rules_free(MatrixAPIFilterRules *rules)
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
 * matrix_api_filter_rules_ref:
 * @rules: a #MatrixAPIFilterRules
 *
 * Increase reference count of @rules by one.
 *
 * Returns: (transfer none): the same #MatrixAPIFilterRules
 */
MatrixAPIFilterRules *
matrix_api_filter_rules_ref(MatrixAPIFilterRules *rules)
{
    rules->refcount++;

    return rules;
}

/**
 * matrix_api_filter_rules_unref:
 * @rules: a #MatrixAPIFilterRules
 *
 * Decrease reference count of @rules by one. If reference count
 * reaches zero, @rules is freed.
 */
void
matrix_api_filter_rules_unref(MatrixAPIFilterRules *rules)
{
    if (--rules->refcount == 0) {
        matrix_api_filter_rules_free(rules);
    }
}

/**
 * matrix_api_filter_rules_set_limit:
 * @rules: a #MatrixAPIFilterRules
 * @limit: (in): the maximum number of events to return.
 *
 * Set the maximum number of events to return by the filter. If @limit
 * is <code>0</code>, no limit will be applied.
 */
void
matrix_api_filter_rules_set_limit(MatrixAPIFilterRules *rules, guint limit)
{
    rules->limit = limit;
}

/**
 * matrix_api_filter_rules_get_limit:
 * @rules: a #MatrixAPIFilterRules
 *
 * Get the current limit set in @rules.
 *
 * Returns: the limit currently set
 */
guint
matrix_api_filter_rules_get_limit(MatrixAPIFilterRules *rules)
{
    return rules->limit;
}

/**
 * matrix_api_filter_rules_set_senders:
 * @rules: a #MatrixAPIFilterRules
 * @senders: (in) (element-type utf8) (transfer full) (allow-none):
 *           a list of Matrix user IDs. Events from these users will
 *           be included in the filtered event list.If %NULL then all
 *           senders are included. See
 *           matrix_api_filter_rules_add_sender() for wildcarding
 *           possibilities
 *
 * Set the list of user IDs to include in the filtered events. @rules
 * takes ownership of @senders, so it should not be freed nor modified
 * directly after this call.
 */
void
matrix_api_filter_rules_set_senders(MatrixAPIFilterRules *rules, GList *senders)
{
    g_list_free_full(rules->senders, g_free);
    rules->senders = senders;
}

/**
 * matrix_api_filter_rules_add_sender:
 * @rules: a #MatrixAPIFilterRules
 * @sender: (in): a Matrix user ID to add to the included senders
 *          list. A <code>*</code> can be used as a wildcard to match
 *          any sequence of characters
 *
 * Add @sender to the list of user IDs to include in the filtered
 * event list. If @sender is already included in the senders list,
 * nothing happens.
 */
void
matrix_api_filter_rules_add_sender(MatrixAPIFilterRules *rules,
                                   const gchar *sender)
{
    g_return_if_fail(sender != NULL);

    if (!g_list_find_custom(rules->senders, sender, (GCompareFunc)g_strcmp0)) {
        rules->senders = g_list_prepend(rules->senders, g_strdup(sender));
    }
}

/**
 * matrix_api_filter_rules_delete_sender:
 * @rules: a #MatrixAPIFilterRules
 * @sender: (in): the user ID to remove from the senders list
 *
 * Remove @sender from the list of user IDs to include in the filtered
 * event list.
 */
void
matrix_api_filter_rules_delete_sender(MatrixAPIFilterRules *rules,
                                      const gchar *sender)
{
    GList *sender_element;

    g_return_if_fail(sender != NULL);

    while (sender_element = g_list_find_custom(rules->senders, sender,
                                               (GCompareFunc)g_strcmp0)) {
        rules->senders = g_list_remove_link(rules->senders, sender_element);
        g_list_free_full(sender_element, g_free);
    }
}

/**
 * matrix_api_filter_rules_get_senders:
 * @rules: a #MatrixAPIFilterRules
 *
 * Get the list of user IDs that will be included in the filtered
 * events.
 *
 * Returns: (element-type utf8) (allow-none) (transfer none): the
 *          list of user IDs. The returned value is owned by @rules
 *          and should not be freed nor modified
 */
const GList *
matrix_api_filter_rules_get_senders(MatrixAPIFilterRules *rules)
{
    return rules->senders;
}

/**
 * matrix_api_filter_rules_set_excluded_senders:
 * @rules: a #MatrixAPIFilterRules
 * @senders: (in) (element-type utf8) (transfer full) (allow-none):
 *           a list of Matrix user IDs. Events from these users will
 *           be included in the filtered event list.If %NULL then all
 *           senders are included. See
 *           matrix_api_filter_rules_add_sender() for wildcarding
 *           possibilities
 *
 * Set the list of Matrix user IDs to exclude from the filtered
 * events. A matching sender will be excluded even if it is listed in
 * the senders list (specified by
 * e.g. matrix_api_filter_rules_set_senders()). @rules takes
 * ownership of @senders, so it should not be freed nor modified
 * directly after this call.
 */
void
matrix_api_filter_rules_set_excluded_senders(MatrixAPIFilterRules *rules,
                                             GList *senders)
{
    g_list_free_full(rules->excluded_senders, g_free);
    rules->excluded_senders = senders;
}

/**
 * matrix_api_filter_rules_add_excluded_sender:
 * @rules: a #MatrixAPIFilterRules
 * @sender: (in): a Matrix user ID to add to the excluded senders
 *          list. See matrix_api_filter_rules_add_sender() for
 *          wildcarding possibilities
 *
 * Add @sender to the list of user IDs to exclude from the filtered
 * event list. If @sender is already in the excluded senders list,
 * nothing happens.
 */
void
matrix_api_filter_rules_add_excluded_sender(MatrixAPIFilterRules *rules,
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
 * matrix_api_filter_rules_delete_excluded_sender:
 * @rules: a #MatrixAPIFilterRules
 * @sender: (in): the Matrix user ID to remove from the excluded
 *          senders list
 *
 * Remove @sender from the list of user IDs to exclude from the
 * filtered event list.
 */
void
matrix_api_filter_rules_delete_excluded_sender(MatrixAPIFilterRules *rules,
                                               const gchar *sender)
{
    GList *sender_element;

    g_return_if_fail(sender != NULL);

    while (sender_element = g_list_find_custom(rules->excluded_senders, sender,
                                               (GCompareFunc)g_strcmp0)) {
        rules->excluded_senders = g_list_remove_link(rules->excluded_senders,
                                                     sender_element);
        g_list_free_full(sender_element, g_free);
    }
}

/**
 * matrix_api_filter_rules_get_excluded_senders:
 * @rules: a #MatrixAPIFilterRules
 *
 * Get the list of user IDs that will be excluded in the filtered
 * events.
 *
 * Returns: (element-type utf8) (allow-none) (transfer none): the
 *          list of user IDs to be excluded. The returned value is
 *          owned by @rules and should not be freed nor modified.
 */
const GList *
matrix_api_filter_rules_get_excluded_senders(MatrixAPIFilterRules *rules)
{
    return rules->excluded_senders;
}

/**
 * matrix_api_filter_rules_set_rooms:
 * @rules: a #MatrixAPIFilterRules
 * @rooms: (in) (element-type utf8) (transfer full) (allow-none): a
 *         list of room IDs. Events from these rooms will be included
 *         in the filtered event list.If %NULL then all rooms are
 *         included. See matrix_api_filter_rules_add_sender() for
 *         wildcarding possibilities
 *
 * Set the list of room IDs to include in the filtered events. @rules
 * takes ownership of @rooms, so it should not be freed nor modified
 * directly after this call.
 */
void
matrix_api_filter_rules_set_rooms(MatrixAPIFilterRules *rules, GList *rooms)
{
    g_list_free_full(rules->rooms, g_free);
    rules->rooms = rooms;
}

/**
 * matrix_api_filter_rules_add_room:
 * @rules: a #MatrixAPIFilterRules
 * @room: (in): a room ID to add to the included rooms list. See
 *        matrix_api_filter_rules_add_sender() for wildcarding
 *        possibilities
 *
 * Add @room to the list of room IDs to include in the filtered
 * event list. If @room is already included in the rooms list,
 * nothing happens.
 */
void
matrix_api_filter_rules_add_room(MatrixAPIFilterRules *rules,
                                 const gchar *room)
{
    g_return_if_fail(room != NULL);

    if (!g_list_find_custom(rules->rooms, room, (GCompareFunc)g_strcmp0)) {
        rules->rooms = g_list_prepend(rules->rooms, g_strdup(room));
    }
}

/**
 * matrix_api_filter_rules_delete_room:
 * @rules: a #MatrixAPIFilterRules
 * @room: (in): the room ID to remove from the rooms list
 *
 * Remove @room from the list of room IDs to include in the filtered
 * event list.
 */
void
matrix_api_filter_rules_delete_room(MatrixAPIFilterRules *rules,
                                    const gchar *room)
{
    GList *room_element;

    g_return_if_fail(room != NULL);

    while (room_element = g_list_find_custom(rules->rooms, room,
                                             (GCompareFunc)g_strcmp0)) {
        rules->rooms = g_list_remove_link(rules->rooms, room_element);
        g_list_free_full(room_element, g_free);
    }
}

/**
 * matrix_api_filter_rules_get_rooms:
 * @rules: a #MatrixAPIFilterRules
 *
 * Get the list of room IDs that will be included in the filtered
 * events.
 *
 * Returns: (element-type utf8) (allow-none) (transfer none): the
 *          list of room IDs. The returned value is owned by @rules
 *          and should not be freed nor modified
 */
const GList *
matrix_api_filter_rules_get_rooms(MatrixAPIFilterRules *rules)
{
    return rules->rooms;
}

/**
 * matrix_api_filter_rules_set_excluded_rooms:
 * @rules: a #MatrixAPIFilterRules
 * @rooms: (in) (element-type utf8) (transfer full) (allow-none): a
 *         list of room IDs. Events from these rooms will be included
 *         in the filtered event list.If %NULL, then all rooms are
 *         included. See matrix_api_filter_rules_add_sender() for
 *         wildcarding possibilities
 *
 * Set the list of room IDs to exclude from the filtered events. A
 * matching room will be excluded even if it is listed in the rooms
 * list (specified by
 * e.g. matrix_api_filter_rules_set_rooms()). @rules takes ownership
 * of @rooms, so it should not be freed nor modified directly after
 * this call.
 */
void
matrix_api_filter_rules_set_excluded_rooms(MatrixAPIFilterRules *rules,
                                           GList *rooms)
{
    g_list_free_full(rules->excluded_rooms, g_free);
    rules->excluded_rooms = rooms;
}

/**
 * matrix_api_filter_rules_add_excluded_room:
 * @rules: a #MatrixAPIFilterRules
 * @room: (in): a room ID to add to the excluded rooms list. See
 *        matrix_api_filter_rules_add_sender() for wildcarding
 *        possibilities
 *
 * Add @room to the list of room IDs to exclude from the filtered
 * event list. If @room is already in the excluded rooms list, nothing
 * happens.
 */
void
matrix_api_filter_rules_add_excluded_room(MatrixAPIFilterRules *rules,
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
 * matrix_api_filter_rules_delete_excluded_room:
 * @rules: a #MatrixAPIFilterRules
 * @room: (in): the room ID to remove from the excluded rooms list
 *
 * Remove @room from the list of room IDs to exclude from the filtered
 * event list.
 */
void
matrix_api_filter_rules_delete_excluded_room(MatrixAPIFilterRules *rules,
                                             const gchar *room)
{
    GList *room_element;

    g_return_if_fail(room != NULL);

    while (room_element = g_list_find_custom(rules->excluded_rooms, room,
                                             (GCompareFunc)g_strcmp0)) {
        rules->excluded_rooms = g_list_remove_link(rules->excluded_rooms,
                                                   room_element);
        g_list_free_full(room_element, g_free);
    }
}

/**
 * matrix_api_filter_rules_get_excluded_rooms:
 * @rules: a #MatrixAPIFilterRules
 *
 * Get the list of room IDs that will be excluded in the filtered
 * events.
 *
 * Returns: (element-type utf8) (allow-none) (transfer none): the
 *          list of room IDs to be excluded. The returned value is
 *          owned by @rules and should not be freed nor modified.
 */
const GList *
matrix_api_filter_rules_get_excluded_rooms(MatrixAPIFilterRules *rules)
{
    return rules->excluded_rooms;
}

/**
 * matrix_api_filter_rules_set_types:
 * @rules: a #MatrixAPIFilterRules
 * @types: (in) (element-type utf8) (transfer full) (allow-none): a
 *         list of event types to include. If %NULL then all event
 *         types are included. See
 *         matrix_api_filter_rules_add_sender() for wildcarding
 *         possibilities
 *
 * Set the list of event types to be included in the filtered events.
 */
void
matrix_api_filter_rules_set_types(MatrixAPIFilterRules *rules, GList *types)
{
    g_list_free_full(rules->types, g_free);
    rules->types = types;
}

/**
 * matrix_api_filter_rules_add_type:
 * @rules: a #MatrixAPIFilterRules
 * @type: (in): an event type to add to the list of included
 *        events. See matrix_api_filter_rules_add_sender() for
 *        wildcarding possibilities
 *
 * Add @type to the list of event types to include in the filtered
 * event list. If @type is already included in the types list, nothing
 * happens.
 */
void
matrix_api_filter_rules_add_type(MatrixAPIFilterRules *rules, const gchar *type)
{
    g_return_if_fail(type != NULL);

    if (g_list_find_custom(rules->types, type, (GCompareFunc)g_strcmp0)) {
        rules->types = g_list_prepend(rules->types, g_strdup(type));
    }
}

/**
 * matrix_api_filter_rules_delete_type:
 * @rules: a #MatrixAPIFilterRules
 * @type: (in): a type to remove from the list of included event
 *        types. See matrix_api_filter_rules_add_sender() for
 *        wildcarding possibilities
 *
 * Remove @type from the list of excluded event type list.
 */
void
matrix_api_filter_rules_delete_type(MatrixAPIFilterRules *rules,
                                    const gchar *type)
{
    GList *type_element;

    g_return_if_fail(type != NULL);

    while (type_element = g_list_find_custom(rules->types, type,
                                             (GCompareFunc)g_strcmp0)) {
        rules->types = g_list_remove_link(rules->types, type_element);
        g_list_free_full(type_element, g_free);
    }
}

/**
 * matrix_api_filter_rules_get_types:
 * @rules: a #MatrixAPIFilterRules
 *
 * Get the list of event types that will be included in the filtered
 * events.
 *
 * Returns: (element-type utf8) (allow-none) (transfer none): the
 *          list of event types. The returned values is owned by
 *          @rules and should not be freed nor modified
 */
const GList *
matrix_api_filter_rules_get_types(MatrixAPIFilterRules *rules)
{
    return rules->types;
}

/**
 * matrix_api_filter_rules_set_excluded_types:
 * @rules: a #MatrixAPIFilterRules
 * @types: (in) (element-type utf8) (transfer full) (allow-none): a
 *         list of event types to exclude. If %NULL then no event
 *         types are excluded. A matching type will be excluded even
 *         if it is listed in the included types. See
 *         matrix_api_filter_rules_add_sender() for wildcarding
 *         possibilities
 *
 * Set the list of event types to be excluded from the filtered
 * events. A matching type will be excluded even if it is listed in
 * the types list (specified by
 * e.g. matrix_api_filter_rules_set_types()).
 */
void
matrix_api_filter_rules_set_excluded_types(MatrixAPIFilterRules *rules,
                                           GList *types)
{
    g_list_free_full(rules->excluded_types, g_free);
    rules->excluded_types = types;
}

/**
 * matrix_api_filter_rules_add_excluded_type:
 * @rules: a #MatrixAPIFilterRules
 * @type: (in): an event type to add to the excluded event type
 *        list. See matrix_api_filter_rules_add_sender() for
 *        wildcarding possibilities
 *
 * Add @type to the list of excluded event types.
 */
void
matrix_api_filter_rules_add_excluded_type(MatrixAPIFilterRules *rules,
                                          const gchar *type)
{
    g_return_if_fail(type != NULL);

    if (!g_list_find_custom(rules->excluded_types, type,
                            (GCompareFunc)g_strcmp0)) {
        rules->excluded_types = g_list_prepend(rules->excluded_types,
                                               g_strdup(type));
    }
}

/**
 * matrix_api_filter_rules_delete_excluded_type:
 * @rules: a #MatrixAPIFilterRules
 * @type: (in): the event type to be removed from the excluded types
 *        list
 *
 * Remove @type from the list of event types to be excluded from the
 * filtered event list.
 */
void
matrix_api_filter_rules_delete_excluded_type(MatrixAPIFilterRules *rules,
                                             const gchar *type)
{
    GList *type_element;

    g_return_if_fail(type != NULL);

    while (type_element = g_list_find_custom(rules->excluded_types, type,
                                             (GCompareFunc)g_strcmp0)) {
        rules->excluded_types = g_list_remove_link(rules->excluded_types,
                                                   type_element);
        g_list_free_full(type_element, g_free);
    }
}

/**
 * matrix_api_filter_rules_get_excluded_types:
 * @rules: a #MatrixAPIFilterRules
 *
 * Get the list of event types that will be excluded from the filtered
 * events.
 *
 * Returns: (element-type utf8) (allow-none) (transfer none): the
 *          list of event types. The returned value is owned by
 *          @rules and should not be freed nor modified
 */
const GList *
matrix_api_filter_rules_get_excluded_types(MatrixAPIFilterRules *rules)
{
    return rules->excluded_types;
}

static void
json_add_string(gchar *str, JsonBuilder *builder)
{
    json_builder_add_string_value(builder, str);
}

/**
 * matrix_api_filter_rules_get_json_node:
 * @rules: a #MatrixAPIFilterRules
 *
 * Gets the #JsonNode representation of this filtering ruleset.
 *
 * Returns: (transfer full): the JSON representation of the filtering
 *          data as a #JsonNode
 */
JsonNode *
matrix_api_filter_rules_get_json_node(MatrixAPIFilterRules *rules)
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
 * matrix_api_filter_rules_get_json_data:
 * @rules: a #MatrixAPIFilterRules
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
matrix_api_filter_rules_get_json_data(MatrixAPIFilterRules *rules,
                                      gsize *datalen)
{
    JsonGenerator *generator;
    JsonNode *node = matrix_api_filter_rules_get_json_node(rules);
    gchar *data;

    generator = json_generator_new();
    json_generator_set_root(generator, node);
    json_node_free(node);

    data = json_generator_to_data(generator, datalen);
    g_object_unref(generator);

    return data;
}

/**
 * MatrixAPIRoomFilter:
 *
 * An opaque structure to hold room filters.
 */
struct _MatrixAPIRoomFilter {
    gboolean include_leave;
    MatrixAPIFilterRules *ephemeral;
    MatrixAPIFilterRules *state;
    MatrixAPIFilterRules *timeline;
    guint refcount;
};

G_DEFINE_BOXED_TYPE(MatrixAPIRoomFilter, matrix_api_room_filter,
                    (GBoxedCopyFunc)matrix_api_room_filter_ref,
                    (GBoxedFreeFunc)matrix_api_room_filter_unref);

/**
 * matrix_api_room_filter_new:
 *
 * Create a new #MatrixAPIRoomFilter object with reference count of 1.
 *
 * Returns: (transfer full): a new #MatrixAPIRoomFilter
 */
MatrixAPIRoomFilter *
matrix_api_room_filter_new(void)
{
    MatrixAPIRoomFilter *filter;

    filter = g_new0(MatrixAPIRoomFilter, 1);
    filter->refcount = 1;

    return filter;
}

static void
matrix_api_room_filter_free(MatrixAPIRoomFilter *filter)
{
    if (filter->ephemeral) {
        matrix_api_filter_rules_unref(filter->ephemeral);
    }

    if (filter->state) {
        matrix_api_filter_rules_unref(filter->state);
    }

    if (filter->timeline) {
        matrix_api_filter_rules_unref(filter->timeline);
    }

    g_free(filter);
}

/**
 * matrix_api_room_filter_ref:
 * @filter: a #MatrixAPIRoomFilter
 *
 * Increase reference count of @filter by one.
 *
 * Returns: (transfer none): the same #MatrixAPIRoomFilter
 */
MatrixAPIRoomFilter *
matrix_api_room_filter_ref(MatrixAPIRoomFilter *filter)
{
    filter->refcount++;

    return filter;
}

/**
 * matrix_api_room_filter_unref:
 * @filter: a #MatrixAPIRoomFilter
 *
 * Decrease reference count of @filter by one. If reference count
 * reaches zero, @filter is freed.
 */
void
matrix_api_room_filter_unref(MatrixAPIRoomFilter *filter)
{
    if (--filter->refcount == 0) {
        matrix_api_room_filter_free(filter);
    }
}

/**
 * matrix_api_room_filter_set_include_leave:
 * @filter: a #MatrixAPIRoomFilter
 * @include_leave: %TRUE if events from left rooms should be included
 *
 * Sets if events for rooms that the user has left should be included
 * in the filtered event list.
 */
void
matrix_api_room_filter_set_include_leave(MatrixAPIRoomFilter *filter,
                                         gboolean include_leave)
{
    filter->include_leave = include_leave;
}

/**
 * matrix_api_room_filter_get_include_leave:
 * @filter: a #MatrixAPIRoomFilter
 *
 * If %TRUE, events from rooms that the user has left will be included
 * in the filtered event list.
 *
 * Returns: the value of <code>include_leave</code>
 */
gboolean
matrix_api_room_filter_get_include_leave(MatrixAPIRoomFilter *filter)
{
    return filter->include_leave;
}

/**
 * matrix_api_room_filter_set_ephemeral:
 * @filter: a #MatrixAPIRoomFilter
 * @rules: (transfer none): filtering rules for ephemeral events
 *
 * Set filtering rules for ephemeral events, i.e. events that are not
 * recorded in the room history (typing notifications, receipts, etc.)
 * @filter gets a reference on @rules, so the caller may unref it.
 */
void
matrix_api_room_filter_set_ephemeral(MatrixAPIRoomFilter *filter,
                                     MatrixAPIFilterRules *rules)
{
    if (filter->ephemeral) {
        matrix_api_filter_rules_unref(filter->ephemeral);
    }

    filter->ephemeral = matrix_api_filter_rules_ref(rules);
}

/**
 * matrix_api_room_filter_get_ephemeral:
 * @filter: a #MatrixAPIRoomFilter
 *
 * Gets the filtering rules for ephemeral events. See
 * matrix_api_room_filter_set_ephemeral() for details.
 *
 * Returns: (transfer none): the filtering rules for ephemeral events.
 */
MatrixAPIFilterRules *
matrix_api_room_filter_get_ephemeral(MatrixAPIRoomFilter *filter)
{
    return filter->ephemeral;
}

/**
 * matrix_api_room_filter_set_state:
 * @filter: a #MatrixAPIRoomFilter
 * @rules: (transfer none): filtering rules for state events
 *
 * Sets filtering rules for state events. @filter obtains a reference
 * on @rules, so the caller may unref it safely.
 */
void
matrix_api_room_filter_set_state(MatrixAPIRoomFilter *filter,
                                 MatrixAPIFilterRules *rules)
{
    if (filter->state) {
        matrix_api_filter_rules_unref(filter->state);
    }

    filter->state = matrix_api_filter_rules_ref(rules);
}

/**
 * matrix_api_room_filter_get_state:
 * @filter: a #MatrixAPIRoomFilter
 *
 * Gets the filtering rules for state events.
 *
 * Returns: (transfer none): the filtering rules for state events
 */
MatrixAPIFilterRules *
matrix_api_room_filter_get_state(MatrixAPIRoomFilter *filter)
{
    return filter->state;
}

/**
 * matrix_api_room_filter_set_timeline:
 * @filter: a #MatrixAPIRoomFilter
 * @rules: (transfer none): filtering rules for timeline events
 *         (messages and state events from rooms)
 *
 * Set filtering rules for timeline events.
 */
void
matrix_api_room_filter_set_timeline(MatrixAPIRoomFilter *filter,
                                    MatrixAPIFilterRules *rules)
{
    if (filter->timeline) {
        matrix_api_filter_rules_unref(filter->timeline);
    }

    filter->timeline = matrix_api_filter_rules_ref(rules);
}

/**
 * matrix_api_room_filter_get_timeline:
 * @filter: a #MatrixAPIRoomFilter
 *
 * Gets filtering rules for timeline events (see
 * matrix_api_room_filter_set_timeline() for details.
 *
 * Returns: (transfer none): the filtering rules for timeline events
 */
MatrixAPIFilterRules *
matrix_api_room_filter_get_timeline(MatrixAPIRoomFilter *filter)
{
    return filter->timeline;
}

/**
 * matrix_api_room_filter_get_json_node:
 * @filter: a #MatrixAPIRoomFilter
 *
 * Gets the #JsonNode representation of the filtering ruleset.
 *
 * Returns: (transfer full): the JSON representation of the filtering
 *          ruleset as a #JsonNode
 */
JsonNode *
matrix_api_room_filter_get_json_node(MatrixAPIRoomFilter *filter)
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
                               matrix_api_filter_rules_get_json_node(
                                       filter->ephemeral));
    }

    if (filter->state) {
        json_builder_set_member_name(builder, "state");
        json_builder_add_value(builder,
                               matrix_api_filter_rules_get_json_node(
                                       filter->state));
    }

    if (filter->timeline) {
        json_builder_set_member_name(builder, "timeline");
        json_builder_add_value(builder,
                               matrix_api_filter_rules_get_json_node(
                                       filter->timeline));
    }

    json_builder_end_object(builder);

    node = json_builder_get_root(builder);
    g_object_unref(builder);

    return node;
}

/**
 * matrix_api_room_filter_get_json_data:
 * @filter: a #MatrixAPIRoomFilter
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
matrix_api_room_filter_get_json_data(MatrixAPIRoomFilter *filter,
                                     gsize *datalen)
{
    JsonGenerator *generator;
    JsonNode *node = matrix_api_room_filter_get_json_node(filter);
    gchar *data;

    generator = json_generator_new();
    json_generator_set_root(generator, node);
    json_node_free(node);

    data = json_generator_to_data(generator, datalen);
    g_object_unref(generator);

    return data;
}

/**
 * MatrixAPIFilter:
 *
 * An opaque structure to hold an event filter.
 */
struct _MatrixAPIFilter {
    GList *event_fields;
    MatrixAPIEventFormat event_format;
    MatrixAPIFilterRules *presence;
    MatrixAPIRoomFilter *room;
    guint refcount;
};

G_DEFINE_BOXED_TYPE(MatrixAPIFilter, matrix_api_filter,
                    (GBoxedCopyFunc)matrix_api_filter_ref,
                    (GBoxedFreeFunc)matrix_api_filter_unref);

/**
 * matrix_api_filter_new:
 *
 * Create a new #MatrixAPIFilter object with reference count of 1.
 *
 * Returns: (transfer full): a new #MatrixAPIFilter
 */
MatrixAPIFilter *
matrix_api_filter_new(void)
{
    MatrixAPIFilter *filter;

    filter = g_new0(MatrixAPIFilter, 1);
    filter->refcount = 1;

    return filter;
}

static void
matrix_api_filter_free(MatrixAPIFilter *filter)
{
    g_list_free_full(filter->event_fields, g_free);

    if (filter->presence) {
        matrix_api_filter_rules_unref(filter->presence);
    }

    if (filter->room) {
        matrix_api_room_filter_unref(filter->room);
    }

    g_free(filter);
}

/**
 * matrix_api_filter_ref:
 * @filter: a #MatrixAPIFilter
 *
 * Increase reference count of @filter by one.
 *
 * Returns: (transfer none): the same #MatrixAPIFilter
 */
MatrixAPIFilter *
matrix_api_filter_ref(MatrixAPIFilter *filter)
{
    filter->refcount++;

    return filter;
}

/**
 * matrix_api_filter_unref:
 * @filter: a #MatrixAPIFilter
 *
 * Decrease reference count of @filter by one. If reference count
 * reaches zero, @filter is freed.
 */
void
matrix_api_filter_unref(MatrixAPIFilter *filter)
{
    if (--filter->refcount == 0) {
        matrix_api_filter_free(filter);
    }
}

/**
 * matrix_api_filter_set_event_fields:
 * @filter: a #MatrixAPIFilter
 * @event_fields: (in) (element-type utf8) (transfer full) (allow-none):
 *                a list of event fields to include. If %NULL then all
 *                fields are included. The entries may include
 *                <code>.</code> charaters to indicate sub-fields. So
 *                <code>['content.body']</code> will include the
 *                <code>body</code> field of the <code>content</code>
 *                object. A literal <code>.</code> character in a
 *                field name may be escaped using a <code>\</code>. A
 *                server may include more fields than were requested
 *
 * Set the event fields to include in the filtered events.
 */
void
matrix_api_filter_set_event_fields(MatrixAPIFilter *filter,
                                   GList *event_fields)
{
    g_list_free_full(filter->event_fields, g_free);
    filter->event_fields = event_fields;
}

/**
 * matrix_api_filter_add_event_field:
 * @filter: a #MatrixAPIFilter
 * @event_field: an event field to add to the list. See
 *               matrix_api_filter_set_event_fields() for details
 *
 * Add an event field to the list of fields that will be present in
 * the filtered events.
 */
void
matrix_api_filter_add_event_field(MatrixAPIFilter *filter,
                                  const gchar *event_field)
{
    g_return_if_fail(event_field != NULL);

    if (!g_list_find_custom(filter->event_fields, event_field,
                            (GCompareFunc)g_strcmp0)) {
        filter->event_fields = g_list_prepend(filter->event_fields,
                                              g_strdup(event_field));
    }
}

/**
 * matrix_api_filter_delete_event_field:
 * @filter: a #MatrixAPIFilter
 * @event_field: an event field to remove from the list
 *
 * Remove @event_field from the list of fields that will be present in
 * the filtered events.
 */
void
matrix_api_filter_delete_event_field(MatrixAPIFilter *filter,
                                     const gchar *event_field)
{
    GList *event_field_element;

    g_return_if_fail(event_field != NULL);

    while (event_field_element = g_list_find_custom(filter->event_fields,
                                                    event_field,
                                                    (GCompareFunc)g_strcmp0)) {
        filter->event_fields = g_list_remove_link(filter->event_fields,
                                                  event_field_element);
        g_list_free_full(event_field_element, g_free);
    }
}

/**
 * matrix_api_filter_get_event_fields:
 * @filter: a #MatrixAPIFilter
 *
 * Get the list of event fields that will be present in the filtered
 * events.
 *
 * Returns: (transfer none) (element-type utf8) (allow-none): the list
 *          of event fields. The returned value is owned by @filter
 *          and should not be freed nor modified.
 */
const GList *
matrix_api_filter_get_event_fields(MatrixAPIFilter *filter)
{
    return filter->event_fields;
}

/**
 * matrix_api_filter_set_event_format:
 * @filter: a #MatrixAPIFilter
 * @event_format: the desired event format for filtered events
 *
 * Set the desired event format for the filtered events (e.g. for
 * matrix_api_sync())
 */
void
matrix_api_filter_set_event_format(MatrixAPIFilter *filter,
                                   MatrixAPIEventFormat event_format)
{
    filter->event_format = event_format;
}

/**
 * matrix_api_filter_get_event_format:
 * @filter: a #MatrixAPIFilter
 *
 * Get the desired event format set in @filter.
 *
 * Returns: the event format currently set
 */
MatrixAPIEventFormat
matrix_api_filter_get_event_format(MatrixAPIFilter *filter)
{
    return filter->event_format;
}

/**
 * matrix_api_filter_set_presence_filter:
 * @filter: a #MatrixAPIFilter
 * @presence_filter: (transfer none): the desired filters to use
 *
 * Set a filtering ruleset for presence events.
 */
void
matrix_api_filter_set_presence_filter(MatrixAPIFilter *filter,
                                      MatrixAPIFilterRules *presence_filter)
{
    if (filter->presence) {
        matrix_api_filter_rules_unref(filter->presence);
    }

    filter->presence = matrix_api_filter_rules_ref(presence_filter);
}

/**
 * matrix_api_filter_get_presence_filter:
 * @filter: a #MatrixAPIFilter
 *
 * Get the current filtering ruleset for presence events.
 *
 * Returns: (transfer none): the current ruleset. The returned value
 *          is owned by @filter; if the callee wants to use it
 *          separately, it should create a reference for it
 */
MatrixAPIFilterRules *
matrix_api_filter_get_presence_filter(MatrixAPIFilter *filter)
{
    return filter->presence;
}

/**
 * matrix_api_filter_set_room_filter:
 * @filter: a #MatrixAPIFilter
 * @room_filter: the desired room filters to use in @filter
 *
 * Set a new filtering ruleset for room events in @filter.
 */
void
matrix_api_filter_set_room_filter(MatrixAPIFilter *filter,
                                  MatrixAPIRoomFilter *room_filter)
{
    if (filter->room) {
        matrix_api_room_filter_unref(filter->room);
    }

    filter->room = matrix_api_room_filter_ref(room_filter);
}

/**
 * matrix_api_filter_get_room_filter:
 * @filter: a #MatrixAPIFilter
 *
 * Get the filtering ruleset for room events in @filter.
 *
 * Returns: (transfer none): the current filtering ruleset for room
 * events
 */
MatrixAPIRoomFilter *
matrix_api_filter_get_room_filter(MatrixAPIFilter *filter)
{
    return filter->room;
}

/**
 * matrix_api_filter_get_json_node:
 * @filter: a #MatrixAPIFilter
 *
 * Get the JSON representation of @filter as a #JsonNode
 *
 * Returns: (transfer full): the JSON representation of @filter
 */
JsonNode *
matrix_api_filter_get_json_node(MatrixAPIFilter *filter)
{
    JsonBuilder *builder;
    JsonNode *root, *tmp;

    builder = json_builder_new();
    json_builder_begin_object(builder);

    json_builder_set_member_name(builder, "event_fields");
    json_builder_begin_array(builder);
    g_list_foreach(filter->event_fields, (GFunc)json_add_string, builder);
    json_builder_end_array(builder);

    json_builder_set_member_name(builder, "event_format");
    json_builder_add_string_value(builder,
                                  g_enum_to_string(
                                          MATRIX_TYPE_API_EVENT_FORMAT,
                                          filter->event_format,
                                          TRUE));

    json_builder_set_member_name(builder, "presence");
    tmp = matrix_api_filter_rules_get_json_node(filter->presence);
    json_builder_add_value(builder, tmp);
    json_node_free(tmp);

    json_builder_set_member_name(builder, "room");
    tmp = matrix_api_room_filter_get_json_node(filter->room);
    json_builder_add_value(builder, tmp);
    json_node_free(tmp);

    json_builder_end_object(builder);
    root = json_builder_get_root(builder);
    g_object_unref(builder);

    return root;
}

gchar *
matrix_api_filter_get_json_data(MatrixAPIFilter *filter, gsize *datalen)
{
    JsonGenerator *generator;
    JsonNode *node = matrix_api_filter_get_json_node(filter);
    gchar *data;

    generator = json_generator_new();
    json_generator_set_root(generator, node);
    json_node_free(node);

    data = json_generator_to_data(generator, datalen);
    g_object_unref(generator);

    return data;
}

/**
 * MatrixAPI3PidCredential:
 *
 * An opaque structure to store credentials to use with Identity
 * Server communication.
 */
struct _MatrixAPI3PidCredential {
    gchar *id_server;
    gchar *session_id;
    gchar *client_secret;
    guint refcount;
};

G_DEFINE_BOXED_TYPE(MatrixAPI3PidCredential, matrix_api_3pid_credential,
                    (GBoxedCopyFunc)matrix_api_3pid_credential_ref,
                    (GBoxedFreeFunc)matrix_api_3pid_credential_unref);

/**
 * matrix_api_3pid_credential_new:
 *
 * Create a new #MatrixAPI3PidCredential object with reference count
 * of 1.
 *
 * Returns: (transfer full): a new #MatrixAPI3PidCredential
 */
MatrixAPI3PidCredential *
matrix_api_3pid_credential_new(void)
{
    MatrixAPI3PidCredential *credential;

    credential = g_new0(MatrixAPI3PidCredential, 1);
    credential->refcount = 1;

    return credential;
}

static void
matrix_api_3pid_credential_free(MatrixAPI3PidCredential *credential)
{
    g_free(credential->id_server);
    g_free(credential->session_id);
    g_free(credential->client_secret);
    g_free(credential);
}

/**
 * matrix_api_3pid_credential_ref:
 * @credential: a #MatrixAPI3PidCredential
 *
 * Increase reference count of @credential by one.
 *
 * Returns: (transfer none): the same #MatrixAPI3PidCredential
 */
MatrixAPI3PidCredential *
matrix_api_3pid_credential_ref(MatrixAPI3PidCredential *credential)
{
    credential->refcount++;

    return credential;
}

/**
 * matrix_api_3pid_credential_unref:
 * @credential: a #MatrixAPI3PidCredential
 *
 * Decrease reference count of @credential by one. If reference count
 * reaches zero, @credential gets freed.
 */
void
matrix_api_3pid_credential_unref(MatrixAPI3PidCredential *credential)
{
    if (--credential->refcount == 0) {
        matrix_api_3pid_credential_free(credential);
    }
}

/**
 * matrix_api_3pid_credential_set_id_server:
 * @credential: a #MatrixAPI3PidCredential
 * @id_server: the Identity Server to use
 *
 * Set the Identity Server to use for this credential.
 */
void
matrix_api_3pid_credential_set_id_server(MatrixAPI3PidCredential *credential,
                                         const gchar *id_server)
{
    g_free(credential->id_server);
    credential->id_server = g_strdup(id_server);
}

/**
 * matrix_api_3pid_credential_get_id_server:
 * @credential: a #MatrixAPI3PidCredential
 *
 * Get the Identity Server used for this credential.
 *
 * Returns: the Identity Server's name. The returned value is owned by
 *          @credential and should not be freed nor modified
 */
const gchar *
matrix_api_3pid_credential_get_id_server(MatrixAPI3PidCredential *credential)
{
    return credential->id_server;
}

/**
 * matrix_api_3pid_credential_set_session_id:
 * @credential: a #MatrixAPI3PidCredential
 * @session_id: the session identifier given by the Identity Server
 *
 * Set the session identifier got from the Identity Server.
 */
void
matrix_api_3pid_credential_set_session_id(MatrixAPI3PidCredential *credential,
                                          const gchar *session_id)
{
    g_free(credential->session_id);
    credential->session_id = g_strdup(session_id);
}

/**
 * matrix_api_3pid_credential_get_session_id:
 * @credential: a #MatrixAPI3PidCredential
 *
 * Get the session identifier for this credential.
 *
 * Returns: the session identifier. The returned value is owned by
 *          @credential and should not be freed nor modified.
 */
const gchar *
matrix_api_3pid_credential_get_session_id(MatrixAPI3PidCredential *credential)
{
    return credential->session_id;
}

/**
 * matrix_api_3pid_credential_set_client_secret:
 * @credential: a #MatrixAPI3PidCredential
 * @client_secret: the client secret used in the session with the
 *                 Identity Server
 *
 * Set the client secret that was used in the session with the
 * Identity Server.
 */
void
matrix_api_3pid_credential_set_client_secret(MatrixAPI3PidCredential *credential,
                                             const gchar *client_secret)
{
    g_free(credential->client_secret);
    credential->client_secret = g_strdup(client_secret);
}

/**
 * matrix_api_3pid_credential_get_client_secret:
 * @credential: a #MatrixAPI3PidCredential
 *
 * Get the client secret that was used in the session with the
 * Identity Server.
 *
 * Returns: the client secret. The returned value is owned by
 *          @credential and should not be freed nor modified.
 */
const gchar *
matrix_api_3pid_credential_get_client_secret(MatrixAPI3PidCredential *credential)
{
    return credential->client_secret;
}

/**
 * MatrixAPIPusher:
 *
 * An opaque structure for pusher rulesets.
 */
typedef struct _MatrixAPIPusher {
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
} MatrixAPIPusher;

G_DEFINE_BOXED_TYPE(MatrixAPIPusher, matrix_api_pusher,
                    (GBoxedCopyFunc)matrix_api_pusher_ref,
                    (GBoxedFreeFunc)matrix_api_pusher_unref);

/**
 * matrix_api_pusher_new:
 *
 * Create a new #MatrixAPIPusher object with reference count of 1.
 *
 * Returns: (transfer full): a new #MatrixAPIPusher
 */
MatrixAPIPusher *
matrix_api_pusher_new(void)
{
    MatrixAPIPusher *pusher;

    pusher = g_new0(MatrixAPIPusher, 1);
    pusher->refcount = 1;

    return pusher;
}

static void
matrix_api_pusher_free(MatrixAPIPusher *pusher)
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
 * matrix_api_pusher_ref:
 * @pusher: a #MatrixAPIPusher
 *
 * Increase reference count of @pusher by one.
 *
 * Returns: (transfer none): the same #MatrixAPIPusher
 */
MatrixAPIPusher *
matrix_api_pusher_ref(MatrixAPIPusher *pusher)
{
    pusher->refcount++;

    return pusher;
}

/**
 * matrix_api_pusher_unref:
 * @pusher: a #MatrixAPIPusher
 *
 * Decrease reference count of @pusher by one. If reference count
 * reaches zero, @pusher is freed.
 */
void
matrix_api_pusher_unref(MatrixAPIPusher *pusher)
{
    if (--pusher->refcount == 0) {
        matrix_api_pusher_free(pusher);
    }
}

/**
 * matrix_api_pusher_set_device_display_name:
 * @pusher: a #MatrixAPIPusher
 * @device_display_name: a string that will allow the user to identify
 *                       what device owns this pusher
 *
 * Set the device display name for @pusher.
 */
void
matrix_api_pusher_set_device_display_name(MatrixAPIPusher *pusher,
                                          const gchar *device_display_name)
{
    g_free(pusher->device_display_name);
    pusher->device_display_name = g_strdup(device_display_name);
}

/**
 * matrix_api_pusher_get_device_display_name:
 * @pusher: a #MatrixAPIPusher
 *
 * Get the device display name from pusher.
 *
 * Returns: the device display name. The returned value is owned by
 *          @pusher and should not be freed nor modified.
 */
const gchar *
matrix_api_pusher_get_device_display_name(MatrixAPIPusher *pusher)
{
    return pusher->device_display_name;
}

/**
 * matrix_api_pusher_set_app_display_name:
 * @pusher: a #MatrixAPIPusher
 * @app_display_name: a string that will allow the user to identify
 *                    what application owns the pusher
 *
 * Sets the app display name for the pusher.
 */
void
matrix_api_pusher_set_app_display_name(MatrixAPIPusher *pusher,
                                       const gchar *app_display_name)
{
    g_free(pusher->app_display_name);
    pusher->app_display_name = g_strdup(app_display_name);
}

/**
 * matrix_api_pusher_get_app_display_name:
 * @pusher: a #MatrixAPIPusher
 *
 * Get the app display name for this pusher.
 *
 * Returns: the app display name. The returned value is owned by
 *          @pusher and should not be freed nor modified.
 */
const gchar *
matrix_api_pusher_get_app_display_name(MatrixAPIPusher *pusher)
{
    return pusher->app_display_name;
}

/**
 * matrix_api_pusher_set_app_id:
 * @pusher: a #MatrixAPIPusher
 * @app_id: a reverse DNS style identifier for the application. It is
 *          recommended that this ends with the platform, such that
 *          different platform versions get different app
 *          identifiers. Maximum length is 64 characters. If @app_id
 *          is longer than this, it will be truncated.
 *
 * Sets the application ID for this pusher.
 */
void
matrix_api_pusher_set_app_id(MatrixAPIPusher *pusher,
                             const gchar *app_id)
{
    g_free(pusher->app_id);
    pusher->app_id = g_strndup(app_id, 64);
}

/**
 * matrix_api_pusher_get_app_id:
 * @pusher: a #MatrixAPIPusher
 *
 * Get the application ID for this pusher.
 *
 * Returns: the application ID. The returned value is owned by @pusher
 *          and should not be freed nor modified.
 */
const gchar *
matrix_api_pusher_get_app_id(MatrixAPIPusher *pusher)
{
    return pusher->app_id;
}

/**
 * matrix_api_pusher_set_append:
 * @pusher: a #MatrixAPIPusher
 * @append: if %TRUE, the homeserver should add another pusher with
 *          the given push key and app ID in addition to any others
 *          with different user IDs. Otherwise, the homeserver must
 *          remove any other pushers with the same App ID and pushkey
 *          for different users
 *
 * Set the appending rule for this pusher.
 */
void
matrix_api_pusher_set_append(MatrixAPIPusher *pusher, gboolean append)
{
    pusher->append = append;
}

/**
 * matrix_api_pusher_get_append:
 * @pusher: a #MatrixAPIPusher
 *
 * Get the appending rule for this pusher. See
 * matrix_api_pusher_set_append() for details.
 *
 * Returns: the append rule
 */
gboolean
matrix_api_pusher_get_append(MatrixAPIPusher *pusher)
{
    return pusher->append;
}

/**
 * matrix_api_pusher_set_kind:
 * @pusher: a #MatrixAPIPusher
 * @kind: the kind of pusher to configure. "http" makes a pusher that
 *        sends HTTP pokes. %NULL deletes the pusher.
 *
 * Set the kind of pusher to configure.
 */
void
matrix_api_pusher_set_kind(MatrixAPIPusher *pusher, const gchar *kind)
{
    g_free(pusher->kind);
    pusher->kind = g_strdup(kind);
}

/**
 * matrix_api_pusher_get_kind:
 * @pusher: a #MatrixAPIPusher
 *
 * Get the kind of pusher in @pusher.
 *
 * Returns: the kind of this pusher. The returned value is owned by
 *          @pusher and should not be freed nor modified
 */
const gchar *
matrix_api_pusher_get_kind(MatrixAPIPusher *pusher)
{
    return pusher->kind;
}

/**
 * matrix_api_pusher_set_lang:
 * @pusher: a #MatrixAPIPusher
 * @lang: the preferred language for receiving notifications,
 *        e.g. "en" or "en-US"
 *
 * Set the preferred language for receiving notifications.
 */
void
matrix_api_pusher_set_lang(MatrixAPIPusher *pusher, const gchar *lang)
{
    g_free(pusher->lang);
    pusher->lang = g_strdup(lang);
}

/**
 * matrix_api_pusher_get_lang:
 * @pusher: a #MatrixAPIPusher
 *
 * Get the preferred language for receiving notifications.
 *
 * Returns: the preferred language. The returned value is owned by
 *          @pusher and should not be modified nor freed
 */
const gchar *
matrix_api_pusher_get_lang(MatrixAPIPusher *pusher)
{
    return pusher->lang;
}

/**
 * matrix_api_pusher_set_profile_tag:
 * @pusher: a #MatrixAPIPusher
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
matrix_api_pusher_set_profile_tag(MatrixAPIPusher *pusher,
                                  const gchar *profile_tag)
{
    g_free(pusher->profile_tag);
    pusher->profile_tag = g_strndup(profile_tag, 32);
}

/**
 * matrix_api_pusher_get_profile_tag:
 * @pusher: a #MatrixAPIPusher
 *
 * Get the profile tag of this pusher.
 *
 * Returns: the profile tag. The returned value is owned by @pusher
 *          and should not be freed nor modified
 */
const gchar *
matrix_api_pusher_get_profile_tag(MatrixAPIPusher *pusher)
{
    return pusher->profile_tag;
}

/**
 * matrix_api_pusher_set_pushkey:
 * @pusher: a #MatrixAPIPusher
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
matrix_api_pusher_set_pushkey(MatrixAPIPusher *pusher, const gchar *pushkey)
{
    g_free(pusher->pushkey);
    pusher->pushkey = g_strndup(pushkey, 512);
}

/**
 * matrix_api_pusher_get_pushkey:
 * @pusher: a #MatrixAPIPusher
 *
 * Get the pushkey for this pusher.
 *
 * Returns: the pushkey. The returned value is owned by @pusher and
 *          should not be freed nor modified
 */
const gchar *
matrix_api_pusher_get_pushkey(MatrixAPIPusher *pusher)
{
    return pusher->pushkey;
}

/**
 * matrix_api_pusher_set_data:
 * @pusher: a #MatrixAPIPusher
 * @data: (transfer none): a dictionary of information for the pusher
 *        implementation itself. For example, if kind is "http", this
 *        should contain an "url" member, which is the URL to use to
 *        send notifications to. This function creates a deep copy of
 *        @data, so it can be freed after this call.
 *
 * Set some extra data for the pusher.
 */
void
matrix_api_pusher_set_data(MatrixAPIPusher *pusher, const JsonNode *data)
{
    if (pusher->data) {
        json_node_free(pusher->data);
    }

    pusher->data = json_node_copy((JsonNode *)data);
}

/**
 * matrix_api_pusher_take_data:
 * @pusher: a #MatrixAPIPusher
 * @data: (transfer full): extra data for the pusher. See
 *        matrix_api_pusher_set_data() for details.
 *
 * Set some extra data for the pusher. It differs from
 * matrix_api_pusher_set_data() that this call assumes ownership over
 * @data, so it should not be freed by the caller.
 */
void
matrix_api_pusher_take_data(MatrixAPIPusher *pusher, JsonNode *data)
{
    if (pusher->data) {
        json_node_free(pusher->data);
    }

    pusher->data = data;
}

/**
 * matrix_api_pusher_get_data:
 * @pusher: a #MatrixAPIPusher
 *
 * Get the extra data of this pusher.
 *
 * Returns: (transfer none): the extra data. The returned value is
 *          owned by @pusher and should not be freed nor modified
 */
const JsonNode *
matrix_api_pusher_get_data(MatrixAPIPusher *pusher)
{
    return pusher->data;
}

/**
 * matrix_api_pusher_get_json_node:
 * @pusher: a #MatrixAPIPusher
 * @err: a #GError
 *
 * Get the JSON representation of the pusher data as a #JsonNode
 * object. As in the pusher object all fields are mandatory, if any of
 * them is %NULL, this function returns %NULL, and fills @err with
 * %MATRIX_API_ERROR_INCOMPLETE.
 *
 * Returns: (transfer full): the JSON representation of the pusher
 *          data.
 */
JsonNode *
matrix_api_pusher_get_json_node(MatrixAPIPusher *pusher, GError **err)
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
                    MATRIX_API_ERROR, MATRIX_API_ERROR_INCOMPLETE,
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
 * matrix_api_pusher_get_json_data:
 * @pusher: a #MatrixAPIPusher
 * @datalen: (out): storage for the the length of the JSON data or
 *           %NULL
 * @err: a #GError
 *
 * Get the JSON representation of the pusher data as a string. As in
 * the pusher object all fields are mandatory, if any of them is
 * %NULL, this function returns %NULL, and fills @err with
 * %MATRIX_API_ERROR_INCOMPLETE.
 *
 * Returns: (transfer full): the JSON representation of the pusher
 *          data.
 */
gchar *matrix_api_pusher_get_json_data(MatrixAPIPusher *pusher,
                                       gsize *datalen,
                                       GError **err)
{
    JsonGenerator *generator;
    JsonNode *node;
    gchar *data;

    if ((node = matrix_api_pusher_get_json_node(pusher, err)) == NULL) {
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
 * MatrixAPIStateEvent:
 *
 * An opaque structure to hold a state event filter.
 */
struct _MatrixAPIStateEvent {
    gchar *type;
    gchar *state_key;
    JsonNode *content;
    guint refcount;
};

G_DEFINE_BOXED_TYPE(MatrixAPIStateEvent, matrix_api_state_event,
                    (GBoxedCopyFunc)matrix_api_state_event_ref,
                    (GBoxedFreeFunc)matrix_api_state_event_unref);

/**
 * matrix_api_state_event_new:
 *
 * Create a new #MatrixAPIStateEvent object with reference count of 1.
 *
 * Returns: (transfer full): a new #MatrixAPIStateEvent
 */
MatrixAPIStateEvent *
matrix_api_state_event_new(void)
{
    MatrixAPIStateEvent *event;

    event = g_new0(MatrixAPIStateEvent, 1);
    event->refcount = 1;

    return event;
}

static void
matrix_api_state_event_free(MatrixAPIStateEvent *event)
{
    g_free(event->type);
    g_free(event->state_key);

    if (event->content) {
        json_node_free(event->content);
    }

    g_free(event);
}

/**
 * matrix_api_state_event_ref:
 * @event: a #MatrixAPIStateEvent
 *
 * Increase reference count of @event by one.
 *
 * Returns: (transfer none): the same #MatrixAPIStateEvent
 */
MatrixAPIStateEvent *
matrix_api_state_event_ref(MatrixAPIStateEvent *event)
{
    event->refcount++;

    return event;
}

/**
 * matrix_api_state_event_unref:
 * @event: a #MatrixAPIStateEvent
 *
 * Decrease reference count of @event by one. If reference count
 * reaches zero, @event is freed.
 */
void
matrix_api_state_event_unref(MatrixAPIStateEvent *event)
{
    if (--event->refcount == 0) {
        matrix_api_state_event_free(event);
    }
}

/**
 * matrix_api_state_event_set_event_type:
 * @event: a #MatrixAPIStateEvent
 * @event_type: the type of the state event
 *
 * Set the type of the state event in @event.
 */
void
matrix_api_state_event_set_event_type(MatrixAPIStateEvent *event,
                                      const gchar *event_type)
{
    g_free(event->type);
    event->type = g_strdup(event_type);
}

/**
 * matrix_api_state_event_get_event_type:
 * @event: a #MatrixAPIStateEvent
 *
 * Get the type of the state event in @event.
 *
 * Returns: (transfer none): the event type. The returned value is
 *          owned by @event and should not be freed nor modified.
 */
const gchar *
matrix_api_state_event_get_event_type(MatrixAPIStateEvent *event)
{
    return event->type;
}

/**
 * matrix_api_state_event_set_state_key:
 * @event: a #MatrixAPIStateEvent
 * @state_key: the key of the state event
 *
 * Set the state key for the state event @event.
 */
void
matrix_api_state_event_set_state_key(MatrixAPIStateEvent *event,
                                     const gchar *state_key)
{
    g_free(event->state_key);
    event->state_key = g_strdup(state_key);
}

/**
 * matrix_api_state_event_get_state_key:
 * @event: a #MatrixAPIStateEvent
 *
 * Get the state key of @event.
 *
 * Returns: (transfer none): the state key. The returned value is
 *          owned by @event and should not be freed nor modified
 */
const gchar *
matrix_api_state_event_get_state_key(MatrixAPIStateEvent *event)
{
    return event->state_key;
}

/**
 * matrix_api_state_event_set_content:
 * @event: a #MatrixAPIStateEvent
 * @content: the desired content of the state event
 *
 * Set the content of the state event.
 */
void
matrix_api_state_event_set_content(MatrixAPIStateEvent *event,
                                   const JsonNode *content)
{
    if (event->content) {
        json_node_free(event->content);
    }

    event->content = json_node_copy((JsonNode *)content);
}

/**
 * matrix_api_state_event_get_content:
 * @event: a #MatrixAPIStateEvent
 *
 * Get the contents of the state event.
 *
 * Returns: (transfer none): the contents of the state event. The
 *          returned value is owned by @event and should not be freed
 *          nor modified
 */
const JsonNode *
matrix_api_state_event_get_content(MatrixAPIStateEvent *event)
{
    return event->content;
}

/**
 * matrix_api_state_event_get_json_node:
 * @event: a #MatrixAPIStateEvent
 *
 * Get the JSON representation of the state event as a #JsonNode
 * object.
 *
 * Returns: (transfer full): the JSON representation of the state
 *          event
 */
JsonNode *
matrix_api_state_event_get_json_node(MatrixAPIStateEvent *event)
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
 * matrix_api_state_event_get_json_data:
 * @event: a #MatrixAPIStateEvent
 * @datalen: (out): storage for the the length of the JSON data or
 *           %NULL
 *
 * Get the JSON representation of the state event as a string.
 *
 * Returns: (transfer full): the JSON representation of the state
 *          event
 */
gchar *
matrix_api_state_event_get_json_data(MatrixAPIStateEvent *event, gsize *datalen)
{
    JsonGenerator *generator;
    JsonNode *node = matrix_api_state_event_get_json_node(event);
    gchar *data;

    generator = json_generator_new();
    json_generator_set_root(generator, node);
    json_node_free(node);

    data = json_generator_to_data(generator, datalen);
    g_object_unref(generator);

    return data;
}
