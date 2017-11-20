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

#include "matrix-message-notice.h"

/**
 * SECTION:matrix-message-notice
 * @short_description: message type representing notices
 *
 * This is the default message handler for `m.notice` messages.
 *
 * A notice should be considered similar to a plain text messages except that clients should
 * visually distinguish it in some way.  It is intended to be used by automated clients, such
 * as bots, bridges, and other entities, rather than humans.  Additionally, such automated
 * agents which watch a room for messages and respond to them ought to ignore notices.  This
 * helps to prevent infinite loop situations where two automated clients continuously exchange
 * messages, as each responds to the other.
 */
G_DEFINE_TYPE(MatrixMessageNotice, matrix_message_notice, MATRIX_MESSAGE_TYPE_BASE);

static void
matrix_message_notice_real_from_json(MatrixMessageBase *matrix_message_base, JsonNode *json_data, GError **error)
{
    MATRIX_MESSAGE_BASE_CLASS(matrix_message_notice_parent_class)->from_json(matrix_message_base, json_data, error);
}

static void
matrix_message_notice_real_to_json(MatrixMessageBase *matrix_message_base, JsonNode *json_data, GError **error)
{
    MATRIX_MESSAGE_BASE_CLASS(matrix_message_notice_parent_class)->to_json(matrix_message_base, json_data, error);
}

MatrixMessageNotice *
matrix_message_notice_construct(GType object_type)
{
    return (MatrixMessageNotice *)matrix_message_base_construct(object_type);
}

MatrixMessageNotice *
matrix_message_notice_new(void) {
    return matrix_message_notice_construct(MATRIX_MESSAGE_TYPE_NOTICE);
}

static void
matrix_message_notice_class_init(MatrixMessageNoticeClass *klass)
{
    ((MatrixMessageBaseClass *)klass)->from_json = matrix_message_notice_real_from_json;
    ((MatrixMessageBaseClass *)klass)->to_json = matrix_message_notice_real_to_json;
}

static void
matrix_message_notice_init(MatrixMessageNotice *matrix_message_notice)
{}
