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

#ifndef __MATRIX_GLIB_SDK_UTILS_H__
# define __MATRIX_GLIB_SDK_UTILS_H__

# include <glib-object.h>

G_BEGIN_DECLS

gchar *_matrix_g_enum_to_string(GType enum_type, gint value, gboolean convert_dashes);

G_END_DECLS

#endif  /* __MATRIX_GLIB_SDK_UTILS_H__ */
