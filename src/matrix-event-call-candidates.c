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

#include "matrix-event-call-candidates.h"
#include "matrix-enumtypes.h"

/**
 * SECTION:matrix-event-call-candidates
 * @short_description: Event to represent call candidates
 *
 */

struct _MatrixCallCandidate {
    gchar *sdp_mid;     /// The SDP media type this candidate is intended for.
    int sdp_line_index; /// The index of the SDP 'm' line this candidate is intended for.
    gchar *candidate;   /// The SDP 'a' line of the candidate.

    guint refcount;
};

/**
 * MatrixCallCandidate:
 *
 * An opaque data structure to represent a call candidate.
 */
G_DEFINE_BOXED_TYPE(MatrixCallCandidate, matrix_call_candidate, (GBoxedCopyFunc)matrix_call_candidate_unref, (GBoxedFreeFunc)matrix_call_candidate_unref);

/**
 * matrix_call_candidate_new:
 *
 * Create a new #MatrixCallCandidate object with a reference count of 1.
 *
 * Returns: (transfer full): a new #MatrixCallCandidate object
 */
MatrixCallCandidate *
matrix_call_candidate_new(void)
{
    MatrixCallCandidate *ret;

    ret = g_new0(MatrixCallCandidate, 1);
    ret->refcount = 1;

    return ret;
}

/**
 * matrix_call_candidate_ref:
 * @candidate: a #MatrixCallCandidate
 *
 * Increment reference count on @candidate.
 *
 * Returns: (transfer full): the same object
 */
MatrixCallCandidate *
matrix_call_candidate_ref(MatrixCallCandidate *matrix_call_candidate)
{
    g_return_val_if_fail(matrix_call_candidate != NULL, NULL);

    matrix_call_candidate->refcount++;

    return matrix_call_candidate;
}

/**
 * matrix_call_candidate_unref:
 * @candidate: a #MatrixCallCandidate
 *
 * Decrement reference count on @candidate.
 *
 * If reference count reaches zero, @candidate gets freed.
 */
void
matrix_call_candidate_unref(MatrixCallCandidate *matrix_call_candidate)
{
    g_return_if_fail(matrix_call_candidate != NULL);

    g_free(matrix_call_candidate->sdp_mid);
    g_free(matrix_call_candidate->candidate);
    g_free(matrix_call_candidate);
}

/**
 * matrix_call_candidate_get_sdp_mid:
 * @candidate: a #MatrixCallCandidate
 *
 * Get the SDP mid of @candidate.
 *
 * Returns: (transfer none) (nullable): the SDP mid
 */
const gchar *
matrix_call_candidate_get_sdp_mid(MatrixCallCandidate *matrix_call_candidate)
{
    g_return_val_if_fail(matrix_call_candidate != NULL, NULL);

    return matrix_call_candidate->sdp_mid;
}

/**
 * matrix_call_candidate_set_sdp_mid:
 * @candidate: a #MatrixCallCandidate
 * @sdp_mid: an SDP mid
 *
 * Set the SDP mid for @candidate.
 */
void
matrix_call_candidate_set_sdp_mid(MatrixCallCandidate *matrix_call_candidate, const gchar *sdp_mid)
{
    g_return_if_fail(matrix_call_candidate);

    g_free(matrix_call_candidate->sdp_mid);
    matrix_call_candidate->sdp_mid = g_strdup(sdp_mid);
}

/**
 * matrix_call_candidate_get_sdp_line_index:
 * @candidate: a #MatrixCallCandidate
 *
 * Get the SDP line of @candidate.
 *
 * The value returned is owned by @candidate and should not be freed.
 *
 * Returns: (transfer none) (nullable): the SDP line
 */
int
matrix_call_candidate_get_sdp_line_index(MatrixCallCandidate *matrix_call_candidate)
{
    g_return_val_if_fail(matrix_call_candidate != NULL, 0);

    return matrix_call_candidate->sdp_line_index;
}

/**
 * matrix_call_candidate_set_sdp_line_index:
 * @candidate: a #MatrixCallCandidate
 * @sdp_line_index: an SDP line index
 *
 * Set the SDP line index of @candidate.
 */
