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

[CCode (cprefix = "Matrix", gir_namespace = "Matrix", gir_version = "0.0", lower_case_cprefix = "matrix_")]
namespace Matrix {
    [CCode (cheader_filename = "matrix-enumtypes.h", cprefix = "MATRIX_ERROR_")]
    public errordomain Error {
        NONE,
        COMMUNICATION_ERROR,
        INCOMPLETE,
        BAD_REQUEST,
        BAD_RESPONSE,
        INVALID_ROOM_ID,
        UNKNOWN_VALUE,
        INVALID_TYPE,
        UNSUPPORTED,
        INVALID_FORMAT,
        UNAVAILABLE,
        NOT_FOUND,
        ALREADY_EXISTS,
        M_MISSING_TOKEN,
        M_FORBIDDEN,
        M_UNKNOWN,
        M_UNKNOWN_TOKEN,
        M_NOT_JSON,
        M_UNRECOGNIZED,
        M_UNAUTHORIZED,
        M_BAD_JSON,
        M_USER_IN_USE,
        M_ROOM_IN_USE,
        M_BAD_PAGINATION,
        M_BAD_STATE,
        M_NOT_FOUND,
        M_GUEST_ACCESS_FORBIDDEN,
        M_LIMIT_EXCEEDED,
        M_CAPTCHA_NEEDED,
        M_CAPTCHA_INVALID,
        M_MISSING_PARAM,
        M_TOO_LARGE,
        M_EXCLUSIVE,
        M_THREEPID_AUTH_FAILED,
        M_THREEPID_IN_USE,
        M_INVALID_USERNAME,
        UNSPECIFIED,
        UNKNOWN_ERROR;

        public static GLib.Quark quark ();
    }

    public const int MATRIX_ERROR;

    [CCode (cheader_filename = "matrix-enumtypes.h", cprefix = "MATRIX_ACCOUNT_KIND_")]
    public enum AccountKind {
        DEFAULT,
        USER,
        GUEST;
    }

    [CCode (cheader_filename = "matrix-enumtypes.h", cprefix = "MATRIX_EVENT_DIRECTION_")]
    public enum EventDirection {
        FORWARD,
        BACKWARD;
    }

    [CCode (cheader_filename = "matrix-enumtypes.h", cprefix = "MATRIX_EVENT_FORMAT_")]
    public enum EventFormat {
        DEFAULT,
        CLIENT,
        FEDERATION;
    }

    [CCode (cheader_filename = "matrix-enumtypes.h", cprefix = "MATRIX_PRESENCE_")]
    public enum Presence {
        UNKNOWN,
        ONLINE,
        OFFLINE,
        UNAVAILABLE,
        FREE_FOR_CHAT;
    }

    /**
     * The major version number of the Matrix.org GLib SDK.
     */
    [CCode (cheader_filename = "matrix-version.h", cname = "MATRIX_GLIB_MAJOR_VERSION")]
    public const int GLIB_MAJOR_VERSION;

    /**
     * The micro (patch) version number of the Matrix.org GLib SDK.
     */
    [CCode (cheader_filename = "matrix-version.h", cname = "MATRIX_GLIB_MINOR_VERSION")]
    public const int GLIB_MINOR_VERSION;

    /**
     * The minor version number of the Matrix.org GLib SDK.
     */
    [CCode (cheader_filename = "matrix-version.h", cname = "MATRIX_GLIB_MICRO_VERSION")]
    public const int GLIB_MICRO_VERSION;

    /**
     * Check that the Matrix.org GLib SDK in use is compatible with
     * the given version.
     *
     * @param required_major the required major version
     * @param required_minor the required minor version
     * @param required_micro the required micro version
     * @return {{{true}}} if the required version is satisfied; {{{false}}} otherwise.
     */
    [CCode (cheader_filename = "matrix-version.h", cname = "matrix_glib_check_version")]
    public bool glib_check_version(uint required_major,
                                   uint required_minor,
                                   uint required_micro);
}
