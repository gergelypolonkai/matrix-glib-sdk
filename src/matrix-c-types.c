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

#include "matrix-c-types.h"

/**
 * SECTION:matrix-types
 * @title: Generic types
 * @short_description: Generic types for Matrix GLib SDK calls
 *
 * These are the generic types used by many SDK calls for communication with the homeserver.
 */

/**
 * MatrixError:
 * @MATRIX_ERROR_NONE: no error. You should never see this.
 * @MATRIX_ERROR_COMMUNICATION_ERROR: there was a problem in communication (e.g. connection error)
 * @MATRIX_ERROR_INCOMPLETE: the passed/generated data is incomplete
 * @MATRIX_ERROR_BAD_REQUEST: the request is invalid
 * @MATRIX_ERROR_BAD_RESPONSE: malformed response, or the response is not a JSON object
 * @MATRIX_ERROR_INVALID_ROOM_ID: the provided string doesn’t contain a valid room ID
 * @MATRIX_ERROR_UNKNOWN_VALUE: the response from the Matrix.org server contains a value unknown
 *     to this library. These should be reported to the Matrix GLib SDK developers
 * @MATRIX_ERROR_INVALID_TYPE: the provided type is invalid
 * @MATRIX_ERROR_UNSUPPORTED: the operation is unsupported
 * @MATRIX_ERROR_INVALID_FORMAT: the format of the JSON node is invalid (e.g. it is an array instead of an object)
 * @MATRIX_ERROR_UNAVAILABLE: the requested data is not cached yet. Clients getting this message
 *     may go online by some means to get the data
 * @MATRIX_ERROR_NOT_FOUND: the requested data (e.g. member of a room) can not be found
 * @MATRIX_ERROR_ALREADY_EXISTS: the data to create (e.g. when adding a new member to a Room
 *     object) already exists
 * @MATRIX_ERROR_M_MISSING_TOKEN: authorization token is missing from the request
 * @MATRIX_ERROR_M_FORBIDDEN: access was forbidden (e.g. due to a missing/invalid token, or using
 *     a bad password during login)
 * @MATRIX_ERROR_M_UNKNOWN: an error unknown to the Matrix homeserver
 * @MATRIX_ERROR_M_UNKNOWN_TOKEN: the token provided is not known for the homeserver
 * @MATRIX_ERROR_M_NOT_JSON: illegal request, the content is not valid JSON
 * @MATRIX_ERROR_M_UNRECOGNIZED: the homeserver didn't understand the request
 * @MATRIX_ERROR_M_UNAUTHORIZED: the request is unauthorized
 * @MATRIX_ERROR_M_BAD_JSON: the JSON data is not in the required format
 * @MATRIX_ERROR_M_USER_IN_USE: the specified username is in use
 * @MATRIX_ERROR_M_ROOM_IN_USE: the specified room is in use
 * @MATRIX_ERROR_M_BAD_PAGINATION: invalid pagination parameters
 * @MATRIX_ERROR_M_BAD_STATE: invalid state event
 * @MATRIX_ERROR_M_NOT_FOUND: the requested resource is not found
 * @MATRIX_ERROR_M_GUEST_ACCESS_FORBIDDEN: guest access was requested, but ( it is forbidden
 * @MATRIX_ERROR_M_LIMIT_EXCEEDED: the request was rate limited
 * @MATRIX_ERROR_M_CAPTCHA_NEEDED: a captcha is needed to continue
 * @MATRIX_ERROR_M_CAPTCHA_INVALID: the provided captcha is invalid
 * @MATRIX_ERROR_M_MISSING_PARAM: a parameter is missing from the request
 * @MATRIX_ERROR_M_TOO_LARGE: the request data is too large
 * @MATRIX_ERROR_M_EXCLUSIVE: the desired user ID is in an exclusive namespace claimed by an
 *     application server
 * @MATRIX_ERROR_M_THREEPID_AUTH_FAILED: 3rd party authentication failed
 * @MATRIX_ERROR_M_THREEPID_IN_USE: the provided 3rd party ID is already in use
 * @MATRIX_ERROR_M_INVALID_USERNAME: the given username is invalid
 * @MATRIX_ERROR_UNSPECIFIED: no error code was sent by the homeserver. If you see this error,
 *     that usually indicates a homeserver bug
 * @MATRIX_ERROR_UNKNOWN_ERROR: an error unknown to this library
 *
 * Matrix SDK error codes.  The SDK maps most known error codes from homeservers, too; these are
 * the MATRIX_ERROR_M_* codes.
 */

/**
 * MATRIX_ERROR:
 *
 * Error domain for Matrix GLib SDK.  See #GError for more information on error domains.
 */

/**
 * matrix_error_quark:
 *
 * Gets the Matrix error #GQuark
 */
G_DEFINE_QUARK(matrix-error-quark, matrix_error);

/**
 * MatrixAccountKind:
 * @MATRIX_ACCOUNT_KIND_DEFAULT: use the server default (usually #MATRIX_ACCOUNT_KIND_USER)
 * @MATRIX_ACCOUNT_KIND_USER: normal user
 * @MATRIX_ACCOUNT_KIND_GUEST: guest user
 *
 * User account types.
 */

/**
 * MatrixEventDirection:
 * @MATRIX_EVENT_DIRECTION_FORWARD: list events after the specified one
 * @MATRIX_EVENT_DIRECTION_BACKWARD: list events before the specified one
 *
 * Direction of events when requesting an event context.
 */
