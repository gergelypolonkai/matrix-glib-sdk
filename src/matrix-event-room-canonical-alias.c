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

#include "matrix-event-room-canonical-alias.h"
#include "matrix-enumtypes.h"
#include "config.h"

/**
 * SECTION:matrix-event-room-canonical-alias
 * @short_description: event to set a room’s canonical alias
 *
 * This is the default handler for `m.room.canonical_alias` events.
 *
 * This event is used to inform the room about which alias should be considered the canonical
 * one.  This could be for display purposes or as suggestion to users which alias to use to
 * advertise the room.
 */
enum  {
    PROP_0,
    PROP_CANONICAL_ALIAS,
    NUM_PROPERTIES
};

static GParamSpec *matrix_event_room_canonical_alias_properties[NUM_PROPERTIES];

typedef struct {
    gchar* _canonical_alias;
} MatrixEventRoomCanonicalAliasPrivate;

/**
 * MatrixEventRoomCanonicalAlias:
 */
G_DEFINE_TYPE_WITH_PRIVATE(MatrixEventRoomCanonicalAlias, matrix_event_room_canonical_alias, MATRIX_EVENT_TYPE_STATE);

static void
matrix_event_room_canonical_alias_real_from_json(MatrixEventBase *matrix_event_base, JsonNode *json_data, GError **error)
{
    MatrixEventRoomCanonicalAliasPrivate *priv;
    JsonObject *root;
    JsonObject *content_root;
    JsonNode *content_node;
    JsonNode *node;

    g_return_if_fail (json_data != NULL);

    priv = matrix_event_room_canonical_alias_get_instance_private(MATRIX_EVENT_ROOM_CANONICAL_ALIAS(matrix_event_base));

    root = json_node_get_object(json_data);
    content_node = json_object_get_member(root, "content");
    content_root = json_node_get_object(content_node);

#if DEBUG
    if ((node = json_object_get_member(root, "state_key")) != NULL) {
        const gchar *state_key = json_node_get_string(node);

        if ((state_key == NULL) || (*state_key == 0)) {
            g_warning("state_key of a m.room.canonical_alias event is non-empty");
        }
    }
#endif

    if ((node = json_object_get_member(content_root, "alias")) != NULL) {
        g_free(priv->_canonical_alias);
        priv->_canonical_alias = g_strdup(json_node_get_string(node));
    }

    MATRIX_EVENT_BASE_CLASS(matrix_event_room_canonical_alias_parent_class)->from_json(matrix_event_base, json_data, error);
}

static void
matrix_event_room_canonical_alias_real_to_json(MatrixEventBase *matrix_event_base, JsonNode *json_data, GError **error)
{
    MatrixEventRoomCanonicalAliasPrivate *priv;
    JsonObject *root;
    JsonObject *content_root;
    JsonNode *content_node;
    const gchar *state_key;

    g_return_if_fail(json_data != NULL);

    priv = matrix_event_room_canonical_alias_get_instance_private(MATRIX_EVENT_ROOM_CANONICAL_ALIAS(matrix_event_base));
    root = json_node_get_object(json_data);
    content_node = json_object_get_member(root, "content");
    content_root = json_node_get_object(content_node);

    state_key = matrix_event_state_get_state_key(MATRIX_EVENT_STATE(matrix_event_base));
    if ((state_key == NULL) || (*state_key == 0)) {
        g_set_error(error, MATRIX_ERROR, MATRIX_ERROR_INCOMPLETE,
                    "Won't generate a m.room.canonical_alias event with a non-empty state_key");

        return;
    }

    if (priv->_canonical_alias != NULL) {
        json_object_set_string_member(content_root, "alias", priv->_canonical_alias);
    }

    MATRIX_EVENT_BASE_CLASS(matrix_event_room_canonical_alias_parent_class)->to_json(matrix_event_base, json_data, error);
}

MatrixEventRoomCanonicalAlias *
matrix_event_room_canonical_alias_construct(GType object_type)
{
    return (MatrixEventRoomCanonicalAlias *)matrix_event_state_construct(object_type);
}

/**
 * matrix_event_room_canonical_alias_new:
 *
 * Create a new #MatrixEventRoomCanonicalAlias object
 *
 * Returns: (transfer full): a new #MatrixEventRoomCanonicalAlias
 */
MatrixEventRoomCanonicalAlias *
matrix_event_room_canonical_alias_new(void)
{
    return matrix_event_room_canonical_alias_construct(MATRIX_EVENT_TYPE_ROOM_CANONICAL_ALIAS);
}

/**
 * matrix_event_room_canonical_alias_get_canonical_alias:
 * @event: a #MatrixEventRoomCanonicalAlias
 *
 * Get the canonical alias from @event.
 *
 * The returned value is owned by @event and should not be freed.
 *
 * Returns: (transfer none) (nullable): the canonical alias
 */
