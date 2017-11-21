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

#include "matrix-event-room-join-rules.h"
#include "matrix-enumtypes.h"
#include "utils.h"

/**
 * SECTION:matrix-event-room-join-rules
 * @short_description: event to hold the join rules for a room
 *
 * This is the default event handler for `m.room.join_rules` events.
 *
 * A room may be `public` meaning anyone can join the room without any prior action.
 * Alternatively, it can be `invite` meaning that a user who wishes to join the room must
 * first receive an invite to the room from someone already inside of the room.  Currently,
 * `knock` and `private` are reserved keywords which are not implemented.
 */
enum {
    PROP_0,
    PROP_JOIN_RULES,
    NUM_PROPERTIES
};

static GParamSpec *matrix_event_room_join_rules_properties[NUM_PROPERTIES];

typedef struct {
    MatrixJoinRules _join_rules;
} MatrixEventRoomJoinRulesPrivate;

/**
 * MatrixEventRoomJoinRules:
 */
G_DEFINE_TYPE_WITH_PRIVATE(MatrixEventRoomJoinRules, matrix_event_room_join_rules, MATRIX_EVENT_TYPE_STATE);

static void
matrix_event_room_join_rules_real_from_json(MatrixEventBase *matrix_event_base, JsonNode *json_data, GError **error)
{
    MatrixEventRoomJoinRulesPrivate *priv;
    JsonObject *root;
    JsonObject *content_root;
    JsonNode *content_node;
    JsonNode *node;

    g_return_if_fail(json_data != NULL);

    priv = matrix_event_room_join_rules_get_instance_private(MATRIX_EVENT_ROOM_JOIN_RULES(matrix_event_base));
    root = json_node_get_object(json_data);
    content_node = json_object_get_member(root, "content");
    content_root = json_node_get_object(content_node);

#ifdef DEBUG
    if ((node = json_object_get_member(root, "state_key")) != NULL) {
        const gchar *state_key = json_node_get_string(node);

        if ((state_key == NULL) || (*state_key != 0)) {
            g_warning("state_key of a m.room.join_rules is non-empty");
        }
    }
#endif

    if ((node = json_object_get_member(content_root, "join_rule")) != NULL) {
        GError *inner_error = NULL;
        MatrixJoinRules join_rules = _matrix_g_enum_nick_to_value(MATRIX_TYPE_JOIN_RULES, json_node_get_string(node), &inner_error);

        if (inner_error != NULL) {
            priv->_join_rules = MATRIX_JOIN_RULES_UNKNOWN;

#ifdef DEBUG
            g_warning("Unknown value %s in a m.room.join_rules event", json_node_get_string(node));
#endif
        } else {
            priv->_join_rules = join_rules;
        }
    } else {
        priv->_join_rules = MATRIX_JOIN_RULES_UNKNOWN;

        g_warning("content.join_rules is missing from a m.room.join_rules event.");
    }

    MATRIX_EVENT_BASE_CLASS(matrix_event_room_join_rules_parent_class)->from_json(matrix_event_base, json_data, error);
}

static void
matrix_event_room_join_rules_real_to_json(MatrixEventBase *matrix_event_base, JsonNode *json_data, GError **error)
{
    MatrixEventRoomJoinRulesPrivate *priv;
    JsonObject *root;
    JsonObject *content_root;
    JsonNode *content_node;
    const gchar *state_key;
    gchar *join_rules;

    g_return_if_fail(json_data != NULL);

    priv = matrix_event_room_join_rules_get_instance_private(MATRIX_EVENT_ROOM_JOIN_RULES(matrix_event_base));

    root = json_node_get_object(json_data);
    content_node = json_object_get_member(root, "content");
    content_root = json_node_get_object(content_node);

    state_key = matrix_event_state_get_state_key(MATRIX_EVENT_STATE(matrix_event_base));

    if ((state_key == NULL) || (*state_key == 0)) {
        g_set_error(error, MATRIX_ERROR, MATRIX_ERROR_INCOMPLETE,
                    "Won't generate a m.room.join_rules event with a non-empty state_key");

        return;
    }

    if (priv->_join_rules == MATRIX_JOIN_RULES_UNKNOWN) {
        g_set_error(error, MATRIX_ERROR, MATRIX_ERROR_UNKNOWN_VALUE,
                    "Won't send a m.room.join_rules event with an unknown rule");

        return;
    }

    join_rules = _matrix_g_enum_to_string(MATRIX_TYPE_JOIN_RULES, priv->_join_rules, '_');
    json_object_set_string_member(content_root, "join_rule", join_rules);
    g_free(join_rules);

    MATRIX_EVENT_BASE_CLASS(matrix_event_room_join_rules_parent_class)->to_json(matrix_event_base, json_data, error);
}

