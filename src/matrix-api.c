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
 * @short_description: Low level API interface
 *
 * The #MatrixAPI interface allows low-level communication with Matrix.org homeservers.
 */

/**
 * MatrixAPI:
 *
 * Base interface for low level Client/Server API functionality.
 */

/**
 * MatrixAPIInterface:
 * @abort_pending: the virtual function pointer to matrix_api_abort_pending()
 * @get_3pids: the virtual function pointer to matrix_api_get_3pids()
 * @add_3pid: the virtual function pointer to matrix_api_add_3pid()
 * @deactivate_account: the virtual function pointer to matrix_api_deactivate_account()
 * @change_password: the virtual function pointer to matrix_api_change_password()
 * @whoami: the virtual function pointer to matrix_api_whoami()
 * @get_profile: the virtual function pointer to matrix_api_get_profile()
 * @get_avatar_url: the virtual function pointer to matrix_api_get_avatar_url()
 * @set_avatar_url: the virtual function pointer to matrix_api_set_avatar_url()
 * @get_display_name: the virtual function pointer to matrix_api_get_display_name()
 * @set_display_name: the virtual function pointer to matrix_api_set_display_name()
 * @register_account: the virtual function pointer to matrix_api_register_account()
 * @register_account_email: the virtual function pointer to matrix_api_register_account_email()
 * @set_account_data: the virtual function pointer to matrix_api_set_account_data()
 * @get_room_tags: the virtual function pointer to matrix_api_get_room_tags()
 * @delete_room_tag: the virtual function pointer to matrix_api_delete_room_tag()
 * @add_room_tag: the virtual function pointer to matrix_api_add_room_tag()
 * @whois: the virtual function pointer to matrix_api_whois()
 * @versions: the virtual function pointer to matrix_api_versions()
 * @create_room: the virtual function pointer to matrix_api_create_room()
 * @delete_room_alias: the virtual function pointer to matrix_api_delete_room_alias()
 * @get_room_id: the virtual function pointer to matrix_api_get_room_id()
 * @create_room_alias: the virtual function pointer to matrix_api_create_room_alias()
 * @event_stream: the virtual function pointer to matrix_api_event_stream()
 * @get_event: the virtual function pointer to matrix_api_get_event()
 * @initial_sync: the virtual function pointer to matrix_api_initial_sync()
 * @get_event_context: the virtual function pointer to matrix_api_get_event_context()
 * @initial_sync_room: the virtual function pointer to matrix_api_initial_sync_room()
 * @list_room_members: the virtual function pointer to matrix_api_list_room_members()
 * @list_room_messages: the virtual function pointer to matrix_api_list_room_messages()
 * @send_event_receipt: the virtual function pointer to matrix_api_send_event_receipt()
 * @redact_event: the virtual function pointer to matrix_api_redact_event()
 * @send_event: the virtual function pointer to matrix_api_send_event()
 * @get_room_state: the virtual function pointer to matrix_api_get_room_state()
 * @send_state_event: the virtual function pointer to matrix_api_send_state_event()
 * @notify_room_typing: the virtual function pointer to matrix_api_notify_room_typing()
 * @sync: the virtual function pointer to matrix_api_sync()
 * @create_filter: the virtual function pointer to matrix_api_create_filter()
 * @download_filter: the virtual function pointer to matrix_api_download_filter()
 * @join_room_id_or_alias: the virtual function pointer to matrix_api_join_room_id_or_alias()
 * @ban_user: the virtual function pointer to matrix_api_ban_user()
 * @forget_room: the virtual function pointer to matrix_api_forget_room()
 * @invite_user_3rdparty: the virtual function pointer to matrix_api_invite_user_3rdparty()
 * @invite_user: the virtual function pointer to matrix_api_invite_user()
 * @join_room: the virtual function pointer to matrix_api_join_room()
 * @get_joined_rooms: the virtual function pointer to matrix_api_get_joined_rooms()
 * @kick_user: the virtual function pointer to matrix_api_kick_user()
 * @leave_room: the virtual function pointer to matrix_api_leave_room()
 * @unban_user: the virtual function pointer to matrix_api_unban_user()
 * @login: the virtual function pointer to matrix_api_login()
 * @logout: the virtual function pointer to matrix_api_logout()
 * @get_presence_list: the virtual function pointer to matrix_api_get_presence_list()
 * @update_presence_list: the virtual function pointer to matrix_api_update_presence_list()
 * @get_presence: the virtual function pointer to matrix_api_get_presence()
 * @set_presence: the virtual function pointer to matrix_api_set_presence()
 * @list_public_rooms: the virtual function pointer to matrix_api_list_public_rooms()
 * @get_pushers: the virtual function pointer to matrix_api_get_pushers()
 * @update_pusher: the virtual function pointer to matrix_api_update_pusher()
 * @get_pushrules: the virtual function pointer to matrix_api_get_pushrules()
 * @delete_pushrule: the virtual function pointer to matrix_api_delete_pushrule()
 * @get_pushrule: the virtual function pointer to matrix_api_get_pushrule()
 * @add_pushrule: the virtual function pointer to matrix_api_add_pushrule()
 * @toggle_pushrule: the virtual function pointer to matrix_api_toggle_pushrule()
 * @search: the virtual function pointer to matrix_api_search()
 * @get_turn_server: the virtual function pointer to matrix_api_get_turn_server()
 * @media_download: the virtual function pointer to matrix_api_media_download()
 * @media_thumbnail: the virtual function pointer to matrix_api_media_thumbnail()
 * @media_upload: the virtual function pointer to matrix_api_media_upload()
 * @get_token: the virtual function pointer to matrix_api_get_token()
 * @set_token: the virtual function pointer to matrix_api_set_token()
 * @get_user_id: the virtual function pointer to matrix_api_get_user_id()
 * @set_user_id: the virtual function pointer to matrix_api_set_user_id()
 * @get_homeserver: the virtual function pointer to matrix_api_get_homeserver()
 * @set_homeserver: the virtual function pointer to matrix_api_set_homeserver()
 *
 * The virtual function table for for #MatrixAPI.
 */
G_DEFINE_INTERFACE(MatrixAPI, matrix_api, G_TYPE_OBJECT);

/**
 * MATRIX_DEPRECATED_FOR:
 * @f: a function name
 *
 * Macro to notify compilers about deprecation, in favour of @f.
 */

/**
 * matrix_api_abort_pending:
 * @api: a #MatrixAPI
 *
 * Abort all pending requests toward a Matrix homeserver.  Be aware that this may leave
 * requests in an inconsistent state.
 */
void
matrix_api_abort_pending(MatrixAPI *matrix_api)
{
    g_return_if_fail(matrix_api != NULL);

    MATRIX_API_GET_IFACE(matrix_api)->abort_pending(matrix_api);
}

/**
 * matrix_api_get_3pids:
 * @api: a #MatrixAPI
 * @callback: the callback function to call when the request is finished
 * @user_data: user data to pass to the callback function
 * @error: (nullable): a #GError, or %NULL to ignore errors
 *
 * Get a list of the third party identifiers that a homeserver has associated with the
 * user’s account.
 *
 * This is not the same as the list of third party identifiers bound to the user’s Matrix ID
 * in Identity Servers.
 *
 * Identifiers in this list may be used by the homeserver as, for example, identifiers to accept
 * to reset the user’s account password.
 */
void
matrix_api_get_3pids(MatrixAPI *matrix_api,
                     MatrixAPICallback callback,
                     gpointer user_data,
                     GError **error)
{
    g_return_if_fail(matrix_api != NULL);

    MATRIX_API_GET_IFACE(matrix_api)->get_3pids(matrix_api, callback, user_data, error);
}

/**
 * matrix_api_add_3pid:
 * @api: a #MatrixAPI
 * @bind_creds: whether the homeserver should also bind this third party identifier to the
 *     account’s Matrix ID with the passed Identity Server.
 * @threepid_creds: (not nullable): the credentials to associate with the account
 * @callback: the function to call when the request is finished
 * @user_data: user data to be passed to @callback
 * @error: (nullable): a #GError, or %NULL to ignore errors
 *
 * Add contact information to the user’s account.
 */
void
matrix_api_add_3pid(MatrixAPI *matrix_api,
                    gboolean bind_creds,
                    Matrix3PidCredential *threepid_creds,
                    MatrixAPICallback callback,
                    gpointer user_data,
                    GError **error)
{
    g_return_if_fail(matrix_api != NULL);
    g_return_if_fail(threepid_creds != NULL);

    MATRIX_API_GET_IFACE(matrix_api)->add_3pid(matrix_api, callback, user_data, bind_creds, threepid_creds, error);
}

/**
 * matrix_api_deactivate_account:
 * @api: a #MatrixAPI
 * @session: optional session token provided by the server
 * @login_type: the login type the client is trying to complete
 * @callback: the function to call when the request is finished
 * @user_data: user data to be passed to @callback
 * @error: (nullable): a #GError, or %NULL to ignore errors
 *
 * Deactivate the account.
 */
void
matrix_api_deactivate_account(MatrixAPI *matrix_api,
                              const gchar *session,
                              const gchar *login_type,
                              MatrixAPICallback callback,
                              gpointer user_data,
                              GError **error)
{
    g_return_if_fail(matrix_api != NULL);

    MATRIX_API_GET_IFACE(matrix_api)->deactivate_account(matrix_api, callback, user_data, session, login_type, error);
}

