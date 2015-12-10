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

#ifndef __MATRIX_API_H__
#define __MATRIX_API_H__

#include <glib-object.h>

G_BEGIN_DECLS

#define MATRIX_TYPE_API         (matrix_api_get_type())
#define MATRIX_API(o)           (G_TYPE_CHECK_INSTANCE_CAST((o), MATRIX_TYPE_API, MatrixAPI))
#define MATRIX_API_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST((k), MATRIX_TYPE_API, MatrixAPIClass))
#define MATRIX_IS_API(o)        (G_TYPE_CHECK_INSTANCE_TYPE((o), MATRIX_TYPE_API))
#define MATRIX_IS_API_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE((k), MATRIX_TYPE_API))
#define MATRIX_API_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS((o), MATRIX_TYPE_API, MatrixAPIClass))

typedef struct _MatrixAPI      MatrixAPI;
typedef struct _MatrixAPIClass MatrixAPIClass;

struct _MatrixAPI {
    /* Parent instance structure */
    GObject parent_instance;

    /* Instance members */
};

struct _MatrixAPIClass {
    GObjectClass parent_class;
};

GType matrix_api_get_type(void) G_GNUC_CONST;

G_END_DECLS

#endif /* __MATRIX_API_H__ */
