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

#include "matrix-event-room-aliases.h"
#include "matrix-types.h"
#include "config.h"

/**
 * SECTION:matrix-event-room-aliases
 * @short_description: event to list the aliases of a room
 *
 * This is the default event handler for `m.room.aliases` events.
 *
 * This event is sent by a homeserver directly to inform of changes to the list of aliases it
 * knows about for that room.
 *
 * The state_key for this event is set to the homeserver which owns the room alias.
 *
 * The entire set of known aliases for the room is the union of all the `m.room.aliases`
 * events, one for each homeserver.  Clients should check the validity of any room alias given
 * in this list before presenting it to the user as trusted fact.  The lists given by this
 * event should be considered simply as advice on which aliases might exist, for which the
 * client can perform the lookup to confirm whether it receives the correct room ID.
 */
enum  {
    PROP_0,
    PROP_ALIASES,
    NUM_PROPS
};

static GParamSpec* matrix_event_room_aliases_properties[NUM_PROPS];

typedef struct {
    gchar** _aliases;
    gint _aliases_len;
    gint __aliases_size_;
} MatrixEventRoomAliasesPrivate;

/**
 * MatrixEventRoomAliases:
 */
G_DEFINE_TYPE_WITH_PRIVATE(MatrixEventRoomAliases, matrix_event_room_aliases, MATRIX_EVENT_TYPE_STATE);

static void
matrix_event_room_aliases_real_from_json(MatrixEventBase *matrix_event_base, JsonNode *json_data, GError **error)
{
    MatrixEventRoomAliasesPrivate *priv;
    JsonObject *root;
    JsonObject *content_root;
    JsonNode *content_node;
    JsonNode *node;
    GError *inner_error = NULL;

    g_return_if_fail (json_data != NULL);

    priv = matrix_event_room_aliases_get_instance_private(MATRIX_EVENT_ROOM_ALIASES(matrix_event_base));
    root = json_node_get_object(json_data);
    content_node = json_object_get_member(root, "content");
    content_root = json_node_get_object(content_node);

    if ((node = json_object_get_member(content_root, "aliases")) != NULL) {
        JsonArray *aliases;
        gint n_aliases;

        aliases = json_node_get_array(node);
        n_aliases = json_array_get_length(aliases);

        for (gint i = 0; i < priv->_aliases_len; i++) {
            g_free(priv->_aliases[i]);
        }
        g_free(priv->_aliases);

        priv->_aliases = g_new(gchar *, n_aliases);

        for (gint i = 0; i < n_aliases; i++) {
            JsonNode *element = json_array_get_element(aliases, i);

            priv->_aliases[i] = g_strdup(json_node_get_string(element));
        }
    } else if (DEBUG) {
        g_warning("content.aliases is missing from a m.room.aliases event");
    }

    MATRIX_EVENT_BASE_CLASS(matrix_event_room_aliases_parent_class)->from_json(matrix_event_base, json_data, &inner_error);

    if (inner_error != NULL) {
        g_propagate_error(error, inner_error);
    }
}

static void
matrix_event_room_aliases_real_to_json(MatrixEventBase *matrix_event_base, JsonNode *json_data, GError **error)
{
    MatrixEventRoomAliasesPrivate *priv;
    JsonObject *root;
    JsonObject *content_root;
    JsonArray *aliases_ary;
    JsonNode *content_node;
    JsonNode *aliases_node;
    GError *inner_error = NULL;

    priv = matrix_event_room_aliases_get_instance_private(MATRIX_EVENT_ROOM_ALIASES(matrix_event_base));

    if (priv->_aliases_len == 0) {
        g_set_error(error, MATRIX_ERROR, MATRIX_ERROR_INCOMPLETE,
                    "Won't generate a m.room.aliases event without aliases");

        return;
    }

    root = json_node_get_object(json_data);
    content_node = json_object_get_member(root, "content");
    content_root = json_node_get_object(content_node);

    aliases_ary = json_array_new();

    for (gint i = 0; i < priv->_aliases_len; i++) {
        json_array_add_string_element(aliases_ary, priv->_aliases[i]);
    }

    aliases_node = json_node_new(JSON_TYPE_ARRAY);
    json_node_set_array(aliases_node, aliases_ary);
    json_object_set_member(content_root, "aliases", aliases_node);

    MATRIX_EVENT_BASE_CLASS(matrix_event_room_aliases_parent_class)->to_json(matrix_event_base, json_data, &inner_error);

    if (inner_error != NULL) {
        g_propagate_error(error, inner_error);
    }
}

/**
 * matrix_event_room_aliases_new:
 *
 * Create a new #MatrixEventRoomAliases object.
 *
 * Returns: (transfer full): a new #MatrixEventRoomAliases object
 */
