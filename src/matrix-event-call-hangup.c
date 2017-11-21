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

#include "matrix-event-call-hangup.h"

/**
 * SECTION:matrix-event-call-hangup
 * @short_description: event to signal that a calling party has hung up the line
 *
 * This event is sent by either party to signal their termination of the call.  This can be
 * sent either once the call has has been established or before to abort the call.
 */

/**
 * MatrixEventCallHangup:
 *
 */
G_DEFINE_TYPE(MatrixEventCallHangup, matrix_event_call_hangup, MATRIX_EVENT_TYPE_CALL);

static void
matrix_event_call_hangup_real_from_json(MatrixEventBase *matrix_event_base, JsonNode *json_data, GError **error)
{
    g_return_if_fail(json_data != NULL);

    MATRIX_EVENT_BASE_CLASS(matrix_event_call_hangup_parent_class)->from_json(matrix_event_base, json_data, error);
}

static void
matrix_event_call_hangup_real_to_json(MatrixEventBase *matrix_event_base, JsonNode *json_data, GError **error)
{
    g_return_if_fail(json_data != NULL);

    MATRIX_EVENT_BASE_CLASS(matrix_event_call_hangup_parent_class)->to_json(matrix_event_base, json_data, error);
}

MatrixEventCallHangup *
matrix_event_call_hangup_construct(GType object_type)
{
    return (MatrixEventCallHangup *)matrix_event_call_construct(object_type);
}

/**
 * matrix_event_call_hangup_new:
 *
 * Create a new #MatrixEventCallHangup object.
 *
 * Returns: (transfer full): a new #MatrixEventCallHangup object
 */
MatrixEventCallHangup *
matrix_event_call_hangup_new(void)
{
    return matrix_event_call_hangup_construct(MATRIX_EVENT_TYPE_CALL_HANGUP);
}

static void
matrix_event_call_hangup_class_init(MatrixEventCallHangupClass *klass)
{
    ((MatrixEventBaseClass *)klass)->from_json = matrix_event_call_hangup_real_from_json;
    ((MatrixEventBaseClass *)klass)->to_json = matrix_event_call_hangup_real_to_json;
}

static void
matrix_event_call_hangup_init(MatrixEventCallHangup *matrix_event_call_hangup)
{}
