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

#ifndef __MATRIX_HTTP_API_H__
#define __MATRIX_HTTP_API_H__

#include <glib-object.h>

#include "matrix-api.h"

G_BEGIN_DECLS

#define MATRIX_TYPE_HTTP_API         (matrix_http_api_get_type())
#define MATRIX_HTTP_API(o)           (G_TYPE_CHECK_INSTANCE_CAST((o), MATRIX_TYPE_HTTP_API, MatrixHTTPAPI))
#define MATRIX_HTTP_API_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST((k), MATRIX_TYPE_HTTP_API, MatrixHTTPAPIClass))
#define MATRIX_IS_HTTP_API(o)        (G_TYPE_CHECK_INSTANCE_TYPE((o), MATRIX_TYPE_HTTP_API))
#define MATRIX_IS_HTTP_API_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE((k), MATRIX_TYPE_HTTP_API))
#define MATRIX_HTTP_API_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS((o), MATRIX_TYPE_HTTP_API, MatrixHTTPAPIClass))

typedef struct _MatrixHTTPAPI      MatrixHTTPAPI;
typedef struct _MatrixHTTPAPIClass MatrixHTTPAPIClass;

struct _MatrixHTTPAPI {
    /* Parent instance structure */
    GObject parent_instance;

    /* Instance members */
};

struct _MatrixHTTPAPIClass {
    GObjectClass parent_class;
};

GType matrix_http_api_get_type(void) G_GNUC_CONST;
void matrix_http_api_set_validate_certificate(MatrixHTTPAPI *api,
                                              gboolean validate_certificate);
gboolean matrix_http_api_get_validate_certificate(MatrixHTTPAPI *api);
const gchar *matrix_http_api_get_base_url(MatrixHTTPAPI *api);

GHashTable *matrix_http_api_gen_parameters(const gchar *param1_name, ...);
MatrixHTTPAPI *matrix_http_api_new(const gchar *base_url, const gchar *token);
void matrix_http_api_login(MatrixAPI *api,
                           MatrixAPICallback callback,
                           gpointer user_data,
                           gchar *login_type,
                           GHashTable *parameters);
void matrix_http_api_register_account(MatrixAPI *api,
                                      MatrixAPICallback callback,
                                      gpointer user_data,
                                      gchar *login_type,
                                      GHashTable *parameters);
void matrix_http_api_initial_sync(MatrixAPI *api,
                                  MatrixAPICallback callback,
                                  gpointer user_data,
                                  guint limit);

G_END_DECLS

#endif /* __MATRIX_HTTP_API_H__ */
