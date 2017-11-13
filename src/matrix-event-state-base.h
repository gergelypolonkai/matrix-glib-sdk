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

#ifndef __MATRIX_GLIB_SDK_EVENT_STATE_BASE_H__
# define __MATRIX_GLIB_SDK_EVENT_STATE_BASE_H__

# include <glib-object.h>
# include <json-glib/json-glib.h>
# include "matrix-event-room-base.h"

# define MATRIX_EVENT_TYPE_STATE matrix_event_state_get_type()
#define MATRIX_EVENT_STATE(o) (G_TYPE_CHECK_INSTANCE_CAST((o), MATRIX_EVENT_TYPE_STATE, MatrixEventState))
#define MATRIX_EVENT_STATE_CLASS(c) (G_TYPE_CHECK_CLASS_CAST((c), MATRIX_EVENT_TYPE_STATE, MatrixEventStateClass))
#define MATRIX_EVENT_IS_STATE(o) (G_TYPE_CHECK_INSTANCE_TYPE((o), MATRIX_EVENT_TYPE_STATE))
#define MATRIX_EVENT_IS_STATE_CLASS(c) (G_TYPE_CHECK_CLASS_TYPE((c), MATRIX_EVENT_TYPE_STATE))
#define MATRIX_EVENT_STATE_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS((o), MATRIX_EVENT_TYPE_STATE, MatrixEventStateClass))

typedef struct _MatrixEventStateClass MatrixEventStateClass;
typedef struct _MatrixEventState MatrixEventState;
G_DEFINE_AUTOPTR_CLEANUP_FUNC(MatrixEventState, g_object_unref)

struct _MatrixEventStateClass {
    MatrixEventRoomClass parent_class;
};

struct _MatrixEventState {
    MatrixEventRoom parent_instance;
};

GType matrix_event_state_get_type(void);
JsonNode *matrix_event_state_get_stripped_node(MatrixEventState *event);
MatrixEventState *matrix_event_state_construct(GType object_type);
const gchar *matrix_event_state_get_state_key(MatrixEventState *event);
void matrix_event_state_set_state_key(MatrixEventState *event, const gchar *state_key);
JsonNode *matrix_event_state_get_prev_content(MatrixEventState *event);
void matrix_event_state_set_prev_content(MatrixEventState *event, JsonNode *prev_content);

#endif  /* __MATRIX_GLIB_SDK_EVENT_STATE_BASE_H__ */
