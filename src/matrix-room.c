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

#include "matrix-room.h"
#include "matrix-enumtypes.h"

/**
 * SECTION:matrix-room
 * @short_description: class to hold room data
 *
 */

#define MATRIX_ROOM_TYPE_MEMBER_DATA (matrix_room_member_data_get_type ())

enum  {
    PROP_0,
    PROP_ROOM_ID,
    PROP_ALIASES,
    PROP_AVATAR_URL,
    PROP_AVATAR_THUMBNAIL_URL,
    PROP_CANONICAL_ALIAS,
    PROP_CREATOR,
    PROP_FEDERATE,
    PROP_GUEST_ACCESS,
    PROP_HISTORY_VISIBILITY,
    PROP_JOIN_RULES,
    PROP_NAME,
    PROP_DEFAULT_POWER_LEVEL,
    PROP_DEFAULT_EVENT_LEVEL,
    PROP_DEFAULT_STATE_LEVEL,
    PROP_BAN_LEVEL,
    PROP_KICK_LEVEL,
    PROP_REDACT_LEVEL,
    PROP_INVITE_LEVEL,
    PROP_TOPIC,
    PROP_TYPING_USERS,
    PROP_AVATAR_INFO,
    PROP_AVATAR_THUMBNAIL_INFO,
    NUM_PROPERTIES
};

static GParamSpec *matrix_room_properties[NUM_PROPERTIES];

typedef struct {
    MatrixProfile* profile;
    gboolean thirdparty;
} MatrixRoomMemberData;

MatrixRoomMemberData *
matrix_room_member_data_new(void)
{
    return g_new0(MatrixRoomMemberData, 1);
}

void
matrix_room_member_data_free(MatrixRoomMemberData *member_data)
{
    g_return_if_fail(member_data != NULL);

    g_object_unref(member_data->profile);
    g_free(member_data);
}

typedef struct {
    gchar *room_id;
    gchar **aliases;
    gint aliases_len;
    gchar *avatar_url;
    MatrixImageInfo *avatar_info;
    gchar *avatar_thumbnail_url;
    MatrixImageInfo *avatar_thumbnail_info;
    gchar *canonical_alias;
    gchar *creator;
    gboolean federate;
    MatrixGuestAccess guest_access;
    MatrixHistoryVisibility history_visibility;
    MatrixJoinRules join_rules;
    gchar *name;
    gint default_power_level;
    gint default_event_level;
    gint default_state_level;
    gint ban_level;
    gint kick_level;
    gint redact_level;
    gint invite_level;
    gchar *topic;
    gchar **typing_users;
    gint typing_users_len;
    GHashTable* event_levels;
    GHashTable* user_levels;
    GHashTable* members;
} MatrixRoomPrivate;

/**
 * MatrixRoom:
 *
 * The #MatrixRoom class is there to hold different parameters of a room like its known aliases,
 * its members, etc.
 */
G_DEFINE_TYPE_WITH_PRIVATE(MatrixRoom, matrix_room, G_TYPE_OBJECT);

/**
 * matrix_room_new:
 * @room_id: a room ID
 *
 * Create a new #MatrixRoom object.
 *
 * Returns: (transfer full): a new #MatrixRoom object
 */
MatrixRoom *
matrix_room_new(const gchar *room_id)
{
    g_return_val_if_fail(room_id != NULL, NULL);

    return (MatrixRoom *)g_object_new(MATRIX_TYPE_ROOM,
                                      "room-id", room_id,
                                      NULL);
}

/**
 * matrix_room_add_member:
 * @room: a #MatrixRoom
 * @user_id: the Matrix ID of the user to add
 * @profile: a #MatrixProfile
 * @third_party: if true, the member is marked as a pending 3rd party invitation
 * @error: a #GError, or %NULL to ignore errors
 *
 * Add a member to the room member list.  If a member with the same @user_id exists, @error is
 * set to #MATRIX_ERROR_ALREADY_EXISTS.
 */
void
matrix_room_add_member(MatrixRoom *matrix_room, const gchar *user_id, MatrixProfile *profile, gboolean third_party, GError **error)
{
    MatrixRoomPrivate *priv;
    MatrixRoomMemberData *data = NULL;

    g_return_if_fail(matrix_room != NULL);
    g_return_if_fail(user_id != NULL);

    priv = matrix_room_get_instance_private(matrix_room);

    if ((data = g_hash_table_lookup(priv->members, user_id)) != NULL) {
        g_set_error(error, MATRIX_ERROR, MATRIX_ERROR_ALREADY_EXISTS,
                    "User already exists in that room");

        return;
    }

    data = matrix_room_member_data_new();

    if (profile == NULL) {
        data->profile = matrix_profile_new();
    } else {
        data->profile = profile;
    }

    data->thirdparty = third_party;

    g_hash_table_insert(priv->members, g_strdup(user_id), data);
}

/**
 * matrix_room_get_or_add_member:
 * @room: a #MatrixRoom
 * @user_id: the Matrix ID of the user to add
 * @third_party: if %TRUE, the member is marked as a pending 3rd party invitation
 * @error: a #GError, or %NULL to ignore errors
 *
 * Gets the profile of the room member specified in @user_id.  If that user is not added to the
 * room yet, it gets added with an empty profile and that profile is returned.
 *
 * Returns: (transfer none): the #MatrixProfile of the user
 */
MatrixProfile *
matrix_room_get_or_add_member(MatrixRoom *matrix_room, const gchar *user_id, gboolean third_party, GError **error)
{
    MatrixProfile *profile;
    GError *inner_error = NULL;

    g_return_val_if_fail(matrix_room != NULL, NULL);
    g_return_val_if_fail(user_id != NULL, NULL);

    profile = matrix_room_get_member(matrix_room, user_id, NULL, &inner_error);

    if (inner_error == NULL) {
        return profile;
    }

    if ((inner_error->domain != MATRIX_ERROR) || (inner_error->code == MATRIX_ERROR_NOT_FOUND)) {
        g_propagate_error(error, inner_error);

        return NULL;
    }

    g_clear_error(&inner_error);

    matrix_room_add_member(matrix_room, user_id, NULL, third_party, &inner_error);

    if (inner_error != NULL) {
        g_propagate_error(error, inner_error);

        return NULL;
    }

    profile = matrix_room_get_member(matrix_room, user_id, &third_party, &inner_error);

    if (inner_error != NULL) {
        g_propagate_error(error, inner_error);

        return NULL;
    }

    return profile;
}

