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

#include "matrix-event-room-message.h"
#include "matrix-enumtypes.h"

/**
 * SECTION:matrix-event-room-message
 * @short_description: event to hold messages in a room
 *
 * This is the default event handler for `m.room.message` events.
 *
 * This event is used when sending messages in a room.  Messages are not limited to be text.
 * The `msgtype` key outlines the type of message, e.g. text, audio, image, video, etc.  The
 * `body` key is text and MUST be used with every kind of `msgtype` as a fallback mechanism for
 * when a client cannot render a message.  This allows clients to display *something* even if
 * it is just plain text.
 */
enum  {
    PROP_0,
    PROP_BODY,
    PROP_MESSAGE,
    PROP_FALLBACK_CONTENT,
    NUM_PROPERTIES
};

static GParamSpec* matrix_event_room_message_properties[NUM_PROPERTIES];

typedef struct {
    MatrixMessageBase* _message;
    gchar *body;
    JsonNode* _fallback_content;
} MatrixEventRoomMessagePrivate;

/**
 * MatrixEventRoomMessage:
 */
G_DEFINE_TYPE_WITH_PRIVATE(MatrixEventRoomMessage, matrix_event_room_message, MATRIX_EVENT_TYPE_ROOM);

static void
matrix_event_room_message_real_from_json(MatrixEventBase *matrix_event_base, JsonNode *json_data, GError **error)
{
    MatrixEventRoomMessagePrivate *priv;
    JsonObject *root;
    JsonObject *content_root;
    JsonNode *content_node;
    JsonNode *node;
    MatrixMessageBase *message;
    GError *inner_error = NULL;

    g_return_if_fail(json_data != NULL);

    priv = matrix_event_room_message_get_instance_private(MATRIX_EVENT_ROOM_MESSAGE(matrix_event_base));
    root = json_node_get_object(json_data);
    content_node = json_object_get_member(root, "content");

    json_node_unref(priv->_fallback_content);
    priv->_fallback_content = NULL;

    content_root = json_node_get_object(content_node);
    node = json_object_get_member(content_root, "body");

    g_free(priv->body);
    priv->body = g_strdup(json_node_get_string(node));

    /*
     * We don't want to fail on unknown message types, so let's save the JSON content
     * instead. Silent (ie. without exception) null is only returned if there is no handler class
     * installed
     */
    message = matrix_message_base_new_from_json(content_node, &inner_error);

    if (inner_error != NULL) {
        g_propagate_error(error, inner_error);
    }

    priv->_message = message;

    if (message == NULL) {
        priv->_fallback_content = json_node_ref(content_node);
    }

    MATRIX_EVENT_BASE_CLASS(matrix_event_room_message_parent_class)->from_json(matrix_event_base, json_data, error);
}

static void
matrix_event_room_message_real_to_json(MatrixEventBase *matrix_event_base, JsonNode *json_data, GError **error)
{
    MatrixEventRoomMessagePrivate *priv;
    JsonObject *root;
    JsonNode *node;
    GError *inner_error = NULL;

    g_return_if_fail(json_data != NULL);

    priv = matrix_event_room_message_get_instance_private(MATRIX_EVENT_ROOM_MESSAGE(matrix_event_base));

    if (priv->_message == NULL) {
        g_set_error(error, MATRIX_ERROR, MATRIX_ERROR_INCOMPLETE,
                    "Won't generate a m.room.message event without content");

        return;
    }

    root = json_node_get_object(json_data);

    node = matrix_message_base_get_json(priv->_message, &inner_error);

    if (inner_error != NULL) {
        g_propagate_error(error, inner_error);

        return;
    }

    json_object_set_member(root, "content", node);

    MATRIX_EVENT_BASE_CLASS(matrix_event_room_message_parent_class)->to_json(matrix_event_base, json_data, error);
}

MatrixEventRoomMessage *
matrix_event_room_message_construct(GType object_type)
{
    return (MatrixEventRoomMessage *)matrix_event_room_construct(object_type);
}

