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

#include "matrix-event-call-answer.h"
#include "utils.h"
#include "config.h"

/**
 * SECTION:matrix-event-call-answer
 * @short_description: event sent when a callee wishes to answer the call
 *
 * This event is sent by the callee when they wish to answer the call.
 */

enum  {
    PROP_0,
    PROP_ANSWER_TYPE,
    PROP_ANSWER_SDP,
    NUM_PROPERTIES
};

static GParamSpec *matrix_event_call_answer_properties[NUM_PROPERTIES];

typedef struct {
    MatrixCallAnswerType _answer_type;
    gchar* _answer_sdp;
} MatrixEventCallAnswerPrivate;

/**
 * MatrixEventCallAnswer:
 *
 * Object structure.
 */
G_DEFINE_TYPE_WITH_PRIVATE(MatrixEventCallAnswer, matrix_event_call_answer, MATRIX_EVENT_TYPE_CALL);

static void
matrix_event_call_answer_real_from_json(MatrixEventBase *matrix_event_base, JsonNode *json_data, GError **error)
{
    MatrixEventCallAnswerPrivate *priv;
    JsonObject *root;
    JsonObject *content_root;
    JsonNode *content_node;
    JsonNode *node;

    g_return_if_fail(json_data != NULL);

    priv = matrix_event_call_answer_get_instance_private(MATRIX_EVENT_CALL_ANSWER(matrix_event_base));
    root = json_node_get_object(json_data);
    content_node = json_object_get_member(root, "content");
    content_root = json_node_get_object(content_node);

    if ((node = json_object_get_member(content_root, "answer")) != NULL) {
        JsonObject *answer_root = json_node_get_object(node);

        if ((node = json_object_get_member(answer_root, "type")) != NULL) {
            GError *inner_error = NULL;
            MatrixCallAnswerType answer_type = _matrix_g_enum_nick_to_value(MATRIX_TYPE_CALL_ANSWER_TYPE, json_node_get_string(node), &inner_error);

            if (inner_error != NULL) {
                priv->_answer_type = MATRIX_CALL_ANSWER_TYPE_UNKNOWN;

#if DEBUG
                g_warning("Unknown value %s for content.answer.type in a m.call.answer event", json_node_get_string(node));
#endif
            } else {
                priv->_answer_type = answer_type;
            }
        } else {
            g_warning("content.answer.type is missing from a m.call.answer event");
        }

        if ((node = json_object_get_member(answer_root, "sdp")) != NULL) {
            g_free(priv->_answer_sdp);
            priv->_answer_sdp = g_strdup(json_node_get_string(node));
        } else {
            g_warning("content.answer.sdp is missing from a m.call.answer event");
        }
    }

    MATRIX_EVENT_BASE_CLASS(matrix_event_call_answer_parent_class)->from_json(matrix_event_base, json_data, error);
}

static void
matrix_event_call_answer_real_to_json(MatrixEventBase *matrix_event_base, JsonNode *json_data, GError **error)
{
    MatrixEventCallAnswerPrivate *priv;
    JsonObject *root;
    JsonObject *content_root;
    JsonObject *answer_root;
    JsonNode *content_node;
    JsonNode *answer_node;
    gchar *answer_type;

    g_return_if_fail(json_data != NULL);

    priv = matrix_event_call_answer_get_instance_private(MATRIX_EVENT_CALL_ANSWER(matrix_event_base));

    if (priv->_answer_type == MATRIX_CALL_ANSWER_TYPE_UNKNOWN) {
        g_set_error(error, MATRIX_ERROR, MATRIX_ERROR_INCOMPLETE,
                    "Won't generate a m.call.answer event without a valid answer.type");

        return;
    }

    if (priv->_answer_sdp == NULL) {
        g_set_error(error, MATRIX_ERROR, MATRIX_ERROR_INCOMPLETE,
                    "Won't generate a m.call.answer event without answer.sdp");

        return;
    }

    root = json_node_get_object(json_data);
    content_node = json_object_get_member(root, "content");
    content_root = json_node_get_object(content_node);

    answer_root = json_object_new();
    answer_node = json_node_new(JSON_NODE_OBJECT);
    json_node_set_object(answer_node, answer_root);

    answer_type = _matrix_g_enum_to_string(MATRIX_TYPE_CALL_ANSWER_TYPE, priv->_answer_type, '_');
    json_object_set_string_member(answer_root, "type", answer_type);
    g_free(answer_type);

    json_object_set_string_member(answer_root, "sdp", priv->_answer_sdp);

    json_object_set_member(content_root, "answer", answer_node);

    MATRIX_EVENT_BASE_CLASS(matrix_event_call_answer_parent_class)->to_json(matrix_event_base, json_data, error);
}

