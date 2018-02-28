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

#include <libsoup/soup.h>
#include <json-glib/json-glib.h>
#include <string.h>
#include "matrix-http-api.h"
#include "matrix-enumtypes.h"
#include "config.h"
#include "utils.h"
#include "matrix-compacts.h"
#include "matrix-event-state-base.h"
#include "matrix-event-base.h"

/**
 * SECTION:matrix-http-api
 * @short_description: low-level API to communicate with homeservers via HTTP
 *
 * This is a class for low level communication with a Matrix.org server via HTTP.
 */
enum  {
    PROP_0,
    PROP_BASE_URL,
    PROP_VALIDATE_CERTIFICATE,
    PROP_USER_ID,
    PROP_TOKEN,
    PROP_HOMESERVER,
    NUM_PROPERTIES
};

static GParamSpec *matrix_http_api_properties[NUM_PROPERTIES];

typedef struct {
    SoupSession *soup_session;
    gchar *base_url;
    SoupURI *api_uri;
    SoupURI *media_uri;
    gchar *token;
    gchar *homeserver;
    gchar *user_id;
} MatrixHTTPAPIPrivate;

static void matrix_http_api_matrix_api_interface_init(MatrixAPIInterface * iface);

G_DEFINE_TYPE_EXTENDED(MatrixHTTPAPI, matrix_http_api, G_TYPE_OBJECT, 0, G_ADD_PRIVATE(MatrixHTTPAPI) G_IMPLEMENT_INTERFACE(MATRIX_TYPE_API, matrix_http_api_matrix_api_interface_init));

typedef enum  {
    CALL_TYPE_API,
    CALL_TYPE_MEDIA
} CallType;

#define API_ENDPOINT "/_matrix/client/r0/"
#define MEDIA_ENDPOINT "/_matrix/media/r0/"

#define string_get(s, i) (((s) == NULL) ? 0 : s[i])
#define string_offset(s, o) (((s) == NULL) ? NULL : (s) + o)
#define uri_encode(s) gchar *enc_ ## s = soup_uri_encode(s, NULL)

static void
_matrix_http_api_set_url(MatrixHTTPAPI *matrix_http_api, SoupURI **uri, const gchar *base_url, const gchar *endpoint)
{
    gchar *url;
    SoupURI *new_uri;

    g_return_if_fail(matrix_http_api != NULL);
    g_return_if_fail(base_url != NULL);
    g_return_if_fail(endpoint != NULL);
    g_return_if_fail(uri != NULL);

    if (string_get(base_url, strlen(base_url) - 1) == '/') {
        url = g_strdup_printf("%s%s", base_url, string_offset(endpoint, 1));
    } else {
        url = g_strdup_printf("%s%s", base_url, endpoint);
    }

    new_uri = soup_uri_new(url);

    if (SOUP_URI_VALID_FOR_HTTP(new_uri)) {
         *uri = new_uri;
    } else {
         *uri = NULL;
    }
    g_free(url);
}

MatrixHTTPAPI *
matrix_http_api_construct(GType object_type, const gchar *base_url, const gchar *token)
{
    MatrixHTTPAPI *ret;
    MatrixHTTPAPIPrivate *priv;

    g_return_val_if_fail(base_url != NULL, NULL);

    ret = (MatrixHTTPAPI*) g_object_new(object_type,
                                        "base-url", base_url,
                                        "token", token,
                                        NULL);
    priv = matrix_http_api_get_instance_private(ret);

    g_object_set(priv->soup_session, "ssl-strict", TRUE, NULL);

    return ret;
}

/**
 * matrix_http_api_new:
 * @base_url: the base URL of the homeserver to use
 * @token: an access token to use
 *
 * Create a new #MatrixHTTPAPI object.
 *
 * Returns: (transfer full): a new #MatrixHTTPAPI object
 */
MatrixHTTPAPI *
matrix_http_api_new(const gchar *base_url, const gchar *token)
{
    return matrix_http_api_construct(MATRIX_TYPE_HTTP_API, base_url, token);
}

static GHashTable *
_matrix_http_api_create_query_params(void)
{
    return g_hash_table_new_full(g_str_hash, g_str_equal, g_free, g_free);
}

typedef struct {
    MatrixHTTPAPI *matrix_http_api;
    MatrixAPICallback cb;
    CallType call_type;
    gboolean accept_non_json;
    gpointer cb_target;
    guint refcount;
} SendCallbackData;

static void
_matrix_http_api_response_callback(SoupSession *session, SoupMessage *msg, gpointer user_data)
{
    SendCallbackData *callback_data = user_data;
    MatrixHTTPAPI *matrix_http_api = callback_data->matrix_http_api;
    MatrixHTTPAPIPrivate *priv = matrix_http_api_get_instance_private(matrix_http_api);
    CallType call_type = callback_data->call_type;
    gboolean accept_non_json = callback_data->accept_non_json;
    MatrixAPICallback cb = callback_data->cb;
    void *cb_target = callback_data->cb;
    SoupURI *request_uri = soup_message_get_uri(msg);
    const gchar *request_url = soup_uri_get_path(request_uri);
    GError *err = NULL;
    GByteArray *raw_content = NULL;
    JsonNode *content = NULL;

    switch (call_type) {
        case CALL_TYPE_API:
            request_url = g_strdup(request_url + strlen(API_ENDPOINT));

            break;
        case CALL_TYPE_MEDIA:
            request_url = g_strdup(request_url + strlen(MEDIA_ENDPOINT));

            break;
    }

    if ((msg->status_code < 100) || (msg->status_code >= 400)) {
        err = g_error_new(MATRIX_ERROR, MATRIX_ERROR_COMMUNICATION_ERROR,
                          "%s %u: %s",
                          (msg->status_code < 100) ? "Network error" : "HTTP",
                          msg->status_code,
                          msg->reason_phrase);
    } else {
        SoupBuffer *buffer = soup_message_body_flatten(msg->response_body);
        gsize datalen = buffer->length;
        JsonParser *parser = json_parser_new();
        gboolean is_json;

        is_json = json_parser_load_from_data(parser, buffer->data, (gssize)buffer->length, NULL);

        if (is_json) {
#if DEBUG
            g_debug("Response (%s): %s", request_url, buffer->data);
#endif

            content = json_parser_get_root(parser);

            if (json_node_get_node_type(content) == JSON_NODE_OBJECT) {
                JsonObject *root = json_node_get_object(content);
                JsonNode *node;
                JsonNode *errcode_node;
                JsonNode *error_node;

                /* Check if the response holds an access token; if it
                 * does, set it as our new token */
                if ((node = json_object_get_member(root, "access_token")) != NULL) {
                    const gchar *access_token;

                    if ((access_token = json_node_get_string(node)) != NULL) {
#if DEBUG
                        g_debug("Got new access token: %s", access_token);
#endif

                        g_free(priv->token);
                        priv->token = g_strdup(access_token);
                    }
                }

                /* Check if the response holds a homeserver name */
                if ((node = json_object_get_member(root, "home_server")) != NULL) {
                    const gchar *homeserver = json_node_get_string(node);

#if DEBUG
                    g_debug("Our home server calls itself %s", homeserver);
#endif

                    g_free(priv->homeserver);
                    priv->homeserver = g_strdup(homeserver);
                }

                /* Check if the response holds a user ID; if it does,
                 * set this as our user ID */
                if ((node = json_object_get_member(root, "user_id")) != NULL) {
                    const gchar *user_id = json_node_get_string(node);

#if DEBUG
                    g_debug("We are reported to be logged in as %s", user_id);
#endif

                    g_free(priv->user_id);
                    priv->user_id = g_strdup(user_id);
                }

                /* Check if the response holds an error */
                errcode_node = json_object_get_member(root, "errcode");
                error_node = json_object_get_member(root, "error");
                const gchar *error = NULL;
                const gchar *errcode = NULL;

                if ((errcode_node != NULL) || (error_node != NULL)) {
                    err = g_error_new_literal(MATRIX_ERROR, MATRIX_ERROR_UNKNOWN_ERROR,
                                              "The error is not known to this library");

                    if (error_node != NULL) {
                        error = json_node_get_string(error_node);
                    }

                    if (errcode_node != NULL) {
                        errcode = json_node_get_string(errcode_node);

                        if (g_ascii_strncasecmp(errcode, "M_", 2) == 0) {
                            gchar *errcode_enum_name = g_strdup_printf("MATRIX_ERROR_%s", errcode);
                            GEnumClass *error_class = g_type_class_ref(MATRIX_TYPE_ERROR);
                            GEnumValue *enum_value = g_enum_get_value_by_name(error_class, errcode_enum_name);
                            g_type_class_unref(error_class);
                            g_free(errcode_enum_name);

                            if (enum_value != NULL) {
                                err->code = enum_value->value;
                            } else {
                                g_warning("An unknown error code '%s' was sent by the homeserver. You may want to report it to the Matrix GLib developers", errcode);
                            }
                        }
                    } else {
                        g_warning("An error was sent by the homeserver, but no error code was specified. You may want to report this to the homeserver admins.");
                        g_clear_error(&err);

                        err = g_error_new_literal(MATRIX_ERROR, MATRIX_ERROR_UNSPECIFIED, "No error code was sent by the server");
                    }

                    if ((errcode_node != NULL) && (error_node != NULL)) {
                        err->message = g_strdup_printf("%s: %s", errcode, error);
                    } else if (errcode_node != NULL) {
                        err->message = g_strdup(errcode);
                    } else {
                        err->message = g_strdup_printf("(No errcode given) %s", error);
                    }
                }
            } else if (json_node_get_node_type(content) != JSON_NODE_ARRAY) {
                err = g_error_new_literal(MATRIX_ERROR, MATRIX_ERROR_BAD_RESPONSE,
                                          "Bad response: not a JSON object, nor an array.");
            }
        } else {
            if (accept_non_json) {
                raw_content = g_byte_array_sized_new((uint)datalen);
                g_byte_array_append(raw_content, (guint8 *)buffer->data, buffer->length);

#if DEBUG
                g_debug("Binary data (%s): %u bytes", request_url, (uint)datalen);
#endif
            } else {
                err = g_error_new_literal(MATRIX_ERROR, MATRIX_ERROR_BAD_RESPONSE,
                                          "Malformed response (invalid JSON)");

#if DEBUG
                g_debug("Malformed response (%s): %s", request_url, buffer->data);
#endif
            }
        }
    }

    /* Call the assigned function, if any */
    if (cb != NULL) {
        cb(MATRIX_API(matrix_http_api),
           soup_message_headers_get_content_type(msg->response_headers, NULL),
           content, raw_content,
           err,
           cb_target);
    }

    g_free(callback_data);
}