/**
 * matrix_room_get_member:
 * @room: a #MatrixRoom
 * @user_id: the Matrix ID of the user to find
 * @third_party: gets a true value if the member is actually a pending 3rd party invitation
 * @error: (nullable): a #GError, or %NULL to ignore errors
 *
 * Gets the profile of the room member specified in @user_id.  If that user is not added to
 * the room yet, @error is set to #MATRIX_ERROR_NOT_FOUND.
 *
 * Returns: (transfer none): the profile of the user
 */
MatrixProfile *
matrix_room_get_member(MatrixRoom *matrix_room, const gchar *user_id, gboolean *third_party, GError **error)
{
    MatrixRoomMemberData *data;
    MatrixRoomPrivate *priv;

    g_return_val_if_fail(matrix_room != NULL, NULL);
    g_return_val_if_fail(user_id != NULL, NULL);

    priv = matrix_room_get_instance_private(matrix_room);

    if ((data = g_hash_table_lookup(priv->members, user_id)) == NULL) {
        g_set_error(error, MATRIX_ERROR, MATRIX_ERROR_NOT_FOUND,
                    "No such room member");

        return NULL;
    }

    if (third_party != NULL) {
        *third_party = data->thirdparty;
    }

    return data->profile;
}

/**
 * matrix_room_remove_member:
 * @room: a #MatrixRoom
 * @user_id: a Matrix ID
 * @error: (nullable): a #GError, or %NULL to ignore errors
 *
 * Removes @user_id from the member list.
 */
void
matrix_room_remove_member(MatrixRoom *matrix_room, const gchar *user_id, GError **error)
{
    MatrixRoomMemberData *data;
    MatrixRoomPrivate *priv;

    g_return_if_fail(matrix_room != NULL);
    g_return_if_fail(user_id != NULL);

    priv = matrix_room_get_instance_private(matrix_room);

    if ((data = g_hash_table_lookup(priv->members, user_id)) == NULL) {
        g_set_error(error, MATRIX_ERROR, MATRIX_ERROR_NOT_FOUND,
                    "No such room member");

        return;
    }

    g_hash_table_remove(priv->members, user_id);
}

/**
 * matrix_room_clear_user_levels:
 * @room: a #MatrixRoom
 *
 * Clear the stored individual user levels.  This should be called eg. when receiving a new
 * `m.room.power_levels` event.
 */
void
matrix_room_clear_user_levels(MatrixRoom *matrix_room)
{
    MatrixRoomPrivate *priv;

    g_return_if_fail(matrix_room != NULL);

    priv = matrix_room_get_instance_private(matrix_room);

    g_hash_table_remove_all(priv->user_levels);
}

/**
 * matrix_room_set_user_level:
 * @room: a #MatrixRoom
 * @user_id: a fully qualified Matrix ID
 * @level: the new power level
 *
 * Set an individual power level for a user.
 */
void
matrix_room_set_user_level(MatrixRoom *matrix_room, const gchar *user_id, gint level)
{
    MatrixRoomPrivate *priv;

    g_return_if_fail(matrix_room != NULL);
    g_return_if_fail(user_id != NULL);

    priv = matrix_room_get_instance_private(matrix_room);

    g_hash_table_insert (priv->user_levels, g_strdup(user_id), GINT_TO_POINTER(level));
}

/**
 * matrix_room_get_user_level:
 * @room: a #MatrixRoom
 * @user_id: a fully qualified Matrix ID
 *
 * Get the power level of a user.  If the user doesn’t have an individually set power level,
 * the default value is returned.
 *
 * Returns: the level of the user.
 */
gint
matrix_room_get_user_level(MatrixRoom *matrix_room, const gchar *user_id)
{
    MatrixRoomPrivate *priv;

    g_return_val_if_fail(matrix_room != NULL, 0);
    g_return_val_if_fail(user_id != NULL, 0);

    priv = matrix_room_get_instance_private(matrix_room);

    gpointer level = NULL;

    if (!g_hash_table_lookup_extended(priv->user_levels, user_id, NULL, &level)) {
        return priv->default_power_level;
    }

    return GPOINTER_TO_INT(level);
}

/**
 * matrix_room_clear_event_levels:
 * @room: a #MatrixRoom
 *
 * Clear the stored event level requirements.  This should be called eg. when receiving a new
 * `m.room.power_levels` event.
 */
void
matrix_room_clear_event_levels(MatrixRoom *matrix_room)
{
    MatrixRoomPrivate *priv;

    g_return_if_fail(matrix_room != NULL);

    priv = matrix_room_get_instance_private(matrix_room);

    g_hash_table_remove_all(priv->event_levels);
}

/**
 * matrix_room_set_event_level:
 * @room: a #MatrixRoom
 * @event_type: the event type to restrict
 * @level: the desired level for the event type
 *
 * Set the required level to send an event of type @event_type.
 */
void
matrix_room_set_event_level(MatrixRoom *matrix_room, const gchar *event_type, gint level)
{
    MatrixRoomPrivate *priv;

    g_return_if_fail(matrix_room != NULL);
    g_return_if_fail (event_type != NULL);

    priv = matrix_room_get_instance_private(matrix_room);

    g_hash_table_insert(priv->event_levels, g_strdup(event_type), GINT_TO_POINTER(level));
}

/**
 * matrix_room_get_event_level:
 * @room: a #MatrixRoom
 * @event_type: the event type to query
 * @error: (nullable): a #GError, or %NULL to ignore errors
 *
 * Get the required level to send an event of type @event_type.  If there is no level
 * requirement set for this event type, this function returns %NULL as there is no way to
 * decide if #MatrixRoom:default-state-level or #MatrixRoom:default-event-level should be used.
 *
 * Returns: the level required to send a specific event
 */
gint
matrix_room_get_event_level(MatrixRoom *matrix_room, const gchar *event_type, GError **error)
{
    MatrixRoomPrivate *priv;
    gpointer level;

    g_return_val_if_fail(matrix_room != NULL, 0);
    g_return_val_if_fail(event_type != NULL, 0);

    priv = matrix_room_get_instance_private(matrix_room);

    if (!g_hash_table_lookup_extended(priv->event_levels, event_type, NULL, &level)) {
        g_set_error(error, MATRIX_ERROR, MATRIX_ERROR_NOT_FOUND, "No level set for this event type");

        return 0;
    }

    return GPOINTER_TO_INT(level);
}

