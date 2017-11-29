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

#include "matrix-client.h"
#include "matrix-marshalers.h"

/**
 * SECTION:matrix-client
 * @short_description: Interface for Matrix client implementations
 * @title: Interface for Matrix client implementations
 */

enum {
    SIGNAL_LOGIN_FINISHED,
    SIGNAL_EVENT,
    SIGNAL_POLLING_STARTED,
    SIGNAL_POLLING_STOPPED,
    NUM_SIGNALS
};
static guint matrix_client_signals[NUM_SIGNALS] = {0};

/**
  * MatrixClientInterface:
  * @login_with_password: the virtual function pointer for matrix_client_login_with_password()
  * @register_with_password: the virtual function pointer for matrix_client_register_with_password()
  * @logout: the virtual function pointer for matrix_client_logout()
  * @begin_polling: the virtual function pointer for matrix_client_begin_polling()
  * @stop_polling: the virtual function pointer for matrix_client_stop_polling()
  * @get_user_profile: the virtual function pointer for matrix_client_get_user_profile()
  * @get_user_presence: the virtual function pointer for matrix_client_get_user_presence()
  * @get_room_by_id: the virtual function pointer for matrix_client_get_room_by_id()
  * @get_room_by_alias: the virtual function pointer for matrix_client_get_room_by_alias()
  * @send: the virtual function pointer for matrix_client_send()
  * @save_state: the virtual function pointer for matrix_client_save_state()
  * @load_state: the virtual function pointer for matrix_client_load_state()
  * @login_finished: the class closure for the #MatrixClient::login-finished signal
  * @event: the class closure for the #MatrixClient::event signal
  *
  * Base interface for client communication with a Matrix.org homeserver
  */
G_DEFINE_INTERFACE(MatrixClient, matrix_client, G_TYPE_OBJECT);

/**
 * MatrixClientSendCallback:
 * @client: the #MatrixClient that initiated the request
 * @event_id: the event ID of the event just sent
 * @error: (nullable): a #GError holding errors that happened during sending, if any
 * @user_data: (nullable): user data set when event sending was initiated
 *
 * Callback function type for matrix_client_send().
 */

/**
 * MatrixClientEventCallback:
 * @client: the #MatrixClient that sent the signal
 * @room_id: the room ID from which the event originated
 * @raw_event: the raw event
 * @matrix_event: (nullable): a #MatrixMessageBase derived object
 * @user_data: (nullable): user data set when the signal was connected
 *
 * Callback function type for matrix_client_connect_event().
 *
 * @matrix_event may be unset if there was no message handler registered for the message type
 * (using matrix_message_register_type()), or if the message couldn’t be deserialised by the
 * message handler.
 */

/**
 * matrix_client_login_with_password:
 * @client: a #MatrixClient
 * @username: the username to login with
 * @password: the password to use
 * @error: a #GError, or %NULL to ignore errors
 *
 * Authenticate with the Matrix.org server with a username and password.
 */
void
matrix_client_login_with_password(MatrixClient *matrix_client, const gchar *username, const gchar *password, GError **error)
{
    g_return_if_fail(matrix_client != NULL);

    MATRIX_CLIENT_GET_IFACE(matrix_client)->login_with_password(matrix_client, username, password, error);
}

/**
 * matrix_client_register_with_password:
 * @client: a #MatrixClient
 * @username: the username to register. If omitted, the server will generate one
 * @password: the password to use with the registration
 * @error: a #GError, or %NULL to ignore errors
 *
 * Register @username with the homeserver as a normal user.  Upon success, the user is registered
 * and authenticated.
 *
 * Implementations must emit the #MatrixClient::login-finished signal when a response arrives.
 *
 * This method registers a normal user account.  If you want to register a different kind of user,
 * use matrix_api_register_account().
 */
void
matrix_client_register_with_password(MatrixClient *matrix_client, const gchar *username, const gchar *password, GError **error)
{
    g_return_if_fail(matrix_client != NULL);

    MATRIX_CLIENT_GET_IFACE(matrix_client)->register_with_password(matrix_client, username, password, error);
}

/**
 * matrix_client_logout:
 * @client: an object implementing the #MatrixClient interface
 * @error: a #GError, or %NULL to ignore errors
 *
 * Logout from the homeserver. As Matrix.org doesn’t have such a concept, this cancels all ongoing
 * requests and clears the authentication data (e.g. tokens).
 */
void
matrix_client_logout(MatrixClient *matrix_client, GError **error)
{
    g_return_if_fail(matrix_client != NULL);

    MATRIX_CLIENT_GET_IFACE(matrix_client)->logout(matrix_client, error);
}

