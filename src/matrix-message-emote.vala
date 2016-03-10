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
 * The emote message type
 *
 * This message is similar to `m.text` except that the sender is
 * 'performing' the action contained in the `body` key, similar to
 * `/me` in IRC. This message should be prefixed by the name of the
 * sender. This message could also be represented in a different
 * colour to distinguish it from regular `m.text` messages.
 */
public class Matrix.Message.Emote : Matrix.Message.Base {}
