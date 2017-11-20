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

#ifndef __MATRIX_GLIB_SDK_MESSAGE_FILE_H__
# define __MATRIX_GLIB_SDK_MESSAGE_FILE_H__

# include <glib-object.h>
# include "matrix-message-base.h"
# include "matrix-types.h"

G_BEGIN_DECLS

# define MATRIX_MESSAGE_TYPE_FILE matrix_message_file_get_type()
G_DECLARE_DERIVABLE_TYPE(MatrixMessageFile, matrix_message_file, MATRIX_MESSAGE, FILE, MatrixMessageBase);

struct _MatrixMessageFileClass {
    MatrixMessageBaseClass parent_class;
};

MatrixMessageFile* matrix_message_file_new (void);
MatrixMessageFile* matrix_message_file_construct (GType object_type);
const gchar* matrix_message_file_get_filename (MatrixMessageFile* self);
void matrix_message_file_set_filename (MatrixMessageFile* self, const gchar* value);
const gchar* matrix_message_file_get_url (MatrixMessageFile* self);
void matrix_message_file_set_url (MatrixMessageFile* self, const gchar* value);
MatrixFileInfo* matrix_message_file_get_info (MatrixMessageFile* self);
void matrix_message_file_set_info (MatrixMessageFile* self, MatrixFileInfo* value);
const gchar* matrix_message_file_get_thumbnail_url (MatrixMessageFile* self);
void matrix_message_file_set_thumbnail_url (MatrixMessageFile* self, const gchar* value);
MatrixImageInfo* matrix_message_file_get_thumbnail_info (MatrixMessageFile* self);
void matrix_message_file_set_thumbnail_info (MatrixMessageFile* self, MatrixImageInfo* value);

G_END_DECLS

#endif  /* __MATRIX_GLIB_SDK_MESSAGE_FILE_H__ */
