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

/**
 * Class to hold a m.room.avatar event
 *
 * A picture that is associated with the room. This can be displayed
 * alongside the room information.
 */
public class Matrix.Event.RoomAvatar : Matrix.Event.State {
    /**
     * The URL to the image.
     */
    public string? url { get; set; default = null; }

    /**
     * The URL to the thumbnail of the image.
     */
    public string? thumbnail_url { get; set; default = null; }

    /**
     * Metadata about the image referred to in url.
     */
    public ImageInfo? info { get; set; default = null; }

    /**
     * Metadata about the image referred to in thumbnail_url.
     */
    public ImageInfo? thumbnail_info { get; set; default = null; }

    protected override void
    from_json(Json.Node json_data)
        throws Matrix.Error
    {
        var content_root = json_data.get_object()
            .get_member("content").get_object();
        Json.Node? node;

        if ((node = content_root.get_member("url")) != null) {
            _url = node.get_string();
        } else {
            warning("content.url is missing from a m.room.avatar event");
        }

        if ((node = content_root.get_member("thumbnail_url")) != null) {
            _thumbnail_url = node.get_string();
        }

        if ((node = content_root.get_member("info")) != null) {
            var info_obj = node.get_object();
            _info = ImageInfo();

            if ((node = info_obj.get_member("size")) != null) {
                _info.size = (int)node.get_int();
            } else {
                warning("content.info.size is missing from a m.room.avatar event");
            }

            if ((node = info_obj.get_member("h")) != null) {
                _info.height = (int)node.get_int();
            } else {
                warning("content.info.h is missing from a m.room.avatar event");
            }

            if ((node = info_obj.get_member("w")) != null) {
                _info.width = (int)node.get_int();
            } else {
                warning("content.info.w is missing from a m.room.avatar event");
            }

            if ((node = info_obj.get_member("mimetype")) != null) {
                _info.mimetype = node.get_string();
            } else {
                warning("content.info.mimetype is missing from a m.room.avatar event");
            }
        }

        if ((node = content_root.get_member("thumbnail_info")) != null) {
            var thumbnail_info_obj = node.get_object();
            _thumbnail_info = ImageInfo();

            if ((node = thumbnail_info_obj.get_member("size")) != null) {
                _thumbnail_info.size = (int)node.get_int();
            } else {
                warning("content.thumbnail_info.size is missing from a m.room.avatar event");
            }

            if ((node = thumbnail_info_obj.get_member("h")) != null) {
                _thumbnail_info.height = (int)node.get_int();
            } else {
                warning("content.thumbnail_info.h is missing from a m.room.avatar event");
            }

            if ((node = thumbnail_info_obj.get_member("w")) != null) {
                _thumbnail_info.width = (int)node.get_int();
            } else {
                warning("content.thumbnail_info.w is missing from a m.room.avatar event");
            }

            if ((node = thumbnail_info_obj.get_member("mimetype")) != null) {
                _thumbnail_info.mimetype = node.get_string();
            } else {
                warning("content.thumbnail_info.mimetype is missing from a m.room.avatar event");
            }
        }

        base.from_json(json_data);
    }

    protected override void
    to_json(Json.Node json_data)
        throws Matrix.Error
    {
        if (_url == null) {
            throw new Matrix.Error.INCOMPLETE(
                    "Won't generate a m.room.avatar event without url");
        }

        var content_root = json_data.get_object()
            .get_member("content").get_object();

        content_root.set_string_member("url", _url);

        if (_thumbnail_url != null) {
            content_root.set_string_member("thumbnail_url", _thumbnail_url);
        }

        if (_info != null) {
            content_root.set_member("info", _info.get_json_node());
        }

        if (_thumbnail_info != null) {
            content_root.set_member("thumbnail_info",
                                    _thumbnail_info.get_json_node());
        }

        base.to_json(json_data);
    }
}