const gchar *
matrix_room_get_room_id(MatrixRoom *matrix_room)
{
    MatrixRoomPrivate *priv;

    g_return_val_if_fail(matrix_room != NULL, NULL);

    priv = matrix_room_get_instance_private(matrix_room);

    return priv->room_id;
}

static void
matrix_room_set_room_id(MatrixRoom *matrix_room, const gchar *room_id)
{
    MatrixRoomPrivate *priv;

    g_return_if_fail(matrix_room != NULL);

    priv = matrix_room_get_instance_private(matrix_room);

    if (g_strcmp0(room_id, priv->room_id) != 0) {
        g_free(priv->room_id);
        priv->room_id = g_strdup(room_id);

        g_object_notify_by_pspec((GObject *)matrix_room, matrix_room_properties[PROP_ROOM_ID]);
    }
}

/**
 * matrix_room_get_aliases:
 * @room: a #MatrixRoom
 * @n_aliases: (nullable): a placeholder for the length of the list
 *
 * Returns: (transfer none): the list of room aliases
 */
gchar **
matrix_room_get_aliases(MatrixRoom *matrix_room, int *n_aliases)
{
    MatrixRoomPrivate *priv;

    g_return_val_if_fail(matrix_room != NULL, NULL);

    priv = matrix_room_get_instance_private(matrix_room);

    if (n_aliases != NULL) {
        *n_aliases = priv->aliases_len;
    }

    return priv->aliases;
}

/**
 * matrix_room_set_aliases:
 * @room: a #MatrixRoom
 * @aliases: a list of aliases to assign to @room
 * @n_aliases: the length of @aliases
 */
void
matrix_room_set_aliases(MatrixRoom *matrix_room, const gchar **aliases, int n_aliases)
{
    MatrixRoomPrivate *priv;

    g_return_if_fail(matrix_room != NULL);

    priv = matrix_room_get_instance_private(matrix_room);

    for (gint i = 0; i < priv->aliases_len; i++) {
        g_free(priv->aliases[i]);
    }

    g_free(priv->aliases);

    priv->aliases = g_new(gchar *, n_aliases);

    for (gint i = 0; i < n_aliases; i++) {
        priv->aliases[i] = g_strdup(aliases[i]);
    }

    g_object_notify_by_pspec((GObject *)matrix_room, matrix_room_properties[PROP_ALIASES]);
}

const gchar *
matrix_room_get_avatar_url(MatrixRoom *matrix_room)
{
    MatrixRoomPrivate *priv;

    g_return_val_if_fail(matrix_room != NULL, NULL);

    priv = matrix_room_get_instance_private(matrix_room);

    return priv->avatar_url;
}

void
matrix_room_set_avatar_url(MatrixRoom *matrix_room, const gchar *avatar_url)
{
    MatrixRoomPrivate *priv;

    g_return_if_fail(matrix_room != NULL);

    priv = matrix_room_get_instance_private(matrix_room);

    if (g_strcmp0(avatar_url, priv->avatar_url) != 0) {
        g_free(priv->avatar_url);
        priv->avatar_url = g_strdup(avatar_url);

        g_object_notify_by_pspec((GObject *)matrix_room, matrix_room_properties[PROP_AVATAR_URL]);
    }
}

MatrixImageInfo *
matrix_room_get_avatar_info(MatrixRoom *matrix_room)
{
    MatrixRoomPrivate *priv;

    g_return_val_if_fail(matrix_room != NULL, NULL);

    priv = matrix_room_get_instance_private(matrix_room);

    return priv->avatar_info;
}

void
matrix_room_set_avatar_info(MatrixRoom *matrix_room, MatrixImageInfo *avatar_info)
{
    MatrixRoomPrivate *priv;

    g_return_if_fail(matrix_room != NULL);

    priv = matrix_room_get_instance_private(matrix_room);

    if (priv->avatar_info != avatar_info) {
        matrix_image_info_unref(priv->avatar_info);
        priv->avatar_info = matrix_image_info_ref(avatar_info);

        g_object_notify_by_pspec((GObject *)matrix_room, matrix_room_properties[PROP_AVATAR_INFO]);
    }
}

const gchar *
matrix_room_get_avatar_thumbnail_url(MatrixRoom *matrix_room)
{
    MatrixRoomPrivate *priv;

    g_return_val_if_fail(matrix_room != NULL, NULL);

    priv = matrix_room_get_instance_private(matrix_room);

    return priv->avatar_thumbnail_url;
}

void
matrix_room_set_avatar_thumbnail_url(MatrixRoom *matrix_room, const gchar *avatar_thumbnail_url)
{
    MatrixRoomPrivate *priv;

    g_return_if_fail(matrix_room != NULL);

    priv = matrix_room_get_instance_private(matrix_room);

    if (g_strcmp0(avatar_thumbnail_url, priv->avatar_thumbnail_url) != 0) {
        g_free(priv->avatar_thumbnail_url);
        priv->avatar_thumbnail_url = g_strdup(avatar_thumbnail_url);

        g_object_notify_by_pspec((GObject *)matrix_room, matrix_room_properties[PROP_AVATAR_THUMBNAIL_URL]);
    }
}

MatrixImageInfo *
matrix_room_get_avatar_thumbnail_info(MatrixRoom *matrix_room)
{
    MatrixRoomPrivate *priv;

    g_return_val_if_fail(matrix_room != NULL, NULL);

    priv = matrix_room_get_instance_private(matrix_room);

    return priv->avatar_thumbnail_info;
}

void
matrix_room_set_avatar_thumbnail_info(MatrixRoom *matrix_room, MatrixImageInfo *avatar_thumbnail_info)
{
    MatrixRoomPrivate *priv;

    g_return_if_fail(matrix_room != NULL);

    priv = matrix_room_get_instance_private(matrix_room);

    if (priv->avatar_thumbnail_info != avatar_thumbnail_info) {
        matrix_image_info_unref(priv->avatar_thumbnail_info);
        priv->avatar_thumbnail_info = matrix_image_info_ref(avatar_thumbnail_info);

        g_object_notify_by_pspec((GObject *)matrix_room, matrix_room_properties[PROP_AVATAR_THUMBNAIL_INFO]);
    }
}

