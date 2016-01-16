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

#include "config.h"
#include "matrix-http-api.h"
#include "matrix-enumtypes.h"
#include "utils.h"

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
#define MEDIA_ENDPOINT "/_matrix/media/v1/"

typedef struct _MatrixHTTPAPIPrivate {
    SoupSession *soup_session;
    SoupURI *uri;
    SoupURI *media_uri;
    gchar *token;
    gchar *refresh_token;
    gchar *user_id;
    gchar *homeserver;
    gboolean validate_certificate;
} MatrixHTTPAPIPrivate;

enum {
    PROP_VALIDATE_CERTIFICATE = 1,
    PROP_BASE_URL,
    PROP_TOKEN,
    PROP_REFRESH_TOKEN,
    PROP_USER_ID,
    PROP_HOMESERVER,
    N_PROPERTIES
};

typedef enum {
    CALL_API,
    CALL_MEDIA
} CallType;

typedef struct {
    MatrixHTTPAPI *api;
    JsonNode *request_content;
    GByteArray *raw_content;
    MatrixAPICallback callback;
    gpointer callback_data;
    gboolean accept_non_json;
    CallType call_type;
} MatrixHTTPAPIRequest;

GParamSpec *obj_properties[N_PROPERTIES] = {NULL,};

static void matrix_http_api_matrix_api_init(MatrixAPIInterface *iface);
static void i_set_token(MatrixAPI *api, const gchar *token);
static const gchar *i_get_token(MatrixAPI *api);
static void i_set_refresh_token(MatrixAPI *api, const gchar *refresh_token);
static const gchar *i_get_refresh_token(MatrixAPI *api);
static const gchar *i_get_user_id(MatrixAPI *api);
static const gchar *i_get_homeserver(MatrixAPI *api);

G_DEFINE_TYPE_WITH_CODE(MatrixHTTPAPI, matrix_http_api, G_TYPE_OBJECT,
                        G_ADD_PRIVATE(MatrixHTTPAPI)
                        G_IMPLEMENT_INTERFACE(MATRIX_TYPE_API,
                                              matrix_http_api_matrix_api_init));

static void
matrix_http_api_finalize(GObject *gobject)
{
    MatrixHTTPAPIPrivate *priv = matrix_http_api_get_instance_private(
            MATRIX_HTTP_API(gobject));

    g_free(priv->token);
    g_free(priv->refresh_token);
    g_free(priv->user_id);
    g_free(priv->homeserver);

    g_signal_handlers_destroy(gobject);
    G_OBJECT_CLASS(matrix_http_api_parent_class)->finalize(gobject);
}

static void
_set_url(SoupURI **uri, const gchar *base, const gchar *endpoint)
{
    gchar *url;
    SoupURI *new_uri;

    if (base[strlen(base) - 1] == '/') {
        url = g_strdup_printf("%s%s", base, endpoint + 1);
    } else {
        url = g_strdup_printf("%s%s", base, endpoint);
    }

    new_uri = soup_uri_new(url);

    if (new_uri && SOUP_URI_VALID_FOR_HTTP(new_uri)) {
        *uri = new_uri;
    } else {
        if (new_uri) {
            soup_uri_free(new_uri);
        }

        *uri = NULL;
    }
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
            matrix_http_api_set_validate_certificate(api, g_value_get_boolean(value));

            break;

        case PROP_BASE_URL:
        {
            const gchar *base_url;
            gchar *last_occurence;
            SoupURI *api_uri, *media_uri;

            base_url = g_value_get_string(value);

            if (!g_str_is_ascii(base_url)) {
                g_warning("URL specified (%s) is not ASCII", base_url);

                return;
            }

            /* Check if the provided URL already ends with the API endpoint */
            if ((last_occurence = g_strrstr(base_url, API_ENDPOINT)) != NULL) {
                g_warning("Provided URL (%s) already contains the API endpoint. Please use an URL without it!", base_url);

                return;
            }

            _set_url(&api_uri, base_url, API_ENDPOINT);
            _set_url(&media_uri, base_url, MEDIA_ENDPOINT);

            if (api_uri && media_uri) {
                gchar *api_url, *media_url;

                if (priv->uri) {
                    soup_uri_free(priv->uri);
                }

                if (priv->media_uri) {
                    soup_uri_free(priv->media_uri);
                }

                priv->uri = api_uri;
                priv->media_uri = media_uri;

                // Free all tokens and IDs, as they won’t be valid for
                // the new server
                g_free(priv->token);
                priv->token = NULL;
                g_free(priv->refresh_token);
                priv->refresh_token = NULL;
                g_free(priv->homeserver);
                priv->homeserver = NULL;
                g_free(priv->user_id);
                priv->user_id = NULL;

                api_url = soup_uri_to_string(api_uri, FALSE);
                media_url = soup_uri_to_string(media_uri, FALSE);

                g_debug("API URL: %s", api_url);
                g_debug("Media URL: %s", media_url);

                g_free(api_url);
                g_free(media_url);
            } else {
                if (api_uri) {
                    soup_uri_free(api_uri);
                }

                if (media_uri) {
                    soup_uri_free(media_uri);
                }

                g_warning("Invalid URL: %s", base_url);
            }

            break;
        }

        case PROP_TOKEN:
            i_set_token(MATRIX_API(api), g_value_get_string(value));

            break;

        case PROP_REFRESH_TOKEN:
            i_set_refresh_token(MATRIX_API(api),
                                g_value_get_string(value));

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
            g_value_set_string(value,
                               i_get_token(MATRIX_API(api)));

            break;

        case PROP_REFRESH_TOKEN:
            g_value_set_string(value, i_get_refresh_token(MATRIX_API(api)));

            break;

        case PROP_USER_ID:
            g_value_set_string(value, i_get_user_id(MATRIX_API(api)));

            break;

        case PROP_HOMESERVER:
            g_value_set_string(value, i_get_homeserver(MATRIX_API(api)));

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
    g_object_class_install_property(gobject_class,
                                    PROP_VALIDATE_CERTIFICATE,
                                    obj_properties[PROP_VALIDATE_CERTIFICATE]);

    /**
     * MatrixHTTPAPI:base-url:
     *
     * The base URL to use for communication with the Matrix.org
     * server. If the URL doesn’t end with the correct API endpoint
     * (/_matrix/client/api/v1), it gets appended automatically.
     *
     * Changing the base URL automatically clears all authorization
     * tokens.
     */
    obj_properties[PROP_BASE_URL] = g_param_spec_string(
            "base-url", "Server's base URL",
            "Matrix.org home server to connect to.",
            NULL,
            G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);
    g_object_class_install_property(gobject_class,
                                    PROP_BASE_URL,
                                    obj_properties[PROP_BASE_URL]);

    g_object_class_override_property(gobject_class, PROP_TOKEN, "token");
    g_object_class_override_property(gobject_class,
                                     PROP_REFRESH_TOKEN,
                                     "refresh-token");
    g_object_class_override_property(gobject_class, PROP_USER_ID, "user-id");
    g_object_class_override_property(gobject_class,
                                     PROP_HOMESERVER,
                                     "homeserver");
}

