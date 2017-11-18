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

#ifndef __MATRIX_GLIB_SDK_MESSAGE_EMOTE_H___
# define __MATRIX_GLIB_SDK_MESSAGE_EMOTE_H___

# include <glib-object.h>
# include "matrix-message-base.h"

G_BEGIN_DECLS

# define MATRIX_MESSAGE_TYPE_EMOTE matrix_message_emote_get_type()
G_DECLARE_DERIVABLE_TYPE(MatrixMessageEmote, matrix_message_emote, MATRIX_MESSAGE, EMOTE, MatrixMessageBase);

struct _MatrixMessageEmoteClass {
    MatrixMessageBaseClass parent_class;
};

MatrixMessageEmote *matrix_message_emote_new(void);
MatrixMessageEmote *matrix_message_emote_construct(GType object_type);

G_END_DECLS


#endif  /* __MATRIX_GLIB_SDK_MESSAGE_EMOTE_H___ */