const gchar *
matrix_room_get_canonical_alias(MatrixRoom *matrix_room)
{
    MatrixRoomPrivate *priv;

    g_return_val_if_fail(matrix_room != NULL, NULL);

    priv = matrix_room_get_instance_private(matrix_room);

    return priv->canonical_alias;
}

void
matrix_room_set_canonical_alias(MatrixRoom *matrix_room, const gchar *canonical_alias)
{
    MatrixRoomPrivate *priv;

    g_return_if_fail(matrix_room != NULL);

    priv = matrix_room_get_instance_private(matrix_room);

    if (g_strcmp0(canonical_alias, priv->canonical_alias) != 0) {
        g_free(priv->canonical_alias);
        priv->canonical_alias = g_strdup(canonical_alias);

        g_object_notify_by_pspec((GObject *)matrix_room, matrix_room_properties[PROP_CANONICAL_ALIAS]);
    }
}

const gchar *
matrix_room_get_creator(MatrixRoom *matrix_room)
{
    MatrixRoomPrivate *priv;

    g_return_val_if_fail(matrix_room != NULL, NULL);

    priv = matrix_room_get_instance_private(matrix_room);

    return priv->creator;
}

void
matrix_room_set_creator(MatrixRoom *matrix_room, const gchar *creator)
{
    MatrixRoomPrivate *priv;

    g_return_if_fail(matrix_room != NULL);

    priv = matrix_room_get_instance_private(matrix_room);

    if (g_strcmp0(creator, priv->creator) != 0) {
        g_free(priv->creator);
        priv->creator = g_strdup(creator);

        g_object_notify_by_pspec((GObject *)matrix_room, matrix_room_properties[PROP_CREATOR]);
    }
}

gboolean
matrix_room_get_federate(MatrixRoom *matrix_room)
{
    MatrixRoomPrivate *priv;

    g_return_val_if_fail(matrix_room != NULL, FALSE);

    priv = matrix_room_get_instance_private(matrix_room);

    return priv->federate;
}

void
matrix_room_set_federate(MatrixRoom *matrix_room, gboolean federate)
{
    MatrixRoomPrivate *priv;

    g_return_if_fail(matrix_room != NULL);

    priv = matrix_room_get_instance_private(matrix_room);

    if (priv->federate != federate) {
        priv->federate = federate;

        g_object_notify_by_pspec((GObject *)matrix_room, matrix_room_properties[PROP_FEDERATE]);
    }
}

MatrixGuestAccess
matrix_room_get_guest_access(MatrixRoom *matrix_room)
{
    MatrixRoomPrivate *priv;

    g_return_val_if_fail(matrix_room != NULL, 0);

    priv = matrix_room_get_instance_private(matrix_room);

    return priv->guest_access;
}

void
matrix_room_set_guest_access(MatrixRoom *matrix_room, MatrixGuestAccess guest_access)
{
    MatrixRoomPrivate *priv;

    g_return_if_fail(matrix_room != NULL);

    priv = matrix_room_get_instance_private(matrix_room);

    if (priv->guest_access != guest_access) {
        priv->guest_access = guest_access;

        g_object_notify_by_pspec((GObject *)matrix_room, matrix_room_properties[PROP_GUEST_ACCESS]);
    }
}

MatrixHistoryVisibility
matrix_room_get_history_visibility(MatrixRoom *matrix_room)
{
    MatrixRoomPrivate *priv;

    g_return_val_if_fail(matrix_room != NULL, 0);

    priv = matrix_room_get_instance_private(matrix_room);

    return priv->history_visibility;
}

void
matrix_room_set_history_visibility(MatrixRoom *matrix_room, MatrixHistoryVisibility history_visibility)
{
    MatrixRoomPrivate *priv;

    g_return_if_fail(matrix_room != NULL);

    priv = matrix_room_get_instance_private(matrix_room);

    if (priv->history_visibility != history_visibility) {
        priv->history_visibility = history_visibility;

        g_object_notify_by_pspec((GObject *)matrix_room, matrix_room_properties[PROP_HISTORY_VISIBILITY]);
    }
}

MatrixJoinRules
matrix_room_get_join_rules(MatrixRoom *matrix_room)
{
    MatrixRoomPrivate *priv;

    g_return_val_if_fail(matrix_room != NULL, 0);

    priv = matrix_room_get_instance_private(matrix_room);

    return priv->join_rules;
}

void
matrix_room_set_join_rules(MatrixRoom *matrix_room, MatrixJoinRules join_rules)
{
    MatrixRoomPrivate *priv;

    g_return_if_fail(matrix_room != NULL);

    priv = matrix_room_get_instance_private(matrix_room);

    if (priv->join_rules != join_rules) {
        priv->join_rules = join_rules;

        g_object_notify_by_pspec((GObject *)matrix_room, matrix_room_properties[PROP_JOIN_RULES]);
    }
}

const gchar *
matrix_room_get_name(MatrixRoom *matrix_room)
{
    MatrixRoomPrivate *priv;

    g_return_val_if_fail(matrix_room != NULL, NULL);

    priv = matrix_room_get_instance_private(matrix_room);

    return priv->name;
}

void
matrix_room_set_name(MatrixRoom *matrix_room, const gchar *name)
{
    MatrixRoomPrivate *priv;

    g_return_if_fail(matrix_room != NULL);

    priv = matrix_room_get_instance_private(matrix_room);

    if (g_strcmp0(name, priv->name) != 0) {
        g_free(priv->name);
        priv->name = g_strdup(name);

        g_object_notify_by_pspec((GObject *)matrix_room, matrix_room_properties[PROP_NAME]);
    }
}

gint
matrix_room_get_default_power_level(MatrixRoom *matrix_room)
{
    MatrixRoomPrivate *priv;

    g_return_val_if_fail(matrix_room != NULL, 0);

    priv = matrix_room_get_instance_private(matrix_room);

    return priv->default_power_level;
}

void
matrix_room_set_default_power_level(MatrixRoom *matrix_room, gint default_power_level)
{
    MatrixRoomPrivate *priv;

    g_return_if_fail(matrix_room != NULL);

    priv = matrix_room_get_instance_private(matrix_room);

    if (priv->default_power_level != default_power_level) {
        priv->default_power_level = default_power_level;

        g_object_notify_by_pspec((GObject *)matrix_room, matrix_room_properties[PROP_DEFAULT_POWER_LEVEL]);
    }
}

