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

#ifndef __MATRIX_GLIB_SDK_EVENT_ROOM_NAME_H__
# define __MATRIX_GLIB_SDK_EVENT_ROOM_NAME_H__

# include <glib-object.h>
# include "matrix-event-state-base.h"

G_BEGIN_DECLS

# define MATRIX_EVENT_TYPE_ROOM_NAME matrix_event_room_name_get_type()
G_DECLARE_DERIVABLE_TYPE(MatrixEventRoomName, matrix_event_room_name, MATRIX_EVENT, ROOM_NAME, MatrixEventState);

struct _MatrixEventRoomNameClass {
    MatrixEventStateClass parent_class;
};

MatrixEventRoomName *matrix_event_room_name_new(void);
MatrixEventRoomName *matrix_event_room_name_construct(GType object_type);
const gchar *matrix_event_room_name_get_name(MatrixEventRoomName *event);
void matrix_event_room_name_set_name(MatrixEventRoomName *event, const gchar *name);

G_END_DECLS

#endif  /* __MATRIX_GLIB_SDK_EVENT_ROOM_NAME_H__ */
