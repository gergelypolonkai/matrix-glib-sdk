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

#include "matrix-event-room-create.h"
#include "matrix-types.h"
#include "config.h"

/**
 * SECTION:matrix-event-room-create
 * @short_description: event describing the creation of a room
 *
 * This is the default event handler for `m.room.create` events.
 *
 * This is the first event in a room and cannot be changed.  It acts as the root of all other
 * events.
 */
enum  {
    PROP_0,
    PROP_CREATOR,
    PROP_FEDERATE,
    NUM_PROPERTIES
};

static GParamSpec *matrix_event_room_create_properties[NUM_PROPERTIES];

typedef struct {
    gchar* _creator;
    gboolean _federate;
} MatrixEventRoomCreatePrivate;

/**
 * MatrixEventRoomCreate:
 */
G_DEFINE_TYPE_WITH_PRIVATE(MatrixEventRoomCreate, matrix_event_room_create, MATRIX_EVENT_TYPE_STATE);

static void
matrix_event_room_create_real_from_json(MatrixEventBase *matrix_event_base, JsonNode *json_data, GError **error)
{
    MatrixEventRoomCreatePrivate *priv;
    JsonObject *root;
    JsonObject *content_root;
    JsonNode *content_node;
    JsonNode *node;

    g_return_if_fail(json_data != NULL);

    priv = matrix_event_room_create_get_instance_private(MATRIX_EVENT_ROOM_CREATE(matrix_event_base));

    root = json_node_get_object(json_data);
    content_node = json_object_get_member(root, "content");
    content_root = json_node_get_object(content_node);

#if DEBUG
    if ((node = json_object_get_member(root, "state_key")) != NULL) {
        const gchar *state_key = json_node_get_string(node);

        if ((state_key == NULL) || (*state_key == 0)) {
            g_warning("state_key of a m.room.create event is non-empty");
        }
    }
#endif

    if ((node = json_object_get_member(content_root, "creator")) != NULL) {
        g_free(priv->_creator);
        priv->_creator = g_strdup(json_node_get_string(node));
    } else {
        g_warning("content.creator is missing from a m.room.create event");
    }

    if ((node = json_object_get_member(content_root, "m.federate")) != NULL) {
        priv->_federate = json_node_get_boolean(node);
    }

    MATRIX_EVENT_BASE_CLASS(matrix_event_room_create_parent_class)->from_json(matrix_event_base, json_data, error);
}

static void
matrix_event_room_create_real_to_json(MatrixEventBase *matrix_event_base, JsonNode *json_data, GError **error)
{
    MatrixEventRoomCreatePrivate *priv;
    JsonObject *root;
    JsonObject *content_root;
    JsonNode *content_node;
    const gchar *state_key;

    g_return_if_fail(json_data != NULL);

    priv = matrix_event_room_create_get_instance_private(MATRIX_EVENT_ROOM_CREATE(matrix_event_base));

    if (priv->_creator == NULL) {
        g_set_error(error, MATRIX_ERROR, MATRIX_ERROR_INCOMPLETE,
                    "Won't generate a m.room.create event without a creator key");

        return;
    }

    state_key = matrix_event_state_get_state_key(MATRIX_EVENT_STATE(matrix_event_base));

    if ((state_key == NULL) || (*state_key == 0)) {
        g_set_error(error, MATRIX_ERROR, MATRIX_ERROR_INCOMPLETE,
                    "Won't generater a m.root.create event with a non-empty state_key");

        return;
    }

    root = json_node_get_object(json_data);
    content_node = json_object_get_member(root, "content");
    content_root = json_node_get_object(content_node);

    json_object_set_string_member(content_root, "creator", priv->_creator);
    json_object_set_boolean_member(content_root, "m.federate", priv->_federate);

    MATRIX_EVENT_BASE_CLASS (matrix_event_room_create_parent_class)->to_json(matrix_event_base, json_data, error);
}

/**
 * matrix_event_room_create_new:
 *
 * Create a new #MatrixEventRoomCreate object.
 *
 * Returns: (transfer none): a new #MatrixEventRoomCreate object
 */
MatrixEventRoomCreate *
matrix_event_room_create_new(void)
{
    return (MatrixEventRoomCreate *)matrix_event_state_construct(MATRIX_EVENT_TYPE_ROOM_CREATE);
}

/**
 * matrix_event_room_create_get_creator:
 * @event: a #MatrixEventRoomCreate
 *
 * Get the creator of the room.
 *
 * The returned value is owned by @event and should not be freed.
 *
 * Returns: (transfer none) (nullable): the creator of the room
 */
const gchar *
matrix_event_room_create_get_creator(MatrixEventRoomCreate *matrix_event_room_create)
{
    MatrixEventRoomCreatePrivate *priv;

    g_return_val_if_fail(matrix_event_room_create != NULL, NULL);

    priv = matrix_event_room_create_get_instance_private(matrix_event_room_create);

    return priv->_creator;
}

/**
 * matrix_event_room_create_set_creator:
 * @event: a #MatrixEventRoomCreate
 * @creator: (transfer none) (nullable): the user ID of the room’s creator
 *
 * Set the creator of the room in @event.
 */