static void
matrix_http_api_init(MatrixHTTPAPI *api)
{
    MatrixHTTPAPIPrivate *priv = matrix_http_api_get_instance_private(api);

    priv->uri = NULL;
    priv->token = NULL;
    priv->refresh_token = NULL;
    priv->user_id = NULL;
    priv->homeserver = NULL;
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
 * an optional authorization token.
 *
 * Returns: (transfer full): a new #MatrixHTTPAPI object cast to
 *          #MatrixAPI
 */
MatrixAPI *
matrix_http_api_new(const gchar *base_url, const gchar *token)
{
    MatrixHTTPAPI *api = g_object_new(MATRIX_TYPE_HTTP_API,
                                      "base-url", base_url,
                                      "token", token,
                                      NULL);

    return MATRIX_API(api);
}

static void
i_set_token(MatrixAPI *api, const gchar *token)
{
    MatrixHTTPAPIPrivate *priv = matrix_http_api_get_instance_private(
            MATRIX_HTTP_API(api));

    g_free(priv->token);
    priv->token = g_strdup(token);
}

static const gchar *
i_get_token(MatrixAPI *api)
{
    MatrixHTTPAPIPrivate *priv = matrix_http_api_get_instance_private(
            MATRIX_HTTP_API(api));

    return priv->token;
}

static void
i_set_refresh_token(MatrixAPI *api, const gchar *refresh_token)
{
    MatrixHTTPAPIPrivate *priv = matrix_http_api_get_instance_private(
            MATRIX_HTTP_API(api));

    g_free(priv->refresh_token);
    priv->refresh_token = g_strdup(refresh_token);
}

static const gchar *
i_get_refresh_token(MatrixAPI *api)
{
    MatrixHTTPAPIPrivate *priv = matrix_http_api_get_instance_private(
            MATRIX_HTTP_API(api));

    return priv->refresh_token;
}

static const gchar *
i_get_user_id(MatrixAPI *api)
{
    MatrixHTTPAPIPrivate *priv = matrix_http_api_get_instance_private(
            MATRIX_HTTP_API(api));

    return priv->user_id;
}

static const gchar *
i_get_homeserver(MatrixAPI *api)
{
    MatrixHTTPAPIPrivate *priv = matrix_http_api_get_instance_private(
            MATRIX_HTTP_API(api));

    return priv->homeserver;
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
    g_object_set(priv->soup_session, "ssl-strict", validate_certificate, NULL);
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
_response_callback(SoupSession *session,
                   SoupMessage *msg,
                   MatrixHTTPAPIRequest *request)
{
    MatrixHTTPAPI *api = request->api;
    MatrixHTTPAPIPrivate *priv = matrix_http_api_get_instance_private(api);
    GError *err = NULL;
    JsonNode *content = NULL;
    GByteArray *raw_content = NULL;

    if (msg->status_code < SOUP_STATUS_CONTINUE) {
        err = g_error_new(MATRIX_API_ERROR,
                          MATRIX_API_ERROR_COMMUNICATION_ERROR,
                          "%s %d: %s",
                          (msg->status_code < 100) ? "Network error" : "HTTP",
                          msg->status_code, msg->reason_phrase);
    } else { // No error
        SoupBuffer *buffer;
        const guint8 *data;
        gsize datalen;
        JsonParser *parser;
        SoupURI *request_uri = soup_message_get_uri(msg);
        const gchar *request_url;

        switch (request->call_type) {
            case CALL_API:
                request_url = soup_uri_get_path(request_uri)
                    + strlen(API_ENDPOINT);

                break;

            case CALL_MEDIA:
                request_url = soup_uri_get_path(request_uri)
                    + strlen(MEDIA_ENDPOINT);

                break;
        }

        buffer = soup_message_body_flatten(msg->response_body);
        soup_buffer_get_data(buffer, &data, &datalen);

        parser = json_parser_new();
        if (json_parser_load_from_data(parser,
                                       (const gchar *)data, datalen,
                                       &err)) {
            g_debug("Data (%s): %s", request_url, data);
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
                        g_debug("Got new access token: %s", access_token);

                        i_set_token(MATRIX_API(api), access_token);
                    }
                }

                /* Check if the response holds a refresh token; if it
                 * does, set it as our new refresh token */
                if ((node = json_object_get_member(
                             root_object, "refresh_token")) != NULL) {
                    const gchar *refresh_token;

                    if ((refresh_token = json_node_get_string(node)) != NULL) {
                        g_debug("Got new refresh token: %s", refresh_token);

                        i_set_refresh_token(MATRIX_API(api), refresh_token);
                    }
                }

                /* Check if the response holds a homeserver name */
                if ((node = json_object_get_member(
                             root_object, "home_server")) != NULL) {
                    const gchar *homeserver = json_node_get_string(node);

                    g_free(priv->homeserver);
                    priv->user_id = g_strdup(homeserver);

                    g_debug("Our home server calls itself %s", homeserver);
                }

                /* Check if the response holds a user ID; if it does,
                 * set this as our user ID */
                if ((node = json_object_get_member(
                             root_object, "user_id")) != NULL) {
                    const gchar *user_id = json_node_get_string(node);

                    g_free(priv->user_id);
                    priv->user_id = g_strdup(user_id);

                    g_debug("We are reported to be logged in as %s", user_id);
                }

                { // Check if the response holds an error
                    JsonNode *errcode_node = json_object_get_member(root_object,
                                                                    "errcode");
                    JsonNode *error_node = json_object_get_member(root_object,
                                                                  "error");

                    if (errcode_node || error_node) {
                        gchar *message;
                        const gchar *errcode = NULL;
                        const gchar *error = NULL;
                        MatrixAPIError error_code = MATRIX_API_ERROR_UNKNOWN_ERROR;

                        if (errcode_node) {
                            GEnumClass *error_class;
                            GEnumValue *value;

                            errcode = json_node_get_string(errcode_node);

                            if (strncmp("M_", errcode, 2) == 0) {
                                gchar *matrix_error_code = g_strdup_printf(
                                        "MATRIX_API_ERROR_%s", errcode + 2);

                                error_class = g_type_class_ref(
                                        MATRIX_TYPE_API_ERROR);
                                value = g_enum_get_value_by_name(
                                        error_class, matrix_error_code);
                                g_free(matrix_error_code);
                                g_type_class_unref(error_class);

                                if (value) {
                                    error_code = value->value;
                                } else {
                                    g_info("An unknown error code '%s' was sent by the homeserver. You may want to report it to the %s developers", errcode, PACKAGE_NAME);
                                }
                            }
                        } else {
                            g_info("An error was sent by the homeserver, but no error code was specified. You may want to report this to the homeserver administrators.");
                            error_code = MATRIX_API_ERROR_UNSPECIFIED;
                        }

                        if (error_node) {
                            error = json_node_get_string(error_node);
                        }

                        if (errcode_node && error_node) {
                            message = g_strdup_printf("%s: %s", errcode, error);
                        } else if (errcode_node) {
                            message = g_strdup(errcode);
                        } else {
                            message = g_strdup_printf(
                                    "(No errcode given) %s", error);
                        }

                        err = g_error_new_literal(MATRIX_API_ERROR,
                                                  error_code,
                                                  message);
                    }
                }
            } else if (!JSON_NODE_HOLDS_ARRAY(content)) {
                // Not a JSON object, neither an array
                err = g_error_new(MATRIX_API_ERROR,
                                  MATRIX_API_ERROR_BAD_RESPONSE,
                                  "Bad response (not a JSON object)");
                g_debug("Bad response: %s", data);
            }
        } else { // Invalid JSON
            if (request->accept_non_json) {
                raw_content = g_byte_array_sized_new(datalen);
                g_byte_array_append(raw_content, data, datalen);
                g_debug("Binary data (%s): %" G_GSIZE_FORMAT " bytes",
                        request_url,
                        datalen);
            } else {
                err = g_error_new(MATRIX_API_ERROR,
                                  MATRIX_API_ERROR_BAD_RESPONSE,
                                  "Malformed response (invalid JSON)");
                g_debug("Malformed response (%s): %s", request_url, data);
            }
        }
    }

    /* Call the assigned function, if any */
    if (request->callback) {
        request->callback(
                MATRIX_API(api),
                soup_message_headers_get_content_type(
                        msg->response_headers,
                        NULL),
                content,
                raw_content,
                request->callback_data,
                err);
    }

    g_clear_error(&err);
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

#define create_query_params() (g_hash_table_new_full(g_str_hash,        \
                                                     (GEqualFunc)g_strcmp0, \
                                                     NULL,              \
                                                     g_free))


static void
_send(MatrixHTTPAPI *api,
      MatrixAPICallback callback,
      gpointer user_data,
      CallType call_type,
      const gchar *method,
      const gchar *path,
      GHashTable *params,
      const gchar *content_type,
      JsonNode *json_content,
      GByteArray *raw_content,
      gboolean accept_non_json,
      GError **error)
{
    MatrixHTTPAPIPrivate *priv = matrix_http_api_get_instance_private(api);
    SoupURI *request_path;
    SoupMessage *message;
    gchar *data, *url;
    gsize datalen;
    MatrixHTTPAPIRequest *request;

    if (!priv->uri) {
        g_set_error(error,
                    MATRIX_API_ERROR, MATRIX_API_ERROR_COMMUNICATION_ERROR,
                    "No valid base URL");

        return;
    }

    if (json_content && raw_content) {
        g_critical("Too many parameters for MatrixHTTPAPI._send. This is a bug");
    }

    if (raw_content && !content_type) {
        g_critical("Raw content needs content_type to be set. This is a bug");
    }

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

    switch (call_type) {
        case CALL_API:
            request_path = soup_uri_new_with_base(priv->uri, path);

            break;

        case CALL_MEDIA:
            request_path = soup_uri_new_with_base(priv->media_uri, path);

            break;
    }

    if (!params) {
        params = create_query_params();
    }

    if (priv->token) {
        g_debug("Adding access token '%s'", priv->token);

        g_hash_table_replace(params, "access_token", g_strdup(priv->token));
    }

    soup_uri_set_query_from_form(request_path, params);

    g_hash_table_unref(params);

    message = soup_message_new_from_uri(method, request_path);
    url = soup_uri_to_string(request_path, FALSE);
    soup_uri_free(request_path);

    if (json_content) {
        JsonGenerator *generator;

        generator = json_generator_new();
        json_generator_set_root(generator, (JsonNode *)json_content);
        data = json_generator_to_data(generator, &datalen);
    } else if (raw_content) {
        data = raw_content->data;
        datalen = raw_content->len;
    } else {
        data = g_strdup("{}");
        datalen = 2;
    }

    g_debug("Sending (%s %s): %s", method, url, data);

    soup_message_set_flags(message, SOUP_MESSAGE_NO_REDIRECT);
    soup_message_set_request(message,
                             (content_type == NULL)
                                 ? "application/json"
                                 : content_type,
                             raw_content ? SOUP_MEMORY_COPY : SOUP_MEMORY_TAKE,
                             data, datalen);
    g_object_ref(message);

    request = g_new0(MatrixHTTPAPIRequest, 1);
    request->request_content = json_content;
    request->raw_content = raw_content;
    request->api = api;
    request->callback = callback;
    request->callback_data = user_data;
    request->accept_non_json = accept_non_json;
    request->call_type = call_type;

    soup_session_queue_message(priv->soup_session,
                               message,
                               (SoupSessionCallback)_response_callback,
                               request);
}

