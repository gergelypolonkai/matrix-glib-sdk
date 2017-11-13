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

#include "matrix-event-base.h"
#include "matrix-types.h"
#include "matrix-enumtypes.h"
#include "config.h"

/**
 * SECTION:matrix-event-base
 * @short_description: abstract base class for Matrix events
 * @title: Base class for events
 *
 * #MatrixEventBase is the abstract base class of Matrix events.  All event classes in this
 * library are derived from it, and custom classes should derived from it, too, if one wants to
 * benefit from #MatrixClient’s (de)serialization capabilities.
 *
 * This class only defines the MatrixEventBase:event-type property, which is tied to the
 * `event_type` field in a JSON representation.  Subclasses should not get or set this property.
 *
 * Event objects can be created from JSON data (a #JsonNode) by calling
 * matrix_event_base_new_from_json(), which will return the correct GObject type as long as
 * it was registered with matrix_event_register_type().
 */
enum  {
    PROP_0,
    PROP_EVENT_TYPE,
    PROP_JSON,
    NUM_PROPS
};

static GParamSpec* matrix_event_base_properties[NUM_PROPS];
static GHashTable *matrix_event_type_handlers = NULL;

typedef struct {
    GError* _construct_error;
    gboolean _inited;
    JsonNode* _json;
    gchar *_event_type;
} MatrixEventBasePrivate;

static void matrix_event_base_g_initable_interface_init (GInitableIface *iface);

/**
 * MatrixEventBase:
 *
 * Abstract base class for event handlers.
 */

/**
 * MatrixEventBaseClass:
 * @from_json: function to initialize themselves from JSON data
 * @to_json: function to export their data to JSON
 *
 * Class structure for #MatrixEventBase.
 */
G_DEFINE_TYPE_EXTENDED(MatrixEventBase, matrix_event_base, G_TYPE_OBJECT, G_TYPE_FLAG_ABSTRACT, G_ADD_PRIVATE (MatrixEventBase) G_IMPLEMENT_INTERFACE(G_TYPE_INITABLE, matrix_event_base_g_initable_interface_init));

static gboolean
matrix_event_base_initable_init(GInitable *g_initable, GCancellable *cancellable, GError **error)
{
    MatrixEventBasePrivate *priv;

    if (cancellable != NULL) {
        g_set_error(error, MATRIX_ERROR, MATRIX_ERROR_UNSUPPORTED,
                    "Cancellable initialization not supported");

        return FALSE;
    }

    priv = matrix_event_base_get_instance_private(MATRIX_EVENT_BASE(g_initable));

    if (priv->_construct_error != NULL) {
        g_propagate_error(error, priv->_construct_error);

        return FALSE;
    }

    priv->_inited = TRUE;

    return TRUE;
}

static void
matrix_event_base_initialize_from_json(MatrixEventBase *matrix_event_base, JsonNode *json_data, GError **error)
{
    MatrixEventBasePrivate *priv = matrix_event_base_get_instance_private(matrix_event_base);
    JsonObject *root;
    JsonNode *node;
    const gchar *json_event_type;
    GError *inner_error = NULL;

    g_return_if_fail(matrix_event_base != NULL);

    if (json_node_get_node_type(json_data) != JSON_NODE_OBJECT) {
        g_set_error(error, MATRIX_ERROR, MATRIX_ERROR_INVALID_FORMAT,
                    "The event is not valid");

        return;
    }

    root = json_node_get_object(json_data);

    if ((node = json_object_get_member(root, "type")) == NULL) {
        g_set_error(error, MATRIX_ERROR, MATRIX_ERROR_INCOMPLETE,
                    "Event type is not specified");

        return;
    }

    json_event_type = json_node_get_string(node);

    if ((priv->_event_type != NULL) && (g_strcmp0(priv->_event_type, json_event_type) != 0)) {
        g_set_error(error, MATRIX_ERROR, MATRIX_ERROR_INVALID_TYPE,
                    "Changing event type is not supported");

        return;
    }

    if ((node = json_object_get_member(root, "content")) == NULL) {
        g_warning("content key is missing from an %s event", json_event_type);

        // As event type objects depend on having this node, let’s add it now.
        JsonNode *content_node = json_node_new(JSON_NODE_OBJECT);
        json_node_set_object(content_node, json_object_new());
        json_object_set_member(root, "content", content_node);
    }

    matrix_event_base_from_json(matrix_event_base, json_data, &inner_error);

    if (inner_error != NULL) {
        g_propagate_error(error, inner_error);

        return;
    }

    g_free(priv->_event_type);
    priv->_event_type = g_strdup(json_event_type);
}