void
matrix_event_room_create_set_creator(MatrixEventRoomCreate *matrix_event_room_create, const gchar *creator)
{
    MatrixEventRoomCreatePrivate *priv;

    g_return_if_fail(matrix_event_room_create != NULL);

    priv = matrix_event_room_create_get_instance_private(matrix_event_room_create);

    if (g_strcmp0(creator, priv->_creator) != 0) {
        g_free(priv->_creator);
        priv->_creator = g_strdup(creator);

        g_object_notify_by_pspec((GObject *)matrix_event_room_create, matrix_event_room_create_properties[PROP_CREATOR]);
    }
}

/**
 * matrix_event_room_create_get_federate:
 * @event: a #MatrixEventRoomCreate
 *
 * Get the federation status of the room.  If this function returns %TRUE, the room may be
 * federated to other homeservers.
 *
 * Returns: the federation status of the room
 */
gboolean
matrix_event_room_create_get_federate(MatrixEventRoomCreate *matrix_event_room_create)
{
    MatrixEventRoomCreatePrivate *priv;

    g_return_val_if_fail(matrix_event_room_create != NULL, FALSE);

    priv = matrix_event_room_create_get_instance_private(matrix_event_room_create);

    return priv->_federate;
}

/**
 * matrix_event_room_create_set_federate:
 * @event: a #MatrixEventRoomCreate
 * @federate: a federation status
 *
 * Set the federation status of the room.  See matrix_event_room_create_get_federate() for
 * more information.
 */
void
matrix_event_room_create_set_federate(MatrixEventRoomCreate *matrix_event_room_create, gboolean federate)
{
    MatrixEventRoomCreatePrivate *priv;

    g_return_if_fail(matrix_event_room_create != NULL);

    priv = matrix_event_room_create_get_instance_private(matrix_event_room_create);

    if (priv->_federate != federate) {
        priv->_federate = federate;

        g_object_notify_by_pspec((GObject *)matrix_event_room_create, matrix_event_room_create_properties[PROP_FEDERATE]);
    }
}

static void
matrix_event_room_create_finalize(GObject *gobject)
{
    MatrixEventRoomCreatePrivate *priv = matrix_event_room_create_get_instance_private(MATRIX_EVENT_ROOM_CREATE(gobject));

    g_free(priv->_creator);

    G_OBJECT_CLASS(matrix_event_room_create_parent_class)->finalize(gobject);
}

static void
matrix_event_room_create_get_property(GObject *gobject, guint property_id, GValue *value, GParamSpec *pspec)
{
    MatrixEventRoomCreate *matrix_event_room_create = MATRIX_EVENT_ROOM_CREATE(gobject);

    switch (property_id) {
        case PROP_CREATOR:
            g_value_set_string(value, matrix_event_room_create_get_creator(matrix_event_room_create));

            break;
        case PROP_FEDERATE:
            g_value_set_boolean(value, matrix_event_room_create_get_federate(matrix_event_room_create));

            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, property_id, pspec);

            break;
    }
}

static void
matrix_event_room_create_set_property(GObject *gobject, guint property_id, const GValue *value, GParamSpec *pspec)
{
    MatrixEventRoomCreate *matrix_event_room_create = MATRIX_EVENT_ROOM_CREATE(gobject);

    switch (property_id) {
        case PROP_CREATOR:
            matrix_event_room_create_set_creator(matrix_event_room_create, g_value_get_string(value));

            break;
        case PROP_FEDERATE:
            matrix_event_room_create_set_federate(matrix_event_room_create, g_value_get_boolean(value));

            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, property_id, pspec);

            break;
    }
}

static void
matrix_event_room_create_class_init(MatrixEventRoomCreateClass *klass)
{
    ((MatrixEventBaseClass *)klass)->from_json = matrix_event_room_create_real_from_json;
    ((MatrixEventBaseClass *)klass)->to_json = matrix_event_room_create_real_to_json;
    G_OBJECT_CLASS(klass)->get_property = matrix_event_room_create_get_property;
    G_OBJECT_CLASS(klass)->set_property = matrix_event_room_create_set_property;
    G_OBJECT_CLASS(klass)->finalize = matrix_event_room_create_finalize;

    /**
     * MatrixEventRoomCreate:creator:
     *
     * The user_id of the room creator. This is set by the homeserver.
     */
    matrix_event_room_create_properties[PROP_CREATOR] = g_param_spec_string(
            "creator", "creator", "creator",
            NULL,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_CREATOR, matrix_event_room_create_properties[PROP_CREATOR]);

    /**
     * MatrixEventRoomCreate:federate:
     *
     * Whether users on other servers can join this room.  Defaults to %TRUE if key does not exist.
     */
    matrix_event_room_create_properties[PROP_FEDERATE] = g_param_spec_boolean(
            "federate", "federate", "federate",
            FALSE,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_FEDERATE, matrix_event_room_create_properties[PROP_FEDERATE]);
}

static void
matrix_event_room_create_init(MatrixEventRoomCreate *matrix_event_room_create)
{
    MatrixEventRoomCreatePrivate *priv = matrix_event_room_create_get_instance_private(matrix_event_room_create);

    priv->_creator = NULL;
    priv->_federate = FALSE;
}
