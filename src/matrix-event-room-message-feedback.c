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

#include "matrix-event-room-message-feedback.h"
#include "matrix-enumtypes.h"

/**
 * SECTION:matrix-event-room-message-feedback
 * @short_description: event to hold message feedbacks
 *
 * This is the default event handler for `m.room.message.feedback` events.
 *
 * Usage of this event is discouraged in favour of the receipts module.  Most clients will not
 * recognise this event.
 *
 * Feedback events are events sent to acknowledge a message in some way.  There are two
 * supported acknowledgements: `delivered` (sent when the event has been received) and `read`
 * (sent when the event has been observed by the end-user).  The `target_event_id` should
 * reference the `m.room.message` event being acknowledged.
 */
enum  {
    PROP_0,
    PROP_FEEDBACK_TYPE,
    PROP_TARGET_EVENT_ID,
    NUM_PROPERTIES
};

static GParamSpec* matrix_event_room_message_feedback_properties[NUM_PROPERTIES];

typedef struct {
    gchar* _feedback_type;
    gchar* _target_event_id;
} MatrixEventRoomMessageFeedbackPrivate;

/**
 * MatrixEventRoomMessageFeedback:
 */
G_DEFINE_TYPE_WITH_PRIVATE(MatrixEventRoomMessageFeedback, matrix_event_room_message_feedback, MATRIX_EVENT_TYPE_ROOM);

static void
matrix_event_room_message_feedback_real_from_json(MatrixEventBase *matrix_event_base, JsonNode *json_data, GError **error)
{
    MatrixEventRoomMessageFeedbackPrivate *priv;
    JsonObject *root;
    JsonObject *content_root;
    JsonNode *content_node;
    JsonNode *node;

    g_return_if_fail(json_data != NULL);

    priv = matrix_event_room_message_feedback_get_instance_private(MATRIX_EVENT_ROOM_MESSAGE_FEEDBACK(matrix_event_base));
    root = json_node_get_object(json_data);
    content_node = json_object_get_member(root, "content");
    content_root = json_node_get_object(content_node);

    if ((node = json_object_get_member(content_root, "type")) != NULL) {
        g_free(priv->_feedback_type);
        priv->_feedback_type = g_strdup(json_node_get_string(node));
    } else {
        g_warning("content.type is missing from a m.room.message.feedback event");
    }

    if ((node = json_object_get_member(content_root, "target_event_id")) != NULL) {
        g_free(priv->_target_event_id);
        priv->_target_event_id = g_strdup(json_node_get_string(node));
    } else {
        g_warning("content.target_event_id is missing from a m.room.message.feedback event");
    }

    MATRIX_EVENT_BASE_CLASS(matrix_event_room_message_feedback_parent_class)->from_json(matrix_event_base, json_data, error);
}

static void
matrix_event_room_message_feedback_real_to_json(MatrixEventBase *matrix_event_base, JsonNode *json_data, GError **error)
{
    MatrixEventRoomMessageFeedbackPrivate *priv;
    JsonObject *root;
    JsonObject *content_root;
    JsonNode *content_node;

    g_return_if_fail(json_data != NULL);

    priv = matrix_event_room_message_feedback_get_instance_private(MATRIX_EVENT_ROOM_MESSAGE_FEEDBACK(matrix_event_base));
    root = json_node_get_object(json_data);
    content_node = json_object_get_member(root, "content");
    content_root = json_node_get_object(content_node);

    if ((priv->_feedback_type == NULL) || (priv->_target_event_id == NULL)) {
        g_set_error(error, MATRIX_ERROR, MATRIX_ERROR_INCOMPLETE,
                    "Won't generate a m.room.message.feedback without all fields set");

        return;
    }

    json_object_set_string_member(content_root, "type", priv->_feedback_type);
    json_object_set_string_member(content_root, "target_event_id", priv->_target_event_id);

    MATRIX_EVENT_BASE_CLASS(matrix_event_room_message_feedback_parent_class)->to_json(matrix_event_base, json_data, error);
}

