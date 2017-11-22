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

#include "matrix-event-room-redaction.h"
#include "matrix-types.h"

/**
 * SECTION:matrix-event-room-redaction
 * @short_description: event to designate event redactions
 *
 * This is the default event handler for `m.room.redaction` events.
 *
 * Events can be redacted by either room or server admins.  Redacting an event means that all
 * keys not required by the protocol are stripped off, allowing admins to remove offensive or
 * illegal content that may have been attached to any event.  This cannot be undone, allowing
 * server owners to physically delete the offending data.  There is also a concept of a
 * moderator hiding a message event, which can be undone, but cannot be applied to state events.
 * The event that has been redacted is specified in the `redacts` event level key.
 */
enum  {
    PROP_0,
    PROP_REASON,
    PROP_REDACTED_EVENT_ID,
    NUM_PROPERTIES
};

static GParamSpec *matrix_event_room_redaction_properties[NUM_PROPERTIES];

typedef struct {
    gchar* _reason;
    gchar* _redacted_event_id;
} MatrixEventRoomRedactionPrivate;

/**
 * MatrixEventRoomRedaction:
 */
G_DEFINE_TYPE_WITH_PRIVATE(MatrixEventRoomRedaction, matrix_event_room_redaction, MATRIX_EVENT_TYPE_ROOM);

static void
matrix_event_room_redaction_real_from_json(MatrixEventBase *matrix_event_base, JsonNode *json_data, GError **error)
{
    MatrixEventRoomRedactionPrivate *priv;
    JsonObject *root;
    JsonObject *content_root;
    JsonNode *content_node;
    JsonNode *node;

    g_return_if_fail (json_data != NULL);

    priv = matrix_event_room_redaction_get_instance_private(MATRIX_EVENT_ROOM_REDACTION(matrix_event_base));
    root = json_node_get_object(json_data);
    content_node = json_object_get_member(root, "content");
    content_root = json_node_get_object(content_node);

    if ((node = json_object_get_member(content_root, "reason")) != NULL) {
        g_free(priv->_reason);
        priv->_reason = g_strdup(json_node_get_string(node));
    }

    if ((node = json_object_get_member(root, "redacts")) != NULL) {
        g_free(priv->_redacted_event_id);
        priv->_redacted_event_id = g_strdup(json_node_get_string(node));
    } else {
        g_warning("redacts is missing from a m.room.redaction event");
    }

    MATRIX_EVENT_BASE_CLASS(matrix_event_room_redaction_parent_class)->from_json(matrix_event_base, json_data, error);
}

static void
matrix_event_room_redaction_real_to_json(MatrixEventBase *matrix_event_base, JsonNode *json_data, GError **error)
{
    MatrixEventRoomRedactionPrivate *priv;
    JsonObject *root;
    JsonObject *content_root;
    JsonNode *content_node;

    g_return_if_fail (json_data != NULL);

    priv = matrix_event_room_redaction_get_instance_private(MATRIX_EVENT_ROOM_REDACTION(matrix_event_base));
    root = json_node_get_object(json_data);

    if (priv->_redacted_event_id == NULL) {
        g_set_error(error, MATRIX_ERROR, MATRIX_ERROR_INCOMPLETE,
                    "Won't generate a m.room.redaction event without /* TODO:  */he redacts field");

        return;
    }

    json_object_set_string_member(root, "redacts", priv->_redacted_event_id);

    if (priv->_reason != NULL) {
        content_node = json_object_get_member(root, "content");
        content_root = json_node_get_object(content_node);

        json_object_set_string_member(content_root, "reason", priv->_reason);
    }

    MATRIX_EVENT_BASE_CLASS(matrix_event_room_redaction_parent_class)->to_json(matrix_event_base, json_data, error);
}

/**
 * matrix_event_room_redaction_new:
 *
 * Create a new #MatrixEventRoomRedaction object.
 *
 * Returns: (transfer full): a new #MatrixEventRoomRedaction object
 */
MatrixEventRoomRedaction *
matrix_event_room_redaction_new(void)
{
    return (MatrixEventRoomRedaction *)matrix_event_room_construct(MATRIX_EVENT_TYPE_ROOM_REDACTION);
}

const gchar *
matrix_event_room_redaction_get_reason(MatrixEventRoomRedaction *matrix_event_room_redaction)
{
    MatrixEventRoomRedactionPrivate *priv;

    g_return_val_if_fail(matrix_event_room_redaction != NULL, NULL);

    priv = matrix_event_room_redaction_get_instance_private(matrix_event_room_redaction);

    return priv->_reason;
}

void
matrix_event_room_redaction_set_reason(MatrixEventRoomRedaction *matrix_event_room_redaction, const gchar *reason)
{
    MatrixEventRoomRedactionPrivate *priv;

    g_return_if_fail(matrix_event_room_redaction != NULL);

    priv = matrix_event_room_redaction_get_instance_private(matrix_event_room_redaction);

    if (g_strcmp0(reason, priv->_reason) != 0) {
        g_free(priv->_reason);
        priv->_reason = g_strdup(reason);

        g_object_notify_by_pspec((GObject *)matrix_event_room_redaction, matrix_event_room_redaction_properties[PROP_REASON]);
    }
}

