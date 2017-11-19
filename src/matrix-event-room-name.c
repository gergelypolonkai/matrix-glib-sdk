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

#include "matrix-event-room-name.h"
#include "matrix-enumtypes.h"
#include "config.h"

/**
 * SECTION:matrix-event-room-name
 * @short_description: event to handle room name changes
 *
 * This is the default event handler for `m.room.name` events.
 *
 * A room has an opaque room ID which is not human-friendly to read.  A room alias is
 * human-friendly, but not all rooms have room aliases.  The room name is a human-friendly
 * string designed to be displayed to the end-user.  The room name is not unique, as multiple
 * rooms can have the same room name set.  The room name can also be set when creating a room
 * using matrix_api_create_room() with name set.
 */
enum  {
    PROP_0,
    PROP_NAME,
    NUM_PROPERTIES
};

static GParamSpec *matrix_event_room_name_properties[NUM_PROPERTIES];

typedef struct {
    gchar* _name;
} MatrixEventRoomNamePrivate;

/**
 * MatrixEventRoomName:
 */
G_DEFINE_TYPE_WITH_PRIVATE(MatrixEventRoomName, matrix_event_room_name, MATRIX_EVENT_TYPE_STATE);

static void
matrix_event_room_name_real_from_json(MatrixEventBase *matrix_event_base, JsonNode *json_data, GError **error)
{
    MatrixEventRoomNamePrivate *priv;
    JsonObject *root;
    JsonObject *content_root;
    JsonNode *content_node;
    JsonNode *node;

    g_return_if_fail (json_data != NULL);

    priv = matrix_event_room_name_get_instance_private(MATRIX_EVENT_ROOM_NAME(matrix_event_base));
    root = json_node_get_object(json_data);
    content_node = json_object_get_member(root, "content");
    content_root = json_node_get_object(content_node);

    if (DEBUG && ((node = json_object_get_member(root, "state_key")) != NULL)) {
        gchar *state_key = (gchar *)json_node_get_string(node);

        if ((state_key != NULL) && (*state_key == 0)) {
            g_warning("state_key of a m.room.name event is non-empty");
        }
    }

    if ((node = json_object_get_member(content_root, "name")) != NULL) {
        g_free(priv->_name);
        priv->_name = g_strdup(json_node_get_string(node));
    } else {
        g_warning("content.name is missing from a m.room.name event");
    }

    MATRIX_EVENT_BASE_CLASS(matrix_event_room_name_parent_class)->from_json(matrix_event_base, json_data, error);
}

static void
matrix_event_room_name_real_to_json(MatrixEventBase *matrix_event_base, JsonNode *json_data, GError **error)
{
    MatrixEventRoomNamePrivate *priv;
    JsonObject *root;
    JsonObject *content_root;
    JsonNode *content_node;
    const gchar *state_key;

    g_return_if_fail(json_data != NULL);

    priv = matrix_event_room_name_get_instance_private(MATRIX_EVENT_ROOM_NAME(matrix_event_base));
    root = json_node_get_object(json_data);
    content_node = json_object_get_member(root, "content");
    content_root = json_node_get_object(content_node);
    state_key = matrix_event_state_get_state_key(MATRIX_EVENT_STATE(matrix_event_base));

    if ((state_key == NULL) || (*state_key == 0)) {
        g_set_error(error, MATRIX_ERROR, MATRIX_ERROR_INCOMPLETE,
                    "Won't send a m.room.name with a non-empty state key");

        return;
    }

    if ((priv->_name == NULL) || (*(priv->_name) == 0)) {
        g_set_error(error, MATRIX_ERROR, MATRIX_ERROR_INCOMPLETE,
                    "Won't send a m.room.name event without a name");

        return;
    }

    json_object_set_string_member(content_root, "name", priv->_name);

    MATRIX_EVENT_BASE_CLASS(matrix_event_room_name_parent_class)->to_json(matrix_event_base, json_data, error);
}

MatrixEventRoomName *
matrix_event_room_name_construct(GType object_type)
{
    return (MatrixEventRoomName *)matrix_event_state_construct(object_type);
}

