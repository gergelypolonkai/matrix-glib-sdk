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

#ifndef __MATRIX_GLIX_SDK_EVENT_BASE_H__
# define __MATRIX_GLIX_SDK_EVENT_BASE_H__

# include <glib-object.h>
# include <json-glib/json-glib.h>

# define MATRIX_EVENT_TYPE_BASE (matrix_event_base_get_type ())
# define MATRIX_EVENT_BASE(o) (G_TYPE_CHECK_INSTANCE_CAST((o), MATRIX_EVENT_TYPE_BASE, MatrixEventBase))
# define MATRIX_EVENT_BASE_CLASS(c) (G_TYPE_CHECK_CLASS_CAST((c), MATRIX_EVENT_TYPE_BASE, MatrixEventBaseClass))
# define MATRIX_EVENT_IS_BASE(o) (G_TYPE_CHECK_INSTANCE_TYPE((o), MATRIX_EVENT_TYPE_BASE))
# define MATRIX_EVENT_IS_BASE_CLASS(c) (G_TYPE_CHECK_CLASS_TYPE((c), MATRIX_EVENT_TYPE_BASE))
# define MATRIX_EVENT_BASE_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS((o), MATRIX_EVENT_TYPE_BASE, MatrixEventBaseClass))

typedef struct _MatrixEventBase MatrixEventBase;
typedef struct _MatrixEventBaseClass MatrixEventBaseClass;
G_DEFINE_AUTOPTR_CLEANUP_FUNC(MatrixEventBase, g_object_unref);

struct _MatrixEventBase {
    GObject parent_instance;
};

struct _MatrixEventBaseClass {
    GObjectClass parent_instance;

    void (*from_json)(MatrixEventBase *matrix_event_base, JsonNode *json_data, GError **error);
    void (*to_json)(MatrixEventBase *matrix_event_base, JsonNode *json_data, GError **error);
};

GType matrix_event_get_handler(const gchar *event_type);
void matrix_event_register_type(const gchar *event_type, GType event_gtype, GError **error);
void matrix_event_unregister_type(const gchar *event_type);

GType matrix_event_base_get_type(void) G_GNUC_CONST;
void matrix_event_base_from_json(MatrixEventBase *event, JsonNode *json_data, GError **error);
void matrix_event_base_to_json(MatrixEventBase *event, JsonNode *json_data, GError **error);
MatrixEventBase *matrix_event_base_new_from_json(const gchar *event_type, JsonNode *json_data, GError **error);
MatrixEventBase *matrix_event_base_construct(GType object_type);
const gchar *matrix_event_base_get_event_type(MatrixEventBase *event);
JsonNode *matrix_event_base_get_json(MatrixEventBase *event);

#endif  /* __MATRIX_GLIX_SDK_EVENT_BASE_H__ */