static void
matrix_event_base_real_from_json(MatrixEventBase *matrix_event_base, JsonNode *json_data, GError **error)
{
    MatrixEventBasePrivate *priv = matrix_event_base_get_instance_private(matrix_event_base);
    JsonObject *root;
    JsonNode *node;

    g_return_if_fail(matrix_event_base != NULL);

    root = json_node_get_object(json_data);

    if ((node = json_object_get_member(root, "type")) != NULL) {
        g_free(priv->_event_type);
        priv->_event_type = g_strdup(json_node_get_string(node));
    } else if (DEBUG) {
        g_warning("type is not present in an event");
    }
}

/**
 * matrix_event_base_from_json:
 * @event: a #MatrixEventBase (or derived) object
 * @json_data: a #JsonNode to load data from.  It must hold a #JsonObject
 * @error: a #GError, or %NULL to ignore errors
 *
 * Load data from a JSON object to the fields of @event.
 */
void
matrix_event_base_from_json(MatrixEventBase *matrix_event_base, JsonNode *json_data, GError **error)
{
    g_return_if_fail(matrix_event_base != NULL);
    g_return_if_fail(json_data != NULL);
    g_return_if_fail(json_node_get_node_type(json_data) != JSON_NODE_OBJECT);

    MATRIX_EVENT_BASE_GET_CLASS(matrix_event_base)->from_json(matrix_event_base, json_data, error);
}

static void
matrix_event_base_real_to_json(MatrixEventBase *matrix_event_base, JsonNode *json_data, GError **error)
{
    MatrixEventBasePrivate *priv = matrix_event_base_get_instance_private(matrix_event_base);
    JsonObject *root;

    g_return_if_fail(matrix_event_base != NULL);

    root = json_node_get_object(json_data);

    if (priv->_event_type == NULL) {
        g_set_error(error, MATRIX_ERROR, MATRIX_ERROR_INCOMPLETE,
                    "Won't generate an event without type");

        return;
    }

    json_object_set_string_member(root, "type", priv->_event_type);
}

/**
 * matrix_event_base_to_json:
 * @event: a #MatrixEventBase (or derived) object
 * @json_data: a #JsonNode initialised to hold a #JsonObject
 * @error: a #GError, or %NULL to ignore errors
 *
 * Export event data to a JSON object.
 */
void
matrix_event_base_to_json(MatrixEventBase *matrix_event_base, JsonNode *json_data, GError **error)
{
    g_return_if_fail(matrix_event_base != NULL);
    g_return_if_fail(json_data != NULL);
    g_return_if_fail(json_node_get_node_type(json_data) != JSON_NODE_OBJECT);

    MATRIX_EVENT_BASE_GET_CLASS(matrix_event_base)->to_json(matrix_event_base, json_data, error);
}

/**
 * matrix_event_base_new_from_json:
 * @event_type: (nullable) (transfer none): an event type
 * @json_data: (not nullable) (transfer none): a #JsonNode, holding a #JsonObject
 * @error: (nullable): a #GError, or %NULL to ignore errors
 *
 * Create a new #MatrixEventBase derived object based on @event_type.  If @event_type is %NULL,
 * the event type is taken directly from the JSON data, namely the `"event_type"` field.
 *
 * After figuring out the event type (either from @event_type or from the event itself), this
 * function calls matrix_event_get_handler() to get the handler #GType for this event.  If
 * none found, @error is set to #MATRIX_ERROR_INVALID_TYPE, and this function returns %NULL.
 *
 * The actual return type of this function is the same as the handling class (which is
 * required to be a subclass of #MatrixEventBase).
 *
 * When object initialisation is done, matrix_event_base_from_json() is called to populate
 * the event object.
 *
 * Returns: (transfer full): a new #MatrixEventBase devired object
 */
MatrixEventBase *
matrix_event_base_new_from_json(const gchar *event_type, JsonNode *json_data, GError **error)
{
    GType event_gtype;
    MatrixEventBase *ret = NULL;
    GError *inner_error = NULL;

    if (event_type == NULL) {
        JsonNode *node;

        if (json_data == NULL) {
            g_set_error(error, MATRIX_ERROR, MATRIX_ERROR_INCOMPLETE,
                        "Either event_type or json_data must be set!");

            return NULL;
        }

        if (json_node_get_node_type(json_data) != JSON_NODE_OBJECT) {
            g_set_error(error, MATRIX_ERROR, MATRIX_ERROR_INVALID_FORMAT,
                        "Event is not a JSON object!");

            return NULL;
        }

        if ((node = json_object_get_member(json_node_get_object(json_data), "type")) == NULL) {
            g_set_error(error, MATRIX_ERROR, MATRIX_ERROR_INCOMPLETE,
                        "event_type is null and JSON object doesn't contain type!");

            return NULL;
        }

        event_type = (gchar *)json_node_get_string(node);
    }

    if ((event_gtype = matrix_event_get_handler(event_type)) == G_TYPE_NONE) {
        g_set_error(error, MATRIX_ERROR, MATRIX_ERROR_INVALID_TYPE,
                    "No registered type for event type %s",
                    event_type);

        return NULL;
    }

    ret = (MatrixEventBase *)g_object_new(event_gtype,
                                          "event_type", event_type,
                                          "json", json_data);

    matrix_event_base_initable_init(G_INITABLE(ret), NULL, &inner_error);

    if (inner_error != NULL) {
        g_propagate_error(error, inner_error);

        return NULL;
    }

    return ret;
}