MatrixEventRoomMessageFeedback *
matrix_event_room_message_feedback_construct(GType object_type)
{
    return (MatrixEventRoomMessageFeedback *)matrix_event_room_construct(object_type);
}

/**
 * matrix_event_room_message_feedback_new:
 *
 * Create a new #MatrixEventRoomMessageFeedback object.
 *
 * Returns: (transfer full): a new #MatrixEventRoomMessageFeedback object
 */
MatrixEventRoomMessageFeedback *
matrix_event_room_message_feedback_new(void)
{
    return matrix_event_room_message_feedback_construct(MATRIX_EVENT_TYPE_ROOM_MESSAGE_FEEDBACK);
}

/**
 * matrix_event_room_message_feedback_get_feedback_type:
 * @event: a #MatrixEventRoomMessageFeedback
 *
 * Get the feedback type from @event.
 *
 * The returned value is owned by @event and should not be freed.
 *
 * Returns: (transfer none) (nullable): the feedback type
 */
const gchar *
matrix_event_room_message_feedback_get_feedback_type(MatrixEventRoomMessageFeedback *matrix_event_room_message_feedback)
{
    MatrixEventRoomMessageFeedbackPrivate *priv;

    g_return_val_if_fail(matrix_event_room_message_feedback != NULL, NULL);

    priv = matrix_event_room_message_feedback_get_instance_private(matrix_event_room_message_feedback);

    return priv->_feedback_type;
}

/**
 * matrix_event_room_message_feedback_set_feedback_type:
 * @event: a #MatrixEventRoomMessageFeedback
 * @feedback_type: (transfer none) (nullable): a feedback type
 *
 * Set the feedback type in @event.
 */
void
matrix_event_room_message_feedback_set_feedback_type(MatrixEventRoomMessageFeedback *matrix_event_room_message_feedback, const gchar *feedback_type)
{
    MatrixEventRoomMessageFeedbackPrivate *priv;

    g_return_if_fail(matrix_event_room_message_feedback != NULL);

    priv = matrix_event_room_message_feedback_get_instance_private(matrix_event_room_message_feedback);

    if (g_strcmp0(feedback_type, priv->_feedback_type) != 0) {
        g_free(priv->_feedback_type);
        priv->_feedback_type = g_strdup(feedback_type);

        g_object_notify_by_pspec((GObject *)matrix_event_room_message_feedback, matrix_event_room_message_feedback_properties[PROP_FEEDBACK_TYPE]);
    }
}

/**
 * matrix_event_room_message_feedback_get_target_event_id:
 * @event: a #MatrixEventRoomMessageFeedback
 *
 * Get the target event’s ID from @event.
 *
 * The returned value is owned by @event and should not be freed.
 *
 * Returns: (transfer none) (nullable): the ID of the target event
 */
const gchar *
matrix_event_room_message_feedback_get_target_event_id(MatrixEventRoomMessageFeedback *matrix_event_room_message_feedback)
{
    MatrixEventRoomMessageFeedbackPrivate *priv;

    g_return_val_if_fail(matrix_event_room_message_feedback != NULL, NULL);

    priv = matrix_event_room_message_feedback_get_instance_private(matrix_event_room_message_feedback);
    return priv->_target_event_id;
}

/**
 * matrix_event_room_message_feedback_set_target_event_id:
 * @event: a #MatrixEventRoomMessageFeedback
 * @target_event_id: the ID of the target event
 *
 * Set the ID of the target event in @event.
 */
void
matrix_event_room_message_feedback_set_target_event_id(MatrixEventRoomMessageFeedback *matrix_event_room_message_feedback, const gchar *target_event_id)
{
    MatrixEventRoomMessageFeedbackPrivate *priv;

    g_return_if_fail(matrix_event_room_message_feedback != NULL);

    priv = matrix_event_room_message_feedback_get_instance_private(matrix_event_room_message_feedback);

    if (g_strcmp0(target_event_id, priv->_target_event_id) != 0) {
        g_free(priv->_target_event_id);
        priv->_target_event_id = g_strdup(target_event_id);

        g_object_notify_by_pspec((GObject *)matrix_event_room_message_feedback, matrix_event_room_message_feedback_properties[PROP_TARGET_EVENT_ID]);
    }
}

