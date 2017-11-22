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

#include "matrix-event-room-power-levels.h"
#include "matrix-enumtypes.h"
#include "config.h"

/**
 * SECTION:matrix-event-room-power-levels
 * @short_description: event to control power level assignments
 *
 * This is the default event handler for `m.room.power_levels` events.
 *
 * This event specifies the minimum level a user must have in order to perform a certain
 * action.  It also specifies the levels of each user in the room.  If a `user_id` is in the
 * users list, then that `user_id` has the associated power level.  Otherwise they have the
 * default level set in `users_default`.  If `users_default` is not supplied, it is assumed to
 * be 0.  The level required to send a certain event is governed by the keys `events`,
 * `state_default` and `events_default`.  If an event type is specified in events, then the
 * user must have at least the level specified in order to send that event.  If the event type
 * is not supplied, it defaults to `events_default` for message events and `state_default` for
 * state events.
 */
enum  {
    PROP_0,
    PROP_USERS_DEFAULT,
    PROP_EVENTS_DEFAULT,
    PROP_STATE_DEFAULT,
    PROP_BAN,
    PROP_KICK,
    PROP_REDACT,
    PROP_INVITE,
    PROP_EVENT_LEVELS,
    PROP_USER_LEVELS,
    NUM_PROPERTIES
};

static GParamSpec *matrix_event_room_power_levels_properties[NUM_PROPERTIES];

typedef struct {
    gint _users_default;
    gint _events_default;
    gint _state_default;
    gint _ban;
    gint _kick;
    gint _redact;
    gint _invite;
    GHashTable* _event_levels;
    GHashTable* _user_levels;
} MatrixEventRoomPowerLevelsPrivate;

/**
 * MatrixEventRoomPowerLevels:
 */
G_DEFINE_TYPE_WITH_PRIVATE(MatrixEventRoomPowerLevels, matrix_event_room_power_levels, MATRIX_EVENT_TYPE_STATE);

static void
matrix_event_room_power_levels_real_from_json(MatrixEventBase *matrix_event_base, JsonNode *json_data, GError **error)
{
    MatrixEventRoomPowerLevelsPrivate *priv;
    JsonObject *root;
    JsonObject *content_root;
    JsonNode *content_node;
    JsonNode *node;

    g_return_if_fail (json_data != NULL);

    priv = matrix_event_room_power_levels_get_instance_private(MATRIX_EVENT_ROOM_POWER_LEVELS(matrix_event_base));
    root = json_node_get_object(json_data);
    content_node = json_object_get_member(root, "content");
    content_root = json_node_get_object(content_node);

#if DEBUG
    if ((node = json_object_get_member(root, "state_key")) != NULL) {
        const gchar *state_key = json_node_get_string(node);

        if ((state_key == NULL) || (*state_key == 0)) {
            g_warning("state_key of a m.room.power_levels event is non-empty");
        }
    }
#endif

    if ((node = json_object_get_member(content_root, "ban")) != NULL) {
        priv->_ban = json_node_get_int(node);
    } else {
        g_warning("content.ban is missing from a m.room.power_levels event");
    }

    if ((node = json_object_get_member(content_root, "kick")) != NULL) {
        priv->_kick = json_node_get_int(node);
    } else {
        g_warning("content.kick is missing from a m.room.power_levels event");
    }

    if ((node = json_object_get_member(content_root, "redact")) != NULL) {
        priv->_redact = json_node_get_int(node);
    } else {
        g_warning("content.redact is missing from a m.room.power_levels event");
    }

    if ((node = json_object_get_member(content_root, "events_default")) != NULL) {
        priv->_events_default = json_node_get_int(node);
    } else {
        g_warning("content.events_default is missing from a m.room.power_levels event");
    }

    if ((node = json_object_get_member(content_root, "state_default")) != NULL) {
        priv->_state_default = json_node_get_int(node);
    } else {
        g_warning("content.state_default is missing from a m.room.power_levels event");
    }

    if ((node = json_object_get_member(content_root, "users_default")) != NULL) {
        priv->_users_default = json_node_get_int(node);
    }

    if ((node = json_object_get_member(content_root, "invite")) != NULL) {
        priv->_invite = json_node_get_int(node);
    }

    if ((node = json_object_get_member(content_root, "events")) != NULL) {
        JsonObject *events_root;
        JsonObjectIter iter;
        const gchar *event_name;
        JsonNode *event_node;

        g_hash_table_remove_all(priv->_event_levels);

        events_root = json_node_get_object(node);
        json_object_iter_init(&iter, events_root);

        while (json_object_iter_next(&iter, &event_name, &event_node)) {
            g_hash_table_insert(priv->_event_levels, g_strdup(event_name), GINT_TO_POINTER(json_node_get_int(event_node)));
        }
    }

    if ((node = json_object_get_member(content_root, "users")) != NULL) {
        JsonObject *users_root;
        JsonObjectIter iter;
        const gchar *user_id;
        JsonNode *user_node;

        g_hash_table_remove_all(priv->_user_levels);

        users_root = json_node_get_object(node);
        json_object_iter_init(&iter, users_root);

        while (json_object_iter_next(&iter, &user_id, &user_node)) {
            g_hash_table_insert(priv->_user_levels, g_strdup(user_id), GINT_TO_POINTER(json_node_get_int(user_node)));
        }
    }

    MATRIX_EVENT_BASE_CLASS(matrix_event_room_power_levels_parent_class)->from_json(matrix_event_base, json_data, error);
}