gint
matrix_room_get_default_event_level(MatrixRoom *matrix_room)
{
    MatrixRoomPrivate *priv;

    g_return_val_if_fail(matrix_room != NULL, 0);

    priv = matrix_room_get_instance_private(matrix_room);

    return priv->default_event_level;
}

void
matrix_room_set_default_event_level(MatrixRoom *matrix_room, gint default_event_level)
{
    MatrixRoomPrivate *priv;

    g_return_if_fail(matrix_room != NULL);

    priv = matrix_room_get_instance_private(matrix_room);

    if (priv->default_event_level != default_event_level) {
        priv->default_event_level = default_event_level;

        g_object_notify_by_pspec((GObject *)matrix_room, matrix_room_properties[PROP_DEFAULT_EVENT_LEVEL]);
    }
}

gint
matrix_room_get_default_state_level(MatrixRoom *matrix_room)
{
    MatrixRoomPrivate *priv;

    g_return_val_if_fail(matrix_room != NULL, 0);

    priv = matrix_room_get_instance_private(matrix_room);

    return priv->default_state_level;
}

void
matrix_room_set_default_state_level(MatrixRoom *matrix_room, gint defaul_state_level)
{
    MatrixRoomPrivate *priv;

    g_return_if_fail(matrix_room != NULL);

    priv = matrix_room_get_instance_private(matrix_room);

    if (priv->default_state_level != defaul_state_level) {
        priv->default_state_level = defaul_state_level;

        g_object_notify_by_pspec((GObject *)matrix_room, matrix_room_properties[PROP_DEFAULT_STATE_LEVEL]);
    }
}

gint
matrix_room_get_ban_level(MatrixRoom *matrix_room)
{
    MatrixRoomPrivate *priv;

    g_return_val_if_fail(matrix_room != NULL, 0);

    priv = matrix_room_get_instance_private(matrix_room);

    return priv->ban_level;
}

void
matrix_room_set_ban_level(MatrixRoom *matrix_room, gint ban_level)
{
    MatrixRoomPrivate *priv;

    g_return_if_fail(matrix_room != NULL);

    priv = matrix_room_get_instance_private(matrix_room);

    if (priv->ban_level != ban_level) {
        priv->ban_level = ban_level;

        g_object_notify_by_pspec((GObject *)matrix_room, matrix_room_properties[PROP_BAN_LEVEL]);
    }
}

gint
matrix_room_get_kick_level(MatrixRoom *matrix_room)
{
    MatrixRoomPrivate *priv;

    g_return_val_if_fail(matrix_room != NULL, 0);

    priv = matrix_room_get_instance_private(matrix_room);

    return priv->kick_level;
}

void
matrix_room_set_kick_level(MatrixRoom *matrix_room, gint kick_level)
{
    MatrixRoomPrivate *priv;

    g_return_if_fail(matrix_room != NULL);

    priv = matrix_room_get_instance_private(matrix_room);

    if (priv->kick_level != kick_level) {
        priv->kick_level = kick_level;

        g_object_notify_by_pspec((GObject *)matrix_room, matrix_room_properties[PROP_KICK_LEVEL]);
    }
}

gint
matrix_room_get_redact_level(MatrixRoom *matrix_room)
{
    MatrixRoomPrivate *priv;

    g_return_val_if_fail(matrix_room != NULL, 0);

    priv = matrix_room_get_instance_private(matrix_room);

    return priv->redact_level;
}

void
matrix_room_set_redact_level(MatrixRoom *matrix_room, gint redact_level)
{
    MatrixRoomPrivate *priv;

    g_return_if_fail(matrix_room != NULL);

    priv = matrix_room_get_instance_private(matrix_room);

    if (priv->redact_level != redact_level) {
        priv->redact_level = redact_level;

        g_object_notify_by_pspec((GObject *)matrix_room, matrix_room_properties[PROP_REDACT_LEVEL]);
    }
}

gint
matrix_room_get_invite_level(MatrixRoom *matrix_room)
{
    MatrixRoomPrivate *priv;

    g_return_val_if_fail(matrix_room != NULL, 0);

    priv = matrix_room_get_instance_private(matrix_room);

    return priv->invite_level;
}

void
matrix_room_set_invite_level(MatrixRoom *matrix_room, gint invite_level)
{
    MatrixRoomPrivate *priv;

    g_return_if_fail(matrix_room != NULL);

    priv = matrix_room_get_instance_private(matrix_room);

    if (priv->invite_level != invite_level) {
        priv->invite_level = invite_level;

        g_object_notify_by_pspec((GObject *)matrix_room, matrix_room_properties[PROP_INVITE_LEVEL]);
    }
}

const gchar *
matrix_room_get_topic(MatrixRoom *matrix_room)
{
    MatrixRoomPrivate *priv;

    g_return_val_if_fail(matrix_room != NULL, NULL);

    priv = matrix_room_get_instance_private(matrix_room);

    return priv->topic;
}

void
matrix_room_set_topic(MatrixRoom *matrix_room, const gchar *topic)
{
    MatrixRoomPrivate *priv;

    g_return_if_fail(matrix_room != NULL);

    priv = matrix_room_get_instance_private(matrix_room);

    if (g_strcmp0(topic, priv->topic) != 0) {
        g_free(priv->topic);
        priv->topic = g_strdup(topic);

        g_object_notify_by_pspec((GObject *)matrix_room, matrix_room_properties[PROP_TOPIC]);
    }
}

/**
 * matrix_room_get_typing_users:
 * @room: a #MatrixRoom
 * @n_typing_users: (nullable): placeholder for the length of the returned list
 *
 * Returns: (transfer none): the list of typing users in @room
 */
gchar **
matrix_room_get_typing_users(MatrixRoom *matrix_room, int *n_typing_users)
{
    MatrixRoomPrivate *priv;

    g_return_val_if_fail(matrix_room != NULL, NULL);

    priv = matrix_room_get_instance_private(matrix_room);

    if (n_typing_users != NULL) {
        *n_typing_users = priv->typing_users_len;
    }

    return priv->typing_users;
}

