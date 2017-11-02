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

#ifndef __MATRIX_TYPE_H__
# define __MATRIX_TYPE_H__

# include <glib-object.h>

typedef enum {
    MATRIX_ERROR_NONE,
    MATRIX_ERROR_COMMUNICATION_ERROR,
    MATRIX_ERROR_INCOMPLETE,
    MATRIX_ERROR_BAD_REQUEST,
    MATRIX_ERROR_BAD_RESPONSE,
    MATRIX_ERROR_INVALID_ROOM_ID,
    MATRIX_ERROR_UNKNOWN_VALUE,
    MATRIX_ERROR_INVALID_TYPE,
    MATRIX_ERROR_UNSUPPORTED,
    MATRIX_ERROR_INVALID_FORMAT,
    MATRIX_ERROR_UNAVAILABLE,
    MATRIX_ERROR_NOT_FOUND,
    MATRIX_ERROR_ALREADY_EXISTS,

    /* Add Matrix-defined error codes under here, prefixing them with
     * `MATRIX_ERROR_`, i.e. `M_FORBIDDEN` =>
     * `MATRIX_ERROR_M_FORBIDDEN` */

    MATRIX_ERROR_M_MISSING_TOKEN = 500,
    MATRIX_ERROR_M_FORBIDDEN,
    MATRIX_ERROR_M_UNKNOWN,
    MATRIX_ERROR_M_UNKNOWN_TOKEN,
    MATRIX_ERROR_M_NOT_JSON,
    MATRIX_ERROR_M_UNRECOGNIZED,
    MATRIX_ERROR_M_UNAUTHORIZED,
    MATRIX_ERROR_M_BAD_JSON,
    MATRIX_ERROR_M_USER_IN_USE,
    MATRIX_ERROR_M_ROOM_IN_USE,
    MATRIX_ERROR_M_BAD_PAGINATION,
    MATRIX_ERROR_M_BAD_STATE,
    MATRIX_ERROR_M_NOT_FOUND,
    MATRIX_ERROR_M_GUEST_ACCESS_FORBIDDEN,
    MATRIX_ERROR_M_LIMIT_EXCEEDED,
    MATRIX_ERROR_M_CAPTCHA_NEEDED,
    MATRIX_ERROR_M_CAPTCHA_INVALID,
    MATRIX_ERROR_M_MISSING_PARAM,
    MATRIX_ERROR_M_TOO_LARGE,
    MATRIX_ERROR_M_EXCLUSIVE,
    MATRIX_ERROR_M_THREEPID_AUTH_FAILED,
    MATRIX_ERROR_M_THREEPID_IN_USE,
    MATRIX_ERROR_M_INVALID_USERNAME,

    /* Allow for a lot of Matrix.org defined codes. Do not define
     * Matrix-specific error codes after this! */

    MATRIX_ERROR_UNSPECIFIED = 16383,
    MATRIX_ERROR_UNKNOWN_ERROR
} MatrixError;

# define MATRIX_ERROR matrix_error_quark()
GQuark matrix_error_quark(void);

typedef enum {
    MATRIX_ACCOUNT_KIND_DEFAULT,
    MATRIX_ACCOUNT_KIND_USER,
    MATRIX_ACCOUNT_KIND_GUEST
} MatrixAccountKind;

typedef enum {
    MATRIX_EVENT_DIRECTION_FORWARD,
    MATRIX_EVENT_DIRECTION_BACKWARD
} MatrixEventDirection;

#endif  /* __MATRIX_TYPE_H__ */
