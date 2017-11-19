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

#ifndef __MATRIX_GLIB_SDK_EVENT_CALL_BASE_H__
# define __MATRIX_GLIB_SDK_EVENT_CALL_BASE_H__

# include <glib-object.h>
# include "matrix-event-room-base.h"

G_BEGIN_DECLS

# define MATRIX_EVENT_TYPE_CALL (matrix_event_call_get_type ())
# define MATRIX_EVENT_CALL(o) (G_TYPE_CHECK_INSTANCE_CAST((o), MATRIX_EVENT_TYPE_CALL, MatrixEventCall))
# define MATRIX_EVENT_CALL_CLASS(c) (G_TYPE_CHECK_CLASS_CAST((c), MATRIX_EVENT_TYPE_CALL, MatrixEventCallClass))
# define MATRIX_EVENT_IS_CALL(o) (G_TYPE_CHECK_INSTANCE_TYPE((o), MATRIX_EVENT_TYPE_CALL))
# define MATRIX_EVENT_IS_CALL_CLASS(c) (G_TYPE_CHECK_CLASS_TYPE((c), MATRIX_EVENT_TYPE_CALL))
# define MATRIX_EVENT_CALL_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS((o), MATRIX_EVENT_TYPE_CALL, MatrixEventCallClass))

typedef struct _MatrixEventCall MatrixEventCall;
typedef struct _MatrixEventCallClass MatrixEventCallClass;
G_DEFINE_AUTOPTR_CLEANUP_FUNC(MatrixEventCall, g_object_unref)

struct _MatrixEventCall {
    MatrixEventRoom parent_instance;
};

struct _MatrixEventCallClass {
    MatrixEventRoomClass parent_class;
};

GType matrix_event_call_get_type (void) G_GNUC_CONST;
MatrixEventCall* matrix_event_call_construct (GType object_type);
const gchar *matrix_event_call_get_call_id (MatrixEventCall *event);
void matrix_event_call_set_call_id (MatrixEventCall *event, const gchar *call_id);
gint matrix_event_call_get_version (MatrixEventCall *event);
void matrix_event_call_set_version (MatrixEventCall *event, gint version);

G_END_DECLS

#endif  /* __MATRIX_GLIB_SDK_EVENT_CALL_BASE_H__ */