void
matrix_call_candidate_set_sdp_line_index(MatrixCallCandidate *matrix_call_candidate, int sdp_line_index)
{
    g_return_if_fail(matrix_call_candidate != NULL);

    matrix_call_candidate->sdp_line_index = sdp_line_index;
}

/**
 * matrix_call_candidate_get_candidate:
 * @candidate: a #MatrixCallCandidate
 *
 * Get the call candidate from @candidate.
 *
 * The returned value is owned by @candidate and should not be freed.
 *
 * Returns: (transfer none) (nullable): the candidate name
 */
const gchar *
matrix_call_candidate_get_candidate(MatrixCallCandidate *matrix_call_candidate)
{
    g_return_val_if_fail(matrix_call_candidate != NULL, NULL);

    return matrix_call_candidate->candidate;
}

/**
 * matrix_call_candidate_set_candidate:
 * @candidate: a #MatrixCallCandidate
 * @candidate_name: (transfer none) (nullable): the name of the candidate
 *
 * Set the name of the call candidate.
 */
void
matrix_call_candidate_set_candidate(MatrixCallCandidate *matrix_call_candidate, const gchar *candidate)
{
    g_return_if_fail(matrix_call_candidate != NULL);

    g_free(matrix_call_candidate->candidate);
    matrix_call_candidate->candidate = g_strdup(candidate);
}

enum {
    PROP_0,
    PROP_CANDIDATES,
    NUM_PROPS
};

static GParamSpec *matrix_event_call_candidates_properties[NUM_PROPS];

typedef struct {
    MatrixCallCandidate** _candidates;
    gint _candidates_len;
} MatrixEventCallCandidatesPrivate;

/**
 * MatrixEventCallCandidates:
 *
 * This event is sent by callers after sending an invite and by the callee after answering.
 * Its purpose is to give the other party additional ICE candidates to try using to communicate.
 */
G_DEFINE_TYPE_WITH_PRIVATE(MatrixEventCallCandidates, matrix_event_call_candidates, MATRIX_EVENT_TYPE_CALL);

static void
matrix_event_call_candidates_real_from_json(MatrixEventBase *matrix_event_base, JsonNode *json_data, GError **error)
{
    MatrixEventCallCandidatesPrivate *priv;
    JsonObject *root;
    JsonObject *content_root;
    JsonNode *content_node;
    JsonNode *node;

    g_return_if_fail(json_data != NULL);

    priv = matrix_event_call_candidates_get_instance_private(MATRIX_EVENT_CALL_CANDIDATES(matrix_event_base));
    root = json_node_get_object(json_data);
    content_node = json_object_get_member(root, "content");
    content_root = json_node_get_object(content_node);

    if ((node = json_object_get_member(content_root, "candidates")) != NULL) {
        JsonArray *candidates = json_node_get_array(node);
        gint len = json_array_get_length(candidates);

        priv->_candidates = g_new(MatrixCallCandidate *, len);

        for (gint i = 0; i < len; i++) {
            JsonNode *cand_node = json_array_get_element(candidates, i);
            JsonObject *cand_root = json_node_get_object(cand_node);
            priv->_candidates[i] = matrix_call_candidate_new();

            if ((node = json_object_get_member(cand_root, "sdpMid")) != NULL) {
                priv->_candidates[i]->sdp_mid = g_strdup(json_node_get_string(node));
            } else {
                g_warning("sdpMid is missing from a candidate of a m.call.candidates event");
            }

            if ((node = json_object_get_member(cand_root, "sdpMLineIndex")) != NULL) {
                priv->_candidates[i]->sdp_line_index = json_node_get_int(node);
            } else {
                g_warning("sdpMLineIndex is missing from a candidate of a m.call.candidates event");
            }

            if ((node = json_object_get_member(cand_root, "candidate")) != NULL) {
                priv->_candidates[i]->candidate = g_strdup(json_node_get_string(node));
            } else {
                g_warning("candidate is missing from a candidate of a m.call.candidates event");
            }
        };
    } else {
        g_warning("content.candidates is missing from a m.call.candidates event");
    }

    MATRIX_EVENT_BASE_CLASS(matrix_event_call_candidates_parent_class)->from_json(matrix_event_base, json_data, error);
}

