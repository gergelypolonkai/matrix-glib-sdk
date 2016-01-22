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

enum {
    PROP_BASE_URL = 1,
    PROP_VALIDATE_CERTIFICATE,
    N_PROPERTIES
};

static GParamSpec *obj_properties[N_PROPERTIES] = {NULL,};

static void matrix_http_client_matrix_client_init(MatrixClientInterface *iface);

G_DEFINE_TYPE_WITH_CODE(MatrixHTTPClient, matrix_http_client, MATRIX_TYPE_HTTP_API,
                        G_IMPLEMENT_INTERFACE(MATRIX_TYPE_CLIENT,
                                              matrix_http_client_matrix_client_init));

static void
matrix_http_client_matrix_client_init(MatrixClientInterface *iface)
{
    iface->login_with_password = NULL;
    iface->register_with_password = NULL;
    iface->logout = NULL;
    iface->begin_polling = NULL;
    iface->stop_polling = NULL;
}

static void
matrix_http_client_init(MatrixHTTPClient *client)
{}

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
