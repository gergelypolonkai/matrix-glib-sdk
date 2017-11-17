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

#include "matrix-message-base.h"
#include "matrix-enumtypes.h"
#include "config.h"

/**
 * SECTION:message-handling
 * @short_description: Message handling
 * @title: Handling message objects
 * @section_id: message_handling
 *
 * To handle message objects, one can use the built-in message classes like #MatrixMessageText,
 * or, for non-standard message types, implement their own by subclassing #MatrixMessageBase.
 *
 * After the class is registered in the GType system, one should call
 * matrix_message_register_type():
 *
 * |[
 * G_DEFINE_TYPE(MyCustomMessage, my_custom_message, MATRIX_MESSAGE_TYPE_BASE);
 *
 * // implement the from_json&lpar;&rpar; and to_json&lpar;&rpar; methods
 *
 * void
 * some_func(JsonNode *custom_message_data)
 * {
 *     GError *error = NULL;
 *     MatrixMessageBase *message;
 *
 *     matrix_message_register_type("my.custom.message", my_custom_message_get_type&lpar;&rpar;, NULL);
 *
 *     message = matrix_message_base_new_from_json(json_data, &error);
 *
 *     if (error != NULL) {
 *         g_error("Could not parse JSON data as a message.");
 *
 *         return;
 *     }
 *
 *     if (!MY_IS_CUSTOM_MESSAGE(message)) {
 *         g_error("The message is not of 'my.custom.message'!");
 *
 *         return;
 *     }
 *
 *     // Do something with message
 * }
 * ]|
 *
 * Registered message types are also used by classes implementing the #MatrixClient interface.
 */

/**
 * SECTION:matrix-message-base
 * @short_description: Base class for Matrix messages
 * @title: Message handler base class
 *
 * Base class for Matrix message handlers.
 */

enum  {
    PROP_0,
    PROP_MESSAGE_TYPE,
    PROP_BODY,
    PROP_JSON,
    NUM_PROPS
};

static GParamSpec *matrix_message_base_properties[NUM_PROPS];

typedef struct {
    gchar* _message_type;
    gchar* _body;
    gboolean _inited;
    GError* _construct_error;
    JsonNode* _json;
} MatrixMessageBasePrivate;


GHashTable *matrix_message_type_handlers = NULL;

static void matrix_message_base_g_initable_interface_init(GInitableIface *iface);

/**
 * MatrixMessageBase:
 *
 * Base class for message handlers.
 */

/**
 * MatrixMessageBaseClass:
 * @from_json: function to initialise a #MatrixMessageBase derived object from JSON data.  Must be
 * implemented.
 * @to_json: function to serialise a #MatrixMessageBase derived object to JSON.  Must be
 * implemented.
 *
 * The class structure for the #MatrixMessageBase type.
 */
G_DEFINE_TYPE_EXTENDED(MatrixMessageBase, matrix_message_base, G_TYPE_OBJECT, G_TYPE_FLAG_ABSTRACT, G_ADD_PRIVATE(MatrixMessageBase) G_IMPLEMENT_INTERFACE(G_TYPE_INITABLE, matrix_message_base_g_initable_interface_init));

static gboolean
matrix_message_base_real_init(GInitable *g_initable, GCancellable *cancellable, GError **error)
{
    MatrixMessageBasePrivate *priv = matrix_message_base_get_instance_private(MATRIX_MESSAGE_BASE(g_initable));

    if (cancellable != NULL) {
        g_set_error(error, MATRIX_ERROR, MATRIX_ERROR_UNSUPPORTED,
                    "Cancellable initialization not supported");

        return FALSE;
    }

    if (priv->_construct_error != NULL) {
        g_propagate_error(error, priv->_construct_error);

        return FALSE;
    }

    priv->_inited = TRUE;

    return TRUE;
}

static void
matrix_message_base_initialize_from_json(MatrixMessageBase *matrix_message_base, JsonNode *json_data, GError **error)
{
    JsonGenerator *gen;
    GError *inner_error = NULL;

    if (json_node_get_node_type(json_data) != JSON_TYPE_OBJECT) {
        g_set_error(error, MATRIX_ERROR, MATRIX_ERROR_INVALID_FORMAT,
                    "The message is not valid");

        return;
    }

    gen = json_generator_new();
    json_generator_set_root(gen, json_data);

    matrix_message_base_from_json(matrix_message_base, json_data, &inner_error);

    g_object_unref(gen);

    if (inner_error != NULL) {
        g_propagate_error(error, inner_error);
    }
}

