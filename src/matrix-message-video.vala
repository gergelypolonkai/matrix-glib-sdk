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
 * This message represents a single video clip.
 */
public class Matrix.Message.Video : Matrix.Message.Base {
    /**
     * The URL to the video clip.
     */
    public string? url { get; set; default = null; }

    /**
     * Metadata about the video clip referred to in `url`.
     */
    public VideoInfo? info { get; set; default = null; }

    protected override void
    from_json(Json.Node json_data)
        throws Matrix.Error
    {
        var root = json_data.get_object();
        Json.Node? node;

        if ((node = root.get_member("url")) != null) {
            _url = node.get_string();
        } else if (Config.DEBUG) {
            warning("url is missing from a m.video message");
        }

        if ((node = root.get_member("info")) != null) {
            _info = VideoInfo();
            _info.set_from_json(node);
        }

        base.from_json(json_data);
    }

    protected override void
    to_json(Json.Node json_data)
        throws Matrix.Error
    {
        if (_url == null) {
            throw new Matrix.Error.INCOMPLETE(
                    "Won't generate a m.video message without url");
        }

        var root = json_data.get_object();

        root.set_string_member("url", _url);

        if (_info != null) {
            root.set_member("info", _info.get_json_node());
        }

        base.to_json(json_data);
    }
}