void
matrix_room_set_typing_users(MatrixRoom *matrix_room, gchar **typing_users, int n_typing_users)
{
    MatrixRoomPrivate *priv;

    g_return_if_fail(matrix_room != NULL);

    priv = matrix_room_get_instance_private(matrix_room);

    for (gint i = 0; i < priv->typing_users_len; i++) {
        g_free(priv->typing_users[i]);
    }

    g_free(priv->typing_users);

    priv->typing_users = g_new(gchar *, n_typing_users);

    for (gint i = 0; i < n_typing_users; i++) {
        priv->typing_users[i] = g_strdup(typing_users[i]);
    }

    g_object_notify_by_pspec((GObject *)matrix_room, matrix_room_properties[PROP_TYPING_USERS]);
}

static void
matrix_room_finalize(GObject *gobject)
{
    MatrixRoomPrivate *priv = matrix_room_get_instance_private(MATRIX_ROOM(gobject));

    g_free(priv->room_id);

    for (gint i = 0; i < priv->aliases_len; i++) {
        g_free(priv->aliases[i]);
    }

    g_free(priv->aliases);
    g_free(priv->avatar_url);
    matrix_image_info_unref(priv->avatar_info);
    g_free(priv->avatar_thumbnail_url);
    matrix_image_info_unref(priv->avatar_thumbnail_info);
    g_free(priv->canonical_alias);
    g_free(priv->creator);
    g_free(priv->name);
    g_free(priv->topic);

    for (gint i = 0; i < priv->typing_users_len; i++) {
        g_free(priv->typing_users[i]);
    }

    g_free(priv->typing_users);
    g_hash_table_unref(priv->event_levels);
    g_hash_table_unref(priv->user_levels);
    g_hash_table_unref(priv->members);

    G_OBJECT_CLASS(matrix_room_parent_class)->finalize(gobject);
}

static void
matrix_room_get_property(GObject *gobject, guint property_id, GValue *value, GParamSpec *pspec)
{
    MatrixRoom *matrix_room = MATRIX_ROOM(gobject);

    switch (property_id) {
        case PROP_ROOM_ID:
            g_value_set_string(value, matrix_room_get_room_id(matrix_room));

            break;
        case PROP_ALIASES:
            g_value_set_boxed(value, matrix_room_get_aliases(matrix_room, NULL));

            break;
        case PROP_AVATAR_URL:
            g_value_set_string(value, matrix_room_get_avatar_url(matrix_room));

            break;
        case PROP_AVATAR_THUMBNAIL_URL:
            g_value_set_string(value, matrix_room_get_avatar_thumbnail_url(matrix_room));

            break;
        case PROP_CANONICAL_ALIAS:
            g_value_set_string(value, matrix_room_get_canonical_alias(matrix_room));

            break;
        case PROP_CREATOR:
            g_value_set_string(value, matrix_room_get_creator(matrix_room));

            break;
        case PROP_FEDERATE:
            g_value_set_boolean(value, matrix_room_get_federate(matrix_room));

            break;
        case PROP_GUEST_ACCESS:
            g_value_set_enum(value, matrix_room_get_guest_access(matrix_room));

            break;
        case PROP_HISTORY_VISIBILITY:
            g_value_set_enum(value, matrix_room_get_history_visibility(matrix_room));

            break;
        case PROP_JOIN_RULES:
            g_value_set_enum(value, matrix_room_get_join_rules(matrix_room));

            break;
        case PROP_NAME:
            g_value_set_string(value, matrix_room_get_name(matrix_room));

            break;
        case PROP_DEFAULT_POWER_LEVEL:
            g_value_set_int(value, matrix_room_get_default_power_level(matrix_room));

            break;
        case PROP_DEFAULT_EVENT_LEVEL:
            g_value_set_int(value, matrix_room_get_default_event_level(matrix_room));

            break;
        case PROP_DEFAULT_STATE_LEVEL:
            g_value_set_int(value, matrix_room_get_default_state_level(matrix_room));

            break;
        case PROP_BAN_LEVEL:
            g_value_set_int(value, matrix_room_get_ban_level(matrix_room));

            break;
        case PROP_KICK_LEVEL:
            g_value_set_int(value, matrix_room_get_kick_level(matrix_room));

            break;
        case PROP_REDACT_LEVEL:
            g_value_set_int(value, matrix_room_get_redact_level(matrix_room));

            break;
        case PROP_INVITE_LEVEL:
            g_value_set_int(value, matrix_room_get_invite_level(matrix_room));

            break;
        case PROP_TOPIC:
            g_value_set_string(value, matrix_room_get_topic(matrix_room));

            break;
        case PROP_TYPING_USERS:
            g_value_set_boxed(value, matrix_room_get_typing_users(matrix_room, NULL));

            break;
        case PROP_AVATAR_INFO:
            g_value_set_boxed(value, matrix_room_get_avatar_info(matrix_room));

            break;
        case PROP_AVATAR_THUMBNAIL_INFO:
            g_value_set_boxed(value, matrix_room_get_avatar_thumbnail_info(matrix_room));

            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, property_id, pspec);

            break;
    }
}

