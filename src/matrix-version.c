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

#include "matrix-version.h"

/**
 * SECTION:matrix-version
 * @short_description: Matrix.org GLib SDK version information
 * @title: Version information
 * @stability: Stable
 * @include: matrix-client/matrix-client.h
 *
 * Version information for the Matrix.org GLib SDK.
 */

/**
 * MATRIX_GLIB_MAJOR_VERSION:
 *
 * The major version number of the Matrix.org GLib SDK.
 */

/**
 * MATRIX_GLIB_MINOR_VERSION:
 *
 * The minor version number of the Matrix.org GLib SDK.
 */

/**
 * MATRIX_GLIB_MICRO_VERSION:
 *
 * The micro (patch) version number of the Matrix.org GLib SDK.
 */

/**
 * MATRIX_GLIB_CHECK_VERSION:
 * @major: the major version to check for
 * @minor: the minor version to check for
 * @micro: the micro version to check for
 *
 * Checks the version number of the Matrix.org GLib SDK that is being
 * compiled against.
 *
 * Returns: TRUE if the required version is satisfied; FALSE otherwise.
 */

/**
 * matrix_glib_check_version:
 * @required_major: the required major version
 * @required_minor: the required minor version
 * @required_micro: the required micro version
 *
 * Check that the Matrix.org GLib SDK in use is compatible with the
 * given version.
 *
 * Returns: TRUE if the required version is satisfied; FALSE otherwise.
 */

gboolean
matrix_glib_check_version(guint required_major,
                          guint required_minor,
                          guint required_micro)
{
    return MATRIX_GLIB_CHECK_VERSION(required_major, required_minor, required_micro);
}