static void
_matrix_http_api_send(MatrixHTTPAPI *matrix_http_api,
                      MatrixAPICallback cb,
                      void *cb_target,
                      CallType call_type,
                      const gchar *method,
                      const gchar *path,
                      GHashTable *parms,
                      const gchar *content_type,
                      JsonNode *json_content,
                      GByteArray *raw_content,
                      gboolean accept_non_json,
                      GError **error)
{
    MatrixHTTPAPIPrivate *priv;
    SoupMessage *message;
    SoupURI *request_path = NULL;
    gpointer request_data;
    gsize request_len;
    SendCallbackData *callback_data;

    g_return_if_fail(matrix_http_api != NULL);
    g_return_if_fail(method != NULL);
    g_return_if_fail(path != NULL);

    priv = matrix_http_api_get_instance_private(matrix_http_api);

    if ((priv->api_uri == NULL) || (priv->media_uri == NULL)) {
        g_set_error(error, MATRIX_ERROR, MATRIX_ERROR_COMMUNICATION_ERROR, "No valid base URL");

        return;
    }

    if ((json_content != NULL) && (raw_content != NULL)) {
        g_critical("json_content and raw_content cannot be used together. This is a bug.");
    }

    if (!g_str_is_ascii(method)) {
        g_critical("Method must be ASCII. This is a bug.");
    }

    if ((g_ascii_strcasecmp(method, "GET") != 0) &&
        (g_ascii_strcasecmp(method, "POST") != 0) &&
        (g_ascii_strcasecmp(method, "PUT") != 0) &&
        (g_ascii_strcasecmp(method, "DELETE") != 0)) {
        g_critical("Method %s is invalid. This is a bug.", method);
    }

    if (call_type == CALL_TYPE_MEDIA) {
        request_path = soup_uri_new_with_base(priv->media_uri, path);
    } else {
        request_path = soup_uri_new_with_base(priv->api_uri, path);
    }

    if (parms == NULL) {
        parms = _matrix_http_api_create_query_params();
    }

    if (priv->token != NULL) {
#if DEBUG
        g_debug("Adding access token '%s'", priv->token);
#endif

        g_hash_table_replace(parms, g_strdup("access_token"), priv->token);
    }

    soup_uri_set_query_from_form(request_path, parms);

    message = soup_message_new_from_uri(method, request_path);

    if (json_content != NULL) {
        JsonGenerator *generator = json_generator_new();

        json_generator_set_root(generator, json_content);
        request_data = json_generator_to_data(generator, &request_len);
    } else if (raw_content != NULL) {
        request_len = raw_content->len;
        request_data = raw_content->data;
    } else {
        request_len = 2;
        request_data = g_strdup("{}");
    }

#if DEBUG
    g_debug("Sending %lu bytes (%s %s): %s",
            request_len,
            method,
            soup_uri_to_string(request_path, FALSE),  // TODO: Free this!
            (raw_content != NULL) ? "<Binary data>" : (gchar *)request_data);
#endif

    soup_message_set_flags(message, SOUP_MESSAGE_NO_REDIRECT);
    soup_message_set_request(message,
                             (content_type == NULL) ? "application/json" : content_type,
                             SOUP_MEMORY_COPY,
                             request_data,
                             request_len);

    callback_data = g_new0(SendCallbackData, 1);

    callback_data->matrix_http_api = matrix_http_api;
    callback_data->refcount = 1;
    callback_data->cb = cb;
    callback_data->cb_target = cb_target;
    callback_data->call_type = call_type;
    callback_data->accept_non_json = accept_non_json;

    soup_session_queue_message(priv->soup_session, message, _matrix_http_api_response_callback, callback_data);
}

static void
matrix_http_api_media_download(MatrixAPI *matrix_api, MatrixAPICallback cb, void *cb_target, const gchar *server_name, const gchar *media_id, GError **error)
{
    gchar *path;

    g_return_if_fail(server_name != NULL);
    g_return_if_fail(media_id != NULL);

    uri_encode(server_name);
    uri_encode(media_id);
    path = g_strconcat("download/", enc_server_name, "/", enc_media_id, NULL);
    g_free(enc_server_name);
    g_free(enc_media_id);

    _matrix_http_api_send(MATRIX_HTTP_API(matrix_api), cb, cb_target, CALL_TYPE_MEDIA, "GET", path, NULL, NULL, NULL, NULL, TRUE, error);

    g_free(path);
}

static void
matrix_http_api_media_thumbnail(MatrixAPI *matrix_api, MatrixAPICallback cb, void *cb_target, const gchar *server_name, const gchar *media_id, guint width, guint height, MatrixResizeMethod method, GError **error)
{
    gchar *path;
    GHashTable *parms;

    g_return_if_fail(server_name != NULL);
    g_return_if_fail(media_id != NULL);

    uri_encode(server_name);
    uri_encode(media_id);
    path = g_strconcat("download/", enc_server_name, "/", enc_media_id, NULL);
    g_free(enc_server_name);
    g_free(enc_media_id);

    parms = _matrix_http_api_create_query_params();

    if (width > 0) {
        g_hash_table_replace(parms, g_strdup("width"), g_strdup_printf("%u", width));
    }

    if (height > 0) {
        g_hash_table_replace(parms, g_strdup("height"), g_strdup_printf("%u", height));
    }

    if (method != MATRIX_RESIZE_METHOD_DEFAULT) {
        switch (method) {
            case MATRIX_RESIZE_METHOD_CROP:
                g_hash_table_replace(parms, g_strdup("method"), g_strdup("crop"));

                break;
            case MATRIX_RESIZE_METHOD_SCALE:
                g_hash_table_replace(parms, g_strdup("method"), g_strdup("scale"));

                break;
            // This is here only to prevent compiler warnings
            case MATRIX_RESIZE_METHOD_DEFAULT: break;
        }
    }

    _matrix_http_api_send(MATRIX_HTTP_API(matrix_api), cb, cb_target, CALL_TYPE_MEDIA, "GET", path, parms, NULL, NULL, NULL, TRUE, error);

    g_free(path);
    g_hash_table_unref(parms);
}

static void
matrix_http_api_media_upload(MatrixAPI *matrix_api, MatrixAPICallback cb, void *cb_target, const gchar *content_type, GByteArray *content, GError **error)
{
    g_return_if_fail(content != NULL);

    _matrix_http_api_send(MATRIX_HTTP_API(matrix_api), cb, cb_target, CALL_TYPE_MEDIA, "POST", "upload", NULL, content_type, NULL, content, FALSE, error);
}

static void
matrix_http_api_get_presence_list(MatrixAPI *matrix_api, MatrixAPICallback cb, void *cb_target, const gchar *user_id, GError **error)
{
    gchar *path;

    g_return_if_fail(user_id != NULL);

    uri_encode(user_id);
    path = g_strconcat("presence/list/", enc_user_id, NULL);
    g_free(enc_user_id);

    _matrix_http_api_send(MATRIX_HTTP_API(matrix_api), cb, cb_target, CALL_TYPE_API, "GET", path, NULL, NULL, NULL, NULL, FALSE, error);

    g_free(path);
}

static void
matrix_http_api_update_presence_list(MatrixAPI *matrix_api, MatrixAPICallback cb, void *cb_target, const gchar *user_id, gchar **drop_ids, int n_drop_ids, gchar **invite_ids, int n_invite_ids, GError **error)
{
    gchar *path;
    JsonBuilder *builder;
    JsonNode *root_node;

    g_return_if_fail(user_id != NULL);

    uri_encode(user_id);
    path = g_strconcat("presence/", enc_user_id, "/status", NULL);
    g_free(enc_user_id);

    builder = json_builder_new();
    json_builder_begin_object(builder);

    if (n_drop_ids > 0) {
        json_builder_set_member_name(builder, "drop");
        json_builder_begin_array(builder);

        for (gint i = 0; i < n_drop_ids; i++) {
            json_builder_add_string_value(builder, drop_ids[i]);
        }

        json_builder_end_array(builder);
    }

    if (n_invite_ids > 0) {
        json_builder_set_member_name(builder, "invite");
        json_builder_begin_array(builder);

        for (gint i = 0; i < n_invite_ids; i++) {
            json_builder_add_string_value(builder, invite_ids[i]);
        }

        json_builder_end_array(builder);
    }

    json_builder_end_object(builder);

    root_node = json_builder_get_root(builder);

    _matrix_http_api_send(MATRIX_HTTP_API(matrix_api), cb, cb_target, CALL_TYPE_API, "POST", path, NULL, NULL, root_node, NULL, FALSE, error);

    g_free(path);
    json_node_unref(root_node);
    g_object_unref(builder);
}

static void
matrix_http_api_get_presence(MatrixAPI *matrix_api, MatrixAPICallback cb, void *cb_target, const gchar *user_id, GError **error)
{
    gchar *path;

    g_return_if_fail(user_id != NULL);

    uri_encode(user_id);
    path = g_strconcat("presence/", enc_user_id, "/status", NULL);
    g_free(enc_user_id);

    _matrix_http_api_send(MATRIX_HTTP_API(matrix_api), cb, cb_target, CALL_TYPE_API, "GET", path, NULL, NULL, NULL, NULL, FALSE, error);

    g_free(path);
}

static void
matrix_http_api_set_presence(MatrixAPI *matrix_api, MatrixAPICallback cb, void *cb_target, const gchar *user_id, MatrixPresence presence, const gchar *status_message, GError **error)
{
    gchar *path;
    gchar *presence_str;
    JsonBuilder *builder;
    JsonNode *root_node;

    g_return_if_fail(user_id != NULL);

    uri_encode(user_id);
    path = g_strconcat("presence/", enc_user_id, "/status", NULL);
    g_free(enc_user_id);

    builder = json_builder_new();
    json_builder_begin_object(builder);

    json_builder_set_member_name(builder, "presence");
    presence_str = _matrix_g_enum_to_string(MATRIX_TYPE_PRESENCE, presence, '_');
    json_builder_add_string_value(builder, presence_str);
    g_free(presence_str);

    if (status_message != NULL) {
        json_builder_set_member_name(builder, "status_msg");
        json_builder_add_string_value(builder, status_message);
    }

    json_builder_end_object(builder);

    root_node = json_builder_get_root(builder);

    _matrix_http_api_send(MATRIX_HTTP_API(matrix_api), cb, cb_target, CALL_TYPE_API, "POST", path, NULL, NULL, root_node, NULL, FALSE, error);

    json_node_unref(root_node);
    g_object_unref(builder);
    g_free(path);
}

static void
matrix_http_api_update_pusher(MatrixAPI *matrix_api, MatrixAPICallback cb, void *cb_target, MatrixPusher *pusher, GError **error)
{
    JsonNode *pusher_node;
    GError *inner_error = NULL;

    g_return_if_fail(pusher != NULL);

    pusher_node = matrix_json_compact_get_json_node(MATRIX_JSON_COMPACT(pusher), &inner_error);

    if (inner_error != NULL) {
        g_propagate_error(error, inner_error);
        return;
    }

    _matrix_http_api_send(MATRIX_HTTP_API(matrix_api), cb, cb_target, CALL_TYPE_API, "POST", "pushers/set", NULL, NULL, pusher_node, NULL, FALSE, error);

    json_node_unref(pusher_node);
}

static void
matrix_http_api_get_pushers(MatrixAPI *matrix_api, MatrixAPICallback cb, void *cb_target, GError **error)
{
    _matrix_http_api_send(MATRIX_HTTP_API(matrix_api), cb, cb_target, CALL_TYPE_API, "GET", "pushrules", NULL, NULL, NULL, NULL, FALSE, error);
}

static gchar *
_matrix_http_api_pusher_url(const gchar *scope, MatrixPusherKind kind, const gchar *rule_id)
{
    gchar *kind_str;
    gchar *url;

    g_return_val_if_fail(scope != NULL, NULL);
    g_return_val_if_fail(rule_id != NULL, NULL);

    uri_encode(scope);
    kind_str = _matrix_g_enum_to_string(MATRIX_TYPE_PUSHER_KIND, kind, '_');
    uri_encode(kind_str);
    g_free(kind_str);
    uri_encode(rule_id);
    url = g_strconcat("pushrules/", enc_scope, "/", enc_kind_str, "/", enc_rule_id, NULL);
    g_free(enc_scope);
    g_free(enc_kind_str);
    g_free(enc_rule_id);

    return url;
}

