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

#ifndef __MATRIX_GLIB_SDK_EVENT_ROOM_JOIN_RULES_H__
# define __MATRIX_GLIB_SDK_EVENT_ROOM_JOIN_RULES_H__

# include <glib-object.h>
# include "matrix-event-state-base.h"
# include "matrix-types.h"

G_BEGIN_DECLS

#define MATRIX_EVENT_TYPE_ROOM_JOIN_RULES (matrix_event_room_join_rules_get_type ())
G_DECLARE_DERIVABLE_TYPE(MatrixEventRoomJoinRules, matrix_event_room_join_rules, MATRIX_EVENT, ROOM_JOIN_RULES, MatrixEventState)

struct _MatrixEventRoomJoinRulesClass {
    MatrixEventStateClass parent_class;
};

MatrixEventRoomJoinRules *matrix_event_room_join_rules_new (void);
MatrixJoinRules matrix_event_room_join_rules_get_join_rules (MatrixEventRoomJoinRules *event);
void matrix_event_room_join_rules_set_join_rules (MatrixEventRoomJoinRules *event, MatrixJoinRules join_rules);

G_END_DECLS

#endif  /* __MATRIX_GLIB_SDK_EVENT_ROOM_JOIN_RULES_H__ */
