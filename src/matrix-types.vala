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

namespace Matrix {
    /**
     * Room membership types.
     */
    public enum RoomMembership {
        /**
         * the membership sent by the server is unknown to this SDK
         */
        UNKNOWN,

        /**
         * the user has been invited to join a room, but has not yet
         * joined it. They may not participate in the room until they
         * join
         */
        INVITE,

        /**
         * the user has joined the room (possibly after accepting an
         * invite), and may participate in it
         */
        JOIN,

        /**
         * the user was once joined to the room, but has since left
         * (possibly by choice, or possibly by being kicked)
         */
        LEAVE,

        /**
         * the user has been banned from the room, and is no longer
         * allowed to join it until they are un-banned from the room
         * (by having their membership state set to a value other than
         * {@link Matrix.RoomMembership.BAN})
         */
        BAN,

        /**
         * this is a reserved word, which currently has no meaning
         */
        KNOCK;
    }

    /**
     * Preset values for matrix_api_create_room() calls.
     */
    public enum RoomPreset {
        /**
         * no preset
         */
        NONE,

        /**
         * preset for private rooms
         */
        PRIVATE,

        /**
         * same as private rooms, but all users get the same power
         * level as the room creator
         */
        TRUSTED_PRIVATE,

        /**
         * preset for public rooms
         */
        PUBLIC;
    }

    /**
     * Visibility values for room creation. Not to be confused with
     * join rules.
     */
    public enum RoomVisibility {
        /**
         * use a server-assigned value (usually
         * {@link Matrix.RoomVisibility.PRIVATE})
         */
        DEFAULT,

        /**
         * make the room visible in the public room list
         */
        PUBLIC,

        /**
         * hide the room from the public room list
         */
        PRIVATE;
    }

    /**
     * Room join rules.
     */
    public enum JoinRules {
        /**
         * a value unknown to this library
         */
        UNKNOWN,

        /**
         * anyone can join
         */
        PUBLIC,

        /**
         * users may join upon invite
         */
        INVITE,

        /**
         * reserved word, not usable yet
         */
        PRIVATE,

        /**
         * reserved word, not usable yet
         */
        KNOCK;
    }

    /**
     * Search ordering.
     */
    public enum SearchOrder {
        /**
         * order messages as they arrived
         */
        RECENT,

        /**
         * order messages by relevance
         */
        RANK;
    }

    /**
     * Search keys.
     */
    public enum SearchKey {
        /**
         * search in the body of a message
         */
        CONTENT_BODY,

        /**
         * search in the name of rooms
         */
        CONTENT_NAME,

        /**
         * search in the topic of rooms
         */
        CONTENT_TOPIC;
    }

    /**
     * Search grouping
     *
     * The client can request that the results are returned along with
     * grouping information, e.g. grouped by room_id. In this case the
     * response will contain a group entry for each distinct value of
     * room_id. Each group entry contains at least a list of the
     * event_ids that are in that group, as well as potentially other
     * metadata about the group.
     */
    public enum SearchGroupBy {
        /**
         * no grouping
         */
        NONE,

        /**
         * group by room ID
         */
        ROOM_ID,

        /**
         * group by sender
         */
        SENDER;
    }

    /**
     * Room history visibility
     */
    public enum HistoryVisibility {
        /**
         * represents a value unknown to this library
         */
        UNKNOWN,

        /**
         * only room members can see the room history, and only what
         * happened after they got an invitation
         */
        INVITED,

        /**
         * only room members can see the room history, and only what
         * happened after they joined
         */
        JOINED,

        /**
         * only room members can see the room history, but they see
         * all of it
         */
        SHARED,

        /**
         * anyone can see the room history
         */
        WORLD_READABLE;
    }

    /**
     * Room guest access
     */
    public enum GuestAccess {
        /**
         * represents a value unknown to this library
         */
        UNKNOWN,

        /**
         * guest users are allowed to access the room
         */
        CAN_JOIN,

        /**
         * guest users are not allowed to access the room
         */
        FORBIDDEN;
    }