static void
matrix_event_call_candidates_real_to_json(MatrixEventBase *matrix_event_base, JsonNode *json_data, GError **error)
{
    MatrixEventCallCandidatesPrivate *priv;
    JsonObject *root;
    JsonObject *content_root;
    JsonArray *cands;
    JsonNode *content_node;
    JsonNode *cands_node;

    g_return_if_fail(json_data != NULL);

    priv = matrix_event_call_candidates_get_instance_private(MATRIX_EVENT_CALL_CANDIDATES(matrix_event_base));
    root = json_node_get_object(json_data);
    content_node = json_object_get_member(root, "content");
    content_root = json_node_get_object(content_node);

    if (priv->_candidates_len < 1) {
        g_set_error(error, MATRIX_ERROR, MATRIX_ERROR_INCOMPLETE,
                    "Won't generate a m.call.candidates event without candidates");

        return;
    }

    cands = json_array_new();
    cands_node = json_node_new(JSON_NODE_ARRAY);
    json_node_set_array(cands_node, cands);
    json_object_set_member(content_root, "candidates", cands_node);

    for (gint i = 0; i < priv->_candidates_len; i++) {
        MatrixCallCandidate *entry = priv->_candidates[i];
        JsonObject *cand_root;
        JsonNode *cand_node;

        if (entry->sdp_mid == NULL) {
            g_set_error(error, MATRIX_ERROR, MATRIX_ERROR_INCOMPLETE,
                        "Won't generate a m.call.candidates event with a missing sdpMid for candidates");

            return;
        }

        if (entry->candidate == NULL) {
            g_set_error(error, MATRIX_ERROR, MATRIX_ERROR_INCOMPLETE,
                        "Won't generate a m.call.candidates event with a missing candidate for candidates");

            return;
        }

        cand_root = json_object_new();
        cand_node = json_node_new(JSON_NODE_OBJECT);
        json_node_set_object(cand_node, cand_root);

        json_object_set_string_member(cand_root, "sdpMid", entry->sdp_mid);
        json_object_set_int_member(cand_root, "sdpMLineIndex", entry->sdp_line_index);
        json_object_set_string_member(cand_root, "candidate", entry->candidate);

        json_array_add_element(cands, cand_node);
    }

    if (json_array_get_length(cands) < 1) {
        g_set_error(error, MATRIX_ERROR, MATRIX_ERROR_INCOMPLETE,
                    "Won't generate a m.call.candidates event with empty candidates list");

        return;
    }

    MATRIX_EVENT_BASE_CLASS(matrix_event_call_candidates_parent_class)->to_json(matrix_event_base, json_data, error);
}

MatrixEventCallCandidates *
matrix_event_call_candidates_construct(GType object_type)
{
    return (MatrixEventCallCandidates *)matrix_event_call_construct(object_type);
}

/**
 * matrix_event_call_candidates_new:
 *
 * Create a new #MatrixEventCallCandidates object.
 *
 * Returns: (transfer full): a new #MatrixEventCallCandidates object
 */
MatrixEventCallCandidates *
matrix_event_call_candidates_new(void)
{
    return matrix_event_call_candidates_construct(MATRIX_EVENT_TYPE_CALL_CANDIDATES);
}

/**
 * matrix_event_call_candidates_get_candidates:
 * @event: a #MatrixEventCallCandidates:
 * @n_candidates: (nullable): placeholder for the length of the list, or %NULL to ignore
 *
 * Get the list of the candidates from @event.
 *
 * The returned value is owned by @event, and should not be freed.
 *
 * Returns: (transfer none) (nullable): the list of candidates
 */
MatrixCallCandidate **
matrix_event_call_candidates_get_candidates(MatrixEventCallCandidates *matrix_event_call_candidates, int *n_candidates)
{
    MatrixEventCallCandidatesPrivate *priv;

    g_return_val_if_fail(matrix_event_call_candidates != NULL, NULL);

    priv = matrix_event_call_candidates_get_instance_private(matrix_event_call_candidates);

    if (n_candidates != NULL) {
        *n_candidates = priv->_candidates_len;
    }

    return priv->_candidates;
}

