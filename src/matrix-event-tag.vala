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

public class Matrix.Event.Tag : Matrix.Event.Base {
    private HashTable<string, Json.Node>? _tags = null;

    protected override void
    from_json(Json.Node json_data)
        throws Matrix.Error
    {
        var content_root = json_data.get_object()
            .get_member("content").get_object();
        Json.Node? node;

        if ((node = content_root.get_member("tags")) != null) {
            var tags_root = node.get_object();

            tags_root.foreach_member((tobj, tag, tag_contents) => {
                    if (_tags == null) {
                        _tags = new HashTable<string, Json.Node>(
                                str_hash, str_equal);
                    }

                    _tags.replace(tag, tag_contents);
                });
        }

        base.from_json(json_data);
    }

    protected override void
    to_json(Json.Node json_data)
        throws Matrix.Error
    {
        if (_tags != null) {
            var tags_root = new Json.Object();

            _tags.foreach((tag, tag_contents) => {
                    tags_root.set_member(tag, tag_contents);
                });

            if (tags_root.get_size() > 0) {
                var content_root = json_data.get_object()
                    .get_member("content").get_object();
                var tags_node = new Json.Node(Json.NodeType.OBJECT);

                tags_node.set_object(tags_root);
                content_root.set_member("tags", tags_node);
            }
        }

        base.to_json(json_data);
    }
}
