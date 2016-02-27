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
     * Abstract parent class for classes that can be saved to JSON.
     */
    public abstract class JsonCompact {
        public abstract Json.Node?
        get_json_node()
            throws Matrix.Error;

        public string
        get_json_data(out size_t datalen)
            throws Matrix.Error
        {
            var generator = new Json.Generator();

            generator.set_root(get_json_node());

            return generator.to_data(out datalen);
        }
    }
}
