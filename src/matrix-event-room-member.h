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

#ifndef __MATRX_GLIB_SDK_EVENT_ROOM_MEMBER_H__
# define __MATRX_GLIB_SDK_EVENT_ROOM_MEMBER_H__

# include <glib-object.h>
# include "matrix-event-state-base.h"
# include "matrix-types.h"

G_BEGIN_DECLS

# define MATRIX_EVENT_TYPE_ROOM_MEMBER matrix_event_room_member_get_type()
G_DECLARE_DERIVABLE_TYPE(MatrixEventRoomMember, matrix_event_room_member, MATRIX_EVENT, ROOM_MEMBER, MatrixEventState)

struct _MatrixEventRoomMemberClass {
    MatrixEventStateClass parent_class;
};

MatrixEventRoomMember *matrix_event_room_member_new (void);
MatrixRoomMembership matrix_event_room_member_get_membership (MatrixEventRoomMember *event);
void matrix_event_room_member_set_membership (MatrixEventRoomMember *event, MatrixRoomMembership membership);
const gchar *matrix_event_room_member_get_avatar_url (MatrixEventRoomMember *event);
void matrix_event_room_member_set_avatar_url (MatrixEventRoomMember *event, const gchar *avatar_url);
const gchar *matrix_event_room_member_get_display_name (MatrixEventRoomMember *event);
void matrix_event_room_member_set_display_name (MatrixEventRoomMember *event, const gchar *display_name);
const gchar *matrix_event_room_member_get_tpi_display_name (MatrixEventRoomMember *event);
void matrix_event_room_member_set_tpi_display_name (MatrixEventRoomMember *event, const gchar *tpi_display_name);
const gchar *matrix_event_room_member_get_tpi_signed_mxid (MatrixEventRoomMember *event);
void matrix_event_room_member_set_tpi_signed_mxid (MatrixEventRoomMember *event, const gchar *tpi_signed_mxid);
const gchar *matrix_event_room_member_get_tpi_signed_token (MatrixEventRoomMember *event);
void matrix_event_room_member_set_tpi_signed_token (MatrixEventRoomMember *event, const gchar *tpi_signed_token);
JsonNode *matrix_event_room_member_get_tpi_signature (MatrixEventRoomMember *event);
void matrix_event_room_member_set_tpi_signature (MatrixEventRoomMember *event, JsonNode *tpi_signature);
MatrixEventState **matrix_event_room_member_get_invite_room_state (MatrixEventRoomMember *event, int *n_invite_room_state);
void matrix_event_room_member_set_invite_room_state (MatrixEventRoomMember *event, MatrixEventState **invite_room_state, int n_invite_room_state);
const gchar *matrix_event_room_member_get_user_id (MatrixEventRoomMember *event);
void matrix_event_room_member_set_user_id (MatrixEventRoomMember *event, const gchar *user_id);

G_END_DECLS

#endif  /* __MATRX_GLIB_SDK_EVENT_ROOM_MEMBER_H__ */
