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

#ifndef __MATRIX_GLIB_SDK_HTTP_API_H__
# define __MATRIX_GLIB_SDK_HTTP_API_H__

# include <glib-object.h>
# include "matrix-api.h"

G_BEGIN_DECLS

# define MATRIX_TYPE_HTTP_API (matrix_http_api_get_type ())
# define MATRIX_HTTP_API(o) (G_TYPE_CHECK_INSTANCE_CAST((o), MATRIX_TYPE_HTTP_API, MatrixHTTPAPI))
# define MATRIX_HTTP_API_CLASS(c) (G_TYPE_CHECK_CLASS_CAST((c), MATRIX_TYPE_HTTP_API, MatrixHTTPAPIClass))
# define MATRIX_IS_HTTP_API(o) (G_TYPE_CHECK_INSTANCE_TYPE((o), MATRIX_TYPE_HTTP_API))
# define MATRIX_IS_HTTP_API_CLASS(c) (G_TYPE_CHECK_CLASS_TYPE((c), MATRIX_TYPE_HTTP_API))
# define MATRIX_HTTP_API_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS((o), MATRIX_TYPE_HTTP_API, MatrixHTTPAPIClass))

typedef struct _MatrixHTTPAPI MatrixHTTPAPI;
typedef struct _MatrixHTTPAPIClass MatrixHTTPAPIClass;
G_DEFINE_AUTOPTR_CLEANUP_FUNC(MatrixHTTPAPI, g_object_unref);

struct _MatrixHTTPAPI {
    GObject parent_instance;

    gchar *_homeserver;
    gchar *_user_id;
};

struct _MatrixHTTPAPIClass {
    GObjectClass parent_class;
};

GType matrix_http_api_get_type(void) G_GNUC_CONST;
MatrixHTTPAPI *matrix_http_api_new(const gchar *base_url, const gchar *token, const gchar *refresh_token);
MatrixHTTPAPI *matrix_http_api_construct(GType object_type, const gchar* base_url, const gchar* token, const gchar *refresh_token);
const gchar *matrix_http_api_get_base_url(MatrixHTTPAPI *http_api);
void matrix_http_api_set_base_url(MatrixHTTPAPI *http_api, const gchar *base_url);
gboolean matrix_http_api_get_validate_certificate(MatrixHTTPAPI *http_api);
void matrix_http_api_set_validate_certificate(MatrixHTTPAPI *http_api, gboolean validate_certificate);

G_END_DECLS

#endif  /* __MATRIX_GLIB_SDK_HTTP_API_H__ */