/**
 * matrix_event_room_message_new:
 *
 * Create a new #MatrixEventRoomMessage object.
 *
 * Returns: (transfer full): a new #MatrixEventRoomMessage object
 */
MatrixEventRoomMessage *
matrix_event_room_message_new(void) {
    return matrix_event_room_message_construct(MATRIX_EVENT_TYPE_ROOM_MESSAGE);
}

/**
 * matrix_event_room_message_get_message:
 * @event: a #MatrixEventRoomMessage
 *
 * Get the message object from @event, if it could be resoled to a known message type.
 * Resolution is done by matrix_message_get_handler().
 *
 * The returned value is owned by @event and should not be freed.
 *
 * Returns: (transfer none) (nullable): a #MatrixMessageBase derived object
 */
MatrixMessageBase *
matrix_event_room_message_get_message(MatrixEventRoomMessage *matrix_event_room_message)
{
    MatrixEventRoomMessagePrivate *priv;

    g_return_val_if_fail(matrix_event_room_message != NULL, NULL);

    priv = matrix_event_room_message_get_instance_private(matrix_event_room_message);

    return priv->_message;
}

/**
 * matrix_event_room_message_set_message:
 * @event: a #MatrixEventRoomMessage
 * @message: (transfer none): a #MatrixMessageBase derived object
 *
 * Set the message of @event.
 *
 * @event gets a reference on @message, so it is safe to unref it after calling this function.
 */
void
matrix_event_room_message_set_message(MatrixEventRoomMessage *matrix_event_room_message, MatrixMessageBase *message)
{
    MatrixEventRoomMessagePrivate *priv;

    g_return_if_fail(matrix_event_room_message != NULL);

    priv = matrix_event_room_message_get_instance_private(matrix_event_room_message);

    if (message != priv->_message) {
        g_object_unref(priv->_message);
        priv->_message = g_object_ref(message);

        g_object_notify_by_pspec((GObject *)matrix_event_room_message, matrix_event_room_message_properties[PROP_MESSAGE]);
    }
}

/**
 * matrix_event_room_message_get_fallback_content:
 * @event: a #MatrixEventRoomMessage
 *
 * Get the fallback content from @event.
 */
JsonNode *
matrix_event_room_message_get_fallback_content(MatrixEventRoomMessage *matrix_event_room_message)
{
    MatrixEventRoomMessagePrivate *priv;

    g_return_val_if_fail(matrix_event_room_message != NULL, NULL);

    priv = matrix_event_room_message_get_instance_private(matrix_event_room_message);

    return priv->_fallback_content;
}

/**
 * matrix_event_room_message_get_body:
 * @event: a #MatrixEventRoomMessage
 *
 * Get the body (ie. fallback textual content) of @event.
 *
 * The returned value is owned by @event and should not be freed.
 *
 * Returns: (transfer none) (nullable): the message body
 */
const gchar *
matrix_event_room_message_get_body(MatrixEventRoomMessage *event)
{
    MatrixEventRoomMessagePrivate *priv = matrix_event_room_message_get_instance_private(event);

    g_return_val_if_fail(event != NULL, NULL);

    return priv->body;
}

/**
 * matrix_event_room_message_set_body:
 * @event: a #MatrixEventRoomMessage
 * @body: (transfer none) (nullable): the textual body of the message
 *
 * Set the textual content of @event.
 */
void
matrix_event_room_message_set_body(MatrixEventRoomMessage *event, const gchar *body)
{
    MatrixEventRoomMessagePrivate *priv = matrix_event_room_message_get_instance_private(event);

    g_return_if_fail(event != NULL);

    if (g_strcmp0(body, priv->body) != 0) {
        g_free(priv->body);
        priv->body = g_strdup(body);

        g_object_notify_by_pspec(G_OBJECT(event), matrix_event_room_message_properties[PROP_BODY]);
    }
}