static void
i_login(MatrixAPI *api,
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

    _send(MATRIX_HTTP_API(api),
          callback, user_data,
          CALL_API,
          "POST", "login", NULL, NULL, body, NULL,
          FALSE, error);
}

static void
add_state_event(MatrixAPIStateEvent *event, JsonBuilder *builder)
{
    JsonNode *node = matrix_api_state_event_get_json_node(event);

    json_builder_add_value(builder, node);
    json_node_free(node);
}

static void
add_string(gchar *str, JsonBuilder *builder)
{
    json_builder_add_string_value(builder, str);
}

static void
i_create_room(MatrixAPI *api,
              MatrixAPICallback callback,
              gpointer user_data,
              MatrixAPIRoomPreset preset,
              const gchar *room_name,
              const gchar *room_alias,
              const gchar *topic,
              MatrixAPIRoomVisibility visibility,
              JsonNode *creation_content,
              GList *initial_state,
              GList *invitees,
              GError **error)
{
    JsonNode *body;
    JsonObject *root_object;
    JsonBuilder *builder;

    builder = json_builder_new();
    json_builder_begin_object(builder);

    if (creation_content) {
        json_builder_set_member_name(builder, "creation_content");
        json_builder_add_value(builder, creation_content);
    }

    if (initial_state) {
        json_builder_set_member_name(builder, "initial_state");
        json_builder_begin_array(builder);
        g_list_foreach(initial_state, (GFunc)add_state_event, builder);
        json_builder_end_array(builder);
    }

    if (invitees) {
        json_builder_set_member_name(builder, "invite");
        json_builder_begin_array(builder);
        g_list_foreach(invitees, (GFunc)add_string, builder);
        json_builder_end_array(builder);
    }

    if (room_name) {
        json_builder_set_member_name(builder, "name");
        json_builder_add_string_value(builder, room_name);
    }

    if (preset != MATRIX_API_ROOM_PRESET_NONE) {
        gchar *preset_string = g_enum_to_string(
                MATRIX_TYPE_API_ROOM_PRESET, preset, TRUE);

       if (preset_string) {
            json_builder_set_member_name(builder, "preset");
            json_builder_add_string_value(builder, preset_string);
            g_free(preset_string);
        } else {
            g_debug("Invalid room preset type");
        }
    }

    if (room_alias) {
        json_builder_set_member_name(builder, "room_alias_name");
        json_builder_add_string_value(builder, room_alias);
    }

    if (topic) {
        json_builder_set_member_name(builder, "topic");
        json_builder_add_string_value(builder, topic);
    }

    if (visibility != MATRIX_API_ROOM_VISIBILITY_DEFAULT) {
        gchar *visibility_string = g_enum_to_string(
                MATRIX_TYPE_API_ROOM_VISIBILITY, visibility, TRUE);

        if (visibility_string) {
            json_builder_set_member_name(builder, "visibility");
            json_builder_add_string_value(builder, visibility_string);
            g_free(visibility_string);
        } else {
            g_debug("Invalid room visibility type");
        }
    }

    json_builder_end_object(builder);
    body = json_builder_get_root(builder);
    g_object_unref(builder);

    _send(MATRIX_HTTP_API(api),
          callback, user_data,
          CALL_API,
          "POST", "createRoom", NULL, NULL, body, NULL,
          FALSE, error);
}

static void
i_initial_sync(MatrixAPI *api,
               MatrixAPICallback callback,
               gpointer user_data,
               guint limit,
               gboolean archived,
               GError **err)
{
    GHashTable *params;

    params = create_query_params();

    if (limit != 0) {
        g_hash_table_replace(params, "limit", g_strdup_printf("%d", limit));
    }

    if (archived) {
        g_hash_table_replace(params, "archived", g_strdup("true"));
    }

    _send(MATRIX_HTTP_API(api),
          callback, user_data,
          CALL_API,
          "GET", "initialSync", params, NULL, NULL, NULL,
          FALSE, err);
}

static void
i_event_stream(MatrixAPI *api,
               MatrixAPICallback callback,
               gpointer user_data,
               const gchar *from_token,
               gulong timeout,
               GError **err)
{
    GHashTable *params;

    params = create_query_params();

    if (from_token) {
        g_hash_table_replace(params, "from", g_strdup(from_token));
    }

    if (timeout != 0) {
        g_hash_table_replace(params,
                             "timeout", g_strdup_printf("%lu", timeout));
    }

    _send(MATRIX_HTTP_API(api),
          callback, user_data,
          CALL_API,
          "GET", "events", params, NULL, NULL, NULL,
          FALSE, err);
}

static void
i_leave_room(MatrixAPI *api,
             MatrixAPICallback callback,
             gpointer user_data,
             const gchar *room_id,
             GError **error)
{
    gchar *encoded_room_id, *path;

    encoded_room_id = soup_uri_encode(room_id, NULL);
    path = g_strdup_printf("rooms/%s/leave", encoded_room_id);
    g_free(encoded_room_id);

    _send(MATRIX_HTTP_API(api),
          callback, user_data,
          CALL_API,
          "POST", path, NULL, NULL, NULL, NULL,
          FALSE, error);
    g_free(path);
}

static void
i_list_public_rooms(MatrixAPI *api,
                    MatrixAPICallback callback,
                    gpointer user_data,
                    GError **error)
{
    _send(MATRIX_HTTP_API(api),
          callback, user_data,
          CALL_API,
          "GET", "publicRooms", NULL, NULL, NULL, NULL,
          FALSE, error);
}

static void
i_join_room(MatrixAPI *api,
            MatrixAPICallback callback,
            gpointer user_data,
            const gchar *room_id,
            GError **error)
{
    gchar *encoded_room_id, *path;

    // TODO: a more thorough check should be used here
    if (*room_id != '!') {
        g_set_error(error,
                    MATRIX_API_ERROR, MATRIX_API_ERROR_INVALID_ROOM_ID,
                    "Invalid room ID");

        return;
    }

    encoded_room_id = soup_uri_encode(room_id, NULL);
    path = g_strdup_printf("rooms/%s/join", encoded_room_id);
    g_free(encoded_room_id);

    _send(MATRIX_HTTP_API(api),
          callback, user_data,
          CALL_API,
          "POST", path, NULL, NULL, NULL, NULL,
          FALSE, error);
    g_free(path);
}

static void
i_get_presence_list(MatrixAPI *api,
                    MatrixAPICallback callback,
                    gpointer user_data,
                    const gchar *user_id,
                    GError **error)
{
    gchar *encoded_user_id;
    gchar *path;

    encoded_user_id = soup_uri_encode(user_id, NULL);
    path = g_strdup_printf("presence/list/%s", encoded_user_id);
    g_free(encoded_user_id);

    _send(MATRIX_HTTP_API(api),
          callback, user_data,
          CALL_API,
          "GET", path, NULL, NULL, NULL, NULL,
          FALSE, error);
    g_free(path);
}

static void
i_get_user_presence(MatrixAPI *api,
                    MatrixAPICallback callback,
                    gpointer user_data,
                    const gchar *user_id,
                    GError **error)
{
    gchar *encoded_user_id;
    gchar *path;

    encoded_user_id = soup_uri_encode(user_id, NULL);
    path = g_strdup_printf("presence/%s/status", encoded_user_id);
    g_free(encoded_user_id);

    _send(MATRIX_HTTP_API(api),
          callback, user_data,
          CALL_API,
          "GET", path, NULL, NULL, NULL, NULL,
          FALSE, error);
    g_free(path);
}

static void
i_media_download(MatrixAPI *api,
                 MatrixAPICallback callback,
                 gpointer user_data,
                 const gchar *server_name,
                 const gchar *media_id,
                 GError **error)
{
    gchar *encoded_server_name, *encoded_media_id, *path;

    encoded_server_name = soup_uri_encode(server_name, NULL);
    encoded_media_id = soup_uri_encode(media_id, NULL);
    path = g_strdup_printf("download/%s/%s",
                           encoded_server_name,
                           encoded_media_id);
    g_free(encoded_server_name);
    g_free(encoded_media_id);

    _send(MATRIX_HTTP_API(api),
          callback, user_data,
          CALL_MEDIA,
          "GET", path, NULL, NULL, NULL, NULL,
          TRUE, error);
    g_free(path);
}

static void
i_media_thumbnail(MatrixAPI *api,
                  MatrixAPICallback callback,
                  gpointer user_data,
                  const gchar *server_name,
                  const gchar *media_id,
                  guint width,
                  guint height,
                  MatrixAPIResizeMethod method,
                  GError **error)
{
    gchar *encoded_server_name,
          *encoded_media_id,
          *path;
    GHashTable *params;

    encoded_server_name = soup_uri_encode(server_name, NULL);
    encoded_media_id = soup_uri_encode(media_id, NULL);
    path = g_strdup_printf("download/%s/%s",
                           encoded_server_name,
                           encoded_media_id);
    g_free(encoded_server_name);
    g_free(encoded_media_id);

    params = create_query_params();

    if (width > 0) {
        g_hash_table_replace(params, "width", g_strdup_printf("%u", width));
    }

    if (height > 0) {
        g_hash_table_replace(params, "height", g_strdup_printf("%u", height));
    }

    if (method != MATRIX_API_RESIZE_METHOD_DEFAULT) {
        switch (method) {
            case MATRIX_API_RESIZE_METHOD_CROP:
                g_hash_table_replace(params, "method", g_strdup("crop"));

                break;

            case MATRIX_API_RESIZE_METHOD_SCALE:
                g_hash_table_replace(params, "method", g_strdup("scale"));

                break;
        }
    }

     _send(MATRIX_HTTP_API(api),
           callback, user_data,
           CALL_MEDIA,
           "GET", path, params, NULL, NULL, NULL,
           TRUE, error);
    g_free(path);
}