/**
 * matrix_api_change_password:
 * @api: a #MatrixAPI
 * @new_password: (not nullable): the new password for the account
 * @callback: the function to call when the request is finished
 * @user_data: user data to be passed to @callback
 * @error: (nullable): a #GError, or %NULL to ignore errors
 *
 * Change the active user’s password.
 */
void
matrix_api_change_password(MatrixAPI *matrix_api,
                           const gchar *new_password,
                           MatrixAPICallback callback,
                           gpointer user_data,
                           GError **error)
{
    g_return_if_fail(matrix_api != NULL);
    g_return_if_fail(new_password != NULL);

    MATRIX_API_GET_IFACE(matrix_api)->change_password(matrix_api, callback, user_data, new_password, error);
}

/**
 * matrix_api_whoami:
 * @api: a #MatrixAPI
 * @callback: the function to call when the request is finished
 * @user_data: user data to be passed to @callback
 * @error: (nullable): a #GError, or %NULL to ignore errors
 *
 * Get the user’s Matrix ID.
 */
void
matrix_api_whoami(MatrixAPI *api,
                  MatrixAPICallback callback,
                  gpointer user_data,
                  GError **error)
{
    g_return_if_fail(api != NULL);

    MATRIX_API_GET_IFACE(api)->whoami(api, callback, user_data, error);
}

/**
 * matrix_api_get_profile:
 * @api: a #MatrixAPI
 * @user_id: (not nullable): the user whose profile to get
 * @callback: the function to call when the request is finished
 * @user_data: user data to be passed to @callback
 * @error: (nullable): a #GError, or %NULL to ignore errors
 *
 * Get a user’s profile.
 */
void
matrix_api_get_profile(MatrixAPI *matrix_api,
                       const gchar *user_id,
                       MatrixAPICallback callback,
                       gpointer user_data,
                       GError **error)
{
    g_return_if_fail(matrix_api != NULL);
    g_return_if_fail(user_id != NULL);

    MATRIX_API_GET_IFACE(matrix_api)->get_profile(matrix_api, callback, user_data, user_id, error);
}

/**
 * matrix_api_get_avatar_url:
 * @api: a #MatrixAPI
 * @user_id: (not nullable): the user whose avatar URL to get
 * @callback: the function to call when the request is finished
 * @user_data: user data to be passed to @callback
 * @error: (nullable): a #GError, or %NULL to ignore errors
 *
 * Get the URL of the specified user’s avatar.
 */
void
matrix_api_get_avatar_url(MatrixAPI *matrix_api,
                          const gchar *user_id,
                          MatrixAPICallback callback,
                          gpointer user_data,
                          GError **error)
{
    g_return_if_fail(matrix_api != NULL);
    g_return_if_fail(user_id != NULL);

    MATRIX_API_GET_IFACE(matrix_api)->get_avatar_url(matrix_api, callback, user_data, user_id, error);
}

/**
 * matrix_api_set_avatar_url:
 * @api: a #MatrixAPI
 * @callback: the function to call when the request is finished
 * @user_id: (not nullable): the user whose avatar URL to set
 * @avatar_url: the avatar URL info
 * @user_data: user data to be passed to @callback
 * @error: (nullable): a #GError, or %NULL to ignore errors
 *
 * Set the user’s avatar URL.
 */
void
matrix_api_set_avatar_url(MatrixAPI *matrix_api,
                          const gchar *user_id,
                          const gchar *avatar_url,
                          MatrixAPICallback callback,
                          gpointer user_data,
                          GError **error)
{
    g_return_if_fail(matrix_api != NULL);
    g_return_if_fail(user_id != NULL);

    MATRIX_API_GET_IFACE(matrix_api)->set_avatar_url(matrix_api, callback, user_data, user_id, avatar_url, error);
}

/**
 * matrix_api_get_display_name:
 * @api: a #MatrixAPI
 * @user_id: (not nullable): the user whose display name to get
 * @callback: the function to call when the request is finished
 * @user_data: user data to be passed to @callback
 * @error: (nullable): a #GError, or %NULL to ignore errors
 *
 * Get the user’s display name.
 */
void
matrix_api_get_display_name(MatrixAPI *matrix_api,
                            const gchar *user_id,
                            MatrixAPICallback callback,
                            gpointer user_data,
                            GError **error)
{
    g_return_if_fail(matrix_api != NULL);
    g_return_if_fail(user_id != NULL);

    MATRIX_API_GET_IFACE(matrix_api)->get_display_name(matrix_api, callback, user_data, user_id, error);
}

/**
 * matrix_api_set_display_name:
 * @api: a #MatrixAPI
 * @user_id: (not nullable): the user whose display name to set
 * @display_name: the display name info
 * @callback: the function to call when the request is finished
 * @user_data: user data to be passed to @callback
 * @error: (nullable): a #GError, or %NULL to ignore errors
 *
 * Set the user’s display name.
 */
void
matrix_api_set_display_name(MatrixAPI *matrix_api,
                            const gchar *user_id,
                            const gchar *display_name,
                            MatrixAPICallback callback,
                            gpointer user_data,
                            GError **error)
{
    g_return_if_fail(matrix_api != NULL);
    g_return_if_fail(user_id != NULL);

    MATRIX_API_GET_IFACE(matrix_api)->set_display_name(matrix_api, callback, user_data, user_id, display_name, error);
}

/**
 * matrix_api_register_account:
 * @api: a #MatrixAPI
 * @account_kind: the type of account to register
 * @bind_email: if %TRUE, the server binds the e-mail used for authentication to the Matrix ID
 *     with the ID server
 * @username: the local part of the desired Matrix ID. If omitted, the server will generate a
 *     local part
 * @password: (not nullable): the desired password for the account
 * @callback: the function to call when the request is finished
 * @user_data: user data to be passed to @callback
 * @error: (nullable): a #GError, or %NULL to ignore errors
 *
 * Attempt to register an account with the homeserver using @username and @password.
 *
 * Implementations of this method must set the #MatrixAPI:token property on a successful login.
 */
void
matrix_api_register_account(MatrixAPI *matrix_api,
                            MatrixAccountKind account_kind,
                            gboolean bind_email,
                            const gchar *username,
                            const gchar *password,
                            MatrixAPICallback callback,
                            gpointer user_data,
                            GError **error)
{
    g_return_if_fail(matrix_api != NULL);
    g_return_if_fail(password != NULL);

    MATRIX_API_GET_IFACE(matrix_api)->register_account(matrix_api, callback, user_data, account_kind, bind_email, username, password, error);
}

void
matrix_api_register_account_email(MatrixAPI *api,
                                  const gchar *id_server,
                                  const gchar *client_secret,
                                  const gchar *email,
                                  guint send_attempt,
                                  MatrixAPICallback callback,
                                  gpointer user_data,
                                  GError **error)
{
    g_return_if_fail(api != NULL);
    g_return_if_fail(id_server);
    g_return_if_fail(client_secret);
    g_return_if_fail(email);

    MATRIX_API_GET_IFACE(api)->register_account_email(api, callback, user_data, id_server, client_secret, email, send_attempt, error);
}

/**
 * matrix_api_set_account_data:
 * @api: a #MatrixAPI
 * @user_id: (not nullable): the user to set account data for.  An access token must be
 *    present and be authorized to make requests for this user ID
 * @room_id: the room to set account data for.  If %NULL, the account data will be set globally
 * @event_type: (not nullable): the event type of the account data to set.  Custom types
 *     should be namespaced to avoid clashes
 * @content: (not nullable): the content of the account data
 * @callback: the function to call when the request is finished
 * @user_data: user data to be passed to @callback
 * @error: (nullable): a #GError, or %NULL to ignore errors
 *
 * Set some account data for the client.  This config is only visible to the user who set the
 * account data.  The config will be synced to clients in the top-level account data.
 */
void
matrix_api_set_account_data(MatrixAPI *matrix_api,
                            const gchar *user_id,
                            const gchar *room_id,
                            const gchar *event_type,
                            JsonNode *content,
                            MatrixAPICallback callback,
                            gpointer user_data,
                            GError **error)
{
    g_return_if_fail(matrix_api != NULL);
    g_return_if_fail(user_id != NULL);
    g_return_if_fail(event_type != NULL);
    g_return_if_fail(content != NULL);

    MATRIX_API_GET_IFACE(matrix_api)->set_account_data(matrix_api, callback, user_data, user_id, room_id, event_type, content, error);
}

/**
 * matrix_api_get_room_tags:
 * @api: a #MatrixAPI
 * @user_id: (not nullable): the ID of the user to get the tags for.  An access token must be
 *     set, and it must be authorised to make requests for this user ID
 * @room_id: (not nullable): the room to get tags for
 * @callback: the function to call when the request is finished
 * @user_data: user data to be passed to @callback
 * @error: (nullable): a #GError, or %NULL to ignore errors
 *
 * List the tags set by a user on a room.
 */
void
matrix_api_get_room_tags(MatrixAPI *matrix_api,
                         const gchar *user_id,
                         const gchar *room_id,
                         MatrixAPICallback callback,
                         gpointer user_data,
                         GError **error)
{
    g_return_if_fail(matrix_api != NULL);
    g_return_if_fail(user_id != NULL);
    g_return_if_fail(room_id != NULL);

    MATRIX_API_GET_IFACE(matrix_api)->get_room_tags(matrix_api, callback, user_data, user_id, room_id, error);
}