static void
matrix_room_set_property(GObject *gobject, guint property_id, const GValue *value, GParamSpec *pspec)
{
    MatrixRoom * matrix_room = MATRIX_ROOM(gobject);

    switch (property_id) {
        case PROP_ROOM_ID:
            matrix_room_set_room_id(matrix_room, g_value_get_string(value));

            break;
        case PROP_ALIASES:
        {
            gpointer boxed = g_value_get_boxed(value);

            matrix_room_set_aliases(matrix_room, boxed, (boxed == NULL) ? 0 : g_strv_length(boxed));

            break;
        }
        case PROP_AVATAR_URL:
            matrix_room_set_avatar_url(matrix_room, g_value_get_string(value));

            break;
        case PROP_AVATAR_THUMBNAIL_URL:
            matrix_room_set_avatar_thumbnail_url(matrix_room, g_value_get_string(value));

            break;
        case PROP_CANONICAL_ALIAS:
            matrix_room_set_canonical_alias(matrix_room, g_value_get_string(value));

            break;
        case PROP_CREATOR:
            matrix_room_set_creator(matrix_room, g_value_get_string(value));

            break;
        case PROP_FEDERATE:
            matrix_room_set_federate(matrix_room, g_value_get_boolean(value));

            break;
        case PROP_GUEST_ACCESS:
            matrix_room_set_guest_access(matrix_room, g_value_get_enum(value));

            break;
        case PROP_HISTORY_VISIBILITY:
            matrix_room_set_history_visibility(matrix_room, g_value_get_enum(value));

            break;
        case PROP_JOIN_RULES:
            matrix_room_set_join_rules(matrix_room, g_value_get_enum (value));

            break;
        case PROP_NAME:
            matrix_room_set_name(matrix_room, g_value_get_string(value));

            break;
        case PROP_DEFAULT_POWER_LEVEL:
            matrix_room_set_default_power_level(matrix_room, g_value_get_int(value));

            break;
        case PROP_DEFAULT_EVENT_LEVEL:
            matrix_room_set_default_event_level(matrix_room, g_value_get_int(value));

            break;
        case PROP_DEFAULT_STATE_LEVEL:
            matrix_room_set_default_state_level(matrix_room, g_value_get_int(value));

            break;
        case PROP_BAN_LEVEL:
            matrix_room_set_ban_level(matrix_room, g_value_get_int(value));

            break;
        case PROP_KICK_LEVEL:
            matrix_room_set_kick_level(matrix_room, g_value_get_int(value));

            break;
        case PROP_REDACT_LEVEL:
            matrix_room_set_redact_level(matrix_room, g_value_get_int(value));

            break;
        case PROP_INVITE_LEVEL:
            matrix_room_set_invite_level(matrix_room, g_value_get_int(value));

            break;
        case PROP_TOPIC:
            matrix_room_set_topic(matrix_room, g_value_get_string(value));

            break;
        case PROP_TYPING_USERS:
        {
            gpointer boxed = g_value_get_boxed(value);

            matrix_room_set_typing_users(matrix_room, boxed, (boxed == NULL) ? 0 : g_strv_length(boxed));

            break;
        }
        case PROP_AVATAR_INFO:
            matrix_room_set_avatar_info(matrix_room, g_value_get_boxed(value));

            break;
        case PROP_AVATAR_THUMBNAIL_INFO:
            matrix_room_set_avatar_thumbnail_info(matrix_room, g_value_get_boxed(value));

            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, property_id, pspec);

            break;
    }
}