static void
i_media_upload(MatrixAPI *api,
               MatrixAPICallback callback,
               gpointer user_data,
               const gchar *content_type,
               GByteArray *content,
               GError **error)
{
    _send(MATRIX_HTTP_API(api),
          callback, user_data,
          CALL_MEDIA,
          "POST", "upload", NULL, content_type, NULL, content,
          FALSE, error);
}

static void
i_update_presence_list(MatrixAPI *api,
                       MatrixAPICallback callback,
                       gpointer user_data,
                       const gchar *user_id,
                       GList *drop_ids,
                       GList *invite_ids,
                       GError **error)
{
    gchar *encoded_user_id;
    gchar *path;
    JsonBuilder *builder;
    JsonNode *body;

    encoded_user_id = soup_uri_encode(user_id, NULL);
    path = g_strdup_printf("presence/%s/status", encoded_user_id);
    g_free(encoded_user_id);

    builder = json_builder_new();
    json_builder_begin_object(builder);

    if (drop_ids) {
        json_builder_set_member_name(builder, "drop");
        json_builder_begin_array(builder);
        g_list_foreach(drop_ids, (GFunc)add_string, builder);
        json_builder_end_array(builder);
    }

    if (invite_ids) {
        json_builder_set_member_name(builder, "invide");
        json_builder_begin_array(builder);
        g_list_foreach(invite_ids, (GFunc)add_string, builder);
        json_builder_end_array(builder);
    }

    json_builder_end_object(builder);
    body = json_builder_get_root(builder);
    g_object_unref(builder);

    _send(MATRIX_HTTP_API(api),
          callback, user_data,
          CALL_API,
          "POST", path, NULL, NULL, body, NULL,
          FALSE, error);
    g_free(path);
}

static void
i_set_user_presence(MatrixAPI *api,
                    MatrixAPICallback callback,
                    gpointer user_data,
                    const gchar *user_id,
                    MatrixAPIPresence presence,
                    const gchar *status_message,
                    GError **error)
{
    gchar *encoded_user_id;
    gchar *path, *presence_string, *a;
    JsonBuilder *builder;
    JsonNode *body;
    GEnumClass *presence_class;
    GEnumValue *value;

    encoded_user_id = soup_uri_encode(user_id, NULL);
    path = g_strdup_printf("presence/%s/status", encoded_user_id);
    g_free(encoded_user_id);

    builder = json_builder_new();
    json_builder_begin_object(builder);

    json_builder_set_member_name(builder, "presence");
    presence_string = g_enum_to_string(MATRIX_TYPE_API_PRESENCE, presence, TRUE);
    json_builder_add_string_value(builder, presence_string);
    g_free(presence_string);

    if (status_message) {
        json_builder_set_member_name(builder, "status_msg");
        json_builder_add_string_value(builder, status_message);
    }

    json_builder_end_object(builder);
    body = json_builder_get_root(builder);
    g_object_unref(builder);

    _send(MATRIX_HTTP_API(api),
          callback, user_data,
          CALL_API,
          "POST", path, NULL, NULL, body, NULL,
          FALSE, error);
    g_free(path);
}

static void
i_update_pusher(MatrixAPI *api,
                MatrixAPICallback callback,
                gpointer user_data,
                MatrixAPIPusher *pusher,
                GError **error)
{
    JsonNode *pusher_node;

    if ((pusher_node = matrix_api_pusher_get_json_node(
                 pusher, error)) == NULL) {
        return;
    }

    _send(MATRIX_HTTP_API(api),
          callback, user_data,
          CALL_API,
          "POST", "pushers/set", NULL, NULL, pusher_node, NULL,
          FALSE, error);
}

static void
i_get_pushers(MatrixAPI *api,
              MatrixAPICallback callback,
              gpointer user_data,
              GError **error)
{
    _send(MATRIX_HTTP_API(api),
          callback, user_data,
          CALL_API,
          "GET", "pushrules/", NULL, NULL, NULL, NULL,
          FALSE, error);
}

static void
i_delete_pusher(MatrixAPI *api,
                MatrixAPICallback callback,
                gpointer user_data,
                const gchar *scope,
                MatrixAPIPusherKind kind,
                const gchar *rule_id,
                GError **error)
{
    gchar *encoded_scope, *encoded_rule_id, *kind_string, *path;

    encoded_scope = soup_uri_encode(scope, NULL);
    encoded_rule_id = soup_uri_encode(rule_id, NULL);
    kind_string = g_enum_to_string(MATRIX_TYPE_API_PUSHER_KIND, kind, TRUE);

    path = g_strdup_printf("pushrules/%s/%s/%s",
                           encoded_scope,
                           kind_string,
                           encoded_rule_id);

    g_free(encoded_scope);
    g_free(encoded_rule_id);
    g_free(kind_string);

    _send(MATRIX_HTTP_API(api),
          callback, user_data,
          CALL_API,
          "DELETE", path, NULL, NULL, NULL, NULL,
          FALSE, error);
    g_free(path);
}

static void
i_get_pusher(MatrixAPI *api,
             MatrixAPICallback callback,
             gpointer user_data,
             const gchar *scope,
             MatrixAPIPusherKind kind,
             const gchar *rule_id,
             GError **error)
{
    gchar *encoded_scope, *encoded_rule_id, *kind_string, *path;

    encoded_scope = soup_uri_encode(scope, NULL);
    encoded_rule_id = soup_uri_encode(rule_id, NULL);
    kind_string = g_enum_to_string(MATRIX_TYPE_API_PUSHER_KIND, kind, TRUE);

    path = g_strdup_printf("pushrules/%s/%s/%s",
                           encoded_scope,
                           kind_string,
                           encoded_rule_id);

    g_free(encoded_scope);
    g_free(encoded_rule_id);
    g_free(kind_string);

    _send(MATRIX_HTTP_API(api),
          callback, user_data,
          CALL_API,
          "GET", path, NULL, NULL, NULL, NULL,
          FALSE, error);
    g_free(path);
}

static void
add_condition_kind_object(MatrixAPIPusherConditionKind kind,
                          JsonBuilder *builder)
{
    gchar *kind_string = g_enum_to_string(
            MATRIX_TYPE_API_PUSHER_CONDITION_KIND, kind, TRUE);

    if (!kind_string) {
        g_warning("Invalid condition kind");

        return;
    }

    json_builder_begin_object(builder);
    json_builder_set_member_name(builder, "kind");
    json_builder_add_string_value(builder, kind_string);
    json_builder_end_object(builder);

    g_free(kind_string);
}

static void i_add_pusher(MatrixAPI *api,
                         MatrixAPICallback callback,
                         gpointer user_data,
                         const gchar *scope,
                         MatrixAPIPusherKind kind,
                         const gchar *rule_id,
                         const gchar *before,
                         const gchar *after,
                         GList *actions,
                         GList *conditions,
                         GError **error)
{
    gchar *encoded_scope, *encoded_rule_id, *kind_string, *path;
    GHashTable *params;
    JsonBuilder *builder;
    JsonNode *body;

    encoded_scope = soup_uri_encode(scope, NULL);
    encoded_rule_id = soup_uri_encode(rule_id, NULL);
    kind_string = g_enum_to_string(MATRIX_TYPE_API_PUSHER_KIND, kind, TRUE);

    path = g_strdup_printf("pushrules/%s/%s/%s",
                           encoded_scope,
                           kind_string,
                           encoded_rule_id);

    g_free(encoded_scope);
    g_free(encoded_rule_id);
    g_free(kind_string);

    params = create_query_params();

    if (before) {
        g_hash_table_replace(params, "before", g_strdup(before));
    }

    if (after) {
        g_hash_table_replace(params, "after", g_strdup(after));
    }

    builder = json_builder_new();
    json_builder_begin_object(builder);

    json_builder_set_member_name(builder, "actions");
    json_builder_begin_array(builder);
    g_list_foreach(actions, (GFunc)add_string, builder);
    json_builder_end_array(builder);

    if (conditions) {
        json_builder_set_member_name(builder, "conditions");
        json_builder_begin_array(builder);
        g_list_foreach(conditions, (GFunc)add_condition_kind_object, builder);
        json_builder_end_array(builder);
    }

    json_builder_end_object(builder);
    body = json_builder_get_root(builder);
    g_object_unref(builder);

    _send(MATRIX_HTTP_API(api),
          callback, user_data,
          CALL_API,
          "GET", path, params, NULL, body, NULL,
          FALSE, error);
    g_free(path);
}

