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

#ifndef __MATRIX_GLIB_SDK_EVENT_ROOM_AVATAR_H__
# define __MATRIX_GLIB_SDK_EVENT_ROOM_AVATAR_H__

# include <glib-object.h>
# include "matrix-event-state-base.h"
# include "matrix-types.h"

G_BEGIN_DECLS

# define MATRIX_EVENT_TYPE_ROOM_AVATAR matrix_event_room_avatar_get_type()
G_DECLARE_DERIVABLE_TYPE(MatrixEventRoomAvatar, matrix_event_room_avatar, MATRIX_EVENT, ROOM_AVATAR, MatrixEventState)

struct _MatrixEventRoomAvatarClass {
    MatrixEventStateClass parent_class;
};

MatrixEventRoomAvatar* matrix_event_room_avatar_new (void);
const gchar *matrix_event_room_avatar_get_url (MatrixEventRoomAvatar *event);
void matrix_event_room_avatar_set_url (MatrixEventRoomAvatar *event, const gchar *url);
const gchar *matrix_event_room_avatar_get_thumbnail_url (MatrixEventRoomAvatar *event);
void matrix_event_room_avatar_set_thumbnail_url (MatrixEventRoomAvatar *event, const gchar *thumbnail_url);
MatrixImageInfo *matrix_event_room_avatar_get_info (MatrixEventRoomAvatar *event);
void matrix_event_room_avatar_set_info (MatrixEventRoomAvatar *event, MatrixImageInfo *info);
MatrixImageInfo *matrix_event_room_avatar_get_thumbnail_info (MatrixEventRoomAvatar *event);
void matrix_event_room_avatar_set_thumbnail_info (MatrixEventRoomAvatar *event, MatrixImageInfo *thumbnail_info);

G_END_DECLS

#endif  /* __MATRIX_GLIB_SDK_EVENT_ROOM_AVATAR_H__ */
