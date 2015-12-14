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

#include "matrix-api.h"

/**
 * SECTION:matrix-api
 * @title: MatrixAPI
 * @short_description: An interface for actual API implementations,
 * like #MatrixHTTPAPI
 *
 * This interface provides a skeleton for all API functionality for
 * client communication with a Matrix.org homeserver.
 **/

/**
 * MatrixAPI:
 *
 * An opaque pointer type.
 **/

/**
 * MatrixAPIInterface:
 * @login: virtual function for matrix_api_login()
 * @register_account: virtual_function for
 *                    matrix_api_register_account()
 * @initial_sync: virtual function for matrix_api_initial_sync()
 * @event_stream: virtual function for matrix_api_event_stream()
 * @create_room: virtual function for matrix_api_create_room()
 * @join_room: virtual function for matrix_api_join_room()
 * @send_state_event: virtual function for
 *                    matrix_api_send_state_event()
 * @send_message_event: virtual function for
 *                      matrix_api_send_message_event()
 * @send_message: virtual function for matrix_api_send_message()
 * @send_emote: virtual function for matrix_api_send_emote()
 * @get_room_name: virtual function for matrix_api_get_room_name()
 * @get_room_topic: virtual function for matrix_api_get_room_topic()
 * @leave_room: virtual function for matrix_api_leave_room()
 * @invite_user: virtual function for matrix_api_invite_user()
 * @kick_user: virtual function for matrix_api_kick_user()
 * @set_membership: virtual function for matrix_api_set_membership()
 * @ban_user: virtual function for matrix_api_ban_user()
 * @get_room_state: virtual function for matrix_api_get_room_state()
 * @get_text_body: virtual function for matrix_api_get_text_body()
 * @get_html_body: virtual function for matrix_api_get_html_body()
 * @get_emote_body: virtual function for matrix_api_get_emote_body()
 * @_send: virtual function for matrix_api_send()
 *
 * The interface vtable for #MatrixAPI
 */

/**
 * MatrixAPICallback:
 * @api: A #MatrixAPI implementation
 * @content: the JSON content of the response, as a #JsonNode
 * @data: User data specified when calling original request function
 *
 * A callback function to use with API calls.
 */

G_DEFINE_INTERFACE(MatrixAPI, matrix_api, G_TYPE_OBJECT);

static void
matrix_api_default_init(MatrixAPIInterface *iface)
{
}

/**
 * matrix_api_ban_user:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async): the function to call when the request is
 *            finished
 * @user_data: user data to pass to the callback function
 * @room_id: the room ID where the user should be banned
 * @user_id: the user ID to ban
 * @reason: (allow-none): the reason of the ban
 *
 * Ban the specified user from the specified room. An optional reason
 * can be specified.
 */
void
matrix_api_ban_user(MatrixAPI *api,
                    MatrixAPICallback callback,
                    gpointer user_data,
                    gchar *room_id,
                    gchar *user_id,
                    gchar *reason)
{
    g_return_if_fail(MATRIX_IS_API(api));

    MATRIX_API_GET_IFACE(api)
        ->ban_user(api, callback, user_data, room_id, user_id, reason);
}

/**
 * matrix_api_create_room:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async): the function to call when the request is
 *            finished
 * @user_data: user data to pass to the callback function
 * @alias: the alias (name) of the room
 * @is_public: if %TRUE, the room will be accessible for anyone
 * @invitees: (allow-none): list of user IDs to invite to the new room
 *
 * Create a new room with the given name and invite the users in
 * @invitees.
 */
void
matrix_api_create_room(MatrixAPI *api,
                       MatrixAPICallback callback,
                       gpointer user_data,
                       gchar *alias,
                       gboolean is_public,
                       GStrv invitees)
{
    g_return_if_fail(MATRIX_IS_API(api));

    MATRIX_API_GET_IFACE(api)
        ->create_room(api, callback, user_data, alias, is_public, invitees);
}

/**
 * matrix_api_event_stream:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async): the function to call when the request is
 *            finished
 * @user_data: user data to pass to the callback function
 * @from_token: (allow-none): events will be listed from this token
 * @timeout: timeout of the request
 *
 * Get the event stream, optionally beginning from @from_token.
 */
void
matrix_api_event_stream(MatrixAPI *api,
                        MatrixAPICallback callback,
                        gpointer user_data,
                        gchar *from_token,
                        gulong timeout)
{
    g_return_if_fail(MATRIX_IS_API(api));

    MATRIX_API_GET_IFACE(api)
        ->event_stream(api, callback, user_data, from_token, timeout);
}

/**
 * matrix_api_get_room_name:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async): the function to call when the request is
 *            finished
 * @user_data: user data to pass to the callback function
 * @room_id: the room ID to get a name for
 *
 * Get the name (alias) of a room.
 */
