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

#ifndef __MATRIX_GLIB_SDK_MESSAGE_LOCATION_H__
# define __MATRIX_GLIB_SDK_MESSAGE_LOCATION_H__

# include <glib-object.h>
# include "matrix-message-base.h"
# include "matrix-compacts.h"

G_BEGIN_DECLS

# define MATRIX_MESSAGE_TYPE_LOCATION matrix_message_location_get_type()
G_DECLARE_DERIVABLE_TYPE(MatrixMessageLocation, matrix_message_location, MATRIX_MESSAGE, LOCATION, MatrixMessageBase);

struct _MatrixMessageLocationClass {
    MatrixMessageBaseClass parent_class;
};

MatrixMessageLocation* matrix_message_location_new (void);
MatrixMessageLocation* matrix_message_location_construct (GType object_type);
const gchar* matrix_message_location_get_geo_uri (MatrixMessageLocation* self);
void matrix_message_location_set_geo_uri (MatrixMessageLocation* self, const gchar* value);
const gchar* matrix_message_location_get_thumbnail_url (MatrixMessageLocation* self);
void matrix_message_location_set_thumbnail_url (MatrixMessageLocation* self, const gchar* value);
MatrixImageInfo* matrix_message_location_get_thumbnail_info (MatrixMessageLocation* self);
void matrix_message_location_set_thumbnail_info (MatrixMessageLocation* self, MatrixImageInfo* value);


G_END_DECLS


#endif  /* __MATRIX_GLIB_SDK_MESSAGE_LOCATION_H__ */
