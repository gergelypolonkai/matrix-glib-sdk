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

#include "matrix-event-typing.h"
#include "matrix-types.h"
#include "config.h"

/**
 * SECTION:matrix-event-typing
 * @short_description: event to inform clients of the typing users
 *
 * This is the default event handler for `m.typing` events.
 *
 * Informs the client of the list of users currently typing.
 */
enum  {
    PROP_0,
    PROP_ROOM_ID,
    PROP_USER_IDS,
    NUM_PROPS
};

static GParamSpec* matrix_event_typing_properties[NUM_PROPS];

typedef struct {
    gchar* _room_id;
    gchar** _user_ids;
    gint _user_ids_len;
    gint __user_ids_size_;
} MatrixEventTypingPrivate;

/**
 * MatrixEventTyping:
 */
G_DEFINE_ABSTRACT_TYPE_WITH_PRIVATE(MatrixEventTyping, matrix_event_typing, MATRIX_EVENT_TYPE_BASE);

static void
set_user_id(JsonArray *ary, guint idx, JsonNode *member_node, gpointer user_data) {
    gchar **user_ids = (gchar **)user_data;

    user_ids[idx] = g_strdup(json_node_get_string(member_node));
}

static void
matrix_event_typing_real_from_json(MatrixEventBase *matrix_event_base, JsonNode *json_data, GError **error)
{
    MatrixEventTypingPrivate *priv = matrix_event_typing_get_instance_private(MATRIX_EVENT_TYPING(matrix_event_base));
    JsonObject *root;
    JsonObject *content_root;
    JsonNode *content_node;
    JsonNode *node;
    GError *inner_error = NULL;

    root = json_node_get_object(json_data);
    content_node = json_object_get_member(root, "content");
    content_root = json_node_get_object(content_node);

    if ((node = json_object_get_member(root, "room_id")) != NULL) {
        g_free(priv->_room_id);
        priv->_room_id = g_strdup(json_node_get_string(node));
    } else if (DEBUG) {
        g_warning("room_id is missing from a m.typing event");
    }

    if ((node = json_object_get_member(content_root, "user_ids")) != NULL) {
        JsonArray *user_ids_ary = json_node_get_array(node);
        gint user_ids_len = json_array_get_length(user_ids_ary);

        for (int i = 0; i < priv->_user_ids_len; i++) {
            g_free(priv->_user_ids[i]);
        }
        g_free(priv->_user_ids);

        priv->_user_ids = g_new(gchar *, user_ids_len);

        for (int i = 0; i < user_ids_len; i++) {
            json_array_foreach_element(user_ids_ary, set_user_id, priv->_user_ids);
        }

        priv->_user_ids_len = user_ids_len;
    } else if (DEBUG) {
        g_warning("content.user_ids is missing from a m.typing event");
    }

    MATRIX_EVENT_BASE_CLASS(matrix_event_typing_parent_class)->from_json(matrix_event_base, json_data, &inner_error);

    if (inner_error != NULL) {
        g_propagate_error(error, inner_error);
    }
}

static void
matrix_event_typing_real_to_json(MatrixEventBase *matrix_event_base, JsonNode *json_data, GError **error)
{
    MatrixEventTypingPrivate *priv = matrix_event_typing_get_instance_private(MATRIX_EVENT_TYPING(matrix_event_base));
    JsonObject *root;
    JsonObject *content_root;
    JsonArray *user_ids_ary;
    JsonNode *content_node;
    JsonNode *user_ids_node;
    GError *inner_error = NULL;

    if (priv->_room_id == NULL) {
        g_set_error(error, MATRIX_ERROR, MATRIX_ERROR_INCOMPLETE,
                    "Won't generate a m.typing event without room_id");

        return;
    }

    root = json_node_get_object(json_data);
    content_node = json_object_get_member(root, "content");
    content_root = json_node_get_object(content_node);

    json_object_set_string_member(root, "room_id", priv->_room_id);

    user_ids_ary = json_array_new();
    user_ids_node = json_node_new(JSON_NODE_ARRAY);
    json_object_set_member(content_root, "user_ids", user_ids_node);
    json_node_set_array(user_ids_node, user_ids_ary);

    for (gint i = 0; i < priv->_user_ids_len; i++) {
        json_array_add_string_element(user_ids_ary, priv->_user_ids[i]);
    }

    MATRIX_EVENT_BASE_CLASS(matrix_event_typing_parent_class)->to_json(matrix_event_base, json_data, &inner_error);

    if (inner_error != NULL) {
        g_propagate_error (error, inner_error);
    }
}

MatrixEventTyping *
matrix_event_typing_new(void)
{
    return (MatrixEventTyping *)matrix_event_base_construct(MATRIX_EVENT_TYPE_TYPING);
}

const gchar *
matrix_event_typing_get_room_id(MatrixEventTyping *matrix_event_typing)
{
    MatrixEventTypingPrivate *priv;

    g_return_val_if_fail(matrix_event_typing != NULL, NULL);

    priv = matrix_event_typing_get_instance_private(matrix_event_typing);

    return priv->_room_id;
}

void
matrix_event_typing_set_room_id(MatrixEventTyping *matrix_event_typing, const gchar *room_id)
{
    MatrixEventTypingPrivate *priv;

    g_return_if_fail(matrix_event_typing != NULL);

    priv = matrix_event_typing_get_instance_private(matrix_event_typing);

    if (g_strcmp0(room_id, priv->_room_id) != 0) {
        g_free(priv->_room_id);
        priv->_room_id = g_strdup(room_id);

        g_object_notify_by_pspec((GObject *)matrix_event_typing, matrix_event_typing_properties[PROP_ROOM_ID]);
    }
}