    /**
     * Call offer types
     */
    public enum CallOfferType {
        /**
         * represents a value unknown to this library
         */
        UNKNOWN,

        /**
         * call offer
         */
        OFFER;
    }

    /**
     * Call answer types
     */
    public enum CallAnswerType {
        /**
         * represents a value unknown to this library
         */
        UNKNOWN,

        /**
         * call answer
         */
        ANSWER;
    }

    /**
     * Information about the file referred to in a URL.
     */
    public struct FileInfo {
        int? size;
        string? mimetype;

        public void
        set_from_json(Json.Node json_data)
        {
            size = null;
            mimetype = null;

            var root = json_data.get_object();
            Json.Node? node;

            if ((node = root.get_member("size")) != null) {
                size = (int)node.get_int();
            } else if (Config.DEBUG) {
                warning("size is missing from a FileInfo");
            }

            if ((node = root.get_member("mimetype")) != null) {
                mimetype = node.get_string();
            } else if (Config.DEBUG) {
                warning("mimetype is missing from a FileInfo");
            }
        }

        public Json.Node
        get_json_node()
            throws Matrix.Error
        {
            if ((size == null)
                || (mimetype == null)) {
                throw new Matrix.Error.INCOMPLETE(
                        "Won't generate a FileInfo without all fields set.");
            }

            var node = new Json.Node(Json.NodeType.OBJECT);
            var obj = new Json.Object();
            node.set_object(obj);

            obj.set_int_member("size", size);
            obj.set_string_member("mimetype", mimetype);

            return node;
        }
    }

    public struct ImageInfo {
        int? size;
        int? height;
        int? width;
        string? mimetype;

        public void
        set_from_json(Json.Node json_data)
        {
            size = null;
            mimetype = null;
            height = null;
            width = null;

            var root = json_data.get_object();
            Json.Node? node;

            if ((node = root.get_member("w")) != null) {
                width = (int)node.get_int();
            } else if (Config.DEBUG) {
                warning("w is missing from an ImageInfo");
            }

            if ((node = root.get_member("h")) != null) {
                height = (int)node.get_int();
            } else if (Config.DEBUG) {
                warning("h is missing from an ImageInfo");
            }

            if ((node = root.get_member("size")) != null) {
                size = (int)node.get_int();
            } else if (Config.DEBUG) {
                warning("size is missing from an ImageInfo");
            }

            if ((node = root.get_member("mimetype")) != null) {
                mimetype = node.get_string();
            } else if (Config.DEBUG) {
                warning("mimetype is missing from an ImageInfo");
            }
        }

        public Json.Node
        get_json_node()
            throws Matrix.Error
        {
            if ((size == null)
                || (height == null)
                || (width == null)
                || (mimetype == null)) {
                throw new Matrix.Error.INCOMPLETE(
                        "Won't generate an ImageInfo without all fields set.");
            }

            var node = new Json.Node(Json.NodeType.OBJECT);
            var obj = new Json.Object();
            node.set_object(obj);

            obj.set_int_member("size", size);
            obj.set_int_member("h", height);
            obj.set_int_member("w", width);
            obj.set_string_member("mimetype", mimetype);

            return node;
        }

        /**
         * Check if two ImageInfo objects are identical.
         *
         * @param other the ImageInfo object to compare with
         * @return true if the two objects hold different values
         */
        public bool
        differs(ImageInfo other)
        {
            return ((size != other.size)
                    || (height != other.height)
                    || (width != other.width)
                    || (mimetype != other.mimetype));
        }
    }

    public struct AudioInfo {
        int? size;
        string? mimetype;
        int? duration;

        public void
        set_from_json(Json.Node json_data)
        {
            size = null;
            mimetype = null;
            duration = null;

            var root = json_data.get_object();
            Json.Node? node;

            if ((node = root.get_member("size")) != null) {
                size = (int)node.get_int();
            } else if (Config.DEBUG) {
                warning("size is missing from an ImageInfo");
            }

            if ((node = root.get_member("mimetype")) != null) {
                mimetype = node.get_string();
            } else if (Config.DEBUG) {
                warning("mimetype is missing from an ImageInfo");
            }

            if ((node = root.get_member("duration")) != null) {
                duration = (int)node.get_int();
            } else if (Config.DEBUG) {
                warning("duration is missing from an ImageInfo");
            }
        }

