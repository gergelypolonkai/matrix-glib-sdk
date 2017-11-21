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

#ifndef __MATRIX_GLIB_SDK_EVENT_ROOM_MESSAGE_H__
# define __MATRIX_GLIB_SDK_EVENT_ROOM_MESSAGE_H__

# include <glib-object.h>
# include "matrix-event-room-base.h"
# include "matrix-message-base.h"

G_BEGIN_DECLS

#define MATRIX_EVENT_TYPE_ROOM_MESSAGE matrix_event_room_message_get_type()
G_DECLARE_DERIVABLE_TYPE(MatrixEventRoomMessage, matrix_event_room_message, MATRIX_EVENT, ROOM_MESSAGE, MatrixEventRoom);

struct _MatrixEventRoomMessageClass {
    MatrixEventRoomClass parent_class;
};

MatrixEventRoomMessage *matrix_event_room_message_new(void);
MatrixEventRoomMessage *matrix_event_room_message_construct(GType object_type);
GType matrix_message_base_get_type(void) G_GNUC_CONST;
MatrixMessageBase *matrix_event_room_message_get_message(MatrixEventRoomMessage *event);
void matrix_event_room_message_set_message(MatrixEventRoomMessage *event, MatrixMessageBase *message);
JsonNode *matrix_event_room_message_get_fallback_content(MatrixEventRoomMessage *event);
const gchar *matrix_event_room_message_get_body(MatrixEventRoomMessage *event);
void matrix_event_room_message_set_body(MatrixEventRoomMessage *event, const gchar *body);

G_END_DECLS

#endif  /* __MATRIX_GLIB_SDK_EVENT_ROOM_MESSAGE_H__ */