/**
 * matrix_event_room_join_rules_new:
 *
 * Create a new #MatrixEventRoomJoinRules object.
 *
 * Returns: (transfer full): a new #MatrixEventRoomJoinRules object
 */
MatrixEventRoomJoinRules *
matrix_event_room_join_rules_new(void) {
    return (MatrixEventRoomJoinRules *)matrix_event_state_construct(MATRIX_EVENT_TYPE_ROOM_JOIN_RULES);
}

/**
 * matrix_event_room_join_rules_get_join_rules:
 * @event: a #MatrixEventRoomJoinRules
 *
 * Get the join rules for the room in @event.
 *
 * Returns: a #MatrixJoinRules value
 */
MatrixJoinRules
matrix_event_room_join_rules_get_join_rules(MatrixEventRoomJoinRules *matrix_event_room_join_rules)
{
    MatrixEventRoomJoinRulesPrivate *priv;

    g_return_val_if_fail(matrix_event_room_join_rules != NULL, 0);

    priv = matrix_event_room_join_rules_get_instance_private(matrix_event_room_join_rules);

    return priv->_join_rules;
}

/**
 * matrix_event_room_join_rules_set_join_rules:
 * @event: a #MatrixEventRoomJoinRules
 * @join_rules: a #MatrixJoinRules value
 *
 * Set the join rules for the room in @event.
 */
void
matrix_event_room_join_rules_set_join_rules(MatrixEventRoomJoinRules *matrix_event_room_join_rules, MatrixJoinRules join_rules)
{
    MatrixEventRoomJoinRulesPrivate *priv;

    g_return_if_fail(matrix_event_room_join_rules != NULL);

    priv = matrix_event_room_join_rules_get_instance_private(matrix_event_room_join_rules);

    if (priv->_join_rules != join_rules) {
        priv->_join_rules = join_rules;

        g_object_notify_by_pspec((GObject *)matrix_event_room_join_rules, matrix_event_room_join_rules_properties[PROP_JOIN_RULES]);
    }
}

static void
matrix_event_room_join_rules_finalize(GObject *gobject)
{
    G_OBJECT_CLASS(matrix_event_room_join_rules_parent_class)->finalize(gobject);
}

static void
matrix_event_room_join_rules_get_property(GObject *gobject, guint property_id, GValue *value, GParamSpec *pspec)
{
    MatrixEventRoomJoinRules *matrix_event_room_join_rules = MATRIX_EVENT_ROOM_JOIN_RULES(gobject);

    switch (property_id) {
        case PROP_JOIN_RULES:
            g_value_set_enum(value, matrix_event_room_join_rules_get_join_rules(matrix_event_room_join_rules));

            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, property_id, pspec);

            break;
    }
}

static void
matrix_event_room_join_rules_set_property(GObject *gobject, guint property_id, const GValue *value, GParamSpec *pspec)
{
    MatrixEventRoomJoinRules *matrix_event_room_join_rules = MATRIX_EVENT_ROOM_JOIN_RULES(gobject);

    switch (property_id) {
        case PROP_JOIN_RULES:
            matrix_event_room_join_rules_set_join_rules(matrix_event_room_join_rules, g_value_get_enum(value));

            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, property_id, pspec);

            break;
    }
}

static void
matrix_event_room_join_rules_class_init(MatrixEventRoomJoinRulesClass *klass)
{
    ((MatrixEventBaseClass *)klass)->from_json = matrix_event_room_join_rules_real_from_json;
    ((MatrixEventBaseClass *)klass)->to_json = matrix_event_room_join_rules_real_to_json;
    G_OBJECT_CLASS(klass)->get_property = matrix_event_room_join_rules_get_property;
    G_OBJECT_CLASS(klass)->set_property = matrix_event_room_join_rules_set_property;
    G_OBJECT_CLASS(klass)->finalize = matrix_event_room_join_rules_finalize;

    /**
     * MatrixEventRoomJoinRules:join-rules:
     *
     * The join rules.
     */
    matrix_event_room_join_rules_properties[PROP_JOIN_RULES] = g_param_spec_enum(
            "join-rules", "join-rules", "join-rules",
            MATRIX_TYPE_JOIN_RULES, MATRIX_JOIN_RULES_UNKNOWN,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_JOIN_RULES, matrix_event_room_join_rules_properties[PROP_JOIN_RULES]);
}

static void
matrix_event_room_join_rules_init(MatrixEventRoomJoinRules *matrix_event_room_join_rules)
{
    MatrixEventRoomJoinRulesPrivate *priv = matrix_event_room_join_rules_get_instance_private(matrix_event_room_join_rules);

    priv->_join_rules = MATRIX_JOIN_RULES_UNKNOWN;
}
