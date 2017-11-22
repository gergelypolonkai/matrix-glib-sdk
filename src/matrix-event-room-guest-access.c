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

#include "matrix-event-room-guest-access.h"
#include "utils.h"
#include "matrix-enumtypes.h"

/**
 * SECTION:matrix-event-room-guest-access
 * @short_description: event describing guest access to a room
 *
 * This is the default event handler for `m.room.guest_access` events.
 *
 * This event controls whether guest users are allowed to join rooms.  If this event is absent,
 * servers should act as if it is present and has the `guest_access` value set to `forbidden`.
 */
enum  {
    PROP_0,
    PROP_GUEST_ACCESS,
    NUM_PROPERTIES
};

static GParamSpec *matrix_event_room_guest_access_properties[NUM_PROPERTIES];

typedef struct {
    MatrixGuestAccess _guest_access;
} MatrixEventRoomGuestAccessPrivate;

/**
 * MatrixEventRoomGuestAccess:
 */
G_DEFINE_TYPE_WITH_PRIVATE(MatrixEventRoomGuestAccess, matrix_event_room_guest_access, MATRIX_EVENT_TYPE_STATE);

static void
matrix_event_room_guest_access_real_from_json(MatrixEventBase *matrix_event_base, JsonNode *json_data, GError **error)
{
    MatrixEventRoomGuestAccessPrivate *priv;
    JsonObject *root;
    JsonObject *content_root;
    JsonNode *content_node;
    JsonNode *node;

    g_return_if_fail(json_data != NULL);

    priv = matrix_event_room_guest_access_get_instance_private(MATRIX_EVENT_ROOM_GUEST_ACCESS(matrix_event_base));
    root = json_node_get_object(json_data);
    content_node = json_object_get_member(root, "content");
    content_root = json_node_get_object(content_node);

#if DEBUG
    if ((node = json_object_get_member(root, "state_key")) != NULL) {
        const gchar *state_key = json_node_get_string(node);

        if ((state_key == NULL) || (*state_key == 0)) {
            g_warning("state_key of a m.room.guest_access is non-empty");
        }
    }
#endif

    if ((node = json_object_get_member(content_root, "guest_access")) != NULL) {
        GError *inner_error = NULL;
        MatrixGuestAccess guest_access = _matrix_g_enum_nick_to_value(MATRIX_TYPE_GUEST_ACCESS, json_node_get_string(node), &inner_error);

        if (inner_error != NULL) {
            priv->_guest_access = MATRIX_GUEST_ACCESS_UNKNOWN;

#if DEBUG
            g_warning("Unknown value '%s' in a m.room.guest_access event", json_node_get_string(node));
#endif
        } else {
            priv->_guest_access = guest_access;
        }
    } else {
        g_warning("content.guest_access is missing from a m.room.guest_access event");
    }

    MATRIX_EVENT_BASE_CLASS(matrix_event_room_guest_access_parent_class)->from_json(matrix_event_base, json_data, error);
}

static void
matrix_event_room_guest_access_real_to_json(MatrixEventBase *matrix_event_base, JsonNode *json_data, GError **error)
{
    MatrixEventRoomGuestAccessPrivate *priv;
    JsonObject *root;
    JsonObject *content_root;
    JsonNode *content_node;
    const gchar *state_key;
    gchar *guest_access;

    g_return_if_fail(json_data != NULL);

    priv = matrix_event_room_guest_access_get_instance_private(MATRIX_EVENT_ROOM_GUEST_ACCESS(matrix_event_base));
    root = json_node_get_object(json_data);
    content_node = json_object_get_member(root, "content");
    content_root = json_node_get_object(content_node);

    state_key = matrix_event_state_get_state_key(MATRIX_EVENT_STATE(matrix_event_base));

    if ((state_key == NULL) || (*state_key == 0)) {
        g_set_error(error, MATRIX_ERROR, MATRIX_ERROR_INCOMPLETE,
                    "Won't generate a m.room.guest_access event with a non-empty state key");

        return;
    }

    if (priv->_guest_access == MATRIX_GUEST_ACCESS_UNKNOWN) {
        g_set_error(error, MATRIX_ERROR, MATRIX_ERROR_UNKNOWN_VALUE,
                    "Won't generate a m.room.guest_access event with an unknown content.guest_access key");

        return;
    }

    guest_access = _matrix_g_enum_to_string(MATRIX_TYPE_GUEST_ACCESS, priv->_guest_access, '_');
    json_object_set_string_member(content_root, "guest_access", guest_access);
    g_free(guest_access);

    MATRIX_EVENT_BASE_CLASS(matrix_event_room_guest_access_parent_class)->to_json(matrix_event_base, json_data, error);
}

