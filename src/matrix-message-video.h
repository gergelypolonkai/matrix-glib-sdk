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

#ifndef __MATRIX_GLIB_SDK_MESSAGE_VIDEO_H__
# define __MATRIX_GLIB_SDK_MESSAGE_VIDEO_H__

# include <glib-object.h>
# include "matrix-message-base.h"
# include "matrix-types.h"

G_BEGIN_DECLS

# define MATRIX_MESSAGE_TYPE_VIDEO (matrix_message_video_get_type ())
G_DECLARE_DERIVABLE_TYPE(MatrixMessageVideo, matrix_message_video, MATRIX_MESSAGE, VIDEO, MatrixMessageBase)

struct _MatrixMessageVideoClass {
    MatrixMessageBaseClass parent_class;
};

MatrixMessageVideo* matrix_message_video_new (void);
const gchar* matrix_message_video_get_url (MatrixMessageVideo* self);
void matrix_message_video_set_url (MatrixMessageVideo* self, const gchar* value);
MatrixVideoInfo* matrix_message_video_get_info (MatrixMessageVideo* self);
void matrix_message_video_set_info (MatrixMessageVideo* self, MatrixVideoInfo* value);

G_END_DECLS

#endif  /* __MATRIX_GLIB_SDK_MESSAGE_VIDEO_H__ */