/**
 * matrix_event_base_construct:
 * @object_type: a #GType to construct
 *
 * Returns: (transfer full): a new instance of @object_type
 */
MatrixEventBase *
matrix_event_base_construct(GType object_type)
{
    return (MatrixEventBase *)g_object_new(object_type, NULL);
}

/**
 * matrix_event_base_get_event_type:
 * @event: a #MatrixEventBase (or derived) object
 *
 * Get the event type of @event.
 *
 * The returned value is owned by @event and should not be freed.
 *
 * Returns: (transfer none): the event type
 */
const gchar *
matrix_event_base_get_event_type(MatrixEventBase *matrix_event_base)
{
    MatrixEventBasePrivate *priv = matrix_event_base_get_instance_private(matrix_event_base);

    g_return_val_if_fail(matrix_event_base != NULL, NULL);

    return priv->_event_type;
}

static void
matrix_event_base_set_event_type(MatrixEventBase *matrix_event_base, const gchar *event_type)
{
    MatrixEventBasePrivate *priv = matrix_event_base_get_instance_private(matrix_event_base);

    g_return_if_fail(matrix_event_base != NULL);

    g_free(priv->_event_type);
    priv->_event_type = g_strdup (event_type);

    g_object_notify_by_pspec((GObject *)matrix_event_base,
                             matrix_event_base_properties[PROP_EVENT_TYPE]);
}

JsonNode *
matrix_event_base_get_json(MatrixEventBase *matrix_event_base)
{
    MatrixEventBasePrivate *priv;
    JsonNode *result;
    JsonNode *content_node;
    JsonObject *root;
    JsonObject *content_root;
    GError *inner_error = NULL;

    g_return_val_if_fail(matrix_event_base != NULL, NULL);

    priv = matrix_event_base_get_instance_private(matrix_event_base);

    result = json_node_new (JSON_NODE_OBJECT);
    root = json_object_new();
    json_node_set_object(priv->_json, root);

    content_node = json_node_new(JSON_NODE_OBJECT);
    content_root = json_object_new();
    json_node_set_object(content_node, content_root);
    json_object_set_member(root, "content", content_node);

    matrix_event_base_to_json(matrix_event_base, priv->_json, &inner_error);

    if (inner_error != NULL) {
        g_error("Unable to generate JSON content: %s", inner_error->message);
        g_error_free(inner_error);
        g_object_unref(result);

        return NULL;
    }

    json_node_unref(priv->_json);

    priv->_json = result;

    return priv->_json;
}

static void
matrix_event_base_set_json(MatrixEventBase *matrix_event_base, JsonNode *json)
{
    GError* inner_error = NULL;

    g_return_if_fail(matrix_event_base != NULL);

    if (json != NULL) {
        matrix_event_base_initialize_from_json(matrix_event_base, json, &inner_error);

        if (inner_error != NULL) {
            g_error("Unable to initialise from JSON data: %s", inner_error->message);
            g_error_free(inner_error);

            return;
        }
    }

    g_object_notify_by_pspec((GObject *)matrix_event_base, matrix_event_base_properties[PROP_JSON]);
}

static void
matrix_event_base_get_property(GObject *gobject, guint prop_id, GValue *value, GParamSpec *pspec)
{
    MatrixEventBase *matrix_event_base = MATRIX_EVENT_BASE(gobject);

    switch (prop_id) {
        case PROP_EVENT_TYPE:
            g_value_set_string(value, matrix_event_base_get_event_type(matrix_event_base));

            break;
        case PROP_JSON:
            g_value_set_boxed(value, matrix_event_base_get_json(matrix_event_base));

            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, pspec);

            break;
    }
}

static void
matrix_event_base_set_property(GObject *gobject, guint prop_id, const GValue *value, GParamSpec *pspec)
{
    MatrixEventBase *matrix_event_base = MATRIX_EVENT_BASE(gobject);

    switch (prop_id) {
        case PROP_EVENT_TYPE:
            matrix_event_base_set_event_type(matrix_event_base, g_value_get_string(value));

            break;
        case PROP_JSON:
            matrix_event_base_set_json(matrix_event_base, g_value_get_boxed(value));

            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, pspec);

            break;
    }
}