MatrixEventCallAnswer *
matrix_event_call_answer_construct(GType object_type)
{
    return (MatrixEventCallAnswer *)matrix_event_call_construct(object_type);
}

/**
 * matrix_event_call_answer_new:
 *
 * Create a new #MatrixEventCallAnswer object.
 *
 * Returns: (transfer full): a new #MatrixEventCallAnswer object
 */
MatrixEventCallAnswer *
matrix_event_call_answer_new(void)
{
    return matrix_event_call_answer_construct(MATRIX_EVENT_TYPE_CALL_ANSWER);
}

/**
 * matrix_event_call_answer_get_answer_type:
 * @event: a #MatrixEventCallAnswer
 *
 * Get the answer type from @event.
 *
 * Returns: the answer type.
 */
MatrixCallAnswerType
matrix_event_call_answer_get_answer_type(MatrixEventCallAnswer *matrix_event_call_answer)
{
    MatrixEventCallAnswerPrivate *priv;

    g_return_val_if_fail(matrix_event_call_answer != NULL, 0);

    priv = matrix_event_call_answer_get_instance_private(matrix_event_call_answer);

    return priv->_answer_type;
}

/**
 * matrix_event_call_answer_set_answer_type:
 * @event: a #MatrixEventCallAnswer
 * @answer_type: the answer type to set
 *
 * Set the answer type in @event.
 */
void
matrix_event_call_answer_set_answer_type(MatrixEventCallAnswer *matrix_event_call_answer, MatrixCallAnswerType answer_type)
{
    MatrixEventCallAnswerPrivate *priv;

    g_return_if_fail(matrix_event_call_answer != NULL);

    priv = matrix_event_call_answer_get_instance_private(matrix_event_call_answer);

    if (priv->_answer_type != answer_type) {
        priv->_answer_type = answer_type;

        g_object_notify_by_pspec((GObject *)matrix_event_call_answer, matrix_event_call_answer_properties[PROP_ANSWER_TYPE]);
    }
}

/**
 * matrix_event_call_answer_get_answer_sdp:
 * @event: a #MatrixEventCallAnswer
 *
 * Get the answer SDP of @event.
 *
 * The returned value is owned by @event and should not be freed.
 *
 * Returns: (transfer none) (nullable): the answer SDP
 */
const gchar *
matrix_event_call_answer_get_answer_sdp(MatrixEventCallAnswer *matrix_event_call_answer)
{
    MatrixEventCallAnswerPrivate *priv;

    g_return_val_if_fail(matrix_event_call_answer != NULL, NULL);

    priv = matrix_event_call_answer_get_instance_private(matrix_event_call_answer);

    return priv->_answer_sdp;
}

/**
 * matrix_event_call_answer_set_answer_sdp:
 * @event: a #MatrixEventCallAnswer
 * @answer_sdp: (transfer none): an answew SDP
 *
 * Set the answer SDP in @event.
 */
