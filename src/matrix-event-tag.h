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

#ifndef __MATRIX_GLIB_SDK_EVENT_TAG_H__
# define __MATRIX_GLIB_SDK_EVENT_TAG_H__

# include <glib-object.h>
# include "matrix-event-base.h"

G_BEGIN_DECLS

# define MATRIX_EVENT_TYPE_TAG matrix_event_tag_get_type()
G_DECLARE_DERIVABLE_TYPE(MatrixEventTag, matrix_event_tag, MATRIX_EVENT, TAG, MatrixEventBase);

struct _MatrixEventTagClass {
    MatrixEventBaseClass parent_class;
};

MatrixEventTag* matrix_event_tag_new(void);
MatrixEventTag* matrix_event_tag_construct(GType object_type);
GHashTable *matrix_event_tag_get_tags(MatrixEventTag *event);
void matrix_event_tag_set_tags(MatrixEventTag *event, GHashTable *tags);

G_END_DECLS

#endif  /* __MATRIX_GLIB_SDK_EVENT_TAG_H__ */