static void
matrix_event_base_finalize(GObject *gobject)
{
    MatrixEventBase *matrix_event_base = MATRIX_EVENT_BASE(gobject);
    MatrixEventBasePrivate *priv = matrix_event_base_get_instance_private(matrix_event_base);

    if (priv->_construct_error) {
        g_error_free(priv->_construct_error);
    }

    priv->_json = (json_node_unref(priv->_json), NULL);

    g_free(priv->_event_type);

    G_OBJECT_CLASS(matrix_event_base_parent_class)->finalize(gobject);
}

static void
matrix_event_base_class_init(MatrixEventBaseClass *klass)
{
    ((MatrixEventBaseClass *)klass)->from_json = matrix_event_base_real_from_json;
    ((MatrixEventBaseClass *)klass)->to_json = matrix_event_base_real_to_json;

    G_OBJECT_CLASS(klass)->get_property = matrix_event_base_get_property;
    G_OBJECT_CLASS(klass)->set_property = matrix_event_base_set_property;
    G_OBJECT_CLASS(klass)->finalize = matrix_event_base_finalize;

    /**
     * MatrixEventBase:event-type:
     *
     * The type of the event.  It should be namespaced similar to the Java package naming
     * conventions, e.g. `com.example.subdomain.event.type`.  It cannot be changed after object
     * initialization.
     */
    matrix_event_base_properties[PROP_EVENT_TYPE] = g_param_spec_string(
            "event-type", "event-type", "event-type",
            NULL,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY);
    g_object_class_install_property(G_OBJECT_CLASS(klass),
                                    PROP_EVENT_TYPE,
                                    matrix_event_base_properties[PROP_EVENT_TYPE]);

    /**
     * MatrixEventBase:json:
     *
     * The event as a JSON node.
     */
    matrix_event_base_properties[PROP_JSON] = g_param_spec_boxed(
            "json", "json", "json",
            JSON_TYPE_NODE,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY);
    g_object_class_install_property(G_OBJECT_CLASS(klass),
                                    PROP_JSON,
                                    matrix_event_base_properties[PROP_JSON]);
}

static void
matrix_event_base_g_initable_interface_init(GInitableIface *iface)
{
    iface->init = matrix_event_base_initable_init;
}

static void
matrix_event_base_init(MatrixEventBase *matrix_event_base)
{
    MatrixEventBasePrivate *priv = matrix_event_base_get_instance_private(matrix_event_base);

    priv->_construct_error = NULL;
    priv->_inited = FALSE;
    priv->_event_type = NULL;

    matrix_event_base_set_event_type(matrix_event_base, NULL);
}

/**
 * matrix_event_get_handler:
 * @event_type: (transfer none) (not nullable): the event type to look up
 *
 * Get the #GType of the class that is registered to handle events with type @event_type.
 *
 * Returns: a #GType, or #G_TYPE_NONE if no handler is registered
 */
GType
matrix_event_get_handler(const gchar *event_type)
{
    GTypeClass *klass;

    g_return_val_if_fail(event_type != NULL, G_TYPE_NONE);

    if ((klass = (GTypeClass *)g_hash_table_lookup(matrix_event_type_handlers, event_type)) != NULL) {
        return G_TYPE_FROM_CLASS(klass);
    }

    if (DEBUG) {
        g_warning ("matrix-event-base.vala:243: No registered type for %s", event_type);
    }

    return G_TYPE_NONE;
}

/**
 * matrix_event_register_type:
 * @event_type: (transfer none): the type of the event
 * @event_gtype: the #GType of the event’s handler
 * @error: a #GError, or %NULL to ignore errors
 *
 * Registers @event_type to be handled by the type @event_gtype.
 */
void
matrix_event_register_type(const gchar *event_type, GType event_gtype, GError **error)
{
    gchar *key;
    GTypeClass *klass;

    g_return_if_fail(event_type != NULL);

    if (!g_type_is_a (event_gtype, MATRIX_EVENT_TYPE_BASE)) {
        g_set_error(error, MATRIX_ERROR, MATRIX_ERROR_INVALID_TYPE,
                    "Invalid event type handler. It must be a subclass of MatrixEvent");

        return;
    }

    if (matrix_event_type_handlers == NULL) {
        matrix_event_type_handlers = g_hash_table_new_full (g_str_hash, g_str_equal, g_free, g_type_class_unref);
    }

    key = g_strdup (event_type);
    klass = g_type_class_ref (event_gtype);

    g_hash_table_replace(matrix_event_type_handlers, key, klass);
}

/**
 * matrix_event_unregister_type:
 * @event_type: (transfer none): the event type to remove
 *
 * Unregister @param event_type.
 */
void
matrix_event_unregister_type(const gchar *event_type)
{
    g_return_if_fail(event_type != NULL && matrix_event_type_handlers != NULL);

    g_hash_table_remove (matrix_event_type_handlers, event_type);
}
