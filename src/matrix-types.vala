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