static void
matrix_event_room_power_levels_real_to_json(MatrixEventBase *matrix_event_base, JsonNode *json_data, GError **error)
{
    MatrixEventRoomPowerLevelsPrivate *priv;
    JsonObject *root;
    JsonObject *content_root;
    JsonObject *users_root;
    JsonObject *events_root;
    JsonNode *content_node;
    JsonNode *users_node;
    JsonNode *events_node;
    GHashTableIter iter;
    const gchar *state_key;
    gpointer key;
    gpointer value;

    g_return_if_fail (json_data != NULL);

    priv = matrix_event_room_power_levels_get_instance_private(MATRIX_EVENT_ROOM_POWER_LEVELS(matrix_event_base));
    root = json_node_get_object(json_data);
    content_node = json_object_get_member(root, "content");
    content_root = json_node_get_object(content_node);

    state_key = matrix_event_state_get_state_key(MATRIX_EVENT_STATE(matrix_event_base));

    if ((state_key == NULL) || (*state_key == 0)) {
        g_set_error(error, MATRIX_ERROR, MATRIX_ERROR_INCOMPLETE,
                    "Won't generate a m.room.power_levels event with a non-empty state_key");

        return;
    }

    if (priv->_user_levels == NULL) {
        g_set_error(error, MATRIX_ERROR, MATRIX_ERROR_INCOMPLETE,
                    "Won't create an m.room.power_levels event without a content.users key");

        return;
    }

    if (priv->_event_levels == NULL) {
        g_set_error(error, MATRIX_ERROR, MATRIX_ERROR_INCOMPLETE,
                    "Won't create an m.room.power_levels event without a content.events key");

        return;
    }

    json_object_set_int_member(content_root, "ban", priv->_ban);
    json_object_set_int_member(content_root, "kick", priv->_kick);
    json_object_set_int_member(content_root, "redact", priv->_redact);
    json_object_set_int_member(content_root, "state_default", priv->_state_default);
    json_object_set_int_member(content_root, "events_default", priv->_events_default);

    users_root = json_object_new();
    users_node = json_node_new(JSON_NODE_OBJECT);
    json_node_set_object(users_node, users_root);

    g_hash_table_iter_init(&iter, priv->_user_levels);

    while (g_hash_table_iter_next(&iter, &key, &value)) {
        json_object_set_int_member(users_root, g_strdup(key), GPOINTER_TO_INT(value));
    }

    json_object_set_member(content_root, "users", users_node);

    events_root = json_object_new();
    events_node = json_node_new(JSON_NODE_OBJECT);
    json_node_set_object(events_node, events_root);

    g_hash_table_iter_init(&iter, priv->_event_levels);

    while (g_hash_table_iter_next(&iter, &key, &value)) {
        json_object_set_int_member(events_root, g_strdup(key), GPOINTER_TO_INT(value));
    }

    json_object_set_member(content_root, "events", events_node);

    MATRIX_EVENT_BASE_CLASS(matrix_event_room_power_levels_parent_class)->to_json(matrix_event_base, json_data, error);
}