static void
matrix_event_room_message_finalize(GObject *gobject)
{
    MatrixEventRoomMessagePrivate *priv = matrix_event_room_message_get_instance_private(MATRIX_EVENT_ROOM_MESSAGE(gobject));

    g_object_unref(priv->_message);
    json_node_unref(priv->_fallback_content);

    G_OBJECT_CLASS(matrix_event_room_message_parent_class)->finalize(gobject);
}

static void
matrix_event_room_message_get_property(GObject *gobject, guint property_id, GValue *value, GParamSpec *pspec)
{
    MatrixEventRoomMessage *matrix_event_room_message = MATRIX_EVENT_ROOM_MESSAGE(gobject);

    switch (property_id) {
        case PROP_MESSAGE:
            g_value_set_object(value, matrix_event_room_message_get_message(matrix_event_room_message));

            break;
        case PROP_FALLBACK_CONTENT:
            g_value_set_boxed(value, matrix_event_room_message_get_fallback_content(matrix_event_room_message));

            break;
        case PROP_BODY:
            g_value_set_string(value, matrix_event_room_message_get_body(matrix_event_room_message));

            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, property_id, pspec);

            break;
    }
}

static void
matrix_event_room_message_set_property(GObject *gobject, guint property_id, const GValue *value, GParamSpec *pspec)
{
    MatrixEventRoomMessage *matrix_event_room_message = MATRIX_EVENT_ROOM_MESSAGE(gobject);

    switch (property_id) {
        case PROP_MESSAGE:
            matrix_event_room_message_set_message(matrix_event_room_message, g_value_get_object (value));

            break;
        case PROP_BODY:
            matrix_event_room_message_set_body(matrix_event_room_message, g_value_get_string(value));

            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, property_id, pspec);

            break;
    }
}

static void
matrix_event_room_message_class_init(MatrixEventRoomMessageClass *klass)
{
    ((MatrixEventBaseClass *)klass)->from_json = matrix_event_room_message_real_from_json;
    ((MatrixEventBaseClass *)klass)->to_json = matrix_event_room_message_real_to_json;
    G_OBJECT_CLASS(klass)->get_property = matrix_event_room_message_get_property;
    G_OBJECT_CLASS(klass)->set_property = matrix_event_room_message_set_property;
    G_OBJECT_CLASS(klass)->finalize = matrix_event_room_message_finalize;

    /**
     * MatrixEventRoomMessage:message:
     *
     * The message as a #MatrixMessageBase derived object.
     */
    matrix_event_room_message_properties[PROP_MESSAGE] = g_param_spec_object(
            "message", "message", "message",
            MATRIX_MESSAGE_TYPE_BASE,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_MESSAGE, matrix_event_room_message_properties[PROP_MESSAGE]);

    /**
     * MatrixEventRoomMessage:fallback-content:
     *
     * The message as a JSON object. This gets set by matrix_message_base_new_from_json() if
     * no handler is installed for the given message type.
     */
    matrix_event_room_message_properties[PROP_FALLBACK_CONTENT] = g_param_spec_boxed(
            "fallback-content", "fallback-content", "fallback-content",
            JSON_TYPE_NODE,
            G_PARAM_STATIC_STRINGS | G_PARAM_READABLE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_FALLBACK_CONTENT, matrix_event_room_message_properties[PROP_FALLBACK_CONTENT]);

    /**
     * MatrixEventRoomMessage:body:
     *
     * The fallback textual body of the message.
     */
    matrix_event_room_message_properties[PROP_BODY] = g_param_spec_string(
            "body", "body", "body",
            NULL,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_BODY, matrix_event_room_message_properties[PROP_BODY]);
}

static void
matrix_event_room_message_init(MatrixEventRoomMessage *matrix_event_room_message)
{
    MatrixEventRoomMessagePrivate *priv = matrix_event_room_message_get_instance_private(matrix_event_room_message);

    priv->_message = NULL;
    priv->_fallback_content = NULL;
}