/**
 * matrix_event_room_guest_access_new:
 *
 * Create a new #MatrixEventRoomGuestAccess object.
 *
 * Returns: (transfer full): a new #MatrixEventRoomGuestAccess object
 */
MatrixEventRoomGuestAccess *
matrix_event_room_guest_access_new(void)
{
    return (MatrixEventRoomGuestAccess *)matrix_event_state_construct(MATRIX_EVENT_TYPE_ROOM_GUEST_ACCESS);
}

/**
 * matrix_event_room_guest_access_get_guest_access:
 * @event: a #MatrixEventRoomGuestAccess
 *
 * Get the guest access status of the room from @event.
 *
 * Returns: the guest access status
 */
MatrixGuestAccess
matrix_event_room_guest_access_get_guest_access(MatrixEventRoomGuestAccess *matrix_event_room_guest_access)
{
    MatrixEventRoomGuestAccessPrivate *priv;

    g_return_val_if_fail(matrix_event_room_guest_access != NULL, 0);

    priv = matrix_event_room_guest_access_get_instance_private(matrix_event_room_guest_access);

    return priv->_guest_access;
}

/**
 * matrix_event_room_guest_access_set_guest_access:
 * @event: a #MatrixEventRoomGuestAccess
 * @guest_access: a #MatrixGuestAccess value
 *
 * Set the guest access status for the room in @event.
 */
void
matrix_event_room_guest_access_set_guest_access(MatrixEventRoomGuestAccess *matrix_event_room_guest_access, MatrixGuestAccess guest_access)
{
    MatrixEventRoomGuestAccessPrivate *priv;

    g_return_if_fail(matrix_event_room_guest_access != NULL);

    priv = matrix_event_room_guest_access_get_instance_private(matrix_event_room_guest_access);

    if (priv->_guest_access != guest_access) {
        priv->_guest_access = guest_access;

        g_object_notify_by_pspec((GObject *)matrix_event_room_guest_access, matrix_event_room_guest_access_properties[PROP_GUEST_ACCESS]);
    }
}

static void
matrix_event_room_guest_access_finalize (GObject *gobject)
{
    G_OBJECT_CLASS(matrix_event_room_guest_access_parent_class)->finalize(gobject);
}

static void
matrix_event_room_guest_access_get_property(GObject *gobject, guint property_id, GValue *value, GParamSpec *pspec)
{
    MatrixEventRoomGuestAccess *matrix_event_room_guest_access = MATRIX_EVENT_ROOM_GUEST_ACCESS(gobject);

    switch (property_id) {
        case PROP_GUEST_ACCESS:
            g_value_set_enum(value, matrix_event_room_guest_access_get_guest_access(matrix_event_room_guest_access));

            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, property_id, pspec);

            break;
    }
}

static void
matrix_event_room_guest_access_set_property(GObject *gobject, guint property_id, const GValue *value, GParamSpec *pspec) {
    MatrixEventRoomGuestAccess *matrix_event_room_guest_access = MATRIX_EVENT_ROOM_GUEST_ACCESS(gobject);

    switch (property_id) {
        case PROP_GUEST_ACCESS:
            matrix_event_room_guest_access_set_guest_access(matrix_event_room_guest_access, g_value_get_enum(value));
            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, property_id, pspec);
        break;
    }
}

static void
matrix_event_room_guest_access_class_init(MatrixEventRoomGuestAccessClass *klass)
{
    ((MatrixEventBaseClass *)klass)->from_json = matrix_event_room_guest_access_real_from_json;
    ((MatrixEventBaseClass *)klass)->to_json = matrix_event_room_guest_access_real_to_json;
    G_OBJECT_CLASS(klass)->get_property = matrix_event_room_guest_access_get_property;
    G_OBJECT_CLASS(klass)->set_property = matrix_event_room_guest_access_set_property;
    G_OBJECT_CLASS(klass)->finalize = matrix_event_room_guest_access_finalize;

    /**
     * MatrixEventRoomGuestAccess:guest-access:
     *
     * Whether guests can join the room.
     */
    matrix_event_room_guest_access_properties[PROP_GUEST_ACCESS] = g_param_spec_enum(
            "guest-access", "guest-access", "guest-access",
            MATRIX_TYPE_GUEST_ACCESS, MATRIX_GUEST_ACCESS_UNKNOWN,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_GUEST_ACCESS, matrix_event_room_guest_access_properties[PROP_GUEST_ACCESS]);
}

static void
matrix_event_room_guest_access_init(MatrixEventRoomGuestAccess *matrix_event_room_guest_access)
{
    MatrixEventRoomGuestAccessPrivate *priv = matrix_event_room_guest_access_get_instance_private(matrix_event_room_guest_access);

    priv->_guest_access = MATRIX_GUEST_ACCESS_UNKNOWN;
}