/**
 * matrix_api_delete_room_tag:
 * @api: a #MatrixAPI
 * @user_id: (not nullable): the id of the user to remove a tag for
 * @room_id: (not nullable): the id of the room to remove the tag from
 * @tag: (not nullable): the tag to remove
 * @callback: the function to call when the request is finished
 * @user_data: user data to be passed to @callback
 * @error: (nullable): a #GError, or %NULL to ignore errors
 *
 * Remove a tag from the room.
 */
void
matrix_api_delete_room_tag(MatrixAPI *matrix_api,
                           const gchar *user_id,
                           const gchar *room_id,
                           const gchar *tag,
                           MatrixAPICallback callback,
                           gpointer user_data,
                           GError **error)
{
    g_return_if_fail(matrix_api != NULL);
    g_return_if_fail(user_id != NULL);
    g_return_if_fail(room_id != NULL);
    g_return_if_fail(tag != NULL);

    MATRIX_API_GET_IFACE(matrix_api)->delete_room_tag(matrix_api, callback, user_data, user_id, room_id, tag, error);
}

/**
 * matrix_api_add_room_tag:
 * @api: a #MatrixAPI
 * @user_id: (not nullable): the ID of the user to add the tag for
 * @room_id: (not nullable): the ID of the room to add the tag for
 * @tag: (not nullable): the tag to add
 * @content: extra data for the tag, e.g. ordering
 * @callback: the function to call when the request is finished
 * @user_data: user data to be passed to @callback
 * @error: (nullable): a #GError, or %NULL to ignore errors
 *
 * Add a tag to the room.
 */
void
matrix_api_add_room_tag(MatrixAPI *matrix_api,
                        const gchar *user_id,
                        const gchar *room_id,
                        const gchar *tag,
                        JsonNode *content,
                        MatrixAPICallback callback,
                        gpointer user_data,
                        GError **error)
{
    g_return_if_fail(matrix_api != NULL);
    g_return_if_fail(user_id != NULL);
    g_return_if_fail(room_id != NULL);
    g_return_if_fail(tag != NULL);

    MATRIX_API_GET_IFACE(matrix_api)->add_room_tag(matrix_api, callback, user_data, user_id, room_id, tag, content, error);
}

/**
 * matrix_api_whois:
 * @api: a #MatrixAPI
 * @user_id: (not nullable): the user ID to look up
 * @callback: the function to call when the request is finished
 * @user_data: user data to be passed to @callback
 * @error: (nullable): a #GError, or %NULL to ignore errors
 *
 * Get information about a particular user.
 */
void
matrix_api_whois(MatrixAPI *matrix_api,
                 const gchar *user_id,
                 MatrixAPICallback callback,
                 gpointer user_data,
                 GError **error)
{
    g_return_if_fail(matrix_api != NULL);
    g_return_if_fail(user_id != NULL);

    MATRIX_API_GET_IFACE(matrix_api)->whois(matrix_api, callback, user_data, user_id, error);
}

/**
 * matrix_api_versions:
 * @api: a #MatrixAPI
 * @callback: the function to call when the request is finished
 * @user_data: user data to be passed to @callback
 * @error: (nullable): a #GError, or %NULL to ignore errors
 *
 * Get the versions of the specification supported by the server.
 */
void
matrix_api_versions(MatrixAPI *matrix_api,
                    MatrixAPICallback callback,
                    gpointer user_data,
                    GError **error)
{
    g_return_if_fail(matrix_api != NULL);

    MATRIX_API_GET_IFACE(matrix_api)->versions(matrix_api, callback, user_data, error);
}

/**
 * matrix_api_create_room:
 * @api: a #MatrixAPI
 * @preset: a room preset to use
 * @room_name: the desired display name for the room
 * @room_alias: an alias of the room
 * @topic: the topic of the room
 * @visibility: the initial visibility of the room
 * @creation_content: extra keys to be added to the content of m.room.create
 * @initial_state: a list of state events to set in the new room
 * @n_initial_state: the length of @initial_state
 * @invitees: list of user IDs to invite to the new room
 * @n_invitees: the length of @invitees
 * @invite_3pids: a list of 3rd party credentials to invite to the new room
 * @n_invite_3pids: the length of @invite_3pids
 * @callback: the function to call when the request is finished
 * @user_data: user data to be passed to @callback
 * @error: (nullable): a #GError, or %NULL to ignore errors
 *
 * Create a new room with the given name and invite the users in @invitees.
 */
void
matrix_api_create_room(MatrixAPI *matrix_api,
                       MatrixRoomPreset preset,
                       const gchar *room_name,
                       const gchar *room_alias,
                       const gchar *topic,
                       MatrixRoomVisibility visibility,
                       JsonNode *creation_content,
                       MatrixEventState **initial_state,
                       int n_initial_state,
                       gchar **invitees,
                       int n_invitees,
                       Matrix3PidCredential **invite_3pids,
                       int n_invite_3pids,
                       MatrixAPICallback callback,
                       gpointer user_data,
                       GError **error)
{
    g_return_if_fail(matrix_api != NULL);

    MATRIX_API_GET_IFACE(matrix_api)->create_room(matrix_api, callback, user_data, preset, room_name, room_alias, topic, visibility, creation_content, initial_state, n_initial_state, invitees, n_invitees, invite_3pids, n_invite_3pids, error);
}

/**
 * matrix_api_delete_room_alias:
 * @api: a #MatrixAPI
 * @room_alias: (not nullable): the alias name to remove
 * @callback: the function to call when the request is finished
 * @user_data: user data to be passed to @callback
 * @error: (nullable): a #GError, or %NULL to ignore errors
 *
 * Remove the mapping of @room_alias to its room ID.
 *
 * Servers may choose to implement additional access control checks here, for instance that
 * room aliases can only be deleted by their creator or a server administrator.
 */
void
matrix_api_delete_room_alias(MatrixAPI *matrix_api,
                             const gchar *room_alias,
                             MatrixAPICallback callback,
                             gpointer user_data,
                             GError **error)
{
    g_return_if_fail(matrix_api != NULL);
    g_return_if_fail(room_alias != NULL);

    MATRIX_API_GET_IFACE(matrix_api)->delete_room_alias(matrix_api, callback, user_data, room_alias, error);
}

/**
 * matrix_api_get_joined_rooms:
 * @api: a #MatrixAPI
 * @callback: the function to call when the request is finished
 * @user_data: user data to be passed to @callback
 * @error: (nullable): a #GError, or %NULL to ignore errors
 *
 * Get the rooms the current user is joined to.
 */
void
matrix_api_get_joined_rooms(MatrixAPI *api,
                            MatrixAPICallback callback,
                            gpointer user_data,
                            GError **error)
{
    g_return_if_fail(api != NULL);

    MATRIX_API_GET_IFACE(api)->get_joined_rooms(api, callback, user_data, error);
}

/**
 * matrix_api_get_room_id:
 * @api: a #MatrixAPI
 * @room_alias: (not nullable): the room alias
 * @callback: the function to call when the request is finished
 * @user_data: user data to be passed to @callback
 * @error: (nullable): a #GError, or %NULL to ignore errors
 *
 * Get the room ID corresponding to this room alias.
 */
void
matrix_api_get_room_id(MatrixAPI *matrix_api,
                       const gchar *room_alias,
                       MatrixAPICallback callback,
                       gpointer user_data,
                       GError **error)
{
    g_return_if_fail(matrix_api != NULL);
    g_return_if_fail(room_alias != NULL);

    MATRIX_API_GET_IFACE(matrix_api)->get_room_id(matrix_api, callback, user_data, room_alias, error);
}

/**
 * matrix_api_create_room_alias:
 * @api: a #MatrixAPI
 * @room_id: (not nullable): the room ID to add this alias to
 * @room_alias: (not nullable): the room alias to set
 * @callback: the function to call when the request is finished
 * @user_data: user data to be passed to @callback
 * @error: (nullable): a #GError, or %NULL to ignore errors
 *
 * Create a new mapping from room alias to room ID.
 */
void
matrix_api_create_room_alias(MatrixAPI *matrix_api,
                             const gchar *room_id,
                             const gchar *room_alias,
                             MatrixAPICallback callback,
                             gpointer user_data,
                             GError **error)
{
    g_return_if_fail(matrix_api != NULL);
    g_return_if_fail(room_id != NULL);
    g_return_if_fail(room_alias != NULL);

    MATRIX_API_GET_IFACE(matrix_api)->create_room_alias(matrix_api, callback, user_data, room_id, room_alias, error);
}

/**
 * matrix_api_event_stream:
 * @api: a #MatrixAPI
 * @from_token: events will be listed from this token
 * @timeout: timeout of the request
 * @callback: the function to call when the request is finished
 * @user_data: user data to be passed to @callback
 * @error: (nullable): a #GError, or %NULL to ignore errors
 *
 * Get the event stream, optionally beginning from @from_token.
 *
 * Deprecated: This functionality is deprecated on the Matrix specification level.
 *     Use matrix_api_sync() instead.
 */
void
matrix_api_event_stream(MatrixAPI *matrix_api,
                        const gchar *from_token,
                        gulong timeout,
                        MatrixAPICallback callback,
                        gpointer user_data,
                        GError **error)
{
    g_return_if_fail(matrix_api != NULL);

    MATRIX_API_GET_IFACE(matrix_api)->event_stream(matrix_api, callback, user_data, from_token, timeout, error);
}

