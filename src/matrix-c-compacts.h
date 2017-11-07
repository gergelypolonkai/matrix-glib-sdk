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

#ifndef __MATRIX_GLIB_SDK_COMPACTS_H__
# define __MATRIX_GLIB_SDK_COMPACTS_H__

# include <glib-object.h>
# include <json-glib/json-glib.h>

G_BEGIN_DECLS

# define MATRIX_TYPE_JSON_COMPACT matrix_json_compact_get_type()
# define MATRIX_JSON_COMPACT(o) (G_TYPE_CHECK_INSTANCE_CAST((o), MATRIX_TYPE_JSON_COMPACT, MatrixJsonCompact))
# define MATRIX_JSON_COMPACT_CLASS(c) (G_TYPE_CHECK_CLASS_CAST((c), MATRIX_TYPE_JSON_COMPACT, MatrixJsonCompactClass))
# define MATRIX_IS_JSON_COMPACT(o) (G_TYPE_CHECK_INSTANCE_TYPE((o), MATRIX_TYPE_JSON_COMPACT))
# define MATRIX_IS_JSON_COMPACT_CLASS(c) (G_TYPE_CHECK_CLASS_TYPE((c), MATRIX_TYPE_JSON_COMPACT))
# define MATRIX_JSON_COMPACT_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS((o), MATRIX_TYPE_JSON_COMPACT, MatrixJsonCompactClass))

typedef struct _MatrixJsonCompactClass MatrixJsonCompactClass;
typedef struct _MatrixJsonCompact MatrixJsonCompact;
void matrix_json_compact_unref(MatrixJsonCompact *json_compact);
G_DEFINE_AUTOPTR_CLEANUP_FUNC(MatrixJsonCompact, matrix_json_compact_unref);

struct _MatrixJsonCompact {
    GTypeInstance parent_instance;

    /* < private > */
    volatile int refcount;
};

struct _MatrixJsonCompactClass {
    GTypeClass parent_class;

    void (*finalize)(MatrixJsonCompact *json_compact);
    JsonNode *(*get_json_node)(MatrixJsonCompact *json_compact, GError **error);
};

GType matrix_json_compact_get_type(void) G_GNUC_CONST;
MatrixJsonCompact *matrix_json_compact_construct(GType object_type);
MatrixJsonCompact *matrix_json_compact_ref(MatrixJsonCompact *json_compact);
JsonNode *matrix_json_compact_get_json_node(MatrixJsonCompact *json_compact, GError **error);
gchar *matrix_json_compact_get_json_data(MatrixJsonCompact *json_compact, gsize *datalen, GError **error);

G_END_DECLS

#endif  /* __MATRIX_GLIB_SDK_COMPACTS_H__ */
