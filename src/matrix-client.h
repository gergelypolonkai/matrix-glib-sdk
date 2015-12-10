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

#ifndef __MATRIX_CLIENT_H__
#define __MATRIX_CLIENT_H__

#include <glib-object.h>

G_BEGIN_DECLS

#define MATRIX_TYPE_CLIENT         (matrix_client_get_type())
#define MATRIX_CLIENT(o)           (G_TYPE_CHECK_INSTANCE_CAST((o), MATRIX_TYPE_CLIENT, MatrixClient))
#define MATRIX_CLIENT_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST((k), MATRIX_TYPE_CLIENT, MatrixClientClass))
#define MATRIX_IS_CLIENT(o)        (G_TYPE_CHECK_INSTANCE_TYPE((o), MATRIX_TYPE_CLIENT))
#define MATRIX_IS_CLIENT_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE((k), MATRIX_TYPE_CLIENT))
#define MATRIX_CLIENT_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS((o), MATRIX_TYPE_CLIENT, MatrixClientClass))

typedef struct _MatrixClient      MatrixClient;
typedef struct _MatrixClientClass MatrixClientClass;

struct _MatrixClient {
    /* Parent instance structure */
    GObject parent_instance;

    /* Instance members */
};

struct _MatrixClientClass {
    /* Parent class */
    GObjectClass parent_class;
};

GType matrix_client_get_type(void) G_GNUC_CONST;
MatrixClient *matrix_client_new(const gchar *homeserver, const gchar *token);

G_END_DECLS

#endif /* __MATRIX_CLIENT_H__ */