/**
 * matrix_api_get_event:
 * @api: a #MatrixAPI
 * @event_id: (not nullable): the event ID to get
 * @callback: the function to call when the request is finished
 * @user_data: user data to be passed to @callback
 * @error: (nullable): a #GError, or %NULL to ignore errors
 *
 * Get a single event by event ID.
 */
void
matrix_api_get_event(MatrixAPI *matrix_api,
                     const gchar *event_id,
                     MatrixAPICallback callback,
                     gpointer user_data,
                     GError **error)
{
    g_return_if_fail(matrix_api != NULL);
    g_return_if_fail(event_id != NULL);

    MATRIX_API_GET_IFACE(matrix_api)->get_event(matrix_api, callback, user_data, event_id, error);
}

/**
 * matrix_api_initial_sync:
 * @api: a #MatrixAPI
 * @limit: the maximum number of events to get
 * @archived: whether to include rooms that the user has left
 * @callback: the function to call when the request is finished
 * @user_data: user data to be passed to @callback
 * @error: (nullable): a #GError, or %NULL to ignore errors
 *
 * Perform an initial sync of events.
 */
void
matrix_api_initial_sync(MatrixAPI *matrix_api,
                        guint limit,
                        gboolean archived,
                        MatrixAPICallback callback,
                        gpointer user_data,
                        GError **error)
{
    g_return_if_fail(matrix_api != NULL);

    MATRIX_API_GET_IFACE(matrix_api)->initial_sync(matrix_api, callback, user_data, limit, archived, error);
}

/**
 * matrix_api_get_event_context:
 * @api: a #MatrixAPI
 * @room_id: (not nullable): the room to get events from
 * @event_id: (not nullable): the event to get context around
 * @limit: the maximum number of events to get.  If 0, a default value is used(10, according
 *     to the specification)
 * @callback: the function to call when the request is finished
 * @user_data: user data to be passed to @callback
 * @error: (nullable): a #GError, or %NULL to ignore errors
 *
 * Gets a number of events that happened just before and after the specified event.
 */
void
matrix_api_get_event_context(MatrixAPI *matrix_api,
                             const gchar *room_id,
                             const gchar *event_id,
                             guint limit,
                             MatrixAPICallback callback,
                             gpointer user_data,
                             GError **error)
{
    g_return_if_fail(matrix_api != NULL);

    MATRIX_API_GET_IFACE(matrix_api)->get_event_context(matrix_api, callback, user_data, room_id, event_id, limit, error);
}

/**
 * matrix_api_initial_sync_room:
 * @api: a #MatrixAPI
 * @room_id: (not nullable): the room to get the data for
 * @callback: the function to call when the request is finished
 * @user_data: user data to be passed to @callback
 * @error: (nullable): a #GError, or %NULL to ignore errors
 *
 * Get a copy of the current state and the most recent messages in a room.
 */
void
matrix_api_initial_sync_room(MatrixAPI *matrix_api,
                             const gchar *room_id,
                             MatrixAPICallback callback,
                             gpointer user_data,
                             GError **error)
{
    g_return_if_fail(matrix_api != NULL);
    g_return_if_fail(room_id != NULL);

    MATRIX_API_GET_IFACE(matrix_api)->initial_sync_room(matrix_api, callback, user_data, room_id, error);
}

/**
 * matrix_api_list_room_members:
 * @api: a #MatrixAPI
 * @room_id: (not nullable): the room to get the member events for
 * @callback: the function to call when the request is finished
 * @user_data: user data to be passed to @callback
 * @error: (nullable): a #GError, or %NULL to ignore errors
 *
 * Get the list of members for a room.
 */
void
matrix_api_list_room_members(MatrixAPI *matrix_api,
                             const gchar *room_id,
                             MatrixAPICallback callback,
                             gpointer user_data,
                             GError **error)
{
    g_return_if_fail(matrix_api != NULL);
    g_return_if_fail(room_id != NULL);

    MATRIX_API_GET_IFACE(matrix_api)->list_room_members(matrix_api, callback, user_data, room_id, error);
}

/**
 * matrix_api_list_room_messages:
 * @api: a #MatrixAPI
 * @room_id: (not nullable): the room to get the events for
 * @from_token: (not nullable): the token to start returning events from.  This token can be obtained by
 *     calling matrix_api_initial_sync() or matrix_api_initial_sync_room()
 * @direction: the direction of the returned events
 * @limit: the maximum number of events to return.  If 0, a default value will be used(10,
 *     according to the specification
 * @callback: the function to call when the request is finished
 * @user_data: user data to be passed to @callback
 * @error: (nullable): a #GError, or %NULL to ignore errors
 *
 * Get a list of message and state events for a room.
 */
void
matrix_api_list_room_messages(MatrixAPI *matrix_api,
                              const gchar *room_id,
                              const gchar *from_token,
                              MatrixEventDirection direction,
                              guint limit,
                              MatrixAPICallback callback,
                              gpointer user_data,
                              GError **error)
{
    g_return_if_fail(matrix_api != NULL);
    g_return_if_fail(room_id != NULL);
    g_return_if_fail(from_token != NULL);

    MATRIX_API_GET_IFACE(matrix_api)->list_room_messages(matrix_api, callback, user_data, room_id, from_token, direction, limit, error);
}

/**
 * matrix_api_send_event_receipt:
 * @api: a #MatrixAPI
 * @room_id: (not nullable): the room in which to send the event
 * @receipt_type: (not nullable): type of the receipt
 * @event_id: (not nullable): the event ID to acknowledge up to
 * @receipt: (not nullable): extra receipt information to attach.  Note that the server will automatically
 *     attach the `ta` field
 * @callback: the function to call when the request is finished
 * @user_data: user data to be passed to @callback
 * @error: (nullable): a #GError, or %NULL to ignore errors
 *
 * Update the marker for the given receipt type to the event ID specified.
 */
void
matrix_api_send_event_receipt(MatrixAPI *matrix_api,
                              const gchar *room_id,
                              MatrixReceiptType receipt_type,
                              const gchar *event_id,
                              JsonNode *receipt,
                              MatrixAPICallback callback,
                              gpointer user_data,
                              GError **error)
{
    g_return_if_fail(matrix_api != NULL);
    g_return_if_fail(room_id != NULL);
    g_return_if_fail(event_id != NULL);
    g_return_if_fail(receipt_type != MATRIX_RECEIPT_TYPE_UNKNOWN);

    MATRIX_API_GET_IFACE(matrix_api)->send_event_receipt(matrix_api, callback, user_data, room_id, receipt_type, event_id, receipt, error);
}

/**
 * matrix_api_redact_event:
 * @api: a #MatrixAPI
 * @room_id: (not nullable): the room from which to redact the event
 * @event_id: (not nullable): the event ID to acknowledge up to
 * @txn_id: (not nullable): the transaction ID for this event.  Clients should generate a
 *     unique ID; it will be used by the server to ensure idempotency of requests
 * @reason: the reason for the event being redacted
 * @callback: the function to call when the request is finished
 * @user_data: user data to be passed to @callback
 * @error: (nullable): a #GError, or %NULL to ignore errors
 *
 * Strip all information out of an event which isn’t critical to the integrity of the
 * server-side representation of the room. This  cannot be undone.
 *
 * Users may redact their own events, and any user with a power level greater than or equal to
 * `redact` power level of the room may redact events there.
 */
void
matrix_api_redact_event(MatrixAPI *matrix_api,
                        const gchar *room_id,
                        const gchar *event_id,
                        const gchar *txn_id,
                        const gchar *reason,
                        MatrixAPICallback callback,
                        gpointer user_data,
                        GError **error)
{
    g_return_if_fail(matrix_api != NULL);
    g_return_if_fail(room_id);
    g_return_if_fail(event_id);
    g_return_if_fail(txn_id);

    MATRIX_API_GET_IFACE(matrix_api)->redact_event(matrix_api, callback, user_data, room_id, event_id, txn_id, reason, error);
}

/**
 * matrix_api_send_event:
 * @api: a #MatrixAPI
 * @callback: the function to call when the request is finished
 * @room_id: the room to send the event to
 * @event_type: the type of event to send
 * @txn_id: the transaction ID for this event.  Clients should generate a unique ID; it will be
 *     used by the server to ensure idempotency of requests
 * @content: the content of the event as a #JsonNode
 * @user_data: user data to be passed to @callback
 * @error: (nullable): a #GError, or %NULL to ignore errors
 *
 * Send a message event to the room.
 */
void
matrix_api_send_event(MatrixAPI *matrix_api,
                      const gchar *room_id,
                      const gchar *event_type,
                      const gchar *txn_id,
                      JsonNode *content,
                      MatrixAPICallback callback,
                      gpointer user_data,
                      GError **error)
{
    g_return_if_fail(matrix_api != NULL);
    g_return_if_fail(room_id);
    g_return_if_fail(event_type);
    g_return_if_fail(txn_id);
    g_return_if_fail(content);

    MATRIX_API_GET_IFACE(matrix_api)->send_event(matrix_api, callback, user_data, room_id, event_type, txn_id, content, error);
}

