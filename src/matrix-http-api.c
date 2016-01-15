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

typedef struct {
    MatrixHTTPAPI *api;
    JsonNode *request_content;
    MatrixAPICallback callback;
    gpointer callback_data;
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
            SoupURI *api_uri;

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

            if (api_uri) {
                gchar *api_url;

                if (priv->uri) {
                    soup_uri_free(priv->uri);
                }


                priv->uri = api_uri;

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

                g_debug("API URL: %s", api_url);

                g_free(api_url);
            } else {
                if (api_uri) {
                    soup_uri_free(api_uri);
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

        buffer = soup_message_body_flatten(msg->response_body);
        soup_buffer_get_data(buffer, &data, &datalen);

        parser = json_parser_new();
        if (json_parser_load_from_data(parser,
                                       (const gchar *)data, datalen,
                                       &err)) {
            SoupURI *request_uri = soup_message_get_uri(msg);

            g_debug("Data (%s): %s", soup_uri_get_path(request_uri) + strlen(API_ENDPOINT), data);
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
            err = g_error_new(MATRIX_API_ERROR,
                              MATRIX_API_ERROR_BAD_RESPONSE,
                              "Malformed response (invalid JSON)");
            g_debug("Malformed response: %s", data);
        }
    }

    /* Call the assigned function, if any */
    if (request->callback) {
        request->callback(
                MATRIX_API(api),
                content,
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
      const gchar *method,
      const gchar *path,
      GHashTable *params,
      const JsonNode *content,
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

    request_path = soup_uri_new_with_base(priv->uri, path);

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

    if (content) {
        JsonGenerator *generator;

        generator = json_generator_new();
        json_generator_set_root(generator, (JsonNode *)content);
        data = json_generator_to_data(generator, &datalen);
    } else {
        data = g_strdup("{}");
        datalen = 2;
    }

    g_debug("Sending (%s %s): %s", method, url, data);

    soup_message_set_flags(message, SOUP_MESSAGE_NO_REDIRECT);
    soup_message_set_request(message,
                             "application/json",
                             SOUP_MEMORY_TAKE,
                             data, datalen);
    g_object_ref(message);

    request = g_new0(MatrixHTTPAPIRequest, 1);
    request->request_content = (JsonNode *)content;
    request->api = api;
    request->callback = callback;
    request->callback_data = user_data;

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
          "POST", "login", NULL, body,
          error);
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
        GEnumClass *_enum_class = g_type_class_ref(MATRIX_TYPE_API_ROOM_PRESET);
        GEnumValue *enum_value;

        if ((enum_value = g_enum_get_value(
                     G_ENUM_CLASS(_enum_class),
                     preset)) != NULL) {
            gchar *i;
            gchar *value_nick = g_strdup(enum_value->value_nick);

            for (i = value_nick; *i; i++) {
                if (*i == '-') {
                    *i = '_';
                }
            }

            json_builder_set_member_name(builder, "preset");
            json_builder_add_string_value(builder, value_nick);
            g_free(value_nick);
        } else {
            g_debug("Invalid room preset type");
        }

        g_type_class_unref(_enum_class);
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
        GEnumClass *_enum_class = g_type_class_ref(
                MATRIX_TYPE_API_ROOM_VISIBILITY);
        GEnumValue *enum_value;

        if ((enum_value = g_enum_get_value(
                     G_ENUM_CLASS(_enum_class),
                     visibility)) != NULL) {
            gchar *i;
            gchar *value_nick = g_strdup(enum_value->value_nick);

            for (i = value_nick; *i; i++) {
                if (*i == '-') {
                    *i = '_';
                }
            }

            json_builder_set_member_name(builder, "visibility");
            json_builder_add_string_value(builder, value_nick);
            g_free(value_nick);
        } else {
            g_debug("Invalid room visibility type");
        }

        g_type_class_unref(_enum_class);
    }

    json_builder_end_object(builder);
    body = json_builder_get_root(builder);
    g_object_unref(builder);

    _send(MATRIX_HTTP_API(api),
          callback, user_data,
          "POST", "createRoom", NULL, body,
          error);
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
          "GET", "initialSync", params, NULL,
          err);
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
          "GET", "events", params, NULL,
          err);
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
          "POST", path, NULL, NULL,
          error);
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
          "GET", "publicRooms", NULL, NULL,
          error);
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
          "POST", path, NULL, NULL,
          error);
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
          "GET", path, NULL, NULL,
          error);
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
    iface->media_download = NULL;
    iface->media_thumbnail = NULL;
    iface->media_upload = NULL;

    /* Presence */
    iface->get_presence_list = i_get_presence_list;
    iface->update_presence_list = NULL;
    iface->get_user_presence = NULL;
    iface->set_user_presence = NULL;

    /* Push notifications */
    iface->modify_pusher = NULL;
    iface->get_pushers = NULL;
    iface->delete_pusher = NULL;
    iface->get_pusher = NULL;
    iface->add_pusher = NULL;
    iface->toggle_pusher = NULL;

    /* Room creation */
    iface->create_room = i_create_room;

    /* Room directory */
    iface->delete_room_alias = NULL;
    iface->get_room_id = NULL;
    iface->create_room_alias = NULL;

    /* Room discovery */
    iface->list_public_rooms = i_list_public_rooms;

    /* Room membership */
    iface->ban_user = NULL;
    iface->forget_room = NULL;
    iface->invite_user_3rdparty = NULL;
    iface->invite_user = NULL;
    iface->join_room = i_join_room;
    iface->leave_room = i_leave_room;

    /* Room participation */
    iface->event_stream = i_event_stream;
    iface->get_event = NULL;
    iface->initial_sync = i_initial_sync;
    iface->get_event_context = NULL;
    iface->initial_sync_room = NULL;
    iface->list_room_members = NULL;
    iface->list_room_messages = NULL;
    iface->send_event_receipt = NULL;
    iface->redact_event = NULL;
    iface->send_message_event = NULL;
    iface->get_room_state = NULL;
    iface->send_room_event = NULL;
    iface->notify_room_typing = NULL;
    iface->sync = NULL;
    iface->create_filter = NULL;
    iface->download_filter = NULL;

    /* Search */

    /* Server administration */
    iface->whois = NULL;

    /* Session management */
    iface->login = i_login;
    iface->token_refresh = NULL;

    /* User data */
    iface->get_3pids = NULL;
    iface->add_3pid = NULL;
    iface->change_password = NULL;
    iface->get_profile = NULL;
    iface->get_avatar_url = NULL;
    iface->set_avatar_url = NULL;
    iface->get_display_name = NULL;
    iface->set_display_name = NULL;
    iface->register_account = NULL;
    iface->set_account_data = NULL;
    iface->get_room_tags = NULL;
    iface->delete_room_tag = NULL;
    iface->add_room_tag = NULL;

    /* VoIP */
    iface->get_turn_server = NULL;
}
