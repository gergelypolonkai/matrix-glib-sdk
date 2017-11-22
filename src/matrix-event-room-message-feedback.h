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

#ifndef __MATRIX_GLIB_SDK_EVENT_ROOM_MESSAGE_FEEDBACK_H__
# define __MATRIX_GLIB_SDK_EVENT_ROOM_MESSAGE_FEEDBACK_H__

# include <glib-object.h>
# include "matrix-event-room-base.h"

G_BEGIN_DECLS

#define MATRIX_EVENT_TYPE_ROOM_MESSAGE_FEEDBACK (matrix_event_room_message_feedback_get_type ())
G_DECLARE_DERIVABLE_TYPE(MatrixEventRoomMessageFeedback, matrix_event_room_message_feedback, MATRIX_EVENT, ROOM_MESSAGE_FEEDBACK, MatrixEventRoom);

struct _MatrixEventRoomMessageFeedbackClass {
    MatrixEventRoomClass parent_class;
};

MatrixEventRoomMessageFeedback *matrix_event_room_message_feedback_new(void);
MatrixEventRoomMessageFeedback *matrix_event_room_message_feedback_construct(GType object_type);
const gchar *matrix_event_room_message_feedback_get_feedback_type(MatrixEventRoomMessageFeedback *event);
void matrix_event_room_message_feedback_set_feedback_type(MatrixEventRoomMessageFeedback *event, const gchar *feedback_type);
const gchar *matrix_event_room_message_feedback_get_target_event_id(MatrixEventRoomMessageFeedback *event);
void matrix_event_room_message_feedback_set_target_event_id(MatrixEventRoomMessageFeedback *event, const gchar *target_event_id);

G_END_DECLS

#endif  /* __MATRIX_GLIB_SDK_EVENT_ROOM_MESSAGE_FEEDBACK_H__ */