static void
_matrix_http_api_pusher_modif(MatrixHTTPAPI *matrix_http_api, MatrixAPICallback cb, void *cb_target, const gchar *method, const gchar *scope, MatrixPusherKind kind, const gchar *rule_id, GError **error)
{
    gchar *path;

    g_return_if_fail(matrix_http_api != NULL);
    g_return_if_fail(method != NULL);
    g_return_if_fail(scope != NULL);
    g_return_if_fail(rule_id != NULL);

    path = _matrix_http_api_pusher_url(scope, kind, rule_id);

    _matrix_http_api_send(matrix_http_api, cb, cb_target, CALL_TYPE_API, method, path, NULL, NULL, NULL, NULL, FALSE, error);

    g_free(path);
}

static void
matrix_http_api_delete_pushrule(MatrixAPI *matrix_api, MatrixAPICallback cb, void *cb_target, const gchar *scope, MatrixPusherKind kind, const gchar *rule_id, GError **error)
{
    g_return_if_fail(scope != NULL);
    g_return_if_fail(rule_id != NULL);

    _matrix_http_api_pusher_modif(MATRIX_HTTP_API(matrix_api), cb, cb_target, "DELETE", scope, kind, rule_id, error);
}

static void
matrix_http_api_get_pushrule(MatrixAPI *matrix_api, MatrixAPICallback cb, void *cb_target, const gchar *scope, MatrixPusherKind kind, const gchar *rule_id, GError **error)
{
    g_return_if_fail(scope != NULL);
    g_return_if_fail(rule_id != NULL);

    _matrix_http_api_pusher_modif(MATRIX_HTTP_API(matrix_api), cb, cb_target, "GET", scope, kind, rule_id, error);
}

static void
matrix_http_api_add_pushrule(MatrixAPI *matrix_api, MatrixAPICallback cb, void *cb_target, const gchar *scope, MatrixPusherKind kind, const gchar *rule_id, const gchar *before, const gchar *after, gchar **actions, int n_actions, MatrixPusherConditionKind *conditions, int n_conditions, GError **error)
{
    JsonBuilder *builder;
    GHashTable *parms;
    gchar *path;
    JsonNode *root_node;

    g_return_if_fail(scope != NULL);
    g_return_if_fail(rule_id != NULL);

    parms = _matrix_http_api_create_query_params();

    if (before != NULL) {
        g_hash_table_replace(parms, g_strdup("before"), g_strdup(before));
    }

    if (after != NULL) {
        g_hash_table_replace(parms, g_strdup("after"), g_strdup(after));
    }

    builder = json_builder_new();
    json_builder_begin_object(builder);

    json_builder_set_member_name(builder, "actions");
    json_builder_begin_array(builder);

    for (gint i = 0; i < n_actions; i++) {
        json_builder_add_string_value(builder, actions[i]);
    }

    json_builder_end_array(builder);

    if (n_conditions > 0) {
        json_builder_set_member_name(builder, "conditions");
        json_builder_begin_array(builder);

        for (gint i = 0; i < n_conditions; i++) {
            gchar *kind_str = _matrix_g_enum_to_string(MATRIX_TYPE_PUSHER_CONDITION_KIND, conditions[i], '_');

            if (kind_str == NULL) {
                g_warning("Skipping invalid condition kind %d", conditions[i]);

                continue;
            }

            json_builder_begin_object(builder);
            json_builder_set_member_name(builder, "kind");
            json_builder_add_string_value(builder, kind_str);
            json_builder_end_object(builder);
        }

        json_builder_end_array(builder);
    }

    json_builder_end_object(builder);
    root_node = json_builder_get_root(builder);

    path = _matrix_http_api_pusher_url(scope, kind, rule_id);
    _matrix_http_api_send(MATRIX_HTTP_API(matrix_api), cb, cb_target, CALL_TYPE_API, "GET", path, parms, NULL, root_node, NULL, FALSE, error);

    g_free(path);
    json_node_unref(root_node);
    g_object_unref(builder);
}


static void
matrix_http_api_toggle_pushrule(MatrixAPI *matrix_api, MatrixAPICallback cb, void *cb_target, const gchar *scope, MatrixPusherKind kind, const gchar *rule_id, gboolean enabled, GError **error)
{
    JsonBuilder *builder;
    JsonNode *root_node;
    gchar *path;

    g_return_if_fail(scope != NULL);
    g_return_if_fail(rule_id != NULL);

    builder = json_builder_new();
    json_builder_begin_object(builder);

    json_builder_set_member_name(builder, "enabled");
    json_builder_add_boolean_value(builder, enabled);

    json_builder_end_object(builder);
    root_node = json_builder_get_root(builder);

    path = _matrix_http_api_pusher_url(scope, kind, rule_id);

    _matrix_http_api_send(MATRIX_HTTP_API(matrix_api), cb, cb_target, CALL_TYPE_API, "GET", path, NULL, NULL, root_node, NULL, FALSE, error);

    g_free(path);
    json_node_unref(root_node);
    g_object_unref(builder);
}

static void
matrix_http_api_get_pushrules(MatrixAPI *matrix_api, MatrixAPICallback cb, void *cb_target, GError **error)
{
    _matrix_http_api_send(MATRIX_HTTP_API(matrix_api), cb, cb_target, CALL_TYPE_API, "GET", "pushrules", NULL, NULL, NULL, NULL, FALSE, error);
}

static void
matrix_http_api_create_room(MatrixAPI *matrix_api, MatrixAPICallback cb, void *cb_target, MatrixRoomPreset preset, const gchar *room_name, const gchar *room_alias, const gchar *topic, MatrixRoomVisibility visibility, JsonNode *creation_content, MatrixEventState **initial_state, int n_initial_state, gchar **invitees, int n_invitees, Matrix3PidCredential **invite_3pids, int n_invite_3pids, GError **error)
{
    JsonBuilder *builder = json_builder_new();
    JsonNode *root_node;

    json_builder_begin_object(builder);

    if (creation_content != NULL) {
        json_builder_set_member_name(builder, "creation_content");
        json_builder_add_value(builder, creation_content);
    }

    if (n_initial_state > 0) {
        json_builder_set_member_name(builder, "initial_state");
        json_builder_begin_array(builder);

        for (gint i = 0; i < n_initial_state; i++) {
            JsonNode *event_node = matrix_event_base_get_json(MATRIX_EVENT_BASE(initial_state[i]));

            json_builder_add_value(builder, event_node);
        }

        json_builder_end_array(builder);
    }

    if (n_invitees > 0) {
        json_builder_set_member_name(builder, "invite");
        json_builder_begin_array(builder);

        for (gint i = 0; i < n_invitees; i++) {
            json_builder_add_string_value(builder, invitees[i]);
        }

        json_builder_end_array(builder);
    }

    if (n_invite_3pids > 0) {
        json_builder_set_member_name(builder, "invite_3pid");
        json_builder_begin_array(builder);

        for (gint i = 0; i < n_invite_3pids; i++) {
            GError *inner_error = NULL;
            JsonNode *node = matrix_json_compact_get_json_node(MATRIX_JSON_COMPACT(invite_3pids[i]), &inner_error);

            if (inner_error == NULL) {
                json_builder_add_value(builder, node);
            }
            // TODO exceptions should be handled here somehow
        }

        json_builder_end_array(builder);
    }

    if (room_name != NULL) {
        json_builder_set_member_name(builder, "name");
        json_builder_add_string_value(builder, room_name);
    }

    if (preset != MATRIX_ROOM_PRESET_NONE) {
        gchar *preset_str = _matrix_g_enum_to_string(MATRIX_TYPE_ROOM_PRESET, preset, '_');

        if (preset_str != NULL) {
            json_builder_set_member_name(builder, "preset");
            json_builder_add_string_value(builder, preset_str);
            g_free(preset_str);
        } else {
            g_warning("Invalid room preset type");
        }
    }

    if (room_alias != NULL) {
        json_builder_set_member_name(builder, "room_alias_name");
        json_builder_add_string_value(builder, room_alias);
    }

    if (topic != NULL) {
        json_builder_set_member_name(builder, "topic");
        json_builder_add_string_value(builder, topic);
    }

    if (visibility != MATRIX_ROOM_VISIBILITY_DEFAULT) {
        gchar *visibility_str = _matrix_g_enum_to_string(MATRIX_TYPE_ROOM_VISIBILITY, visibility, '_');

        if (visibility_str != NULL) {
            json_builder_set_member_name(builder, "visibility");
            json_builder_add_string_value(builder, visibility_str);
            g_free(visibility_str);
        } else {
            g_warning("Invalid room visibility type");
        }
    }

    json_builder_end_object(builder);
    root_node = json_builder_get_root(builder);

    _matrix_http_api_send(MATRIX_HTTP_API(matrix_api), cb, cb_target, CALL_TYPE_API, "POST", "createRoom", NULL, NULL, root_node, NULL, FALSE, error);

    json_node_unref(root_node);
    g_object_unref(builder);
}

static void
matrix_http_api_delete_room_alias(MatrixAPI *matrix_api, MatrixAPICallback cb, void *cb_target, const gchar *room_alias, GError **error)
{
    gchar *path;

    g_return_if_fail(room_alias != NULL);

    uri_encode(room_alias);
    path = g_strconcat("directory/room/", enc_room_alias, NULL);
    g_free(enc_room_alias);

    _matrix_http_api_send(MATRIX_HTTP_API(matrix_api), cb, cb_target, CALL_TYPE_API, "DELETE", path, NULL, NULL, NULL, NULL, FALSE, error);

    g_free(path);
}

static void
matrix_http_api_get_room_id(MatrixAPI *matrix_api, MatrixAPICallback cb, void *cb_target, const gchar *room_alias, GError **error)
{
    gchar *path;

    g_return_if_fail(room_alias != NULL);

    uri_encode(room_alias);
    path = g_strconcat("directory/room/", enc_room_alias, NULL);
    g_free(enc_room_alias);

    _matrix_http_api_send(MATRIX_HTTP_API(matrix_api), cb, cb_target, CALL_TYPE_API, "GET", path, NULL, NULL, NULL, NULL, FALSE, error);

    g_free(path);
}

static void
matrix_http_api_create_room_alias(MatrixAPI *matrix_api, MatrixAPICallback cb, void *cb_target, const gchar *room_id, const gchar *room_alias, GError **error)
{
    JsonBuilder *builder;
    JsonNode *root_node;
    gchar *path;

    g_return_if_fail(room_id != NULL);
    g_return_if_fail(room_alias != NULL);

    uri_encode(room_alias);
    path = g_strconcat("directory/room/", enc_room_alias, NULL);
    g_free(enc_room_alias);

    builder = json_builder_new();
    json_builder_begin_object(builder);

    json_builder_set_member_name(builder, "room_id");
    json_builder_add_string_value(builder, room_id);

    json_builder_end_object(builder);

    root_node = json_builder_get_root(builder);

    _matrix_http_api_send(MATRIX_HTTP_API(matrix_api), cb, cb_target, CALL_TYPE_API, "PUT", path, NULL, NULL, root_node, NULL, FALSE, error);

    json_node_unref(root_node);
    g_object_unref(builder);
}

