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

#ifndef __MATRIX_GLIB_SDK_EVENT_TYPING_H__
# define __MATRIX_GLIB_SDK_EVENT_TYPING_H__

# include <glib-object.h>
# include "matrix-event-base.h"

G_BEGIN_DECLS

# define MATRIX_EVENT_TYPE_TYPING matrix_event_typing_get_type()
G_DECLARE_DERIVABLE_TYPE(MatrixEventTyping, matrix_event_typing, MATRIX_EVENT, TYPING, MatrixEventBase)

struct _MatrixEventTypingClass {
    MatrixEventBaseClass parent_class;
};

MatrixEventTyping* matrix_event_typing_new(void);
const gchar* matrix_event_typing_get_room_id(MatrixEventTyping *event);
void matrix_event_typing_set_room_id(MatrixEventTyping *event, const gchar *room_id);
gchar **matrix_event_typing_get_user_ids(MatrixEventTyping *event, int *n_user_ids);
void matrix_event_typing_set_user_ids(MatrixEventTyping *event, gchar **user_ids, int n_user_ids);

G_END_DECLS

#endif  /* __MATRIX_GLIB_SDK_EVENT_TYPING_H__ */
