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
 * Class to hold an m.typing event.
 *
 * Informs the client of the list of users currently typing.
 */
public class Matrix.Event.Typing : Matrix.Event.Base {
    private List<string>? _user_ids = null;

    public string? room_id { get; set; default = null; }
    public List<string>? user_ids {
        get {
            return _user_ids;
        }

        set {
            _user_ids = value.copy();
        }

        default = null;
    }
}