static void
matrix_http_api_list_public_rooms(MatrixAPI *matrix_api, MatrixAPICallback cb, void *cb_target, GError **error)
{
    _matrix_http_api_send(MATRIX_HTTP_API(matrix_api), cb, cb_target, CALL_TYPE_API, "GET", "publicRooms", NULL, NULL, NULL, NULL, FALSE, error);
}

static void
matrix_http_api_ban_user(MatrixAPI *matrix_api, MatrixAPICallback cb, void *cb_target, const gchar *room_id, const gchar *user_id, const gchar *reason, GError **error)
{
    gchar *path;
    JsonNode *root_node;
    JsonBuilder *builder;

    g_return_if_fail(room_id != NULL);
    g_return_if_fail(user_id != NULL);

    uri_encode(room_id);
    path = g_strconcat("rooms/", enc_room_id, "/ban", NULL);
    g_free(enc_room_id);

    builder = json_builder_new();

    json_builder_begin_object(builder);

    json_builder_set_member_name(builder, "user_id");
    json_builder_add_string_value(builder, user_id);

    if (reason != NULL) {
        json_builder_set_member_name(builder, "reason");
        json_builder_add_string_value(builder, reason);
    }

    json_builder_end_object(builder);
    root_node = json_builder_get_root(builder);

    _matrix_http_api_send(MATRIX_HTTP_API(matrix_api), cb, cb_target, CALL_TYPE_API, "POST", path, NULL, NULL, root_node, NULL, FALSE, error);

    g_free(path);
    json_node_unref(root_node);
    g_object_unref(builder);
}

static void
matrix_http_api_unban_user(MatrixAPI *matrix_api, MatrixAPICallback cb, void *cb_target, const gchar *room_id, const gchar *user_id, GError **error)
{
    gchar *path;
    JsonBuilder *builder;
    JsonNode *root_node;

    g_return_if_fail(room_id != NULL);
    g_return_if_fail(user_id != NULL);

    uri_encode(room_id);
    path = g_strconcat("rooms/", enc_room_id, "/unban", NULL);
    g_free(enc_room_id);

    builder = json_builder_new();

    json_builder_begin_object(builder);

    json_builder_set_member_name(builder, "user_id");
    json_builder_add_string_value(builder, user_id);

    json_builder_end_object(builder);
    root_node = json_builder_get_root(builder);

    _matrix_http_api_send(MATRIX_HTTP_API(matrix_api), cb, cb_target, CALL_TYPE_API, "POST", path, NULL, NULL, root_node, NULL, FALSE, error);

    g_free(path);
    json_node_unref(root_node);
    g_object_unref(builder);
}

static void
matrix_http_api_forget_room(MatrixAPI *matrix_api, MatrixAPICallback cb, void *cb_target, const gchar *room_id, GError **error)
{
    gchar *path;

    g_return_if_fail(room_id != NULL);

    uri_encode(room_id);
    path = g_strconcat("rooms/", enc_room_id, "/forget", NULL);
    g_free(enc_room_id);

    _matrix_http_api_send(MATRIX_HTTP_API(matrix_api), cb, cb_target, CALL_TYPE_API, "POST", path, NULL, NULL, NULL, NULL, FALSE, error);

    g_free(path);
}

static void
matrix_http_api_invite_user_3rdparty(MatrixAPI *matrix_api, MatrixAPICallback cb, void *cb_target, const gchar *room_id, Matrix3PidCredential *credential, GError **error)
{
    gchar *path;
    JsonNode *body;

    g_return_if_fail(room_id != NULL);
    g_return_if_fail(credential != NULL);

    uri_encode(room_id);
    path = g_strconcat("rooms/", enc_room_id, "/invite", NULL);
    g_free(enc_room_id);

    if ((body = matrix_json_compact_get_json_node(MATRIX_JSON_COMPACT(credential), error)) == NULL) {
        return;
    }

    _matrix_http_api_send(MATRIX_HTTP_API(matrix_api), cb, cb_target, CALL_TYPE_API, "POST", path, NULL, NULL, body, NULL, FALSE, error);

    g_free(path);
    json_node_unref(body);
}

static void
matrix_http_api_invite_user(MatrixAPI *matrix_api, MatrixAPICallback cb, void *cb_target, const gchar *room_id, const gchar *user_id, GError **error)
{
    gchar *path;
    JsonBuilder *builder;
    JsonNode *root_node;

    g_return_if_fail(room_id != NULL);
    g_return_if_fail(user_id != NULL);

    uri_encode(room_id);
    path = g_strconcat("rooms/", enc_room_id, "/invite", NULL);
    g_free(enc_room_id);

    builder = json_builder_new();

    json_builder_begin_object(builder);

    json_builder_set_member_name(builder, "user_id");
    json_builder_add_string_value(builder, user_id);

    json_builder_end_object(builder);
    root_node = json_builder_get_root(builder);

    _matrix_http_api_send(MATRIX_HTTP_API(matrix_api), cb, cb_target, CALL_TYPE_API, "POST", path, NULL, NULL, root_node, NULL, FALSE, error);

    g_free(path);
    json_node_unref(root_node);
    g_object_unref(builder);
}

static void
matrix_http_api_join_room(MatrixAPI *matrix_api, MatrixAPICallback cb, void *cb_target, const gchar *room_id, GError **error)
{
    gchar *path;

    g_return_if_fail(room_id != NULL);

    uri_encode(room_id);
    path = g_strconcat("rooms/", enc_room_id, "/join", NULL);
    g_free(enc_room_id);

    _matrix_http_api_send(MATRIX_HTTP_API(matrix_api), cb, cb_target, CALL_TYPE_API, "POST", path, NULL, NULL, NULL, NULL, FALSE, error);

    g_free(path);
}

static void
matrix_http_api_leave_room(MatrixAPI *matrix_api, MatrixAPICallback cb, void *cb_target, const gchar *room_id, GError **error)
{
    gchar *path;

    g_return_if_fail(room_id != NULL);

    uri_encode(room_id);
    path = g_strconcat("rooms/", enc_room_id, "/leave", NULL);
    g_free(enc_room_id);

    _matrix_http_api_send(MATRIX_HTTP_API(matrix_api), cb, cb_target, CALL_TYPE_API, "POST", path, NULL, NULL, NULL, NULL, FALSE, error);

    g_free(path);
}

static void
matrix_http_api_join_room_id_or_alias(MatrixAPI *matrix_api, MatrixAPICallback cb, void *cb_target, const gchar *room_id_or_alias, GError **error)
{
    gchar *path;

    g_return_if_fail(room_id_or_alias != NULL);

    uri_encode(room_id_or_alias);
    path = g_strconcat("join/", enc_room_id_or_alias, NULL);
    g_free(enc_room_id_or_alias);

    _matrix_http_api_send(MATRIX_HTTP_API(matrix_api), cb, cb_target, CALL_TYPE_API, "POST", path, NULL, NULL, NULL, NULL, FALSE, error);

    g_free(path);
}

static void
matrix_http_api_kick_user(MatrixAPI *matrix_api, MatrixAPICallback cb, void *cb_target, const gchar *room_id, const gchar *user_id, const gchar *reason, GError **error)
{
    gchar *path;
    JsonBuilder *builder;
    JsonNode *root_node;

    g_return_if_fail(room_id != NULL);
    g_return_if_fail(user_id != NULL);

    uri_encode(room_id);
    path = g_strconcat("rooms/", enc_room_id, "/kick", NULL);
    g_free(enc_room_id);

    builder = json_builder_new();

    json_builder_begin_object(builder);

    json_builder_set_member_name(builder, "user_id");
    json_builder_add_string_value(builder, user_id);

    if (reason != NULL) {
        json_builder_set_member_name(builder, "reason");
        json_builder_add_string_value(builder, reason);
    }

    json_builder_end_object(builder);
    root_node = json_builder_get_root(builder);

    _matrix_http_api_send(MATRIX_HTTP_API(matrix_api), cb, cb_target, CALL_TYPE_API, "POST", path, NULL, NULL, root_node, NULL, FALSE, error);

    g_free(path);
    json_node_unref(root_node);
    g_object_unref(builder);
}

static void
matrix_http_api_event_stream(MatrixAPI *matrix_api, MatrixAPICallback cb, void *cb_target, const gchar *from_token, gulong timeout, GError **error)
{
    GHashTable *parms = _matrix_http_api_create_query_params();

    if (from_token != NULL) {
        g_hash_table_replace(parms, g_strdup("from"), g_strdup(from_token));
    }

    if (timeout > 0) {
        g_hash_table_replace(parms, g_strdup("timeout"), g_strdup_printf("%lu", timeout));
    }

    _matrix_http_api_send(MATRIX_HTTP_API(matrix_api), cb, cb_target, CALL_TYPE_API, "GET", "events", parms, NULL, NULL, NULL, FALSE, error);

    g_hash_table_unref(parms);
}

static void
matrix_http_api_get_event(MatrixAPI *matrix_api, MatrixAPICallback cb, void *cb_target, const gchar *event_id, GError **error)
{
    gchar *path;

    g_return_if_fail(event_id != NULL);

    uri_encode(event_id);
    path = g_strconcat("events/", enc_event_id, NULL);
    g_free(enc_event_id);

    _matrix_http_api_send(MATRIX_HTTP_API(matrix_api), cb, cb_target, CALL_TYPE_API, "GET", path, NULL, NULL, NULL, NULL, FALSE, error);

    g_free(path);
}

static void
matrix_http_api_initial_sync(MatrixAPI *matrix_api, MatrixAPICallback cb, void *cb_target, guint limit, gboolean archived, GError **error)
{
    GHashTable *parms = _matrix_http_api_create_query_params();

    if (limit != 0) {
        g_hash_table_replace(parms, g_strdup("limit"), g_strdup_printf("%u", limit));
    }

    if (archived) {
        g_hash_table_replace(parms, g_strdup("archived"), g_strdup("true"));
    }

    _matrix_http_api_send(MATRIX_HTTP_API(matrix_api),
                          cb, cb_target,
                          CALL_TYPE_API, "GET", "initialSync",
                          parms, NULL, NULL, NULL, FALSE, error);

    g_hash_table_unref(parms);
}

static void
matrix_http_api_get_event_context(MatrixAPI *matrix_api, MatrixAPICallback cb, void *cb_target, const gchar *room_id, const gchar *event_id, guint limit, GError **error)
{
    gchar *path;
    GHashTable *parms = NULL;

    g_return_if_fail(room_id != NULL);
    g_return_if_fail(event_id != NULL);

    uri_encode(room_id);
    uri_encode(event_id);
    path = g_strconcat("rooms/", enc_room_id, "/context", enc_event_id, NULL);
    g_free(enc_room_id);
    g_free(enc_event_id);

    if (limit > 0) {
        parms = _matrix_http_api_create_query_params();

        g_hash_table_replace(parms, g_strdup("limit"), g_strdup_printf("%u", limit));
    }

    _matrix_http_api_send(MATRIX_HTTP_API(matrix_api),
                          cb, cb_target,
                          CALL_TYPE_API, "GET", path,
                          parms, NULL, NULL, NULL, FALSE, error);

    g_free(path);
    g_hash_table_unref(parms);
}

