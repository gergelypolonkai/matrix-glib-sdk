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

#ifndef __MATRIX_GLIB_SDK_ROOM_H__
# define __MATRIX_GLIB_SDK_ROOM_H__

# include <glib-object.h>
# include "matrix-profile.h"
# include "matrix-types.h"

G_BEGIN_DECLS

# define MATRIX_TYPE_ROOM matrix_room_get_type()
G_DECLARE_DERIVABLE_TYPE(MatrixRoom, matrix_room, MATRIX, ROOM, GObject)

struct _MatrixRoomClass {
    GObjectClass parent_class;
};

MatrixRoom *matrix_room_new(const gchar *room_id);
void matrix_room_add_member(MatrixRoom *room, const gchar *user_id, MatrixProfile *profile, gboolean third_party, GError **error);
MatrixProfile *matrix_room_get_or_add_member(MatrixRoom *room, const gchar *user_id, gboolean third_party, GError **error);
MatrixProfile *matrix_room_get_member(MatrixRoom *room, const gchar *user_id, gboolean *third_party, GError **error);
void matrix_room_remove_member(MatrixRoom *room, const gchar *user_id, GError **error);
void matrix_room_clear_user_levels(MatrixRoom *room);
void matrix_room_set_user_level(MatrixRoom *room, const gchar *user_id, gint level);
gint matrix_room_get_user_level(MatrixRoom *room, const gchar *user_id);
void matrix_room_clear_event_levels(MatrixRoom *room);
void matrix_room_set_event_level(MatrixRoom *room, const gchar *event_type, gint level);
gint matrix_room_get_event_level(MatrixRoom *room, const gchar *event_type, GError **error);
const gchar *matrix_room_get_room_id(MatrixRoom *room);
gchar **matrix_room_get_aliases(MatrixRoom *room, int *n_aliases);
void matrix_room_set_aliases(MatrixRoom *room, const gchar **aliases, int n_aliases);
const gchar *matrix_room_get_avatar_url(MatrixRoom *room);
void matrix_room_set_avatar_url(MatrixRoom *room, const gchar *avatar_url);
MatrixImageInfo *matrix_room_get_avatar_info(MatrixRoom *room);
void matrix_room_set_avatar_info(MatrixRoom *room, MatrixImageInfo *avatar_info);
const gchar *matrix_room_get_avatar_thumbnail_url(MatrixRoom *room);
void matrix_room_set_avatar_thumbnail_url(MatrixRoom *room, const gchar *avatar_thumbnail_url);
MatrixImageInfo *matrix_room_get_avatar_thumbnail_info(MatrixRoom *room);
void matrix_room_set_avatar_thumbnail_info(MatrixRoom *room, MatrixImageInfo *avatar_thumbnail_info);
const gchar *matrix_room_get_canonical_alias(MatrixRoom *room);
void matrix_room_set_canonical_alias(MatrixRoom *room, const gchar *canonical_alias);
const gchar *matrix_room_get_creator(MatrixRoom *room);
void matrix_room_set_creator(MatrixRoom *room, const gchar *creator);
gboolean matrix_room_get_federate(MatrixRoom *room);
void matrix_room_set_federate(MatrixRoom *room, gboolean federate);
MatrixGuestAccess matrix_room_get_guest_access(MatrixRoom *room);
void matrix_room_set_guest_access(MatrixRoom *room, MatrixGuestAccess guest_access);
MatrixHistoryVisibility matrix_room_get_history_visibility(MatrixRoom *room);
void matrix_room_set_history_visibility(MatrixRoom *room, MatrixHistoryVisibility history_visibility);
MatrixJoinRules matrix_room_get_join_rules(MatrixRoom *room);
void matrix_room_set_join_rules(MatrixRoom *room, MatrixJoinRules join_rules);
const gchar *matrix_room_get_name(MatrixRoom *room);
void matrix_room_set_name(MatrixRoom *room, const gchar *name);
gint matrix_room_get_default_power_level(MatrixRoom *room);
void matrix_room_set_default_power_level(MatrixRoom *room, gint default_power_level);
gint matrix_room_get_default_event_level(MatrixRoom *room);
void matrix_room_set_default_event_level(MatrixRoom *room, gint default_event_level);
gint matrix_room_get_default_state_level(MatrixRoom *room);
void matrix_room_set_default_state_level(MatrixRoom *room, gint default_state_level);
gint matrix_room_get_ban_level(MatrixRoom *room);
void matrix_room_set_ban_level(MatrixRoom *room, gint ban_level);
gint matrix_room_get_kick_level(MatrixRoom *room);
void matrix_room_set_kick_level(MatrixRoom *room, gint kick_level);
gint matrix_room_get_redact_level(MatrixRoom *room);
void matrix_room_set_redact_level(MatrixRoom *room, gint redact_level);
gint matrix_room_get_invite_level(MatrixRoom *room);
void matrix_room_set_invite_level(MatrixRoom *room, gint invite_level);
const gchar *matrix_room_get_topic(MatrixRoom *room);
void matrix_room_set_topic(MatrixRoom *room, const gchar *topic);
gchar **matrix_room_get_typing_users(MatrixRoom *room, int *n_typing_users);
void matrix_room_set_typing_users(MatrixRoom *room, gchar **typing_users, int n_typing_users);

G_END_DECLS

#endif  /* __MATRIX_GLIB_SDK_ROOM_H__ */
