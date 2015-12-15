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

#define API_ENDPOINT "/_matrix/client/api/v1/"

typedef struct _MatrixHTTPAPIPrivate {
    SoupSession *soup_session;
    SoupURI *uri;
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
            gchar *url;

            base_url = g_value_get_string(value);

            if (!g_str_is_ascii(base_url)) {
                g_warning("URL specified (%s) is not ASCII", base_url);

                return;
            }

            if (priv->uri) {
                soup_uri_free(priv->uri);
            }

            last_occurence = g_strrstr(base_url, API_ENDPOINT);

            /* Check if the provided URL already ends with the API endpoint */
            if ((g_strcmp0(last_occurence, API_ENDPOINT) == 0) ||
                (g_strcmp0(last_occurence, API_ENDPOINT"/") == 0)) {
                /* if so, just use it */
                url = g_strdup(base_url);
            } else {
                /* if not, add the API endpoint */

                gchar *url_tmp;

                /* If the provided URL already contains the API
                 * endpoint, but it’s not at the end, print a message,
                 * but still continue */
                if (last_occurence != NULL) {
                    g_info("Provided URL (%s) already contains the API endpoint but not at the end; appending anyway", base_url);
                }

                url_tmp = g_strdup(base_url);
                if (url_tmp[strlen(url_tmp) - 1] == '/') {
                    url_tmp[strlen(url_tmp) - 1] = 0;
                }

                url = g_strdup_printf("%s%s", url_tmp, API_ENDPOINT);
                g_free(url_tmp);
            }

            priv->uri = soup_uri_new(url);
            g_free(url);

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
            g_value_take_string(value, soup_uri_to_string(priv->uri, FALSE));

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
     * and matrix_http_api_register_account() calls set this
     * automatically.
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

    priv->uri = NULL;
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
                GError *err = NULL;

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

                /* Check if the response holds an error code */
                if ((node = json_object_get_member(
                             root_object, "errcode")) != NULL) {
                    const gchar *errcode = json_node_get_string(node);
                    gchar *message = NULL;
                    MatrixAPIError error_code = MATRIX_API_ERROR_UNKNOWN_ERROR;

                    /* Set the message as M_CODE: message */
                    if ((node = json_object_get_member(
                                 root_object, "error")) != NULL) {
                        message = g_strdup_printf("%s: %s",
                                                  errcode,
                                                  json_node_get_string(node));
                    } else {
                        /* If there is no message, issue a warning and
                         * set up the message as plain M_CODE */
                        message = g_strdup(errcode);
                    }

                    /* Set the actual GError code according to errcode */
                    if (strcmp("M_MISSING_TOKEN", errcode) == 0) {
                        error_code = MATRIX_API_ERROR_MISSING_TOKEN;
                    } else if (strcmp("M_FORBIDDEN", errcode) == 0) {
                        error_code = MATRIX_API_ERROR_FORBIDDEN;
                    } else if (strcmp("M_UNKNOWN", errcode) == 0) {
                        error_code = MATRIX_API_ERROR_UNKNOWN;
                    }

                    err = g_error_new_literal(MATRIX_API_ERROR,
                                              error_code,
                                              message);

                }

                /* Call the assigned function, if any */
                if (request->callback) {
                    request->callback(
                            MATRIX_API(api),
                            content,
                            request->callback_data,
                            err);
                    g_clear_error(&err);
                }
            } else {
                g_debug("Invalid response: %s", data);
            }
        } else {
            g_debug("Malformed response: %s", data);
        }
    }
}

/**
 * matrix_http_api_get_base_url:
 * @api: a #MatrixHTTPAPI implementation
 *
 * Get the base URL set for @api.
 *
 * Returns: (transfer full): the base URL set for @api
 */
const gchar *
matrix_http_api_get_base_url(MatrixHTTPAPI *api)
{
    MatrixHTTPAPIPrivate *priv = matrix_http_api_get_instance_private(api);

    return soup_uri_to_string(priv->uri, FALSE);
}

static void
matrix_http_api_send(MatrixAPI *api,
                     MatrixAPICallback callback,
                     gpointer user_data,
                     const gchar *method,
                     const gchar *path,
                     const JsonNode *content,
                     GError **error)
{
    if (!g_str_is_ascii(method)) {
        g_warning("Method must be ASCII encoded!");

        return;
    }

    if ((g_ascii_strcasecmp("GET", method) != 0)
        && (g_ascii_strcasecmp("POST", method) != 0)
        && (g_ascii_strcasecmp("PUT", method) != 0)
        && (g_ascii_strcasecmp("DELETE", method) != 0)) {
        g_warning("Invalid method name '%s'", method);

        return;
    }

}

void
matrix_http_api_login(MatrixAPI *api,
                      MatrixAPICallback callback,
                      gpointer user_data,
                      const gchar *type,
                      const JsonNode *content,
                      GError **error)
{
    JsonNode *body;
    JsonObject *root_object;

    body = json_node_copy((JsonNode *)content);
    root_object = json_node_get_object(body);
    json_object_set_string_member(root_object, "type", type);

    matrix_http_api_send(api,
                         callback, user_data,
                         "POST", "/login", body,
                         error);
}