static void
matrix_http_api_initial_sync_room(MatrixAPI *matrix_api, MatrixAPICallback cb, void *cb_target, const gchar *room_id, GError **error)
{
    gchar *path;

    g_return_if_fail(room_id != NULL);

    uri_encode(room_id);
    path = g_strconcat("rooms/", enc_room_id, "/initialSync", NULL);
    g_free(enc_room_id);

    _matrix_http_api_send(MATRIX_HTTP_API(matrix_api),
                          cb, cb_target,
                          CALL_TYPE_API, "GET", path,
                          NULL, NULL, NULL, NULL, FALSE, error);

    g_free(path);
}

static void
matrix_http_api_list_room_members(MatrixAPI *matrix_api, MatrixAPICallback cb, void *cb_target, const gchar *room_id, GError **error)
{
    gchar *path;

    g_return_if_fail(room_id != NULL);

    uri_encode(room_id);
    path = g_strconcat("rooms/", enc_room_id, "/members", NULL);
    g_free(enc_room_id);

    _matrix_http_api_send(MATRIX_HTTP_API(matrix_api),
                          cb, cb_target,
                          CALL_TYPE_API, "GET", path,
                          NULL, NULL, NULL, NULL, FALSE, error);

    g_free(path);
}

static void
matrix_http_api_list_room_messages(MatrixAPI *matrix_api, MatrixAPICallback cb, void *cb_target, const gchar *room_id, const gchar *from_token, MatrixEventDirection direction, guint limit, GError **error)
{
    gchar *path;
    GHashTable *parms;

    g_return_if_fail(room_id != NULL);
    g_return_if_fail(from_token != NULL);

    uri_encode(room_id);
    path = g_strconcat("rooms/", enc_room_id, "/messages", NULL);
    g_free(enc_room_id);

    parms = _matrix_http_api_create_query_params();

    g_hash_table_replace(parms, g_strdup("from"), g_strdup(from_token));

    switch (direction) {
        case MATRIX_EVENT_DIRECTION_BACKWARD:
            g_hash_table_replace(parms, g_strdup("dir"), g_strdup("b"));

            break;
        case MATRIX_EVENT_DIRECTION_FORWARD:
            g_hash_table_replace(parms, g_strdup("dir"), g_strdup("f"));

            break;
    }

    if (limit > 0) {
        g_hash_table_replace(parms, g_strdup("limit"), g_strdup_printf("%u", limit));
    }

    _matrix_http_api_send(MATRIX_HTTP_API(matrix_api),
                          cb, cb_target,
                          CALL_TYPE_API, "GET", path,
                          parms, NULL, NULL, NULL, FALSE, error);

    g_free(path);
    g_hash_table_unref(parms);
}

static void
matrix_http_api_send_event_receipt(MatrixAPI *matrix_api, MatrixAPICallback cb, void *cb_target, const gchar *room_id, MatrixReceiptType receipt_type, const gchar *event_id, JsonNode *receipt, GError **error)
{
    gchar *path;
    gchar *type_str;

    g_return_if_fail(room_id != NULL);
    g_return_if_fail(event_id != NULL);
    g_return_if_fail(receipt != NULL);

    uri_encode(room_id);
    type_str = _matrix_g_enum_to_string(MATRIX_TYPE_RECEIPT_TYPE, receipt_type, '_');
    uri_encode(type_str);
    g_free(type_str);
    uri_encode(event_id);
    path = g_strconcat("rooms/", enc_room_id, "/receipt/", enc_type_str, "/", enc_event_id, NULL);
    g_free(enc_room_id);
    g_free(enc_type_str);
    g_free(enc_event_id);

    _matrix_http_api_send(MATRIX_HTTP_API(matrix_api),
                          cb, cb_target,
                          CALL_TYPE_API, "POST", path,
                          NULL, NULL, receipt, NULL, FALSE, error);

    g_free(path);
}

static void
matrix_http_api_redact_event(MatrixAPI *matrix_api, MatrixAPICallback cb, void *cb_target, const gchar *room_id, const gchar *event_id, const gchar *txn_id, const gchar *reason, GError **error)
{
    gchar *path;
    JsonNode *body = NULL;

    g_return_if_fail(room_id != NULL);
    g_return_if_fail(event_id != NULL);
    g_return_if_fail(txn_id != NULL);

    uri_encode(room_id);
    uri_encode(event_id);
    uri_encode(txn_id);
    path = g_strconcat("rooms/", enc_room_id, "/redact/", enc_event_id, "/", enc_txn_id, NULL);
    g_free(enc_room_id);
    g_free(enc_event_id);
    g_free(enc_txn_id);

    if (reason != NULL) {
        JsonBuilder *builder = json_builder_new();

        json_builder_begin_object(builder);

        json_builder_set_member_name(builder, "reason");
        json_builder_add_string_value(builder, reason);

        json_builder_end_object(builder);
        body = json_builder_get_root(builder);
    }

    _matrix_http_api_send(MATRIX_HTTP_API(matrix_api),
                          cb, cb_target,
                          CALL_TYPE_API, "PUT", path,
                          NULL, NULL, body, NULL, FALSE, error);

    g_free(path);
    json_node_unref(body);
}

static void
matrix_http_api_send_event(MatrixAPI *matrix_api, MatrixAPICallback cb, void *cb_target, const gchar *room_id, const gchar *event_type, const gchar *txn_id, JsonNode *content, GError **error)
{
    gchar *path;

    g_return_if_fail(room_id != NULL);
    g_return_if_fail(event_type != NULL);
    g_return_if_fail(txn_id != NULL);
    g_return_if_fail(content != NULL);

    uri_encode(room_id);
    uri_encode(event_type);
    uri_encode(txn_id);
    path = g_strconcat("rooms/", enc_room_id, "/send/", enc_event_type, "/", enc_txn_id, NULL);
    g_free(enc_room_id);
    g_free(enc_event_type);
    g_free(enc_txn_id);

    _matrix_http_api_send(MATRIX_HTTP_API(matrix_api),
                          cb, cb_target,
                          CALL_TYPE_API, "PUT", path,
                          NULL, NULL, content, NULL, FALSE, error);

    g_free(path);
}

static void
matrix_http_api_get_room_state(MatrixAPI *matrix_api, MatrixAPICallback cb, void *cb_target, const gchar *room_id, const gchar *event_type, const gchar *state_key, GError **error)
{
    gchar *path;

    g_return_if_fail(room_id != NULL);

    uri_encode(room_id);
    path = g_strconcat("rooms/", enc_room_id, "/state", NULL);
    g_free(enc_room_id);

    if (event_type != NULL) {
        gchar *tmp = path;

        uri_encode(event_type);
        path = g_strconcat(tmp, "/", enc_event_type, NULL);
        g_free(enc_event_type);
        g_free(tmp);
    }

    if (state_key != NULL) {
        gchar *tmp = path;

        uri_encode(state_key);
        path = g_strconcat(tmp, "/", enc_state_key, NULL);
        g_free(enc_state_key);
        g_free(tmp);
    }

    _matrix_http_api_send(MATRIX_HTTP_API(matrix_api),
                          cb, cb_target,
                          CALL_TYPE_API, "GET", path,
                          NULL, NULL, NULL, NULL, FALSE, error);

    g_free(path);
}

static void
matrix_http_api_send_state_event(MatrixAPI *matrix_api, MatrixAPICallback cb, void *cb_target, const gchar *room_id, const gchar *event_type, const gchar *state_key, JsonNode *content, GError **error)
{
    gchar *path;

    g_return_if_fail(room_id != NULL);
    g_return_if_fail(event_type != NULL);
    g_return_if_fail(content != NULL);

    uri_encode(room_id);
    uri_encode(event_type);
    path = g_strconcat("rooms/", enc_room_id, "/state/", enc_event_type, NULL);
    g_free(enc_room_id);
    g_free(enc_event_type);

    if (state_key != NULL) {
        gchar *tmp = path;

        uri_encode(state_key);
        path = g_strconcat(tmp, "/", enc_state_key, NULL);
        g_free(enc_state_key);
        g_free(tmp);
    }

    _matrix_http_api_send(MATRIX_HTTP_API(matrix_api),
                          cb, cb_target,
                          CALL_TYPE_API, "PUT", path,
                          NULL, NULL, content, NULL, FALSE, error);
}

static void
matrix_http_api_notify_room_typing(MatrixAPI *matrix_api, MatrixAPICallback cb, void *cb_target, const gchar *user_id, const gchar *room_id, guint timeout, gboolean typing, GError **error)
{
    gchar *path;
    JsonBuilder *builder;
    JsonNode *root_node;

    g_return_if_fail(user_id != NULL);
    g_return_if_fail(room_id != NULL);

    uri_encode(room_id);
    uri_encode(user_id);

    path = g_strconcat("rooms/", enc_room_id, "/typing/", enc_user_id, NULL);

    g_free(enc_room_id);
    g_free(enc_user_id);

    builder = json_builder_new();

    json_builder_begin_object(builder);

    if (timeout > 0) {
        json_builder_set_member_name(builder, "timeout");
        json_builder_add_int_value(builder, timeout);
    }

    json_builder_set_member_name(builder, "typing");
    json_builder_add_boolean_value(builder, typing);

    json_builder_end_object(builder);
    root_node = json_builder_get_root(builder);

    _matrix_http_api_send(MATRIX_HTTP_API(matrix_api),
                          cb, cb_target,
                          CALL_TYPE_API, "PUT", path,
                          NULL, NULL, root_node, NULL, FALSE, error);

    g_free(path);
    json_node_unref(root_node);
    g_object_unref(builder);
}

static void
matrix_http_api_sync(MatrixAPI *matrix_api, MatrixAPICallback cb, void *cb_target, const gchar *filter_id, MatrixFilter *filter, const gchar *since, gboolean full_state, gboolean set_presence, gulong timeout, GError **error)
{
    GHashTable *parms;

    if ((filter_id != NULL) && (filter != NULL)) {
        g_set_error(error, MATRIX_ERROR, MATRIX_ERROR_BAD_REQUEST,
                    "Cannot set both filter_id and filter");

        return;
    }

    parms = _matrix_http_api_create_query_params();

    if (filter_id != NULL) {
        g_hash_table_replace(parms, g_strdup("filter"), g_strdup(filter_id));
    }

    if (filter != NULL) {
        GError *inner_error = NULL;
        gchar *filter_data = matrix_json_compact_get_json_data(MATRIX_JSON_COMPACT(filter), NULL, &inner_error);

        if (inner_error != NULL) {
            g_propagate_error(error, inner_error);
            g_hash_table_unref(parms);

            return;
        }

        g_hash_table_replace(parms, g_strdup("filter"), filter_data);
    }

    if (since != NULL) {
        g_hash_table_replace(parms, g_strdup("since"), g_strdup(since));
    }

    g_hash_table_replace(parms, g_strdup("full_state"), g_strdup((full_state) ? "true" : "false"));

    if (!set_presence) {
        g_hash_table_replace(parms, g_strdup("set_presence"), g_strdup("offline"));
    }

    if (timeout != 0) {
        g_hash_table_replace(parms, g_strdup("timeout"), g_strdup_printf("%lu", timeout));
    }

    _matrix_http_api_send(MATRIX_HTTP_API(matrix_api),
                          cb, cb_target,
                          CALL_TYPE_API, "GET", "sync",
                          parms, NULL, NULL, NULL, FALSE, error);

    g_hash_table_unref(parms);
}