/**
 * matrix_api_get_room_state:
 * @api: a #MatrixAPI
 * @room_id: (not nullable): the room ID to get a state for
 * @event_type: the type of state to look up
 * @state_key: the key of the state to look up.  If @event_type is %NULL, this parameter is
 *     ignored
 * @callback: the function to call when the request is finished
 * @user_data: user data to be passed to @callback
 * @error: (nullable): a #GError, or %NULL to ignore errors
 *
 * Look up the contents of a state event in a room.  If both @event_type and @state_key are
 * empty, get a list of state events for that room.
 *
 * If @event_type is %NULL but @state_key is not %NULL, this function returns immediately and
 * @error is set to #MATRIX_ERROR_INCOMPLETE.
 */
void
matrix_api_get_room_state(MatrixAPI *matrix_api,
                          const gchar *room_id,
                          const gchar *event_type,
                          const gchar *state_key,
                          MatrixAPICallback callback,
                          gpointer user_data,
                          GError **error)
{
    g_return_if_fail(matrix_api != NULL);
    g_return_if_fail(room_id != NULL);

    if ((state_key != NULL) && (event_type == NULL)) {
        g_set_error(error, MATRIX_ERROR, MATRIX_ERROR_INCOMPLETE,
                    "event_type must be set if state_key is set");

        return;
    }

    MATRIX_API_GET_IFACE(matrix_api)->get_room_state(matrix_api, callback, user_data, room_id, event_type, state_key, error);
}

/**
 * matrix_api_send_state_event:
 * @api: a #MatrixAPI
 * @room_id: (not nullable): the room ID to get a state for
 * @event_type: the type of state to look up
 * @state_key: the key of the state to look up.  If @event_type is %NULL, this parameter is ignored
 * @content: (not nullable): the content of the state event
 * @callback: the function to call when the request is finished
 * @user_data: user data to be passed to @callback
 * @error: (nullable): a #GError, or %NULL to ignore errors
 *
 * Send a state event to the room. These events will be overwritten if @room_id, @event_type
 * and @state_key all match.
 *
 * This request cannot use transaction IDs.
 *
 * The required fields in the body of the request(@content) vary depending on the type of
 * the event.
 *
 * If @state_key is not %NULL but @event_type is %NULL, this function returns immediately, and
 * @error is set to #MATRIX_ERROR_INCOMPLETE.
 */
void
matrix_api_send_state_event(MatrixAPI *matrix_api,
                            const gchar *room_id,
                            const gchar *event_type,
                            const gchar *state_key,
                            JsonNode *content,
                            MatrixAPICallback callback,
                            gpointer user_data,
                            GError **error)
{
    g_return_if_fail(matrix_api != NULL);

    if ((room_id == NULL) || (content == NULL) || ((event_type == NULL) && (state_key != NULL))) {
        g_set_error(error, MATRIX_ERROR, MATRIX_ERROR_INCOMPLETE,
                    "room_id and content must be set, and event_type cannot be NULL if state_key is not NULL");

        return;
    }

    MATRIX_API_GET_IFACE(matrix_api)->send_state_event(matrix_api, callback, user_data, room_id, event_type, state_key, content, error);
}

/**
 * matrix_api_notify_room_typing:
 * @api: a #MatrixAPI
 * @user_id: (not nullable): the user who has started to type
 * @room_id: (not nullable): the room in which the user is typing
 * @timeout: the length of time in milliseconds to mark this user as typing
 * @typing: whether the user is typing or not.  If %FALSE, @timeout can be omitted (ie. set to 0)
 * @callback: the function to call when the request is finished
 * @user_data: user data to be passed to @callback
 * @error: (nullable): a #GError, or %NULL to ignore errors
 *
 * Tell the server the user is typing for the next @timeout milliseconds.  If @typing is
 * %FALSE, it tells the server that the user stopped typing.
 */
void
matrix_api_notify_room_typing(MatrixAPI *matrix_api,
                              const gchar *user_id,
                              const gchar *room_id,
                              guint timeout,
                              gboolean typing,
                              MatrixAPICallback callback,
                              gpointer user_data,
                              GError **error)
{
    g_return_if_fail(matrix_api != NULL);

    MATRIX_API_GET_IFACE(matrix_api)->notify_room_typing(matrix_api, callback, user_data, user_id, room_id, timeout, typing, error);
}

/**
 * matrix_api_sync:
 * @api: a #MatrixAPI
 * @filter_id: a filter ID created by the filter API (e.g. matrix_api_create_filter())
 * @filter: a definition on what events to fetch
 * @since: a point in time to continue a sync from
 * @full_state: if %TRUE, all state events will be returned, even if @since is not empty.
 *     If %FALSE, and @since is not empty, only states which have changed since the point
 *     indicated by @since will be returned
 * @set_presence: controls whether the client is automatically marked as online by polling
 *     this API.
 * @timeout: the maximum time to poll in milliseconds
 * @callback: the function to call when the request is finished
 * @user_data: user data to be passed to @callback
 * @error: (nullable): a #GError, or %NULL to ignore errors
 *
 * Synchronize the client’s state with the latest state on the server.  Clients should use this
 * API when they first log in to get an initial snapshot of the state on the server and then
 * continue to call this API to get incremental details to the state and to receive new messages.
 */
void
matrix_api_sync(MatrixAPI *matrix_api,
                const gchar *filter_id,
                MatrixFilter *filter,
                const gchar *since,
                gboolean full_state,
                gboolean set_presence,
                gulong timeout,
                MatrixAPICallback callback,
                gpointer user_data,
                GError **error)
{
    g_return_if_fail(matrix_api != NULL);

    MATRIX_API_GET_IFACE(matrix_api)->sync(matrix_api, callback, user_data, filter_id, filter, since, full_state, set_presence, timeout, error);
}

/**
 * matrix_api_create_filter:
 * @api: a #MatrixAPI
 * @user_id: (not nullable): the ID of the user uploading the filter. An access token must be present (either
 *     specifying one with matrix_api_set_token() or requested from the server via
 *     matrix_api_register_account() or matrix_api_login()).
 * @filter: (not nullable): the filter to upload
 * @callback: the function to call when the request is finished
 * @user_data: user data to be passed to @callback
 * @error: (nullable): a #GError, or %NULL to ignore errors
 *
 * Upload a new filter definition to the homeserver.  It will return a filter ID that may be
 * used in future requests.
 */
void
matrix_api_create_filter(MatrixAPI *matrix_api,
                         const gchar *user_id,
                         MatrixFilter *filter,
                         MatrixAPICallback callback,
                         gpointer user_data,
                         GError **error)
{
    g_return_if_fail(matrix_api != NULL);
    g_return_if_fail(user_id != NULL);
    g_return_if_fail(filter != NULL);

    MATRIX_API_GET_IFACE(matrix_api)->create_filter(matrix_api, callback, user_data, user_id, filter, error);
}

/**
 * matrix_api_download_filter:
 * @api: a #MatrixAPI
 * @user_id: (not nullable): the user ID to download a filter from
 * @filter_id: (not nullable): the filter ID to download
 * @callback: the function to call when the request is finished
 * @user_data: user data to be passed to @callback
 * @error: (nullable): a #GError, or %NULL to ignore errors
 *
 * Download a filter.
 */
void
matrix_api_download_filter(MatrixAPI *matrix_api,
                           const gchar *user_id,
                           const gchar *filter_id,
                           MatrixAPICallback callback,
                           gpointer user_data,
                           GError **error)
{
    g_return_if_fail(matrix_api != NULL);
    g_return_if_fail(user_id != NULL);
    g_return_if_fail(filter_id != NULL);

    MATRIX_API_GET_IFACE(matrix_api)->download_filter(matrix_api, callback, user_data, user_id, filter_id, error);
}

/**
 * matrix_api_join_room_id_or_alias:
 * @api: a #MatrixAPI
 * @callback: the function to call when the reuest is finished
 * @room_id_or_alias: (not nullable): the ID or alias of a room to join * @user_data: user data to be passed to @callback
 * @user_data: user data to be passed to @callback
 * @error: (nullable): a #GError, or %NULL to ignore errors
 *
 * Join a room by a room ID or an alias
 */
void
matrix_api_join_room_id_or_alias(MatrixAPI *matrix_api,
                                 const gchar *room_id_or_alias,
                                 MatrixAPICallback callback,
                                 gpointer user_data,
                                 GError **error)
{
    g_return_if_fail(matrix_api != NULL);
    g_return_if_fail(room_id_or_alias != NULL);

    MATRIX_API_GET_IFACE(matrix_api)->join_room_id_or_alias(matrix_api, callback, user_data, room_id_or_alias, error);
}

/**
 * matrix_api_ban_user:
 * @api: a #MatrixAPI
 * @callback: the function to call when the request is finished
 * @room_id: (not nullable): the room ID where the user should be banned
 * @user_id: (not nullable): the user ID to ban
 * @reason: the reason of the ban
 * @user_data: user data to be passed to @callback
 * @error: (nullable): a #GError, or %NULL to ignore errors
 *
 * Ban the specified user from the specified room. An optional reason can be specified.
 */
void
matrix_api_ban_user(MatrixAPI *matrix_api,
                    const gchar *room_id,
                    const gchar *user_id,
                    const gchar *reason,
                    MatrixAPICallback callback,
                    gpointer user_data,
                    GError **error)
{
    g_return_if_fail(matrix_api != NULL);
    g_return_if_fail(room_id != NULL);
    g_return_if_fail(user_id != NULL);

    MATRIX_API_GET_IFACE(matrix_api)->ban_user(matrix_api, callback, user_data, room_id, user_id, reason, error);
}