/**
 * matrix_event_call_candidates_set_candidates:
 * @event: a #MatrixEventCallCandidates
 * @candidates: a list of #MatrixCallCandidate objects
 * @n_candidates: the number of elements in @candidates
 *
 * Set the list of call candidates in @event.
 */
void
matrix_event_call_candidates_set_candidates(MatrixEventCallCandidates *matrix_event_call_candidates, MatrixCallCandidate **candidates, int n_candidates)
{
    MatrixEventCallCandidatesPrivate *priv;

    g_return_if_fail(matrix_event_call_candidates != NULL);

    priv = matrix_event_call_candidates_get_instance_private(matrix_event_call_candidates);

    for (gint i = 0; i < priv->_candidates_len; i++) {
        matrix_call_candidate_unref(priv->_candidates[i]);
    }

    g_free(priv->_candidates);

    priv->_candidates = (MatrixCallCandidate **)g_new(MatrixCallCandidate, n_candidates);
    priv->_candidates_len = n_candidates;

    for (gint i = 0; i < n_candidates; i++) {
        priv->_candidates[i] = matrix_call_candidate_ref(candidates[i]);
    }
}

static void
matrix_event_call_candidates_finalize(GObject *gobject)
{
    MatrixEventCallCandidatesPrivate *priv = matrix_event_call_candidates_get_instance_private(MATRIX_EVENT_CALL_CANDIDATES(gobject));

    for (gint i = 0; i < priv->_candidates_len; i++) {
        matrix_call_candidate_unref(priv->_candidates[i]);
    }

    g_free(priv->_candidates);

    G_OBJECT_CLASS(matrix_event_call_candidates_parent_class)->finalize(gobject);
}

static void
matrix_event_call_candidates_get_property(GObject *gobject, guint property_id, GValue *value, GParamSpec *pspec)
{
    MatrixEventCallCandidates *matrix_event_call_candidates = MATRIX_EVENT_CALL_CANDIDATES(gobject);

    switch (property_id) {
        case PROP_CANDIDATES:
            g_value_set_boxed(value, matrix_event_call_candidates_get_candidates(matrix_event_call_candidates, NULL));

            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, property_id, pspec);
            break;
    }
}

static void
matrix_event_call_candidates_set_property(GObject *gobject, guint property_id, const GValue *value, GParamSpec *pspec)
{
    MatrixEventCallCandidates *matrix_event_call_candidates = MATRIX_EVENT_CALL_CANDIDATES(gobject);

    switch (property_id) {
        case PROP_CANDIDATES:
        {
            gpointer boxed = g_value_get_boxed(value);

            matrix_event_call_candidates_set_candidates(matrix_event_call_candidates, g_value_get_boxed(value), (boxed == NULL) ? 0 : g_strv_length(boxed));
        }
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, property_id, pspec);

            break;
    }
}

static void
matrix_event_call_candidates_class_init(MatrixEventCallCandidatesClass *klass)
{
    ((MatrixEventBaseClass *)klass)->from_json = matrix_event_call_candidates_real_from_json;
    ((MatrixEventBaseClass *)klass)->to_json = matrix_event_call_candidates_real_to_json;
    G_OBJECT_CLASS(klass)->get_property = matrix_event_call_candidates_get_property;
    G_OBJECT_CLASS(klass)->set_property = matrix_event_call_candidates_set_property;
    G_OBJECT_CLASS(klass)->finalize = matrix_event_call_candidates_finalize;

    /**
     * MatrixEventCallCandidates:candidates:
     *
     * The list of call candidates.
     */
    matrix_event_call_candidates_properties[PROP_CANDIDATES] = g_param_spec_boxed(
            "candidates", "candidates", "candidates",
            MATRIX_TYPE_CALL_CANDIDATE,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_CANDIDATES, matrix_event_call_candidates_properties[PROP_CANDIDATES]);
}

static void
matrix_event_call_candidates_init(MatrixEventCallCandidates *matrix_event_call_candidates)
{
    MatrixEventCallCandidatesPrivate *priv = matrix_event_call_candidates_get_instance_private(matrix_event_call_candidates);

    priv->_candidates = NULL;
}