static void
matrix_http_api_create_filter(MatrixAPI *matrix_api, MatrixAPICallback cb, void *cb_target, const gchar *user_id, MatrixFilter *filter, GError **error)
{
    gchar *path;
    JsonNode *filter_node;
    GError *inner_error = NULL;

    g_return_if_fail(user_id != NULL);
    g_return_if_fail(filter != NULL);

    filter_node = matrix_json_compact_get_json_node(MATRIX_JSON_COMPACT(filter), &inner_error);

    if (inner_error != NULL) {
        g_propagate_error(error, inner_error);

        return;
    }

    uri_encode(user_id);
    path = g_strconcat("user/", enc_user_id, "/filter", NULL);
    g_free(enc_user_id);

    _matrix_http_api_send(MATRIX_HTTP_API(matrix_api),
                          cb, cb_target,
                          CALL_TYPE_API, "POST", path,
                          NULL, NULL, filter_node, NULL, FALSE, error);

    json_node_unref(filter_node);
    g_free(path);
}

static void
matrix_http_api_download_filter(MatrixAPI *matrix_api, MatrixAPICallback cb, void *cb_target, const gchar *user_id, const gchar *filter_id, GError **error)
{
    gchar *path;

    g_return_if_fail(user_id != NULL);
    g_return_if_fail(filter_id != NULL);

    uri_encode(user_id);
    uri_encode(filter_id);
    path = g_strconcat("user/", enc_user_id, "/filter/", enc_filter_id, NULL);
    g_free(enc_user_id);
    g_free(enc_filter_id);

    _matrix_http_api_send(MATRIX_HTTP_API(matrix_api),
                          cb, cb_target,
                          CALL_TYPE_API, "GET", path,
                          NULL, NULL, NULL, NULL, FALSE, error);

    g_free(path);
}

static void
matrix_http_api_search(MatrixAPI *matrix_api, MatrixAPICallback cb, void *cb_target, const gchar *next_batch, MatrixSearchCategories *search_categories, GError **error)
{
    GHashTable *parms = NULL;
    JsonNode *search_node;
    GError *inner_error = NULL;

    g_return_if_fail(search_categories != NULL);

    search_node = matrix_json_compact_get_json_node(MATRIX_JSON_COMPACT(search_categories), &inner_error);

    if (inner_error != NULL) {
        g_propagate_error(error, inner_error);

        return;
    }

    if (next_batch != NULL) {
        parms = _matrix_http_api_create_query_params();

        g_hash_table_replace(parms, g_strdup("next_batch"), g_strdup(next_batch));
    }

    _matrix_http_api_send(MATRIX_HTTP_API(matrix_api),
                          cb, cb_target,
                          CALL_TYPE_API, "POST", "search",
                          parms, NULL, search_node, NULL, FALSE, error);

    json_node_unref(search_node);
    g_hash_table_unref(parms);
}

static void
matrix_http_api_whois(MatrixAPI *matrix_api, MatrixAPICallback cb, void *cb_target, const gchar *user_id, GError **error)
{
    gchar *path;

    g_return_if_fail(user_id != NULL);

    uri_encode(user_id);
    path = g_strconcat("admin/whois/", enc_user_id, NULL);
    g_free(enc_user_id);

    _matrix_http_api_send(MATRIX_HTTP_API(matrix_api),
                          cb, cb_target,
                          CALL_TYPE_API, "GET", path,
                          NULL, NULL, NULL, NULL, FALSE, error);

    g_free(path);
}

static void
matrix_http_api_versions(MatrixAPI *matrix_api, MatrixAPICallback cb, void *cb_target, GError **error)
{
    _matrix_http_api_send(MATRIX_HTTP_API(matrix_api),
                          cb, cb_target,
                          CALL_TYPE_API, "GET", "versions",
                          NULL, NULL, NULL, NULL, FALSE, error);
}

static void
matrix_http_api_login(MatrixAPI *matrix_api, MatrixAPICallback cb, void *cb_target, const gchar *login_type, JsonNode *content, GError **error)
{
    JsonNode *body;
    JsonObject *root;

    g_return_if_fail(login_type != NULL);

    body = _matrix_json_node_deep_copy(content);
    root = json_node_get_object(body);

    json_object_set_string_member(root, "type", login_type);

    _matrix_http_api_send(MATRIX_HTTP_API(matrix_api),
                          cb, cb_target,
                          CALL_TYPE_API, "POST", "login",
                          NULL, NULL, body, NULL, FALSE, error);

    json_node_unref(body);
}

static void
matrix_http_api_logout(MatrixAPI *matrix_api, MatrixAPICallback cb, void *cb_target, GError **error)
{
    _matrix_http_api_send(MATRIX_HTTP_API(matrix_api),
                          cb, cb_target,
                          CALL_TYPE_API, "POST", "logout",
                          NULL, NULL, NULL, NULL, FALSE, error);
}

static void
matrix_http_api_get_3pids(MatrixAPI *matrix_api, MatrixAPICallback cb, void *cb_target, GError **error)
{
    _matrix_http_api_send(MATRIX_HTTP_API(matrix_api),
                          cb, cb_target,
                          CALL_TYPE_API, "GET", "account/3pid",
                          NULL, NULL, NULL, NULL, FALSE, error);
}

static void
matrix_http_api_add_3pid(MatrixAPI *matrix_api, MatrixAPICallback cb, void *cb_target, gboolean bind_creds, Matrix3PidCredential *creds, GError **error)
{
    JsonNode *creds_node;
    JsonNode *root_node;
    JsonBuilder *builder;
    GError *inner_error = NULL;

    g_return_if_fail(creds != NULL);

    creds_node = matrix_json_compact_get_json_node(MATRIX_JSON_COMPACT(creds), &inner_error);

    if (inner_error != NULL) {
        g_propagate_error(error, inner_error);

        return;
    }

    builder = json_builder_new();

    json_builder_begin_object(builder);

    json_builder_set_member_name(builder, "bind");
    json_builder_add_boolean_value(builder, bind_creds);

    json_builder_set_member_name(builder, "threePidCreds");
    json_builder_add_value(builder, creds_node);

    json_builder_end_object(builder);

    root_node = json_builder_get_root(builder);

    _matrix_http_api_send(MATRIX_HTTP_API(matrix_api),
                          cb, cb_target,
                          CALL_TYPE_API, "POST", "account/3pid",
                          NULL, NULL, root_node, NULL, FALSE, error);

    json_node_unref(root_node);
    g_object_unref(builder);
}

static void
matrix_http_api_change_password(MatrixAPI *matrix_api, MatrixAPICallback cb, void *cb_target, const gchar *new_password, GError **error)
{
    JsonBuilder *builder;
    JsonNode *root_node;

    g_return_if_fail(new_password != NULL);

    builder = json_builder_new();

    json_builder_begin_object(builder);

    json_builder_set_member_name(builder, "new_password");
    json_builder_add_string_value(builder, new_password);

    json_builder_end_object(builder);
    root_node = json_builder_get_root(builder);

    _matrix_http_api_send(MATRIX_HTTP_API(matrix_api),
                          cb, cb_target,
                          CALL_TYPE_API, "POST", "account/password",
                          NULL, NULL, root_node, NULL, FALSE, error);

    json_node_unref(root_node);
    g_object_unref(builder);
}

static void
matrix_http_api_get_profile(MatrixAPI *matrix_api, MatrixAPICallback cb, void *cb_target, const gchar *user_id, GError **error)
{
    gchar *path;

    g_return_if_fail(user_id != NULL);

    uri_encode(user_id);
    path = g_strconcat("profile/", enc_user_id, NULL);
    g_free(enc_user_id);

    _matrix_http_api_send(MATRIX_HTTP_API(matrix_api),
                          cb, cb_target,
                          CALL_TYPE_API, "GET", path,
                          NULL, NULL, NULL, NULL, FALSE, error);

    g_free(path);
}

static void
matrix_http_api_get_avatar_url(MatrixAPI *matrix_api, MatrixAPICallback cb, void *cb_target, const gchar *user_id, GError **error)
{
    gchar *path;

    g_return_if_fail(user_id != NULL);

    uri_encode(user_id);
    path = g_strconcat("profile/", enc_user_id, "/avatar_url", NULL);
    g_free(enc_user_id);

    _matrix_http_api_send(MATRIX_HTTP_API(matrix_api),
                          cb, cb_target,
                          CALL_TYPE_API, "GET", path,
                          NULL, NULL, NULL, NULL, FALSE, error);

    g_free(path);
}

static void
matrix_http_api_set_avatar_url(MatrixAPI *matrix_api, MatrixAPICallback cb, void *cb_target, const gchar *user_id, const gchar *avatar_url, GError **error)
{
    gchar *path;
    JsonBuilder *builder;
    JsonNode *root_node;

    g_return_if_fail(user_id != NULL);
    g_return_if_fail(avatar_url != NULL);

    uri_encode(user_id);
    path = g_strconcat("profile/", enc_user_id, "/avatar_url", NULL);
    g_free(enc_user_id);

    builder = json_builder_new();

    json_builder_begin_object(builder);

    json_builder_set_member_name(builder, "avatar_url");
    json_builder_add_string_value(builder, avatar_url);

    json_builder_end_object(builder);

    root_node = json_builder_get_root(builder);

    _matrix_http_api_send(MATRIX_HTTP_API(matrix_api),
                          cb, cb_target,
                          CALL_TYPE_API, "PUT", path,
                          NULL, NULL, root_node, NULL, FALSE, error);

    json_node_unref(root_node);
    g_object_unref(builder);
    g_free(path);
}

static void
matrix_http_api_get_display_name(MatrixAPI *matrix_api, MatrixAPICallback cb, void *cb_target, const gchar *user_id, GError **error)
{
    gchar *path;

    g_return_if_fail(user_id != NULL);

    uri_encode(user_id);
    path = g_strconcat("profile/", enc_user_id, "/displayname", NULL);
    g_free(enc_user_id);

    _matrix_http_api_send(MATRIX_HTTP_API(matrix_api),
                          cb, cb_target,
                          CALL_TYPE_API, "GET", path,
                          NULL, NULL, NULL, NULL, FALSE, error);

    g_free(path);
}

static void
matrix_http_api_set_display_name(MatrixAPI *matrix_api, MatrixAPICallback cb, void *cb_target, const gchar *user_name, const gchar *display_name, GError **error)
{
    gchar *path;
    JsonBuilder *builder;
    JsonNode *root_node;

    g_return_if_fail(user_name != NULL);
    g_return_if_fail(display_name != NULL);

    uri_encode(user_name);
    path = g_strconcat("profile/", enc_user_name, "/displayname", NULL);
    g_free(enc_user_name);

    builder = json_builder_new();

    json_builder_begin_object(builder);

    json_builder_set_member_name(builder, "displayname");
    json_builder_add_string_value(builder, display_name);

    json_builder_end_object(builder);
    root_node = json_builder_get_root(builder);

    _matrix_http_api_send(MATRIX_HTTP_API(matrix_api),
                          cb, cb_target,
                          CALL_TYPE_API, "PUT", path,
                          NULL, NULL, root_node, NULL, FALSE, error);

    json_node_unref(root_node);
    g_object_unref(builder);
    g_free(path);
}

