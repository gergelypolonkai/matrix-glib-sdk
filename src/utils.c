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

#include "utils.h"
#include "matrix-types.h"

// GTK-Doc looking comments in this file intentionally do not begin with a double star, as the
// functions here are internal

/*
 * _matrix_g_enum_to_string:
 *
 * @enum_type: a #GEnumType
 * @value: an integer value registered in @enum_type
 * @convert_dashes: if non-NUL, dashes in the value nick will be converted to this char
 *
 * Get the value nick (lower case, dash-separated name) of an enum, like matrix-error-none.  If
 * @convert_dashes is not NUL (`'\0'`), dashes will be converted to that char.
 *
 * Returns: (transfer full) the converted enum name, which must be freed
 */
gchar *
_matrix_g_enum_to_string(GType enum_type, gint value, gchar convert_dashes)
{
    GEnumClass *enum_class = g_type_class_ref(enum_type);
    GEnumValue *enum_value = g_enum_get_value(enum_class, value);
    gchar *nick = NULL;

    if (value) {
        nick = g_strdup(enum_value->value_nick);

        if (convert_dashes) {
            gchar *a;

            for (a = nick; *a; a++) {
                if (*a == '-') {
                    *a = convert_dashes;
                }
            }
        }
    }

    g_type_class_unref(enum_class);

    return nick;
}

/*
 * _matrix_g_enum_nick_to_value:
 *
 * @enum_type: a #GEnumType
 * @nick: a value nick registered in @enum_type
 * @error: a #GError, or NULL to ignore errors
 *
 * Get the integer value of the enum nick @nick from @enum_type.  If the nick contains underscores
 * (`_`), they will be converted to dashes (`-`) first.
 *
 * If @nick cannot be found in @enum_type, this function returns NULL, and sets @error to
 * #MATRIX_ERROR_UNKNOWN_VALUE.
 *
 * Returns: the integer value of @nick, or 0 if not found
 */
gint
_matrix_g_enum_nick_to_value(GType enum_type, const gchar *nick, GError **error)
{
    GEnumClass *enum_class = g_type_class_ref(enum_type);
    GEnumValue *enum_value;
    gchar *nick_c = NULL;
    gchar *a;
    gint ret = 0;

    nick_c = g_strdup(nick);

    for (a = nick_c; *a; a++) {
        if (*a == '_') {
            *a = '-';
        }
    }

    enum_value = g_enum_get_value_by_nick(enum_class, nick_c);
    g_free(nick_c);

    if (enum_value) {
        ret = enum_value->value;
    } else {
        g_set_error(error, MATRIX_ERROR, MATRIX_ERROR_UNKNOWN_VALUE,
                    "Value %s is unknown", nick);
    }

    g_type_class_unref(enum_class);

    return ret;
}

static void
_copy_obj_member(JsonObject *old_obj, const gchar *member_name, JsonNode *member_node, gpointer user_data)
{
    JsonObject *new_obj = user_data;

    json_object_set_member(new_obj, member_name, _matrix_json_node_deep_copy(member_node));
}

static void
_copy_ary_member(JsonArray *old_ary, guint idx, JsonNode *element_node, gpointer user_data)
{
    JsonArray *new_ary = user_data;

    json_array_add_element(new_ary, _matrix_json_node_deep_copy(element_node));
}

JsonNode *
_matrix_json_node_deep_copy(JsonNode *node)
{
    JsonNode *ret;
    JsonNodeType node_type;

    g_return_val_if_fail(node != NULL, NULL);

    node_type = json_node_get_node_type(node);
    ret = json_node_new(node_type);

    switch (node_type) {
        case JSON_NODE_OBJECT:
        {
            JsonObject *new_obj = json_object_new();
            JsonObject *old_obj = json_node_get_object(node);

            json_object_foreach_member(old_obj, _copy_obj_member, new_obj);

            json_node_set_object(ret, new_obj);

            break;
        }
        case JSON_NODE_ARRAY:
        {
            JsonArray *new_ary = json_array_new();
            JsonArray *old_ary = json_node_get_array(node);

            json_array_foreach_element(old_ary, _copy_ary_member, new_ary);

            json_node_set_array(ret, new_ary);

            break;
        }
        case JSON_NODE_VALUE:
        {
            GValue node_value = G_VALUE_INIT;

            json_node_get_value(node, &node_value);
            json_node_set_value(ret, &node_value);

            g_value_unset(&node_value);

            break;
        }
        case JSON_NODE_NULL:
            break;
    }

    return ret;
}