MatrixEventRoomAliases *
matrix_event_room_aliases_new(void)
{
    return (MatrixEventRoomAliases *)matrix_event_state_construct(MATRIX_EVENT_TYPE_ROOM_ALIASES);
}

/**
 * matrix_event_room_aliases_get_aliases:
 * @event: a MatrixEventRoomAliases
 * @n_aliases: placeholder for the length of the list, or %NULL to ignore
 *
 * Get the list of aliases from @event.
 *
 * The returned value is owned by @event and should not be freed.
 *
 * Returns: (transfer none) (nullable): the list of aliases
 */
const gchar **
matrix_event_room_aliases_get_aliases(MatrixEventRoomAliases *matrix_event_room_aliases, int *n_aliases)
{
    MatrixEventRoomAliasesPrivate *priv;

    g_return_val_if_fail(matrix_event_room_aliases != NULL, NULL);

    priv = matrix_event_room_aliases_get_instance_private(matrix_event_room_aliases);

    if (n_aliases != NULL) {
        *n_aliases = priv->_aliases_len;
    }

    return (const gchar **)priv->_aliases;
}

/**
 * matrix_event_room_aliases_set_aliases:
 * @event: a #MatrixEventRoomAliases
 * @aliases: a list of aliases
 * @n_aliases: the length of @aliases
 *
 * Set the list of aliases in @event.
 */
void
matrix_event_room_aliases_set_aliases(MatrixEventRoomAliases *matrix_event_room_aliases, const gchar **aliases, int n_aliases)
{
    MatrixEventRoomAliasesPrivate *priv;

    g_return_if_fail(matrix_event_room_aliases != NULL);

    priv = matrix_event_room_aliases_get_instance_private(matrix_event_room_aliases);

    for (gint i = 0; i < priv->_aliases_len; i++) {
        g_free(priv->_aliases[i]);
    }

    g_free(priv->_aliases);
    priv->_aliases = g_new(gchar *, n_aliases);

    for (gint i = 0; i < n_aliases; i++) {
        priv->_aliases[i] = g_strdup(aliases[i]);
    }
}

static void
matrix_event_room_aliases_finalize(GObject *gobject)
{
    MatrixEventRoomAliasesPrivate *priv = matrix_event_room_aliases_get_instance_private(MATRIX_EVENT_ROOM_ALIASES(gobject));

    for (gint i = 0; i < priv->_aliases_len; i++) {
        g_free(priv->_aliases[i]);
    }

    g_free(priv->_aliases);

    G_OBJECT_CLASS(matrix_event_room_aliases_parent_class)->finalize(gobject);
}

static void
matrix_event_room_aliases_get_property(GObject *gobject, guint property_id, GValue *value, GParamSpec *pspec)
{
    MatrixEventRoomAliases *self = MATRIX_EVENT_ROOM_ALIASES(gobject);

    switch (property_id) {
        case PROP_ALIASES:
            g_value_set_boxed(value, matrix_event_room_aliases_get_aliases(self, NULL));

            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, property_id, pspec);

            break;
    }
}

static void
matrix_event_room_aliases_set_property(GObject *gobject, guint property_id, const GValue *value, GParamSpec *pspec)
{
    MatrixEventRoomAliases *self = MATRIX_EVENT_ROOM_ALIASES(gobject);

    switch (property_id) {
        case PROP_ALIASES:
        {
            gpointer boxed;

            boxed = g_value_get_boxed(value);
            matrix_event_room_aliases_set_aliases(self, boxed, (boxed == NULL) ? 0 : g_strv_length (boxed));

            break;
        }
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, property_id, pspec);

            break;
    }
}

static void
matrix_event_room_aliases_class_init(MatrixEventRoomAliasesClass *klass)
{
    ((MatrixEventBaseClass *)klass)->from_json =  matrix_event_room_aliases_real_from_json;
    ((MatrixEventBaseClass *)klass)->to_json =  matrix_event_room_aliases_real_to_json;
    G_OBJECT_CLASS (klass)->get_property = matrix_event_room_aliases_get_property;
    G_OBJECT_CLASS (klass)->set_property = matrix_event_room_aliases_set_property;
    G_OBJECT_CLASS (klass)->finalize = matrix_event_room_aliases_finalize;

    /**
     * MatrixEventRoomAliases:aliases:
     *
     * A list of room aliases.
     */
    matrix_event_room_aliases_properties[PROP_ALIASES] = g_param_spec_boxed(
            "aliases", "aliases", "aliases",
            G_TYPE_STRV,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_ALIASES, matrix_event_room_aliases_properties[PROP_ALIASES]);
}

static void
matrix_event_room_aliases_init(MatrixEventRoomAliases *matrix_event_room_aliases)
{}