void
matrix_event_call_answer_set_answer_sdp(MatrixEventCallAnswer *matrix_event_call_answer, const gchar *answer_sdp)
{
    MatrixEventCallAnswerPrivate *priv;

    g_return_if_fail(matrix_event_call_answer != NULL);

    priv = matrix_event_call_answer_get_instance_private(matrix_event_call_answer);

    if (g_strcmp0(answer_sdp, priv->_answer_sdp) != 0) {
        g_free(priv->_answer_sdp);
        priv->_answer_sdp = g_strdup(answer_sdp);

        g_object_notify_by_pspec((GObject *)matrix_event_call_answer, matrix_event_call_answer_properties[PROP_ANSWER_SDP]);
    }
}

static void
matrix_event_call_answer_finalize(GObject *gobject) {
    MatrixEventCallAnswerPrivate *priv = matrix_event_call_answer_get_instance_private(MATRIX_EVENT_CALL_ANSWER(gobject));

    g_free(priv->_answer_sdp);

    G_OBJECT_CLASS(matrix_event_call_answer_parent_class)->finalize(gobject);
}

static void
matrix_event_call_answer_get_property(GObject *gobject, guint property_id, GValue *value, GParamSpec *pspec)
{
    MatrixEventCallAnswer *matrix_event_call_answer = MATRIX_EVENT_CALL_ANSWER(gobject);

    switch (property_id) {
        case PROP_ANSWER_TYPE:
            g_value_set_enum(value, matrix_event_call_answer_get_answer_type(matrix_event_call_answer));

            break;
        case PROP_ANSWER_SDP:
            g_value_set_string(value, matrix_event_call_answer_get_answer_sdp(matrix_event_call_answer));

            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, property_id, pspec);

            break;
    }
}

static void
matrix_event_call_answer_set_property(GObject *gobject, guint property_id, const GValue *value, GParamSpec *pspec)
{
    MatrixEventCallAnswer *matrix_event_call_answer = MATRIX_EVENT_CALL_ANSWER(gobject);

    switch (property_id) {
        case PROP_ANSWER_TYPE:
            matrix_event_call_answer_set_answer_type(matrix_event_call_answer, g_value_get_enum(value));

            break;
        case PROP_ANSWER_SDP:
            matrix_event_call_answer_set_answer_sdp(matrix_event_call_answer, g_value_get_string(value));

            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, property_id, pspec);

            break;
    }
}

static void
matrix_event_call_answer_class_init(MatrixEventCallAnswerClass *klass)
{
    ((MatrixEventBaseClass *)klass)->from_json = matrix_event_call_answer_real_from_json;
    ((MatrixEventBaseClass *)klass)->to_json = matrix_event_call_answer_real_to_json;
    G_OBJECT_CLASS(klass)->get_property = matrix_event_call_answer_get_property;
    G_OBJECT_CLASS(klass)->set_property = matrix_event_call_answer_set_property;
    G_OBJECT_CLASS(klass)->finalize = matrix_event_call_answer_finalize;

    /**
     * MatrixEventCallAnswer:answer-type:
     *
     * The type of session description.
     */
    matrix_event_call_answer_properties[PROP_ANSWER_TYPE] = g_param_spec_enum(
            "answer-type", "answer-type", "answer-type",
            MATRIX_TYPE_CALL_ANSWER_TYPE, MATRIX_CALL_ANSWER_TYPE_UNKNOWN,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_ANSWER_TYPE, matrix_event_call_answer_properties[PROP_ANSWER_TYPE]);

    /**
     * MatrixEventCallAnswer:answer-sdp:
     * The SDP text of the session description.
     */
    matrix_event_call_answer_properties[PROP_ANSWER_SDP] = g_param_spec_string(
            "answer-sdp", "answer-sdp", "answer-sdp",
            NULL,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_ANSWER_SDP, matrix_event_call_answer_properties[PROP_ANSWER_SDP]);
}

static void
matrix_event_call_answer_init(MatrixEventCallAnswer *matrix_event_call_answer)
{
    MatrixEventCallAnswerPrivate *priv = matrix_event_call_answer_get_instance_private(matrix_event_call_answer);

    priv->_answer_type = MATRIX_CALL_ANSWER_TYPE_UNKNOWN;
    priv->_answer_sdp = NULL;
}