/**
 * matrix_event_room_power_levels_set_user_level:
 * @event: a #MatrixEventRoomPowerLevels
 * @user_id: (transfer none) (not nullable): a Matrix ID
 * @level: the level to set for @user_id
 *
 * Set the level of @user_id to @level in @event.
 */
void
matrix_event_room_power_levels_set_user_level(MatrixEventRoomPowerLevels *matrix_event_room_power_levels, const gchar *user_id, gint level)
{
    MatrixEventRoomPowerLevelsPrivate *priv;

    g_return_if_fail(matrix_event_room_power_levels != NULL);
    g_return_if_fail(user_id != NULL);

    priv = matrix_event_room_power_levels_get_instance_private(matrix_event_room_power_levels);

    g_hash_table_insert(priv->_user_levels, g_strdup(user_id), GINT_TO_POINTER(level));
}

/**
 * matrix_event_room_power_levels_set_event_level:
 * @event: a #MatrixEventRoomPowerLevels
 * @event_type: an event type name
 * @level: the required level for sending @event_type messages
 *
 * Set the level required to send messages with type @event_type to @level.
 */
void
matrix_event_room_power_levels_set_event_level(MatrixEventRoomPowerLevels *matrix_event_room_power_levels, const gchar *event_type, gint level)
{
    MatrixEventRoomPowerLevelsPrivate *priv;

    g_return_if_fail(matrix_event_room_power_levels != NULL);
    g_return_if_fail(event_type != NULL);

    priv = matrix_event_room_power_levels_get_instance_private(matrix_event_room_power_levels);

    g_hash_table_insert(priv->_event_levels, g_strdup(event_type), GINT_TO_POINTER(level));
}

MatrixEventRoomPowerLevels *
matrix_event_room_power_levels_construct(GType object_type)
{
    return (MatrixEventRoomPowerLevels *)matrix_event_state_construct(object_type);
}

/**
 * matrix_event_room_power_levels_new:
 *
 * Create a new #MatrixEventRoomPowerLevels object
 *
 * Returns: (transfer full): a new #MatrixEventRoomPowerLevels object
 */
MatrixEventRoomPowerLevels *
matrix_event_room_power_levels_new(void)
{
    return matrix_event_room_power_levels_construct(MATRIX_EVENT_TYPE_ROOM_POWER_LEVELS);
}

/**
 * matrix_event_room_power_levels_get_users_default:
 * @event: a #MatrixEventRoomPowerLevels
 *
 * Get the default user level from @event.
 *
 * Returns: a power level
 */
gint
matrix_event_room_power_levels_get_users_default(MatrixEventRoomPowerLevels *matrix_event_room_power_levels)
{
    MatrixEventRoomPowerLevelsPrivate *priv;

    g_return_val_if_fail(matrix_event_room_power_levels != NULL, 0);

    priv = matrix_event_room_power_levels_get_instance_private(matrix_event_room_power_levels);

    return priv->_users_default;
}

/**
 * matrix_event_room_power_levels_set_users_default:
 * @event: a #MatrixEventRoomPowerLevels
 * @users_default: a power level
 *
 * Set the default user level in @event.
 */
void
matrix_event_room_power_levels_set_users_default(MatrixEventRoomPowerLevels *matrix_event_room_power_levels, gint users_default)
{
    MatrixEventRoomPowerLevelsPrivate *priv;

    g_return_if_fail(matrix_event_room_power_levels != NULL);

    priv = matrix_event_room_power_levels_get_instance_private(matrix_event_room_power_levels);

    if (priv->_users_default != users_default) {
        priv->_users_default = users_default;

        g_object_notify_by_pspec((GObject *)matrix_event_room_power_levels, matrix_event_room_power_levels_properties[PROP_USERS_DEFAULT]);
    }
}

/**
 * matrix_event_room_power_levels_get_events_default:
 * @event: a #MatrixEventRoomPowerLevels
 *
 * Get the level required to send messages in @event, if not otherwise set with
 * matrix_event_room_power_levels_set_event_level()
 */