/**
 * matrix_message_base_new_from_json: (constructor)
 * @json_data: a #JsonNode containing a Matrix message
 * @error: a #GError, or NULL to ignore errors
 *
 * Create a new message object from the message contained in @json_data.
 *
 * The exact type of the returned object is based on the message contents (precisely, its
 * `msgtype` field).  This function uses matrix_message_get_handler() internally to find the
 * message handler for the message type; if there is none, this function returns NULL, but doesn’t
 * set @error.  If there is an error during reading or processing the JSON data, NULL is returned,
 * and @error is set accordingly.
 *
 * See the description of matrix_message_register_type() for more details.
 *
 * Returns: (transfer full): a new #MatrixMessageBase object or NULL in case of an error
 */
MatrixMessageBase *
matrix_message_base_new_from_json(JsonNode *json_data, GError **error)
{
    JsonObject *root;
    JsonNode *node;
    GType msg_gtype;
    MatrixMessageBase *ret;
    GError *inner_error = NULL;

    g_return_val_if_fail(json_data != NULL, NULL);

    root = json_node_get_object(json_data);

    if ((node = json_object_get_member(root, "msgtype")) == NULL) {
        g_set_error(error, MATRIX_ERROR, MATRIX_ERROR_INCOMPLETE,
                    "Can not process a message without msgtype");

        return NULL;
    }

    if ((msg_gtype = matrix_message_get_handler(json_node_get_string(node))) == G_TYPE_NONE) {
        return NULL;
    }

    ret = (MatrixMessageBase *)g_object_new(msg_gtype,
                                            "json", json_data,
                                            NULL);

    g_initable_init(G_INITABLE(ret), NULL, &inner_error);

    if (inner_error != NULL) {
        g_propagate_error(error, inner_error);
        g_object_unref(ret);

        return NULL;
    }

    return ret;
}

static void
matrix_message_base_real_from_json(MatrixMessageBase *matrix_message_base, JsonNode *json_data, GError **error)
{
    MatrixMessageBasePrivate *priv;
    JsonObject *root;
    JsonNode *node;

    g_return_if_fail (json_data != NULL);

    priv = matrix_message_base_get_instance_private(matrix_message_base);
    root = json_node_get_object(json_data);

    if ((node = json_object_get_member(root, "msgtype")) != NULL) {
        g_free(priv->_message_type);
        priv->_message_type = g_strdup(json_node_get_string(node));
    } else if (DEBUG) {
        g_warning("msgtype is not present in a message");
    }

    if ((node = json_object_get_member(root, "body")) != NULL) {
        g_free(priv->_body);
        priv->_body = g_strdup(json_node_get_string(node));
    } else if (DEBUG) {
        g_warning("body is not presente in a message");
    }
}

/**
 * matrix_message_base_from_json: (virtual from_json)
 * @message: An object derived from the #MatrixMessageBase class
 * @json_data: the #JsonNode to initialise to set object properties from
 * @error: a #GError, or NULL to ignore errors
 *
 * Initializes the message object using the data in @json_data.
 *
 * For #MatrixMessageBase objects, this method reads the `msgtype` and `body` fields, saving them
 * to the corresponding properties.  Subclasses should implement this method to initialize
 * themselves from JSON data.
 */
void
matrix_message_base_from_json(MatrixMessageBase *matrix_message_base, JsonNode *json_data, GError **error)
{
    g_return_if_fail(matrix_message_base != NULL);

    MATRIX_MESSAGE_BASE_GET_CLASS(matrix_message_base)->from_json(matrix_message_base, json_data, error);
}

