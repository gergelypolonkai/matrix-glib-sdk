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

#include "matrix-http-api.h"

#include <string.h>
#include <libsoup/soup.h>

/**
 * SECTION:matrix-http-api
 * @short_description: Low level API calls to communicate with a
 *                     Matrix.org server via HTTP
 * @title: MatrixHTTPAPI
 * @stability: Unstable
 * @include: matrix-glib/matrix.h
 *
 * This is a class for low level communication with a Matrix.org
 * server via HTTP.
 */

/**
 * MatrixHTTPAPI:
 *
 * The MatrixHTTPAPI object’s instance definition.
 */

/**
 * MatrixHTTPAPIClass:
 * @parent_class: the parent class structure (#GObjectClass)
 *
 * The MatrixHTTPAPI object’s class definition.
 */

#define API_ENDPOINT "/_matrix/client/api/v1"

typedef struct _MatrixHTTPAPIPrivate {
    SoupSession *soup_session;
    guint txn_id;
    gchar *url;
    gchar *token;
    gboolean validate_certificate;
} MatrixHTTPAPIPrivate;

enum {
    PROP_VALIDATE_CERTIFICATE = 1,
    PROP_BASE_URL,
    PROP_TOKEN,
    N_PROPERTIES
};

typedef struct {
    MatrixHTTPAPI *api;
    JsonNode *request_content;
    MatrixAPICallback callback;
    gpointer callback_data;
} MatrixHTTPAPIRequest;

GParamSpec *obj_properties[N_PROPERTIES] = {NULL,};

static void matrix_http_api_matrix_api_init(MatrixAPIInterface *iface);

G_DEFINE_TYPE_WITH_CODE(MatrixHTTPAPI, matrix_http_api, G_TYPE_OBJECT,
                        G_ADD_PRIVATE(MatrixHTTPAPI)
                        G_IMPLEMENT_INTERFACE(MATRIX_TYPE_API, matrix_http_api_matrix_api_init));

static void
matrix_http_api_matrix_api_init(MatrixAPIInterface *iface)
{
    iface->login = matrix_http_api_login;
    iface->register_account = matrix_http_api_register_account;
    iface->initial_sync = matrix_http_api_initial_sync;
    iface->create_room = matrix_http_api_create_room;
}

static void
matrix_http_api_finalize(GObject *gobject)
{
    g_signal_handlers_destroy(gobject);
    G_OBJECT_CLASS(matrix_http_api_parent_class)->finalize(gobject);
}

static void
matrix_http_api_set_property(GObject *gobject,
                        guint prop_id,
                        const GValue *value,
                        GParamSpec *pspec)
{
    MatrixHTTPAPI *api = MATRIX_HTTP_API(gobject);
    MatrixHTTPAPIPrivate *priv = matrix_http_api_get_instance_private(api);

    switch (prop_id) {
        case PROP_VALIDATE_CERTIFICATE:
            priv->validate_certificate = g_value_get_boolean(value);

            break;

        case PROP_BASE_URL:
        {
            const gchar *base_url;
            gchar *last_occurence;

            base_url = g_value_get_string(value);

            if (!g_str_is_ascii(base_url)) {
                g_warning("URL specified (%s) is not ASCII", base_url);

                return;
            }

            g_free(priv->url);

            last_occurence = g_strrstr(base_url, API_ENDPOINT);

            /* Check if the provided URL already ends with the API endpoint */
            if ((g_strcmp0(last_occurence, API_ENDPOINT) == 0) ||
                (g_strcmp0(last_occurence, API_ENDPOINT"/") == 0)) {
                /* if so, just use it */
                priv->url = g_strdup(base_url);
            } else {
                /* if not, add the API endpoint */

                gchar *url;

                /* If the provided URL already contains the API
                 * endpoint, but it’s not at the end, print a message,
                 * but still continue */
                if (last_occurence != NULL) {
                    g_info("Provided URL (%s) already contains the API endpoint but not at the end; appending anyway", base_url);
                }

                url = g_strdup(base_url);
                if (url[strlen(url) - 1] == '/') {
                    url[strlen(url) - 1] = 0;
                }

                priv->url = g_strdup_printf("%s%s", url, API_ENDPOINT);

                g_free(url);
            }

            break;
        }

        case PROP_TOKEN:
            if (priv->token) {
                g_free(priv->token);
            }

            priv->token = g_strdup(g_value_get_string(value));

            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, pspec);
    }
}