gint
matrix_event_room_power_levels_get_events_default(MatrixEventRoomPowerLevels *matrix_event_room_power_levels)
{
    MatrixEventRoomPowerLevelsPrivate *priv;

    g_return_val_if_fail(matrix_event_room_power_levels != NULL, 0);

    priv = matrix_event_room_power_levels_get_instance_private(matrix_event_room_power_levels);

    return priv->_events_default;
}

/**
 * matrix_event_room_power_levels_set_events_default:
 * @event: a #MatrixEventRoomPowerLevels
 * @events_default: a power level
 *
 * Set the event level required for sending message events.  This is the default value to be
 * used for events that are not set with matrix_event_room_power_levels_set_event_level().
 */
void
matrix_event_room_power_levels_set_events_default(MatrixEventRoomPowerLevels *matrix_event_room_power_levels, gint events_default)
{
    MatrixEventRoomPowerLevelsPrivate *priv;

    g_return_if_fail(matrix_event_room_power_levels != NULL);

    priv = matrix_event_room_power_levels_get_instance_private(matrix_event_room_power_levels);

    if (priv->_events_default != events_default) {
        priv->_events_default = events_default;

        g_object_notify_by_pspec((GObject *)matrix_event_room_power_levels, matrix_event_room_power_levels_properties[PROP_EVENTS_DEFAULT]);
    }
}

gint
matrix_event_room_power_levels_get_state_default(MatrixEventRoomPowerLevels *matrix_event_room_power_levels)
{
    MatrixEventRoomPowerLevelsPrivate *priv;

    g_return_val_if_fail(matrix_event_room_power_levels != NULL, 0);

    priv = matrix_event_room_power_levels_get_instance_private(matrix_event_room_power_levels);
    return priv->_state_default;
}

void
matrix_event_room_power_levels_set_state_default(MatrixEventRoomPowerLevels *matrix_event_room_power_levels, gint state_default)
{
    MatrixEventRoomPowerLevelsPrivate *priv;

    g_return_if_fail(matrix_event_room_power_levels != NULL);

    priv = matrix_event_room_power_levels_get_instance_private(matrix_event_room_power_levels);

    if (priv->_state_default != state_default) {
        priv->_state_default = state_default;

        g_object_notify_by_pspec((GObject *)matrix_event_room_power_levels, matrix_event_room_power_levels_properties[PROP_STATE_DEFAULT]);
    }
}

gint
matrix_event_room_power_levels_get_ban(MatrixEventRoomPowerLevels *matrix_event_room_power_levels)
{
    MatrixEventRoomPowerLevelsPrivate *priv;

    g_return_val_if_fail(matrix_event_room_power_levels != NULL, 0);

    priv = matrix_event_room_power_levels_get_instance_private(matrix_event_room_power_levels);

    return priv->_ban;
}

void
matrix_event_room_power_levels_set_ban(MatrixEventRoomPowerLevels *matrix_event_room_power_levels, gint ban)
{
    MatrixEventRoomPowerLevelsPrivate *priv;

    g_return_if_fail(matrix_event_room_power_levels != NULL);

    priv = matrix_event_room_power_levels_get_instance_private(matrix_event_room_power_levels);

    if (priv->_ban != ban) {
        priv->_ban = ban;

        g_object_notify_by_pspec((GObject *) matrix_event_room_power_levels, matrix_event_room_power_levels_properties[PROP_BAN]);
    }
}

gint
matrix_event_room_power_levels_get_kick(MatrixEventRoomPowerLevels *matrix_event_room_power_levels)
{
    MatrixEventRoomPowerLevelsPrivate *priv;

    g_return_val_if_fail(matrix_event_room_power_levels != NULL, 0);

    priv = matrix_event_room_power_levels_get_instance_private(matrix_event_room_power_levels);

    return priv->_kick;
}

void
matrix_event_room_power_levels_set_kick(MatrixEventRoomPowerLevels *matrix_event_room_power_levels, gint kick)
{
    MatrixEventRoomPowerLevelsPrivate *priv;

    g_return_if_fail(matrix_event_room_power_levels != NULL);

    priv = matrix_event_room_power_levels_get_instance_private(matrix_event_room_power_levels);

    if (priv->_kick != kick) {
        priv->_kick = kick;

        g_object_notify_by_pspec((GObject *)matrix_event_room_power_levels, matrix_event_room_power_levels_properties[PROP_KICK]);
    }
}