static void
matrix_message_base_real_to_json(MatrixMessageBase *matrix_message_base, JsonNode *json_data, GError **error)
{
    MatrixMessageBasePrivate *priv;
    JsonObject *root;

    g_return_if_fail(json_data != NULL);

    priv = matrix_message_base_get_instance_private(matrix_message_base);
    root = json_node_get_object(json_data);

    if (priv->_message_type == NULL) {
        g_set_error(error, MATRIX_ERROR, MATRIX_ERROR_INCOMPLETE,
                    "Won't generate a message with an empty msgtype");

        return;
    }

    if (priv->_body == NULL) {
        g_set_error(error, MATRIX_ERROR, MATRIX_ERROR_INCOMPLETE,
                    "Won't generate a message without body");

        return;
    }

    json_object_set_string_member(root, "msgtype", priv->_message_type);
    json_object_set_string_member(root, "body", priv->_body);
}

/**
 * matrix_message_base_to_json: (virtual to_json)
 * @message: An object derived from the #MatrixMessageBase class
 * @json_data: A #JsonNode that must be initialised as a #JSON_NODE_OBJECT
 * @error: A #GError, or NULL to ignore errors
 *
 * Serializes the message object into a #JsonNode in @json_data.  Subclasses should implement this
 * method to export their data to JSON.
 */
void
matrix_message_base_to_json(MatrixMessageBase *matrix_message_base, JsonNode *json_data, GError **error)
{
    g_return_if_fail(matrix_message_base != NULL);

    MATRIX_MESSAGE_BASE_GET_CLASS(matrix_message_base)->to_json(matrix_message_base, json_data, error);
}

MatrixMessageBase *
matrix_message_base_construct(GType object_type)
{
    return  (MatrixMessageBase *)g_object_new(object_type, NULL);
}

/**
 * matrix_message_base_get_message_type:
 * @message: an object derived from the #MatrixMessageBase class
 *
 * Get the message type of @message.
 *
 * Returns: (transfer none): the message type
 */
const gchar *
matrix_message_base_get_message_type(MatrixMessageBase *matrix_message_base)
{
    MatrixMessageBasePrivate *priv;

    g_return_val_if_fail(matrix_message_base != NULL, NULL);

    priv = matrix_message_base_get_instance_private(matrix_message_base);

    return priv->_message_type;
}

/**
 * matrix_message_base_set_message_type:
 * @message: an object derived from the #MatrixMessageBase class
 * @message_type: (transfer none): the message type to set
 *
 * Set the message type of @message.
 */
void
matrix_message_base_set_message_type(MatrixMessageBase *matrix_message_base, const gchar *message_type)
{
    MatrixMessageBasePrivate *priv;

    g_return_if_fail(matrix_message_base != NULL);

    priv = matrix_message_base_get_instance_private(matrix_message_base);

    if (g_strcmp0(message_type, priv->_message_type) != 0) {
        g_free(priv->_message_type);
        priv->_message_type = g_strdup(message_type);

        g_object_notify_by_pspec((GObject *)matrix_message_base, matrix_message_base_properties[PROP_MESSAGE_TYPE]);
    }
}

/**
 * matrix_message_base_get_body:
 * @message: an object derived from the #MatrixMessageBase class
 *
 * Get the body of the message.  For some message type this can be used as a fallback content.
 *
 * Returns: (transfer none): the body of @message
 */
const gchar *
matrix_message_base_get_body(MatrixMessageBase *matrix_message_base)
{
    MatrixMessageBasePrivate *priv;

    g_return_val_if_fail(matrix_message_base != NULL, NULL);

    priv = matrix_message_base_get_instance_private(matrix_message_base);

    return priv->_body;
}

/**
 * matrix_message_base_set_body:
 * @message: an object derived from the #MatrixMessageBase class
 * @body: (transfer none): the body to set
 *
 * Set the body of the message.
 */
void
matrix_message_base_set_body(MatrixMessageBase *matrix_message_base, const gchar *body)
{
    MatrixMessageBasePrivate *priv;

    g_return_if_fail(matrix_message_base != NULL);

    priv = matrix_message_base_get_instance_private(matrix_message_base);

    if (g_strcmp0(body, priv->_body) != 0) {
        g_free(priv->_body);
        priv->_body = g_strdup(body);

        g_object_notify_by_pspec((GObject *)matrix_message_base, matrix_message_base_properties[PROP_BODY]);
    }
}

