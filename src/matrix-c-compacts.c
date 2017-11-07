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

#include <gobject/gvaluecollector.h>
#include "matrix-c-compacts.h"
#include "matrix-types.h"

static JsonNode * matrix_json_compact_get_json_node_impl(MatrixJsonCompact *matrix_json_compact,
                                                         GError **error);

/**
 * MatrixJsonCompactClass:
 * @finalize: instance finalization function.  Should chain up to the @finalize method of the
 *     parent class
 * @get_json_node: instance method to get the JSON node described by the instance
 *
 * Class structure for #MatrixJsonCompact.
 */
static void
matrix_json_compact_class_init(MatrixJsonCompactClass *klass)
{
    ((MatrixJsonCompactClass *)klass)->get_json_node = matrix_json_compact_get_json_node_impl;
}

/**
 * matrix_json_compact_construct:
 * @object_type: the #GType to construct
 *
 * Returns: (transfer full): a new instance of @object_type
 */
MatrixJsonCompact *
matrix_json_compact_construct(GType object_type)
{
    MatrixJsonCompact *ret = (MatrixJsonCompact *)g_type_create_instance(object_type);

    ret->refcount = 1;

    return ret;
}

static void
matrix_json_compact_init(MatrixJsonCompact *matrix_json_compact)
{}

/**
 * matrix_json_compact_ref:
 * @json_compact: a #MatrixJsonCompact object
 *
 * Increment reference count on @json_compact.
 *
 * Returns: (transfer full): @json_compact
 */
MatrixJsonCompact *
matrix_json_compact_ref(MatrixJsonCompact *matrix_json_compact)
{
    g_return_val_if_fail(matrix_json_compact != NULL, NULL);

    ++(matrix_json_compact->refcount);

    return matrix_json_compact;
}

static void
matrix_json_compact_free(MatrixJsonCompact *matrix_json_compact)
{
    g_return_if_fail(matrix_json_compact != NULL);

    g_free(matrix_json_compact);
}

/**
 * matrix_json_compact_unref:
 * @json_compact: (transfer full): a #MatrixJsonCompact object
 *
 * Decrement reference count on @json_compact.
 *
 * If reference count reaches 0, the object is disposed.
 */
void
matrix_json_compact_unref(MatrixJsonCompact *matrix_json_compact)
{
    g_return_if_fail(matrix_json_compact != NULL);

    if (--(matrix_json_compact->refcount) == 0) {
        matrix_json_compact_free(matrix_json_compact);
    }
}

static JsonNode *
matrix_json_compact_get_json_node_impl(MatrixJsonCompact *matrix_json_compact, GError **error)
{
    g_critical("Type `%s' does not implement abstract method `matrix_json_compact_get_json_node'",
               g_type_name(G_TYPE_FROM_INSTANCE(matrix_json_compact)));

    return NULL;
}

/**
 * matrix_json_compact_get_json_node:
 * @json_compact: a #MatrixJsonCompact object
 * @error: a #GError, or %NULL to ignore errors
 *
 * Export the contents of @json_compact as a #JsonNode object.
 *
 * Subclasses must implement this method.
 *
 * Returns: (transfer full): a #JsonNode
 */
JsonNode *
matrix_json_compact_get_json_node(MatrixJsonCompact *matrix_json_compact, GError **error)
{
    g_return_val_if_fail(matrix_json_compact != NULL, NULL);

    return MATRIX_JSON_COMPACT_GET_CLASS(matrix_json_compact)->get_json_node(matrix_json_compact,
                                                                             error);
}

/**
 * matrix_json_compact_get_json_data:
 * @json_compact: a #MatrixJsonCompact object
 * @datalen: (nullable): a place to hold the length of the result, or %NULL to ignore
 * @error: (nullable): a #GError, or %NULL to ignore errors
 *
 * Export the contents of the @json_compact as a stringified JSON object.  If @datalen is
 * not %NULL, the length of the resulting string is stored there.
 *
 * This function calls matrix_json_compact_get_json_node() internally.  If any error happens
 * during the export, it is stored in @error.
 *
 * The returned string is owned by the caller and must be freed.
 *
 * Returns: (transfer full): a stringified JSON object
 */
gchar *
matrix_json_compact_get_json_data(MatrixJsonCompact *matrix_json_compact,
                                  gsize *datalen,
                                  GError **error)
{
    gchar* result = NULL;
    JsonGenerator* generator = NULL;
    JsonNode* node = NULL;
    gsize result_len;
    GError * inner_error = NULL;

    g_return_val_if_fail(matrix_json_compact != NULL, NULL);

    node = matrix_json_compact_get_json_node(matrix_json_compact, &inner_error);

    if (inner_error != NULL) {
        g_object_unref(generator);
        g_propagate_error (error, inner_error);

        return NULL;
    }

    generator = json_generator_new ();
    json_generator_set_root (generator, node);
    result = json_generator_to_data (generator, &result_len);
    json_node_free(node);
    g_object_unref(generator);

    if (datalen) {
        *datalen = result_len;
    }

    return result;
}