static void
i_toggle_pusher(MatrixAPI *api,
                MatrixAPICallback callback,
                gpointer user_data,
                const gchar *scope,
                MatrixAPIPusherKind kind,
                const gchar *rule_id,
                gboolean enabled,
                GError **error)
{
    gchar *encoded_scope, *encoded_rule_id, *kind_string, *path;
    JsonBuilder *builder;
    JsonNode *body;

    encoded_scope = soup_uri_encode(scope, NULL);
    encoded_rule_id = soup_uri_encode(rule_id, NULL);
    kind_string = g_enum_to_string(MATRIX_TYPE_API_PUSHER_KIND, kind, TRUE);

    path = g_strdup_printf("pushrules/%s/%s/%s",
                           encoded_scope,
                           kind_string,
                           encoded_rule_id);

    g_free(encoded_scope);
    g_free(encoded_rule_id);
    g_free(kind_string);

    builder = json_builder_new();
    json_builder_begin_object(builder);

    json_builder_set_member_name(builder, "enabled");
    json_builder_add_boolean_value(builder, enabled);

    json_builder_end_object(builder);
    body = json_builder_get_root(builder);
    g_object_unref(builder);

    _send(MATRIX_HTTP_API(api),
          callback, user_data,
          CALL_API,
          "GET", path, NULL, NULL, body, NULL,
          FALSE, error);
    g_free(path);
}

static void
i_delete_room_alias(MatrixAPI *api,
                    MatrixAPICallback callback,
                    gpointer user_data,
                    const gchar *room_alias,
                    GError **error)
{
    gchar *encoded_room_alias, *path;

    encoded_room_alias = soup_uri_encode(room_alias, NULL);
    path = g_strdup_printf("room/%s", encoded_room_alias);
    g_free(encoded_room_alias);

    _send(MATRIX_HTTP_API(api),
          callback, user_data,
          CALL_API,
          "DELETE", path, NULL, NULL, NULL, NULL,
          FALSE, error);
    g_free(path);
}

static void
i_get_room_id(MatrixAPI *api,
              MatrixAPICallback callback,
              gpointer user_data,
              const gchar *room_alias,
              GError **error)
{
    gchar *encoded_room_alias, *path;

    encoded_room_alias = soup_uri_encode(room_alias, NULL);
    path = g_strdup_printf("room/%s", encoded_room_alias);
    g_free(encoded_room_alias);

    _send(MATRIX_HTTP_API(api),
          callback, user_data,
          CALL_API,
          "GET", path, NULL, NULL, NULL, NULL,
          FALSE, error);
    g_free(path);
}

static void
i_create_room_alias(MatrixAPI *api,
                    MatrixAPICallback callback,
                    gpointer user_data,
                    const gchar *room_id,
                    const gchar *room_alias,
                    GError **error)
{
    gchar *encoded_room_alias, *path;
    JsonBuilder *builder;
    JsonNode *body;

    encoded_room_alias = soup_uri_encode(room_alias, NULL);
    path = g_strdup_printf("room/%s", encoded_room_alias);
    g_free(encoded_room_alias);

    builder = json_builder_new();
    json_builder_begin_object(builder);

    json_builder_set_member_name(builder, "room_id");
    json_builder_add_string_value(builder, room_id);

    json_builder_end_object(builder);
    body = json_builder_get_root(builder);
    g_object_unref(builder);

    _send(MATRIX_HTTP_API(api),
          callback, user_data,
          CALL_API,
          "PUT", path, NULL, NULL, body, NULL,
          FALSE, error);
    g_free(path);
}

static void
i_ban_user(MatrixAPI *api,
           MatrixAPICallback callback,
           gpointer user_data,
           const gchar *room_id,
           const gchar *user_id,
           const gchar *reason,
           GError **error)
{
    gchar *encoded_room_id, *path;
    JsonBuilder *builder;
    JsonNode *body;

    encoded_room_id = soup_uri_encode(room_id, NULL);
    path = g_strdup_printf("rooms/%s/ban", encoded_room_id);
    g_free(encoded_room_id);

    builder = json_builder_new();
    json_builder_begin_object(builder);

    json_builder_set_member_name(builder, "user_id");
    json_builder_add_string_value(builder, user_id);

    if (reason) {
        json_builder_set_member_name(builder, "reason");
        json_builder_add_string_value(builder, reason);
    }

    json_builder_end_object(builder);
    body = json_builder_get_root(builder);
    g_object_unref(builder);

    _send(MATRIX_HTTP_API(api),
          callback, user_data,
          CALL_API,
          "POST", path, NULL, NULL, body, NULL,
          FALSE, error);
    g_free(path);
}

static void
i_forget_room(MatrixAPI *api,
              MatrixAPICallback callback,
              gpointer user_data,
              const gchar *room_id,
              GError **error)
{
    gchar *encoded_room_id, *path;

    encoded_room_id = soup_uri_encode(room_id, NULL);
    path = g_strdup_printf("rooms/%s/forget", encoded_room_id);
    g_free(encoded_room_id);

    _send(MATRIX_HTTP_API(api),
          callback, user_data,
          CALL_API,
          "POST", path, NULL, NULL, NULL, NULL,
          FALSE, error);
    g_free(path);
}

static void
i_invite_user_3rdparty(MatrixAPI *api,
                       MatrixAPICallback callback,
                       gpointer user_data,
                       const gchar *room_id,
                       const gchar *address,
                       const gchar *medium,
                       const gchar *id_server,
                       GError **error)
{
    gchar *encoded_room_id, *path;
    JsonBuilder *builder;
    JsonNode *body;

    encoded_room_id = soup_uri_encode(room_id, NULL);
    path = g_strdup_printf("rooms/%s/invite", encoded_room_id);
    g_free(encoded_room_id);

    builder = json_builder_new();
    json_builder_begin_object(builder);

    json_builder_set_member_name(builder, "id_server");
    json_builder_add_string_value(builder, id_server);

    json_builder_set_member_name(builder, "medium");
    json_builder_add_string_value(builder, medium);

    json_builder_set_member_name(builder, "address");
    json_builder_add_string_value(builder, address);

    json_builder_end_object(builder);
    body = json_builder_get_root(builder);
    g_object_unref(builder);

    _send(MATRIX_HTTP_API(api),
          callback, user_data,
          CALL_API,
          "POST", path, NULL, NULL, body, NULL,
          FALSE, error);
    g_free(path);
}

static void
i_invite_user(MatrixAPI *api,
              MatrixAPICallback callback,
              gpointer user_data,
              const gchar *room_id,
              const gchar *user_id,
              GError **error)
{
    gchar *encoded_room_id, *path;
    JsonBuilder *builder;
    JsonNode *body;

    encoded_room_id = soup_uri_encode(encoded_room_id, NULL);
    path = g_strdup_printf("rooms/%s/invite", encoded_room_id);
    g_free(encoded_room_id);

    builder = json_builder_new();
    json_builder_begin_object(builder);

    json_builder_set_member_name(builder, "user_id");
    json_builder_add_string_value(builder, user_id);

    json_builder_end_object(builder);
    body = json_builder_get_root(builder);
    g_object_unref(builder);

    _send(MATRIX_HTTP_API(api),
          callback, user_data,
          CALL_API,
          "POST", path, NULL, NULL, body, NULL,
          FALSE, error);
    g_free(path);
}

static void
i_get_event(MatrixAPI *api,
            MatrixAPICallback callback,
            gpointer user_data,
            const gchar *event_id,
            GError **error)
{
    gchar *encoded_event_id, *path;

    encoded_event_id = soup_uri_encode(event_id, NULL);
    path = g_strdup_printf("events/%s", encoded_event_id);
    g_free(encoded_event_id);

    _send(MATRIX_HTTP_API(api),
          callback, user_data,
          CALL_API,
          "GET", path, NULL, NULL, NULL, NULL,
          FALSE, error);
    g_free(path);
}

static void
i_get_event_context(MatrixAPI *api,
                    MatrixAPICallback callback,
                    gpointer user_data,
                    const gchar *room_id,
                    const gchar *event_id,
                    guint limit,
                    GError **error)
{
    gchar *encoded_room_id, *encoded_event_id, *path;
    GHashTable *params = NULL;

    encoded_room_id = soup_uri_encode(room_id, NULL);
    encoded_event_id = soup_uri_encode(event_id, NULL);
    path = g_strdup_printf("rooms/%s/context/%s",
                           encoded_room_id, encoded_event_id);
    g_free(encoded_room_id);
    g_free(encoded_event_id);

    if (limit != 0) {
        params = create_query_params();

        g_hash_table_replace(params, "limit", g_strdup_printf("%u", limit));
    }

    _send(MATRIX_HTTP_API(api),
          callback, user_data,
          CALL_API,
          "GET", path, params, NULL, NULL, NULL,
          FALSE, error);
    g_free(params);
}

static void
i_initial_sync_room(MatrixAPI *api,
                    MatrixAPICallback callback,
                    gpointer user_data,
                    const gchar *room_id,
                    GError **error)
{
    gchar *encoded_room_id, *path;

    encoded_room_id = soup_uri_encode(room_id, NULL);
    path = g_strdup_printf("rooms/%s/initialSync", encoded_room_id);
    g_free(encoded_room_id);

    _send(MATRIX_HTTP_API(api),
          callback, user_data,
          CALL_API,
          "GET", path, NULL, NULL, NULL, NULL,
          FALSE, error);
    g_free(path);
}

static void
i_list_room_members(MatrixAPI *api,
                    MatrixAPICallback callback,
                    gpointer user_data,
                    const gchar *room_id,
                    GError **error)
{
    gchar *encoded_room_id, *path;

    encoded_room_id = soup_uri_encode(room_id, NULL);
    path = g_strdup_printf("rooms/%s/members", encoded_room_id);
    g_free(encoded_room_id);

    _send(MATRIX_HTTP_API(api),
          callback, user_data,
          CALL_API,
          "GET", path, NULL, NULL, NULL, NULL,
          FALSE, error);
    g_free(path);
}

