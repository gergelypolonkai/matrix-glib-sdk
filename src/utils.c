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

gchar *
_g_enum_to_string(GType enum_type, gint value, gboolean convert_dashes)
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
                    *a = '_';
                }
            }
        }
    }

    g_type_class_unref(enum_class);

    return nick;
}

gint
_g_enum_nick_to_value(GType enum_type, const gchar *nick, GError **error)
{
    GEnumClass *enum_class = g_type_class_ref(enum_type);
    GEnumValue *enum_value = NULL;
    gchar *nick_c = NULL;
    gchar *a;
    int ret = 0;

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
        g_set_error(error,
                    MATRIX_ERROR, MATRIX_ERROR_UNKNOWN_VALUE,
                    "Value %s is unknown", nick);
    }

    g_type_class_unref(enum_class);

    return ret;
}

static void
deep_copy_object(JsonObject *object,
                 const gchar *member_name,
                 JsonNode *member_node,
                 JsonObject *new_obj)
{
    json_object_set_member(new_obj,
                           member_name,
                           _json_node_deep_copy((const JsonNode *)member_node));
}

static void
deep_copy_array(JsonArray *array,
                guint idx,
                JsonNode *element_node,
                JsonArray *new_array)
{
    json_array_add_element(new_array,
                           _json_node_deep_copy((const JsonNode *)element_node));
}

JsonNode *
_json_node_deep_copy(const JsonNode *node)
{
    JsonNode *ret;

    if (node == NULL) {
        return NULL;
    }

    ret = json_node_new(JSON_NODE_TYPE((JsonNode *)node));

    switch (JSON_NODE_TYPE((JsonNode *)node)) {
        case JSON_NODE_OBJECT:
        {
            JsonObject *new_obj = json_object_new();

            json_object_foreach_member(json_node_get_object((JsonNode *)node),
                                       (JsonObjectForeach)deep_copy_object,
                                       new_obj);

            json_node_set_object(ret, new_obj);

            break;
        }

        case JSON_NODE_ARRAY:
        {
            JsonArray *new_ary = json_array_new();

            json_array_foreach_element(json_node_get_array((JsonNode *)node),
                                       (JsonArrayForeach)deep_copy_array,
                                       new_ary);

            json_node_set_array(ret, new_ary);

            break;
        }

        case JSON_NODE_VALUE:
        {
            GValue val = G_VALUE_INIT;

            json_node_get_value((JsonNode *)node, &val);
            json_node_set_value(ret, &val);

            g_value_unset(&val);

            break;
        }

        case JSON_NODE_NULL:
            break;
    }

    return ret;
}
