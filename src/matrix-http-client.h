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

#ifndef __MATRIX_GLIB_SDK_HTTP_CLIENT_H__
# define __MATRIX_GLIB_SDK_HTTP_CLIENT_H__

# include <glib-object.h>
# include "matrix-http-api.h"

G_BEGIN_DECLS

# define MATRIX_TYPE_HTTP_CLIENT matrix_http_client_get_type()
G_DECLARE_DERIVABLE_TYPE(MatrixHTTPClient, matrix_http_client, MATRIX, HTTP_CLIENT, MatrixHTTPAPI)

struct _MatrixHTTPClientClass {
    MatrixHTTPAPIClass parent_class;
};

MatrixHTTPClient* matrix_http_client_new(const gchar* base_url);
gulong matrix_http_client_next_txn_id(MatrixHTTPClient *client);

G_END_DECLS

#endif /* __MATRIX_GLIB_SDK_HTTP_CLIENT_H__ */
