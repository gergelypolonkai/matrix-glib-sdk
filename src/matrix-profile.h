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

#ifndef __MATRIX_GLIB_SDK_PROFILE_H__
# define __MATRIX_GLIB_SDK_PROFILE_H__

# include <glib-object.h>

G_BEGIN_DECLS

# define MATRIX_TYPE_PROFILE matrix_profile_get_type()
G_DECLARE_DERIVABLE_TYPE(MatrixProfile, matrix_profile, MATRIX, PROFILE, GObject);

struct _MatrixProfileClass {
    GObjectClass parent_class;

    /* < private > */
    gpointer padding[12];
};

MatrixProfile *matrix_profile_new(void);
MatrixProfile *matrix_profile_construct(GType object_type);
    GTimeSpan matrix_profile_get_age (MatrixProfile *profile);
const gchar *matrix_profile_get_avatar_url(MatrixProfile *profile);
void matrix_profile_set_avatar_url(MatrixProfile *profile, const gchar *avatar_url);
const gchar *matrix_profile_get_display_name(MatrixProfile *profile);
void matrix_profile_set_display_name(MatrixProfile *profile, const gchar *display_name);

G_END_DECLS

#endif  /* __MATRIX_GLIB_SDK_PROFILE_H__ */
