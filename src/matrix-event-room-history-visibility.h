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

#ifndef __MATRIX_GLIB_SDK_EVENT_ROOM_HISTORY_VISIBILITY_H__
# define __MATRIX_GLIB_SDK_EVENT_ROOM_HISTORY_VISIBILITY_H__

# include <glib-object.h>
# include "matrix-event-state-base.h"
# include "matrix-types.h"

G_BEGIN_DECLS

#define MATRIX_EVENT_TYPE_ROOM_HISTORY_VISIBILITY (matrix_event_room_history_visibility_get_type ())
G_DECLARE_DERIVABLE_TYPE(MatrixEventRoomHistoryVisibility, matrix_event_room_history_visibility, MATRIX_EVENT, ROOM_HISTORY_VISIBILITY, MatrixEventState)

struct _MatrixEventRoomHistoryVisibilityClass {
    MatrixEventStateClass parent_class;
};

MatrixEventRoomHistoryVisibility *matrix_event_room_history_visibility_new(void);
MatrixHistoryVisibility matrix_event_room_history_visibility_get_visibility(MatrixEventRoomHistoryVisibility *event);
void matrix_event_room_history_visibility_set_visibility(MatrixEventRoomHistoryVisibility *event, MatrixHistoryVisibility history_visibility);

G_END_DECLS

#endif  /* __MATRIX_GLIB_SDK_EVENT_ROOM_HISTORY_VISIBILITY_H__ */