static void
matrix_http_api_get_property(GObject *gobject,
                        guint prop_id,
                        GValue *value,
                        GParamSpec *pspec)
{
    MatrixHTTPAPI *api = MATRIX_HTTP_API(gobject);
    MatrixHTTPAPIPrivate *priv = matrix_http_api_get_instance_private(api);

    switch (prop_id) {
        case PROP_VALIDATE_CERTIFICATE:
            g_value_set_boolean(value, priv->validate_certificate);

            break;

        case PROP_BASE_URL:
            g_value_set_string(value, priv->url);

            break;

        case PROP_TOKEN:
            g_value_set_string(value, priv->token);

            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, pspec);
    }
}

static void
matrix_http_api_class_init(MatrixHTTPAPIClass *klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS(klass);

    gobject_class->set_property = matrix_http_api_set_property;
    gobject_class->get_property = matrix_http_api_get_property;
    gobject_class->finalize = matrix_http_api_finalize;

    /**
     * MatrixHTTPAPI:validate-certificate:
     *
     * Set to %FALSE if you don’t want the SSL/TLS certificates to be
     * validated.
     */
    obj_properties[PROP_VALIDATE_CERTIFICATE] = g_param_spec_boolean(
            "validate-certificate",
            "Validate certificate",
            "TRUE if server certificates should be validated",
            TRUE,
            G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);

    /**
     * MatrixHTTPAPI:base-url:
     *
     * The base URL to use for communication with the Matrix.org
     * server. If the URL doesn’t end with the correct API endpoint
     * (/_matrix/client/api/v1), it gets appended automatically.
     */
    obj_properties[PROP_BASE_URL] = g_param_spec_string(
            "base-url", "Server's base URL",
            "Matrix.org home server to connect to.",
            NULL,
            G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);

    /**
     * MatrixHTTPAPI:token:
     *
     * The token to use for authorization. The matrix_http_api_login()
     * and matrix_http_api_register() calls set this automatically.
     */
    obj_properties[PROP_TOKEN] = g_param_spec_string(
            "token", "Authorization token",
            "The token to use for authorization",
            NULL,
            G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);

    g_object_class_install_properties(gobject_class,
                                      N_PROPERTIES,
                                      obj_properties);
}

static void
matrix_http_api_init(MatrixHTTPAPI *api)
{
    MatrixHTTPAPIPrivate *priv = matrix_http_api_get_instance_private(api);

    priv->txn_id = 0;
    priv->url = NULL;
    priv->token = NULL;
    priv->validate_certificate = TRUE;
    priv->soup_session = soup_session_new();
}

/**
 * matrix_http_api_new:
 * @base_url: the URL to use as the API endpoint
 * @token: (allow-none): an authorization token to use. If %NULL,
 *         requests that need authentication will fail
 *
 * Create a new #MatrixHTTPAPI object with the specified base URL, and
 * an optional authorization token
 */
MatrixHTTPAPI *
matrix_http_api_new(const gchar *base_url, const gchar *token)
{
    return g_object_new(MATRIX_TYPE_HTTP_API,
                        "base-url", base_url,
                        "token", token,
                        NULL);
}

/**
 * matrix_http_api_set_validate_certificate:
 * @api: a #MatrixHTTPAPI implementation
 * @validate_certificate: %TRUE if server certificates should be
 *                        validated
 *
 * Sets if server certificates should be validated.
 */
void
matrix_http_api_set_validate_certificate(MatrixHTTPAPI *api,
                                         gboolean validate_certificate)
{
    MatrixHTTPAPIPrivate *priv = matrix_http_api_get_instance_private(api);

   priv->validate_certificate = validate_certificate;
}

/**
 * matrix_http_api_get_validate_certificate:
 * @api: a #MatrixHTTPAPI implementation
 *
 * Gets the value set by matrix_http_api_set_validate_certificate()
 *
 * Returns: %TRUE if the server certificates should be validated
 */
gboolean
matrix_http_api_get_validate_certificate(MatrixHTTPAPI *api)
{
    MatrixHTTPAPIPrivate *priv = matrix_http_api_get_instance_private(api);

    return priv->validate_certificate;
}

