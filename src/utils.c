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

#include "utils.h"

// GTK-Doc looking comments in this file intentionally do not begin with a double star, as the
// functions here are internal

/*
 * _matrix_g_enum_to_string:
 *
 * @enum_type: a #GEnumType
 * @value: an integer value registered in @enum_type
 * @convert_dashes: if non-NUL, dashes in the value nick will be converted to this char
 *
 * Get the value nick (lower case, dash-separated name) of an enum, like matrix-error-none.  If
 * @convert_dashes is not NUL (`'\0'`), dashes will be converted to that char.
 *
 * Returns: (transfer full) the converted enum name, which must be freed
 */
gchar *
_matrix_g_enum_to_string(GType enum_type, gint value, gboolean convert_dashes)
{
    GEnumClass *enum_class = g_type_class_ref(enum_type);
    GEnumValue *enum_value = g_enum_get_value(enum_class, value);
    gchar *nick = NULL;

    if (value) {
        nick = g_strdup(enum_value->value_nick);

        if (convert_dashes) {
            gchar *a;

            for (a = nick; *a; a++) {
                if (*a == '-') {
                    *a = '_';
                }
            }
        }
    }

    g_type_class_unref(enum_class);

    return nick;
}