static void
matrix_http_api_register_account(MatrixAPI *matrix_api, MatrixAPICallback cb, void *cb_target, MatrixAccountKind account_kind, gboolean bind_email, const gchar *username, const gchar *password, GError **error)
{
    GHashTable *parms = NULL;
    JsonBuilder *builder;
    JsonNode *root_node;

    g_return_if_fail(password != NULL);

    builder = json_builder_new();

    json_builder_begin_object(builder);

    json_builder_set_member_name(builder, "bind_email");
    json_builder_add_boolean_value(builder, bind_email);

    if (username != NULL) {
        json_builder_set_member_name(builder, "username");
        json_builder_add_string_value(builder, username);
    }

    json_builder_set_member_name(builder, "password");
    json_builder_add_string_value(builder, password);

    json_builder_end_object(builder);
    root_node = json_builder_get_root(builder);

    if (account_kind != MATRIX_ACCOUNT_KIND_DEFAULT) {
        gchar *account_kind_str = _matrix_g_enum_to_string(MATRIX_TYPE_ACCOUNT_KIND, account_kind, '_');

        if (account_kind_str != NULL) {
            parms = _matrix_http_api_create_query_params();

            g_hash_table_replace(parms, g_strdup("kind"), account_kind_str);
        }
    }

    _matrix_http_api_send(MATRIX_HTTP_API(matrix_api),
                          cb, cb_target,
                          CALL_TYPE_API, "POST", "register",
                          parms, NULL, root_node, NULL, FALSE, error);

    json_node_unref(root_node);
    g_object_unref(builder);
    g_hash_table_unref(parms);
}

static void
register_account_email(MatrixAPI *api,
                       MatrixAPICallback callback,
                       gpointer user_data,
                       const gchar *id_server,
                       const gchar *client_secret,
                       const gchar *email,
                       guint send_attempt,
                       GError **error)
{
    JsonBuilder *builder;
    JsonNode *root_node;

    builder = json_builder_new();

    json_builder_begin_object(builder);

    json_builder_set_member_name(builder, "id_server");
    json_builder_add_string_value(builder, id_server);

    json_builder_set_member_name(builder, "client_secret");
    json_builder_add_string_value(builder, client_secret);

    json_builder_set_member_name(builder, "email");
    json_builder_add_string_value(builder, email);

    json_builder_set_member_name(builder, "send_attempt");
    json_builder_add_int_value(builder, send_attempt);

    json_builder_end_object(builder);

    root_node = json_builder_get_root(builder);

    _matrix_http_api_send(MATRIX_HTTP_API(api),
                          callback, user_data,
                          CALL_TYPE_API, "POST", "register/email/requestToken",
                          NULL, NULL, root_node, NULL, FALSE, error);

    json_node_unref(root_node);
    g_object_unref(builder);
}

static void
matrix_http_api_set_account_data(MatrixAPI *matrix_api, MatrixAPICallback cb, void *cb_target, const gchar *user_id, const gchar *room_id, const gchar *event_type, JsonNode *content, GError **error)
{
    gchar *path;

    g_return_if_fail(user_id != NULL);
    g_return_if_fail(event_type != NULL);
    g_return_if_fail(content != NULL);

    uri_encode(user_id);
    uri_encode(event_type);

    if (room_id != NULL) {
        uri_encode(room_id);
        path = g_strconcat("user/", enc_user_id, "/rooms/", enc_room_id, "/account_data/", enc_event_type, NULL);
        g_free(enc_room_id);
    } else {
        path = g_strconcat("user/", enc_user_id, "/account_data/", enc_event_type, NULL);
    }

    g_free(enc_user_id);
    g_free(enc_event_type);

    _matrix_http_api_send(MATRIX_HTTP_API(matrix_api),
                          cb, cb_target,
                          CALL_TYPE_API, "PUT", path,
                          NULL, NULL, content, NULL, FALSE, error);

    g_free(path);
}

static void
matrix_http_api_get_room_tags(MatrixAPI *matrix_api, MatrixAPICallback cb, void *cb_target, const gchar *user_id, const gchar *room_id, GError **error)
{
    gchar *path;

    g_return_if_fail(user_id != NULL);
    g_return_if_fail(room_id != NULL);

    uri_encode(user_id);
    uri_encode(room_id);
    path = g_strconcat("user/", enc_user_id, "/rooms/", enc_room_id, "/tags", NULL);
    g_free(enc_user_id);
    g_free(enc_room_id);

    _matrix_http_api_send(MATRIX_HTTP_API(matrix_api),
                          cb, cb_target,
                          CALL_TYPE_API, "GET", path,
                          NULL, NULL, NULL, NULL, FALSE, error);

    g_free(path);
}

static void
matrix_http_api_delete_room_tag(MatrixAPI *matrix_api, MatrixAPICallback cb, void *cb_target, const gchar *user_id, const gchar *room_id, const gchar *tag, GError **error)
{
    gchar *path;

    g_return_if_fail(user_id != NULL);
    g_return_if_fail(room_id != NULL);
    g_return_if_fail(tag != NULL);

    uri_encode(user_id);
    uri_encode(room_id);
    uri_encode(tag);
    path = g_strconcat("user/", enc_user_id, "/rooms/", enc_room_id, "/tags/", enc_tag, NULL);
    g_free(enc_user_id);
    g_free(enc_room_id);
    g_free(enc_tag);

    _matrix_http_api_send(MATRIX_HTTP_API(matrix_api),
                          cb, cb_target,
                          CALL_TYPE_API, "DELETE", path,
                          NULL, NULL, NULL, NULL, FALSE, error);

    g_free(path);
}

static void
matrix_http_api_add_room_tag(MatrixAPI *matrix_api, MatrixAPICallback cb, void *cb_target, const gchar *user_id, const gchar *room_id, const gchar *tag, JsonNode *content, GError **error)
{
    gchar *path;

    g_return_if_fail(user_id != NULL);
    g_return_if_fail(room_id != NULL);
    g_return_if_fail(tag != NULL);

    uri_encode(user_id);
    uri_encode(room_id);
    uri_encode(tag);

    path = g_strconcat("user/", enc_user_id, "/rooms/", enc_room_id, "/tags/", enc_tag, NULL);

    g_free(enc_user_id);
    g_free(enc_room_id);
    g_free(enc_tag);

    _matrix_http_api_send(MATRIX_HTTP_API(matrix_api),
                          cb, cb_target,
                          CALL_TYPE_API, "PUT", path,
                          NULL, NULL, content, NULL, FALSE, error);
}

static void
matrix_http_api_deactivate_account(MatrixAPI *matrix_api, MatrixAPICallback cb, void *cb_target, const gchar *session, const gchar *login_type, GError **error)
{
    JsonNode *root_node = NULL;

    if (login_type != NULL) {
        JsonBuilder *builder = json_builder_new();

        json_builder_begin_object(builder);

        json_builder_set_member_name(builder, "auth");
        json_builder_begin_object(builder);

        if (session != NULL) {
            json_builder_set_member_name(builder, "session");
            json_builder_add_string_value(builder, session);
        }

        json_builder_set_member_name(builder, "type");
        json_builder_add_string_value(builder, login_type);

        json_builder_end_object(builder);
        json_builder_end_object(builder);

        root_node = json_builder_get_root(builder);

        g_object_unref(builder);
    }

    _matrix_http_api_send(MATRIX_HTTP_API(matrix_api),
                          cb, cb_target,
                          CALL_TYPE_API, "POST", "account/deactivate",
                          NULL, NULL, root_node, NULL, FALSE, error);

    json_node_unref(root_node);
}

static void
matrix_http_api_get_turn_server(MatrixAPI *matrix_api, MatrixAPICallback cb, void *cb_target, GError **error)
{
    _matrix_http_api_send(MATRIX_HTTP_API(matrix_api),
                          cb, cb_target,
                          CALL_TYPE_API, "GET", "voip/turnServer",
                          NULL, NULL, NULL, NULL, FALSE, error);
}

static void
whoami(MatrixAPI *api, MatrixAPICallback cb, void *cb_target, GError **error)
{
    _matrix_http_api_send(MATRIX_HTTP_API(api),
                          cb, cb_target,
                          CALL_TYPE_API, "GET", "account/whoami",
                          NULL, NULL, NULL, NULL, FALSE, error);
}

static void
matrix_http_api_abort_pending (MatrixAPI *matrix_api)
{
    MatrixHTTPAPIPrivate *priv = matrix_http_api_get_instance_private(MATRIX_HTTP_API(matrix_api));

    soup_session_abort(priv->soup_session);
}

const gchar *
matrix_http_api_get_base_url (MatrixHTTPAPI *matrix_http_api)
{
    MatrixHTTPAPIPrivate *priv;

    g_return_val_if_fail(matrix_http_api != NULL, NULL);

    priv = matrix_http_api_get_instance_private(matrix_http_api);

    return priv->base_url;
}

static gint
string_last_index_of(const gchar *str, const gchar *needle, gint start_index)
{
    gchar *occurence = NULL;

    g_return_val_if_fail(str != NULL, -1);
    g_return_val_if_fail(needle != NULL, -1);

    if ((occurence = g_strrstr (str + start_index, needle)) != NULL){
        return (gint)(occurence - ((gchar*)str));
    }

    return -1;
}

void
matrix_http_api_set_base_url(MatrixHTTPAPI *matrix_http_api, const gchar *base_url)
{
    MatrixHTTPAPIPrivate *priv;
    SoupURI *api_uri;
    SoupURI *media_uri;

    g_return_if_fail(matrix_http_api != NULL);

    priv = matrix_http_api_get_instance_private(matrix_http_api);

    if (!g_str_is_ascii(base_url)) {
        g_warning("URL specified(%s) is not ASCII", base_url);

        return;
    }

    if (string_last_index_of(base_url, API_ENDPOINT, 0) != -1) {
        g_warning("Provided URL (%s) already contains the API endpoint. Please use an URL without it!",
                  base_url);

        return;
    }

    _matrix_http_api_set_url(matrix_http_api, &api_uri, base_url, API_ENDPOINT);
    _matrix_http_api_set_url(matrix_http_api, &media_uri, base_url, MEDIA_ENDPOINT);

    if ((api_uri != NULL) && (media_uri != NULL)) {
        if (priv->api_uri != NULL) {
            soup_uri_free(priv->api_uri);
        }

        if (priv->media_uri != NULL) {
            soup_uri_free(priv->media_uri);
        }

        g_free(priv->base_url);

        priv->api_uri = api_uri;
        priv->media_uri = media_uri;
        priv->base_url = g_strdup(base_url);

        g_free(priv->token);
        priv->token = NULL;
        g_free(priv->homeserver);
        g_free(priv->user_id);
        priv->homeserver = NULL;
        priv->user_id = NULL;

#if DEBUG
        gchar *uri;

        uri = soup_uri_to_string(api_uri, FALSE);
        g_debug("API URL: %s", uri);
        g_free(uri);

        uri = soup_uri_to_string(media_uri, FALSE);
        g_debug("Media URL: %s", uri);
        g_free(uri);
#endif
    } else {
        g_warning("Invalid base URL: %s", base_url);
    }

    g_object_notify_by_pspec((GObject *)matrix_http_api, matrix_http_api_properties[PROP_BASE_URL]);
}