static void
matrix_room_class_init(MatrixRoomClass *klass)
{
    G_OBJECT_CLASS(klass)->get_property = matrix_room_get_property;
    G_OBJECT_CLASS(klass)->set_property = matrix_room_set_property;
    G_OBJECT_CLASS(klass)->finalize = matrix_room_finalize;

    /**
     * MatrixRoom:room-id:
     *
     * The ID of the room this object belongs to.
     */
    matrix_room_properties[PROP_ROOM_ID] = g_param_spec_string(
            "room-id", "room-id", "room-id",
            NULL,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_ROOM_ID, matrix_room_properties[PROP_ROOM_ID]);

    /**
     * MatrixRoom:aliases:
     *
     * All the known room aliases.
     */
    matrix_room_properties[PROP_ALIASES] = g_param_spec_boxed(
            "aliases", "aliases", "aliases",
            G_TYPE_STRV,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_ALIASES, matrix_room_properties[PROP_ALIASES]);

    /**
     * MatrixRoom:avatar-url:
     *
     * The URL of the room’s avatar.
     */
    matrix_room_properties[PROP_AVATAR_URL] = g_param_spec_string(
            "avatar-url", "avatar-url", "avatar-url",
            NULL,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_AVATAR_URL, matrix_room_properties[PROP_AVATAR_URL]);

    /**
     * MatrixRoom:avatar-thumbnail-url:
     *
     * The URL of the room avatar’s thumbnail.
     */
    matrix_room_properties[PROP_AVATAR_THUMBNAIL_URL] = g_param_spec_string(
            "avatar-thumbnail-url", "avatar-thumbnail-url", "avatar-thumbnail-url",
            NULL,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_AVATAR_THUMBNAIL_URL, matrix_room_properties[PROP_AVATAR_THUMBNAIL_URL]);

    /**
     * MatrixRoom:canonical-alias:
     *
     * The canonical alias of the room.
     */
    matrix_room_properties[PROP_CANONICAL_ALIAS] = g_param_spec_string(
            "canonical-alias", "canonical-alias", "canonical-alias",
            NULL,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_CANONICAL_ALIAS, matrix_room_properties[PROP_CANONICAL_ALIAS]);

    /**
     * MatrixRoom:creator:
     *
     * The Matrix ID of the room’s creator.
     */
    matrix_room_properties[PROP_CREATOR] = g_param_spec_string(
            "creator", "creator", "creator",
            NULL,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_CREATOR, matrix_room_properties[PROP_CREATOR]);

    /**
     * MatrixRoom:federate:
     *
     * If %FALSE, the room is not federated.
     */
    matrix_room_properties[PROP_FEDERATE] = g_param_spec_boolean(
            "federate", "federate", "federate",
            FALSE,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_FEDERATE, matrix_room_properties[PROP_FEDERATE]);

    /**
     * MatrixRoom:guest-access:
     *
     * Wether guests are allowed to join the room.
     */
    matrix_room_properties[PROP_GUEST_ACCESS] = g_param_spec_enum(
            "guest-access", "guest-access", "guest-access",
            MATRIX_TYPE_GUEST_ACCESS, MATRIX_GUEST_ACCESS_UNKNOWN,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_GUEST_ACCESS, matrix_room_properties[PROP_GUEST_ACCESS]);

    /**
     * MatrixRoom:history-visibility:
     * This value controls the visibility of the room’s history.
     */
    matrix_room_properties[PROP_HISTORY_VISIBILITY] = g_param_spec_enum(
            "history-visibility", "history-visibility", "history-visibility",
            MATRIX_TYPE_HISTORY_VISIBILITY, MATRIX_HISTORY_VISIBILITY_UNKNOWN,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_HISTORY_VISIBILITY, matrix_room_properties[PROP_HISTORY_VISIBILITY]);
    /**
     * MatrixRoom:join-rules:
     *
     * Controls who can join the room.
     */
    matrix_room_properties[PROP_JOIN_RULES] = g_param_spec_enum(
            "join-rules", "join-rules", "join-rules",
            MATRIX_TYPE_JOIN_RULES, MATRIX_JOIN_RULES_UNKNOWN,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_JOIN_RULES, matrix_room_properties[PROP_JOIN_RULES]);

    /**
     * MatrixRoom:name:
     *
     * The room’s name.
     */
    matrix_room_properties[PROP_NAME] = g_param_spec_string(
            "name", "name", "name",
            NULL,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_NAME, matrix_room_properties[PROP_NAME]);

    /**
     * MatrixRoom:default-power-level:
     *
     * The default power level users get upon joining the room.
     */
    matrix_room_properties[PROP_DEFAULT_POWER_LEVEL] = g_param_spec_int(
            "default-power-level", "default-power-level", "default-power-level",
            G_MININT, G_MAXINT, 0,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_DEFAULT_POWER_LEVEL, matrix_room_properties[PROP_DEFAULT_POWER_LEVEL]);

    /**
     * MatrixRoom:default-event-level:
     *
     * The power level required to send non-state events to the room.  This gets applied if
     * the event type doesn’t have an explicit level requirement (see
     * matrix_room_set_event_level() and matrix_room_get_event_level() for more information).
     */
    matrix_room_properties[PROP_DEFAULT_EVENT_LEVEL] = g_param_spec_int(
            "default-event-level", "default-event-level", "default-event-level",
            G_MININT, G_MAXINT, 0,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_DEFAULT_EVENT_LEVEL, matrix_room_properties[PROP_DEFAULT_EVENT_LEVEL]);

    /**
     * MatrixRoom:default-state-level:
     *
     * The power level required to send state events to the room.  This get applied if the
     * event type doesn’t have an explicit level requirement (see matrix_room_set_event_level()
     * and matrix_room_get_event_level() for more information).
     */
    matrix_room_properties[PROP_DEFAULT_STATE_LEVEL] = g_param_spec_int(
            "default-state-level", "default-state-level", "default-state-level",
            G_MININT, G_MAXINT, 10,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_DEFAULT_STATE_LEVEL, matrix_room_properties[PROP_DEFAULT_STATE_LEVEL]);

    /**
     * MatrixRoom:ban-level:
     *
     * The power level required to ban other users from the room.
     */
    matrix_room_properties[PROP_BAN_LEVEL] = g_param_spec_int(
            "ban-level", "ban-level", "ban-level",
            G_MININT, G_MAXINT, 5,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_BAN_LEVEL, matrix_room_properties[PROP_BAN_LEVEL]);

    /**
     * MatrixRoom:kick-level:
     *
     * The power level required to kick other users from the room.
     */
    matrix_room_properties[PROP_KICK_LEVEL] = g_param_spec_int(
            "kick-level", "kick-level", "kick-level",
            G_MININT, G_MAXINT, 5,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_KICK_LEVEL, matrix_room_properties[PROP_KICK_LEVEL]);

    /**
     * MatrixRoom:redact-level:
     *
     * The power level required to redact events in the room.
     */
    matrix_room_properties[PROP_REDACT_LEVEL] = g_param_spec_int(
            "redact-level", "redact-level", "redact-level",
            G_MININT, G_MAXINT, 20,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_REDACT_LEVEL, matrix_room_properties[PROP_REDACT_LEVEL]);

    /**
     * MatrixRoom:invite-level:
     *
     * The power level required to invite users to the room.
     */
    matrix_room_properties[PROP_INVITE_LEVEL] = g_param_spec_int(
            "invite-level", "invite-level", "invite-level",
            G_MININT, G_MAXINT, 0,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_INVITE_LEVEL, matrix_room_properties[PROP_INVITE_LEVEL]);

    /**
     * MatrixRoom:topic:
     *
     * The room’s topic.
     */
    matrix_room_properties[PROP_TOPIC] = g_param_spec_string(
            "topic", "topic", "topic",
            NULL,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_TOPIC, matrix_room_properties[PROP_TOPIC]);

    /**
     * MatrixRoom:typing-users:
     *
     * The users currently typing in the room.
     */
    matrix_room_properties[PROP_TYPING_USERS] = g_param_spec_boxed(
            "typing-users", "typing-users", "typing-users",
            G_TYPE_STRV,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_TYPING_USERS, matrix_room_properties[PROP_TYPING_USERS]);

    /**
     * MatrixRoom:avatar-info:
     *
     * A #MatrixImageInfo describing #MatrixRoom:avatar-url.
     */
    matrix_room_properties[PROP_AVATAR_INFO] = g_param_spec_boxed(
            "avatar-info", "avatar-info", "avatar-info",
            MATRIX_TYPE_IMAGE_INFO,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_AVATAR_INFO, matrix_room_properties[PROP_AVATAR_INFO]);

    /**
     * MatrixRoom:avatar-thumbnail-info:
     *
     * A #MatrixImageInfo describing #MatrixRoom:avatar-thumbnail-url:
     */
    matrix_room_properties[PROP_AVATAR_THUMBNAIL_INFO] = g_param_spec_boxed(
            "avatar-thumbnail-info", "avatar-thumbnail-info", "avatar-thumbnail-info",
            MATRIX_TYPE_IMAGE_INFO,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_AVATAR_THUMBNAIL_INFO, matrix_room_properties[PROP_AVATAR_THUMBNAIL_INFO]);
}

static void
matrix_room_init(MatrixRoom *matrix_room)
{
    MatrixRoomPrivate *priv = matrix_room_get_instance_private(matrix_room);

    priv->avatar_url = NULL;
    priv->avatar_info = NULL;
    priv->avatar_thumbnail_url = NULL;
    priv->avatar_thumbnail_info = NULL;
    priv->canonical_alias = NULL;
    priv->creator = NULL;
    priv->federate = FALSE;
    priv->guest_access = MATRIX_GUEST_ACCESS_UNKNOWN;
    priv->history_visibility = MATRIX_HISTORY_VISIBILITY_UNKNOWN;
    priv->join_rules = MATRIX_JOIN_RULES_UNKNOWN;
    priv->name = NULL;
    priv->default_power_level = 0;
    priv->default_event_level = 0;
    priv->default_state_level = 10;
    priv->ban_level = 5;
    priv->kick_level = 5;
    priv->redact_level = 20;
    priv->invite_level = 0;
    priv->topic = NULL;
    priv->event_levels = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, g_free);
    priv->user_levels = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, g_free);
    priv->members = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, (GDestroyNotify)matrix_room_member_data_free);
}