const gchar *
matrix_event_room_redaction_get_redacted_event_id(MatrixEventRoomRedaction *matrix_event_room_redaction)
{
    MatrixEventRoomRedactionPrivate *priv;

    g_return_val_if_fail(matrix_event_room_redaction != NULL, NULL);

    priv = matrix_event_room_redaction_get_instance_private(matrix_event_room_redaction);

    return priv->_redacted_event_id;
}

void
matrix_event_room_redaction_set_redacted_event_id(MatrixEventRoomRedaction *matrix_event_room_redaction, const gchar *redacted_event_id)
{
    MatrixEventRoomRedactionPrivate *priv;

    g_return_if_fail(matrix_event_room_redaction != NULL);

    priv = matrix_event_room_redaction_get_instance_private(matrix_event_room_redaction);

    if (g_strcmp0(redacted_event_id, priv->_redacted_event_id) != 0) {
        g_free(priv->_redacted_event_id);
        priv->_redacted_event_id = g_strdup(redacted_event_id);

        g_object_notify_by_pspec((GObject *)matrix_event_room_redaction, matrix_event_room_redaction_properties[PROP_REDACTED_EVENT_ID]);
    }
}

static void
matrix_event_room_redaction_finalize(GObject *gobject)
{
    MatrixEventRoomRedactionPrivate *priv = matrix_event_room_redaction_get_instance_private(MATRIX_EVENT_ROOM_REDACTION(gobject));

    g_free(priv->_reason);
    g_free(priv->_redacted_event_id);

    G_OBJECT_CLASS(matrix_event_room_redaction_parent_class)->finalize(gobject);
}

static void
matrix_event_room_redaction_get_property(GObject *gobject, guint property_id, GValue *value, GParamSpec *pspec)
{
    MatrixEventRoomRedaction *matrix_event_room_redaction = MATRIX_EVENT_ROOM_REDACTION(gobject);

    switch (property_id) {
        case PROP_REASON:
            g_value_set_string(value, matrix_event_room_redaction_get_reason(matrix_event_room_redaction));

            break;
        case PROP_REDACTED_EVENT_ID:
            g_value_set_string(value, matrix_event_room_redaction_get_redacted_event_id(matrix_event_room_redaction));

            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, property_id, pspec);
        break;
    }
}

static void
matrix_event_room_redaction_set_property(GObject *gobject, guint property_id, const GValue *value, GParamSpec *pspec)
{
    MatrixEventRoomRedaction *matrix_event_room_redaction = MATRIX_EVENT_ROOM_REDACTION(gobject);

    switch (property_id) {
        case PROP_REASON:
            matrix_event_room_redaction_set_reason(matrix_event_room_redaction, g_value_get_string(value));

            break;
        case PROP_REDACTED_EVENT_ID:
            matrix_event_room_redaction_set_redacted_event_id(matrix_event_room_redaction, g_value_get_string(value));

            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, property_id, pspec);

            break;
    }
}

static void
matrix_event_room_redaction_class_init(MatrixEventRoomRedactionClass *klass)
{
    ((MatrixEventBaseClass *)klass)->from_json = matrix_event_room_redaction_real_from_json;
    ((MatrixEventBaseClass *)klass)->to_json = matrix_event_room_redaction_real_to_json;
    G_OBJECT_CLASS(klass)->get_property = matrix_event_room_redaction_get_property;
    G_OBJECT_CLASS(klass)->set_property = matrix_event_room_redaction_set_property;
    G_OBJECT_CLASS(klass)->finalize = matrix_event_room_redaction_finalize;

    /**
     * MatrixEventRoomRedaction:reason:
     *
     * The reason for the redaction, if any.
     */
    matrix_event_room_redaction_properties[PROP_REASON] = g_param_spec_string(
            "reason", "reason", "reason",
            NULL,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_REASON, matrix_event_room_redaction_properties[PROP_REASON]);

    /**
     * MatrixEventRoomRedaction:redacted-event-id:
     *
     * The event ID that was redacted.
     */
    matrix_event_room_redaction_properties[PROP_REDACTED_EVENT_ID] = g_param_spec_string(
            "redacted-event-id", "redacted-event-id", "redacted-event-id",
            NULL,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_REDACTED_EVENT_ID, matrix_event_room_redaction_properties[PROP_REDACTED_EVENT_ID]);
}

static void
matrix_event_room_redaction_init(MatrixEventRoomRedaction *matrix_event_room_redaction) {
    MatrixEventRoomRedactionPrivate *priv = matrix_event_room_redaction_get_instance_private(matrix_event_room_redaction);

    priv->_reason = NULL;
    priv->_redacted_event_id = NULL;
}
