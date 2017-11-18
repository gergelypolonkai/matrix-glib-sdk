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

#include "matrix-message-emote.h"

/**
 * SECTION:matrix-message-emote
 * @short_description: message type to handle emotes
 *
 * This is the default message handler for `m.emote` messages.
 *
 * This message is similar to `m.text` (see #MatrixMessageText) except that the sender is
 * “performing” the action contained in the `body` key, similar to the `/me` command in IRC.
 * When displaying this message, it should be prefixed by the name of the sender, eg. “John
 * Doe thinks”.  This message could also be represented in a different colour to distinguish
 * it from regular `m.text` messages.
 */
G_DEFINE_TYPE(MatrixMessageEmote, matrix_message_emote, MATRIX_MESSAGE_TYPE_BASE);

static void
matrix_message_emote_real_from_json(MatrixMessageBase *matrix_message_base, JsonNode *json_data, GError **error)
{
    MATRIX_MESSAGE_BASE_CLASS(matrix_message_emote_parent_class)->from_json(matrix_message_base, json_data, error);
}

static void
matrix_message_emote_real_to_json(MatrixMessageBase *matrix_message_base, JsonNode *json_data, GError **error)
{
    MATRIX_MESSAGE_BASE_CLASS(matrix_message_emote_parent_class)->to_json(matrix_message_base, json_data, error);
}

MatrixMessageEmote *
matrix_message_emote_new(void) {
    return (MatrixMessageEmote *)matrix_message_base_construct(MATRIX_MESSAGE_TYPE_EMOTE);
}

static void
matrix_message_emote_class_init(MatrixMessageEmoteClass *klass)
{
    ((MatrixMessageBaseClass *)klass)->from_json = matrix_message_emote_real_from_json;
    ((MatrixMessageBaseClass *)klass)->to_json = matrix_message_emote_real_to_json;
}

static void
matrix_message_emote_init(MatrixMessageEmote *matrix_message_emote)
{}
