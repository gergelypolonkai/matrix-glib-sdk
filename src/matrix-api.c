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

#include "matrix-api.h"

#include <string.h>
#include <libsoup/soup.h>

/**
 * SECTION:matrix-api
 * @short_description: Low level API calls to communicate with a
 *                     Matrix.org server
 * @title: MatrixAPI
 * @stability: Unstable
 * @include: matrix-glib/matrix.h
 *
 * This is a class for low level communication with a Matrix.org
 * server.
 */

/**
 * MatrixAPI:
 *
 * The MatrixAPI object’s instance definition.
 */

/**
 * MatrixAPIClass:
 * @parent_class: the parent class structure (#GObjectClass)
 *
 * The MatrixAPI object’s class definition.
 */

#define API_ENDPOINT "/_matrix/client/api/v1"

typedef struct _MatrixAPIPrivate {
    SoupSession *soup_session;
    guint txn_id;
    gchar *url;
    gchar *token;
    gboolean validate_cert;
} MatrixAPIPrivate;

enum {
    PROP_URL = 1,
    N_PROPERTIES
};

GParamSpec *obj_properties[N_PROPERTIES] = {NULL,};

G_DEFINE_TYPE_WITH_PRIVATE(MatrixAPI, matrix_api, G_TYPE_OBJECT);

static void
matrix_api_finalize(GObject *gobject)
{
    g_signal_handlers_destroy(gobject);
    G_OBJECT_CLASS(matrix_api_parent_class)->finalize(gobject);
}

static void
matrix_api_set_property(GObject *gobject,
                        guint prop_id,
                        const GValue *value,
                        GParamSpec *pspec)
{
    MatrixAPI *api = MATRIX_API(gobject);
    MatrixAPIPrivate *priv = matrix_api_get_instance_private(api);

    switch (prop_id) {
        case PROP_URL:
        {
            const gchar *base_url;
            gchar *last_occurence;

            base_url = g_value_get_string(value);

            if (!g_str_is_ascii(base_url)) {
                g_warning("URL specified (%s) is not ASCII", base_url);

                return;
            }

            last_occurence = g_strrstr(base_url, API_ENDPOINT);

            if ((g_strcmp0(last_occurence, API_ENDPOINT) == 0) ||
                (g_strcmp0(last_occurence, API_ENDPOINT"/") == 0)) {
                priv->url = g_strdup(base_url);
            } else {
                priv->url = g_strdup_printf(
                        "%s%s%s",
                        base_url,
                        (base_url[strlen(base_url) - 1] == '/') ? "" : "/",
                        API_ENDPOINT);
            }

            break;
        }

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, pspec);
    }
}

static void
matrix_api_get_property(GObject *gobject,
                        guint prop_id,
                        GValue *value,
                        GParamSpec *pspec)
{
    MatrixAPI *api = MATRIX_API(gobject);
    MatrixAPIPrivate *priv = matrix_api_get_instance_private(api);

    switch (prop_id) {
        case PROP_URL:
            g_value_set_string(value, priv->url);

            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, pspec);
    }
}

static void
matrix_api_class_init(MatrixAPIClass *klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS(klass);

    gobject_class->set_property = matrix_api_set_property;
    gobject_class->get_property = matrix_api_get_property;
    gobject_class->finalize = matrix_api_finalize;

    /**
     * MatrixAPI:url:
     *
     * The base URL to use for communication with the Matrix.org
     * server.
     */
    obj_properties[PROP_URL] = g_param_spec_string(
            "url", "Server URL",
            "Matrix.org home server to connect to.",
            NULL,
            G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);

    g_object_class_install_properties(gobject_class,
                                N_PROPERTIES,
                                obj_properties);
}

static void
matrix_api_init(MatrixAPI *api)
{
    MatrixAPIPrivate *priv = matrix_api_get_instance_private(api);

    priv->txn_id = 0;
    priv->url = NULL;
    priv->token = NULL;
    priv->validate_cert = TRUE;
    priv->soup_session = soup_session_new_with_options(
            SOUP_SESSION_ADD_FEATURE_BY_TYPE, SOUP_TYPE_CONTENT_SNIFFER,
            NULL);
}

MatrixAPI *
matrix_api_new(const gchar *base_url, const gchar *token)
{
    return g_object_new(MATRIX_TYPE_API,
                        "base-url", base_url,
                        "token", token,
                        NULL);
}