        public Json.Node
        get_json_node()
            throws Matrix.Error
        {
            if ((size == null)
                || (mimetype == null)
                || (duration == null)) {
                throw new Matrix.Error.INCOMPLETE(
                        "Won't generate an ImageInfo without all fields set.");
            }

            var node = new Json.Node(Json.NodeType.OBJECT);
            var obj = new Json.Object();
            node.set_object(obj);

            obj.set_int_member("size", size);
            obj.set_string_member("mimetype", mimetype);
            obj.set_int_member("duration", duration);

            return node;
        }
    }

    public struct VideoInfo {
        int? size;
        string? mimetype;
        int? duration;
        int? width;
        int? height;
        string? thumbnail_url;
        ImageInfo? thumbnail_info;

        public void
        set_from_json(Json.Node json_data)
        {
            size = null;
            mimetype = null;
            duration = null;

            var root = json_data.get_object();
            Json.Node? node;

            if ((node = root.get_member("size")) != null) {
                size = (int)node.get_int();
            } else if (Config.DEBUG) {
                warning("size is missing from an VideoInfo");
            }

            if ((node = root.get_member("mimetype")) != null) {
                mimetype = node.get_string();
            } else if (Config.DEBUG) {
                warning("mimetype is missing from an VideoInfo");
            }

            if ((node = root.get_member("duration")) != null) {
                duration = (int)node.get_int();
            } else if (Config.DEBUG) {
                warning("duration is missing from an VideoInfo");
            }

            if ((node = root.get_member("w")) != null) {
                width = (int)node.get_int();
            } else if (Config.DEBUG) {
                warning("w is missing from a VideoInfo");
            }

            if ((node = root.get_member("h")) != null) {
                height = (int)node.get_int();
            } else if (Config.DEBUG) {
                warning("h is missing from a VideoInfo");
            }

            if ((node = root.get_member("thumbnail_url")) != null) {
                thumbnail_url = node.get_string();
            }

            if ((node = root.get_member("thumbnail_info")) != null) {
                thumbnail_info = ImageInfo();
                thumbnail_info.set_from_json(node);
            }
        }

        public Json.Node
        get_json_node()
            throws Matrix.Error
        {
            if ((size == null)
                || (mimetype == null)
                || (duration == null)
                || (width == null)
                || (height == null)
                || (thumbnail_url == null)
                || (thumbnail_info == null)) {
                throw new Matrix.Error.INCOMPLETE(
                        "Won't generate an ImageInfo without all fields set.");
            }

            var node = new Json.Node(Json.NodeType.OBJECT);
            var obj = new Json.Object();
            node.set_object(obj);

            obj.set_int_member("size", size);
            obj.set_string_member("mimetype", mimetype);
            obj.set_int_member("duration", duration);
            obj.set_int_member("w", width);
            obj.set_int_member("h", height);
            obj.set_string_member("thumbnail_url", thumbnail_url);
            obj.set_member("thumbnail_info", thumbnail_info.get_json_node());

            return node;
        }
    }

    private int?
    _g_enum_nick_to_value(Type enum_type, string nick)
    {
        EnumClass enum_class = (EnumClass)enum_type.class_ref();
        unowned EnumValue? enum_val = enum_class.get_value_by_nick(nick);

        if (enum_val != null) {
            return enum_val.value;
        } else {
            return null;
        }
    }

    private string?
    _g_enum_value_to_nick(Type enum_type,
                          int value,
                          bool convert_dashes = true)
    {
        EnumClass enum_class = (EnumClass)enum_type.class_ref();
        unowned EnumValue? enum_val = enum_class.get_value(value);

        if (enum_val != null) {
            var nick = enum_val.value_nick;

            if (convert_dashes) {
                return nick.replace("-", "_");
            }

            return nick;
        } else {
            return null;
        }
    }
}