static void
response_callback(SoupSession *session,
                  SoupMessage *msg,
                  MatrixHTTPAPIRequest *request)
{
    MatrixHTTPAPI *api = request->api;
    MatrixHTTPAPIPrivate *priv = matrix_http_api_get_instance_private(api);

    if (msg->status_code < SOUP_STATUS_CONTINUE) {
        g_info("Request failed: %d: %s", msg->status_code, msg->reason_phrase);
    } else {
        SoupBuffer *buffer;
        const guint8 *data;
        gsize datalen;
        JsonParser *parser;
        GError *err = NULL;
        JsonNode *content;

        buffer = soup_message_body_flatten(msg->response_body);
        soup_buffer_get_data(buffer, &data, &datalen);

        parser = json_parser_new();
        if (json_parser_load_from_data(parser,
                                       (const gchar *)data, datalen,
                                       &err)) {
            g_debug("Data: %s", data);
            content = json_parser_get_root(parser);

            if (JSON_NODE_HOLDS_OBJECT(content)) {
                JsonObject *root_object;
                JsonNode *node;

                root_object = json_node_get_object(content);

                /* Check if the response holds an access token; if it
                 * does, set it as our new token */
                if ((node = json_object_get_member(
                             root_object, "access_token")) != NULL) {
                    const gchar *access_token;

                    if ((access_token = json_node_get_string(node)) != NULL) {
                        g_debug("Access token: %s", access_token);
                        g_free(priv->token);
                        priv->token = g_strdup(access_token);
                    }
                }

                /* Check if the response holds a homeserver name */
                if ((node = json_object_get_member(
                             root_object, "home_server")) != NULL) {
                    const gchar *homeserver = json_node_get_string(node);

                    g_debug("Our home server calls itself %s", homeserver);
                }

               /* Call the assigned function, if any */
                if (request->callback) {
                    request->callback(
                            MATRIX_API(api),
                            content,
                            request->callback_data);
                }
            } else {
                g_debug("Invalid response: %s", data);
            }
        } else {
            g_debug("Malformed response: %s", data);
        }
    }
}

static void
matrix_http_api_send(MatrixHTTPAPI *api,
                     MatrixAPICallback callback,
                     gpointer user_data,
                     const gchar *method,
                     const gchar *path,
                     JsonNode *content,
                     GHashTable *params)
{
    MatrixHTTPAPIPrivate *priv = matrix_http_api_get_instance_private(api);
    SoupMessage *msg;
    JsonGenerator *generator;
    gsize datalen;
    gchar *data;
    gchar *url;
    MatrixHTTPAPIRequest *request;

    if (!g_str_is_ascii(method)) {
        g_warning("Method must be ASCII encoded!");

        return;
    }

    if ((g_ascii_strcasecmp("POST", method) != 0)
        && (g_ascii_strcasecmp("GET", method) != 0)
        && (g_ascii_strcasecmp("PUT", method) != 0)
        && (g_ascii_strcasecmp("DELETE", method) != 0)) {
        g_warning("Invalid method name '%s'", method);

        return;
    }

    generator = json_generator_new();
    json_generator_set_root(generator, content);
    data = json_generator_to_data(generator, &datalen);

    url = g_strdup_printf("%s%s", priv->url, path);
    g_debug("Sending %s to %s", method, url);

    msg = soup_message_new(method, url);
    g_free(url);
    soup_message_set_flags(msg, SOUP_MESSAGE_NO_REDIRECT);
    soup_message_set_request(msg,
                             "application/json",
                             SOUP_MEMORY_TAKE,
                             data,
                             datalen);
    request = g_new0(MatrixHTTPAPIRequest, 1);
    request->request_content = content;
    request->api = api;
    request->callback = callback;
    request->callback_data = user_data;

    g_object_ref(msg);
    soup_session_queue_message(priv->soup_session,
                            msg,
                            (SoupSessionCallback)response_callback, request);
}

static void
update_parameters(gchar *key, gchar *value, JsonBuilder *builder)
{
    JsonNode *node = json_node_new(JSON_NODE_VALUE);

    json_node_set_string(node, value);
    json_builder_set_member_name(builder, key);
    json_builder_add_value(builder, node);
}

static void
matrix_http_api_login_or_register(MatrixAPI *api,
                                  MatrixAPICallback callback,
                                  gpointer user_data,
                                  gchar *type,
                                  gchar *login_type,
                                  GHashTable *parameters)
{
    JsonBuilder *builder;
    JsonNode *content,
             *node;

    builder = json_builder_new();
    json_builder_begin_object(builder);

    node = json_node_new(JSON_NODE_VALUE);
    json_node_set_string(node, login_type);
    json_builder_set_member_name(builder, "type");
    json_builder_add_value(builder, node);

    g_hash_table_foreach(parameters, (GHFunc)update_parameters, builder);

    json_builder_end_object(builder);

    content = json_builder_get_root(builder);

    matrix_http_api_send(MATRIX_HTTP_API(api),
                         callback, user_data,
                         "POST", type,
                         content,
                         parameters);
}

/**
 * matrix_http_api_login:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async): the function to call when the request is
 *            finished
 * @user_data: user data to pass to the callback function
 * @login_type: the login type to use
 * @parameters: parameters to send with the login request
 *
 * Perform /login
 */
void
matrix_http_api_login(MatrixAPI *api,
                      MatrixAPICallback callback,
                      gpointer user_data,
                      gchar *login_type,
                      GHashTable *parameters)
{
    matrix_http_api_login_or_register(api,
                                      callback,
                                      user_data,
                                      "/login",
                                      login_type,
                                      parameters);
}