gint
matrix_event_room_power_levels_get_redact(MatrixEventRoomPowerLevels *matrix_event_room_power_levels)
{
    MatrixEventRoomPowerLevelsPrivate *priv;

    g_return_val_if_fail(matrix_event_room_power_levels != NULL, 0);

    priv = matrix_event_room_power_levels_get_instance_private(matrix_event_room_power_levels);

    return priv->_redact;
}

void
matrix_event_room_power_levels_set_redact(MatrixEventRoomPowerLevels *matrix_event_room_power_levels, gint redact)
{
    MatrixEventRoomPowerLevelsPrivate *priv;

    g_return_if_fail(matrix_event_room_power_levels != NULL);

    priv = matrix_event_room_power_levels_get_instance_private(matrix_event_room_power_levels);

    if (priv->_redact != redact) {
        priv->_redact = redact;

        g_object_notify_by_pspec((GObject *)matrix_event_room_power_levels, matrix_event_room_power_levels_properties[PROP_REDACT]);
    }
}

gint
matrix_event_room_power_levels_get_invite(MatrixEventRoomPowerLevels *matrix_event_room_power_levels)
{
    MatrixEventRoomPowerLevelsPrivate *priv;

    g_return_val_if_fail(matrix_event_room_power_levels != NULL, 0);

    priv = matrix_event_room_power_levels_get_instance_private(matrix_event_room_power_levels);

    return priv->_invite;
}

void
matrix_event_room_power_levels_set_invite(MatrixEventRoomPowerLevels *matrix_event_room_power_levels, gint invite)
{
    MatrixEventRoomPowerLevelsPrivate *priv;

    g_return_if_fail(matrix_event_room_power_levels != NULL);

    priv = matrix_event_room_power_levels_get_instance_private(matrix_event_room_power_levels);

    if (priv->_invite != invite) {
        priv->_invite = invite;

        g_object_notify_by_pspec((GObject *)matrix_event_room_power_levels, matrix_event_room_power_levels_properties[PROP_INVITE]);
    }
}

GHashTable *
matrix_event_room_power_levels_get_event_levels(MatrixEventRoomPowerLevels *matrix_event_room_power_levels)
{
    MatrixEventRoomPowerLevelsPrivate *priv;

    g_return_val_if_fail(matrix_event_room_power_levels != NULL, NULL);

    priv = matrix_event_room_power_levels_get_instance_private(matrix_event_room_power_levels);

    return priv->_event_levels;
}

/**
 * matrix_event_room_power_levels_get_user_levels:
 * @event: a #MatrixEventRoomPowerLevels
 *
 * Get the table of individual user levels from @event.
 *
 * The returned value is owned by @event and should not be freed.
 *
 * The returned #GHashTable has user IDs as keys.  The value are integer power levels
 * converted to pointers; use GPOINTER_TO_INT() to convert them to actual power levels.
 *
 * Returns: (transfer none) (nullable): a table of user levels
 */
GHashTable *
matrix_event_room_power_levels_get_user_levels(MatrixEventRoomPowerLevels *matrix_event_room_power_levels)
{
    MatrixEventRoomPowerLevelsPrivate *priv;

    g_return_val_if_fail(matrix_event_room_power_levels != NULL, NULL);

    priv = matrix_event_room_power_levels_get_instance_private(matrix_event_room_power_levels);

    return priv->_user_levels;
}

static void
matrix_event_room_power_levels_finalize(GObject *gobject)
{
    MatrixEventRoomPowerLevelsPrivate *priv = matrix_event_room_power_levels_get_instance_private(MATRIX_EVENT_ROOM_POWER_LEVELS(gobject));

    g_hash_table_unref(priv->_event_levels);
    g_hash_table_unref(priv->_user_levels);

    G_OBJECT_CLASS(matrix_event_room_power_levels_parent_class)->finalize(gobject);
}

