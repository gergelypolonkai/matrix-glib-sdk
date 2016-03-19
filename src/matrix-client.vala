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
    @event(string? room_id, Json.Node raw_event, Matrix.Event.Base? matrix_event)
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
                  Matrix.Event.Base? matrix_event);

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
                   Matrix.Event.Base? matrix_event)
    {
        Quark equark;

        if (matrix_event == null) {
            equark = typeof(Matrix.Event.Base).qname();
        } else {
            equark = matrix_event.get_type().qname();
        }

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
     * @param event_callback the allback function to connect
     */
    public extern void
    connect_event(GLib.Type event_gtype,
                  owned EventCallback event_callback);

    /**
     * Get the profile of a user specified by @param user_id.
     * If @param room_id is not null, return the room-specific
     * profile. If the user's profile is not cached yet,
     * Matrix.Error.UNAVAILABLE is thrown.
     */
    public abstract Profile?
    get_user_profile(string user_id, string? room_id = null)
        throws Matrix.Error;

    /**
     * Get the presence state of a user specified
     * by @param user_id. If @param room_id is null, return
     * the room specific presence state. If the user's presence
     * state is not cached yet, Matrix.Error.UNAVAILABLE is
     * thrown.
     */
    public abstract Presence
    get_user_presence(string user_id, string? room_id = null)
        throws Matrix.Error;

    /**
     * Get a room object by the room ID specified in @param room_id.
     * If room data is not cached yet, Matrix.Error.UNAVAILABLE is
     * thrown.
     *
     * @param room_id the ID of a room
     * @return a Matrix.Room object
     */
    public abstract Room
    get_room_by_id(string room_id)
        throws Matrix.Error;

    /**
     * Get a room object by the room alias specified
     * in @param room_alias. If room data is not cached yet,
     * {@link Matrix.Error.UNAVAILABLE} is thrown.
     *
     * @param room_alias a room alias
     * @return a Matrix.Room object
     */
    public abstract Room
    get_room_by_alias(string room_alias)
        throws Matrix.Error;

    /**
     * Callback type for {@link Matrix.Client.send}.
     *
     * @param event_id the event_id returned by the server
     * @param err an error raised during event sending, if any
     */
    public delegate void
    SendCallback(string? event_id, GLib.Error? err);

    /**
     * Send an event to the given room. This will use the
     * /room/{roomId}/send or /room/{roomId}/state API depending on
     * the event: if the event has a state key (there is a state_key
     * key in the generated JSON), even if an empty one, it will use
     * the latter.
     *
     * @param room_id the room to send the event to
     * @param evt the event to send
     * @param cb the callback function to call when the request is
     *           finished
     * @param txn_id the transaction ID used by this request. In case
     *               of a state event, it will be untouched
     */
    public abstract void
    send(string room_id,
         Matrix.Event.Base evt,
         SendCallback? cb,
         out ulong txn_id)
        throws Matrix.Error;
}
