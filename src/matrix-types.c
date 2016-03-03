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
