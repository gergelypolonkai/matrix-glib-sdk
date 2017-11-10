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

    [CCode (cheader_filename = "matrix-enumtypes.h", cprefix = "MATRIX_PUSHER_CONDITION_KIND_")]
    public enum PusherConditionKind {
        EVENT_MATCH,
        PROFILE_TAG,
        CONTAINS_DISPLAY_NAME,
        ROOM_MEMBER_COUNT;
    }

    [CCode (cheader_filename = "matrix-enumtypes.h", cprefix = "MATRIX_PUSHER_KIND_")]
    public enum PusherKind {
        OVERRIDE,
        SENDER,
        ROOM,
        CONTENT,
        UNDERRIDE;
    }

    [CCode (cheader_filename = "matrix-enumtypes.h", cprefix = "MATRIX_RECEIPT_TYPE_")]
    public enum ReceiptType {
        READ;
    }

    [CCode (cheader_filename = "matrix-enumtypes.h", cprefix = "MATRIX_RESIZE_METHOD_")]
    public enum ResizeMethod {
        DEFAULT,
        CROP,
        SCALE;
    }

    [CCode (cheader_filename = "matrix-enumtypes.h", cprefix = "MATRIX_ROOM_MEMBERSHIP_")]
    public enum RoomMembership {
        UNKNOWN,
        INVITE,
        JOIN,
        LEAVE,
        BAN,
        KNOCK;
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

    [CCode (cheader_filename = "matrix-enumtypes.h", cprefix = "MATRIX_JOIN_RULES_")]
    public enum JoinRules {
        UNKNOWN,
        PUBLIC,
        INVITE,
        PRIVATE,
        KNOCK;
    }

    [CCode (cheader_filename = "matrix-enumtypes.h", cprefix = "MATRIX_SEARCH_ORDER_")]
    public enum SearchOrder {
        RECENT,
        RANK;
    }

    [CCode (cheader_filename = "matrix-enumtypes.h", cprefix = "MATRIX_SEARCH_KEY_")]
    public enum SearchKey {
        CONTENT_BODY,
        CONTENT_NAME,
        CONTENT_TOPIC;
    }

    [CCode (cheader_filename = "matrix-enumtypes.h", cprefix = "MATRIX_SEARCH_GROUP_BY_")]
    public enum SearchGroupBy {
        NONE,
        ROOM_ID,
        SENDER;
    }

    [CCode (cheader_filename = "matrix-enumtypes.h", cprefix = "MATRIX_HISTORY_VISIBILITY_")]
    public enum HistoryVisibility {
        UNKNOWN,
        INVITED,
        JOINED,
        SHARED,
        WORLD_READABLE;
    }

    [CCode (cheader_filename = "matrix-enumtypes.h", cprefix = "MATRIX_GUEST_ACCESS_")]
    public enum GuestAccess {
        UNKNOWN,
        CAN_JOIN,
        FORBIDDEN;
    }

    [CCode (cheader_filename = "matrix-enumtypes.h", cprefix = "MATRIX_CALL_OFFER_TYPE_")]
    public enum CallOfferType {
        UNKNOWN,
        OFFER;
    }

    [CCode (cheader_filename = "matrix-enumtypes.h", cprefix = "MATRIX_CALL_ANSWER_TYPE_")]
    public enum CallAnswerType {
        UNKNOWN,
        ANSWER;
    }

    [CCode (cheader_filename = "matrix-types.h")]
    public struct FileInfo {
        public void set_size(ssize size);
        public ssize get_size();

        public void set_mimetype(string? mimetype);
        public string? get_mimetype();

        public void set_from_json(Json.Node json_data);

        public Json.Node get_json_node()
            throws Matrix.Error;
    }

    [CCode (cheader_filename = "matrix-types.h")]
    public struct AudioInfo {
        public ssize get_size();
        public void set_size(ssize size);

        public string? get_mimetype();
        public void set_mimetype(string? mimetype);

        public int get_duration();
        public void set_duration(int duration);

        public void set_from_json(Json.Node json_data);

        public Json.Node get_json_node()
            throws Matrix.Error;
    }

    [CCode (cheader_filename = "matrix-types.h")]
    public struct ImageInfo {
        public ssize get_size();
        public void set_size(ssize size);
        public int get_height();
        public void set_height(int height);
        public int get_width();
        public void set_width(int width);
        string? mimetype;

        public void set_from_json(Json.Node json_data);

        public Json.Node get_json_node()
            throws Matrix.Error;

        public bool differs(ImageInfo other);
    }

    [CCode (cheader_filename = "matrix-types.h")]
    public struct VideoInfo {
        public ssize get_size();
        public void set_size(ssize size);

        public string? get_mimetype();
        public void set_mimetype(string? mimetype);

        public int get_duration();
        public void set_duration(int duration);

        public int get_width();
        public void set_width(int witdh);

        public int get_height();
        public void set_height(int height);

        public string? get_thumbnail_url();
        public void set_thumbnail_url();

        public ImageInfo? get_thumbnail_info();
        public void set_image_info(ImageInfo? image_info);

        public void
        set_from_json(Json.Node json_data);

        public Json.Node
        get_json_node()
        throws Matrix.Error;
    }

    /* Compact classes */
    [CCode (cheader_filename = "matrix-c-compacts.h")]
    public abstract class JsonCompact {
        public JsonCompact();

        public abstract Json.Node? get_json_node()
            throws Matrix.Error;

        public string get_json_data(out size_t datalen)
            throws Matrix.Error;
    }

    [CCode (cheader_filename = "matrix-c-compacts.h")]
    public class FilterRules : JsonCompact {
        public uint limit { get; set; }
        public string[] types { get; set; }
        public string[] excluded_types { get; set; }
        public string[] senders { get; set; }
        public string[] excluded_senders { get; set; }
        public string[] rooms { get; set; }
        public string[] excluded_rooms { get; set; }
        public override Json.Node? get_json_node()
            throws Matrix.Error;
    }

    [CCode (cheader_filename = "matrix-c-compacts.h")]
    public class RoomFilter : JsonCompact {
        public bool include_leave { get; set; default=true; }
        public FilterRules? ephemeral { get; set; default = null; }
        public FilterRules? state { get; set; default = null; }
        public FilterRules? timeline { get; set; default = null; }

        public override Json.Node? get_json_node()
            throws Matrix.Error;
    }

    [CCode (cheader_filename = "matrix-c-compacts.h")]
    public class Filter : JsonCompact {
        public string[] event_fields { get; set; }
        public EventFormat event_format { get; set; default = Matrix.EventFormat.CLIENT; }
        public FilterRules? presence_filter { get; set; default = null; }
        public RoomFilter? room_filter { get; set; default = null; }
        public override Json.Node? get_json_node()
            throws Matrix.Error;
    }

    [CCode (cheader_filename = "matrix-c-compacts.h")]
    public class @3PidCredential : JsonCompact {
        public string? id_server { get; set; default = null; }
        public string? session_id { get; set; default = null; }
        public string? client_secret { get; set; default = null; }
        public override Json.Node?
        get_json_node()
            throws Matrix.Error;
    }

    [CCode (cheader_filename = "matrix-c-compacts.h")]
    public class Pusher : JsonCompact {
        public string? device_display_name { get; set; default = null; }
        public string? app_display_name { get; set; default = null; }
        public string? app_id { get; set; default = null; }
        public bool append { get; set; default = true; }
        public string? kind { get; set; default = null; }
        public string? lang { get; set; default = null; }
        public string? profile_tag { get; set; default = null; }
        public string? pushkey { get; set; default = null; }
        public Json.Node? data { get; set; default = null; }
        public override Json.Node? get_json_node()
            throws Matrix.Error;
    }

    [CCode (cheader_filename = "matrix-c-compacts.h")]
    public class EventContext : JsonCompact {
        public int before_limit { get; set; default = -1; }
        public int after_limit { get; set; default = -1; }
        public bool include_profile { get; set; default = false; }

        public override Json.Node? get_json_node()
            throws Matrix.Error;
     }

    [CCode (cheader_filename = "matrix-c-compacts.h")]
    public class SearchGrouping : JsonCompact {
        public SearchGroupBy key { get; set; default = SearchGroupBy.NONE; }

        public override Json.Node? get_json_node()
            throws Matrix.Error;
    }

    [CCode (cheader_filename = "matrix-c-compacts.h")]
    public class SearchGroupings : JsonCompact {
        public SearchGrouping[] group_by { get; set; }

        public override Json.Node?
        get_json_node()
            throws Matrix.Error;
    }

    [CCode (cheader_filename = "matrix-c-compacts.h")]
    public class SearchRoomEvents : JsonCompact {
        public SearchOrder order_by { get; set; default = SearchOrder.RECENT; }
        public SearchKey[] keys { get; set; }
        public EventContext? event_context { get; set; default = null; }
        public bool include_state { get; set; default = false; }
        public string? filter_id { get; set; default = null; }
        public Filter? filter { get; set; default = null; }
        public string search_term { get; set; }
        public SearchGroupings? groupings { get; set; default = null; }

        public override Json.Node? get_json_node()
            throws Matrix.Error;
    }

    [CCode (cheader_filename = "matrix-c-compacts.h")]
    public class SearchCategories : JsonCompact {
        public SearchRoomEvents? room_events { get; set; default = null; }

        public override Json.Node? get_json_node()
            throws Matrix.Error;
    }

    /* Utilities */
    [CCode (cheader_filename = "utils.h", cname = "_matrix_g_enum_to_string")]
    public string? _g_enum_value_to_nick(GLib.Type enum_type, int value, char convert_dashes = '_');

    [CCode (cheader_filename = "utils.h", cname = "_matrix_g_enum_nick_to_value")]
    public int _g_enum_nick_to_value(GLib.Type enum_type, string nick)
        throws Matrix.Error;

    [CCode (cheader_filename = "matrix-profile.h")]
    public class Profile : GLib.Object {
        public GLib.TimeSpan age {get;}
        public string? avatar_url {get; set;}
        public string? display_name {get; set;}

        public Profile();
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
