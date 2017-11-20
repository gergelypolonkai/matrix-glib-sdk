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

#ifndef __MATRIX_GLIB_SDK_MESSAGE_IMAGE_H__
# define __MATRIX_GLIB_SDK_MESSAGE_IMAGE_H__

# include <glib-object.h>
# include "matrix-message-base.h"
# include "matrix-types.h"

G_BEGIN_DECLS

# define MATRIX_MESSAGE_TYPE_IMAGE matrix_message_image_get_type()
G_DECLARE_DERIVABLE_TYPE(MatrixMessageImage, matrix_message_image, MATRIX_MESSAGE, IMAGE, MatrixMessageBase);

struct _MatrixMessageImageClass {
    MatrixMessageBaseClass parent_class;
};

MatrixMessageImage* matrix_message_image_new (void);
MatrixMessageImage* matrix_message_image_construct (GType object_type);
const gchar* matrix_message_image_get_url (MatrixMessageImage* self);
void matrix_message_image_set_url (MatrixMessageImage* self, const gchar* value);
const gchar* matrix_message_image_get_thumbnail_url (MatrixMessageImage* self);
void matrix_message_image_set_thumbnail_url (MatrixMessageImage* self, const gchar* value);
MatrixImageInfo* matrix_message_image_get_info (MatrixMessageImage* self);
void matrix_message_image_set_info (MatrixMessageImage* self, MatrixImageInfo* value);
MatrixImageInfo* matrix_message_image_get_thumbnail_info (MatrixMessageImage* self);
void matrix_message_image_set_thumbnail_info (MatrixMessageImage* self, MatrixImageInfo* value);

G_END_DECLS

#endif  /* __MATRIX_GLIB_SDK_MESSAGE_IMAGE_H__ */