static void
matrix_event_room_power_levels_get_property(GObject *gobject, guint property_id, GValue *value, GParamSpec *pspec)
{
    MatrixEventRoomPowerLevels *matrix_event_room_power_levels = MATRIX_EVENT_ROOM_POWER_LEVELS(gobject);

    switch (property_id) {
        case PROP_USERS_DEFAULT:
            g_value_set_int(value, matrix_event_room_power_levels_get_users_default(matrix_event_room_power_levels));

            break;
        case PROP_EVENTS_DEFAULT:
            g_value_set_int(value, matrix_event_room_power_levels_get_events_default(matrix_event_room_power_levels));

            break;
        case PROP_STATE_DEFAULT:
            g_value_set_int(value, matrix_event_room_power_levels_get_state_default(matrix_event_room_power_levels));

            break;
        case PROP_BAN:
            g_value_set_int(value, matrix_event_room_power_levels_get_ban(matrix_event_room_power_levels));

            break;
        case PROP_KICK:
            g_value_set_int(value, matrix_event_room_power_levels_get_kick(matrix_event_room_power_levels));

            break;
        case PROP_REDACT:
            g_value_set_int(value, matrix_event_room_power_levels_get_redact(matrix_event_room_power_levels));

            break;
        case PROP_INVITE:
            g_value_set_int(value, matrix_event_room_power_levels_get_invite(matrix_event_room_power_levels));

            break;
        case PROP_EVENT_LEVELS:
            g_value_set_boxed(value, matrix_event_room_power_levels_get_event_levels(matrix_event_room_power_levels));

            break;
        case PROP_USER_LEVELS:
            g_value_set_boxed(value, matrix_event_room_power_levels_get_user_levels(matrix_event_room_power_levels));

            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, property_id, pspec);

            break;
    }
}

static void
matrix_event_room_power_levels_set_property(GObject *gobject, guint property_id, const GValue *value, GParamSpec *pspec)
{
    MatrixEventRoomPowerLevels *matrix_event_room_power_levels = MATRIX_EVENT_ROOM_POWER_LEVELS(gobject);

    switch (property_id) {
        case PROP_USERS_DEFAULT:
            matrix_event_room_power_levels_set_users_default(matrix_event_room_power_levels, g_value_get_int(value));

            break;
        case PROP_EVENTS_DEFAULT:
            matrix_event_room_power_levels_set_events_default(matrix_event_room_power_levels, g_value_get_int(value));

            break;
        case PROP_STATE_DEFAULT:
            matrix_event_room_power_levels_set_state_default(matrix_event_room_power_levels, g_value_get_int(value));

            break;
        case PROP_BAN:
            matrix_event_room_power_levels_set_ban(matrix_event_room_power_levels, g_value_get_int(value));

            break;
        case PROP_KICK:
            matrix_event_room_power_levels_set_kick(matrix_event_room_power_levels, g_value_get_int(value));

            break;
        case PROP_REDACT:
            matrix_event_room_power_levels_set_redact(matrix_event_room_power_levels, g_value_get_int(value));

            break;
        case PROP_INVITE:
            matrix_event_room_power_levels_set_invite(matrix_event_room_power_levels, g_value_get_int(value));

            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, property_id, pspec);
        break;
    }
}

