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

#ifndef __MATRIX_GLIB_SDK_EVENT_ROOM_REDACTION_H__
# define __MATRIX_GLIB_SDK_EVENT_ROOM_REDACTION_H__

# include <glib-object.h>
# include "matrix-event-room-base.h"

G_BEGIN_DECLS

#define MATRIX_EVENT_TYPE_ROOM_REDACTION (matrix_event_room_redaction_get_type ())
G_DECLARE_DERIVABLE_TYPE(MatrixEventRoomRedaction, matrix_event_room_redaction, MATRIX_EVENT, ROOM_REDACTION, MatrixEventRoom);

struct _MatrixEventRoomRedactionClass {
    MatrixEventRoomClass parent_class;
};

MatrixEventRoomRedaction* matrix_event_room_redaction_new (void);
MatrixEventRoomRedaction* matrix_event_room_redaction_construct (GType object_type);
const gchar* matrix_event_room_redaction_get_reason (MatrixEventRoomRedaction* self);
void matrix_event_room_redaction_set_reason (MatrixEventRoomRedaction* self, const gchar* value);
const gchar* matrix_event_room_redaction_get_redacted_event_id (MatrixEventRoomRedaction* self);
void matrix_event_room_redaction_set_redacted_event_id (MatrixEventRoomRedaction* self, const gchar* value);

G_END_DECLS

#endif  /* __MATRIX_GLIB_SDK_EVENT_ROOM_REDACTION_H__ */