/**
 * matrix_event_room_name_new:
 *
 * Create a new #MatrixEventRoomName object.
 *
 * Returns: (transfer full): a new #MatrixEventRoomName object
 */
MatrixEventRoomName *
matrix_event_room_name_new(void)
{
    return matrix_event_room_name_construct(MATRIX_EVENT_TYPE_ROOM_NAME);
}

/**
 * matrix_event_room_name_get_name:
 * @event: a #MatrixEventRoomName
 *
 * Get the name from @event.
 *
 * The returned value is owned by @event and should not be freed.
 *
 * Returns: (transfer none) (nullable): the room name
 */
const gchar *
matrix_event_room_name_get_name(MatrixEventRoomName *matrix_event_room_name)
{
    MatrixEventRoomNamePrivate *priv;

    g_return_val_if_fail(matrix_event_room_name != NULL, NULL);

    priv = matrix_event_room_name_get_instance_private(matrix_event_room_name);

    return priv->_name;
}

/**
 * matrix_event_room_name_set_name:
 * @event: a #MatrixEventRoomName
 * @name: a room name
 *
 * Set the name in @event.
 */
void
matrix_event_room_name_set_name(MatrixEventRoomName *matrix_event_room_name, const gchar *name)
{
    MatrixEventRoomNamePrivate *priv;

    g_return_if_fail(matrix_event_room_name != NULL);

    priv = matrix_event_room_name_get_instance_private(matrix_event_room_name);

    if (g_strcmp0(name, priv->_name) != 0) {
        g_free(priv->_name);
        priv->_name = g_strdup(name);

        g_object_notify_by_pspec((GObject *)matrix_event_room_name, matrix_event_room_name_properties[PROP_NAME]);
    }
}

static void
matrix_event_room_name_finalize(GObject *gobject)
{
    MatrixEventRoomNamePrivate *priv = matrix_event_room_name_get_instance_private(MATRIX_EVENT_ROOM_NAME(gobject));

    g_free(priv->_name);

    G_OBJECT_CLASS(matrix_event_room_name_parent_class)->finalize(gobject);
}

static void
matrix_event_room_name_get_property(GObject *gobject, guint property_id, GValue *value, GParamSpec *pspec)
{
    MatrixEventRoomName *matrix_event_room_name = MATRIX_EVENT_ROOM_NAME(gobject);

    switch (property_id) {
        case PROP_NAME:
            g_value_set_string(value, matrix_event_room_name_get_name(matrix_event_room_name));

            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, property_id, pspec);

            break;
    }
}

static void
matrix_event_room_name_set_property(GObject *gobject, guint property_id, const GValue *value, GParamSpec *pspec)
{
    MatrixEventRoomName *matrix_event_room_name = MATRIX_EVENT_ROOM_NAME(gobject);

    switch (property_id) {
        case PROP_NAME:
            matrix_event_room_name_set_name(matrix_event_room_name, g_value_get_string(value));

            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, property_id, pspec);

            break;
    }
}

static void
matrix_event_room_name_class_init(MatrixEventRoomNameClass *klass)
{
    ((MatrixEventBaseClass *)klass)->from_json = matrix_event_room_name_real_from_json;
    ((MatrixEventBaseClass *)klass)->to_json = matrix_event_room_name_real_to_json;
    G_OBJECT_CLASS(klass)->get_property = matrix_event_room_name_get_property;
    G_OBJECT_CLASS(klass)->set_property = matrix_event_room_name_set_property;
    G_OBJECT_CLASS(klass)->finalize = matrix_event_room_name_finalize;

    /**
     * MatrixEventRoomName:name:
     *
     * The room name.
     */
    matrix_event_room_name_properties[PROP_NAME] = g_param_spec_string(
            "name", "name", "name",
            NULL,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_NAME, matrix_event_room_name_properties[PROP_NAME]);
}

static void
matrix_event_room_name_init(MatrixEventRoomName *matrix_event_room_name)
{
    MatrixEventRoomNamePrivate *priv = matrix_event_room_name_get_instance_private(matrix_event_room_name);

    priv->_name = NULL;
}
