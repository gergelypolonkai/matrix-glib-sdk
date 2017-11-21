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

#ifndef __MATRIX_GLIB_SDK_EVENT_CALL_INVITE_H__
# define __MATRIX_GLIB_SDK_EVENT_CALL_INVITE_H__

# include <glib-object.h>
# include "matrix-event-call-base.h"
# include "matrix-enumtypes.h"

G_BEGIN_DECLS

#define MATRIX_EVENT_TYPE_CALL_INVITE (matrix_event_call_invite_get_type ())
G_DECLARE_DERIVABLE_TYPE(MatrixEventCallInvite, matrix_event_call_invite, MATRIX_EVENT, CALL_INVITE, MatrixEventCall);

struct _MatrixEventCallInviteClass {
    MatrixEventCallClass parent_class;
};

MatrixEventCallInvite* matrix_event_call_invite_new (void);
MatrixEventCallInvite* matrix_event_call_invite_construct (GType object_type);
MatrixCallOfferType matrix_event_call_invite_get_offer_type (MatrixEventCallInvite *event);
void matrix_event_call_invite_set_offer_type (MatrixEventCallInvite *event, MatrixCallOfferType offer_type);
const gchar* matrix_event_call_invite_get_sdp (MatrixEventCallInvite *event);
void matrix_event_call_invite_set_sdp (MatrixEventCallInvite *event, const gchar* sdp);
gint matrix_event_call_invite_get_lifetime (MatrixEventCallInvite *event);
void matrix_event_call_invite_set_lifetime (MatrixEventCallInvite *event, gint lifetime);

G_END_DECLS

#endif  /* __MATRIX_GLIB_SDK_EVENT_CALL_INVITE_H__ */
