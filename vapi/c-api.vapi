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
