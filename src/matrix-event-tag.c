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

#include "matrix-event-tag.h"

/**
 * SECTION:matrix-event-tag
 * @short_description: Event handling class for `m.room.tag`
 *
 * This is the default event handler for `m.room.tag` events.
 */
typedef struct {
    GHashTable *tags;
} MatrixEventTagPrivate;

enum {
    PROP_0,
    PROP_TAGS,
    NUM_PROPS
};
static GParamSpec *matrix_event_tag_properties[NUM_PROPS];

/**
 * MatrixEventTag:
 *
 * Object structure
 */
G_DEFINE_TYPE_WITH_PRIVATE(MatrixEventTag, matrix_event_tag, MATRIX_EVENT_TYPE_BASE);

static void
matrix_event_tag_real_from_json(MatrixEventBase *matrix_event_base, JsonNode *json_data, GError **error)
{
    MatrixEventTagPrivate *priv = matrix_event_tag_get_instance_private(MATRIX_EVENT_TAG(matrix_event_base));
    JsonObject *root;
    JsonNode *content_node;
    JsonObject *content_root;
    JsonNode *node;
    GError *inner_error = NULL;

    g_return_if_fail(json_data != NULL);

    root = json_node_get_object(json_data);
    content_node = json_object_get_member(root, "content");
    content_root = json_node_get_object(content_node);

    if ((node = json_object_get_member(content_root, "tags")) != NULL) {
        JsonObjectIter iter;
        JsonObject *tags_root = json_node_get_object(node);
        const gchar *member_name;
        JsonNode *member_node;

        json_object_iter_init(&iter, tags_root);

        while (json_object_iter_next(&iter, &member_name, &member_node)) {
            if (priv->tags == NULL) {
                priv->tags = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, (GDestroyNotify)json_node_unref);
            }

            g_hash_table_replace(priv->tags, g_strdup(member_name), json_node_ref(member_node));
        }

        json_node_free(node);
    }

    MATRIX_EVENT_BASE_CLASS(matrix_event_tag_parent_class)->from_json(matrix_event_base, json_data, &inner_error);

    if (inner_error != NULL) {
        g_propagate_error(error, inner_error);
    }
}

static void
matrix_event_tag_real_to_json(MatrixEventBase *matrix_event_base, JsonNode *json_data, GError **error)
{
    MatrixEventTagPrivate *priv;
    GError *inner_error = NULL;

    priv = matrix_event_tag_get_instance_private(MATRIX_EVENT_TAG(matrix_event_base));

    if (priv->tags != NULL) {
        JsonObject *tags_root = json_object_new();
        GHashTableIter iter;
        gpointer key;
        gpointer value;

        g_hash_table_iter_init(&iter, priv->tags);

        while (g_hash_table_iter_next(&iter, &key, &value)) {
            gchar *tag = (gchar *)key;
            JsonNode *tag_contents = (JsonNode *)value;

            json_object_set_member(tags_root, tag, tag_contents);
        }

        if (json_object_get_size(tags_root) > 0) {
            JsonObject *root;
            JsonObject *content_root;
            JsonNode *content_node;
            JsonNode *tags_node;

            root = json_node_get_object(json_data);
            content_node = json_object_get_member(root, "content");
            content_root = json_node_get_object(content_node);
            tags_node = json_node_new(JSON_NODE_OBJECT);

            json_node_set_object(tags_node, tags_root);
            json_object_set_member(content_root, "tags", tags_node);
        }
    }

    MATRIX_EVENT_BASE_CLASS(matrix_event_tag_parent_class)->to_json(matrix_event_base, json_data, &inner_error);

    if (inner_error != NULL) {
        g_propagate_error(error, inner_error);
    }
}

MatrixEventTag *
matrix_event_tag_construct(GType object_type)
{
    return (MatrixEventTag *)matrix_event_base_construct(object_type);
}

/**
 * matrix_event_tag_new:
 *
 * Create a new #MatrixEventTag object
 *
 * Returns: (transfer full): a new #MatrixEventTag object
 */
MatrixEventTag *
matrix_event_tag_new(void)
{
    return matrix_event_tag_construct(MATRIX_EVENT_TYPE_TAG);
}

/**
 * matrix_event_tag_get_tags:
 * @event: a #MatrixEventTag
 *
 * Get the list of tags as a #GHashTable
 *
 * The returned value is owned by @event and should not be freed.
 *
 * Returns: (transfer none) (nullable): the list of tags
 */
GHashTable *
matrix_event_tag_get_tags(MatrixEventTag *event)
{
    MatrixEventTagPrivate *priv = matrix_event_tag_get_instance_private(event);

    g_return_val_if_fail(event != NULL, NULL);

    return priv->tags;
}

/**
 * matrix_event_tag_set_tags:
 * @event: a #MatrixEventTag
 * @tags: (transfer none) (nullable): a #GHashTable
 *
 * Set the tag list of @event.
 *
 * @tags, if not %NULL, must be a #GHashTable whose keys are strings, and values are
 * #JsonNode objects.
 *
 * @event creates a reference on @tags, so it can be released by the caller.
 */
void
matrix_event_tag_set_tags(MatrixEventTag *event, GHashTable *tags)
{
    MatrixEventTagPrivate *priv = matrix_event_tag_get_instance_private(event);

    g_return_if_fail(event != NULL);

    g_hash_table_unref(priv->tags);
    priv->tags = g_hash_table_ref(tags);
}

static void
matrix_event_tag_finalize(GObject *gobject)
{
    MatrixEventTagPrivate *priv = matrix_event_tag_get_instance_private(MATRIX_EVENT_TAG(gobject));

    g_hash_table_unref(priv->tags);

    G_OBJECT_CLASS(matrix_event_tag_parent_class)->finalize(gobject);
}

static void
matrix_event_tag_set_property(GObject *gobject, guint prop_id, const GValue *value, GParamSpec *pspec)
{
    switch (prop_id) {
        case PROP_TAGS:
            matrix_event_tag_set_tags(MATRIX_EVENT_TAG(gobject), g_value_get_boxed(value));

            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, pspec);

            break;
    }
}

static void
matrix_event_tag_get_property(GObject *gobject, guint prop_id, GValue *value, GParamSpec *pspec)
{
    switch (prop_id) {
        case PROP_TAGS:
            g_value_set_boxed(value, matrix_event_tag_get_tags(MATRIX_EVENT_TAG(gobject)));

            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, pspec);

            break;
    }
}

static void
matrix_event_tag_class_init(MatrixEventTagClass *klass)
{
    ((MatrixEventBaseClass *)klass)->from_json = matrix_event_tag_real_from_json;
    ((MatrixEventBaseClass *)klass)->to_json = matrix_event_tag_real_to_json;

    G_OBJECT_CLASS(klass)->finalize = matrix_event_tag_finalize;
    G_OBJECT_CLASS(klass)->get_property = matrix_event_tag_get_property;
    G_OBJECT_CLASS(klass)->set_property = matrix_event_tag_set_property;

    /**
     * MatrixEventTag:tags:
     *
     * The list of tags as a #GHashTable.
     */
    matrix_event_tag_properties[PROP_TAGS] = g_param_spec_boxed(
            "tags", "tags", "tags",
            G_TYPE_HASH_TABLE,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(G_OBJECT_CLASS(klass),
                                    PROP_TAGS,
                                    matrix_event_tag_properties[PROP_TAGS]);
}

static void
matrix_event_tag_init(MatrixEventTag *matrix_event_tag)
{
    MatrixEventTagPrivate *priv = matrix_event_tag_get_instance_private(matrix_event_tag);

    priv->tags = NULL;
}