/**
 * matrix_message_base_get_json:
 * @message: an object derived from the #MatrixMessageBase class
 * @error: a #GError, or NULL to ignore errors
 *
 * Get the #MatrixMessageBase:json property of @message.  If an error happens during
 * serialization, this function returns NULL, and sets @error accordingly.
 *
 * This function calls matrix_message_base_to_json() internally.
 */
JsonNode *
matrix_message_base_get_json(MatrixMessageBase *matrix_message_base, GError **error)
{
    MatrixMessageBasePrivate *priv;
    GError *inner_error = NULL;

    g_return_val_if_fail(matrix_message_base != NULL, NULL);

    priv = matrix_message_base_get_instance_private(matrix_message_base);
    priv->_json = json_node_new(JSON_TYPE_OBJECT);
    json_node_set_object(priv->_json, json_object_new());

    matrix_message_base_to_json(matrix_message_base, priv->_json, &inner_error);

    if (inner_error != NULL) {
        g_propagate_error(error, inner_error);

        json_node_unref(priv->_json);
        priv->_json = NULL;

        return NULL;
    }

    return priv->_json;
}

static void
matrix_message_base_set_json(MatrixMessageBase *matrix_message_base, JsonNode *json_data)
{
    GError *inner_error = NULL;

    g_return_if_fail(matrix_message_base != NULL);

    matrix_message_base_initialize_from_json(matrix_message_base, json_data, &inner_error);

    if (inner_error != NULL) {
        g_error("Unable to initialise MatrixMessageBase from JSON: %s", inner_error->message);

        g_clear_error(&inner_error);

        return;
    }

    g_object_notify_by_pspec((GObject *)matrix_message_base, matrix_message_base_properties[PROP_JSON]);
}

static void
matrix_message_base_finalize(GObject *gobject)
{
    MatrixMessageBasePrivate *priv = matrix_message_base_get_instance_private(MATRIX_MESSAGE_BASE(gobject));

    g_free(priv->_message_type);
    g_free(priv->_body);
    g_clear_error(&priv->_construct_error);
    json_node_unref(priv->_json);

    G_OBJECT_CLASS (matrix_message_base_parent_class)->finalize(gobject);
}

static void
matrix_message_base_get_property(GObject *gobject, guint property_id, GValue *value, GParamSpec *pspec)
{
    MatrixMessageBase *matrix_message_base = MATRIX_MESSAGE_BASE(gobject);

    switch (property_id) {
        case PROP_MESSAGE_TYPE:
            g_value_set_string(value, matrix_message_base_get_message_type(matrix_message_base));

            break;
        case PROP_BODY:
            g_value_set_string(value, matrix_message_base_get_body(matrix_message_base));

            break;
        case PROP_JSON:
            g_value_set_boxed(value, matrix_message_base_get_json(matrix_message_base, NULL));

            break;
        default:

            G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, property_id, pspec);

            break;
    }
}

static void
matrix_message_base_set_property(GObject *gobject, guint property_id, const GValue *value, GParamSpec *pspec)
{
    MatrixMessageBase *matrix_message_base = MATRIX_MESSAGE_BASE(gobject);

    switch (property_id) {
        case PROP_MESSAGE_TYPE:
            matrix_message_base_set_message_type(matrix_message_base, g_value_get_string(value));

            break;
        case PROP_BODY:
            matrix_message_base_set_body(matrix_message_base, g_value_get_string(value));

            break;
        case PROP_JSON:
            matrix_message_base_set_json(matrix_message_base, g_value_get_boxed(value));

            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, property_id, pspec);

            break;
    }
}

static void
matrix_message_base_class_init(MatrixMessageBaseClass *klass)
{
    klass->from_json = matrix_message_base_real_from_json;
    klass->to_json = matrix_message_base_real_to_json;
    G_OBJECT_CLASS(klass)->get_property = matrix_message_base_get_property;
    G_OBJECT_CLASS(klass)->set_property = matrix_message_base_set_property;
    G_OBJECT_CLASS(klass)->finalize = matrix_message_base_finalize;

    /**
     * MatrixMessageBase:message-type:
     *
     * The message type.
     */
    matrix_message_base_properties[PROP_MESSAGE_TYPE] = g_param_spec_string(
            "message-type", "message-type", "message-type",
            NULL,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_MESSAGE_TYPE, matrix_message_base_properties[PROP_MESSAGE_TYPE]);

    /**
     * MatrixMessageBase:body:
     *
     * The body of the message.
     */
    matrix_message_base_properties[PROP_BODY] = g_param_spec_string(
            "body", "body", "body",
            NULL,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_BODY, matrix_message_base_properties[PROP_BODY]);

    /**
     * MatrixMessageBase:json:
     *
     * The JSON node corresponding to the message.
     */
    matrix_message_base_properties[PROP_JSON] = g_param_spec_boxed(
            "json", "json", "json",
            JSON_TYPE_NODE,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_JSON, matrix_message_base_properties[PROP_JSON]);
}

