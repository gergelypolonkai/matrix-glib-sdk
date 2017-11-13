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

#ifndef __MATRIX_GLIB_SDK_EVENT_ROOM_BASE_H__
# define __MATRIX_GLIB_SDK_EVENT_ROOM_BASE_H__

# include <glib-object.h>
# include "matrix-event-base.h"

# define MATRIX_EVENT_TYPE_ROOM matrix_event_room_get_type()
G_DECLARE_DERIVABLE_TYPE(MatrixEventRoom, matrix_event_room, MATRIX_EVENT, ROOM, MatrixEventBase);

struct _MatrixEventRoomClass {
    MatrixEventBaseClass parent_class;
};

MatrixEventRoom *matrix_event_room_construct(GType object_type);
const gchar *matrix_event_room_get_event_id(MatrixEventRoom *event);
void matrix_event_room_set_event_id(MatrixEventRoom *event, const gchar *event_id);
const gchar *matrix_event_room_get_room_id(MatrixEventRoom *event);
void matrix_event_room_set_room_id(MatrixEventRoom *event, const gchar *room_id);
const gchar *matrix_event_room_get_sender(MatrixEventRoom *event);
void matrix_event_room_set_sender(MatrixEventRoom *event, const gchar *sender);
glong matrix_event_room_get_age(MatrixEventRoom *event);
void matrix_event_room_set_age(MatrixEventRoom *event, glong age);
const gchar *matrix_event_room_get_redacted_because(MatrixEventRoom *event);
void matrix_event_room_set_redacted_because(MatrixEventRoom *event, const gchar *redacted_because);
const gchar *matrix_event_room_get_transaction_id(MatrixEventRoom *event);
void matrix_event_room_set_transaction_id(MatrixEventRoom *event, const gchar *transaction_id);

#endif  /* __MATRIX_GLIB_SDK_EVENT_ROOM_BASE_H__ */