void
matrix_api_get_room_name(MatrixAPI *api,
                         MatrixAPICallback callback,
                         gpointer user_data,
                         gchar *room_id)
{
    g_return_if_fail(MATRIX_IS_API(api));

    MATRIX_API_GET_IFACE(api)
        ->get_room_name(api, callback, user_data, room_id);
}

/**
 * matrix_api_get_room_state:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async): the function to call when the request is
 *            finished
 * @user_data: user data to pass to the callback function
 * @room_id: the room ID to get a state for
 *
 * Get the state of a room.
 */
void
matrix_api_get_room_state(MatrixAPI *api,
                          MatrixAPICallback callback,
                          gpointer user_data,
                          gchar *room_id)
{
    g_return_if_fail(MATRIX_IS_API(api));

    MATRIX_API_GET_IFACE(api)
        ->get_room_state(api, callback, user_data, room_id);
}

/**
 * matrix_api_get_room_topic:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async): the function to call when the request is
 *            finished
 * @user_data: user data to pass to the callback function
 * @room_id: the room ID to get a topic for
 *
 * Get the topic of a room.
 */
void
matrix_api_get_room_topic(MatrixAPI *api,
                          MatrixAPICallback callback,
                          gpointer user_data,
                          gchar *room_id)
{
    g_return_if_fail(MATRIX_IS_API(api));

    MATRIX_API_GET_IFACE(api)
        ->get_room_topic(api, callback, user_data, room_id);
}

/**
 * matrix_api_initial_sync:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async): the function to call when the request is
 *            finished
 * @user_data: user data to pass to the callback function
 * @limit: the maximum number of events to get
 *
 * perform an initial sync of events
 */
void
matrix_api_initial_sync(MatrixAPI *api,
                        MatrixAPICallback callback,
                        gpointer user_data,
                        guint limit)
{
    g_return_if_fail(MATRIX_IS_API(api));

    MATRIX_API_GET_IFACE(api)
        ->initial_sync(api, callback, user_data, limit);
}

/**
 * matrix_api_invite_user:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async): the function to call when the request is
 *            finished
 * @user_data: user data to pass to the callback function
 * @room_id: the room ID to invite the user to
 * @user_id: the user ID to invite
 *
 * Invite a user to a room.
 */
void
matrix_api_invite_user(MatrixAPI *api,
                       MatrixAPICallback callback,
                       gpointer user_data,
                       gchar *room_id,
                       gchar *user_id)
{
    g_return_if_fail(MATRIX_IS_API(api));

    MATRIX_API_GET_IFACE(api)
        ->invite_user(api, callback, user_data, room_id, user_id);
}

/**
 * matrix_api_join_room:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async): the function to call when the request is
 *            finished
 * @user_data: user data to pass to the callback function
 * @room_id_or_alias: the room ID or room alias to join to
 *
 * Join a room.
 */
void
matrix_api_join_room(MatrixAPI *api,
                     MatrixAPICallback callback,
                     gpointer user_data,
                     gchar *room_id_or_alias)
{
    g_return_if_fail(MATRIX_IS_API(api));

    MATRIX_API_GET_IFACE(api)
        ->join_room(api, callback, user_data, room_id_or_alias);
}

/**
 * matrix_api_kick_user:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async): the function to call when the request is
 *            finished
 * @user_data: user data to pass to the callback function
 * @room_id: the room ID to kick the user from
 * @user_id: the user to kick
 * @reason: (allow-none): the reason of kicking
 *
 * Kick a user from a room, with an optional reason.
 */
void
matrix_api_kick_user(MatrixAPI *api,
                     MatrixAPICallback callback,
                     gpointer user_data,
                     gchar *room_id,
                     gchar *user_id,
                     gchar *reason)
{
    g_return_if_fail(MATRIX_IS_API(api));

    MATRIX_API_GET_IFACE(api)
        ->kick_user(api, callback, user_data, room_id, user_id, reason);
}

/**
 * matrix_api_leave_room:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async): the function to call when the request is
 *            finished
 * @user_data: user data to pass to the callback function
 * @room_id: the room ID to kick the user from
 *
 * Leave a room
 */
void
matrix_api_leave_room(MatrixAPI *api,
                      MatrixAPICallback callback,
                      gpointer user_data,
                      gchar *room_id)
{
    g_return_if_fail(MATRIX_IS_API(api));

    MATRIX_API_GET_IFACE(api)
        ->leave_room(api, callback, user_data, room_id);
}

/**
 * matrix_api_login:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async): the function to call when the request is
 *            finished
 * @user_data: user data to pass to the callback function
 * @login_type: the login type to use
 * @parameters: (allow-none): parameters to pass for the login request
 *
 * Attempt to login with type @login_type. Implementations of this
 * method must set the token property on a successful login.
 */
