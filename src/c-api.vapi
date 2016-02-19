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
        M_MISSING_TOKEN,
        M_FORBIDDEN,
        M_UNKNOWN,
        M_UNKNOWN_TOKEN,
        M_NOT_JSON,
        M_UNRECOGNIZED,
        UNSPECIFIED,
        UNKNOWN_ERROR;
        public static GLib.Quark quark ();
    }

    [CCode (cheader_filename = "matrix-enumtypes.h", cprefix = "MATRIX_RESIZE_METHOD_")]
    public enum ResizeMethod {
        DEFAULT,
        CROP,
        SCALE;
    }

    [CCode (cheader_filename = "matrix-enumtypes.h", cprefix = "MATRIX_PRESENCE_")]
    public enum Presence {
        UNKNOWN,
        ONLINE,
        OFFLINE,
        UNAVAILABLE,
        FREE_FOR_CHAT;
    }

    [CCode (cheader_filename = "matrix-enumtypes.h", cprefix = "MATRIX_PUSHER_KIND_")]
    public enum PusherKind {
        OVERRIDE,
        SENDER,
        ROOM,
        CONTENT,
        UNDERRIDE;
    }

    [CCode (cheader_filename = "matrix-enumtypes.h", cprefix = "MATRIX_PUSHER_CONDITION_KIND_")]
    public enum PusherConditionKind {
        EVENT_MATCH,
        PROFILE_TAG,
        CONTAINS_DISPLAY_NAME,
        ROOM_MEMBER_COUNT;
    }

    [CCode (cheader_filename = "matrix-enumtypes.h", cprefix = "MATRIX_ROOM_PRESET_")]
    public enum RoomPreset {
        NONE,
        PRIVATE,
        TRUSTED_PRIVATE,
        PUBLIC;
    }

    [CCode (cheader_filename = "matrix-enumtypes.h", cprefix = "MATRIX_ROOM_VISIBILITY_")]
    public enum RoomVisibility {
        DEFAULT,
        PUBLIC,
        PRIVATE;
    }

    [CCode (cheader_filename = "matrix-enumtypes.h", cprefix = "MATRIX_EVENT_DIRECTION_")]
    public enum EventDirection {
        FORWARD,
        BACKWARD;
    }

    [CCode (cheader_filename = "matrix-enumtypes.h", cprefix = "MATRIX_RECEIPT_TYPE_")]
    public enum ReceiptType {
        READ;
    }

    [CCode (cheader_filename = "matrix-enumtypes.h", cprefix = "MATRIX_ACCOUNT_KIND_")]
    public enum AccountKind {
        DEFAULT,
        USER,
        GUEST;
    }

    [CCode (cheader_filename = "matrix-types.h")]
    public class StateEvent {
        public Json.Node? get_json_node();
    }

    [CCode (cheader_filename = "utils.h", cname = "_json_node_deep_copy")]
    public Json.Node?
    _json_node_deep_copy(Json.Node? node);
}
