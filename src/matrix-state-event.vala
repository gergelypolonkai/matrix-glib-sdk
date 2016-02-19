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
    public class StateEvent : Matrix.Event {
        public string? state_key { get; set; default = null; }
        public Json.Node? content { get; set; default = null; }

        protected override void
        from_json(Json.Node json_data)
            throws Matrix.Error
        {
            var root = json_data.get_object();
            Json.Node? node;

            if ((node = root.get_member("state_key")) != null) {
                state_key = node.get_string();
            }

            if ((node = root.get_member("content")) != null) {
                content = node;
            }

            base.from_json(json_data);
        }

        protected override void
        to_json(Json.Node json_node)
            throws Matrix.Error
        {
            var root = json_node.get_object();

            if (state_key != null) {
                root.set_string_member("state_key", state_key);
            }

            if (content != null) {
                root.set_member("content", content);
            }

            base.to_json(json_node);
        }
    }
}
