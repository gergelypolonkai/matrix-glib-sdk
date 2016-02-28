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
 * Base interface for client communication with a Matrix.org
 * homeserver
 */
public interface Matrix.Client : GLib.Object {
    /**
     * This signal is a sign for a finished login request.
     *
     * Implementations are responsible for emitting this signal when
     * they get a response for a login request.
     *
     * @param success if %TRUE, login was successful
     */
    public virtual signal void
    login_finished(bool success)
    {}

    /**
     * Authenticate with the Matrix.org server with a username and
     * password.
     *
     * @param username the username to login with
     * @param password the password to use
     */
    public abstract void
    login_with_password(string username,
                        string password)
        throws Matrix.Error;

    /**
     * Register @username with the homeserver as a normal user.
     *
     * Upon success, the user is registered and authenticated.
     *
     * Implementations must emit the login-finished signal when a
     * response arrives.
     *
     * This method registers a normal user account. If you want to
     * register a different kind of user, use
     * matrix_api_register_account().
     *
     * @param username the username to register. If omitted, the
     *                 server will generate one
     * @param password the password to use with the registration
     */
    public abstract void
    register_with_password(string? username,
                           string password)
        throws Matrix.Error;

    /**
     * Logout from the homeserver. As Matrix.org doesn’t have such a
     * concept, this cancels all ongoing requests and clears the
     * authentication data (e.g. tokens).
     */
    public abstract void
    logout()
        throws Matrix.Error;

    /**
     * Begin polling the event stream.
     */
    public abstract void
    begin_polling()
        throws Matrix.Error;

    /**
     * Stop polling the event stream. If @param cancel_ongoing is
     * {{{true}}}, ongoing requests will be cancelled, too.
     *
     * @param cancel_ongoing if {{{true}}}, ongoing requests will be
     *                       cancelled, too
     */
    public abstract void
    stop_polling(bool cancel_ongoing)
        throws Matrix.Error;

    /**
     * Convenience function to emits the login-finished signal.
     *
     * @param success set to {{{true}}} if login was successful
     */
    public void
    emit_login_finished(bool success)
    {
        login_finished(success);
    }
}
