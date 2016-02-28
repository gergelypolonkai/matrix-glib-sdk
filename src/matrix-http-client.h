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

#ifndef __MATRIX_HTTP_CLIENT_H__
#define __MATRIX_HTTP_CLIENT_H__

#include <glib-object.h>
#include "matrix-glib.h"
#include "matrix-http-api.h"

G_BEGIN_DECLS

#define MATRIX_TYPE_HTTP_CLIENT         (matrix_http_client_get_type())
#define MATRIX_HTTP_CLIENT(o)           (G_TYPE_CHECK_INSTANCE_CAST((o), MATRIX_TYPE_HTTP_CLIENT, MatrixHTTPClient))
#define MATRIX_HTTP_CLIENT_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST((k), MATRIX_TYPE_HTTP_CLIENT, MatrixHTTPClientClass))
#define MATRIX_IS_HTTP_CLIENT(o)        (G_TYPE_CHECK_INSTANCE_TYPE((o), MATRIX_TYPE_HTTP_CLIENT))
#define MATRIX_IS_HTTP_CLIENT_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE((k), MATRIX_TYPE_HTTP_CLIENT))
#define MATRIX_HTTP_CLIENT_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS((o), MATRIX_TYPE_HTTP_CLIENT, MatrixHTTPClientClass))

typedef struct _MatrixHTTPClient      MatrixHTTPClient;
typedef struct _MatrixHTTPClientClass MatrixHTTPClientClass;

struct _MatrixHTTPClient {
    MatrixHTTPAPI parent_instance;
};

struct _MatrixHTTPClientClass {
    MatrixHTTPAPIClass parent_class;
};

GType matrix_http_client_get_type(void) G_GNUC_CONST;

MatrixClient *matrix_http_client_new(const gchar *base_url);

G_END_DECLS

#endif /* __MATRIX_HTTP_CLIENT_H__ */
