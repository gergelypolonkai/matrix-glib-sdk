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

#include "matrix-message-text.h"

/**
 * SECTION:matrix-message-text
 * @short_description: message handler for plain text messages
 *
 * This is the default message handler for `m.text` messages.
 */
G_DEFINE_TYPE(MatrixMessageText, matrix_message_text, MATRIX_MESSAGE_TYPE_BASE);

static void
matrix_message_text_real_from_json(MatrixMessageBase *matrix_message_base, JsonNode *json_data, GError **error)
{
    GError *inner_error = NULL;

    g_return_if_fail(json_data != NULL);

    MATRIX_MESSAGE_BASE_CLASS(matrix_message_text_parent_class)->from_json(matrix_message_base, json_data, &inner_error);

    if (inner_error != NULL) {
        g_propagate_error(error, inner_error);
    }
}

static void
matrix_message_text_real_to_json(MatrixMessageBase *matrix_message_base, JsonNode *json_data, GError **error)
{
    GError *inner_error = NULL;

    g_return_if_fail(json_data != NULL);

    MATRIX_MESSAGE_BASE_CLASS(matrix_message_text_parent_class)->to_json(matrix_message_base, json_data, &inner_error);

    if (inner_error != NULL) {
        g_propagate_error(error, inner_error);
    }
}

MatrixMessageText *
matrix_message_text_new(void) {
    return (MatrixMessageText *)matrix_message_base_construct(MATRIX_MESSAGE_TYPE_TEXT);
}

static void
matrix_message_text_class_init(MatrixMessageTextClass *klass)
{
    ((MatrixMessageBaseClass *)klass)->from_json = matrix_message_text_real_from_json;
    ((MatrixMessageBaseClass *)klass)->to_json = matrix_message_text_real_to_json;
}

static void matrix_message_text_init(MatrixMessageText *matrix_message_text)
{}
