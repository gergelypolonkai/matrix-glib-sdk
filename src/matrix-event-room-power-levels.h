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

#ifndef __MATRIX_GLIB_SDK_EVENT_ROOM_POWER_LEVELS_H__
# define __MATRIX_GLIB_SDK_EVENT_ROOM_POWER_LEVELS_H__

# include <glib-object.h>
# include "matrix-event-state-base.h"

G_BEGIN_DECLS

#define MATRIX_EVENT_TYPE_ROOM_POWER_LEVELS (matrix_event_room_power_levels_get_type ())
G_DECLARE_DERIVABLE_TYPE(MatrixEventRoomPowerLevels, matrix_event_room_power_levels, MATRIX_EVENT, ROOM_POWER_LEVELS, MatrixEventState);

struct _MatrixEventRoomPowerLevelsClass {
    MatrixEventStateClass parent_class;
};

MatrixEventRoomPowerLevels *matrix_event_room_power_levels_new(void);
MatrixEventRoomPowerLevels *matrix_event_room_power_levels_construct(GType object_type);
void matrix_event_room_power_levels_set_user_level(MatrixEventRoomPowerLevels *event, const gchar *user_id, gint level);
void matrix_event_room_power_levels_set_event_level(MatrixEventRoomPowerLevels *event, const gchar *event_type, gint level);
gint matrix_event_room_power_levels_get_users_default(MatrixEventRoomPowerLevels *event);
void matrix_event_room_power_levels_set_users_default(MatrixEventRoomPowerLevels *event, gint users_default);
gint matrix_event_room_power_levels_get_events_default(MatrixEventRoomPowerLevels *event);
void matrix_event_room_power_levels_set_events_default(MatrixEventRoomPowerLevels *event, gint events_default);
gint matrix_event_room_power_levels_get_state_default(MatrixEventRoomPowerLevels *event);
void matrix_event_room_power_levels_set_state_default(MatrixEventRoomPowerLevels *event, gint state_default);
gint matrix_event_room_power_levels_get_ban(MatrixEventRoomPowerLevels *event);
void matrix_event_room_power_levels_set_ban(MatrixEventRoomPowerLevels *event, gint ban);
gint matrix_event_room_power_levels_get_kick(MatrixEventRoomPowerLevels *event);
void matrix_event_room_power_levels_set_kick(MatrixEventRoomPowerLevels *event, gint kick);
gint matrix_event_room_power_levels_get_redact(MatrixEventRoomPowerLevels *event);
void matrix_event_room_power_levels_set_redact(MatrixEventRoomPowerLevels *event, gint redact);
gint matrix_event_room_power_levels_get_invite(MatrixEventRoomPowerLevels *event);
void matrix_event_room_power_levels_set_invite(MatrixEventRoomPowerLevels *event, gint invite);
GHashTable *matrix_event_room_power_levels_get_event_levels(MatrixEventRoomPowerLevels *event);
GHashTable *matrix_event_room_power_levels_get_user_levels(MatrixEventRoomPowerLevels *event);

G_END_DECLS

#endif  /* __MATRIX_GLIB_SDK_EVENT_ROOM_POWER_LEVELS_H__ */