void
matrix_api_login(MatrixAPI *api,
                 MatrixAPICallback callback,
                 gpointer user_data,
                 gchar *login_type,
                 GHashTable *parameters)
{
    g_return_if_fail(MATRIX_IS_API(api));

    MATRIX_API_GET_IFACE(api)
        ->login(api, callback, user_data, login_type, parameters);
}

/**
 * matrix_api_register_account:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async): the function to call when the request is
 *            finished
 * @user_data: user data to pass to the callback function
 * @login_type: the login type to use
 * @parameters: (allow-none): parameters to pass for the registration
 * request
 *
 * Attempt to register with type @login_type. Implementations of this
 * method must set the token property on a successful login.
 */
void
matrix_api_register_account(MatrixAPI *api,
                            MatrixAPICallback callback,
                            gpointer user_data,
                            gchar *login_type,
                            GHashTable *parameters)
{
    g_return_if_fail(MATRIX_IS_API(api));

    MATRIX_API_GET_IFACE(api)
        ->register_account(api, callback, user_data, login_type, parameters);
}

/**
 * matrix_api_send_emote:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async): the function to call when the request is
 *            finished
 * @user_data: user data to pass to the callback function
 * @room_id: the room to send the emote to
 * @text_content: the emote text to send
 *
 * Send an emote to the room.
 */
void
matrix_api_send_emote(MatrixAPI *api,
                      MatrixAPICallback callback,
                      gpointer user_data,
                      gchar *room_id,
                      gchar *text_content)
{
    g_return_if_fail(MATRIX_IS_API(api));

    MATRIX_API_GET_IFACE(api)
        ->send_emote(api, callback, user_data, room_id, text_content);
}

/**
 * matrix_api_send_message:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async): the function to call when the request is
 *            finished
 * @user_data: user data to pass to the callback function
 * @room_id: the room to send the emote to
 * @text_content: the emote text to send
 * @msg_type: the type of the message to be sent
 *
 * Send a custom message to the room.
 */
void
matrix_api_send_message(MatrixAPI *api,
                        MatrixAPICallback callback,
                        gpointer user_data,
                        gchar *room_id,
                        gchar *text_content,
                        gchar *msg_type)
{
    g_return_if_fail(MATRIX_IS_API(api));

    MATRIX_API_GET_IFACE(api)
        ->send_message(api,
                       callback, user_data,
                       room_id,
                       text_content,
                       msg_type);
}

/**
 * matrix_api_send_message_event:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async): the function to call when the request is
 *            finished
 * @user_data: user data to pass to the callback function
 * @room_id: the room to send the emote to
 * @event_type: the type of the event to send
 * @content: the content of the event as a #JsonNode
 *
 * Send a message event to the room.
 */
void
matrix_api_send_message_event(MatrixAPI *api,
                              MatrixAPICallback callback,
                              gpointer user_data,
                              gchar *room_id,
                              gchar *event_type,
                              JsonNode *content)
{
    g_return_if_fail(MATRIX_IS_API(api));

    MATRIX_API_GET_IFACE(api)
        ->send_message_event(api,
                             callback, user_data,
                             room_id,
                             event_type,
                             content);
}

/**
 * matrix_api_send_state_event:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async): the function to call when the request is
 *            finished
 * @user_data: user data to pass to the callback function
 * @room_id: the room to send the emote to
 * @event_type: the type of the event to send
 * @content: the content of the event as a #JsonNode
 * @state_key: the state key to send
 *
 * Send a state event to the room
 */
void
matrix_api_send_state_event(MatrixAPI *api,
                            MatrixAPICallback callback,
                            gpointer user_data,
                            gchar *room_id,
                            gchar *event_type,
                            JsonNode *content,
                            gchar *state_key)
{
    g_return_if_fail(MATRIX_IS_API(api));

    MATRIX_API_GET_IFACE(api)
        ->send_state_event(api,
                           callback, user_data,
                           room_id,
                           event_type,
                           content,
                           state_key);
}

/**
 * matrix_api_set_membership:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async): the function to call when the request is
 *            finished
 * @user_data: user data to pass to the callback function
 * @room_id: the room to send the emote to
 * @user_id: the user of whom membership will be set
 * @membership: the new membership of the user
 * @reason: (allow-none): the reason of the change
 *
 * Set the membership of the user for the given room.
 */
void
matrix_api_set_membership(MatrixAPI *api,
                          MatrixAPICallback callback,
                          gpointer user_data,
                          gchar *room_id,
                          gchar *user_id,
                          gchar *membership,
                          gchar *reason)
{
    g_return_if_fail(MATRIX_IS_API(api));

    MATRIX_API_GET_IFACE(api)
        ->set_membership(api,
                         callback, user_data,
                         room_id,
                         user_id,
                         membership,
                         reason);
}
