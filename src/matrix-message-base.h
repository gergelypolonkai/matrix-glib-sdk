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

#ifndef __MATRIX_GLIB_SDK_MESSAGE_BASE_H__
# define __MATRIX_GLIB_SDK_MESSAGE_BASE_H__

# include <glib-object.h>
# include <json-glib/json-glib.h>

G_BEGIN_DECLS

# define MATRIX_MESSAGE_TYPE_BASE matrix_message_base_get_type()
# define MATRIX_MESSAGE_BASE(o) (G_TYPE_CHECK_INSTANCE_CAST((o), MATRIX_MESSAGE_TYPE_BASE, MatrixMessageBase))
# define MATRIX_MESSAGE_BASE_CLASS(c) (G_TYPE_CHECK_CLASS_CAST((c), MATRIX_MESSAGE_TYPE_BASE, MatrixMessageBaseClass))
# define MATRIX_MESSAGE_IS_BASE(o) (G_TYPE_CHECK_INSTANCE_TYPE((o), MATRIX_MESSAGE_TYPE_BASE))
# define MATRIX_MESSAGE_IS_BASE_CLASS(c) (G_TYPE_CHECK_CLASS_TYPE((c), MATRIX_MESSAGE_TYPE_BASE))
# define MATRIX_MESSAGE_BASE_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS((o), MATRIX_MESSAGE_TYPE_BASE, MatrixMessageBaseClass))

typedef struct _MatrixMessageBase MatrixMessageBase;
typedef struct _MatrixMessageBaseClass MatrixMessageBaseClass;
G_DEFINE_AUTOPTR_CLEANUP_FUNC(MatrixMessageBase, g_object_unref);

struct _MatrixMessageBase {
    GObject parent_instance;
};

struct _MatrixMessageBaseClass {
    GObjectClass parent_class;

    void (*from_json)(MatrixMessageBase *matrix_message_base, JsonNode *json_data, GError **error);
    void (*to_json)(MatrixMessageBase *matrix_message_base, JsonNode *json_data, GError **error);
};

GType matrix_message_base_get_type(void) G_GNUC_CONST;
MatrixMessageBase* matrix_message_base_new_from_json(JsonNode *json_data, GError **error);
void matrix_message_base_from_json(MatrixMessageBase *message, JsonNode* json_data, GError** error);
void matrix_message_base_to_json(MatrixMessageBase *message, JsonNode *json_data, GError **error);
MatrixMessageBase* matrix_message_base_construct(GType object_type);
const gchar* matrix_message_base_get_message_type(MatrixMessageBase *message);
void matrix_message_base_set_message_type(MatrixMessageBase *message, const gchar *message_type);
const gchar* matrix_message_base_get_body(MatrixMessageBase *message);
void matrix_message_base_set_body(MatrixMessageBase *message, const gchar *body);
JsonNode* matrix_message_base_get_json(MatrixMessageBase *message, GError **error);

GType matrix_message_get_handler(const gchar *message_type);
void matrix_message_register_type(const gchar *message_type, GType message_gtype, GError **error);
void matrix_message_unregister_type(const gchar *message_type);

G_END_DECLS

#endif  /* __MATRIX_GLIB_SDK_MESSAGE_BASE_H__ */
