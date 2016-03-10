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
 * Class to hold a m.file message
 *
 * This message represents a generic file.
 */
public class Matrix.Message.File : Matrix.Message.Base {
    /**
     * The original filename of the uploaded file.
     */
    public string? filename { get; set; default = null; }

    /**
     * The URL of the file.
     */
    public string? url { get; set; default = null; }

    /**
     * Information about the file referred to in ``url``.
     */
    public FileInfo? info { get; set; default = null; }

    /**
     * The URL of the thumbnail of the file.
     */
    public string? thumbnail_url {get; set; default = null; }

    /**
     * Metadata about the image referred to in `thumbnail_url`.
     */
    public ImageInfo? thumbnail_info { get; set; default = null; }

    protected override void
    from_json(Json.Node json_data)
        throws Matrix.Error
    {
        var root = json_data.get_object();
        Json.Node? node;

        if ((node = root.get_member("url")) != null) {
            _url = node.get_string();
        } else if (Config.DEBUG) {
            warning("url is missing from a m.file message");
        }

        if ((node = root.get_member("info")) != null) {
            _info = FileInfo();

            _info.set_from_json(node);
        } else if (Config.DEBUG) {
            warning("info is missing from a m.file message");
        }

        if ((node = root.get_member("thumbnail_url")) != null) {
            _thumbnail_url = node.get_string();
        }

        if ((node = root.get_member("thumbnail_info")) != null) {
            _thumbnail_info = ImageInfo();

            _thumbnail_info.set_from_json(node);
        }

        base.from_json(json_data);
    }

    protected override void
    to_json(Json.Node json_data)
        throws Matrix.Error
    {
        if (_url == null) {
            throw new Matrix.Error.INCOMPLETE(
                    "Won't generate a m.file message without the URL");
        }

        if (_info == null) {
            throw new Matrix.Error.INCOMPLETE(
                    "Won't generate a m.file message without the file info.");
        }

        var root = json_data.get_object();

        root.set_string_member("url", _url);
        root.set_member("info", _info.get_json_node());

        if (_thumbnail_url != null) {
            root.set_string_member("thumbnail_url", _thumbnail_url);
        }

        if (_thumbnail_info != null) {
            root.set_member("thumbnail_info", _thumbnail_info.get_json_node());
        }

        base.to_json(json_data);
    }
}
