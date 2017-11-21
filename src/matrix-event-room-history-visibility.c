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

#include "matrix-event-room-history-visibility.h"
#include "matrix-enumtypes.h"
#include "utils.h"

/**
 * SECTION:matrix-event-room-history-visibility
 * @short_description: event to control history visibility to room members
 *
 * This is the default event handler for `m.room.history_visibility` events.
 *
 * This event controls whether a user can see the events that happened in a room from before
 * they joined.
 */
enum  {
    PROP_0,
    PROP_VISIBILITY,
    NUM_PROPERTIES
};

static GParamSpec *matrix_event_room_history_visibility_properties[NUM_PROPERTIES];

typedef struct {
    MatrixHistoryVisibility _visibility;
} MatrixEventRoomHistoryVisibilityPrivate;

/**
 * MatrixEventRoomHistoryVisibility:
 */
G_DEFINE_TYPE_WITH_PRIVATE(MatrixEventRoomHistoryVisibility, matrix_event_room_history_visibility, MATRIX_EVENT_TYPE_STATE);

static void
matrix_event_room_history_visibility_real_from_json(MatrixEventBase *matrix_event_base, JsonNode *json_data, GError **error)
{
    MatrixEventRoomHistoryVisibilityPrivate *priv;
    JsonObject *root;
    JsonObject *content_root;
    JsonNode *content_node;
    JsonNode *node;

    g_return_if_fail (json_data != NULL);

    priv = matrix_event_room_history_visibility_get_instance_private(MATRIX_EVENT_ROOM_HISTORY_VISIBILITY(matrix_event_base));
    root = json_node_get_object(json_data);
    content_node = json_object_get_member(root, "content");
    content_root = json_node_get_object(content_node);

#ifdef DEBUG
    if ((node = json_object_get_member(root, "state_key")) != NULL) {
        const gchar *state_key = json_node_get_string(node);

        if ((state_key == NULL) || (*state_key == 0)) {
            g_warning("state_key of a m.room.history_visibility event is non-empty");
        }
    }
#endif

    if ((node = json_object_get_member(content_root, "history_visibility")) != NULL) {
        GError *inner_error = NULL;
        MatrixHistoryVisibility visibility = _matrix_g_enum_nick_to_value(MATRIX_TYPE_HISTORY_VISIBILITY, json_node_get_string(node), &inner_error);

        if (inner_error != NULL) {
            priv->_visibility = MATRIX_HISTORY_VISIBILITY_UNKNOWN;
#ifdef DEBUG
            g_warning("Unknown history_visibility value %s", json_node_get_string(node));
#endif
        } else {
            priv->_visibility = visibility;
        }
    }

    MATRIX_EVENT_BASE_CLASS(matrix_event_room_history_visibility_parent_class)->from_json(matrix_event_base, json_data, error);
}

static void
matrix_event_room_history_visibility_real_to_json(MatrixEventBase *matrix_event_base, JsonNode *json_data, GError **error)
{
    MatrixEventRoomHistoryVisibilityPrivate *priv;
    JsonObject *root;
    JsonObject *content_root;
    JsonNode *content_node;
    const gchar *state_key;
    gchar *visibility_str;

    g_return_if_fail (json_data != NULL);

    priv = matrix_event_room_history_visibility_get_instance_private(MATRIX_EVENT_ROOM_HISTORY_VISIBILITY(matrix_event_base));

    root = json_node_get_object(json_data);
    content_node = json_object_get_member(root, "content");
    content_root = json_node_get_object(content_node);

    state_key = matrix_event_state_get_state_key(MATRIX_EVENT_STATE(matrix_event_base));

    if ((state_key == NULL) || (*state_key == 0)) {
        g_set_error(error, MATRIX_ERROR, MATRIX_ERROR_INCOMPLETE,
                    "Won't generate a m.room.history_visibility event with a non-empty state key");

        return;
    }

    if (priv->_visibility == MATRIX_HISTORY_VISIBILITY_UNKNOWN) {
        g_set_error(error, MATRIX_ERROR, MATRIX_ERROR_UNKNOWN_VALUE,
                    "Won't send m.room.history_visibility event with unknown visibility value");

        return;
    }

    visibility_str = _matrix_g_enum_to_string(MATRIX_TYPE_HISTORY_VISIBILITY, priv->_visibility, '_');

    json_object_set_string_member(content_root, "history_visibility", visibility_str);

    g_free(visibility_str);

    MATRIX_EVENT_BASE_CLASS(matrix_event_room_history_visibility_parent_class)->to_json(matrix_event_base, json_data, error);
}

/**
 * matrix_event_room_history_visibility_new:
 *
 * Create a new #MatrixEventRoomHistoryVisibility object.
 *
 * Returns: (transfer full): a new #MatrixEventRoomHistoryVisibility object
 */