/**
 * matrix_client_begin_polling:
 * @client: an object implementing the #MatrixClient interface
 * @error: a #GError, or %NULL to ignore errors
 *
 * Begin polling the event stream.
 */
void
matrix_client_begin_polling(MatrixClient *matrix_client, GError **error)
{
    g_return_if_fail(matrix_client != NULL);

    MATRIX_CLIENT_GET_IFACE(matrix_client)->begin_polling(matrix_client, error);
}

/**
 * matrix_client_stop_polling:
 * @client: a #MatrixClient
 * @cancel_ongoing: if %TRUE, ongoing requests will be cancelled, too
 * @error: a #GError, or %NULL to ignore errors
 *
 * Stop polling the event stream.  If @cancel_ongoing is %TRUE, ongoing requests will be cancelled,
 * too.
 */
void
matrix_client_stop_polling(MatrixClient *matrix_client, gboolean cancel_ongoing, GError **error)
{
    g_return_if_fail(matrix_client != NULL);

    MATRIX_CLIENT_GET_IFACE(matrix_client)->stop_polling(matrix_client, cancel_ongoing, error);
}

/**
 * matrix_client_emit_login_finished:
 * @client: a #MatrixClient
 * @success: set to %TRUE if login was successful
 *
 * Convenience function to emit the #MatrixClient::login-finished signal.
 */
void
matrix_client_emit_login_finished(MatrixClient *matrix_client, gboolean success)
{
    g_signal_emit(matrix_client, matrix_client_signals[SIGNAL_LOGIN_FINISHED], 0, success);
}

/**
 * matrix_client_incoming_event:
 * @client: an object implementing the #MatrixClient interface
 * @room_id: the room the event is associated with
 * @raw_event: the raw event
 * @matrix_event: the event as a Matrix.Event
 *
 * Emits the #MatrixClient::event signal.
 */
void
matrix_client_incoming_event(MatrixClient *matrix_client, const gchar *room_id, JsonNode *raw_event, MatrixEventBase *matrix_event)
{
    GQuark equark;

    g_return_if_fail(raw_event != NULL);

    if (matrix_event == NULL) {
        equark = g_type_qname(MATRIX_EVENT_TYPE_BASE);
    } else {
        equark = g_type_qname(G_TYPE_FROM_INSTANCE((GObject*)matrix_event));
    }

    g_signal_emit(matrix_client, matrix_client_signals[SIGNAL_EVENT], equark, room_id, raw_event, matrix_event);
}

/**
 * matrix_client_get_user_profile:
 * @client: an object implementing the #MatrixClient interface
 * @user_id: (not nullable): the user ID to get the profile for
 * @room_id: (nullable): the room ID to get the profile from, or %NULL to get the global profile
 * @error: (nullable): a #GError, or %NULL to ignore errors
 *
 * Get the profile of a user specified by @user_id.  If @room_id is not %NULL, return the
 * room-specific profile.  If the user’s profile is not cached yet, @error is set to
 * #MATRIX_ERROR_UNAVAILABLE.
 *
 * Returns: (nullable): a #MatrixProfile object
 */
MatrixProfile *
matrix_client_get_user_profile(MatrixClient *matrix_client, const gchar *user_id, const gchar *room_id, GError **error)
{
    g_return_val_if_fail(matrix_client != NULL, NULL);

    return MATRIX_CLIENT_GET_IFACE(matrix_client)->get_user_profile(matrix_client, user_id, room_id, error);
}

/**
 * matrix_client_get_user_presence:
 * @client: an object implementing the #MatrixClient interface
 * @user_id: (not nullable): the user ID to get presence info for
 * @room_id: (nullable): the room ID to get the presence info from, or %NULL to get the global
 *     presence info
 * @error: (nullable): a #GError, or %NULL to ignore errors
 *
 * Get the presence state of a user specified by @user_id.  If @room_id is %NULL, return the room
 * specific presence state.  If the user's presence state is not cached yet, @error is set to
 * #MATRIX_ERROR_UNAVAILABLE.
 *
 * Returns: the presence of @user_id
 */
MatrixPresence
matrix_client_get_user_presence(MatrixClient *matrix_client, const gchar *user_id, const gchar *room_id, GError **error)
{
    g_return_val_if_fail(matrix_client != NULL, 0);

    return MATRIX_CLIENT_GET_IFACE(matrix_client)->get_user_presence(matrix_client, user_id, room_id, error);
}