/**
 * matrix_api_forget_room:
 * @api: a #MatrixAPI
 * @room_id: (not nullable): the room ID to forget
 * @callback: the function to call when the request is finished
 * @user_data: user data to be passed to @callback
 * @error: (nullable): a #GError, or %NULL to ignore errors
 *
 * Stop the requesting user remembering about a particular room.
 *
 * In general, history is a first class citizen in Matrix.  After this API is called, however,
 * a user will no longer be able to retrieve history for this room.  If all users on a
 * homeserver forget a room, the room is eligible for deletion from that homeserver.
 *
 * If the user is currently joined to the room, they will implicitly leave the room as part of
 * this API call.
 */
void
matrix_api_forget_room(MatrixAPI *matrix_api,
                       const gchar *room_id,
                       MatrixAPICallback callback,
                       gpointer user_data,
                       GError **error)
{
    g_return_if_fail(matrix_api != NULL);
    g_return_if_fail(room_id != NULL);

    MATRIX_API_GET_IFACE(matrix_api)->forget_room(matrix_api, callback, user_data, room_id, error);
}

/**
 * matrix_api_invite_user_3rdparty:
 * @api: a #MatrixAPI
 * @room_id: the room ID to which to invite the user
 * @credential: (not nullable): a {@link Matrix.3PidCredential} that identifies a user to invite
 * @callback: the function to call when the request is finished
 * @user_data: user data to be passed to @callback
 * @error: (nullable): a #GError, or %NULL to ignore errors
 *
 * Invite a user to the room by a 3rd party identifier. They do not start participating in the
 * room until they actually join the room.
 *
 * If the identity server does not know a Matrix user identifier for the passed third party
 * identifier, the homeserver will issue an invitation which can be accepted upon providing
 * proof of ownership of the third party identifier.
 */
void
matrix_api_invite_user_3rdparty(MatrixAPI *matrix_api,
                                const gchar *room_id,
                                Matrix3PidCredential *credential,
                                MatrixAPICallback callback,
                                gpointer user_data,
                                GError **error)
{
    g_return_if_fail(matrix_api != NULL);
    g_return_if_fail(credential != NULL);

    MATRIX_API_GET_IFACE(matrix_api)->invite_user_3rdparty(matrix_api, callback, user_data, room_id, credential, error);
}

/**
 * matrix_api_invite_user:
 * @api: a #MatrixAPI
 * @room_id: (not nullable): the room ID to invite the user to
 * @user_id: (not nullable): the user ID to invite
 * @callback: the function to call when the request is finished
 * @user_data: user data to be passed to @callback
 * @error: (nullable): a #GError, or %NULL to ignore errors
 *
 * Invite a user to a room.
 */
void
matrix_api_invite_user(MatrixAPI *matrix_api,
                       const gchar *room_id,
                       const gchar *user_id,
                       MatrixAPICallback callback,
                       gpointer user_data,
                       GError **error)
{
    g_return_if_fail(matrix_api != NULL);
    g_return_if_fail(room_id != NULL);
    g_return_if_fail(user_id != NULL);

    MATRIX_API_GET_IFACE(matrix_api)->invite_user(matrix_api, callback, user_data, room_id, user_id, error);
}

/**
 * matrix_api_join_room:
 * @api: a #MatrixAPI
 * @room_id: (not nullable): the room ID to join to
 * @callback: the function to call when the request is finished
 * @user_data: user data to be passed to @callback
 * @error: (nullable): a #GError, or %NULL to ignore errors
 *
 * Join a room by its room ID.
 */
void
matrix_api_join_room(MatrixAPI *matrix_api,
                     const gchar *room_id,
                     MatrixAPICallback callback,
                     gpointer user_data,
                     GError **error)
{
    g_return_if_fail(matrix_api != NULL);
    g_return_if_fail(room_id != NULL);

    MATRIX_API_GET_IFACE(matrix_api)->join_room(matrix_api, callback, user_data, room_id, error);
}

/**
 * matrix_api_kick_user:
 * @api: a #MatrixAPI
 * @room_id: (not nullable): the room ID to kick the user from
 * @user_id: the user ID to kick from the room
 * @reason: a reason
 * @callback: the function to call when the request is finished
 * @user_data: user data to be passed to @callback
 * @error: (nullable): a #GError, or %NULL to ignore errors
 *
 * Kick a user from a room
 */
void
matrix_api_kick_user(MatrixAPI *matrix_api,
                     const gchar *room_id,
                     const gchar *user_id,
                     const gchar *reason,
                     MatrixAPICallback callback,
                     gpointer user_data,
                     GError **error)
{
    g_return_if_fail(matrix_api != NULL);
    g_return_if_fail(room_id != NULL);

    MATRIX_API_GET_IFACE(matrix_api)->kick_user(matrix_api, callback, user_data, room_id, user_id, reason, error);
}

/**
 * matrix_api_leave_room:
 * @api: a #MatrixAPI
 * @room_id: (not nullable): the room ID to kick the user from
 * @callback: the function to call when the request is finished
 * @user_data: user data to be passed to @callback
 * @error: (nullable): a #GError, or %NULL to ignore errors
 *
 * Leave a room.
 */
void
matrix_api_leave_room(MatrixAPI *matrix_api,
                      const gchar *room_id,
                      MatrixAPICallback callback,
                      gpointer user_data,
                      GError **error)
{
    g_return_if_fail(matrix_api != NULL);
    g_return_if_fail(room_id != NULL);

    MATRIX_API_GET_IFACE(matrix_api)->leave_room(matrix_api, callback, user_data, room_id, error);
}

/**
 * matrix_api_unban_user:
 * @api: a #MatrixAPI
 * @room_id: (not nullable): the room ID to unban the user from
 * @user_id: the user ID to unban from the room
 * @callback: the function to call when the request is finished
 * @user_data: user data to be passed to @callback
 * @error: (nullable): a #GError, or %NULL to ignore errors
 *
 * Unban a user from a room
 */
void
matrix_api_unban_user(MatrixAPI *matrix_api,
                      const gchar *room_id,
                      const gchar *user_id,
                      MatrixAPICallback callback,
                      gpointer user_data,
                      GError **error)
{
    g_return_if_fail(matrix_api != NULL);
    g_return_if_fail(room_id != NULL);

    MATRIX_API_GET_IFACE(matrix_api)->unban_user(matrix_api, callback, user_data, room_id, user_id, error);
}

/**
 * matrix_api_login:
 * @api: a #MatrixAPI
 * @login_type: the login type to use
 * @content: parameters to pass for the login request
 * @callback: the function to call when the request is finished
 * @user_data: user data to be passed to @callback
 * @error: (nullable): a #GError, or %NULL to ignore errors
 *
 * Attempt to login with type @login_type. Implementations of this method must set the #MatrixAPI:token
 * property on a successful login.
 */
void
matrix_api_login(MatrixAPI *matrix_api,
                 const gchar *login_type,
                 JsonNode *content,
                 MatrixAPICallback callback,
                 gpointer user_data,
                 GError **error)
{
    g_return_if_fail(matrix_api != NULL);
    g_return_if_fail(login_type != NULL);
    g_return_if_fail(content != NULL);

    MATRIX_API_GET_IFACE(matrix_api)->login(matrix_api, callback, user_data, login_type, content, error);
}

/**
 * matrix_api_logout:
 * @api: a #MatrixAPI
 * @callback: the function to call when the request is finished
 * @user_data: user data to be passed to @callback
 * @error: (nullable): a #GError, or %NULL to ignore errors
 *
 * Logout from the current session, invalidating the access token.
 */
void
matrix_api_logout(MatrixAPI *matrix_api,
                  MatrixAPICallback callback,
                  gpointer user_data,
                  GError **error)
{
    g_return_if_fail(matrix_api != NULL);

    MATRIX_API_GET_IFACE(matrix_api)->logout(matrix_api, callback, user_data, error);
}

/**
 * matrix_api_get_presence_list:
 * @api: a #MatrixAPI
 * @user_id: (not nullable): the user whose presence list should be retrieved
 * @callback: a function to call when the request is finished
 * @user_data: user data to be passed to @callback
 * @error: (nullable): a #GError, or %NULL to ignore errors
 *
 * Retrieve a list of presence events for every user on this list.
 */
void
matrix_api_get_presence_list(MatrixAPI *matrix_api,
                             const gchar *user_id,
                             MatrixAPICallback callback,
                             gpointer user_data,
                             GError **error)
{
    g_return_if_fail(matrix_api != NULL);
    g_return_if_fail(user_id != NULL);

    MATRIX_API_GET_IFACE(matrix_api)->get_presence_list(matrix_api, callback, user_data, user_id, error);
}

/**
 * matrix_api_update_presence_list:
 * @api: a #MatrixAPI
 * @user_id: (not nullable): the user whose presence list is being modified
 * @drop_ids: a list of user IDs to remove from the list
 * @n_drop_ids: the length of @drop_ids
 * @invite_ids: a list of user IDs to add to the list
 * @n_invite_ids: the length of @invite_ids
 * @callback: a function to call when the request is finished
 * @user_data: user data to be passed to @callback
 * @error: (nullable): a #GError, or %NULL to ignore errors
 *
 * Add or remove users from the specified user’s presence list.
 *
 * If both @drop_ids and @invite_ids are %NULL, this function returns immediately and @error
 * is set to #MATRIX_ERROR_INCOMPLETE.
 */
