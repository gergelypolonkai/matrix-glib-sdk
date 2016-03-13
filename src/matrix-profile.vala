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
 * Object to hold a user's profile
 *
 * This object is used for both global and room specific profiles.
 */
public class Matrix.Profile : Object {
    public TimeSpan age {
        get {
            return new DateTime.now_utc().difference(_last_update);
        }
    }

    /**
     * The human readable name of the user.
     */
    public string? display_name {
        get {
            return _display_name;
        }

        set {
            _display_name = value;
            _last_update = new DateTime.now_utc();
        }

        default = null;
    }

    /**
     * The URL of the user's avatar.
     */
    public string? avatar_url {
        get {
            return _avatar_url;
        }
        set {
            _avatar_url = value;
            _last_update = new DateTime.now_utc();
        }

        default = null;
    }

    private DateTime _last_update;
    private string? _display_name = null;
    private string? _avatar_url = null;
}