/**
 * matrix_event_typing_get_user_ids:
 * @event: a #MatrixEventTyping
 * @n_user_ids: (nullable): placeholder for the length of the returned list
 *
 * Returns: (transfer none): a list of typing users
 */
gchar **
matrix_event_typing_get_user_ids(MatrixEventTyping *matrix_event_typing, int *n_user_ids)
{
    MatrixEventTypingPrivate *priv;

    g_return_val_if_fail(matrix_event_typing != NULL, NULL);

    priv = matrix_event_typing_get_instance_private(matrix_event_typing);

    if (n_user_ids != NULL) {
        *n_user_ids = priv->_user_ids_len;
    }

    return priv->_user_ids;
}

void
matrix_event_typing_set_user_ids(MatrixEventTyping *matrix_event_typing, gchar **user_ids, int n_user_ids)
{
    MatrixEventTypingPrivate *priv;

    g_return_if_fail(matrix_event_typing != NULL);

    priv = matrix_event_typing_get_instance_private(matrix_event_typing);

    if (priv->_user_ids != user_ids) {
        for (gint i = 0; i < priv->_user_ids_len; i++) {
            g_free(priv->_user_ids[i]);
        }

        g_free(priv->_user_ids);

        priv->_user_ids = g_new(gchar *, n_user_ids);

        for (gint i = 0; i < n_user_ids; i++) {
            priv->_user_ids[i] = g_strdup(user_ids[i]);
        }

        priv->_user_ids_len = n_user_ids;

        g_object_notify_by_pspec((GObject *)matrix_event_typing, matrix_event_typing_properties[PROP_USER_IDS]);
    }
}

static void
matrix_event_typing_finalize(GObject *gobject)
{
    MatrixEventTypingPrivate *priv = matrix_event_typing_get_instance_private(MATRIX_EVENT_TYPING(gobject));

    g_free(priv->_room_id);

    for (gint i = 0; i < priv->_user_ids_len; i++) {
        g_free(priv->_user_ids[i]);
    }

    priv->_user_ids = (g_free(priv->_user_ids), NULL);

    G_OBJECT_CLASS(matrix_event_typing_parent_class)->finalize(gobject);
}

static void
matrix_event_typing_get_property(GObject *gobject, guint property_id, GValue* value, GParamSpec* pspec)
{
    MatrixEventTyping *matrix_event_typing = G_TYPE_CHECK_INSTANCE_CAST(gobject, MATRIX_EVENT_TYPE_TYPING, MatrixEventTyping);

    switch (property_id) {
        case PROP_ROOM_ID:
            g_value_set_string(value, matrix_event_typing_get_room_id(matrix_event_typing));

            break;
        case PROP_USER_IDS:
            g_value_set_boxed(value, matrix_event_typing_get_user_ids(matrix_event_typing, NULL));

            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, property_id, pspec);

            break;
    }
}

static void
matrix_event_typing_set_property(GObject *gobject, guint property_id, const GValue *value, GParamSpec* pspec)
{
    MatrixEventTyping *matrix_event_typing = G_TYPE_CHECK_INSTANCE_CAST(gobject, MATRIX_EVENT_TYPE_TYPING, MatrixEventTyping);

    switch (property_id) {
        case PROP_ROOM_ID:
            matrix_event_typing_set_room_id(matrix_event_typing, g_value_get_string (value));

            break;
        case PROP_USER_IDS:
        {
            gpointer boxed = g_value_get_boxed(value);

            matrix_event_typing_set_user_ids(matrix_event_typing, boxed, (boxed == NULL) ? 0 : g_strv_length(boxed));

            break;
        }
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, property_id, pspec);

            break;
    }
}

static void
matrix_event_typing_class_init(MatrixEventTypingClass *klass)
{
    ((MatrixEventBaseClass *)klass)->from_json = matrix_event_typing_real_from_json;
    ((MatrixEventBaseClass *)klass)->to_json = matrix_event_typing_real_to_json;
    G_OBJECT_CLASS(klass)->get_property = matrix_event_typing_get_property;
    G_OBJECT_CLASS(klass)->set_property = matrix_event_typing_set_property;
    G_OBJECT_CLASS(klass)->finalize = matrix_event_typing_finalize;

    /**
     * MatrixEventTyping:room-id:
     *
     * The ID of the room this event belongs to.
     */
    matrix_event_typing_properties[PROP_ROOM_ID] = g_param_spec_string(
            "room-id", "room-id", "room-id",
            NULL,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_ROOM_ID, matrix_event_typing_properties[PROP_ROOM_ID]);

    /**
     * MatrixEventTyping:user-ids:
     *
     * The list of user IDs typing in this room, if any.
     */
    matrix_event_typing_properties[PROP_USER_IDS] = g_param_spec_boxed(
            "user-ids", "user-ids", "user-ids",
            G_TYPE_STRV,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_USER_IDS, matrix_event_typing_properties[PROP_USER_IDS]);
}

static void
matrix_event_typing_init(MatrixEventTyping *matrix_event_typing)
{
    MatrixEventTypingPrivate *priv = matrix_event_typing_get_instance_private(matrix_event_typing);

    priv->_room_id = NULL;
}