static void
matrix_event_room_power_levels_class_init(MatrixEventRoomPowerLevelsClass *klass)
{
    ((MatrixEventBaseClass *)klass)->from_json = matrix_event_room_power_levels_real_from_json;
    ((MatrixEventBaseClass *)klass)->to_json = matrix_event_room_power_levels_real_to_json;
    G_OBJECT_CLASS(klass)->get_property = matrix_event_room_power_levels_get_property;
    G_OBJECT_CLASS(klass)->set_property = matrix_event_room_power_levels_set_property;
    G_OBJECT_CLASS(klass)->finalize = matrix_event_room_power_levels_finalize;

    /**
     * MatrixEventRoomPowerLevels:users-default:
     *
     * The default power level for every user in the room, unless their user ID is mentioned
     * in the #MatrixEventRoomPowerLevels:users property.
     */
    matrix_event_room_power_levels_properties[PROP_USERS_DEFAULT] = g_param_spec_int(
            "users-default", "users-default", "users-default",
            G_MININT, G_MAXINT, 0,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_USERS_DEFAULT, matrix_event_room_power_levels_properties[PROP_USERS_DEFAULT]);

    /**
     * MatrixEventRoomPowerLevels:events-default:
     *
     * The default level required to send message events.  Can be overridden with values in
     * the #MatrixEventRoomPowerLevels:events property.
     */
    matrix_event_room_power_levels_properties[PROP_EVENTS_DEFAULT] = g_param_spec_int(
            "events-default", "events-default", "events-default",
            G_MININT, G_MAXINT, 0,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_EVENTS_DEFAULT, matrix_event_room_power_levels_properties[PROP_EVENTS_DEFAULT]);

    /**
     * MatrixEventRoomPowerLevels:state-default:
     *
     * The default level required to send state events.  Can be overridden with values in the
     * #MatrixEventRoomPowerLevels:events property.
     */
    matrix_event_room_power_levels_properties[PROP_STATE_DEFAULT] = g_param_spec_int(
            "state-default", "state-default", "state-default",
            G_MININT, G_MAXINT, 10,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_STATE_DEFAULT, matrix_event_room_power_levels_properties[PROP_STATE_DEFAULT]);

    /**
     * MatrixEventRoomPowerLevels:ban:
     *
     * The level required to ban a user.
     */
    matrix_event_room_power_levels_properties[PROP_BAN] = g_param_spec_int(
            "ban", "ban", "ban",
            G_MININT, G_MAXINT, 5,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_BAN, matrix_event_room_power_levels_properties[PROP_BAN]);

    /**
     * MatrixEventRoomPowerLevels:kick:
     *
     * The level required to kick a user.
     */
    matrix_event_room_power_levels_properties[PROP_KICK] = g_param_spec_int(
            "kick", "kick", "kick",
            G_MININT, G_MAXINT, 5,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_KICK, matrix_event_room_power_levels_properties[PROP_KICK]);

    /**
     * MatrixEventRoomPowerLevels:redact:
     *
     * The level required to redact an event.
     */
    matrix_event_room_power_levels_properties[PROP_REDACT] = g_param_spec_int(
            "redact", "redact", "redact",
            G_MININT, G_MAXINT, 20,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_REDACT, matrix_event_room_power_levels_properties[PROP_REDACT]);

    /**
     * MatrixEventRoomPowerLevels:invite:
     *
     * The level required to invite someone.
     */
    matrix_event_room_power_levels_properties[PROP_INVITE] = g_param_spec_int(
            "invite", "invite", "invite",
            G_MININT, G_MAXINT, 0,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_INVITE, matrix_event_room_power_levels_properties[PROP_INVITE]);

    /**
     * MatrixEventRoomPowerLevels:event-levels:
     *
     * A hash map to store the required level to send specific events.
     */
    matrix_event_room_power_levels_properties[PROP_EVENT_LEVELS] = g_param_spec_boxed(
            "event-levels", "event-levels", "event-levels",
            G_TYPE_HASH_TABLE,
            G_PARAM_STATIC_STRINGS | G_PARAM_READABLE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_EVENT_LEVELS, matrix_event_room_power_levels_properties[PROP_EVENT_LEVELS]);

    /**
     * MatrixEventRoomPowerLevels:user-levels:
     *
     * A hash map to store current level for individual users.
     */
    matrix_event_room_power_levels_properties[PROP_USER_LEVELS] = g_param_spec_boxed(
            "user-levels", "user-levels", "user-levels",
            G_TYPE_HASH_TABLE,
            G_PARAM_STATIC_STRINGS | G_PARAM_READABLE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_USER_LEVELS, matrix_event_room_power_levels_properties[PROP_USER_LEVELS]);
}

static void
matrix_event_room_power_levels_init(MatrixEventRoomPowerLevels *matrix_event_room_power_levels)
{
    MatrixEventRoomPowerLevelsPrivate *priv = matrix_event_room_power_levels_get_instance_private(matrix_event_room_power_levels);

    priv->_users_default = 0;
    priv->_events_default = 0;
    priv->_state_default = 10;
    priv->_ban = 5;
    priv->_kick = 5;
    priv->_redact = 20;
    priv->_invite = 0;
    priv->_event_levels = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, NULL);
    priv->_user_levels = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, NULL);
}