static void
i_list_room_messages(MatrixAPI *api,
                     MatrixAPICallback callback,
                     gpointer user_data,
                     const gchar *room_id,
                     const gchar *from_token,
                     MatrixAPIEventDirection direction,
                     guint limit,
                     GError **error)
{
    gchar *encoded_room_id, *path;
    GHashTable *params;

    encoded_room_id = soup_uri_encode(room_id, NULL);
    path = g_strdup_printf("rooms/%s/messages", encoded_room_id);
    g_free(encoded_room_id);

    params  = create_query_params();

    g_hash_table_replace(params, "from", g_strdup(from_token));

    switch (direction) {
        case MATRIX_API_EVENT_DIRECTION_BACKWARD:
            g_hash_table_replace(params, "dir", g_strdup("b"));

            break;

        case MATRIX_API_EVENT_DIRECTION_FORWARD:
            g_hash_table_replace(params, "dir", g_strdup("f"));

            break;
    }

    if (limit != 0) {
        g_hash_table_replace(params, "limit", g_strdup_printf("%u", limit));
    }

    _send(MATRIX_HTTP_API(api),
          callback, user_data,
          CALL_API,
          "GET", path, params, NULL, NULL, NULL,
          FALSE, error);
    g_free(path);
}

static void
i_send_event_receipt(MatrixAPI *api,
                     MatrixAPICallback callback,
                     gpointer user_data,
                     const gchar *room_id,
                     MatrixAPIReceiptType receipt_type,
                     const gchar *event_id,
                     JsonNode *receipt,
                     GError **error)
{
    gchar *encoded_room_id, *receipt_type_string, *encoded_event_id, *path;

    encoded_room_id = soup_uri_encode(room_id, NULL);
    encoded_event_id = soup_uri_encode(event_id, NULL);
    receipt_type_string = g_enum_to_string(MATRIX_TYPE_API_RECEIPT_TYPE,
                                         receipt_type,
                                         TRUE);
    path = g_strdup_printf("rooms/%s/receipt/%s/%s",
                           encoded_room_id,
                           receipt_type_string,
                           encoded_event_id);
    g_free(encoded_room_id);
    g_free(encoded_event_id);
    g_free(receipt_type_string);

    _send(MATRIX_HTTP_API(api),
          callback, user_data,
          CALL_API,
          "POST", path, NULL, NULL, receipt, NULL,
          FALSE, error);
    g_free(path);
}

static void
i_redact_event(MatrixAPI *api,
               MatrixAPICallback callback,
               gpointer user_data,
               const gchar *room_id,
               const gchar *event_id,
               const gchar *txn_id,
               const gchar *reason,
               GError **error)
{
    gchar *encoded_room_id, *encoded_event_id, *encoded_txn_id, *path;
    JsonBuilder *builder;
    JsonNode *body = NULL;

    encoded_room_id = soup_uri_encode(room_id, NULL);
    encoded_event_id = soup_uri_encode(event_id, NULL);
    encoded_txn_id = soup_uri_encode(txn_id, NULL);
    path = g_strdup_printf("rooms/%s/redact/%s/%s",
                           encoded_room_id,
                           encoded_event_id,
                           encoded_txn_id);
    g_free(encoded_room_id);
    g_free(encoded_event_id);
    g_free(encoded_txn_id);

    if (reason) {
        builder = json_builder_new();
        json_builder_begin_object(builder);

        json_builder_set_member_name(builder, "reason");
        json_builder_add_string_value(builder, reason);

        json_builder_end_object(builder);
        body = json_builder_get_root(builder);
        g_object_unref(builder);
    }

    _send(MATRIX_HTTP_API(api),
          callback, user_data,
          CALL_API,
          "PUT", path, NULL, NULL, body, NULL,
          FALSE, error);
    g_free(path);
}

static void
i_send_message_event(MatrixAPI *api,
                     MatrixAPICallback callback,
                     gpointer user_data,
                     const gchar *room_id,
                     const gchar *event_type,
                     const gchar *txn_id,
                     JsonNode *content,
                     GError **error)
{
    gchar *encoded_room_id, *encoded_event_type, *encoded_txn_id, *path;

    encoded_room_id = soup_uri_encode(room_id, NULL);
    encoded_event_type = soup_uri_encode(event_type, NULL);
    encoded_txn_id = soup_uri_encode(txn_id, NULL);
    path = g_strdup_printf("rooms/%s/send/%s/%s",
                           encoded_room_id,
                           encoded_event_type,
                           encoded_txn_id);
    g_free(encoded_room_id);
    g_free(encoded_event_type);
    g_free(encoded_txn_id);

    _send(MATRIX_HTTP_API(api),
          callback, user_data,
          CALL_API,
          "PUT", path, NULL, NULL, content, NULL,
          FALSE, error);
    g_free(path);
}

static void
i_get_room_state(MatrixAPI *api,
                 MatrixAPICallback callback,
                 gpointer user_data,
                 const gchar *room_id,
                 const gchar *event_type,
                 const gchar *state_key,
                 GError **error)
{
    gchar *encoded_room_id, *path, *encoded_event_type, *encoded_state_key;

    encoded_room_id = soup_uri_encode(room_id, NULL);

    if (event_type) {
        encoded_event_type = soup_uri_encode(event_type, NULL);

        if (state_key) {
            encoded_state_key = soup_uri_encode(state_key, NULL);
            path = g_strdup_printf("rooms/%s/state/%s/%s",
                                   encoded_room_id,
                                   encoded_event_type,
                                   encoded_state_key);
            g_free(encoded_state_key);
        } else {
            path = g_strdup_printf("rooms/%s/state/%s",
                                   encoded_room_id, encoded_event_type);
        }

        g_free(encoded_event_type);
    } else {
        path = g_strdup_printf("rooms/%s/state", encoded_room_id);
    }

    g_free(encoded_room_id);

    _send(MATRIX_HTTP_API(api),
          callback, user_data,
          CALL_API,
          "GET", path, NULL, NULL, NULL, NULL,
          FALSE, error);
    g_free(path);
}

static void
i_send_room_event(MatrixAPI *api,
                  MatrixAPICallback callback,
                  gpointer user_data,
                  const gchar *room_id,
                  const gchar *event_type,
                  const gchar *state_key,
                  JsonNode *content,
                  GError **error)
{
    gchar *encoded_room_id, *path, *encoded_event_type, *encoded_state_key;

    encoded_room_id = soup_uri_encode(room_id, NULL);

    encoded_event_type = soup_uri_encode(event_type, NULL);

    if (state_key) {
        encoded_state_key = soup_uri_encode(state_key, NULL);
        path = g_strdup_printf("rooms/%s/state/%s/%s",
                               encoded_room_id,
                               encoded_event_type,
                               encoded_state_key);
        g_free(encoded_state_key);
    } else {
        path = g_strdup_printf("rooms/%s/state/%s",
                               encoded_room_id, encoded_event_type);
    }

    g_free(encoded_event_type);
    g_free(encoded_room_id);

    _send(MATRIX_HTTP_API(api),
          callback, user_data,
          CALL_API,
          "PUT", path, NULL, NULL, content, NULL,
          FALSE, error);
    g_free(path);
}

static void
i_notify_room_typing(MatrixAPI *api,
                     MatrixAPICallback callback,
                     gpointer user_data,
                     const gchar *user_id,
                     const gchar *room_id,
                     guint timeout,
                     gboolean typing,
                     GError **error)
{
    gchar *encoded_room_id, *encoded_user_id, *path;
    JsonBuilder *builder;
    JsonNode *body;

    encoded_room_id = soup_uri_encode(room_id, NULL);
    encoded_user_id = soup_uri_encode(user_id, NULL);
    path = g_strdup_printf("rooms/%s/typing/%s",
                           encoded_room_id, encoded_user_id);
    g_free(encoded_room_id);
    g_free(encoded_user_id);

    builder = json_builder_new();
    json_builder_begin_object(builder);

    if (timeout != 0) {
        json_builder_set_member_name(builder, "timeout");
        json_builder_add_int_value(builder, timeout);
    }

    json_builder_set_member_name(builder, "typing");
    json_builder_add_boolean_value(builder, typing);

    json_builder_end_object(builder);
    body = json_builder_get_root(builder);
    g_object_unref(builder);

    _send(MATRIX_HTTP_API(api),
          callback, user_data,
          CALL_API,
          "PUT", path, NULL, NULL, body, NULL,
          FALSE, error);
    g_free(path);
}