/**
 * matrix_http_api_gen_parameters:
 * @param1_name: name of the first parameter
 * @...: value of the first parameter, followed by name/value pairs,
 *       terminated by a NULL value as a parameter name
 *
 * Generate a GHashTable suitable as the parameters argument for
 * different API calls.
 *
 * Returns: (transfer full): a #GHashTable with the specified
 *          parameter table
 */
GHashTable *
matrix_http_api_gen_parameters(const gchar *param1_name, ...)
{
    GHashTable *table;
    va_list var_args;
    gchar *name;

    g_return_val_if_fail(param1_name != NULL, NULL);

    table = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, g_free);

    va_start(var_args, param1_name);

    name = (gchar *)param1_name;

    while (name) {
        gchar *value = va_arg(var_args, gchar *);

        g_hash_table_insert(table, name, value);

        name = va_arg(var_args, gchar *);
    }

    va_end(var_args);

    return table;
}

/**
 * matrix_http_api_get_base_url:
 * @api: a #MatrixHTTPAPI implementation
 *
 * Returns: (transfer none): the base URL set for @api
 */
const gchar *
matrix_http_api_get_base_url(MatrixHTTPAPI *api)
{
    MatrixHTTPAPIPrivate *priv = matrix_http_api_get_instance_private(api);

    return priv->url;
}

/**
 * matrix_http_api_register_account:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async): the function to call when the request is
 *            finished
 * @user_data: user data to pass to the callback function
 * @login_type: the login type to use
 * @parameters: parameters to send with the registration request
 *
 * Perform /register
 */
void
matrix_http_api_register_account(MatrixAPI *api,
                                 MatrixAPICallback callback,
                                 gpointer user_data,
                                 gchar *login_type,
                                 GHashTable *parameters)
{
    matrix_http_api_login_or_register(api,
                                      callback,
                                      user_data,
                                      "/register",
                                      login_type,
                                      parameters);
}

/**
 * matrix_http_api_initial_sync:
 * @api: a #MatrixHTTPAPI object
 * @callback: (scope async): the function to call when the request is
 *            finished
 * @user_data: user data to pass to the callback function
 * @limit: maximum number of messages to return for each room
 *
 * Perform /initialSync
 */
void
matrix_http_api_initial_sync(MatrixAPI *api,
                             MatrixAPICallback callback,
                             gpointer user_data,
                             guint limit)
{
    JsonBuilder *builder;
    JsonNode *content,
             *node;

    builder = json_builder_new();
    json_builder_begin_object(builder);

    node = json_node_new(JSON_NODE_VALUE);
    json_node_set_int(node, limit);
    json_builder_set_member_name(builder, "limit");
    json_builder_add_value(builder, node);

    json_builder_end_object(builder);

    content = json_builder_get_root(builder);

    matrix_http_api_send(MATRIX_HTTP_API(api),
                         callback, user_data,
                         "POST", "/initialSync",
                         content,
                         NULL);
}

/**
 * matrix_http_api_create_room:
 * @api: a #MatrixHTTPAPI object
 * @callback: (scope async): the function to call when the request is
 *            finished
 * @user_data: user data to pass to the callback function
 * @room_alias: an alias for the room
 * @is_public: set to %TRUE if the room should be publicly visible
 * @invitees: a list of user IDs to initially invite to the room
 *
 * Perform /createRoom
 */
void
matrix_http_api_create_room(MatrixAPI *api,
                            MatrixAPICallback callback,
                            gpointer user_data,
                            gchar *room_alias,
                            gboolean is_public,
                            GStrv invitees)
{
    JsonBuilder *builder;
    JsonNode *content,
             *node;

    builder = json_builder_new();
    json_builder_begin_object(builder);

    node = json_node_new(JSON_NODE_VALUE);
    json_node_set_string(node, is_public ? "public" : "private");
    json_builder_set_member_name(builder, "visibility");
    json_builder_add_value(builder, node);

    if (room_alias && *room_alias) {
        node = json_node_new(JSON_NODE_VALUE);
        json_node_set_string(node, room_alias);
        json_builder_set_member_name(builder, "room_alias_name");
        json_builder_add_value(builder, node);
    }

    if (invitees && *invitees) {
        JsonArray *user_array = json_array_new();
        gchar **user_id;

        for (user_id = invitees; *user_id; user_id++) {
            json_array_add_string_element(user_array, *user_id);
        }

        node = json_node_new(JSON_NODE_ARRAY);
        json_node_set_array(node, user_array);
        json_builder_set_member_name(builder, "invite");
        json_builder_add_value(builder, node);
    }

    json_builder_end_object(builder);

    content = json_builder_get_root(builder);

    matrix_http_api_send(MATRIX_HTTP_API(api),
                         callback, user_data,
                         "POST", "/createRoom",
                         content,
                         NULL);
}
