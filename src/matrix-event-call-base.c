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

#include "matrix-event-call-base.h"
#include "matrix-types.h"
#include "matrix-enumtypes.h"

/**
 * SECTION:matrix-event-call-base
 * @short_description: Abstract base class for call related events
 *
 * Base class for m.call.* events.
 */

enum  {
    PROP_0,
    PROP_CALL_ID,
    PROP_VERSION,
    NUM_PROPERTIES
};

static GParamSpec *matrix_event_call_properties[NUM_PROPERTIES];

typedef struct {
    gchar *call_id;
    gint _version;
} MatrixEventCallPrivate;

/**
 * MatrixEventCall:
 *
 * Object structure.
 */
G_DEFINE_TYPE_WITH_PRIVATE(MatrixEventCall, matrix_event_call, MATRIX_EVENT_TYPE_ROOM);

static void
matrix_event_call_real_from_json(MatrixEventBase *matrix_event_base, JsonNode *json_data, GError **error)
{
    MatrixEventCallPrivate *priv;
    JsonObject *root;
    JsonObject *content_root;
    JsonNode *content_node;
    JsonNode *node;

    g_return_if_fail(json_data != NULL);

    priv = matrix_event_call_get_instance_private(MATRIX_EVENT_CALL(matrix_event_base));
    root = json_node_get_object(json_data);
    content_node = json_object_get_member(root, "content");
    content_root = json_node_get_object(content_node);

    if ((node = json_object_get_member(content_root, "call_id")) != NULL) {
        g_free(priv->call_id);
        priv->call_id = g_strdup(json_node_get_string(node));
    } else {
        g_warning("content.call_id is missing from a m.call.* event");
    }

    if ((node = json_object_get_member(content_root, "version")) != NULL) {
        priv->_version = json_node_get_int(node);
    } else {
        g_warning("content.version is missing from a m.call.* event");
    }

    MATRIX_EVENT_BASE_CLASS(matrix_event_call_parent_class)->from_json(matrix_event_base, json_data, error);
}

static void
matrix_event_call_real_to_json(MatrixEventBase *matrix_event_base, JsonNode *json_data, GError **error)
{
    MatrixEventCallPrivate *priv;
    JsonObject *root;
    JsonObject *content_root;
    JsonNode *content_node;

    g_return_if_fail(json_data != NULL);

    priv = matrix_event_call_get_instance_private(MATRIX_EVENT_CALL(matrix_event_base));

    if (priv->call_id == NULL) {
        g_set_error(error, MATRIX_ERROR, MATRIX_ERROR_INCOMPLETE,
                    "Won't generate a m.call.hangup event without call_id");

        return;
    }

    if (priv->_version < 0) {
        g_set_error(error, MATRIX_ERROR, MATRIX_ERROR_INCOMPLETE,
                    "Won't generate a m.call.hangup event without version");

        return;
    }

    root = json_node_get_object(json_data);
    content_node = json_object_get_member(root, "content");
    content_root = json_node_get_object(content_node);

    json_object_set_string_member(content_root, "call_id", priv->call_id);
    json_object_set_int_member(content_root, "version", priv->_version);

    MATRIX_EVENT_BASE_CLASS(matrix_event_call_parent_class)->to_json(matrix_event_base, json_data, error);
}

/**
 * matrix_event_call_construct:
 * @object_type: the #GType of the object to be created
 *
 * Returns: (transfer full): a new instance of @object_type
 */
MatrixEventCall *
matrix_event_call_construct(GType object_type)
{
    return (MatrixEventCall *)matrix_event_room_construct(object_type);
}

/**
 * matrix_event_call_get_call_id:
 * @event: a #MatrixEventCall
 *
 * Get the identifier of the call.
 *
 * The value returned is owned by @event and should not be freed.
 *
 * Returns: (transfer none) (nullable): the call ID
 */
const gchar *
matrix_event_call_get_call_id(MatrixEventCall *matrix_event_call)
{
    MatrixEventCallPrivate *priv;

    g_return_val_if_fail(matrix_event_call != NULL, NULL);

    priv = matrix_event_call_get_instance_private(matrix_event_call);

    return priv->call_id;
}

/**
 * matrix_event_call_set_call_id:
 * @event: a #MatrixEventCall
 * @call_id: a call identifier
 *
 * Set the identifier of the call represented by @event.
 */