static void
i_sync(MatrixAPI *api,
       MatrixAPICallback callback,
       gpointer user_data,
       const gchar *filter_id,
       const MatrixAPIFilter *filter,
       const gchar *since,
       gboolean full_state,
       gboolean set_presence,
       gulong timeout,
       GError **error)
{
    GHashTable *params;

    params = create_query_params();

    if (filter_id && filter) {
        g_set_error(error,
                    MATRIX_API_ERROR, MATRIX_API_ERROR_BAD_REQUEST,
                    "Cannot set both filter_id and filter");

        return;
    }

    if (filter_id) {
        g_hash_table_replace(params, "filter", g_strdup(filter_id));
    }

    if (filter) {
        g_hash_table_replace(params,
                             "filter",
                             matrix_api_filter_get_json_data(
                                     (MatrixAPIFilter *)filter,
                                     NULL));
    }

    if (since) {
        g_hash_table_replace(params, "since", g_strdup(since));
    }

    g_hash_table_replace(params,
                         "full_state",
                         g_strdup((full_state) ? "true" : "false"));

    if (!set_presence) {
        g_hash_table_replace(params, "set_presence", g_strdup("offline"));
    }

    if (timeout != 0) {
        g_hash_table_replace(params,
                             "timeout",
                             g_strdup_printf("%lu", timeout));
    }

    _send(MATRIX_HTTP_API(api),
          callback, user_data,
          CALL_API,
          "GET", "sync", params, NULL, NULL, NULL,
          FALSE, error);
}

static void
i_create_filter(MatrixAPI *api,
                MatrixAPICallback callback,
                gpointer user_data,
                const gchar *user_id,
                MatrixAPIFilter *filter,
                GError **error)
{
    gchar *encoded_user_id, *path;
    JsonNode *filter_node = matrix_api_filter_get_json_node(filter);

    encoded_user_id = soup_uri_encode(user_id, NULL);
    path = g_strdup_printf("user/%s/filter", encoded_user_id);
    g_free(encoded_user_id);

    _send(MATRIX_HTTP_API(api),
          callback, user_data,
          CALL_API,
          "POST", path, NULL, NULL, filter_node, NULL,
          FALSE, error);
    g_free(path);
}

static void
i_download_filter(MatrixAPI *api,
                  MatrixAPICallback callback,
                  gpointer user_data,
                  const gchar *user_id,
                  const gchar *filter_id,
                  GError **error)
{
    gchar *encoded_user_id, *encoded_filter_id, *path;

    encoded_user_id = soup_uri_encode(user_id, NULL);
    encoded_filter_id = soup_uri_encode(filter_id, NULL);
    path = g_strdup_printf("user/%s/filter/%s",
                           encoded_user_id, encoded_filter_id);
    g_free(encoded_user_id);
    g_free(encoded_filter_id);

    _send(MATRIX_HTTP_API(api),
          callback, user_data,
          CALL_API,
          "GET", path, NULL, NULL, NULL, NULL,
          FALSE, error);
    g_free(path);
}

static void
i_whois(MatrixAPI *api,
        MatrixAPICallback callback,
        gpointer user_data,
        const gchar *user_id,
        GError **error)
{
    gchar *encoded_user_id, *path;

    encoded_user_id = soup_uri_encode(user_id, NULL);
    path = g_strdup_printf("admin/whois/%s", encoded_user_id);
    g_free(encoded_user_id);

    _send(MATRIX_HTTP_API(api),
          callback, user_data,
          CALL_API,
          "GET", path, NULL, NULL, NULL, NULL,
          FALSE, error);
    g_free(path);
}

static void
i_token_refresh(MatrixAPI *api,
                MatrixAPICallback callback,
                gpointer user_data,
                const gchar *refresh_token,
                GError **error)
{
    MatrixHTTPAPIPrivate *priv = matrix_http_api_get_instance_private(
            MATRIX_HTTP_API(api));
    JsonBuilder *builder;
    JsonNode *body;

    if (!refresh_token && !priv->token) {
        g_set_error(error,
                    MATRIX_API_ERROR, MATRIX_API_ERROR_MISSING_TOKEN,
                    "No token available");

        return;
    }

    builder = json_builder_new();
    json_builder_begin_object(builder);

    json_builder_set_member_name(builder, "refresh_token");

    if (!refresh_token) {
        json_builder_add_string_value(builder, priv->refresh_token);
    } else {
        json_builder_add_string_value(builder, refresh_token);
    }

    json_builder_end_object(builder);
    body = json_builder_get_root(builder);
    g_object_unref(builder);

    _send(MATRIX_HTTP_API(api),
          callback, user_data,
          CALL_API,
          "POST", "tokenreresh", NULL, NULL, body, NULL,
          FALSE, error);
}

static void
i_get_3pids(MatrixAPI *api,
            MatrixAPICallback callback,
            gpointer user_data,
            GError **error)
{
    _send(MATRIX_HTTP_API(api),
          callback, user_data,
          CALL_API,
          "GET", "account/3pid", NULL, NULL, NULL, NULL,
          FALSE, error);
}

static void
i_versions(MatrixAPI *api,
               MatrixAPICallback callback,
               gpointer user_data,
               GError **error)
{
    _send(MATRIX_HTTP_API(api),
          callback, user_data,
          CALL_API,
          "GET", "versions", NULL, NULL, NULL, NULL,
          FALSE, error);
}

static void
i_add_3pid(MatrixAPI *api,
           MatrixAPICallback callback,
           gpointer user_data,
           gboolean bind_creds,
           MatrixAPI3PidCredential *threepid_creds,
           GError **error)
{
    JsonBuilder *builder;
    JsonNode *body, *id_node;

    if ((id_node = matrix_api_3pid_credential_get_json_node(
                 threepid_creds, error)) == NULL) {
        g_set_error(error,
                    MATRIX_API_ERROR, MATRIX_API_ERROR_INCOMPLETE,
                    "Incomplete credential");

        return;
    }

    builder = json_builder_new();
    json_builder_begin_object(builder);

    json_builder_set_member_name(builder, "bind");
    json_builder_add_boolean_value(builder, bind_creds);

    json_builder_set_member_name(builder, "threePidCreds");
    json_builder_add_value(builder, id_node);

    json_builder_end_object(builder);
    body = json_builder_get_root(builder);
    g_object_unref(builder);

    _send(MATRIX_HTTP_API(api),
          callback, user_data,
          CALL_API,
          "POST", "account/3pid", NULL, NULL, body, NULL,
          FALSE, error);
}

static void
i_change_password(MatrixAPI *api,
                  MatrixAPICallback callback,
                  gpointer user_data,
                  const gchar *new_password,
                  GError **error)
{
    JsonBuilder *builder;
    JsonNode *body;

    builder = json_builder_new();
    json_builder_begin_object(builder);

    json_builder_set_member_name(builder, "new_password");
    json_builder_add_string_value(builder, new_password);

    json_builder_end_object(builder);
    body = json_builder_get_root(builder);
    g_object_unref(builder);

    _send(MATRIX_HTTP_API(api),
          callback, user_data,
          CALL_API,
          "POST", "account/password", NULL, NULL, body, NULL,
          FALSE, error);
}

static void
i_get_profile(MatrixAPI *api,
              MatrixAPICallback callback,
              gpointer user_data,
              const gchar *user_id,
              GError **error)
{
    gchar *encoded_user_id, *path;

    encoded_user_id = soup_uri_encode(user_id, NULL);
    path = g_strdup_printf("profile/%s", encoded_user_id);
    g_free(encoded_user_id);

    _send(MATRIX_HTTP_API(api),
          callback, user_data,
          CALL_API,
          "GET", path, NULL, NULL, NULL, NULL,
          FALSE, error);
    g_free(path);
}

static void
i_get_avatar_url(MatrixAPI *api,
                 MatrixAPICallback callback,
                 gpointer user_data,
                 const gchar *user_id,
                 GError **error)
{
    gchar *encoded_user_id, *path;

    encoded_user_id = soup_uri_encode(user_id, NULL);
    path = g_strdup_printf("profile/%s/avatar_url", encoded_user_id);
    g_free(encoded_user_id);

    _send(MATRIX_HTTP_API(api),
          callback, user_data,
          CALL_API,
          "GET", path, NULL, NULL, NULL, NULL,
          FALSE, error);
    g_free(path);
}

static void
i_set_avatar_url(MatrixAPI *api,
                 MatrixAPICallback callback,
                 gpointer user_data,
                 const gchar *user_id,
                 const gchar *avatar_url,
                 GError **error)
{
    gchar *encoded_user_id, *path;
    JsonBuilder *builder;
    JsonNode *body;

    encoded_user_id = soup_uri_encode(user_id, NULL);
    path = g_strdup_printf("profile/%s/avatar_url", encoded_user_id);
    g_free(encoded_user_id);

    builder = json_builder_new();
    json_builder_begin_object(builder);

    json_builder_set_member_name(builder, "avatar_url");
    json_builder_add_string_value(builder, avatar_url);

    json_builder_end_object(builder);
    body = json_builder_get_root(builder);
    g_object_unref(builder);

    _send(MATRIX_HTTP_API(api),
          callback, user_data,
          CALL_API,
          "PUT", path, NULL, NULL, body, NULL,
          FALSE, error);
    g_free(path);
}

static void
i_get_display_name(MatrixAPI *api,
                 MatrixAPICallback callback,
                 gpointer user_data,
                 const gchar *user_id,
                 GError **error)
{
    gchar *encoded_user_id, *path;

    encoded_user_id = soup_uri_encode(user_id, NULL);
    path = g_strdup_printf("profile/%s/displayname", encoded_user_id);
    g_free(encoded_user_id);

    _send(MATRIX_HTTP_API(api),
          callback, user_data,
          CALL_API,
          "GET", path, NULL, NULL, NULL, NULL,
          FALSE, error);
    g_free(path);
}