void
matrix_api_update_presence_list(MatrixAPI *matrix_api,
                                const gchar *user_id,
                                gchar **drop_ids,
                                int drop_ids_length1,
                                gchar **invite_ids,
                                int invite_ids_length1,
                                MatrixAPICallback callback,
                                gpointer user_data,
                                GError **error)
{
    g_return_if_fail(matrix_api != NULL);
    g_return_if_fail(user_id != NULL);

    if ((drop_ids == NULL) && (invite_ids == NULL)) {
        g_set_error(error, MATRIX_ERROR, MATRIX_ERROR_INCOMPLETE,
                    "At least one of drop_ids and invite_ids must be set.");

        return;
    }

    MATRIX_API_GET_IFACE(matrix_api)->update_presence_list(matrix_api, callback, user_data, user_id, drop_ids, drop_ids_length1, invite_ids, invite_ids_length1, error);
}

/**
 * matrix_api_get_presence:
 * @api: a #MatrixAPI
 * @user_id: (not nullable): the user whose presence list is being modified
 * @callback: a function to call when the request is finished
 * @user_data: user data to be passed to @callback
 * @error: (nullable): a #GError, or %NULL to ignore errors
 *
 * Get the given user’s presence state.
 */
void
matrix_api_get_presence(MatrixAPI *matrix_api,
                        const gchar *user_id,
                        MatrixAPICallback callback,
                        gpointer user_data,
                        GError **error)
{
    g_return_if_fail(matrix_api != NULL);
    g_return_if_fail(user_id != NULL);

    MATRIX_API_GET_IFACE(matrix_api)->get_presence(matrix_api, callback, user_data, user_id, error);
}

/**
 * matrix_api_set_presence:
 * @api: a #MatrixAPI
 * @user_id: (not nullable): the user whose presence list is being modified
 * @presence: the new presence state
 * @status_message: a status message attached to this state
 * @callback: a function to call when the request is finished
 * @user_data: user data to be passed to @callback
 * @error: (nullable): a #GError, or %NULL to ignore errors
 *
 * Set the given user’s presence.  You cannot set the presence of another user.
 */
void
matrix_api_set_presence(MatrixAPI *matrix_api,
                        const gchar *user_id,
                        MatrixPresence presence,
                        const gchar *status_message,
                        MatrixAPICallback callback,
                        gpointer user_data,
                        GError **error)
{
    g_return_if_fail(matrix_api != NULL);
    g_return_if_fail(user_id != NULL);

    MATRIX_API_GET_IFACE(matrix_api)->set_presence(matrix_api, callback, user_data, user_id, presence, status_message, error);
}

/**
 * matrix_api_list_public_rooms:
 * @api: a #MatrixAPI
 * @callback: the function to call when the request is finished
 * @user_data: user data to be passed to @callback
 * @error: (nullable): a #GError, or %NULL to ignore errors
 *
 * List the public rooms on the server.
 */
void
matrix_api_list_public_rooms(MatrixAPI *matrix_api, MatrixAPICallback callback, gpointer user_data, GError **error)
{
    g_return_if_fail(matrix_api != NULL);

    MATRIX_API_GET_IFACE(matrix_api)->list_public_rooms(matrix_api, callback, user_data, error);
}

/**
 * matrix_api_get_pushers:
 * @api: a #MatrixAPI
 * @callback: a function to call when the request is finished
 * @user_data: user data to be passed to @callback
 * @error: (nullable): a #GError, or %NULL to ignore errors
 *
 * Retrieve all push rulesets.
 */
void
matrix_api_get_pushers(MatrixAPI *matrix_api,
                       MatrixAPICallback callback,
                       gpointer user_data,
                       GError **error)
{
    g_return_if_fail(matrix_api != NULL);

    MATRIX_API_GET_IFACE(matrix_api)->get_pushers(matrix_api, callback, user_data, error);
}

/**
 * matrix_api_update_pusher:
 * @api: a #MatrixAPI
 * @pusher: (not nullable): a #MatrixPusher
 * @callback: a function to call when the request is finished
 * @user_data: user data to be passed to @callback
 * @error: (nullable): a #GError, or %NULL to ignore errors
 *
 * Create, update or delete a pusher for the active user on this homeserver.
 */
void
matrix_api_update_pusher(MatrixAPI *matrix_api,
                         MatrixPusher *pusher,
                         MatrixAPICallback callback,
                         gpointer user_data,
                         GError **error)
{
    g_return_if_fail(matrix_api != NULL);

    MATRIX_API_GET_IFACE(matrix_api)->update_pusher(matrix_api, callback, user_data, pusher, error);
}

/**
 * matrix_api_get_pushrules:
 * @api: a #MatrixAPI
 * @callback: a function to call when the request is finished
 * @user_data: user data to be passed to @callback
 * @error: (nullable): a #GError, or %NULL to ignore errors
 *
 * Retrieve all push rulesets.
 */
void
matrix_api_get_pushrules(MatrixAPI *matrix_api,
                         MatrixAPICallback callback,
                         gpointer user_data,
                         GError **error)
{
    g_return_if_fail(matrix_api != NULL);

    MATRIX_API_GET_IFACE(matrix_api)->get_pushrules(matrix_api, callback, user_data, error);
}

/**
 * matrix_api_delete_pushrule:
 * @api: a #MatrixAPI
 * @scope: (not nullable): either `global` to specify global rules, or
 *     `device/&lt;profile tag&gt;` for rules for a given `profile tag`
 * @kind: the kind of rule
 * @rule_id: (not nullable): an identifier for the rule
 * @callback: a function to call when the request is finished
 * @user_data: user data to be passed to @callback
 * @error: (nullable): a #GError, or %NULL to ignore errors
 *
 * Delete a push rule.
 */
void
matrix_api_delete_pushrule(MatrixAPI *matrix_api,
                           const gchar *scope,
                           MatrixPusherKind kind,
                           const gchar *rule_id,
                           MatrixAPICallback callback,
                           gpointer user_data,
                           GError **error)
{
    g_return_if_fail(matrix_api != NULL);
    g_return_if_fail(scope != NULL);
    g_return_if_fail(rule_id != NULL);

    MATRIX_API_GET_IFACE(matrix_api)->delete_pushrule(matrix_api, callback, user_data, scope, kind, rule_id, error);
}

/**
 * matrix_api_get_pushrule:
 * @api: a #MatrixAPI
 * @scope: (not nullable): either `global` to specify global rules, or
 *     `device/&lt;profile tag&gt;` for rules for a given `profile tag`.
 * @kind: the kind of rule
 * @rule_id: (not nullable): an identifier for the rule
 * @callback: a function to call when the request is finished
 * @user_data: user data to be passed to @callback
 * @error: (nullable): a #GError, or %NULL to ignore errors
 *
 * Retrieve a specific push rule.
 */
void
matrix_api_get_pushrule(MatrixAPI *matrix_api,
                        const gchar *scope,
                        MatrixPusherKind kind,
                        const gchar *rule_id,
                        MatrixAPICallback callback,
                        gpointer user_data,
                        GError **error)
{
    g_return_if_fail(matrix_api != NULL);
    g_return_if_fail(scope != NULL);
    g_return_if_fail(rule_id != NULL);

    MATRIX_API_GET_IFACE(matrix_api)->get_pushrule(matrix_api, callback, user_data, scope, kind, rule_id, error);
}

/**
 * matrix_api_add_pushrule:
 * @api: a #MatrixAPI
 * @scope: (not nullable): either `global` to specify global rules, or
 *     `device/&lt;profile tag&gt;` for rules for a given `profile tag`
 * @kind: the kind of rule
 * @rule_id: (not nullable): an identifier for the rule
 * @before: make the new rule the next-most important than this rule ID
 * @after: make the new rule the next-less important than this rule ID
 * @actions: (not nullable): the actions to perform when the conditions for this rule are met
 * @n_actions: the length of @actions
 * @conditions: the conditions that must hold true for an event for a rule to be applied.
 *     A rule with no conditions always matches
 * @n_conditions: the length of @conditions
 * @callback: a function to call when the request is finished
 * @user_data: user data to be passed to @callback
 * @error: (nullable): a #GError, or %NULL to ignore errors
 *
 * Add or change a push rule.
 */
void
matrix_api_add_pushrule(MatrixAPI *matrix_api,
                        const gchar *scope,
                        MatrixPusherKind kind,
                        const gchar *rule_id,
                        const gchar *before,
                        const gchar *after,
                        gchar **actions,
                        int actions_length1,
                        MatrixPusherConditionKind *conditions,
                        int conditions_length1,
                        MatrixAPICallback callback,
                        gpointer user_data,
                        GError **error)
{
    g_return_if_fail(matrix_api != NULL);
    g_return_if_fail(scope != NULL);
    g_return_if_fail(rule_id != NULL);
    g_return_if_fail(actions != NULL);

    MATRIX_API_GET_IFACE(matrix_api)->add_pushrule(matrix_api, callback, user_data, scope, kind, rule_id, before, after, actions, actions_length1, conditions, conditions_length1, error);
}

/**
 * matrix_api_toggle_pushrule:
 * @api: a #MatrixAPI
 * @scope: (not nullable): either `global` to specify global rules, or
 *     `device/&lt;profile tag&gt;` for rules for a given `profile tag`
 * @kind: the kind of rule
 * @rule_id: (not nullable): an identifier for the rule
 * @enabled: if %TRUE, the rule will be enabled, otherwise it gets disabled
 * @callback: a function to call when the request is finished
 * @user_data: user data to be passed to @callback
 * @error: (nullable): a #GError, or %NULL to ignore errors
 *
 * Enable or disable the specified push rule.
 */
