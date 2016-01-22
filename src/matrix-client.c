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
 * @short_description: Base interface for communication with a Matrix.org server
 * @title: MatrixClient
 * @stability: Unstable
 * @include: matrix-glib/matrix-client.h
 *
 * This is the base interface for client communication with a
 * Matrix.org server.
 */

/**
 * MatrixClientInterface:
 * @login_finished: signal is a sign of a finished login request
 * @login_with_password: virtual function for
 *                       matrix_client_login_with_password()
 * @register_with_password: virtual function for
 *                          matrix_client_register_with_password()
 * @logout: virtual function for matrix_client_logout()
 * @begin_polling: virtual function for matrix_client_begin_polling()
 * @stop_polling: virtual function for matrix_client_stop_polling()
 *
 * The interface vtable for #MatrixClient
 */

/**
 * MatrixClient:
 *
 * The MatrixClient object’s interface definition.
 */

G_DEFINE_INTERFACE(MatrixClient, matrix_client, G_TYPE_OBJECT);

static void
matrix_client_default_init(MatrixClientInterface *iface)
{
    /**
     * MatrixClient::login-finished:
     * @client: a #MatrixClient
     * @success: if %TRUE, login was successful
     *
     * This signal is a sign for a finished login request.
     *
     * Implementations of #MatrixClient are responsible for emitting
     * this signal when they get a response for a login request.
     *
     * matrix_client_login_finished() is a convenience function for
     * emitting #MatrixClient::login-finished.
     */
    g_signal_new("login-finished",
                 MATRIX_TYPE_CLIENT,
                 G_SIGNAL_RUN_LAST,
                 G_STRUCT_OFFSET(MatrixClientInterface, login_finished),
                 NULL, NULL, _matrix_marshal_VOID__BOOLEAN,
                 G_TYPE_NONE, 1, G_TYPE_BOOLEAN);
}

/**
 * matrix_client_login_with_password:
 * @client: a #MatrixClient
 * @username: the username to login with
 * @password: the password to use
 * @error: a location for a #GError, or %NULL
 *
 * Authenticate with the Matrix.org server with a username and
 * password.
 */
void
matrix_client_login_with_password(MatrixClient *client,
                                  const gchar *username,
                                  const gchar *password,
                                  GError **error)
{
    g_return_if_fail(MATRIX_IS_CLIENT(client));

    MATRIX_CLIENT_GET_IFACE(client)
        ->login_with_password(client, username, password, error);
}

/**
 * matrix_client_register_with_password:
 * @client: a #MatrixClient
 * @username: (allow-none): the username to register. If omitted, the
 *            server will generate one
 * @password: the password to use with the registration
 * @error: a location for a #GError, or %NULL
 *
 * Register @username with the homeserver as a normal user.
 *
 * Upon success, the user is registered and authenticated.
 *
 * Implementations of #MatrixClient must emit
 * MatrixClient::login-finished when a response arrives.
 *
 * If you want to register a different kind of user, use
 * matrix_api_register_account().
 */
void
matrix_client_register_with_password(MatrixClient *client,
                                     const gchar *username,
                                     const gchar *password,
                                     GError **error)
{
    g_return_if_fail(MATRIX_IS_CLIENT(client));

    MATRIX_CLIENT_GET_IFACE(client)
        ->register_with_password(client, username, password, error);
}

/**
 * matrix_client_logout:
 * @client: a #MatrixClient
 * @error: a location for a #GError, or %NULL
 *
 * Logout from the homeserver. As Matrix.org doesn’t have such an
 * option, this cancels all ongoing requests and clears the
 * authentication data (e.g. tokens).
 */
void
matrix_client_logout(MatrixClient *client, GError **error)
{
    g_return_if_fail(MATRIX_IS_CLIENT(client));

    MATRIX_CLIENT_GET_IFACE(client)
        ->logout(client, error);
}

/**
 * matrix_client_begin_polling:
 * @client: a #MatrixClient
 * @error: a location for a #GError, or %NULL
 *
 * Begin polling the event stream.
 */
void
matrix_client_begin_polling(MatrixClient *client, GError **error)
{
    g_return_if_fail(MATRIX_IS_CLIENT(client));

    MATRIX_CLIENT_GET_IFACE(client)
        ->begin_polling(client, error);
}

/**
 * matrix_client_stop_polling:
 * @client: a #MatrixClient
 * @cancel_ongoing: if %TRUE, ongoing requests will be cancelled, too
 * @error: a location for a #GError, or %NULL
 *
 * Stop polling the event stream. If @cancel_ongoing is %TRUE, ongoing
 * requests will be cancelled, too.
 */
void
matrix_client_stop_polling(MatrixClient *client,
                           gboolean cancel_ongoing,
                           GError **error)
{
    g_return_if_fail(MATRIX_IS_CLIENT(client));

    MATRIX_CLIENT_GET_IFACE(client)
        ->stop_polling(client, cancel_ongoing, error);
}

/**
 * matrix_client_login_finished:
 * @client: a #MatrixClient
 * @success: if %TRUE, login was successful
 *
 * Emits the #MatrixClient::login-finished signal.
 */
void
matrix_client_login_finished(MatrixClient *client, gboolean success)
{
    g_return_if_fail(MATRIX_IS_CLIENT(client));

    g_signal_emit_by_name(client, "login-finished", success);
}
