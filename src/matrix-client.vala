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
     * This signal is a sign of an incoming event. It gets emitted for
     * every signal, regardless if it is handled by other event
     * signals, before other signals.
     *
     * Implementations are responsible for emitting this signal when
     * any kind of event arrives from the event stream or the history.
     *
     * @param room_id the ID of the room associated with this event
     * @param raw_event the raw event as a JSON object
     * @param matrix_event the event as a {@link Matrix.Event}
     */
    [Signal (detailed=true)]
    public virtual signal void
    @event(string? room_id, Json.Node raw_event, Matrix.Event matrix_event)
    {}

    /**
     * Callback function delegate for the event signal.
     *
     * @param room_id the room the event associated with
     * @param raw_event the event as a raw JSON object
     * @param matrix_event the event as a Matrix.Event object
     */
    public delegate void
    EventCallback(Matrix.Client client,
                  string? room_id,
                  Json.Node raw_event,
                  Matrix.Event matrix_event);

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

    /**
     * Emits the #MatrixClient::event signal.
     *
     * @param room_id the room this event is associated with
     * @param raw_event the raw event
     * @param matrix_event the event as a Matrix.Event
     */
    public void
    incoming_event(string? room_id,
                   Json.Node raw_event,
                   Matrix.Event matrix_event)
    {
        Quark equark = matrix_event.get_type().qname();

        this.@event[equark.to_string()](room_id, raw_event, matrix_event);
    }

    /**
     * Connect a handler for events. If @param event_gtype is
     * Matrix.Event, all events will be sent to the callback function,
     * otherwise only events that match the specified event type.
     *
     * If @event_gtype is not derived from
     * {@link Matrix.Event}, @param callback won’t get connected.
     *
     * @param event_gtype the {@link GLib.Type} of a
     *                    {@link Matrix.Event} derivative
     * @param callback the allback function to connect
     */
    public extern void
    connect_event(GLib.Type event_gtype,
                  owned EventCallback event_callback);
}
