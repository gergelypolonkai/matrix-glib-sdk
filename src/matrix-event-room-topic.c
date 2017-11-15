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

#include "matrix-event-room-topic.h"
#include "matrix-types.h"
#include "config.h"

/**
 * SECTION:matrix-event-room-topic
 * @short_description: event to hold a room topic
 *
 * This is the default event handler for `m.room.topic` events.
 *
 * A topic is a short message detailing what is currently being discussed in the room.  It can
 * also be used as a way to display extra information about the room, which may not be
 * suitable for the room name.
 */
enum  {
    PROP_0,
    PROP_TOPIC,
    NUM_PROPS
};

static GParamSpec *matrix_event_room_topic_properties[NUM_PROPS];

typedef struct {
    gchar* _topic;
} MatrixEventRoomTopicPrivate;

/**
 * MatrixEventRoomTopic:
 */
G_DEFINE_TYPE_WITH_PRIVATE(MatrixEventRoomTopic, matrix_event_room_topic, MATRIX_EVENT_TYPE_STATE);

static void
matrix_event_room_topic_real_from_json(MatrixEventBase *matrix_event_base, JsonNode *json_data, GError **error)
{
    MatrixEventRoomTopicPrivate *priv;
    JsonObject *root;
    JsonObject *content_root;
    JsonNode *content_node;
    JsonNode *node;
    GError *inner_error = NULL;

    g_return_if_fail(json_data != NULL);

    priv = matrix_event_room_topic_get_instance_private(MATRIX_EVENT_ROOM_TOPIC(matrix_event_base));
    root = json_node_get_object(json_data);
    content_node = json_object_get_member(root, "content");
    content_root = json_node_get_object(content_node);

    if (DEBUG && ((node = json_object_get_member(root, "state_key")) != NULL)) {
        const gchar *state_key = json_node_get_string(node);

        if ((state_key == NULL) || (*state_key == 0)) {
            g_warning("state_key of a m.room.topic event is non-empty");
        }
    }

    if ((node = json_object_get_member(content_root, "topic")) != NULL) {
        g_free(priv->_topic);
        priv->_topic = g_strdup(json_node_get_string(node));
    } else if (DEBUG) {
        g_warning("content.topic is missing from an m.room.topic event");
    }

    MATRIX_EVENT_BASE_CLASS(matrix_event_room_topic_parent_class)->from_json(matrix_event_base, json_data, &inner_error);

    if (inner_error != NULL) {
        g_propagate_error(error, inner_error);
    }
}

static void
matrix_event_room_topic_real_to_json(MatrixEventBase *matrix_event_base, JsonNode *json_data, GError **error)
{
    MatrixEventRoomTopicPrivate *priv;
    JsonObject *root;
    JsonObject *content_root;
    JsonNode *content_node;
    GError *inner_error = NULL;
    const gchar *state_key;

    g_return_if_fail(json_data != NULL);

    priv = matrix_event_room_topic_get_instance_private(MATRIX_EVENT_ROOM_TOPIC(matrix_event_base));
    root = json_node_get_object(json_data);
    content_node = json_object_get_member(root, "content");
    content_root = json_node_get_object(content_node);
    state_key = matrix_event_state_get_state_key(MATRIX_EVENT_STATE(matrix_event_base));

    if ((state_key == NULL) || (*state_key == 0)) {
        g_set_error(error, MATRIX_ERROR, MATRIX_ERROR_INCOMPLETE,
                    "Won't generate a m.room.topic event with a non-empty state_key");

        return;
    }

    if (priv->_topic != NULL) {
        json_object_set_string_member(content_root, "topic", priv->_topic);
    }

    MATRIX_EVENT_BASE_CLASS(matrix_event_room_topic_parent_class)->to_json(matrix_event_base, json_data, &inner_error);

    if (inner_error != NULL) {
        g_propagate_error(error, inner_error);
    }
}

MatrixEventRoomTopic *
matrix_event_room_topic_new(void) {
    return (MatrixEventRoomTopic *)matrix_event_state_construct(MATRIX_EVENT_TYPE_ROOM_TOPIC);
}

const gchar *
matrix_event_room_topic_get_topic(MatrixEventRoomTopic *matrix_event_room_topic)
{
    MatrixEventRoomTopicPrivate *priv;

    g_return_val_if_fail(matrix_event_room_topic != NULL, NULL);

    priv = matrix_event_room_topic_get_instance_private(matrix_event_room_topic);

    return priv->_topic;
}

void
matrix_event_room_topic_set_topic(MatrixEventRoomTopic *matrix_event_room_topic, const gchar *topic)
{
    MatrixEventRoomTopicPrivate *priv;

    g_return_if_fail(matrix_event_room_topic != NULL);

    priv = matrix_event_room_topic_get_instance_private(matrix_event_room_topic);

    if (g_strcmp0(topic, priv->_topic) != 0) {
        g_free(priv->_topic);
        priv->_topic = g_strdup(topic);

        g_object_notify_by_pspec((GObject *)matrix_event_room_topic, matrix_event_room_topic_properties[PROP_TOPIC]);
    }
}

static void
matrix_event_room_topic_finalize(GObject *gobject)
{
    MatrixEventRoomTopicPrivate *priv;

    priv = matrix_event_room_topic_get_instance_private(MATRIX_EVENT_ROOM_TOPIC(gobject));

    g_free(priv->_topic);

    G_OBJECT_CLASS(matrix_event_room_topic_parent_class)->finalize(gobject);
}

static void
matrix_event_room_topic_get_property(GObject *gobject, guint property_id, GValue* value, GParamSpec* pspec)
{
    MatrixEventRoomTopicPrivate *priv = matrix_event_room_topic_get_instance_private(MATRIX_EVENT_ROOM_TOPIC(gobject));

    switch (property_id) {
        case PROP_TOPIC:
            g_value_set_string(value, priv->_topic);

            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, property_id, pspec);

            break;
    }
}

static void
matrix_event_room_topic_set_property(GObject *gobject, guint property_id, const GValue *value, GParamSpec *pspec)
{
    MatrixEventRoomTopic *matrix_event_room_topic = MATRIX_EVENT_ROOM_TOPIC(gobject);

    switch (property_id) {
        case PROP_TOPIC:
            matrix_event_room_topic_set_topic(matrix_event_room_topic, g_value_get_string(value));

            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, property_id, pspec);

            break;
    }
}

static void
matrix_event_room_topic_class_init(MatrixEventRoomTopicClass *klass)
{
    ((MatrixEventBaseClass *)klass)->from_json = matrix_event_room_topic_real_from_json;
    ((MatrixEventBaseClass *)klass)->to_json = matrix_event_room_topic_real_to_json;
    G_OBJECT_CLASS (klass)->get_property = matrix_event_room_topic_get_property;
    G_OBJECT_CLASS (klass)->set_property = matrix_event_room_topic_set_property;
    G_OBJECT_CLASS (klass)->finalize = matrix_event_room_topic_finalize;

    /**
     * MatrixEventRoomTopic:topic:
     *
     * The topic text.
     */
    matrix_event_room_topic_properties[PROP_TOPIC] = g_param_spec_string(
            "topic", "topic", "topic",
            NULL,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
        g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_TOPIC, matrix_event_room_topic_properties[PROP_TOPIC]);
}

static void
matrix_event_room_topic_init(MatrixEventRoomTopic *matrix_event_room_topic)
{
    MatrixEventRoomTopicPrivate *priv;

    priv = matrix_event_room_topic_get_instance_private(matrix_event_room_topic);

    priv->_topic = NULL;
}