void
matrix_api_toggle_pushrule(MatrixAPI *matrix_api,
                           const gchar *scope,
                           MatrixPusherKind kind,
                           const gchar *rule_id,
                           gboolean enabled,
                           MatrixAPICallback callback,
                           gpointer user_data,
                           GError **error)
{
    g_return_if_fail(matrix_api != NULL);
    g_return_if_fail(scope != NULL);
    g_return_if_fail(rule_id != NULL);

    MATRIX_API_GET_IFACE(matrix_api)->toggle_pushrule(matrix_api, callback, user_data, scope, kind, rule_id, enabled, error);
}

/**
 * matrix_api_search:
 * @api: a #MatrixAPI
 * @next_batch: the point to return events from. If given, this should be a next_batch result
 *     from a previous call to this method
 * @search_categories: describes which categories to search, and their criteria
 * @callback: a function to call when the request is finished * @user_data: user data to be passed to @callback
 * @user_data: user data to be passed to @callback
 * @error: (nullable): a #GError, or %NULL to ignore errors
 *
 * Perform a server side search.
 */
void
matrix_api_search(MatrixAPI *matrix_api,
                  const gchar *next_batch,
                  MatrixSearchCategories *search_categories,
                  MatrixAPICallback callback,
                  gpointer user_data,
                  GError **error)
{
    g_return_if_fail(matrix_api != NULL);

    MATRIX_API_GET_IFACE(matrix_api)->search(matrix_api, callback, user_data, next_batch, search_categories, error);
}

/**
 * matrix_api_get_turn_server:
 * @api: a #MatrixAPI
 * @callback: the function to call when the request is finished
 * @user_data: user data to be passed to @callback
 * @error: (nullable): a #GError, or %NULL to ignore errors
 *
 * Get credentials for the client to use when initiating calls.
 */
void
matrix_api_get_turn_server(MatrixAPI *matrix_api,
                           MatrixAPICallback callback,
                           gpointer user_data,
                           GError **error)
{
    g_return_if_fail(matrix_api != NULL);

    MATRIX_API_GET_IFACE(matrix_api)->get_turn_server(matrix_api, callback, user_data, error);
}

/**
 * matrix_api_media_download:
 * @api: a #MatrixAPI
 * @server_name: (not nullable): the server name from the `mxc:` URI
 * @media_id: (not nullable): the media ID from the `mxc:` URI
 * @callback: a function to call when the request is finished
 * @user_data: user data to be passed to @callback
 * @error: (nullable): a #GError, or %NULL to ignore errors
 *
 * Download content from the content repository.
 */
void
matrix_api_media_download(MatrixAPI *matrix_api,
                          const gchar *server_name,
                          const gchar *media_id,
                          MatrixAPICallback callback,
                          gpointer user_data,
                          GError **error)
{
    g_return_if_fail(matrix_api != NULL);
    g_return_if_fail(server_name != NULL);
    g_return_if_fail(media_id != NULL);

    MATRIX_API_GET_IFACE(matrix_api)->media_download(matrix_api, callback, user_data, server_name, media_id, error);
}

/**
 * matrix_api_media_thumbnail:
 * @api: a #MatrixAPI
 * @server_name: (not nullable): the server name from the `mxc:` URI
 * @media_id: (not nullable): the media ID from the `mxc:` URI
 * @width: the width of the media, in pixels
 * @height: the height of the media, in pixels
 * @method: the resize method to use
 * @callback: a function to call when the request is finished
 * @user_data: user data to be passed to @callback
 * @error: (nullable): a #GError, or %NULL to ignore errors
 *
 * Download a thumbnail of the content from the content repository.  The actual thumbnail may
 * not match the size specified.
 */
void
matrix_api_media_thumbnail(MatrixAPI *matrix_api,
                           const gchar *server_name,
                           const gchar *media_id,
                           guint width,
                           guint height,
                           MatrixResizeMethod method,
                           MatrixAPICallback callback,
                           gpointer user_data,
                           GError **error)
{
    g_return_if_fail(matrix_api != NULL);
    g_return_if_fail(server_name != NULL);
    g_return_if_fail(media_id != NULL);

    MATRIX_API_GET_IFACE(matrix_api)->media_thumbnail(matrix_api, callback, user_data, server_name, media_id, width, height, method, error);
}

/**
 * matrix_api_media_upload:
 * @api: a #MatrixAPI
 * @content_type: the type of the content to be uploaded
 * @content: (not nullable): the content to be uploaded
 * @callback: a function to call when the request is finished
 * @user_data: user data to be passed to @callback
 * @error: (nullable): a #GError, or %NULL to ignore errors
 *
 * Upload some content to the content repository.
 */
void
matrix_api_media_upload(MatrixAPI *matrix_api,
                        const gchar *content_type,
                        GByteArray *content,
                        MatrixAPICallback callback,
                        gpointer user_data,
                        GError **error)
{
    g_return_if_fail(matrix_api != NULL);
    g_return_if_fail(content != NULL);

    MATRIX_API_GET_IFACE(matrix_api)->media_upload(matrix_api, callback, user_data, content_type, content, error);
}

/**
 * matrix_api_get_token:
 * @api: a #MatrixAPI
 *
 * Get the access token used in @api.
 *
 * The returned value is owned by @api and should not be freed.
 *
 * Returns: (transfer none) (nullable): an access token, or %NULL if not yet set
 */
const gchar *
matrix_api_get_token(MatrixAPI *matrix_api)
{
    g_return_val_if_fail(matrix_api != NULL, NULL);

    return MATRIX_API_GET_IFACE(matrix_api)->get_token(matrix_api);
}

/**
 * matrix_api_set_token:
 * @api: a #MatrixAPI
 * @token: (transfer none): an access token
 *
 * Set the access token to be used for authorization while using @api.
 */
void
matrix_api_set_token(MatrixAPI *matrix_api, const gchar *value)
{
    g_return_if_fail(matrix_api != NULL);

    MATRIX_API_GET_IFACE(matrix_api)->set_token(matrix_api, value);
}

/**
 * matrix_api_get_user_id:
 * @api: a #MatrixAPI
 *
 * Get the user ID of the user logged in using @api.
 *
 * The returned value is owned by @api and should not be freed.
 *
 * Returns: (transfer none) (nullable): a Matrix ID, or %NULL if not yet set
 */
const gchar *
matrix_api_get_user_id(MatrixAPI *matrix_api)
{
    g_return_val_if_fail(matrix_api != NULL, NULL);

    return MATRIX_API_GET_IFACE(matrix_api)->get_user_id(matrix_api);
}

/**
 * matrix_api_set_user_id:
 * @api: a #MatrixAPI
 * @user_id: (nullable): a Matrix ID
 *
 * Set the user ID of the user logged in using @api.  This might be useful for restoring state.
 */
void
matrix_api_set_user_id(MatrixAPI *matrix_api, const gchar *user_id)
{
    g_return_if_fail(matrix_api != NULL);

    MATRIX_API_GET_IFACE(matrix_api)->set_user_id(matrix_api, user_id);
}

/**
 * matrix_api_get_homeserver:
 * @api: a #MatrixAPI
 *
 * Get the name of the homeserver, as it calls itself.
 *
 * The returned value is owned by @api and should not be freed.
 *
 * Returns: (transfer none) (nullable): a homeserver name
 */
const gchar *
matrix_api_get_homeserver(MatrixAPI *matrix_api)
{
    g_return_val_if_fail(matrix_api != NULL, NULL);

    return MATRIX_API_GET_IFACE(matrix_api)->get_homeserver(matrix_api);
}

/**
 * matrix_api_set_homeserver:
 * @api: a #MatrixAPI
 * @homeserver: (nullable): a homeserver name
 *
 * Set the name of the homeserver, as it calls itself.  This might be useful for
 * restoring state.
 */
void
matrix_api_set_homeserver(MatrixAPI *api, const gchar *homeserver)
{
    g_return_if_fail(api != NULL);

    MATRIX_API_GET_IFACE(api)->set_homeserver(api, homeserver);
}

static void
matrix_api_default_init(MatrixAPIInterface *iface)
{
    static gboolean initialized = FALSE;

    if(!initialized) {
        initialized = TRUE;

        /**
         * MatrixAPI:token:
         *
         * The token to use for authorization.  The matrix_api_login() and
         * matrix_api_register_account() calls MUST set this automatically.
         */
        g_object_interface_install_property(iface,
                                            g_param_spec_string(
                                                    "token", "token", "token",
                                                    NULL,
                                                    G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE));

        /**
         * MatrixAPI:user-id:
         *
         * The Matrix user ID that is currently authenticated with the server.  It is set
         * automatically by the registration and login process.
         */
        g_object_interface_install_property(iface,
                                            g_param_spec_string(
                                                    "user-id", "user-id", "user-id",
                                                    NULL,
                                                    G_PARAM_STATIC_STRINGS | G_PARAM_READABLE));

        /**
         * MatrixAPI:homeserver:
         *
         * The name of the Matrix home server as it calls itmatrix_api.  It is set
         * automatically by the registration and login process.
         */
        g_object_interface_install_property(iface,
                                            g_param_spec_string(
                                                    "homeserver", "homeserver", "homeserver",
                                                    NULL,
                                                    G_PARAM_STATIC_STRINGS | G_PARAM_READABLE));
    }
}
