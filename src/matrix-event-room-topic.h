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

#ifndef __MATRIX_GLIB_SDK_EVENT_ROOM_TOPIC_H__
# define __MATRIX_GLIB_SDK_EVENT_ROOM_TOPIC_H__

# include <glib-object.h>
# include "matrix-event-state-base.h"

G_BEGIN_DECLS

# define MATRIX_EVENT_TYPE_ROOM_TOPIC matrix_event_room_topic_get_type ()
G_DECLARE_DERIVABLE_TYPE(MatrixEventRoomTopic, matrix_event_room_topic, MATRIX_EVENT, ROOM_TOPIC, MatrixEventState)

struct _MatrixEventRoomTopicClass {
    MatrixEventStateClass parent_class;
};

MatrixEventRoomTopic* matrix_event_room_topic_new (void);
const gchar* matrix_event_room_topic_get_topic (MatrixEventRoomTopic* self);
void matrix_event_room_topic_set_topic (MatrixEventRoomTopic* self, const gchar* value);

G_END_DECLS

#endif  /* __MATRIX_GLIB_SDK_EVENT_ROOM_TOPIC_H__ */