/**
 * matrix_client_get_room_by_id:
 * @client: an object implementing the #MatrixClient interface
 * @room_id: (not nullable): the room ID to get the room information for
 * @error: (nullable): a #GError, or %NULL to ignore errors
 *
 * Get a room object by the room ID specified in @room_id.  If room data is not cached yet, error
 * is set to #MATRIX_ERROR_UNAVAILABLE and %NULL is returned.
 *
 * Returns: (nullable): a #MatrixRoom object corresponding to @room_id
 */
MatrixRoom *
matrix_client_get_room_by_id(MatrixClient *matrix_client, const gchar *room_id, GError **error)
{
    g_return_val_if_fail(matrix_client != NULL, NULL);

    return MATRIX_CLIENT_GET_IFACE(matrix_client)->get_room_by_id(matrix_client, room_id, error);
}

/**
 * matrix_client_get_room_by_alias:
 * @client: an object implementing the #MatrixClient interface
 * @room_alias: (not nullable): a room alias
 * @error: (nullable): a #GError, or %NULL to ignore errors
 *
 * Get a room object by the room alias specified in @room_alias.  If room data is not cached yet,
 * @error is set to #MATRIX_ERROR_UNAVAILABLE and %NULL is returned.
 *
 * Please note that this may be a lengthy operation, especially if there are many rooms with many
 * aliases.
 *
 * Returns: (nullable): a #MatrixRoom object with @room_alias
 */
MatrixRoom *
matrix_client_get_room_by_alias(MatrixClient *matrix_client, const gchar *room_alias, GError **error)
{
    g_return_val_if_fail(matrix_client != NULL, NULL);

    return MATRIX_CLIENT_GET_IFACE(matrix_client)->get_room_by_alias(matrix_client, room_alias, error);
}

/**
 * matrix_client_send:
 * @client: a #MatrixClient
 * @room_id: the room to send the event to
 * @evt: the event to send
 * @callback: the callback function to call when the request is finished
 * @user_data: user data to pass to the callback function
 * @txn_id: the transaction ID to be used during this request.  In case of a state event, it will be
 *     untouched
 * @error: (nullable): a #GError, or %NULL to ignore errors
 *
 * Send an event to the given room.  This should use the correct endpoint (e.g. the
 * `/room/{roomId}/send` or the `/room/{roomId}/state` API in case of a HTTP connection) depending
 * on the event: if the event has a state key (there is a state_key key in the generated JSON),
 * even if an empty one, it will use the latter.
 */
void
matrix_client_send(MatrixClient *matrix_client,
                   const gchar *room_id,
                   MatrixEventBase *evt,
                   gulong txn_id,
                   MatrixClientSendCallback cb,
                   gpointer user_data,
                   GError **error)
{
    g_return_if_fail(matrix_client != NULL);

    MATRIX_CLIENT_GET_IFACE(matrix_client)->send(matrix_client, room_id, evt, cb, user_data, txn_id, error);
}

/**
 * matrix_client_save_state:
 * @client: an object implementing the #MatrixClient interface
 * @filename: the filename to save state info to
 * @error: a #GError, or %NULL to ignore errors
 *
 * Save the client state to a file.  This may include server addresses and credentials, too, so a
 * secure storage is highly recommended.
 */
void
matrix_client_save_state(MatrixClient *matrix_client, const gchar *filename, GError **error)
{
    g_return_if_fail(matrix_client != NULL);

    MATRIX_CLIENT_GET_IFACE(matrix_client)->save_state(matrix_client, filename, error);
}

/**
 * matrix_client_load_state:
 * @client: an object implementing the #MatrixClient interface
 * @filename: the name of the file to load state from
 * @error: (nullable): a #GError, or %NULL to ignore errors
 *
 * Load the state of the client, as saved by matrix_client_save_state().
 */
void
matrix_client_load_state(MatrixClient *matrix_client, const gchar *filename, GError **error)
{
    g_return_if_fail(matrix_client != NULL);

    MATRIX_CLIENT_GET_IFACE(matrix_client)->load_state(matrix_client, filename, error);
}

static void
matrix_client_real_login_finished(MatrixClient *matrix_client, gboolean success)
{}

static void
matrix_client_real_event(MatrixClient *matrix_client, const gchar *room_id, JsonNode *raw_event, MatrixEventBase *matrix_event)
{
    g_return_if_fail(raw_event != NULL);
}

