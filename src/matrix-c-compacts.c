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
#include "matrix-enumtypes.h"
#include "matrix-types.h"
#include "utils.h"

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

typedef struct {
    guint64 _limit;
    gchar **_types;
    guint _types_len;
    gchar **_excluded_types;
    guint _excluded_types_len;
    gchar **_senders;
    guint _senders_len;
    gchar **_excluded_senders;
    guint _excluded_senders_len;
    gchar **_rooms;
    guint _rooms_len;
    gchar **_excluded_rooms;
    guint _excluded_rooms_len;
} MatrixFilterRulesPrivate;

/**
 * MatrixFilterRules:
 *
 * Class to hold filtering rules.
 */
G_DEFINE_TYPE_WITH_PRIVATE(MatrixFilterRules, matrix_filter_rules, MATRIX_TYPE_JSON_COMPACT)

/*
 * STR_ARRAY_TO_JSON:
 * @PRIV: (not nullable): a variable that holds a #MatrixFilterRulesPrivate struct
 * @BUILDER: (not nullable): A #JsonBuilder object to use for array building.  It must have an
 *     object opened with json_builder_begin_object()
 * @NAME: (not nullable) (transfer none): the name of the array within the opened object
 *
 * Creates a new array with @NAME in the currently open object of @BUILDER.  To do so, it will
 * use the field of the same name from @PRIV.
 */