static void
matrix_message_base_g_initable_interface_init(GInitableIface *iface)
{

    iface->init = matrix_message_base_real_init;
}

static void
matrix_message_base_init(MatrixMessageBase *matrix_message_base) {
    MatrixMessageBasePrivate *priv = matrix_message_base_get_instance_private(matrix_message_base);

    priv->_message_type = NULL;
    priv->_body = NULL;
    priv->_inited = FALSE;
    priv->_construct_error = NULL;
    priv->_json = NULL;
}

/**
 * matrix_message_get_handler:
 * @message_type: (transfer none): the message type to look up
 *
 * Get the #GType of the class that is registered to handle messages with type @message_type.
 *
 * Returns: a #GType, or #G_TYPE_NONE if no handler is registered
 */
GType
matrix_message_get_handler(const gchar *message_type)
{
    GTypeClass* klass = NULL;

    g_return_val_if_fail(message_type != NULL, G_TYPE_NONE);

    if (matrix_message_type_handlers != NULL) {
        if ((klass = (GTypeClass *)g_hash_table_lookup (matrix_message_type_handlers, message_type)) != NULL) {
            return G_TYPE_FROM_CLASS(klass);
        }
    }

    if (DEBUG) {
        g_warning ("matrix-message-base.vala:192: No registered type for message %s", message_type);
    }

    return G_TYPE_NONE;
}

/**
 * matrix_message_register_type:
 * @message_type: (transfer none): the type of the message
 * @message_gtype: the #GType of the message’s handler
 * @error: a #GError, or NULL to ignore errors
 *
 * Register @message_type to be handled by the type @message_gtype.  If the message type is
 * already registered, @error is set to #MATRIX_ERROR_ALREADY_EXISTS; in such cases, one should
 * call matrix_message_unregister_type() first.
 *
 * If @message_gtype is not a subclass of #MatrixMessageBase, @error is sot to
 * #MATRIX_ERROR_INVALID_TYPE.
 */
void
matrix_message_register_type(const gchar *message_type, GType message_gtype, GError **error)
{
    gchar *key;
    GTypeClass *value;

    g_return_if_fail(message_type != NULL);

    if (!g_type_is_a (message_gtype, MATRIX_MESSAGE_TYPE_BASE)) {
        g_set_error(error, MATRIX_ERROR, MATRIX_ERROR_INVALID_TYPE, "Invalid message handler type");

        return;
    }

    // Report an error if the message type is already registered
    if ((matrix_message_type_handlers != NULL) && g_hash_table_lookup_extended(matrix_message_type_handlers, message_type, NULL, NULL)) {
        g_set_error(error, MATRIX_ERROR, MATRIX_ERROR_ALREADY_EXISTS, "This message type is already registered.");

        return;
    }

    if (matrix_message_type_handlers == NULL) {
        matrix_message_type_handlers = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, g_type_class_unref);
    }

    key = g_strdup (message_type);
    value = g_type_class_ref(message_gtype);
    g_hash_table_insert(matrix_message_type_handlers, key, value);
}

/**
 * matrix_message_unregister_type:
 * @message_type: (transfer none): the message type to remove
 *
 * Unregister @message_type’s message handler.  It is safe to call this on a message type that is
 * not registered yet, as such calls will be safely ignored.
 */
void
matrix_message_unregister_type(const gchar *message_type)
{
    g_return_if_fail(message_type != NULL);

    if (matrix_message_type_handlers != NULL) {
        g_hash_table_remove (matrix_message_type_handlers, message_type);
    }
}