void
matrix_event_call_set_call_id(MatrixEventCall *matrix_event_call, const gchar *call_id)
{
    MatrixEventCallPrivate *priv;

    g_return_if_fail(matrix_event_call != NULL);

    priv = matrix_event_call_get_instance_private(matrix_event_call);

    if (g_strcmp0(call_id, priv->call_id) != 0) {
        g_free(priv->call_id);
        priv->call_id = g_strdup(call_id);

        g_object_notify_by_pspec((GObject *)matrix_event_call, matrix_event_call_properties[PROP_CALL_ID]);
    }
}

/**
 * matrix_event_call_get_version:
 * @event: a #MatrixEventCall
 *
 * Get the version of the call.
 *
 * Returns: the call version
 */
gint
matrix_event_call_get_version(MatrixEventCall *matrix_event_call)
{
    MatrixEventCallPrivate *priv;

    g_return_val_if_fail(matrix_event_call != NULL, 0);

    priv = matrix_event_call_get_instance_private(matrix_event_call);

    return priv->_version;
}

/**
 * matrix_event_call_set_version:
 * @event: a #MatrixEventCall
 * @version: a call version
 *
 * Set the version of the call.
 */
void
matrix_event_call_set_version(MatrixEventCall *matrix_event_call, gint version)
{
    MatrixEventCallPrivate *priv;

    g_return_if_fail(matrix_event_call != NULL);

    priv = matrix_event_call_get_instance_private(matrix_event_call);

    if (priv->_version != version) {
        priv->_version = version;

        g_object_notify_by_pspec((GObject *)matrix_event_call, matrix_event_call_properties[PROP_VERSION]);
    }
}

static void
matrix_event_call_finalize(GObject *gobject)
{
    MatrixEventCallPrivate *priv = matrix_event_call_get_instance_private(MATRIX_EVENT_CALL(gobject));

    g_free(priv->call_id);

    G_OBJECT_CLASS(matrix_event_call_parent_class)->finalize(gobject);
}

static void
matrix_event_call_get_property(GObject *gobject, guint property_id, GValue *value, GParamSpec *pspec)
{
    MatrixEventCall *matrix_event_call = MATRIX_EVENT_CALL(gobject);

    switch (property_id) {
        case PROP_CALL_ID:
            g_value_set_string(value, matrix_event_call_get_call_id(matrix_event_call));

            break;
        case PROP_VERSION:
            g_value_set_int(value, matrix_event_call_get_version(matrix_event_call));

            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, property_id, pspec);

            break;
    }
}

static void
matrix_event_call_set_property(GObject *gobject, guint property_id, const GValue *value, GParamSpec *pspec)
{
    MatrixEventCall *matrix_event_call = MATRIX_EVENT_CALL(gobject);

    switch (property_id) {
        case PROP_CALL_ID:
            matrix_event_call_set_call_id(matrix_event_call, g_value_get_string(value));

            break;
        case PROP_VERSION:
            matrix_event_call_set_version(matrix_event_call, g_value_get_int(value));

            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, property_id, pspec);

            break;
    }
}

static void
matrix_event_call_class_init(MatrixEventCallClass *klass)
{
    ((MatrixEventBaseClass *)klass)->from_json = matrix_event_call_real_from_json;
    ((MatrixEventBaseClass *)klass)->to_json = matrix_event_call_real_to_json;
    G_OBJECT_CLASS(klass)->get_property = matrix_event_call_get_property;
    G_OBJECT_CLASS(klass)->set_property = matrix_event_call_set_property;
    G_OBJECT_CLASS(klass)->finalize = matrix_event_call_finalize;

    /**
     * MatrixEventCall:call-id:
     *
     * The ID of the call this event relates to.
     */
    matrix_event_call_properties[PROP_CALL_ID] = g_param_spec_string(
            "call-id", "call-id", "call-id",
            NULL,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_CALL_ID, matrix_event_call_properties[PROP_CALL_ID]);

    /**
     * MatrixEventCall:version:
     *
     * The version of the VoIP specification this message adheres to.
     */
    matrix_event_call_properties[PROP_VERSION] = g_param_spec_int(
            "version", "version", "version",
             -1, G_MAXINT, -1,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_VERSION, matrix_event_call_properties[PROP_VERSION]);
}

static void
matrix_event_call_init(MatrixEventCall *matrix_event_call)
{
    MatrixEventCallPrivate *priv = matrix_event_call_get_instance_private(matrix_event_call);

    priv->call_id = NULL;
    priv->_version = -1;
}