static void
i_set_display_name(MatrixAPI *api,
                   MatrixAPICallback callback,
                   gpointer user_data,
                   const gchar *user_id,
                   const gchar *display_name,
                   GError **error)
{
    gchar *encoded_user_id, *path;
    JsonBuilder *builder;
    JsonNode *body;

    encoded_user_id = soup_uri_encode(user_id, NULL);
    path = g_strdup_printf("profile/%s/displayname", encoded_user_id);
    g_free(encoded_user_id);

    builder = json_builder_new();
    json_builder_begin_object(builder);

    json_builder_set_member_name(builder, "displayname");
    json_builder_add_string_value(builder, display_name);

    json_builder_end_object(builder);
    body = json_builder_get_root(builder);
    g_object_unref(builder);

    _send(MATRIX_HTTP_API(api),
          callback, user_data,
          CALL_API,
          "PUT", path, NULL, NULL, body, NULL,
          FALSE, error);
    g_free(path);
}

static void
i_register_account(MatrixAPI *api,
                   MatrixAPICallback callback,
                   gpointer user_data,
                   MatrixAPIAccountKind account_kind,
                   gboolean bind_email,
                   const gchar *username,
                   const gchar *password,
                   GError **error)
{
    JsonBuilder *builder;
    JsonNode *body;
    GHashTable *params = NULL;

    builder = json_builder_new();
    json_builder_begin_object(builder);

    json_builder_set_member_name(builder, "bind_email");
    json_builder_add_boolean_value(builder, bind_email);

    if (username) {
        json_builder_set_member_name(builder, "username");
        json_builder_add_string_value(builder, username);
    }

    json_builder_set_member_name(builder, "password");
    json_builder_add_string_value(builder, password);

    json_builder_end_object(builder);
    body = json_builder_get_root(builder);
    g_object_unref(builder);

    if (account_kind != MATRIX_API_ACCOUNT_KIND_DEFAULT) {
        gchar *kind_string = g_enum_to_string(MATRIX_TYPE_API_ACCOUNT_KIND,
                                              account_kind, TRUE);

        params = create_query_params();

        g_hash_table_replace(params, "kind", kind_string);
    }

    _send(MATRIX_HTTP_API(api),
          callback, user_data,
          CALL_API,
          "POST", "register", params, NULL, body, NULL,
          FALSE, error);
}

static void
i_set_account_data(MatrixAPI *api,
                   MatrixAPICallback callback,
                   gpointer user_data,
                   const gchar *user_id,
                   const gchar *room_id,
                   const gchar *event_type,
                   JsonNode *content,
                   GError **error)
{
    gchar *encoded_user_id, *encoded_type, *path;

    encoded_user_id = soup_uri_encode(user_id, NULL);
    encoded_type = soup_uri_encode(event_type, NULL);

    if (room_id) {
        gchar *encoded_room_id = soup_uri_encode(room_id, NULL);

        path = g_strdup_printf("user/%s/rooms/%s/account_data/%s",
                               encoded_user_id, encoded_room_id, encoded_type);
        g_free(encoded_room_id);
    } else {
        path = g_strdup_printf("user/%s/account_data/%s",
                               encoded_user_id, encoded_type);
    }

    g_free(encoded_user_id);
    g_free(encoded_type);

    _send(MATRIX_HTTP_API(api),
          callback, user_data,
          CALL_API,
          "PUT", path, NULL, NULL, content, NULL,
          FALSE, error);
    g_free(path);
}

static void
i_get_room_tags(MatrixAPI *api,
                MatrixAPICallback callback,
                gpointer user_data,
                const gchar *user_id,
                const gchar *room_id,
                GError **error)
{
    gchar *encoded_user_id, *encoded_room_id, *path;

    encoded_user_id = soup_uri_encode(user_id, NULL);
    encoded_room_id = soup_uri_encode(room_id, NULL);
    path = g_strdup_printf("user/%s/rooms/%s/tags",
                           encoded_user_id, encoded_room_id);
    g_free(encoded_user_id);
    g_free(encoded_room_id);

    _send(MATRIX_HTTP_API(api),
          callback, user_data,
          CALL_API,
          "GET", path, NULL, NULL, NULL, NULL,
          FALSE, error);
    g_free(path);
}

static void
i_delete_room_tag(MatrixAPI *api,
                  MatrixAPICallback callback,
                  gpointer user_data,
                  const gchar *user_id,
                  const gchar *room_id,
                  const gchar *tag,
                  GError **error)
{
    gchar *encoded_user_id, *encoded_room_id, *encoded_tag, *path;

    encoded_user_id = soup_uri_encode(user_id, NULL);
    encoded_room_id = soup_uri_encode(room_id, NULL);
    encoded_tag = soup_uri_encode(tag, NULL);
    path = g_strdup_printf("user/%s/rooms/%s/tags/%s",
                           encoded_user_id, encoded_room_id, encoded_tag);
    g_free(encoded_user_id);
    g_free(encoded_room_id);
    g_free(encoded_tag);

    _send(MATRIX_HTTP_API(api),
          callback, user_data,
          CALL_API,
          "DELETE", path, NULL, NULL, NULL, NULL,
          FALSE, error);
    g_free(path);
}

static void
i_add_room_tag(MatrixAPI *api,
               MatrixAPICallback callback,
               gpointer user_data,
               const gchar *user_id,
               const gchar *room_id,
               const gchar *tag,
               JsonNode *content,
               GError **error)
{
    gchar *encoded_user_id, *encoded_room_id, *encoded_tag, *path;

    encoded_user_id = soup_uri_encode(user_id, NULL);
    encoded_room_id = soup_uri_encode(room_id, NULL);
    encoded_tag = soup_uri_encode(tag, NULL);
    path = g_strdup_printf("user/%s/rooms/%s/tags/%s",
                           encoded_user_id, encoded_room_id, encoded_tag);
    g_free(encoded_user_id);
    g_free(encoded_room_id);
    g_free(encoded_tag);

    _send(MATRIX_HTTP_API(api),
          callback, user_data,
          CALL_API,
          "PUT", path, NULL, NULL, content, NULL,
          FALSE, error);
    g_free(path);
}

static void
matrix_http_api_matrix_api_init(MatrixAPIInterface *iface)
{
    iface->set_token = i_set_token;
    iface->get_token = i_get_token;
    iface->set_refresh_token = i_set_refresh_token;
    iface->get_refresh_token = i_get_refresh_token;
    iface->get_user_id = i_get_user_id;
    iface->get_homeserver = i_get_homeserver;

    /* Media */
    iface->media_download = i_media_download;
    iface->media_thumbnail = i_media_thumbnail;
    iface->media_upload = i_media_upload;

    /* Presence */
    iface->get_presence_list = i_get_presence_list;
    iface->update_presence_list = i_update_presence_list;
    iface->get_user_presence = i_get_user_presence;
    iface->set_user_presence = i_set_user_presence;

    /* Push notifications */
    iface->update_pusher = i_update_pusher;
    iface->get_pushers = i_get_pushers;
    iface->delete_pusher = i_delete_pusher;
    iface->get_pusher = i_get_pusher;
    iface->add_pusher = i_add_pusher;
    iface->toggle_pusher = i_toggle_pusher;

    /* Room creation */
    iface->create_room = i_create_room;

    /* Room directory */
    iface->delete_room_alias = i_delete_room_alias;
    iface->get_room_id = i_get_room_id;
    iface->create_room_alias = i_create_room_alias;

    /* Room discovery */
    iface->list_public_rooms = i_list_public_rooms;

    /* Room membership */
    iface->ban_user = i_ban_user;
    iface->forget_room = i_forget_room;
    iface->invite_user_3rdparty = i_invite_user_3rdparty;
    iface->invite_user = i_invite_user;
    iface->join_room = i_join_room;
    iface->leave_room = i_leave_room;

    /* Room participation */
    iface->event_stream = i_event_stream;
    iface->get_event = i_get_event;
    iface->initial_sync = i_initial_sync;
    iface->get_event_context = i_get_event_context;
    iface->initial_sync_room = i_initial_sync_room;
    iface->list_room_members = i_list_room_members;
    iface->list_room_messages = i_list_room_messages;
    iface->send_event_receipt = i_send_event_receipt;
    iface->redact_event = i_redact_event;
    iface->send_message_event = i_send_message_event;
    iface->get_room_state = i_get_room_state;
    iface->send_room_event = i_send_room_event;
    iface->notify_room_typing = i_notify_room_typing;
    iface->sync = i_sync;
    iface->create_filter = i_create_filter;
    iface->download_filter = i_download_filter;

    /* Search */

    /* Server administration */
    iface->whois = i_whois;
    iface->versions = i_versions;

    /* Session management */
    iface->login = i_login;
    iface->token_refresh = i_token_refresh;

    /* User data */
    iface->get_3pids = i_get_3pids;
    iface->add_3pid = i_add_3pid;
    iface->change_password = i_change_password;
    iface->get_profile = i_get_profile;
    iface->get_avatar_url = i_get_avatar_url;
    iface->set_avatar_url = i_set_avatar_url;
    iface->get_display_name = i_get_display_name;
    iface->set_display_name = i_set_display_name;
    iface->register_account = i_register_account;
    iface->set_account_data = i_set_account_data;
    iface->get_room_tags = i_get_room_tags;
    iface->delete_room_tag = i_delete_room_tag;
    iface->add_room_tag = i_add_room_tag;

    /* VoIP */
    iface->get_turn_server = NULL;
}
