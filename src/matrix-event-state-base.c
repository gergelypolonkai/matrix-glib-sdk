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

#include "matrix-event-state-base.h"
#include "matrix-enumtypes.h"
#include "config.h"

/**
 * SECTION:matrix-event-state-base
 * @short_description: abstract base class for state events
 * @title: base class for state events
 *
 * #MatrixEventState is the base class for state events.  State events are special room events
 * with an extra `state_key` and `prev_content` event which are handled by this class.
 */

enum  {
    PROP_0,
    PROP_STATE_KEY,
    PROP_PREV_CONTENT,
    NUM_PROPS
};

static GParamSpec *matrix_event_state_properties[NUM_PROPS];

typedef struct {
    JsonNode *_prev_content;
    gchar *_state_key;
} MatrixEventStatePrivate;

/**
 * MatrixEventState:
 *
 * The only exposed field for such events is the state key.  There are some events that may
 * send the state key in a different field; handlers of such events can overwrite this
 * property directly, but otherwise it’s descouraged.
 */
G_DEFINE_ABSTRACT_TYPE_WITH_PRIVATE(MatrixEventState, matrix_event_state, MATRIX_EVENT_TYPE_ROOM);

static void
matrix_event_state_real_from_json(MatrixEventBase *matrix_event_base, JsonNode *json_data, GError **error)
{
    MatrixEventStatePrivate *priv;
    JsonObject *root;
    JsonNode *node;
    GError *inner_error = NULL;

    g_return_if_fail(json_data != NULL);

    priv = matrix_event_state_get_instance_private(MATRIX_EVENT_STATE(matrix_event_base));
    root = json_node_get_object(json_data);

    if ((node = json_object_get_member(root, "state_key")) != NULL) {
        g_free(priv->_state_key);
        priv->_state_key = g_strdup(json_node_get_string(node));
    } else if (DEBUG) {
        g_warning("state_key is not present in a State event");
    }

    if ((node = json_object_get_member(root, "prev_content")) != NULL) {
        priv->_prev_content = node;
    }

    MATRIX_EVENT_BASE_CLASS(matrix_event_state_parent_class)->from_json(matrix_event_base, json_data, &inner_error);

    json_node_unref(node);
    json_object_unref(root);

    if (inner_error != NULL) {
        g_propagate_error(error, inner_error);
    }
}

static void
matrix_event_state_real_to_json(MatrixEventBase *matrix_event_base, JsonNode *json_node, GError **error)
{
    MatrixEventState *matrix_event_state;
    MatrixEventStatePrivate *priv;
    JsonObject *root;
    GError *inner_error = NULL;

    matrix_event_state = MATRIX_EVENT_STATE(matrix_event_base);
    priv = matrix_event_state_get_instance_private(matrix_event_state);

    if (priv->_state_key == NULL) {
        g_set_error(error, MATRIX_ERROR, MATRIX_ERROR_INCOMPLETE,
                    "Won't generate state events without state_key");

        return;
    }

    root = json_node_get_object(json_node);

    json_object_set_string_member(root, "state_key", priv->_state_key);

    if (priv->_prev_content != NULL) {
        json_object_set_member(root, "prev_content", priv->_prev_content);
    }

    MATRIX_EVENT_BASE_CLASS(matrix_event_state_parent_class)->to_json(MATRIX_EVENT_BASE(matrix_event_base), json_node, &inner_error);

    json_object_unref(root);

    if (inner_error != NULL) {
        g_propagate_error(error, inner_error);
    }
}

/**
 * matrix_event_state_get_stripped_node:
 * @event: a #MatrixEventState derived object
 *
 * Get a stripped state event.
 *
 * Returns: (transfer full) (nullable): %NULL if the event is not allowed to be stripped, or
 *     the full JSON node otherwise
 */
JsonNode *
matrix_event_state_get_stripped_node(MatrixEventState *matrix_event_state)
{
    MatrixEventBase *matrix_event_base;
    const gchar *event_type;

    g_return_val_if_fail(matrix_event_state != NULL, NULL);

    matrix_event_base = MATRIX_EVENT_BASE(matrix_event_state);
    event_type = matrix_event_base_get_event_type(matrix_event_base);

    // TODO: this may be controlled by an object property instead.
    if ((g_strcmp0(event_type, "m.room.join_rules") != 0) &&
        (g_strcmp0(event_type, "m.room.canonical_alias") != 0) &&
        (g_strcmp0(event_type, "m.room.avatar") != 0) &&
        (g_strcmp0(event_type, "m.room.name") != 0)) {
            g_warning("Trying to strip down event that is not allowed to be stripped.");

            return NULL;
        }

    return matrix_event_base_get_json(matrix_event_base);
}

MatrixEventState *
matrix_event_state_construct(GType object_type)
{
    return (MatrixEventState *)matrix_event_room_construct(object_type);
}

/**
 * matrix_event_state_get_state_key:
 * @event: a #MatrixEventState derived object
 *
 * Get the state key of @event.
 *
 * The returned value is owned by @event and should not be freed.
 *
 * Returns: (transfer none) (nullable): the state key
 */
const gchar *
matrix_event_state_get_state_key(MatrixEventState *matrix_event_state)
{
    MatrixEventStatePrivate *priv = matrix_event_state_get_instance_private(matrix_event_state);

    g_return_val_if_fail(matrix_event_state != NULL, NULL);

    return priv->_state_key;
}