const gchar *
matrix_event_room_canonical_alias_get_canonical_alias(MatrixEventRoomCanonicalAlias *matrix_event_room_canonical_alias)
{
    MatrixEventRoomCanonicalAliasPrivate *priv;

    g_return_val_if_fail(matrix_event_room_canonical_alias != NULL, NULL);

    priv = matrix_event_room_canonical_alias_get_instance_private(matrix_event_room_canonical_alias);

    return priv->_canonical_alias;
}

/**
 * matrix_event_room_canonical_alias_set_canonical_alias:
 * @event: a #MatrixEventRoomCanonicalAlias
 * @canonical_alias: (transfer none) (nullable): a canonical alias
 *
 * Set the canonical alias in @event.
 */
void
matrix_event_room_canonical_alias_set_canonical_alias(MatrixEventRoomCanonicalAlias *matrix_event_room_canonical_alias, const gchar *canonical_alias)
{
    MatrixEventRoomCanonicalAliasPrivate *priv;

    g_return_if_fail(matrix_event_room_canonical_alias != NULL);

    priv = matrix_event_room_canonical_alias_get_instance_private(matrix_event_room_canonical_alias);

    if (g_strcmp0(canonical_alias, priv->_canonical_alias) != 0) {
        g_free(priv->_canonical_alias);
        priv->_canonical_alias = g_strdup(canonical_alias);

        g_object_notify_by_pspec((GObject *)matrix_event_room_canonical_alias, matrix_event_room_canonical_alias_properties[PROP_CANONICAL_ALIAS]);
    }
}

static void
matrix_event_room_canonical_alias_finalize(GObject *gobject)
{
    MatrixEventRoomCanonicalAliasPrivate *priv = matrix_event_room_canonical_alias_get_instance_private(MATRIX_EVENT_ROOM_CANONICAL_ALIAS(gobject));

    g_free(priv->_canonical_alias);

    G_OBJECT_CLASS(matrix_event_room_canonical_alias_parent_class)->finalize(gobject);
}

static void
matrix_event_room_canonical_alias_get_property(GObject *gobject, guint property_id, GValue *value, GParamSpec *pspec)
{
    MatrixEventRoomCanonicalAlias *matrix_event_room_canonical_alias = MATRIX_EVENT_ROOM_CANONICAL_ALIAS(gobject);

    switch (property_id) {
        case PROP_CANONICAL_ALIAS:
            g_value_set_string(value, matrix_event_room_canonical_alias_get_canonical_alias(matrix_event_room_canonical_alias));

            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, property_id, pspec);

            break;
    }
}

static void
matrix_event_room_canonical_alias_set_property(GObject *gobject, guint property_id, const GValue *value, GParamSpec *pspec)
{
    MatrixEventRoomCanonicalAlias *matrix_event_room_canonical_alias = MATRIX_EVENT_ROOM_CANONICAL_ALIAS(gobject);

    switch (property_id) {
        case PROP_CANONICAL_ALIAS:
            matrix_event_room_canonical_alias_set_canonical_alias(matrix_event_room_canonical_alias, g_value_get_string(value));

            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, property_id, pspec);
        break;
    }
}

static void
matrix_event_room_canonical_alias_class_init(MatrixEventRoomCanonicalAliasClass *klass)
{
    ((MatrixEventBaseClass *)klass)->from_json = matrix_event_room_canonical_alias_real_from_json;
    ((MatrixEventBaseClass *)klass)->to_json = matrix_event_room_canonical_alias_real_to_json;
    G_OBJECT_CLASS(klass)->get_property = matrix_event_room_canonical_alias_get_property;
    G_OBJECT_CLASS(klass)->set_property = matrix_event_room_canonical_alias_set_property;
    G_OBJECT_CLASS(klass)->finalize = matrix_event_room_canonical_alias_finalize;

    /**
     * MatrixEventRoomCanonicalAlias:canonical-alias:
     *
     * The canonical alias.
     */
    matrix_event_room_canonical_alias_properties[PROP_CANONICAL_ALIAS] = g_param_spec_string(
            "canonical-alias", "canonical-alias", "canonical-alias",
            NULL,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_CANONICAL_ALIAS, matrix_event_room_canonical_alias_properties[PROP_CANONICAL_ALIAS]);
}

static void
matrix_event_room_canonical_alias_init(MatrixEventRoomCanonicalAlias *matrix_event_room_canonical_alias)
{
    MatrixEventRoomCanonicalAliasPrivate *priv = matrix_event_room_canonical_alias_get_instance_private(matrix_event_room_canonical_alias);

    priv->_canonical_alias = NULL;
}