#define STR_ARRAY_TO_JSON(PRIV, BUILDER, NAME)                                   \
    if ((PRIV)->_ ## NAME ## _len != 0) {                                        \
        guint i;                                                                 \
                                                                                 \
        json_builder_set_member_name((BUILDER), #NAME);                         \
        json_builder_begin_array((BUILDER));                                    \
                                                                                 \
        for (i = 0; i < (PRIV)->_ ## NAME ## _len; i++) {                        \
            json_builder_add_string_value((BUILDER), (PRIV)->_ ## NAME [i]);    \
        };                                                                       \
                                                                                 \
        json_builder_end_array((BUILDER));                                      \
    }                                                                            \
                                                                                 \
    if ((PRIV)->_excluded_ ## NAME ## _len != 0) {                               \
        guint i;                                                                 \
                                                                                 \
        json_builder_set_member_name((BUILDER), "not_" #NAME);                  \
        json_builder_begin_array((BUILDER));                                    \
                                                                                 \
        for (i = 0; i < (PRIV)->_excluded_ ## NAME ## _len; i++) {               \
            json_builder_add_string_value((BUILDER), (PRIV)->_excluded_ ## NAME [i]); \
        };                                                                       \
                                                                                 \
        json_builder_end_array((BUILDER));                                      \
    }


static JsonNode *
matrix_filter_rules_get_json_node(MatrixJsonCompact *matrix_json_compact, GError **error)
{
    MatrixFilterRules *matrix_filter_rules;
    MatrixFilterRulesPrivate *priv;

    g_return_val_if_fail(matrix_json_compact != NULL, NULL);

    matrix_filter_rules = (MatrixFilterRules *)matrix_json_compact;
    priv = matrix_filter_rules_get_instance_private(matrix_filter_rules);

    JsonNode* result = NULL;
    JsonBuilder* builder = NULL;

    builder = json_builder_new ();
    json_builder_begin_object (builder);
    json_builder_set_member_name (builder, "limit");
    json_builder_add_int_value (builder, (gint64)priv->_limit);

    STR_ARRAY_TO_JSON(priv, builder, rooms);
    STR_ARRAY_TO_JSON(priv, builder, senders);
    STR_ARRAY_TO_JSON(priv, builder, types);

    json_builder_end_object(builder);
    result = json_builder_get_root (builder);
    g_object_unref(builder);

    return result;
}

#undef STR_ARRAY_TO_JSON

/**
 * matrix_filter_rules_construct:
 * @object_type: the #GType of the object to construct.  This must be derived from
 *     #MatrixJsonCompact.
 *
 * Construct a new #MatrixFilterRules object.
 *
 * Returns: (transfer full): a new #MatrixFilterRules object
 */
MatrixFilterRules *
matrix_filter_rules_construct(GType object_type)
{
    return (MatrixFilterRules *)matrix_json_compact_construct(object_type);
}

/**
 * matrix_filter_rules_new:
 *
 * Create a new #MatrixFilterRules object with a reference count of 1.
 *
 * Returns: (transfer full): a new #MatrixFilterRules object
 */
MatrixFilterRules *
matrix_filter_rules_new(void)
{
    return matrix_filter_rules_construct(MATRIX_TYPE_FILTER_RULES);
}

/**
 * matrix_filter_rules_get_limit:
 * @filter_rules: a #MatrixFilterRules object
 *
 * Get the message count limit defined in @filter_rules.
 *
 * Returns: the message count limit
 */
guint
matrix_filter_rules_get_limit(MatrixFilterRules *matrix_filter_rules)
{
    MatrixFilterRulesPrivate *priv;

    g_return_val_if_fail(matrix_filter_rules != NULL, 0);

    priv = matrix_filter_rules_get_instance_private(matrix_filter_rules);

    return priv->_limit;
}

/**
 * matrix_filter_rules_set_limit:
 * @filter_rules: a #MatrixFilterRules object
 * @limit: the message count limit
 *
 * Set the message count limit in @filter_rules.
 */
void
matrix_filter_rules_set_limit(MatrixFilterRules *matrix_filter_rules, guint limit)
{
    MatrixFilterRulesPrivate *priv;

    g_return_if_fail(matrix_filter_rules != NULL);

    priv = matrix_filter_rules_get_instance_private(matrix_filter_rules);

    priv->_limit = limit;
}

static inline gchar **
copy_str_array(gchar **src, gint n_src)
{
    gchar **dst;

    if (src == NULL) {
        return NULL;
    }

    dst = g_new0(gchar *, n_src);

    for (gint i = 0; i < n_src; i++) {
        dst[i] = g_strdup(src[i]);
    }

    return dst;
}

static inline void
free_str_array(gchar **list, gint n_list)
{
    gint i = 0;

    if (list == NULL) {
        return;
    }

    g_return_if_fail(list != NULL);

    for (i = 0; i < n_list; i++) {
        g_free(list[i]);
    }
}

/*
 * STR_ARRAY_GETTER:
 * @NAME: the name of a string array in #MatrixFilterRulesPrivate
 *
 * Create a getter function for @NAME in #MatrixFilterRulesPrivate.
 */
#define STR_ARRAY_GETTER(NAME)                                          \
    gchar **                                                            \
    matrix_filter_rules_get_ ## NAME (MatrixFilterRules *matrix_filter_rules, gint *n_ ## NAME) \
    {                                                                   \
        MatrixFilterRulesPrivate * priv;                             \
                                                                        \
        g_return_val_if_fail(matrix_filter_rules != NULL, NULL);       \
                                                                        \
        priv = matrix_filter_rules_get_instance_private(matrix_filter_rules); \
                                                                        \
        if (n_ ## NAME != NULL) {                                       \
            *n_ ## NAME = priv->_ ## NAME ## _len;                      \
        }                                                               \
                                                                        \
        return priv->_ ## NAME;                                             \
    }

/*
 * STR_ARRAY_SETTER:
 * @NAME: the name of a string array in #MatrixFilterRulesPrivate
 *
 * Create a setter function for @NAME in #MatrixFilterRulesPrivate
 */
#define STR_ARRAY_SETTER(NAME)                                          \
    void                                                                \
    matrix_filter_rules_set_ ## NAME (MatrixFilterRules *matrix_filter_rules, gchar ** NAME, gint n_ ## NAME) \
    {                                                                   \
        MatrixFilterRulesPrivate * priv;                             \
                                                                        \
        g_return_if_fail(matrix_filter_rules != NULL);                 \
                                                                        \
        priv = matrix_filter_rules_get_instance_private(matrix_filter_rules); \
                                                                        \
        free_str_array(priv->_ ## NAME, priv->_ ## NAME ##_len);       \
        priv->_ ## NAME = copy_str_array(NAME, n_ ## NAME);            \
        priv->_ ## NAME ## _len = n_ ## NAME;                           \
    }

/**
 * matrix_filter_rules_get_types:
 * @filter_rules: a #MatrixFilterRules object
 * @n_types: (nullable): placeholder for the length of the returned list, or %NULL to ignore
 *
 * Get the list of message types to be included in the filtered results.
 *
 * The returned value is owned by @filter_rules and shouldn’t be freed nor modified.
 *
 * Returns: (nullable) (transfer none): a list of message types
 */
STR_ARRAY_GETTER(types);

/**
 * matrix_filter_rules_set_types:
 * @filter_rules: a #MatrixFilterRules object
 * @types: (nullable): a list of message types
 * @n_types: the number of items in @types
 *
 * Set the list of message types to be included in the filtered results.
 */
STR_ARRAY_SETTER(types);

/**
 * matrix_filter_rules_get_excluded_types:
 * @filter_rules: a #MatrixFilterRules object
 * @n_excluded_types: (nullable): placeholder for the length of the returned list, or %NULL to ignore
 *
 * Get the list of message types to be excluded in the filtered results.
 *
 * The returned value is owned by @filter_rules and shouldn’t be freed nor modified.
 *
 * Returns: (transfer none) (nullable): a list of message types to exclude
 */
STR_ARRAY_GETTER(excluded_types);

/**
 * matrix_filter_rules_set_excluded_types:
 * @filter_rules: a #MatrixFilterRules object
 * @excluded_types: (nullable): a list of types to exclude
 * @n_excluded_types: the number of items in @excluded_types
 *
 * Set the list of message types to be excluded in the filtered results.  The message types in
 * this list will be excluded even if they would be explicitly included (ie. set with
 * matrix_filter_rules_set_types()).
 */
STR_ARRAY_SETTER(excluded_types)

/**
 * matrix_filter_rules_get_senders:
 * @filter_rules: a #MatrixFilterRules object
 * @n_senders: (nullable): placeholder for the length of the returned list, or %NULL to ignore
 *
 * Get the list of senders whose messages should to be included in the filtered results.
 *
 * The returned value is owned by @filter_rules and shouldn’t be freed nor modified.
 *
 * Returns: (transfer none) (nullable): a list of sender Matrix IDs
 */
STR_ARRAY_GETTER(senders);

/**
 * matrix_filter_rules_set_senders:
 * @filter_rules: a #MatrixFilterRules object
 * @senders: (nullable): a list of Matrix IDs
 * @n_senders: the number of items in @senders
 *
 * Set the list of Matrix IDs whose messages should be included in the filtered results.
 */
STR_ARRAY_SETTER(senders);

/**
 * matrix_filter_rules_get_excluded_senders:
 * @filter_rules: a #MatrixFilterRules object
 * @n_excluded_senders: (nullable): placeholder for the length of the returned list, or %NULL
 *     to ignore
 *
 * Get the list of sender Matrix IDs to be excluded from the filtered results.
 *
 * The returned value is owned by @filter_rules and shouldn’t be freed nor modified.
 *
 * Returns: (transfer none) (nullable): a list of Matrix IDs to exclude
 */
STR_ARRAY_GETTER(excluded_senders);

/**
 * matrix_filter_rules_set_excluded_senders:
 * @filter_rules: a #MatrixFilterRules object
 * @excluded_senders: (nullable): a list of Matrix IDs to exclude
 * @n_excluded_senders: the number of items in @excluded_senders
 *
 * Set the list of Matrix IDs whose messages should be excluded from the filtered results.
 * Messages from Matrix IDs in this list will be excluded even if they would be explicitly
 * included (ie. set with matrix_filter_rules_set_senders()).
 */
STR_ARRAY_SETTER(excluded_senders);

/**
 * matrix_filter_rules_get_rooms:
 * @filter_rules: a #MatrixFilterRules object
 * @n_rooms: (nullable): placeholder for the length of the returned list, or %NULL to ignore
 *
 * Get the list of rooms to be included in the filtered results.
 *
 * The returned value is owned by @filter_rules and shouldn’t be freed nor modified.
 *
 * Returns: (transfer none) (nullable): a list of rooms
 */
STR_ARRAY_GETTER(rooms);

/**
 * matrix_filter_rules_set_rooms:
 * @filter_rules: a #MatrixFilterRules object
 * @rooms: (nullable): a list of room IDs
 * @n_rooms: the number of items in @rooms
 *
 * Set the list of rooms to be included in the filtered results.
 */
STR_ARRAY_SETTER(rooms);

/**
 * matrix_filter_rules_get_excluded_rooms:
 * @filter_rules: a #MatrixFilterRules object
 * @n_excluded_rooms: (nullable): placeholder for the length of the returned list, or %NULL to ignore
 *
 * Get the list of room IDs to be excluded in the filtered results.
 *
 * The returned value is owned by @filter_rules and shouldn’t be freed nor modified.
 *
 * Returns: (transfer none) (nullable): a list of room IDs to exclude
 */
STR_ARRAY_GETTER(excluded_rooms);

/**
 * matrix_filter_rules_set_excluded_rooms:
 * @filter_rules: a #MatrixFilterRules object
 * @excluded_rooms: (nullable): a list of room IDs to exclude
 * @n_excluded_rooms: the number of items in @excluded_rooms
 *
 * Set the list of room IDs to be excluded from the filtered results.  The room IDs in this
 * list will be excluded even if they would be explicitly included (ie. set with
 * matrix_filter_rules_set_rooms()).
 */
STR_ARRAY_SETTER(excluded_rooms);

static void
matrix_filter_rules_finalize (MatrixJsonCompact *matrix_json_compact)
{
    MatrixFilterRules *matrix_filter_rules;
    MatrixFilterRulesPrivate *priv;

    g_return_if_fail(matrix_json_compact != NULL);

    matrix_filter_rules = MATRIX_FILTER_RULES(matrix_json_compact);
    priv = matrix_filter_rules_get_instance_private(matrix_filter_rules);
    priv->_types = (free_str_array(priv->_types, priv->_types_len), NULL);
    priv->_excluded_types = (free_str_array(priv->_excluded_types, priv->_excluded_types_len), NULL);
    priv->_senders = (free_str_array(priv->_senders, priv->_senders_len), NULL);
    priv->_excluded_senders = (free_str_array(priv->_excluded_senders, priv->_excluded_senders_len), NULL);
    priv->_rooms = (free_str_array(priv->_rooms, priv->_rooms_len), NULL);
    priv->_excluded_rooms = (free_str_array(priv->_excluded_rooms, priv->_excluded_rooms_len), NULL);

    MATRIX_JSON_COMPACT_CLASS(matrix_filter_rules_parent_class)->finalize(matrix_json_compact);
}

static void
matrix_filter_rules_class_init(MatrixFilterRulesClass *klass)
{
    ((MatrixJsonCompactClass *)klass)->finalize = matrix_filter_rules_finalize;
    ((MatrixJsonCompactClass *)klass)->get_json_node = (JsonNode* (*) (MatrixJsonCompact *, GError**)) matrix_filter_rules_get_json_node;
}


static void
matrix_filter_rules_init(MatrixFilterRules *matrix_filter_rules)
{}

typedef struct {
    gboolean _include_leave;
    MatrixFilterRules *_ephemeral;
    MatrixFilterRules *_state;
    MatrixFilterRules *_timeline;
} MatrixRoomFilterPrivate;

/**
 * MatrixRoomFilter:
 *
 * Class to hold room filters.
 */
G_DEFINE_TYPE_WITH_PRIVATE(MatrixRoomFilter, matrix_room_filter, MATRIX_TYPE_JSON_COMPACT);

static JsonNode *
matrix_room_filter_get_json_node(MatrixJsonCompact *matrix_json_compact, GError **error)
{
    MatrixRoomFilterPrivate *priv;
    JsonBuilder *builder;
    JsonNode *result;
    JsonNode *node;
    GError *inner_error = NULL;

    g_return_val_if_fail(matrix_json_compact != NULL, NULL);

    priv = matrix_room_filter_get_instance_private(MATRIX_ROOM_FILTER(matrix_json_compact));

    builder = json_builder_new();

    json_builder_begin_object(builder);

    json_builder_set_member_name(builder, "include_leave");
    json_builder_add_boolean_value(builder, priv->_include_leave);

    if (priv->_ephemeral != NULL) {
        json_builder_set_member_name(builder, "ephemeral");
        node = matrix_json_compact_get_json_node(MATRIX_JSON_COMPACT(priv->_ephemeral), &inner_error);

        if (inner_error) {
            g_object_unref(builder);
            g_propagate_error(error, inner_error);

            return NULL;
        }

        json_builder_add_value(builder, node);
    }

    if (priv->_state != NULL) {
        json_builder_set_member_name(builder, "state");
        node = matrix_json_compact_get_json_node(MATRIX_JSON_COMPACT(priv->_state), &inner_error);

        if (inner_error) {
            g_object_unref(builder);
            g_propagate_error(error, inner_error);

            return NULL;
        }

        json_builder_add_value(builder, node);
    }

    if (priv->_timeline != NULL) {
        json_builder_set_member_name(builder, "timeline");
        node = matrix_json_compact_get_json_node(MATRIX_JSON_COMPACT(priv->_timeline), &inner_error);

        if (inner_error) {
            g_object_unref(builder);
            g_propagate_error(error, inner_error);

            return NULL;
        }

        json_builder_add_value(builder, node);
    }

    json_builder_end_object(builder);

    result = json_builder_get_root(builder);
    g_object_unref(builder);

    return result;
}

MatrixRoomFilter *
matrix_room_filter_construct(GType object_type)
{
    MatrixRoomFilter *matrix_room_filter = NULL;

    matrix_room_filter = (MatrixRoomFilter *)matrix_json_compact_construct(object_type);

    return matrix_room_filter;
}

/**
 * matrix_room_filter_new:
 *
 * Create a new #MatrixRoomFilter object with a reference count of 1.
 *
 * Returns: (transfer full): a new #MatrixRoomFilter object
 */
MatrixRoomFilter *
matrix_room_filter_new(void)
{
    return matrix_room_filter_construct(MATRIX_TYPE_ROOM_FILTER);
}

/**
 * matrix_room_filter_get_include_leave:
 * @room_filter: a #MatrixRoomFilter
 *
 * Returns: %TRUE if events for rooms the user has left will be included, %FALSE otherwise
 */
gboolean
matrix_room_filter_get_include_leave(MatrixRoomFilter *matrix_room_filter) {
    MatrixRoomFilterPrivate *priv;

    g_return_val_if_fail(matrix_room_filter != NULL, FALSE);

    priv = matrix_room_filter_get_instance_private(matrix_room_filter);

    return priv->_include_leave;
}

/**
 * matrix_room_filter_set_include_leave:
 * @room_filter: a #MatrixRoomFilter object
 * @include_leave: a boolean value
 *
 * If @include_leave is %TRUE,  events for rooms that the user has left will
 * be included in the filtered event list.
 */
void
matrix_room_filter_set_include_leave(MatrixRoomFilter *matrix_room_filter, gboolean include_leave)
{
    MatrixRoomFilterPrivate *priv;

    g_return_if_fail(matrix_room_filter != NULL);

    priv = matrix_room_filter_get_instance_private(matrix_room_filter);
    priv->_include_leave = include_leave;
}

/**
 * matrix_room_filter_get_ephemeral:
 * @room_filter: a #MatrixRoomFilter object
 *
 * Get the filtering rules for ephemeral events, like typing notifications and event receipts
 * will also be included in the results.
 *
 * The returned value is owwned by @room_filter and shouldn’t be freed.
 *
 * Returns: (transfer none) (nullable): the filtering rules to be used on ephemeral events
 */
MatrixFilterRules *
matrix_room_filter_get_ephemeral(MatrixRoomFilter *matrix_room_filter)
{
    MatrixRoomFilterPrivate *priv;

    g_return_val_if_fail(matrix_room_filter != NULL, NULL);

    priv = matrix_room_filter_get_instance_private(matrix_room_filter);

    return priv->_ephemeral;
}

/**
 * matrix_room_filter_set_ephemeral:
 * @room_filter: a #MatrixRoomFilter object
 * @ephemeral_rules: (transfer none) (nullable): a #MatrixFilterRules to apply to ephemeral events
 *
 * Set filtering rules for ephemeral events (events that are not recorded in the room history,
 * like typing notifications, event receipts, etc.)
 */
void
matrix_room_filter_set_ephemeral(MatrixRoomFilter *matrix_room_filter, MatrixFilterRules *ephemeral)
{
    MatrixRoomFilterPrivate *priv;

    g_return_if_fail(matrix_room_filter != NULL);

    priv = matrix_room_filter_get_instance_private(matrix_room_filter);

    matrix_json_compact_unref(MATRIX_JSON_COMPACT(priv->_ephemeral));
    priv->_ephemeral = (MatrixFilterRules *)matrix_json_compact_ref(MATRIX_JSON_COMPACT(ephemeral));
}

/**
 * matrix_room_filter_get_state:
 * @room_filter: a #MatrixRoomFilter object
 *
 * Get the filtering rules for state events.
 *
 * The returned value is owned by @room_filter and should not be freed.
 *
 * Returns: (transfer none) (nullable): the filtering rules for state events.
 */
MatrixFilterRules *
matrix_room_filter_get_state(MatrixRoomFilter *matrix_room_filter)
{
    MatrixRoomFilterPrivate *priv;

    g_return_val_if_fail(matrix_room_filter != NULL, NULL);

    priv = matrix_room_filter_get_instance_private(matrix_room_filter);

    return priv->_state;
}

/**
 * matrix_room_filter_set_state:
 * @room_filter: a #MatrixRoomFilter object
 * @state_rules: (transfer none) (nullable): a #MatrixFilterRules object to apply to state events
 *
 * Set filtering rules for state events.
 */
void
matrix_room_filter_set_state(MatrixRoomFilter *matrix_room_filter, MatrixFilterRules *state)
{
    MatrixRoomFilterPrivate *priv;

    g_return_if_fail(matrix_room_filter != NULL);

    priv = matrix_room_filter_get_instance_private(matrix_room_filter);

    matrix_json_compact_unref(MATRIX_JSON_COMPACT(priv->_state));
    priv->_state = (MatrixFilterRules *)matrix_json_compact_ref(MATRIX_JSON_COMPACT(state));
}

/**
 * matrix_room_filter_get_timeline:
 * @room_filter: a #MatrixRoomFilter object
 *
 * Get the filtering rules for timeline events (like messages).
 *
 * The returned value is owned by @room_filter, and shouldn’t be freed.
 *
 * Returns: (transfer none) (nullable): the filtering rules for timeline events.
 */
MatrixFilterRules *
matrix_room_filter_get_timeline(MatrixRoomFilter *matrix_room_filter)
{
    MatrixRoomFilterPrivate *priv;

    g_return_val_if_fail(matrix_room_filter != NULL, NULL);

    priv = matrix_room_filter_get_instance_private(matrix_room_filter);

    return priv->_timeline;
}

/**
 * matrix_room_filter_set_timeline:
 * @room_filter: a #MatrixRoomFilter object
 * @timeline_rules: (transfer none) (nullable): a #MatrixFilterRules to apply to timeline events
 *
 * Set filtering rules for timeline events.
 */
void
matrix_room_filter_set_timeline(MatrixRoomFilter *matrix_room_filter, MatrixFilterRules *timeline)
{
    MatrixRoomFilterPrivate *priv;

    g_return_if_fail(matrix_room_filter != NULL);

    priv = matrix_room_filter_get_instance_private(matrix_room_filter);

    matrix_json_compact_unref(MATRIX_JSON_COMPACT(priv->_timeline));
    priv->_timeline = (MatrixFilterRules *)matrix_json_compact_ref(MATRIX_JSON_COMPACT(timeline));
}

static void
matrix_room_filter_finalize(MatrixJsonCompact *matrix_json_compact)
{
    MatrixRoomFilterPrivate *priv;

    priv = matrix_room_filter_get_instance_private(MATRIX_ROOM_FILTER(matrix_json_compact));
    matrix_json_compact_unref(MATRIX_JSON_COMPACT(priv->_ephemeral));
    matrix_json_compact_unref(MATRIX_JSON_COMPACT(priv->_state));
    matrix_json_compact_unref(MATRIX_JSON_COMPACT(priv->_timeline));

    MATRIX_JSON_COMPACT_CLASS (matrix_room_filter_parent_class)->finalize (matrix_json_compact);
}

static void
matrix_room_filter_class_init(MatrixRoomFilterClass* klass)
{
    ((MatrixJsonCompactClass *)klass)->finalize = matrix_room_filter_finalize;
    ((MatrixJsonCompactClass *)klass)->get_json_node = matrix_room_filter_get_json_node;
}

static void
matrix_room_filter_init(MatrixRoomFilter *matrix_room_filter)
{
    MatrixRoomFilterPrivate *priv;

    priv = matrix_room_filter_get_instance_private(matrix_room_filter);

    priv->_include_leave = TRUE;
    priv->_ephemeral = NULL;
    priv->_state = NULL;
    priv->_timeline = NULL;
}

typedef struct {
    gchar **_event_fields;
    guint _event_fields_len;
    MatrixEventFormat _event_format;
    MatrixFilterRules *_presence_filter;
    MatrixRoomFilter*_room_filter;
} MatrixFilterPrivate;

/**
 * MatrixFilter:
 *
 * Class to hold a message filter, eg. to be used with matrix_api_sync().
 */
G_DEFINE_TYPE_WITH_PRIVATE(MatrixFilter, matrix_filter, MATRIX_TYPE_JSON_COMPACT);

static JsonNode *
matrix_filter_get_json_node(MatrixJsonCompact *matrix_json_compact, GError **error)
{
    MatrixFilterPrivate *priv;
    JsonBuilder *builder;
    JsonNode *node;
    GError *inner_error = NULL;

    g_return_val_if_fail(matrix_json_compact != NULL, NULL);

    priv = matrix_filter_get_instance_private(MATRIX_FILTER(matrix_json_compact));

    builder = json_builder_new();

    json_builder_begin_object(builder);

    json_builder_set_member_name(builder, "event_fields");
    json_builder_begin_array(builder);

    for (guint i = 0; i < priv->_event_fields_len; i++) {
        json_builder_add_string_value(builder, priv->_event_fields[i]);
    }

    json_builder_end_array(builder);

    json_builder_set_member_name(builder, "event_format");
    json_builder_add_string_value(builder, _matrix_g_enum_to_string(MATRIX_TYPE_EVENT_FORMAT, priv->_event_format, TRUE));

    json_builder_set_member_name(builder, "presence");
    node = matrix_json_compact_get_json_node(MATRIX_JSON_COMPACT(priv->_presence_filter), &inner_error);

    if (inner_error != NULL) {
        g_propagate_error(error, inner_error);
        g_object_unref(builder);

        return NULL;
    }
    json_builder_add_value(builder, node);

    json_builder_set_member_name(builder, "room");
    node = matrix_json_compact_get_json_node(MATRIX_JSON_COMPACT(priv->_room_filter), &inner_error);

    if (inner_error != NULL) {
        g_propagate_error(error, inner_error);
        g_object_unref(builder);

        return NULL;
    }

    json_builder_add_value(builder, node);

    json_builder_end_object(builder);

    node = json_builder_get_root(builder);

    g_object_unref(builder);

    return node;
}

MatrixFilter *
matrix_filter_construct(GType object_type)
{
    return (MatrixFilter *)matrix_json_compact_construct(object_type);
}

/**
 * matrix_filter_new:
 *
 * Create a new #MatrixFilter object.
 *
 * Returns: (transfer full): a new #MatrixFilter object
 */
MatrixFilter *
matrix_filter_new(void)
{
    return matrix_filter_construct(MATRIX_TYPE_FILTER);
}

/**
 * matrix_filter_get_event_fields:
 * @filter: a #MatrixFilter object
 * @n_event_fields: (nullable): placeholder for the length of the result, or %NULL to ignore
 *
 * Get the fields to be included in the filtered events.
 *
 * The returned value is owned by @filter and should not be freed.
 *
 * Returns: (transfer none): the list of fields to include
 */
gchar **
matrix_filter_get_event_fields(MatrixFilter *matrix_filter, int *n_event_fields)
{
    MatrixFilterPrivate *priv;

    g_return_val_if_fail(matrix_filter != NULL, NULL);

    priv = matrix_filter_get_instance_private(matrix_filter);

    if (n_event_fields != NULL) {
        *n_event_fields = priv->_event_fields_len;
    }

    return priv->_event_fields;
}

/**
 * matrix_filter_set_event_fields:
 * @filter: a #MatrixFilter object
 * @event_fields: (transfer none) (nullable): a list of event field names
 * @n_event_fields: the number of elements in @event_fields
 *
 * Set the event fields to be included in the filtered events.
 */
void
matrix_filter_set_event_fields(MatrixFilter *matrix_filter, gchar **event_fields, int n_event_fields)
{
    MatrixFilterPrivate *priv;

    g_return_if_fail(matrix_filter != NULL);

    priv = matrix_filter_get_instance_private(matrix_filter);

    free_str_array(priv->_event_fields, priv->_event_fields_len);
    priv->_event_fields = copy_str_array(event_fields, n_event_fields);
    priv->_event_fields_len = n_event_fields;
}

/**
 * matrix_filter_get_event_format:
 * @filter: a #MatrixFilter object
 *
 * Get the format that will be used to represent the filtered events.
 *
 * Returns: a #MatrixEventFormat value
 */
MatrixEventFormat
matrix_filter_get_event_format(MatrixFilter *matrix_filter)
{
    MatrixFilterPrivate *priv;

    g_return_val_if_fail(matrix_filter != NULL, 0);

    priv = matrix_filter_get_instance_private(matrix_filter);

    return priv->_event_format;
}

/**
 * matrix_filter_set_event_format:
 * @filter: a #MatrixFilter object
 * @event_format: the event format to use
 *
 * Set the desired event format for the filtered events (eg. for used with matrix_api_sync())
 */
void
matrix_filter_set_event_format(MatrixFilter *matrix_filter, MatrixEventFormat event_format)
{
    MatrixFilterPrivate *priv;

    g_return_if_fail(matrix_filter != NULL);

    priv = matrix_filter_get_instance_private(matrix_filter);

    priv->_event_format = event_format;
}

/**
 * matrix_filter_get_presence_filter:
 * @filter: a #MatrixFilter object
 *
 * Get the filtering ruleset for presence events.
 *
 * The returned value is owned by @filter and should not be freed.
 *
 * Returns: (transfer none) (nullable): a #MatrixFilterRules object
 */
MatrixFilterRules *
matrix_filter_get_presence_filter(MatrixFilter *matrix_filter)
{
    MatrixFilterPrivate *priv;

    g_return_val_if_fail(matrix_filter != NULL, NULL);

    priv = matrix_filter_get_instance_private(matrix_filter);

    return priv->_presence_filter;
}

/**
 * matrix_filter_set_presence_filter:
 * @filter: a #MatrixFilter object
 * @presence_filter: (transfer none): a #MatrixFilterRules object to be applied to presence events
 *
 * Set a filtering ruleset for presence events.
 */
void
matrix_filter_set_presence_filter(MatrixFilter *matrix_filter, MatrixFilterRules *presence_filter)
{
    MatrixFilterPrivate *priv;

    g_return_if_fail(matrix_filter != NULL);

    priv = matrix_filter_get_instance_private(matrix_filter);

    matrix_json_compact_unref(MATRIX_JSON_COMPACT(priv->_presence_filter));
    priv->_presence_filter = (MatrixFilterRules *)matrix_json_compact_ref(MATRIX_JSON_COMPACT(presence_filter));
}

/**
 * matrix_filter_get_room_filter:
 * @filter: a #MatrixFilter object
 *
 * Get the filtering ruleset to be applied to room events.
 *
 * The returned value is owned by @filter and should not be freed.
 *
 * Returns: (transfer none) (nullable): a #MatrixRoomFilter object
 */
MatrixRoomFilter *
matrix_filter_get_room_filter(MatrixFilter *matrix_filter)
{
    MatrixFilterPrivate *priv;

    g_return_val_if_fail(matrix_filter != NULL, NULL);

    priv = matrix_filter_get_instance_private(matrix_filter);

    return priv->_room_filter;
}

/**
 * matrix_filter_set_room_filter:
 * @filter: a #MatrixFilter object
 * @room_filter: a #MatrixRoomFilter to be applied to room events
 *
 * Set the filtering ruleset for room events.
 */
void
matrix_filter_set_room_filter(MatrixFilter *matrix_filter, MatrixRoomFilter *room_filter)
{
    MatrixFilterPrivate *priv;

    g_return_if_fail(matrix_filter != NULL);

    priv = matrix_filter_get_instance_private(matrix_filter);

    matrix_json_compact_unref(MATRIX_JSON_COMPACT(priv->_room_filter));
    priv->_room_filter = (MatrixRoomFilter *)matrix_json_compact_ref(MATRIX_JSON_COMPACT(room_filter));
}

static void
matrix_filter_finalize(MatrixJsonCompact *matrix_json_compact)
{
    MatrixFilterPrivate *priv = matrix_filter_get_instance_private(MATRIX_FILTER(matrix_json_compact));

    priv->_event_fields = (free_str_array(priv->_event_fields, priv->_event_fields_len), NULL);
    matrix_json_compact_unref(MATRIX_JSON_COMPACT(priv->_presence_filter));
    matrix_json_compact_unref(MATRIX_JSON_COMPACT(priv->_room_filter));

    MATRIX_JSON_COMPACT_CLASS(matrix_filter_parent_class)->finalize(matrix_json_compact);
}

static void
matrix_filter_class_init(MatrixFilterClass *klass)
{
    ((MatrixJsonCompactClass *)klass)->finalize = matrix_filter_finalize;
    ((MatrixJsonCompactClass *)klass)->get_json_node = matrix_filter_get_json_node;
}

static void
matrix_filter_init(MatrixFilter *matrix_filter)
{
    MatrixFilterPrivate *priv = matrix_filter_get_instance_private(matrix_filter);

    priv->_event_format = MATRIX_EVENT_FORMAT_CLIENT;
    priv->_presence_filter = NULL;
    priv->_room_filter = NULL;
}

typedef struct {
    gchar *_id_server;
    gchar *_session_id;
    gchar *_client_secret;
} Matrix3PidCredentialPrivate;

/**
 * Matrix3PidCredential:
 *
 * Class to hold 3rd party credential related data.
 */
G_DEFINE_TYPE_WITH_PRIVATE(Matrix3PidCredential, matrix_3pid_credential, MATRIX_TYPE_JSON_COMPACT);

static JsonNode *
matrix_3pid_credential_get_json_node(MatrixJsonCompact *matrix_json_compact, GError **error)
{
    Matrix3PidCredentialPrivate *priv;
    JsonBuilder *builder;
    JsonNode *node;

    g_return_val_if_fail(matrix_json_compact != NULL, NULL);

    priv = matrix_3pid_credential_get_instance_private(MATRIX_3PID_CREDENTIAL(matrix_json_compact));

    if ((priv->_id_server == NULL)
        || (priv->_session_id == NULL)
        || (priv->_client_secret == NULL)) {
        g_set_error(error, MATRIX_ERROR, MATRIX_ERROR_INCOMPLETE,
                    "All fields of a 3PID credential must be set!");

        return NULL;
    }

    builder = json_builder_new();

    json_builder_begin_object(builder);

    json_builder_set_member_name(builder, "id_server");
    json_builder_add_string_value(builder, priv->_id_server);

    json_builder_set_member_name(builder, "session_id");
    json_builder_add_string_value(builder, priv->_session_id);

    json_builder_set_member_name(builder, "client_secret");
    json_builder_add_string_value(builder, priv->_client_secret);

    json_builder_end_object(builder);

    node = json_builder_get_root(builder);
    g_object_unref(builder);

    return node;
}

Matrix3PidCredential *
matrix_3pid_credential_construct(GType object_type)
{
    return (Matrix3PidCredential *)matrix_json_compact_construct(object_type);
}

/**
 * matrix_3pid_credential_new:
 *
 * Create a new #Matrix3PidCredential object.
 *
 * Returns: (transfer full): a new #Matrix3PidCredential object
 */
Matrix3PidCredential *
matrix_3pid_credential_new(void)
{
    return matrix_3pid_credential_construct (MATRIX_TYPE_3PID_CREDENTIAL);
}

/**
 * matrix_3pid_credential_get_id_server:
 * @credential: a #Matrix3PidCredential object
 *
 * Get the address of the Identity Server that generated this credential.
 *
 * The returned value is owned by @credential, and should not be freed.
 *
 * Returns: (transfer none): the address of the Identity Server
 */
const gchar *
matrix_3pid_credential_get_id_server(Matrix3PidCredential *matrix_3pid_credential)
{
    Matrix3PidCredentialPrivate *priv;

    g_return_val_if_fail(matrix_3pid_credential != NULL, NULL);

    priv = matrix_3pid_credential_get_instance_private(matrix_3pid_credential);

    return priv->_id_server;
}

/**
 * matrix_3pid_credential_set_id_server:
 * @credential: a #Matrix3PidCredential object
 * @id_server: (transfer none): the address of the Identity Server
 *
 * Set the address of the Identity server for the 3rd party credential.
 */
void
matrix_3pid_credential_set_id_server(Matrix3PidCredential *matrix_3pid_credential, const gchar *id_server)
{
    Matrix3PidCredentialPrivate *priv;

    g_return_if_fail(matrix_3pid_credential != NULL);

    priv = matrix_3pid_credential_get_instance_private(matrix_3pid_credential);

    g_free(priv->_id_server);
    priv->_id_server = g_strdup(id_server);
}

/**
 * matrix_3pid_credential_get_session_id:
 * @credential: a #Matrix3PidCredential object
 *
 * Get the sessiod ID received from the Identity Server with this credential.
 *
 * The returned value is owned by @credential, and should not be freed.
 *
 * Returns: (transfer none): the session ID
 */
const gchar *
matrix_3pid_credential_get_session_id(Matrix3PidCredential *matrix_3pid_credential)
{
    Matrix3PidCredentialPrivate *priv;

    g_return_val_if_fail(matrix_3pid_credential != NULL, NULL);

    priv = matrix_3pid_credential_get_instance_private(matrix_3pid_credential);

    return priv->_session_id;
}

/**
 * matrix_3pid_credential_set_session_id:
 * @credential: a #Matrix3PidCredential object
 * @session_id: (transfer none): an Identity Server session ID
 *
 * Set the session ID to be used during communication with the Identity Server for @credential.
 */
void
matrix_3pid_credential_set_session_id(Matrix3PidCredential *matrix_3pid_credential, const gchar *session_id)
{
    Matrix3PidCredentialPrivate *priv;

    g_return_if_fail(matrix_3pid_credential != NULL);

    priv = matrix_3pid_credential_get_instance_private(matrix_3pid_credential);

    g_free(priv->_session_id);
    priv->_session_id = g_strdup(session_id);
}

/**
 * matrix_3pid_credential_get_client_secret:
 * @credential: a #Matrix3PidCredential object
 *
 * Get the client secret that was used in the session with the Identity Server.
 *
 * The returned value is owned by @credential, and should not be freed.
 *
 * Returns: (transfer none): the client secret
 */
const gchar *
matrix_3pid_credential_get_client_secret(Matrix3PidCredential *matrix_3pid_credential)
{
    Matrix3PidCredentialPrivate *priv;

    g_return_val_if_fail(matrix_3pid_credential != NULL, NULL);

    priv = matrix_3pid_credential_get_instance_private(matrix_3pid_credential);

    return priv->_client_secret;
}

/**
 * matrix_3pid_credential_set_client_secret:
 * @credential: a #Matrix3PidCredential object
 * @client_secret: (transfer none): an Identity Server client secret
 *
 * Set the client secret to be used in the session with the Identity Server.
 */
void
matrix_3pid_credential_set_client_secret(Matrix3PidCredential *matrix_3pid_credential, const gchar *client_secret)
{
    Matrix3PidCredentialPrivate *priv;

    g_return_if_fail(matrix_3pid_credential != NULL);

    priv = matrix_3pid_credential_get_instance_private(matrix_3pid_credential);

    g_free(priv->_client_secret);
    priv->_client_secret = g_strdup(client_secret);
}

static void
matrix_3pid_credential_finalize(MatrixJsonCompact* matrix_json_compact)
{
    Matrix3PidCredentialPrivate *priv = matrix_3pid_credential_get_instance_private(MATRIX_3PID_CREDENTIAL(matrix_json_compact));

    g_free(priv->_id_server);
    g_free(priv->_session_id);
    g_free(priv->_client_secret);

    MATRIX_JSON_COMPACT_CLASS(matrix_3pid_credential_parent_class)->finalize(matrix_json_compact);
}

static void
matrix_3pid_credential_class_init(Matrix3PidCredentialClass* klass)
{
    ((MatrixJsonCompactClass *)klass)->finalize = matrix_3pid_credential_finalize;
    ((MatrixJsonCompactClass *)klass)->get_json_node = matrix_3pid_credential_get_json_node;
}

static void
matrix_3pid_credential_init(Matrix3PidCredential* matrix_3pid_credential)
{
    Matrix3PidCredentialPrivate *priv = matrix_3pid_credential_get_instance_private(matrix_3pid_credential);

    priv->_id_server = NULL;
    priv->_session_id = NULL;
    priv->_client_secret = NULL;
}