static void
matrix_client_default_init(MatrixClientInterface  *iface)
{
    static gboolean initialized = FALSE;
    if (!initialized) {
        initialized = TRUE;
        iface->login_finished = matrix_client_real_login_finished;
        iface->event = matrix_client_real_event;
        /**
         * MatrixClient::login-finished:
         * @client: the #MatrixClient that emitted the signal
         * @success: if %TRUE, login was successful
         *
         * This signal is a sign for a finished login request.
         *
         * Implementations are responsible for emitting this signal when they get a response for a
         * login request.
         */
        matrix_client_signals[SIGNAL_LOGIN_FINISHED] = g_signal_new(
                "login-finished",
                MATRIX_TYPE_CLIENT,
                G_SIGNAL_RUN_LAST,
                G_STRUCT_OFFSET(MatrixClientInterface, login_finished),
                NULL, NULL,
                g_cclosure_marshal_VOID__BOOLEAN,
                G_TYPE_NONE, 1, G_TYPE_BOOLEAN);

        /**
         * MatrixClient::event:
         * @client: the #MatrixClient that emitted the signal
         * @room_id: the ID of the room associated with this event
         * @raw_event: the raw event as a JSON object
         * @matrix_event: the event as a #MatrixEventBase derived object
         *
         * This signal is a sign of an incoming event.
         *
         * It gets emitted for every event, regardless if it is handled by other event signals,
         * before other signals.
         *
         * Implementations are responsible for emitting this signal when any kind of event arrives
         * from the event stream or the history.
         */
        matrix_client_signals[SIGNAL_EVENT] = g_signal_new(
                "event",
                MATRIX_TYPE_CLIENT,
                G_SIGNAL_RUN_LAST | G_SIGNAL_DETAILED,
                G_STRUCT_OFFSET(MatrixClientInterface, event),
                NULL, NULL,
                _matrix_marshal_VOID__STRING_BOXED_OBJECT,
                G_TYPE_NONE, 3, G_TYPE_STRING, JSON_TYPE_NODE, MATRIX_EVENT_TYPE_BASE);

        /**
         * MatrixClient::polling-started:
         * @client: the #MatrixClient that emitted the signal
         *
         * This signal is emitted when polling is started.
         */
        matrix_client_signals[SIGNAL_POLLING_STARTED] = g_signal_new(
                "polling-started",
                MATRIX_TYPE_CLIENT,
                G_SIGNAL_RUN_LAST,
                0,
                NULL, NULL,
                g_cclosure_marshal_VOID__VOID,
                G_TYPE_NONE, 0);

        /**
         * MatrixClient::polling-stopped:
         * @client: the object that sent the signal
         * @error: gets set to an actual error if polling is stopped due to one
         *
         * This signal gets invoked when polling is stopped due to any reason.
         */
        matrix_client_signals[SIGNAL_POLLING_STOPPED] = g_signal_new(
                "polling-stopped",
                MATRIX_TYPE_CLIENT,
                G_SIGNAL_RUN_LAST,
                0,
                NULL, NULL,
                g_cclosure_marshal_VOID__BOXED,
                G_TYPE_NONE, 1, G_TYPE_ERROR);
    }
}

/**
 * matrix_client_connect_event:
 * @client: a #MatrixClient
 * @event_gtype: the #GType of a #MatrixEventBase derived type
 * @event_callback: the callback function to connect
 * @user_data: user data passed to the callback function
 * @destroy_notify: function to call on @user_data when it can be destroyed
 *
 * Connect a handler for events. If @event_gtype is #MATRIX_EVENT_TYPE_BASE, all events will be
 * sent to the callback function, otherwise only events that match the specified event type.
 *
 * If @event_gtype is not derived from #MatrixEventBase, @callback won’t get connected.
 */
void
matrix_client_connect_event(MatrixClient *client,
                            GType event_gtype,
                            MatrixClientEventCallback callback,
                            gpointer user_data,
                            GDestroyNotify destroy_notify)
{
    GClosure *closure;
    GQuark equark;
    MatrixEventBaseClass *event_class = MATRIX_EVENT_BASE_CLASS(
            g_type_class_ref(event_gtype));
    guint event_signal_id = g_signal_lookup("event", MATRIX_TYPE_CLIENT);

    if (!MATRIX_EVENT_IS_BASE_CLASS(event_class)) {
        g_warning("Trying to connect to a type that is not derived from MatrixEvent");
        g_type_class_unref(event_class);

        return;
    }

    g_type_class_unref(event_class);
    equark = g_type_qname(event_gtype);

    closure = g_closure_ref(g_cclosure_new(G_CALLBACK(callback),
                                           user_data,
                                           (GClosureNotify)destroy_notify));
    g_closure_set_marshal(closure,
                          _matrix_marshal_VOID__STRING_BOXED_OBJECT);
    g_closure_sink(closure);

    g_signal_connect_closure_by_id(client,
                                   event_signal_id, equark,
                                   closure, FALSE);
}
