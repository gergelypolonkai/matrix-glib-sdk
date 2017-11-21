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

#ifndef __MATRIX_GLIB_SDK_EVENT_CALL_ANSWER_H__
# define __MATRIX_GLIB_SDK_EVENT_CALL_ANSWER_H__

# include <glib-object.h>
# include "matrix-event-call-base.h"
# include "matrix-types.h"

G_BEGIN_DECLS

# define MATRIX_EVENT_TYPE_CALL_ANSWER (matrix_event_call_answer_get_type ())
G_DECLARE_DERIVABLE_TYPE(MatrixEventCallAnswer, matrix_event_call_answer, MATRIX_EVENT, CALL_ANSWER, MatrixEventCall)

struct _MatrixEventCallAnswerClass {
    MatrixEventCallClass parent_class;
};

MatrixEventCallAnswer* matrix_event_call_answer_new (void);
MatrixCallAnswerType matrix_event_call_answer_get_answer_type (MatrixEventCallAnswer *event);
void matrix_event_call_answer_set_answer_type (MatrixEventCallAnswer *event, MatrixCallAnswerType answer_type);
const gchar* matrix_event_call_answer_get_answer_sdp (MatrixEventCallAnswer *event);
void matrix_event_call_answer_set_answer_sdp (MatrixEventCallAnswer *event, const gchar *answer_sdp);

G_END_DECLS

#endif  /* __MATRIX_GLIB_SDK_EVENT_CALL_ANSWER_H__ */
