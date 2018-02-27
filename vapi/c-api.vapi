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
        public void set_size(int64 size);
        public int64 get_size();

        public void set_mimetype(string? mimetype);
        public string? get_mimetype();

        public void set_from_json(Json.Node json_data);

        public Json.Node get_json_node()
            throws Matrix.Error;
    }

    [CCode (cheader_filename = "matrix-types.h")]
    public struct AudioInfo {
        public int64 get_size();
        public void set_size(int64 size);

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
        public int64 get_size();
        public void set_size(int64 size);
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
        public int64 get_size();
        public void set_size(int64 size);

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

    public struct ThirdPartyInvitePublicKey {
    }

    public struct CallCandidate {
    }

    /* Compact classes */
    [CCode (cheader_filename = "matrix-compacts.h")]
    public abstract class JsonCompact {
        public JsonCompact();

        public abstract Json.Node? get_json_node()
            throws Matrix.Error;

        public string get_json_data(out size_t datalen)
            throws Matrix.Error;
    }

    [CCode (cheader_filename = "matrix-compacts.h")]
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

    [CCode (cheader_filename = "matrix-compacts.h")]
    public class RoomFilter : JsonCompact {
        public bool include_leave { get; set; default=true; }
        public FilterRules? ephemeral { get; set; default = null; }
        public FilterRules? state { get; set; default = null; }
        public FilterRules? timeline { get; set; default = null; }

        public override Json.Node? get_json_node()
            throws Matrix.Error;
    }

    [CCode (cheader_filename = "matrix-compacts.h")]
    public class Filter : JsonCompact {
        public string[] event_fields { get; set; }
        public EventFormat event_format { get; set; default = Matrix.EventFormat.CLIENT; }
        public FilterRules? presence_filter { get; set; default = null; }
        public RoomFilter? room_filter { get; set; default = null; }
        public override Json.Node? get_json_node()
            throws Matrix.Error;
    }

    [CCode (cheader_filename = "matrix-compacts.h")]
    public class @3PidCredential : JsonCompact {
        public string? id_server { get; set; default = null; }
        public string? session_id { get; set; default = null; }
        public string? client_secret { get; set; default = null; }
        public override Json.Node?
        get_json_node()
            throws Matrix.Error;
    }

    [CCode (cheader_filename = "matrix-compacts.h")]
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

    [CCode (cheader_filename = "matrix-compacts.h")]
    public class EventContext : JsonCompact {
        public int before_limit { get; set; default = -1; }
        public int after_limit { get; set; default = -1; }
        public bool include_profile { get; set; default = false; }

        public override Json.Node? get_json_node()
            throws Matrix.Error;
     }

    [CCode (cheader_filename = "matrix-compacts.h")]
    public class SearchGrouping : JsonCompact {
        public SearchGroupBy key { get; set; default = SearchGroupBy.NONE; }

        public override Json.Node? get_json_node()
            throws Matrix.Error;
    }

    [CCode (cheader_filename = "matrix-compacts.h")]
    public class SearchGroupings : JsonCompact {
        public SearchGrouping[] group_by { get; set; }

        public override Json.Node?
        get_json_node()
            throws Matrix.Error;
    }

    [CCode (cheader_filename = "matrix-compacts.h")]
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

    [CCode (cheader_filename = "matrix-compacts.h")]
    public class SearchCategories : JsonCompact {
        public SearchRoomEvents? room_events { get; set; default = null; }

        public override Json.Node? get_json_node()
            throws Matrix.Error;
    }

    [CCode (cheader_filename = "matrix-room.h")]
    public class Room : GLib.Object {
        public string room_id { get; construct; }
        public string[] aliases { get; set; }
        public string? avatar_url { get; set; default = null; }
        public Matrix.ImageInfo? avatar_info { get; set; default = null; }
        public string? avatar_thumbnail_url { get; set; default = null; }
        public Matrix.ImageInfo? avatar_thumbnail_info { get; set; default = null; }
        public string? canonical_alias { get; set; default = null; }
        public string? creator { get; set; default = null; }
        public bool federate { get; set; default = false; }
        public Matrix.GuestAccess guest_access { get; set; default = Matrix.GuestAccess.UNKNOWN; }
        public Matrix.HistoryVisibility history_visibility { get; set; default = Matrix.HistoryVisibility.UNKNOWN; }
        public Matrix.JoinRules join_rules { get; set; default = Matrix.JoinRules.UNKNOWN; }
        public string? name { get; set; default = null; }
        public int default_power_level { get; set; default = 0; }
        public int default_event_level { get; set; default = 0; }
        public int default_state_level { get; set; default = 10; }
        public int ban_level { get; set; default = 5; }
        public int kick_level { get; set; default = 5; }
        public int redact_level { get; set; default = 20; }
        public int invite_level { get; set; default = 0; }
        public string? topic { get; set; default = null; }
        public string[] typing_users { get; set; }

        public Room(string room_id);

        public void add_member(string user_id, Profile? profile, bool third_party)
        throws Matrix.Error;

        public Profile
        get_or_add_member(string user_id, bool third_party = false)
        throws Matrix.Error;

        public Profile
        get_member(string user_id, out bool third_party)
        throws Matrix.Error;

        public void
        remove_member(string user_id)
        throws Matrix.Error;

        public void clear_user_levels();

        public void set_user_level(string user_id, int level);

        public int get_user_level(string user_id)
        throws Matrix.Error;

        public void clear_event_levels();

        public void set_event_level(string event_type, int level);

        public int? get_event_level(string event_type);
    }
    /* Utilities */
    [CCode (cheader_filename = "utils.h", cname = "_matrix_g_enum_to_string")]
    public string? _g_enum_value_to_nick(GLib.Type enum_type, int value, char convert_dashes = '_');

    [CCode (cheader_filename = "utils.h", cname = "_matrix_g_enum_nick_to_value")]
    public int _g_enum_nick_to_value(GLib.Type enum_type, string nick)
        throws Matrix.Error;

    [CCode (cheader_filename = "utils.h", cname = "_matrix_json_node_deep_copy")]
    public Json.Node? _json_node_deep_copy(Json.Node? node);

    [CCode (cheader_filename = "matrix-profile.h")]
    public class Profile : GLib.Object {
        public GLib.TimeSpan age {get;}
        public string? avatar_url {get; set;}
        public string? display_name {get; set;}

        public Profile();
    }

    [CCode (cheader_filename = "matrix-api.h", type_cname = "MatrixAPIInterface")]
    public interface API : GLib.Object {
        public abstract string? token { get; set; default = null; }
        public abstract string? user_id { get; default = null; }
        public abstract string? homeserver { get; default = null; }

        public delegate void
        Callback(Matrix.API api,
                 string content_type,
                 Json.Node? json_content,
                 GLib.ByteArray? raw_content,
                 Matrix.Error? err);

        public abstract void abort_pending();

        /* User data */

        public abstract void get_3pids([CCode (scope = "async")]owned Matrix.API.Callback? @callback)
        throws Matrix.Error;

        public abstract void add_3pid([CCode (delegate_target_pos = 1.5, scope = "async")] owned Matrix.API.Callback? @callback, bool bind_creds, Matrix.3PidCredential threepid_creds)
        throws Matrix.Error;

        public abstract void deactivate_account([CCode (delegate_target_pos = 1.5, scope = "async")] owned Matrix.API.Callback? @callback, string? session, string? login_type)
        throws Matrix.Error;

        public abstract void change_password([CCode (delegate_target_pos = 1.5, scope = "async")] owned Matrix.API.Callback? @callback, string new_password)
        throws Matrix.Error;

        public abstract void get_profile([CCode (delegate_target_pos = 1.5, scope = "async")] owned Matrix.API.Callback? @callback, string user_id)
        throws Matrix.Error;

        public abstract void get_avatar_url([CCode (delegate_target_pos = 1.5, scope = "async")] owned Matrix.API.Callback? @callback, string user_id)
        throws Matrix.Error;

        public abstract void set_avatar_url([CCode (delegate_target_pos = 1.5, scope = "async")] owned Matrix.API.Callback? @callback, string user_id, string avatar_url)
        throws Matrix.Error;

        public abstract void get_display_name([CCode (delegate_target_pos = 1.5, scope = "async")] owned Matrix.API.Callback? @callback, string user_id)
        throws Matrix.Error;

        public abstract void set_display_name([CCode (delegate_target_pos = 1.5, scope = "async")] owned Matrix.API.Callback? @callback, string user_id, string display_name)
        throws Matrix.Error;

        public abstract void register_account([CCode (delegate_target_pos = 1.5, scope = "async")] owned Matrix.API.Callback? @callback, Matrix.AccountKind account_kind, bool bind_email, string? username, string password)
        throws Matrix.Error;

        public abstract void set_account_data([CCode (delegate_target_pos = 1.5, scope = "async")] owned Matrix.API.Callback? @callback, string user_id, string? room_id, string event_type, owned Json.Node content)
        throws Matrix.Error;

        public abstract void get_room_tags([CCode (delegate_target_pos = 1.5, scope = "async")] owned Matrix.API.Callback? @callback, string user_id, string room_id)
        throws Matrix.Error;

        public abstract void delete_room_tag([CCode (delegate_target_pos = 1.5, scope = "async")] owned Matrix.API.Callback? @callback, string user_id, string room_id, string tag)
        throws Matrix.Error;

        public abstract void add_room_tag([CCode (delegate_target_pos = 1.5, scope = "async")] owned Matrix.API.Callback? @callback, string user_id, string room_id, string tag, owned Json.Node? content)
        throws Matrix.Error;

        public abstract void whois([CCode (delegate_target_pos = 1.5, scope = "async")] owned Matrix.API.Callback? @callback, string user_id)
        throws Matrix.Error;

        public abstract void versions([CCode (scope = "async")] owned Matrix.API.Callback? @callback)
        throws Matrix.Error;

        public abstract void create_room([CCode (delegate_target_pos = 1.5, scope = "async")] owned Matrix.API.Callback? @callback, Matrix.RoomPreset preset, string? room_name, string? room_alias, string? topic, Matrix.RoomVisibility visibility, Json.Node? creation_content, Matrix.Event.State[] initial_state, string[] invitees, Matrix.3PidCredential[] invite_3pids)
        throws Matrix.Error;

        public abstract void delete_room_alias([CCode (delegate_target_pos = 1.5, scope = "async")] owned Matrix.API.Callback? @callback, string room_alias)
        throws Matrix.Error;

        public abstract void get_room_id([CCode (delegate_target_pos = 1.5, scope = "async")] owned Matrix.API.Callback? @callback, string room_alias)
        throws Matrix.Error;

        public abstract void create_room_alias([CCode (delegate_target_pos = 1.5, scope = "async")] owned Matrix.API.Callback? @callback, string room_id, string room_alias)
        throws Matrix.Error;

        [Version (deprecated = true, deprecated_since = "v0", replacement = "sync")]
        public abstract void event_stream([CCode (delegate_target_pos = 1.5, scope = "async")] owned Matrix.API.Callback? @callback, string? from_token, ulong timeout)
        throws Matrix.Error;

        [Version (deprecated = true, deprecated_since = "v0", replacement = "sync")]
        public abstract void get_event([CCode (delegate_target_pos = 1.5, scope = "async")] owned Matrix.API.Callback? @callback, string event_id)
        throws Matrix.Error;

        [Version (deprecated = true, deprecated_since = "v0", replacement = "sync")]
        public abstract void initial_sync([CCode (delegate_target_pos = 1.5, scope = "async")] owned Matrix.API.Callback? @callback, uint limit, bool archived)
        throws Matrix.Error;

        public abstract void get_event_context([CCode (delegate_target_pos = 1.5, scope = "async")] owned Matrix.API.Callback? callback, string room_id, string event_id, uint limit)
        throws Matrix.Error;

        [Version (deprecated = true, deprecated_since = "v0", replacement = "sync")]
        public abstract void initial_sync_room([CCode (delegate_target_pos = 1.5, scope = "async")] owned Matrix.API.Callback? @callback, string room_id)
        throws Matrix.Error;

        public abstract void list_room_members([CCode (delegate_target_pos = 1.5, scope = "async")] owned Matrix.API.Callback? @callback, string room_id)
        throws Matrix.Error;

        public abstract void list_room_messages([CCode (delegate_target_pos = 1.5, scope = "async")] owned Matrix.API.Callback? @callback, string room_id, string from_token, Matrix.EventDirection direction, uint limit)
        throws Matrix.Error;

        public abstract void send_event_receipt([CCode (delegate_target_pos = 1.5, scope = "async")] owned Matrix.API.Callback? @callback, string room_id, Matrix.ReceiptType receipt_type, string event_id, Json.Node receipt)
        throws Matrix.Error;

        public abstract void redact_event([CCode (delegate_target_pos = 1.5, scope = "async")] owned Matrix.API.Callback? @callback, string room_id, string event_id, string txn_id, string? reason)
        throws Matrix.Error;

        public abstract void send_event([CCode (delegate_target_pos = 1.5, scope = "async")] owned Matrix.API.Callback? @callback, string room_id, string event_type, string txn_id, owned Json.Node content)
        throws Matrix.Error;

        public abstract void get_room_state([CCode (delegate_target_pos = 1.5, scope = "async")] owned Matrix.API.Callback? @callback, string room_id, string? event_type, string? state_key)
        throws Matrix.Error;

        public abstract void send_state_event([CCode (delegate_target_pos = 1.5, scope = "async")] owned Matrix.API.Callback? @callback, string room_id, string event_type, string? state_key, owned Json.Node content)
        throws Matrix.Error;

        public abstract void notify_room_typing([CCode (delegate_target_pos = 1.5, scope = "async")] owned Matrix.API.Callback? @callback, string user_id, string room_id, uint timeout, bool typing)
        throws Matrix.Error;

        public abstract void sync([CCode (delegate_target_pos = 1.5, scope = "async")] owned Matrix.API.Callback? @callback, string? filter_id, Matrix.Filter? filter, string? since, bool full_state, bool set_presence, ulong timeout)
        throws Matrix.Error;

        public abstract void create_filter([CCode (delegate_target_pos = 1.5, scope = "async")] owned Matrix.API.Callback? @callback, string user_id, Matrix.Filter filter)
        throws Matrix.Error;

        public abstract void download_filter([CCode (delegate_target_pos = 1.5, scope = "async")] owned Matrix.API.Callback? @callback, string user_id, string filter_id)
        throws Matrix.Error;

        public abstract void join_room_id_or_alias([CCode (delegate_target_pos = 1.5, scope = "async")] owned Matrix.API.Callback? @callback, string room_id_or_alias)
        throws Matrix.Error;

        public abstract void ban_user([CCode (delegate_target_pos = 1.5, scope = "async")] owned Matrix.API.Callback? @callback, string room_id, string user_id, string? reason)
        throws Matrix.Error;

        public abstract void forget_room([CCode (delegate_target_pos = 1.5, scope = "async")] owned Matrix.API.Callback? @callback, string room_id)
        throws Matrix.Error;

        public abstract void invite_user_3rdparty([CCode (delegate_target_pos = 1.5, scope = "async")] owned Matrix.API.Callback? @callback, string room_id, Matrix.3PidCredential credential)
        throws Matrix.Error;

        public abstract void invite_user([CCode (delegate_target_pos = 1.5, scope = "async")] owned Matrix.API.Callback? @callback, string room_id, string user_id)
        throws Matrix.Error;

        public abstract void join_room([CCode (delegate_target_pos = 1.5, scope = "async")] owned Matrix.API.Callback? @callback, string room_id)
        throws Matrix.Error;

        public abstract void kick_user([CCode (delegate_target_pos = 1.5, scope = "async")] owned Matrix.API.Callback? @callback, string room_id, string user_id, string? reason)
        throws Matrix.Error;

        public abstract void leave_room([CCode (delegate_target_pos = 1.5, scope = "async")] owned Matrix.API.Callback? @callback, string room_id)
        throws Matrix.Error;

        public abstract void unban_user([CCode (delegate_target_pos = 1.5, scope = "async")] owned Matrix.API.Callback? @callback, string room_id, string user_id)
        throws Matrix.Error;

        public abstract void login([CCode (delegate_target_pos = 1.5, scope = "async")] owned Matrix.API.Callback? @callback, string login_type, Json.Node? content)
        throws Matrix.Error;

        public abstract void logout([CCode (delegate_target_pos = 1.5, scope = "async")] owned Matrix.API.Callback? @callback)
        throws Matrix.Error;

        public abstract void get_presence_list([CCode (delegate_target_pos = 1.5, scope = "async")] owned Matrix.API.Callback? @callback, string user_id)
        throws Matrix.Error;

        public abstract void update_presence_list([CCode (delegate_target_pos = 1.5, scope = "async")] owned Matrix.API.Callback? @callback, string user_id, string[] drop_ids, string[] invite_ids)
        throws Matrix.Error;

        public abstract void get_presence([CCode (delegate_target_pos = 1.5, scope = "async")] owned Matrix.API.Callback? @callback, string user_id)
        throws Matrix.Error;

        public abstract void set_presence([CCode (delegate_target_pos = 1.5, scope = "async")] owned Matrix.API.Callback? @callback, string user_id, Matrix.Presence presence, string? status_message)
        throws Matrix.Error;

        public abstract void list_public_rooms([CCode (scope = "async")] owned Matrix.API.Callback? @callback)
        throws Matrix.Error;

        public abstract void get_pushers([CCode (scope = "async")] owned Matrix.API.Callback? @callback)
        throws Matrix.Error;

        public abstract void update_pusher([CCode (delegate_target_pos = 1.5, scope = "async")] owned Matrix.API.Callback? @callback, Matrix.Pusher pusher)
        throws Matrix.Error;

        public abstract void get_pushrules([CCode (delegate_target_pos = 1.5, scope = "async")] owned Matrix.API.Callback? @callback)
        throws Matrix.Error;

        public abstract void delete_pushrule([CCode (delegate_target_pos = 1.5, scope = "async")] owned Matrix.API.Callback? @callback, string scope, Matrix.PusherKind kind, string rule_id)
        throws Matrix.Error;

        public abstract void get_pushrule([CCode (delegate_target_pos = 1.5, scope = "async")] owned Matrix.API.Callback? @callback, string scope, Matrix.PusherKind kind, string rule_id)
        throws Matrix.Error;

        public abstract void add_pushrule([CCode (delegate_target_pos = 1.5, scope = "async")] owned Matrix.API.Callback? @callback, string scope, Matrix.PusherKind kind, string rule_id, string? before, string? after, string[] actions, Matrix.PusherConditionKind[] conditions)
        throws Matrix.Error;

        public abstract void toggle_pushrule([CCode (delegate_target_pos = 1.5, scope = "async")] owned Matrix.API.Callback? @callback, string scope, Matrix.PusherKind kind, string rule_id, bool enabled)
        throws Matrix.Error;

        public abstract void search([CCode (delegate_target_pos = 1.5, scope = "async")] owned Matrix.API.Callback? @callback, string? next_batch, SearchCategories search_categories)
        throws Matrix.Error;

        public abstract void get_turn_server([CCode (scope = "async")] owned Matrix.API.Callback? @callback)
        throws Matrix.Error;

        public abstract void media_download([CCode (delegate_target_pos = 1.5, scope = "async", destroy_notify_pos = -1)] owned Matrix.API.Callback? @callback, string server_name, string media_id)
        throws Matrix.Error;

        public abstract void media_thumbnail([CCode (delegate_target_pos = 1.5, scope = "async")] owned Matrix.API.Callback? @callback, string server_name, string media_id, uint width, uint height, Matrix.ResizeMethod method)
        throws Matrix.Error;

        public abstract void media_upload([CCode (delegate_target_pos = 1.5, scope = "async")] owned Matrix.API.Callback? @callback, string? content_type, owned GLib.ByteArray content)
        throws Matrix.Error;
    }

    [CCode (lower_case_csuffix = "http_api", cheader_filename = "matrix-http-api.h")]
    public class HTTPAPI : GLib.Object, API {
        public string base_url { get; set; }
        public bool validate_certificate { get; set; }
        protected string? _user_id;
        public string? user_id { get; default = null; }
        public string? token { get; set; default = null; }
        protected string? _homeserver;
        public string? homeserver { get; default = null; }

        /* Media */

        public void media_download(API.Callback? cb, string server_name, string media_id)
        throws Matrix.Error;

        public void media_thumbnail(API.Callback? cb, string server_name, string media_id, uint width, uint height, ResizeMethod method)
        throws Matrix.Error;

        public void media_upload(API.Callback? cb, string? content_type, owned GLib.ByteArray content)
        throws Matrix.Error;

        /* Presence */

        public void get_presence_list(API.Callback? cb, string user_id)
        throws Matrix.Error;

        public void
        update_presence_list(API.Callback? cb, string user_id, string[] drop_ids, string[] invite_ids)
        throws Matrix.Error;

        public void get_presence(API.Callback? cb, string user_id)
        throws Matrix.Error;

        public void set_presence(API.Callback? cb, string user_id, Presence presence, string? status_message)
        throws Matrix.Error;

        /* Push notifications */

        public void update_pusher(API.Callback? cb, Matrix.Pusher pusher)
        throws Matrix.Error;

        public void get_pushers(API.Callback? cb)
        throws Matrix.Error;


        private void _pusher_modif(API.Callback? cb, string method, string scope, PusherKind kind, string rule_id)
        throws Matrix.Error;

        public void delete_pushrule(API.Callback? cb, string scope, PusherKind kind, string rule_id)
        throws Matrix.Error;

        public void get_pushrule(API.Callback? cb, string scope, PusherKind kind, string rule_id)
        throws Matrix.Error;

        public void add_pushrule(API.Callback? cb, string scope, PusherKind kind, string rule_id, string? before, string? after, string[] actions, PusherConditionKind[] conditions)
        throws Matrix.Error;

        public void toggle_pushrule(API.Callback? cb, string scope, PusherKind kind, string rule_id, bool enabled)
        throws Matrix.Error;

        public void get_pushrules(API.Callback? cb)
        throws Matrix.Error;

        /* Room creation */

        public void create_room(API.Callback? cb, RoomPreset preset, string? room_name, string? room_alias, string? topic, RoomVisibility visibility, Json.Node? creation_content, Matrix.Event.State[] initial_state, string[] invitees, 3PidCredential[] invite_3pids)
        throws Matrix.Error;

        /* Room directory */

        public void delete_room_alias(API.Callback? cb, string room_alias)
        throws Matrix.Error;

        public void get_room_id(API.Callback? cb, string room_alias)
        throws Matrix.Error;

        public void create_room_alias(API.Callback? cb, string room_id, string room_alias)
        throws Matrix.Error;

        /* Room discovery */

        public void list_public_rooms(API.Callback? cb)
        throws Matrix.Error;

        /* Room membership */

        public void ban_user(API.Callback? cb, string room_id, string user_id, string? reason)
        throws Matrix.Error;

        public void unban_user(API.Callback? cb, string room_id, string user_id)
        throws Matrix.Error;

        public void forget_room(API.Callback? cb, string room_id)
        throws Matrix.Error;

        public void invite_user_3rdparty(API.Callback? cb, string room_id, Matrix.3PidCredential credential)
        throws Matrix.Error;

        public void invite_user(API.Callback? cb, string room_id, string user_id)
        throws Matrix.Error;

        public void join_room(API.Callback? cb, string room_id)
        throws Matrix.Error;

        public void leave_room(API.Callback? cb, string room_id)
        throws Matrix.Error;

        public void join_room_id_or_alias(API.Callback? cb, string room_id_or_alias)
        throws Matrix.Error;

        public void kick_user(API.Callback? cb, string room_id, string user_id, string? reason)
        throws Matrix.Error;

        /* Room participation */

        public void event_stream(API.Callback? cb, string? from_token, ulong timeout)
        throws Matrix.Error;

        public void get_event(API.Callback? cb, string event_id)
        throws Matrix.Error;

        public void initial_sync(API.Callback? cb, uint limit, bool archived)
        throws Matrix.Error;

        public void get_event_context(API.Callback? cb, string room_id, string event_id, uint limit)
        throws Matrix.Error;

        public void initial_sync_room(API.Callback? cb, string room_id)
        throws Matrix.Error;

        public void list_room_members(API.Callback? cb, string room_id)
        throws Matrix.Error;

        public void list_room_messages(API.Callback? cb, string room_id, string from_token, EventDirection direction, uint limit)
        throws Matrix.Error;

        public void send_event_receipt(API.Callback? cb, string room_id, ReceiptType receipt_type, string event_id, Json.Node receipt)
        throws Matrix.Error;

        public void redact_event(API.Callback? cb, string room_id, string event_id, string txn_id, string? reason)
        throws Matrix.Error;

        public void send_event(API.Callback? cb, string room_id, string event_type, string txn_id, owned Json.Node content)
        throws Matrix.Error;

        public void get_room_state(API.Callback? cb, string room_id, string? event_type, string? state_key)
        throws Matrix.Error;

        public void send_state_event(API.Callback? cb, string room_id, string event_type, string? state_key, owned Json.Node content)
        throws Matrix.Error;

        public void notify_room_typing(API.Callback? cb, string user_id, string room_id, uint timeout, bool typing)
        throws Matrix.Error;

        public void sync(API.Callback? cb, string? filter_id, Filter? filter, string? since, bool full_state, bool set_presence, ulong timeout)
        throws Matrix.Error;

        public void create_filter(API.Callback? cb, string user_id, Filter filter)
        throws Matrix.Error;

        public void download_filter(API.Callback? cb, string user_id, string filter_id)
        throws Matrix.Error;

        /* Search */

        public void search(Matrix.API.Callback? cb, string? next_batch, SearchCategories search_categories)
        throws Matrix.Error;

        /* Server administration */

        public void whois(API.Callback? cb, string user_id)
        throws Matrix.Error;

        public void versions(API.Callback? cb)
        throws Matrix.Error;

        /* Session management */

        public void login(API.Callback? cb, string login_type, Json.Node? content)
        throws Matrix.Error;

        public void logout(API.Callback? cb)
        throws Matrix.Error;

        /* User data */

        public void get_3pids(API.Callback? cb)
        throws Matrix.Error;

        public void add_3pid(API.Callback? cb, bool bind_creds, Matrix.3PidCredential creds)
        throws Matrix.Error;

        public void change_password(API.Callback? cb, string new_password)
        throws Matrix.Error;

        public void get_profile(API.Callback? cb, string user_id)
        throws Matrix.Error;

        public void get_avatar_url(API.Callback? cb, string user_id)
        throws Matrix.Error;

        public void set_avatar_url(API.Callback? cb, string user_id, string avatar_url)
        throws Matrix.Error;

        public void get_display_name(API.Callback? cb, string user_id)
        throws Matrix.Error;

        public void set_display_name(API.Callback? cb, string user_name, string display_name)
        throws Matrix.Error;

        public void register_account(API.Callback? cb, AccountKind account_kind, bool bind_email, string? username, string password)
        throws Matrix.Error;

        public void set_account_data(API.Callback? cb, string user_id, string? room_id, string event_type, owned Json.Node content)
        throws Matrix.Error;

        public void get_room_tags(API.Callback? cb, string user_id, string room_id)
        throws Matrix.Error;

        public void delete_room_tag(API.Callback? cb, string user_id, string room_id, string tag)
        throws Matrix.Error;

        public void add_room_tag(API.Callback? cb, string user_id, string room_id, string tag, owned Json.Node? content)
        throws Matrix.Error;

        public void deactivate_account(API.Callback? cb, string? session, string? login_type)
        throws Matrix.Error;

        /* VoIP */

        public void get_turn_server(API.Callback? cb)
        throws Matrix.Error;

        /* Non-spec methods */

        public void abort_pending();
    }

    [CCode (cheader_filename = "matrix-client.h", type_cname = "MatrixClientInterface")]
    public interface Client : GLib.Object {
        public virtual signal void login_finished(bool success);

        [Signal (detailed=true)]
        public virtual signal void @event(string? room_id, Json.Node raw_event, Matrix.Event.Base? matrix_event);

        public signal void polling_started();

        public signal void polling_stopped(GLib.Error? error);

        public delegate void EventCallback(Matrix.Client client, string? room_id, Json.Node raw_event, Matrix.Event.Base? matrix_event);

        public abstract void login_with_password(string username, string password)
        throws Matrix.Error;

        public abstract void register_with_password(string? username, string password)
        throws Matrix.Error;

        public abstract void logout()
        throws Matrix.Error;

        public abstract void begin_polling()
        throws Matrix.Error;

        public abstract void stop_polling(bool cancel_ongoing)
        throws Matrix.Error;

        public void emit_login_finished(bool success);

        public void incoming_event(string? room_id, Json.Node raw_event, Matrix.Event.Base? matrix_event);

        public void
        connect_event(GLib.Type event_gtype, owned EventCallback event_callback);

        public abstract Profile?
            get_user_profile(string user_id, string? room_id = null)
        throws Matrix.Error;

        public abstract Presence
        get_user_presence(string user_id, string? room_id = null)
        throws Matrix.Error;

        public abstract Room
        get_room_by_id(string room_id)
        throws Matrix.Error;

        public abstract Room
        get_room_by_alias(string room_alias)
        throws Matrix.Error;

        public delegate void
        SendCallback(string? event_id, GLib.Error? err);

        public abstract void
        send(string room_id,
             Matrix.Event.Base evt,
             SendCallback? cb,
             out ulong txn_id)
        throws Matrix.Error;

        public abstract void
        save_state(string filename)
        throws Matrix.Error, GLib.Error;

        public abstract void
        load_state(string filename)
        throws Matrix.Error, GLib.Error;
    }

    [CCode (cheader_filename = "matrix-http-client.h")]
    public class HTTPClient : HTTPAPI, Client {
        public HTTPClient(string base_url);

        public void login_with_password(string username, string password)
            throws Matrix.Error;

        public void register_with_password(string? username, string password)
        throws Matrix.Error;

        public new void logout()
        throws Matrix.Error;

        public void begin_polling()
        throws Matrix.Error;

        public void stop_polling(bool cancel_ongoing)
        throws Matrix.Error;

        public Profile? get_user_profile(string user_id, string? room_id = null)
        throws Matrix.Error;

        public Presence get_user_presence(string user_id, string? room_id = null)
        throws Matrix.Error;

        public Room get_room_by_id(string room_id)
        throws Matrix.Error;

        public Room get_room_by_alias(string room_alias)
        throws Matrix.Error;

        public ulong next_txn_id();

        public void send(string room_id, Matrix.Event.Base evt, Matrix.Client.SendCallback? cb, out ulong txn_id)
        throws Matrix.Error;

        public void save_state(string filename)
        throws Matrix.Error, GLib.Error;

        public void load_state(string filename)
        throws Matrix.Error, GLib.Error;
    }

    [CCode (cheader_filename = "matrix-version.h", cname = "MATRIX_GLIB_MAJOR_VERSION")]
    public const int GLIB_MAJOR_VERSION;

    [CCode (cheader_filename = "matrix-version.h", cname = "MATRIX_GLIB_MINOR_VERSION")]
    public const int GLIB_MINOR_VERSION;

    [CCode (cheader_filename = "matrix-version.h", cname = "MATRIX_GLIB_MICRO_VERSION")]
    public const int GLIB_MICRO_VERSION;

    [CCode (cheader_filename = "matrix-version.h", cname = "matrix_glib_check_version")]
    public bool glib_check_version(uint required_major,
                                   uint required_minor,
                                   uint required_micro);

    [CCode (gir_namespace = "MatrixEvent", gir_version = "0.0")]
    namespace Event {
        [CCode (cheader_filename = "matrix-event-base.h")]
        public abstract class Base : GLib.Object, GLib.Initable {
            protected string? _event_type;
            public string? event_type { get; construct; default = null; }
            public Json.Node? json { get; set; default = null; }

            public Base();

            public bool init(GLib.Cancellable? cancellable = null)
                throws Error, Matrix.Error;

            public virtual void from_json(Json.Node json_data)
                throws Matrix.Error;

            public virtual void to_json(Json.Node json_data)
                throws Matrix.Error;

            public static Base? new_from_json(owned string? event_type = null, Json.Node? json_data = null)
                throws Matrix.Error, GLib.Error;
        }

        [CCode (cheader_filename = "matrix-event-base.h")]
        public static GLib.Type? get_handler(string event_type);

        [CCode (cheader_filename = "matrix-event-base.h")]
        public static void register_type(string event_type, GLib.Type event_gtype)
            throws Matrix.Error;

        [CCode (cheader_filename = "matrix-event-base.h")]
        public static void unregister_type(string event_type);

        [CCode (cheader_filename = "matrix-event-room-base.h")]
        public abstract class Room : Matrix.Event.Base {
            public string? event_id { get; set; default = null; }
            public string? room_id { get; set; default = null; }
            public string? sender { get; set; default = null; }
            public long age { get; set; default = 0; }
            public string? redacted_because { get; set; default = null; }
            public string? transaction_id { get; set; default = null; }

            public Room();

            protected override void from_json(Json.Node json_data)
                throws Matrix.Error;

            protected override void to_json(Json.Node json_data)
                throws Matrix.Error;
        }

        [CCode (cheader_filename = "matrix-event-state-base.h")]
        public abstract class State : Matrix.Event.Room {
            protected string? _state_key;
            public string? state_key { get; set; default = null; }
            public Json.Node? prev_content { get; set; default = null; }

            public State();

            protected override void from_json(Json.Node json_data)
            throws Matrix.Error;

            protected override void to_json(Json.Node json_node)
            throws Matrix.Error;

            public Json.Node? get_stripped_node();
        }

        [CCode (cheader_filename = "matrix-event-tag.h")]
        public class Tag : Base {
            protected override void from_json(Json.Node json_data)
                throws Matrix.Error;

            protected override void to_json(Json.Node json_data)
                throws Matrix.Error;
        }

        [CCode (cheader_filename = "matrix-event-presence.h")]
        public class Presence : Base {
            public string? avatar_url { get; set; }
            public string? display_name { get; set; }
            public long last_active_ago { get; set; default = -1; }
            public string? user_id { get; set; default = null; }
            public string? event_id { get; set; default = null; }
            public Matrix.Presence presence { get; set; default = Matrix.Presence.UNKNOWN; }

            protected override void from_json(Json.Node json_data)
                throws Matrix.Error;

            protected override void to_json(Json.Node json_data)
                throws Matrix.Error;
        }

        [CCode (cheader_filename = "matrix-event-room-member.h")]
        public class RoomMember : State {
            public RoomMembership membership { get; set; default = RoomMembership.UNKNOWN; }
            public string? avatar_url { get; set; default = null; }
            public string? display_name { get; set; default = null; }
            public string? tpi_display_name { get; set; default = null; }
            public string? tpi_signed_mxid { get; set; default = null; }
            public string? tpi_signed_token { get; set; default = null; }
            public Json.Node? tpi_signature { get; set; default = null; }
            public Matrix.Event.State[] invite_room_state { get; set; }
            public string? user_id { get; set; default = null; }

            protected override void
            from_json(Json.Node json_data)
                throws Matrix.Error;

            protected override void
            to_json(Json.Node json_data)
                throws Matrix.Error;
        }

        [CCode (cheader_filename = "matrix-event-typing.h")]
        public class Typing : Base {
            public string? room_id { get; set; default = null; }
            public string[] user_ids { get; set; }

            protected override void from_json(Json.Node json_data)
                throws Matrix.Error;

            protected override void to_json(Json.Node json_data)
                throws Matrix.Error;
        }

        [CCode (cheader_filename = "matrix-event-room-topic.h")]
        public class RoomTopic : State {
            public string? topic { get; set; default = null; }

            protected override void from_json(Json.Node json_data)
            throws Matrix.Error;

            protected override void to_json(Json.Node json_data)
            throws Matrix.Error;
        }

        [CCode (cheader_filename = "matrix-event-room-aliases.h")]
        public class RoomAliases : State {
            public string[] aliases { get; set; }

            protected override void from_json(Json.Node json_data)
            throws Matrix.Error;

            protected override void to_json(Json.Node json_data)
            throws Matrix.Error;
        }

        [CCode (cheader_filename = "matrix-event-room-avatar.h")]
        public class RoomAvatar : State {
            public string? url { get; set; default = null; }
            public string? thumbnail_url { get; set; default = null; }
            public ImageInfo? info { get; set; default = null; }
            public ImageInfo? thumbnail_info { get; set; default = null; }

            protected override void
            from_json(Json.Node json_data)
                throws Matrix.Error;

            protected override void
            to_json(Json.Node json_data)
                throws Matrix.Error;
        }

        [CCode (cheader_filename = "matrix-event-room-name.h")]
        public class RoomName : State {
            public string? name { get; set; default = null; }

            protected override void
            from_json(Json.Node json_data)
                throws Matrix.Error;

            protected override void
            to_json(Json.Node json_data)
                throws Matrix.Error;
        }

        [CCode (cheader_filename = "matrix-event-call-base.h")]
        public abstract class Call : Room {
            public string? call_id { get; set; default = null; }
            public int version { get; set; default = -1; }

            public Call();

            protected override void
            from_json(Json.Node json_data)
                throws Matrix.Error;

            protected override void
            to_json(Json.Node json_data)
                throws Matrix.Error
            {
            }
        }

        [CCode (cheader_filename = "matrix-event-room-message.h")]
        public class RoomMessage : Room {
            public Matrix.Message.Base? message { get; set; default = null; }
            public Json.Node? fallback_content { get; }

            protected override void from_json(Json.Node json_data)
            throws Matrix.Error;

            protected override void to_json(Json.Node json_data)
            throws Matrix.Error;
        }

        [CCode (cheader_filename = "matrix-event-room-history-visibility.h")]
        public class RoomHistoryVisibility : State {
            public Matrix.HistoryVisibility visibility { get; set; default = Matrix.HistoryVisibility.UNKNOWN; }

            protected override void from_json(Json.Node json_data)
                throws Matrix.Error;

            protected override void to_json(Json.Node json_data)
                throws Matrix.Error;
        }

        [CCode (cheader_filename = "matrix-event-room-join-rules.h")]
        public class RoomJoinRules : State {
            public Matrix.JoinRules join_rules { get; set; default = Matrix.JoinRules.UNKNOWN; }

            protected override void from_json(Json.Node json_data)
                throws Matrix.Error;

            protected override void to_json(Json.Node json_data)
                throws Matrix.Error;
        }

        [CCode (cheader_filename = "matrix-event-call-answer.h")]
        public class CallAnswer : Call {
            public Matrix.CallAnswerType answer_type { get; set; default = Matrix.CallAnswerType.UNKNOWN; }
            public string? answer_sdp { get; set; default = null; }

            protected override void from_json(Json.Node json_data)
                throws Matrix.Error;

            protected override void to_json(Json.Node json_data)
                throws Matrix.Error;
        }

        [CCode (cheader_filename = "matrix-event-call-hangup.h")]
        public class CallHangup : Call {
            protected override void from_json(Json.Node json_data)
                throws Matrix.Error;

            protected override void to_json(Json.Node json_data)
                throws Matrix.Error;
        }

        [CCode (cheader_filename = "matrix-event-call-invite.h")]
        public class CallInvite : Call {
            public Matrix.CallOfferType offer_type { get; set; default = Matrix.CallOfferType.UNKNOWN; }
            public string? sdp { get; set; default = null; }
            public int lifetime { get; set; default = -1; }

            protected override void from_json(Json.Node json_data)
                throws Matrix.Error;

            protected override void to_json(Json.Node json_data)
                throws Matrix.Error;
        }

        [CCode (cheader_filename = "matrix-event-room-canonical-alias.h")]
        public class RoomCanonicalAlias : State {
            public string? canonical_alias { get; set; default = null; }

            protected override void from_json(Json.Node json_data)
                throws Matrix.Error;

            protected override void to_json(Json.Node json_data)
                throws Matrix.Error;
        }

        [CCode (cheader_filename = "matrix-event-room-create.h")]
        public class RoomCreate : State {
            public string? creator { get; set; default = null; }
            public bool federate { get; set; default = false; }

            protected override void from_json(Json.Node json_data)
            throws Matrix.Error;

            protected override void
            to_json(Json.Node json_data)
            throws Matrix.Error;
        }

        [CCode (cheader_filename = "matrix-event-room-power-levels.h")]
        public class RoomPowerLevels : State {
            public int users_default { get; set; default = 0; }
            public int events_default { get; set; default = 0; }
            public int state_default { get; set; default = 10; }
            public int ban { get; set; default = 5; }
            public int kick { get; set; default = 5; }
            public int redact { get; set; default = 20; }
            public int invite { get; set; default = 0; }
            public GLib.HashTable<string, int?> event_levels { get; }
            public GLib.HashTable<string, int?> user_levels { get; }

            protected override void from_json(Json.Node json_data)
            throws Matrix.Error;

            protected override void to_json(Json.Node json_data)
            throws Matrix.Error;

            public void set_user_level(string user_id, int level);

            public void set_event_level(string event_type, int level);
        }

        [CCode (cheader_filename = "matrix-event-room-message-feedback.h")]
        public class RoomMessageFeedback : Room {
            public string? feedback_type { get; set; default = null; }
            public string? target_event_id { get; set; default = null; }

            protected override void from_json(Json.Node json_data)
            throws Matrix.Error;

            protected override void to_json(Json.Node json_data)
            throws Matrix.Error;
        }

        [CCode (cheader_filename = "matrix-event-room-guest-access.h")]
        public class RoomGuestAccess : State {
            public Matrix.GuestAccess guest_access { get; set; default = Matrix.GuestAccess.UNKNOWN; }

            protected override void from_json(Json.Node json_data)
                throws Matrix.Error;

            protected override void to_json(Json.Node json_data)
                throws Matrix.Error;
        }

        [CCode (cheader_filename = "matrix-event-room-redaction.h")]
        public class RoomRedaction : Room {
            public string? reason { get; set; default = null; }
            public string? redacted_event_id { get; set; default = null; }

            protected override void from_json(Json.Node json_data)
            throws Matrix.Error;

            protected override void to_json(Json.Node json_data)
            throws Matrix.Error;
        }

        [CCode (cheader_filename = "matrix-event-room-third-party-invite.h")]
        public class RoomThirdPartyInvite : State {
            public struct PublicKey {
                string? key;
                string? validity_url;
            }

            public string? display_name { get; set; default = null; }
            public string? key_validity_url { get; set; default = null; }
            public string? public_key { get; set; default = null;}
            public Matrix.ThirdPartyInvitePublicKey[] public_keys { get; set; }
            public string? token { get; set; default = null;}

            protected override void from_json(Json.Node json_data)
            throws Matrix.Error;

            protected override void to_json(Json.Node json_data)
            throws Matrix.Error;
        }

        [CCode (cheader_filename = "matrix-event-call-candidates.h")]
        public class CallCandidates : Call {
            public Matrix.CallCandidate[] candidates { get; set; }

            protected override void from_json(Json.Node json_data)
                throws Matrix.Error;

            protected override void to_json(Json.Node json_data)
                throws Matrix.Error;
        }
    }

    [CCode (gir_namespace = "MatrixMessage", gir_version = "0.0")]
    namespace Message {
        [CCode (cname = "matrix_message_get_handler")]
        public static GLib.Type? get_handler(string message_type);

        [CCode (cname = "matrix_message_register_type")]
        public static void register_type(string message_type, GLib.Type message_gtype)
            throws Matrix.Error;

        [CCode (cname = "matrix_message_unregister_type")]
        public void unregister_type(string message_type);

        [CCode (cheader_filename = "matrix-message-base.h")]
        public abstract class Base : GLib.Object, GLib.Initable {
            public string? message_type { get; set; default = null; }
            public string? body { get; set; default = null; }
            public Json.Node? json { get; construct; }

            public Base();

            public bool init(GLib.Cancellable? cancellable = null)
                throws GLib.Error;

            private void initialize_from_json(Json.Node json_data)
                throws Matrix.Error;

            public static Matrix.Message.Base? new_from_json(Json.Node json_data)
                throws Matrix.Error, GLib.Error;

            public virtual void from_json(Json.Node json_data)
                throws Matrix.Error;

            public virtual void to_json(Json.Node json_data)
                throws Matrix.Error;
        }

        [CCode (cheader_filename = "matrix-message-text.h")]
        public class Text : Base {
            public override void from_json(Json.Node json_data)
                throws Matrix.Error;

            public override void to_json(Json.Node json_data)
                throws Matrix.Error;
        }

        [CCode (cheader_filename = "matrix-message-location.h")]
        public class Location : Base {
            public string? geo_uri { get; set; default = null; }
            public string? thumbnail_url { get; set; default = null; }
            public Matrix.ImageInfo? thumbnail_info { get; set; default = null; }

            protected override void
            from_json(Json.Node json_data)
                throws Matrix.Error;

            protected override void
            to_json(Json.Node json_data)
                throws Matrix.Error;
        }

        [CCode (cheader_filename = "matrix-message-emote.h")]
        public class Emote : Base {
            public override void from_json(Json.Node json_data)
                throws Matrix.Error;

            public override void to_json(Json.Node json_data)
                throws Matrix.Error;
        }

        [CCode (cheader_filename = "matrix-message-file.h")]
        public class File : Base {
            public string? filename { get; set; default = null; }
            public string? url { get; set; default = null; }
            public Matrix.FileInfo? info { get; set; default = null; }
            public string? thumbnail_url {get; set; default = null; }
            public Matrix.ImageInfo? thumbnail_info { get; set; default = null; }

            protected override void
            from_json(Json.Node json_data)
                throws Matrix.Error;

            protected override void
            to_json(Json.Node json_data)
                throws Matrix.Error;
        }

        [CCode (cheader_filename = "matrix-message-image.h")]
        public class Image : Base {
            public string? url { get; set; default = null; }
            public string? thumbnail_url { get; set; default = null; }
            public Matrix.ImageInfo? info { get; set; default = null; }
            public Matrix.ImageInfo? thumbnail_info { get; set; default = null; }

            protected override void
            from_json(Json.Node json_data)
            throws Matrix.Error;

            protected override void
            to_json(Json.Node json_data)
            throws Matrix.Error;
        }

        [CCode (cheader_filename = "matrix-message-audio.h")]
        public class Audio : Base {
            public string? url { get; set; default = null; }
            public Matrix.AudioInfo? info { get; set; default = null; }

            protected override void
            from_json(Json.Node json_data)
            throws Matrix.Error;

            protected override void
            to_json(Json.Node json_data)
            throws Matrix.Error;
        }

        [CCode (cheader_filename = "matrix-message-video.h")]
        public class Video : Base {
            public string? url { get; set; default = null; }
            public Matrix.VideoInfo? info { get; set; default = null; }

            protected override void
            from_json(Json.Node json_data)
            throws Matrix.Error;

            protected override void
            to_json(Json.Node json_data)
            throws Matrix.Error;
        }

        [CCode (cheader_filename = "matrix-message-notice.h")]
        public class Notice : Base {
            public override void
            from_json(Json.Node json_data)
                throws Matrix.Error;

            public override void
            to_json(Json.Node json_data)
                throws Matrix.Error;
        }
    }
}
