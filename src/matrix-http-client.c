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

#include "matrix-http-client.h"

/**
 * SECTION:matrix-http-client
 * @short_description: A HTTP based Matrix.org client
 * @title: MatrixHTTPClient
 * @stability: Unstable
 *
 * An event-driven client class to communicate with HTTP based
 * Matrix.org servers.
 */

/**
 * MatrixHTTPClient:
 *
 * The client instance definition.
 */

/**
 * MatrixHTTPClientClass:
 *
 * Class definition for #MatrixHTTPClient.
 */

typedef struct _MatrixHTTPClientPrivate {
    gboolean polling;
    guint event_timeout;
} MatrixHTTPClientPrivate;

static void matrix_http_client_matrix_client_init(MatrixClientInterface *iface);
static void i_begin_polling(MatrixClient *client, GError **error);

G_DEFINE_TYPE_WITH_CODE(MatrixHTTPClient, matrix_http_client, MATRIX_TYPE_HTTP_API,
                        G_ADD_PRIVATE(MatrixHTTPClient)
                        G_IMPLEMENT_INTERFACE(MATRIX_TYPE_CLIENT,
                                              matrix_http_client_matrix_client_init));

static void
cb_login(MatrixAPI *api,
         const gchar *content_type,
         JsonNode *json_content,
         GByteArray *raw_content,
         gpointer user_data,
         GError *error)
{
    matrix_client_login_finished(MATRIX_CLIENT(api), (error == NULL));
}

static void
i_login_with_password(MatrixClient *client,
                      const gchar *username,
                      const gchar *password,
                      GError **error)
{
    JsonBuilder *builder;
    JsonNode *body;

    builder = json_builder_new();
    json_builder_begin_object(builder);

    json_builder_set_member_name(builder, "user");
    json_builder_add_string_value(builder, username);

    json_builder_set_member_name(builder, "password");
    json_builder_add_string_value(builder, password);

    json_builder_end_object(builder);
    body = json_builder_get_root(builder);
    g_object_unref(builder);

    matrix_api_login(MATRIX_API(client),
                     cb_login, NULL,
                     "m.login.password", body,
                     error);
}

static void
cb_register_account(MatrixAPI *api,
                    const gchar *content_type,
                    JsonNode *json_content,
                    GByteArray *raw_content,
                    gpointer user_data,
                    GError *error)
{
    matrix_client_login_finished(MATRIX_CLIENT(api), (error == NULL));
}

static void
i_register_with_password(MatrixClient *client,
                         const gchar *username,
                         const gchar *password,
                         GError **error)
{
    matrix_api_register_account(MATRIX_API(client),
                                cb_register_account, NULL,
                                MATRIX_ACCOUNT_KIND_USER,
                                FALSE,
                                username, password,
                                error);
}

static void
i_logout(MatrixClient *client, GError **error)
{
    matrix_api_set_token(MATRIX_API(client), NULL);
    matrix_api_set_refresh_token(MATRIX_API(client), NULL);
    matrix_api_abort_pending(MATRIX_API(client));
}

static void
process_event(JsonArray *array,
              guint idx,
              JsonNode *event,
              MatrixClient *client)
{}

static void
cb_event_stream(MatrixAPI *api,
                const gchar *content_type,
                JsonNode *json_content,
                GByteArray *raw_content,
                gpointer user_data,
                GError *error)
{
    MatrixHTTPClientPrivate *priv = matrix_http_client_get_instance_private(
            MATRIX_HTTP_CLIENT(api));
    const gchar *end_token = NULL;

    if (!error) {
        JsonObject *root_obj;
        JsonNode *node;

        root_obj = json_node_get_object(json_content);

        if ((node = json_object_get_member(root_obj, "chunk")) != NULL) {
            JsonArray *chunks = json_node_get_array(node);

            json_array_foreach_element(chunks,
                                       (JsonArrayForeach)process_event,
                                       api);
        }

        if ((node = json_object_get_member(root_obj, "end")) != NULL) {
            end_token = json_node_get_string(node);
        }
    }

    // Only continue polling if polling is still enabled, and there
    // was no communication error during the last call
    if (priv->polling
        && (!error || error->code <= MATRIX_ERROR_MISSING_TOKEN)) {
        priv->polling = FALSE;

        matrix_api_event_stream(api,
                                cb_event_stream, NULL,
                                end_token, priv->event_timeout,
                                NULL);
    }
}

static void
i_begin_polling(MatrixClient *client, GError **error)
{
    MatrixHTTPClientPrivate *priv = matrix_http_client_get_instance_private(
            MATRIX_HTTP_CLIENT(client));
    GError *err = NULL;

    matrix_api_event_stream(MATRIX_API(client),
                            cb_event_stream, NULL,
                            NULL, priv->event_timeout,
                            &err);

    if (err) {
        g_propagate_error(error, err);

        return;
    }

    priv->polling = TRUE;
}

static void
i_stop_polling(MatrixClient *client, gboolean cancel_ongoing, GError **error)
{
    MatrixHTTPClientPrivate *priv = matrix_http_client_get_instance_private(
            MATRIX_HTTP_CLIENT(client));

    priv->polling = FALSE;

    if (cancel_ongoing) {
        matrix_api_abort_pending(MATRIX_API(client));
    }
}

static void
matrix_http_client_matrix_client_init(MatrixClientInterface *iface)
{
    iface->login_with_password = i_login_with_password;
    iface->register_with_password = i_register_with_password;
    iface->logout = i_logout;
    iface->begin_polling = i_begin_polling;
    iface->stop_polling = i_stop_polling;
}

static void
matrix_http_client_init(MatrixHTTPClient *client)
{
    MatrixHTTPClientPrivate *priv = matrix_http_client_get_instance_private(
            client);

    priv->polling = FALSE;
    priv->event_timeout = 30000;
}

static void
matrix_http_client_class_init(MatrixHTTPClientClass *klass)
{}

/**
 * matrix_http_client_new:
 * @base_url: the base URL to use for API communication
 *
 * Creates a new #MatrixHTTPClient object.
 *
 * Returns: (transfer full): a new #MatrixHTTPClient. The object is
 *          cast to #MatrixClient for easier interface usage.
 */
MatrixClient *
matrix_http_client_new(const gchar *base_url)
{
    MatrixHTTPClient *client;

    client = g_object_new(MATRIX_TYPE_HTTP_CLIENT,
                          "base-url", base_url,
                          NULL);

    return MATRIX_CLIENT(client);
}
