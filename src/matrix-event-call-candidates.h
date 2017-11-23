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

#ifndef __MATRIX_GLIB_SDK_EVENT_CALL_CANDIDATES_H__
# define __MATRIX_GLIB_SDK_EVENT_CALL_CANDIDATES_H__

# include <glib-object.h>
# include "matrix-event-call-base.h"

G_BEGIN_DECLS

# define MATRIX_TYPE_CALL_CANDIDATE matrix_call_candidate_get_type()
typedef struct _MatrixCallCandidate MatrixCallCandidate;

GType matrix_call_candidate_get_type(void) G_GNUC_CONST;
MatrixCallCandidate *matrix_call_candidate_new(void);
MatrixCallCandidate *matrix_call_candidate_ref(MatrixCallCandidate *candidate);
void matrix_call_candidate_unref(MatrixCallCandidate *candidate);
const gchar *matrix_call_candidate_get_sdp_mid(MatrixCallCandidate *candidate);
void matrix_call_candidate_set_sdp_mid(MatrixCallCandidate *candidate, const gchar *sdp_mid);
int matrix_call_candidate_get_sdp_line_index(MatrixCallCandidate *candidate);
void matrix_call_candidate_set_sdp_line_index(MatrixCallCandidate *candidate, int sdp_line_index);
const gchar *matrix_call_candidate_get_candidate(MatrixCallCandidate *candidate);
void matrix_call_candidate_set_candidate(MatrixCallCandidate *candidate, const gchar *candidate_name);

#define MATRIX_EVENT_TYPE_CALL_CANDIDATES (matrix_event_call_candidates_get_type ())
G_DECLARE_DERIVABLE_TYPE(MatrixEventCallCandidates, matrix_event_call_candidates, MATRIX_EVENT, CALL_CANDIDATES, MatrixEventCall);

struct _MatrixEventCallCandidatesClass {
    MatrixEventCallClass parent_class;
};

MatrixEventCallCandidates* matrix_event_call_candidates_new (void);
MatrixEventCallCandidates* matrix_event_call_candidates_construct (GType object_type);
MatrixCallCandidate* matrix_event_call_candidates_candidate_dup (const MatrixCallCandidate *event);
MatrixCallCandidate** matrix_event_call_candidates_get_candidates (MatrixEventCallCandidates *event, int *n_candidates);
void matrix_event_call_candidates_set_candidates (MatrixEventCallCandidates *event, MatrixCallCandidate **candidates, int n_candidates);

G_END_DECLS

#endif  /* __MATRIX_GLIB_SDK_EVENT_CALL_CANDIDATES_H__ */
