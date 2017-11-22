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

#ifndef __MATRIX_GLIB_SDK_EVENT_ROOM_THIRD_PARTY_INVITE_H__
# define __MATRIX_GLIB_SDK_EVENT_ROOM_THIRD_PARTY_INVITE_H__

# include <glib-object.h>
# include "matrix-event-state-base.h"

G_BEGIN_DECLS

typedef struct _MatrixThirdPartyInvitePublicKey MatrixThirdPartyInvitePublicKey;

GType matrix_third_party_invite_public_key_get_type(void) G_GNUC_CONST;
# define MATRIX_TYPE_THIRD_PARTY_INVITE_PUBLIC_KEY (matrix_third_party_invite_public_key_get_type())

MatrixThirdPartyInvitePublicKey *matrix_third_party_invite_public_key_new(void);
MatrixThirdPartyInvitePublicKey *matrix_third_party_invite_public_key_ref(MatrixThirdPartyInvitePublicKey *matrix_third_party_invite_public_key);
void matrix_third_party_invite_public_key_unref(MatrixThirdPartyInvitePublicKey *matrix_third_party_invite_public_key);
const gchar *matrix_third_party_invite_public_key_get_key(MatrixThirdPartyInvitePublicKey *third_party_invite_public_key);
void matrix_third_party_invite_public_key_set_key(MatrixThirdPartyInvitePublicKey *matrix_third_party_invite_public_key, const gchar *key);
const gchar *matrix_third_party_invite_public_key_get_validity_url(MatrixThirdPartyInvitePublicKey *matrix_third_party_invite_public_key);
void matrix_third_party_invite_public_key_set_validity_url(MatrixThirdPartyInvitePublicKey *matrix_third_party_invite_public_key, const gchar *validity_url);

#define MATRIX_EVENT_TYPE_ROOM_THIRD_PARTY_INVITE (matrix_event_room_third_party_invite_get_type ())
G_DECLARE_DERIVABLE_TYPE(MatrixEventRoomThirdPartyInvite, matrix_event_room_third_party_invite, MATRIX_EVENT, ROOM_THIRD_PARTY_INVITE, MatrixEventState);

struct _MatrixEventRoomThirdPartyInviteClass {
    MatrixEventStateClass parent_class;
};

MatrixEventRoomThirdPartyInvite* matrix_event_room_third_party_invite_new (void);
MatrixEventRoomThirdPartyInvite* matrix_event_room_third_party_invite_construct (GType object_type);
const gchar* matrix_event_room_third_party_invite_get_display_name (MatrixEventRoomThirdPartyInvite* self);
void matrix_event_room_third_party_invite_set_display_name (MatrixEventRoomThirdPartyInvite* self, const gchar* value);
const gchar* matrix_event_room_third_party_invite_get_key_validity_url (MatrixEventRoomThirdPartyInvite* self);
void matrix_event_room_third_party_invite_set_key_validity_url (MatrixEventRoomThirdPartyInvite* self, const gchar* value);
const gchar* matrix_event_room_third_party_invite_get_public_key (MatrixEventRoomThirdPartyInvite* self);
void matrix_event_room_third_party_invite_set_public_key (MatrixEventRoomThirdPartyInvite* self, const gchar* value);
const MatrixThirdPartyInvitePublicKey **matrix_event_room_third_party_invite_get_public_keys(MatrixEventRoomThirdPartyInvite *matrix_event_room_third_party_invite, int *n_public_keys);

G_END_DECLS

#endif  /* __MATRIX_GLIB_SDK_EVENT_ROOM_THIRD_PARTY_INVITE_H__ */