gboolean
matrix_http_api_get_validate_certificate(MatrixHTTPAPI *matrix_http_api)
{
    MatrixHTTPAPIPrivate *priv;
    gboolean result;

    g_return_val_if_fail(matrix_http_api != NULL, FALSE);

    priv = matrix_http_api_get_instance_private(matrix_http_api);

    g_object_get(priv->soup_session, "ssl-strict", &result, NULL);

    return result;
}

void
matrix_http_api_set_validate_certificate(MatrixHTTPAPI *matrix_http_api, gboolean validate_certificate)
{
    MatrixHTTPAPIPrivate *priv;

    g_return_if_fail(matrix_http_api != NULL);

    priv = matrix_http_api_get_instance_private(matrix_http_api);

    g_object_set(priv->soup_session, "ssl-strict", validate_certificate, NULL);

    g_object_notify_by_pspec ((GObject *) matrix_http_api, matrix_http_api_properties[PROP_VALIDATE_CERTIFICATE]);
}

static const gchar *
matrix_http_api_get_user_id (MatrixAPI *api)
{
    MatrixHTTPAPIPrivate *priv = matrix_http_api_get_instance_private(MATRIX_HTTP_API(api));

    return priv->user_id;
}

static const gchar *
matrix_http_api_get_token(MatrixAPI *matrix_api)
{
    MatrixHTTPAPIPrivate *priv = matrix_http_api_get_instance_private(MATRIX_HTTP_API(matrix_api));

    return priv->token;
}

static void
matrix_http_api_set_token(MatrixAPI *matrix_api, const gchar *token)
{
    MatrixHTTPAPIPrivate *priv = matrix_http_api_get_instance_private(MATRIX_HTTP_API(matrix_api));

    if (g_strcmp0(token, priv->token) != 0) {
        g_free(priv->token);
        priv->token = g_strdup(token);

        g_object_notify_by_pspec((GObject *)matrix_api, matrix_http_api_properties[PROP_TOKEN]);
    }
}

static const gchar *
matrix_http_api_get_homeserver(MatrixAPI *api) {
    MatrixHTTPAPIPrivate *priv = matrix_http_api_get_instance_private(MATRIX_HTTP_API(api));

    return priv->homeserver;
}

static void
matrix_http_api_finalize(GObject *gobject)
{
    MatrixHTTPAPIPrivate *priv = matrix_http_api_get_instance_private(MATRIX_HTTP_API(gobject));

    g_object_unref(priv->soup_session);
    g_free(priv->base_url);

    if (priv->api_uri != NULL) {
        soup_uri_free(priv->api_uri);
    }

    if (priv->media_uri != NULL) {
        soup_uri_free(priv->media_uri);
    }

    g_free(priv->token);
    g_free(priv->user_id);
    g_free(priv->homeserver);

    G_OBJECT_CLASS(matrix_http_api_parent_class)->finalize(gobject);
}

static void
matrix_http_api_get_property(GObject *gobject, guint property_id, GValue *value, GParamSpec *pspec)
{
    MatrixHTTPAPI *matrix_http_api = MATRIX_HTTP_API(gobject);

    switch (property_id) {
        case PROP_BASE_URL:
            g_value_set_string(value, matrix_http_api_get_base_url(matrix_http_api));

            break;
        case PROP_VALIDATE_CERTIFICATE:
            g_value_set_boolean(value, matrix_http_api_get_validate_certificate(matrix_http_api));

            break;
        case PROP_USER_ID:
            g_value_set_string(value, matrix_api_get_user_id((MatrixAPI*) matrix_http_api));

            break;
        case PROP_TOKEN:
            g_value_set_string(value, matrix_api_get_token((MatrixAPI*) matrix_http_api));

            break;
        case PROP_HOMESERVER:
            g_value_set_string(value, matrix_api_get_homeserver((MatrixAPI*) matrix_http_api));

            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, property_id, pspec);

            break;
    }
}

static void
matrix_http_api_set_property(GObject *gobject, guint property_id, const GValue *value, GParamSpec *pspec)
{
    MatrixHTTPAPI * matrix_http_api = MATRIX_HTTP_API(gobject);

    switch(property_id) {
        case PROP_BASE_URL:
            matrix_http_api_set_base_url(matrix_http_api, g_value_get_string(value));

            break;
        case PROP_VALIDATE_CERTIFICATE:
            matrix_http_api_set_validate_certificate(matrix_http_api, g_value_get_boolean(value));

            break;
        case PROP_TOKEN:
            matrix_api_set_token((MatrixAPI*) matrix_http_api, g_value_get_string(value));

            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, property_id, pspec);

            break;
    }
}

static void
matrix_http_api_class_init(MatrixHTTPAPIClass *klass)
{
    G_OBJECT_CLASS(klass)->get_property = matrix_http_api_get_property;
    G_OBJECT_CLASS(klass)->set_property = matrix_http_api_set_property;
    G_OBJECT_CLASS(klass)->finalize = matrix_http_api_finalize;

    /**
     * MatrixHTTPAPI:base-url:
     *
     * The base URL of the API.
     */
    matrix_http_api_properties[PROP_BASE_URL] = g_param_spec_string(
            "base-url", "base-url", "base-url",
            NULL,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_BASE_URL, matrix_http_api_properties[PROP_BASE_URL]);

    /**
     * MatrixHTTPAPI:validate-certificate:
     *
     * If %TRUE (the default), the certificate will be checked; otherwise the library accepts
     * invalid (eg. self-signed) certificates.
     */
    matrix_http_api_properties[PROP_VALIDATE_CERTIFICATE] = g_param_spec_boolean(
            "validate-certificate", "validate-certificate", "validate-certificate",
            FALSE,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_VALIDATE_CERTIFICATE, matrix_http_api_properties[PROP_VALIDATE_CERTIFICATE]);

    matrix_http_api_properties[PROP_USER_ID] = g_param_spec_string(
            "user-id", "user-id", "user-id",
            NULL,
            G_PARAM_STATIC_STRINGS | G_PARAM_READABLE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_USER_ID, matrix_http_api_properties[PROP_USER_ID]);

    matrix_http_api_properties[PROP_TOKEN] = g_param_spec_string(
            "token", "token", "token",
            NULL,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_TOKEN, matrix_http_api_properties[PROP_TOKEN]);

    matrix_http_api_properties[PROP_HOMESERVER] = g_param_spec_string(
            "homeserver", "homeserver", "homeserver",
            NULL,
            G_PARAM_STATIC_STRINGS | G_PARAM_READABLE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_HOMESERVER, matrix_http_api_properties[PROP_HOMESERVER]);
}

static void
matrix_http_api_matrix_api_interface_init(MatrixAPIInterface * iface)
{
    iface->media_download = matrix_http_api_media_download;
    iface->media_thumbnail = matrix_http_api_media_thumbnail;
    iface->media_upload = matrix_http_api_media_upload;
    iface->get_presence_list = matrix_http_api_get_presence_list;
    iface->update_presence_list = matrix_http_api_update_presence_list;
    iface->get_presence = matrix_http_api_get_presence;
    iface->set_presence = matrix_http_api_set_presence;
    iface->update_pusher = matrix_http_api_update_pusher;
    iface->get_pushers = matrix_http_api_get_pushers;
    iface->delete_pushrule = matrix_http_api_delete_pushrule;
    iface->get_pushrule = matrix_http_api_get_pushrule;
    iface->add_pushrule = matrix_http_api_add_pushrule;
    iface->toggle_pushrule = matrix_http_api_toggle_pushrule;
    iface->get_pushrules = matrix_http_api_get_pushrules;
    iface->create_room = matrix_http_api_create_room;
    iface->delete_room_alias = matrix_http_api_delete_room_alias;
    iface->get_room_id = matrix_http_api_get_room_id;
    iface->create_room_alias = matrix_http_api_create_room_alias;
    iface->list_public_rooms = matrix_http_api_list_public_rooms;
    iface->ban_user = matrix_http_api_ban_user;
    iface->unban_user = matrix_http_api_unban_user;
    iface->forget_room = matrix_http_api_forget_room;
    iface->invite_user_3rdparty = matrix_http_api_invite_user_3rdparty;
    iface->invite_user = matrix_http_api_invite_user;
    iface->join_room = matrix_http_api_join_room;
    iface->leave_room = matrix_http_api_leave_room;
    iface->join_room_id_or_alias = matrix_http_api_join_room_id_or_alias;
    iface->kick_user = matrix_http_api_kick_user;
    iface->event_stream = matrix_http_api_event_stream;
    iface->get_event = matrix_http_api_get_event;
    iface->initial_sync = matrix_http_api_initial_sync;
    iface->get_event_context = matrix_http_api_get_event_context;
    iface->initial_sync_room = matrix_http_api_initial_sync_room;
    iface->list_room_members = matrix_http_api_list_room_members;
    iface->list_room_messages = matrix_http_api_list_room_messages;
    iface->send_event_receipt = matrix_http_api_send_event_receipt;
    iface->redact_event = matrix_http_api_redact_event;
    iface->send_event = matrix_http_api_send_event;
    iface->get_room_state = matrix_http_api_get_room_state;
    iface->send_state_event = matrix_http_api_send_state_event;
    iface->notify_room_typing = matrix_http_api_notify_room_typing;
    iface->sync = matrix_http_api_sync;
    iface->create_filter = matrix_http_api_create_filter;
    iface->download_filter = matrix_http_api_download_filter;
    iface->search = matrix_http_api_search;
    iface->whois = matrix_http_api_whois;
    iface->versions = matrix_http_api_versions;
    iface->login = matrix_http_api_login;
    iface->logout = matrix_http_api_logout;
    iface->get_3pids = matrix_http_api_get_3pids;
    iface->add_3pid = matrix_http_api_add_3pid;
    iface->change_password = matrix_http_api_change_password;
    iface->get_profile = matrix_http_api_get_profile;
    iface->get_avatar_url = matrix_http_api_get_avatar_url;
    iface->set_avatar_url = matrix_http_api_set_avatar_url;
    iface->get_display_name = matrix_http_api_get_display_name;
    iface->set_display_name = matrix_http_api_set_display_name;
    iface->register_account = matrix_http_api_register_account;
    iface->register_account_email = register_account_email;
    iface->set_account_data = matrix_http_api_set_account_data;
    iface->get_room_tags = matrix_http_api_get_room_tags;
    iface->delete_room_tag = matrix_http_api_delete_room_tag;
    iface->add_room_tag = matrix_http_api_add_room_tag;
    iface->deactivate_account = matrix_http_api_deactivate_account;
    iface->get_turn_server = matrix_http_api_get_turn_server;
    iface->abort_pending = matrix_http_api_abort_pending;
    iface->get_user_id = matrix_http_api_get_user_id;
    iface->get_token = matrix_http_api_get_token;
    iface->set_token = matrix_http_api_set_token;
    iface->get_homeserver = matrix_http_api_get_homeserver;
    iface->whoami = whoami;
}

static void
matrix_http_api_init(MatrixHTTPAPI *matrix_http_api)
{
    MatrixHTTPAPIPrivate *priv = matrix_http_api_get_instance_private(matrix_http_api);

    priv->soup_session = soup_session_new();
    priv->base_url = NULL;
    priv->api_uri = NULL;
    priv->media_uri = NULL;
    priv->token = NULL;
    priv->homeserver = NULL;
    priv->user_id = NULL;
}