MatrixEventRoomHistoryVisibility *
matrix_event_room_history_visibility_new(void)
{
    return (MatrixEventRoomHistoryVisibility *)matrix_event_state_construct(MATRIX_EVENT_TYPE_ROOM_HISTORY_VISIBILITY);
}

/**
 * matrix_event_room_history_visibility_get_visibility:
 * @event: a #MatrixEventRoomHistoryVisibility
 *
 * Get the history visibility status of the room.
 *
 * Returns: a #MatrixHistoryVisibility value
 */
MatrixHistoryVisibility
matrix_event_room_history_visibility_get_visibility(MatrixEventRoomHistoryVisibility *matrix_event_room_history_visibility)
{
    MatrixEventRoomHistoryVisibilityPrivate *priv;

    g_return_val_if_fail(matrix_event_room_history_visibility != NULL, 0);

    priv = matrix_event_room_history_visibility_get_instance_private(matrix_event_room_history_visibility);

    return priv->_visibility;
}

/**
 * matrix_event_room_history_visibility_set_visibility:
 * @event: a #MatrixEventRoomHistoryVisibility
 * @history_visibility: a #MatrixHistoryVisibility value
 *
 * Set the history visibility in @event.
 */
void
matrix_event_room_history_visibility_set_visibility(MatrixEventRoomHistoryVisibility *matrix_event_room_history_visibility, MatrixHistoryVisibility visibility)
{
    MatrixEventRoomHistoryVisibilityPrivate *priv;

    g_return_if_fail(matrix_event_room_history_visibility != NULL);

    priv = matrix_event_room_history_visibility_get_instance_private(matrix_event_room_history_visibility);

    if (priv->_visibility != visibility) {
        priv->_visibility = visibility;

        g_object_notify_by_pspec((GObject *)matrix_event_room_history_visibility, matrix_event_room_history_visibility_properties[PROP_VISIBILITY]);
    }
}

static void
matrix_event_room_history_visibility_finalize(GObject *gobject) {
    G_OBJECT_CLASS(matrix_event_room_history_visibility_parent_class)->finalize(gobject);
}

static void
matrix_event_room_history_visibility_get_property(GObject *gobject, guint property_id, GValue *value, GParamSpec *pspec)
{
    MatrixEventRoomHistoryVisibility *matrix_event_room_history_visibility = MATRIX_EVENT_ROOM_HISTORY_VISIBILITY(gobject);

    switch (property_id) {
        case PROP_VISIBILITY:
            g_value_set_enum(value, matrix_event_room_history_visibility_get_visibility(matrix_event_room_history_visibility));

            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, property_id, pspec);

            break;
    }
}

static void
matrix_event_room_history_visibility_set_property(GObject *gobject, guint property_id, const GValue *value, GParamSpec *pspec)
{
    MatrixEventRoomHistoryVisibility *matrix_event_room_history_visibility = MATRIX_EVENT_ROOM_HISTORY_VISIBILITY(gobject);

    switch (property_id) {
        case PROP_VISIBILITY:
            matrix_event_room_history_visibility_set_visibility(matrix_event_room_history_visibility, g_value_get_enum(value));

            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, property_id, pspec);

            break;
    }
}

static void
matrix_event_room_history_visibility_class_init(MatrixEventRoomHistoryVisibilityClass *klass)
{
    ((MatrixEventBaseClass *)klass)->from_json = matrix_event_room_history_visibility_real_from_json;
    ((MatrixEventBaseClass *)klass)->to_json = matrix_event_room_history_visibility_real_to_json;
    G_OBJECT_CLASS(klass)->get_property = matrix_event_room_history_visibility_get_property;
    G_OBJECT_CLASS(klass)->set_property = matrix_event_room_history_visibility_set_property;
    G_OBJECT_CLASS(klass)->finalize = matrix_event_room_history_visibility_finalize;

    /**
     * MatrixEventRoomHistoryVisibiliy:visibility:
     *
     * Who can see the room history.
     */
    matrix_event_room_history_visibility_properties[PROP_VISIBILITY] = g_param_spec_enum(
            "visibility", "visibility", "visibility",
            MATRIX_TYPE_HISTORY_VISIBILITY, MATRIX_HISTORY_VISIBILITY_UNKNOWN,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_VISIBILITY, matrix_event_room_history_visibility_properties[PROP_VISIBILITY]);
}

static void
matrix_event_room_history_visibility_init(MatrixEventRoomHistoryVisibility *matrix_event_room_history_visibility)
{
    MatrixEventRoomHistoryVisibilityPrivate *priv = matrix_event_room_history_visibility_get_instance_private(matrix_event_room_history_visibility);

    priv->_visibility = MATRIX_HISTORY_VISIBILITY_UNKNOWN;
}