static void
matrix_event_room_message_feedback_finalize(GObject *gobject)
{
    MatrixEventRoomMessageFeedbackPrivate *priv = matrix_event_room_message_feedback_get_instance_private(MATRIX_EVENT_ROOM_MESSAGE_FEEDBACK(gobject));

    g_free(priv->_feedback_type);
    g_free(priv->_target_event_id);

    G_OBJECT_CLASS(matrix_event_room_message_feedback_parent_class)->finalize(gobject);
}

static void
matrix_event_room_message_feedback_get_property(GObject *gobject, guint property_id, GValue *value, GParamSpec *pspec)
{
    MatrixEventRoomMessageFeedback *matrix_event_room_message_feedback = MATRIX_EVENT_ROOM_MESSAGE_FEEDBACK(gobject);

    switch (property_id) {
        case PROP_FEEDBACK_TYPE:
            g_value_set_string(value, matrix_event_room_message_feedback_get_feedback_type(matrix_event_room_message_feedback));

            break;
        case PROP_TARGET_EVENT_ID:
            g_value_set_string(value, matrix_event_room_message_feedback_get_target_event_id(matrix_event_room_message_feedback));

            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, property_id, pspec);

            break;
    }
}

static void
matrix_event_room_message_feedback_set_property(GObject *gobject, guint property_id, const GValue *value, GParamSpec *pspec)
{
    MatrixEventRoomMessageFeedback *matrix_event_room_message_feedback = MATRIX_EVENT_ROOM_MESSAGE_FEEDBACK(gobject);

    switch (property_id) {
        case PROP_FEEDBACK_TYPE:
            matrix_event_room_message_feedback_set_feedback_type(matrix_event_room_message_feedback, g_value_get_string(value));

            break;
        case PROP_TARGET_EVENT_ID:
            matrix_event_room_message_feedback_set_target_event_id(matrix_event_room_message_feedback, g_value_get_string(value));

            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, property_id, pspec);
        break;
    }
}

static void
matrix_event_room_message_feedback_class_init(MatrixEventRoomMessageFeedbackClass *klass)
{
    ((MatrixEventBaseClass *)klass)->from_json = matrix_event_room_message_feedback_real_from_json;
    ((MatrixEventBaseClass *)klass)->to_json = matrix_event_room_message_feedback_real_to_json;
    G_OBJECT_CLASS(klass)->get_property = matrix_event_room_message_feedback_get_property;
    G_OBJECT_CLASS(klass)->set_property = matrix_event_room_message_feedback_set_property;
    G_OBJECT_CLASS(klass)->finalize = matrix_event_room_message_feedback_finalize;

    /**
     * MatrixEventRoomMessageFeedback:feedback-type:
     *
     * The type of the feedback.  As the use of this event type is discouraged, this SDK doesn’t
     * implement this as an actual enum, but the only recognised values are `received` and `read`.
     */
    matrix_event_room_message_feedback_properties[PROP_FEEDBACK_TYPE] = g_param_spec_string(
            "feedback-type", "feedback-type", "feedback-type",
            NULL,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_FEEDBACK_TYPE, matrix_event_room_message_feedback_properties[PROP_FEEDBACK_TYPE]);

    /**
     * MatrixEventRoomMessageFeedback:target-event-id:
     *
     * The event that this feedback is related to.
     */
    matrix_event_room_message_feedback_properties[PROP_TARGET_EVENT_ID] = g_param_spec_string(
            "target-event-id", "target-event-id", "target-event-id",
            NULL,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_TARGET_EVENT_ID, matrix_event_room_message_feedback_properties[PROP_TARGET_EVENT_ID]);
}

static void
matrix_event_room_message_feedback_init(MatrixEventRoomMessageFeedback *matrix_event_room_message_feedback)
{
    MatrixEventRoomMessageFeedbackPrivate *priv = matrix_event_room_message_feedback_get_instance_private(matrix_event_room_message_feedback);

    priv->_feedback_type = NULL;
    priv->_target_event_id = NULL;
}
