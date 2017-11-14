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

#ifndef __MATRIX_GLIB_SDK_EVENT_PRESENCE_H__
# define __MATRIX_GLIB_SDK_EVENT_PRESENCE_H__

# include <glib-object.h>
# include "matrix-event-base.h"
# include "matrix-types.h"

G_BEGIN_DECLS

# define MATRIX_EVENT_TYPE_PRESENCE matrix_event_presence_get_type()
G_DECLARE_DERIVABLE_TYPE(MatrixEventPresence, matrix_event_presence, MATRIX_EVENT, PRESENCE, MatrixEventBase);

struct _MatrixEventPresenceClass {
    MatrixEventBaseClass parent_class;
};

GType matrix_event_presence_get_type (void) G_GNUC_CONST;
MatrixEventPresence* matrix_event_presence_new (void);
MatrixEventPresence* matrix_event_presence_construct (GType object_type);
const gchar* matrix_event_presence_get_avatar_url (MatrixEventPresence *event);
void matrix_event_presence_set_avatar_url (MatrixEventPresence *event, const gchar *avatar_url);
const gchar* matrix_event_presence_get_display_name (MatrixEventPresence *event);
void matrix_event_presence_set_display_name (MatrixEventPresence *event, const gchar *display_name);
glong matrix_event_presence_get_last_active_ago (MatrixEventPresence *event);
void matrix_event_presence_set_last_active_ago (MatrixEventPresence *event, glong last_active_ago);
const gchar* matrix_event_presence_get_user_id (MatrixEventPresence *event);
void matrix_event_presence_set_user_id (MatrixEventPresence *event, const gchar *user_id);
MatrixPresence matrix_event_presence_get_presence (MatrixEventPresence *event);
void matrix_event_presence_set_presence (MatrixEventPresence *event, MatrixPresence presence);

G_END_DECLS

#endif  /* __MATRIX_GLIB_SDK_EVENT_PRESENCE_H__ */