/**
 * matrix_event_state_set_state_key:
 * @event: a #MatrixEventState derived object
 * @state_key: a state key
 *
 * Set the state key of @event.
 */
void
matrix_event_state_set_state_key(MatrixEventState *matrix_event_state, const gchar *state_key)
{
    MatrixEventStatePrivate *priv = matrix_event_state_get_instance_private(matrix_event_state);

    g_return_if_fail(matrix_event_state != NULL);

    g_free(priv->_state_key);
    priv->_state_key = g_strdup(state_key);

    g_object_notify_by_pspec((GObject *)matrix_event_state, matrix_event_state_properties[PROP_STATE_KEY]);
}

/**
 * matrix_event_state_get_prev_content:
 * @event: a #MatrixEventState derived object
 *
 * Get the known previous content for the state represented by @event.
 *
 * The value returned is owned by @event and should not be freed.
 *
 * Returns: (transfer none) (nullable): a #JsonNode representing the last known state of @event
 */
JsonNode *
matrix_event_state_get_prev_content(MatrixEventState *matrix_event_state)
{
    MatrixEventStatePrivate *priv;

    g_return_val_if_fail(matrix_event_state != NULL, NULL);

    priv = matrix_event_state_get_instance_private(matrix_event_state);

    return priv->_prev_content;
}

/**
 * matrix_event_state_set_prev_content:
 * @event: a #MatrixEventState derived object
 * @prev_content: (transfer none) (nullable): the last known content of the state
 *     represented by @event
 *
 * Set the last known content of the state represented by @event.  This is required to prevent
 * the race condition when a client tries to overwrite a state that has been changed since then.
 */
void
matrix_event_state_set_prev_content(MatrixEventState *matrix_event_state, JsonNode *prev_content)
{
    MatrixEventStatePrivate *priv;

    g_return_if_fail(matrix_event_state != NULL);

    priv = matrix_event_state_get_instance_private(matrix_event_state);

    if (priv->_prev_content != prev_content) {
        json_node_unref(priv->_prev_content);
        priv->_prev_content = json_node_ref(prev_content);

        g_object_notify_by_pspec((GObject *)matrix_event_state, matrix_event_state_properties[PROP_PREV_CONTENT]);
    }
}

static void
matrix_event_state_finalize(GObject *gobject)
{
    MatrixEventState *matrix_event_state = MATRIX_EVENT_STATE(gobject);
    MatrixEventStatePrivate *priv = matrix_event_state_get_instance_private(matrix_event_state);

    g_free(priv->_state_key);
    json_node_unref(priv->_prev_content);

    G_OBJECT_CLASS(matrix_event_state_parent_class)->finalize(gobject);
}

static void
matrix_event_state_get_property(GObject *gobject, guint property_id, GValue* value, GParamSpec* pspec)
{
    MatrixEventState *matrix_event_state = MATRIX_EVENT_STATE(gobject);
    MatrixEventStatePrivate *priv = matrix_event_state_get_instance_private(matrix_event_state);

    switch (property_id) {
        case PROP_STATE_KEY:
            g_value_set_string(value, priv->_state_key);

            break;
        case PROP_PREV_CONTENT:
            g_value_set_boxed(value, priv->_prev_content);

            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, property_id, pspec);

            break;
    }
}

static void
matrix_event_state_set_property(GObject *gobject, guint property_id, const GValue* value, GParamSpec* pspec)
{
    MatrixEventState *matrix_event_state = MATRIX_EVENT_STATE(gobject);

    switch (property_id) {
        case PROP_STATE_KEY:
            matrix_event_state_set_state_key(matrix_event_state, g_value_get_string(value));

            break;
        case PROP_PREV_CONTENT:
            matrix_event_state_set_prev_content(matrix_event_state, g_value_get_boxed(value));

            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, property_id, pspec);

            break;
    }
}

static void
matrix_event_state_class_init(MatrixEventStateClass *klass)
{
    ((MatrixEventBaseClass *)klass)->from_json = matrix_event_state_real_from_json;
    ((MatrixEventBaseClass *)klass)->to_json = matrix_event_state_real_to_json;
    G_OBJECT_CLASS(klass)->get_property = matrix_event_state_get_property;
    G_OBJECT_CLASS(klass)->set_property = matrix_event_state_set_property;
    G_OBJECT_CLASS(klass)->finalize = matrix_event_state_finalize;

    /**
     * MatrixEventState:state-key:
     *
     * The state key of the event.
     */
    matrix_event_state_properties[PROP_STATE_KEY] = g_param_spec_string(
            "state-key", "state-key", "state-key",
            NULL,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_STATE_KEY, matrix_event_state_properties[PROP_STATE_KEY]);

    /**
     * MatrixEventState:prev-content:
     *
     * The previous known content of the state
     */
    matrix_event_state_properties[PROP_PREV_CONTENT] = g_param_spec_boxed(
            "prev-content", "prev-content", "prev-content",
            JSON_TYPE_NODE,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_PREV_CONTENT, matrix_event_state_properties[PROP_PREV_CONTENT]);
}

static void
matrix_event_state_init(MatrixEventState *matrix_event_state)
{
    MatrixEventStatePrivate *priv;

    priv = matrix_event_state_get_instance_private(matrix_event_state);

    priv->_prev_content = NULL;
    priv->_state_key = NULL;
}