static void
matrix_value_json_compact_init(GValue *value)
{
    value->data[0].v_pointer = NULL;
}

static void
matrix_value_json_compact_free_value(GValue *value)
{
    matrix_json_compact_unref(value->data[0].v_pointer);
}

static void
matrix_value_json_compact_copy_value(const GValue *src_value, GValue *dest_value)
{
    dest_value->data[0].v_pointer = matrix_json_compact_ref(src_value->data[0].v_pointer);
}

static gpointer
matrix_value_json_compact_peek_pointer(const GValue *value)
{
    return value->data[0].v_pointer;
}

static gchar *
matrix_value_json_compact_collect_value(GValue *value, guint n_collect_values, GTypeCValue *collect_values, guint collect_flags)
{
    if (collect_values[0].v_pointer) {
        MatrixJsonCompact *object = collect_values[0].v_pointer;

        if (object->parent_instance.g_class == NULL) {
            return g_strconcat("invalid unclassed object pointer for value type `", G_VALUE_TYPE_NAME(value), "'", NULL);
        } else if (!g_value_type_compatible(G_TYPE_FROM_INSTANCE(object), G_VALUE_TYPE(value))) {
            return g_strconcat("invalid object type `", g_type_name(G_TYPE_FROM_INSTANCE (object)), "' for value type `", G_VALUE_TYPE_NAME(value), "'", NULL);
        }

        value->data[0].v_pointer = matrix_json_compact_ref(object);
    } else {
        value->data[0].v_pointer = NULL;
    }

    return NULL;
}

static gchar *
matrix_value_json_compact_lcopy_value(const GValue *value, guint n_collect_values, GTypeCValue *collect_values, guint collect_flags)
{
    MatrixJsonCompact **object_p = collect_values[0].v_pointer;

    if (!object_p) {
        return g_strdup_printf("value location for `%s' passed as NULL", G_VALUE_TYPE_NAME(value));
    }

    if (!value->data[0].v_pointer) {
        *object_p = NULL;
    } else if (collect_flags & G_VALUE_NOCOPY_CONTENTS) {
        *object_p = value->data[0].v_pointer;
    } else {
        *object_p = matrix_json_compact_ref (value->data[0].v_pointer);
    }

    return NULL;
}

/**
 * MatrixJsonCompact:
 *
 * Abstract parent for classes that can be saved to JSON but don’t need full #GObject
 * functionality.
 */
GType
matrix_json_compact_get_type(void)
{
    static volatile gsize matrix_json_compact_type_id__volatile = 0;

    if (g_once_init_enter(&matrix_json_compact_type_id__volatile)) {
        static const GTypeValueTable g_define_type_value_table = {
            matrix_value_json_compact_init,
            matrix_value_json_compact_free_value,
            matrix_value_json_compact_copy_value,
            matrix_value_json_compact_peek_pointer, "p",
            matrix_value_json_compact_collect_value, "p",
            matrix_value_json_compact_lcopy_value
        };
        static const GTypeInfo g_define_type_info = {
            sizeof(MatrixJsonCompactClass),
            (GBaseInitFunc)NULL,
            (GBaseFinalizeFunc)NULL,
            (GClassInitFunc)matrix_json_compact_class_init,
            (GClassFinalizeFunc)NULL,
            NULL,
            sizeof(MatrixJsonCompact),
            0,
            (GInstanceInitFunc)matrix_json_compact_init,
            &g_define_type_value_table
        };
        static const GTypeFundamentalInfo g_define_type_fundamental_info = {
            (G_TYPE_FLAG_CLASSED | G_TYPE_FLAG_INSTANTIATABLE | G_TYPE_FLAG_DERIVABLE | G_TYPE_FLAG_DEEP_DERIVABLE)
        };
        GType matrix_json_compact_type_id;
        matrix_json_compact_type_id = g_type_register_fundamental(g_type_fundamental_next(),
                                                                  "MatrixJsonCompact",
                                                                  &g_define_type_info,
                                                                  &g_define_type_fundamental_info,
                                                                  G_TYPE_FLAG_ABSTRACT);
        g_once_init_leave(&matrix_json_compact_type_id__volatile, matrix_json_compact_type_id);
    }

    return matrix_json_compact_type_id__volatile;
}
