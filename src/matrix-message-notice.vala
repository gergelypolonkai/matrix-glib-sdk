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
 * Message type to hold a m.notice message
 *
 * A m.notice message should be considered similar to a plain m.text
 * message except that clients should visually distinguish it in some
 * way. It is intended to be used by automated clients, such as bots,
 * bridges, and other entities, rather than humans. Additionally, such
 * automated agents which watch a room for messages and respond to
 * them ought to ignore m.notice messages. This helps to prevent
 * infinite-loop situations where two automated clients continuously
 * exchange messages, as each responds to the other.
 */
public class Matrix.Message.Notice : Matrix.Message.Base {
    public override void
    from_json(Json.Node json_data)
        throws Matrix.Error
    {
        base.from_json(json_data);
    }

    public override void
    to_json(Json.Node json_data)
        throws Matrix.Error
    {
        base.to_json(json_data);
    }
}
