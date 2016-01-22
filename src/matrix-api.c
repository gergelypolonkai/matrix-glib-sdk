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
 *                     like #MatrixHTTPAPI
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
 * @get_token: virtual function for matrix_api_get_token()
 * @set_token: virtual function for matrix_api_set_token()
 * @get_refresh_token: virtual function for matrix_api_get_refresh_token()
 * @set_refresh_token: virtual function for matrix_api_set_refresh_token()
 * @get_user_id: virtual function for matrix_api_get_user_id()
 * @get_homeserver: virtual function for matrix_api_get_homeserver()
 * @media_download: virtual function for matrix_api_media_download()
 * @media_thumbnail: virtual function for matrix_api_media_thumbnail()
 * @media_upload: virtual function for matrix_api_media_upload()
 * @get_presence_list: virtual function for matrix_api_get_presence_list()
 * @update_presence_list: virtual function for matrix_api_update_presence_list()
 * @get_user_presence: virtual function for matrix_api_get_user_presence()
 * @set_user_presence: virtual function for matrix_api_set_user_presence()
 * @update_pusher: virtual function for matrix_api_update_pusher()
 * @get_pushers: virtual function for matrix_api_get_pushers()
 * @delete_pusher: virtual function for matrix_api_delete_pusher()
 * @get_pusher: virtual function for matrix_api_get_pusher()
 * @add_pusher: virtual function for matrix_api_add_pusher()
 * @toggle_pusher: virtual function for matrix_api_toggle_pusher()
 * @create_room: virtual function for matrix_api_create_room()
 * @delete_room_alias: virtual function for matrix_api_delete_room_alias()
 * @get_room_id: virtual function for matrix_api_get_room_id()
 * @create_room_alias: virtual function for matrix_api_create_room_alias()
 * @list_public_rooms: virtual function for matrix_api_list_public_rooms()
 * @ban_user: virtual function for matrix_api_ban_user()
 * @forget_room: virtual function for matrix_api_forget_room()
 * @invite_user_3rdparty: virtual function for matrix_api_invite_user_3rdparty()
 * @invite_user: virtual function for matrix_api_invite_user()
 * @join_room: virtual function for matrix_api_join_room()
 * @leave_room: virtual function for matrix_api_leave_room()
 * @event_stream: virtual function for matrix_api_event_stream()
 * @get_event: virtual function for matrix_api_get_event()
 * @initial_sync: virtual function for matrix_api_initial_sync()
 * @get_event_context: virtual function for matrix_api_get_event_context()
 * @initial_sync_room: virtual function for matrix_api_initial_sync_room()
 * @list_room_members: virtual function for matrix_api_list_room_members()
 * @list_room_messages: virtual function for matrix_api_list_room_messages()
 * @send_event_receipt: virtual function for matrix_api_send_event_receipt()
 * @redact_event: virtual function for matrix_api_redact_event()
 * @send_message_event: virtual function for matrix_api_send_message_event()
 * @get_room_state: virtual function for matrix_api_get_room_state()
 * @send_room_event: virtual function for matrix_api_send_room_event()
 * @notify_room_typing: virtual function for matrix_api_notify_room_typing()
 * @sync: virtual function for matrix_api_sync()
 * @create_filter: virtual function for matrix_api_create_filter()
 * @download_filter: virtual function for matrix_api_download_filter()
 * @whois: virtual function for matrix_api_whois()
 * @versions: virtual function for matrix_api_versions()
 * @login: virtual function for matrix_api_login()
 * @token_refresh: virtual function for matrix_api_token_refresh()
 * @get_3pids: virtual function for matrix_api_get_3pids()
 * @add_3pid: virtual function for matrix_api_add_3pid()
 * @change_password: virtual function for matrix_api_change_password()
 * @get_profile: virtual function for matrix_api_get_profile()
 * @get_avatar_url: virtual function for matrix_api_get_avatar_url()
 * @set_avatar_url: virtual function for matrix_api_set_avatar_url()
 * @get_display_name: virtual function for matrix_api_get_display_name()
 * @set_display_name: virtual function for matrix_api_set_display_name()
 * @register_account: virtual function for matrix_api_register_account()
 * @set_account_data: virtual function for matrix_api_set_account_data()
 * @get_room_tags: virtual function for matrix_api_get_room_tags()
 * @delete_room_tag: virtual function for matrix_api_delete_room_tag()
 * @add_room_tag: virtual function for matrix_api_add_room_tag()
 * @get_turn_server: virtual function for matrix_api_get_turn_server()
 * @abort_pending: virtual function for matrix_api_abort_pending()
 *
 * The interface vtable for #MatrixAPI
 */

/**
 * MatrixAPICallback:
 * @api: A #MatrixAPI implementation
 * @content_type: the content_type of the response
 * @json_content: (allow-none): the JSON content of the response, as a
 *                #JsonNode
 * @raw_content: (allow-none): the raw content of the response
 * @user_data: User data specified when calling original request function
 * @err: a #GError. It will hold any errors from the underlying API
 *       (including communication or type errors)
 *
 * A callback function to use with API calls. Either @json_content or
 * @raw_content will be set.
 */

G_DEFINE_INTERFACE(MatrixAPI, matrix_api, G_TYPE_OBJECT);

static void
matrix_api_default_init(MatrixAPIInterface *iface)
{
    /**
     * MatrixAPI:token:
     *
     * The token to use for authorization. The matrix_api_login() and
     * matrix_api_register_account() calls must set this
     * automatically.
     */
    g_object_interface_install_property(
            iface,
            g_param_spec_string("token", "Authorization token",
                                "The authorization token to use in requests",
                                NULL,
                                G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

    /**
     * MatrixAPI:refresh-token:
     *
     * The token to use for refreshing the authorization token. It is
     * issued by the server after a successful registration, login,
     * and token refresh, but can also be changed with
     * matrix_api_set_refresh_token() (or setting this property).
     */
    g_object_interface_install_property(
            iface,
            g_param_spec_string("refresh-token", "Refresh token",
                                "The token issued by the server for authorization token renewal",
                                NULL,
                                G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

    /**
     * MatrixAPI:user-id:
     *
     * The Matrix user ID that is currently authenticated to the
     * server. It is set by the registration and login processes
     * automatically, and cannot be set from outside.
     */
    g_object_interface_install_property(
            iface,
            g_param_spec_string("user-id", "User ID",
                                "The Matrix user ID that is authenticated to the server",
                                NULL,
                                G_PARAM_READABLE));

    /**
     * MatrixAPI:homeserver:
     *
     * The Matrix home server, as it calls itself. It is filled
     * automatically by login and register calls.
     */
    g_object_interface_install_property(
            iface,
            g_param_spec_string("homeserver", "Homeserver",
                                "The home server, as it calls itself",
                                NULL,
                                G_PARAM_READABLE));
}

/* Property getters and setters */

/**
 * matrix_api_get_token:
 * @api: a #MatrixAPI implementation
 *
 * Get the authorization token currently set. See
 * matrix_api_set_token() for limitations.
 *
 * Returns: (transfer none) (allow-none): the authorization token that
 *          will be used in subsequent requests, or %NULL if none
 *          set. The returned value is owned by the @api object and
 *          should not be freed nor modified
 */
const gchar *
matrix_api_get_token(MatrixAPI *api)
{
    g_return_val_if_fail(MATRIX_IS_API(api), NULL);

    return MATRIX_API_GET_IFACE(api)
        ->get_token(api);
}

/**
 * matrix_api_set_token:
 * @api: a #MatrixAPI implementation
 * @token: (allow-none): the authorization token to set
 *
 * Set the authorization token to use in subsequent requests. If
 * @token is %NULL, the token will be unset, and a new login must take
 * place to get a new authorization token.
 *
 * <warning><para>Some implementations, like #MatrixHTTPAPI, use
 * asynchronous requests. This means that pending requests will use
 * the old token and thus, may fail because of this.</para></warning>
 */
void
matrix_api_set_token(MatrixAPI *api, const gchar *token)
{
    g_return_if_fail(MATRIX_IS_API(api));

    MATRIX_API_GET_IFACE(api)
        ->set_token(api, token);
}

/**
 * matrix_api_get_refresh_token:
 * @api: a #MatrixAPI implementation
 *
 * Get the refresh token currently set. See matrix_api_set_token() for
 * limitations.
 *
 * Returns: (transfer none) (allow-none): the refresh token to be used
 *          for authorization token refreshment, or %NULL if there is
 *          none set. The returned value is owned by the @api object
 *          and should not be freed nor modified
 */
const gchar *
matrix_api_get_refresh_token(MatrixAPI *api)
{
    g_return_val_if_fail(MATRIX_IS_API(api), NULL);

    return MATRIX_API_GET_IFACE(api)
        ->get_refresh_token(api);
}

/**
 * matrix_api_set_refresh_token:
 * @api: a #MatrixAPI implementation
 * @refresh_token: (allow-none): the refresh token to set
 *
 * Set the refresh token to be used in subsequent requests. See
 * matrix_api_set_token() for limitations. If @refresh_token is %NULL,
 * the refresh token is unset, and a new login must take place to get
 * a new one.
 */
void
matrix_api_set_refresh_token(MatrixAPI *api, const gchar *refresh_token)
{
    g_return_if_fail(MATRIX_IS_API(api));

    MATRIX_API_GET_IFACE(api)
        ->set_refresh_token(api, refresh_token);
}

/**
 * matrix_api_get_user_id:
 * @api: a #MatrixAPI implementation
 *
 * Get the Matrix user ID that is currently authenticated with the
 * server.
 *
 * Returns: (transfer none) (allow-none): the Matrix user ID
 *          authenticated by the last successful register or login
 *          call (ie. matrix_api_login() or
 *          matrix_api_register_account()). If no user is
 *          authenticated, this function returns %NULL. The returned
 *          value is owned by the @api object and should not be freed
 *          nor modified
 */
const gchar *
matrix_api_get_user_id(MatrixAPI *api)
{
    g_return_val_if_fail(MATRIX_IS_API(api), NULL);

    return MATRIX_API_GET_IFACE(api)
        ->get_user_id(api);
}

/**
 * matrix_api_get_homeserver:
 * @api: a #MatrixAPI implementation
 *
 * Get the homeserver's name, as it calls itself. It gets set
 * automatically by login and register calls, e.g. matrix_api_login()
 * or matrix_api_register_account().
 *
 * Returns: (transfer none) (allow-none): the Matrix homeserver's
 * name, as it calls itself. If no homeserver name is reported yet
 * (e.g. because login or register wasn't called yet), this function
 * returns %NULL.
 */
const gchar *
matrix_api_get_homeserver(MatrixAPI *api)
{
    g_return_val_if_fail(MATRIX_IS_API(api), NULL);

    return MATRIX_API_GET_IFACE(api)
        ->get_homeserver(api);
}

/* Media */

/**
 * matrix_api_media_download:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async) (allow-none): a function to call when the
 *            request is finished
 * @user_data: (closure): user data to pass to the callback function
 *             @callback
 * @server_name: the server name from the <code>mxc://</code> URI (the
 *               authority component)
 * @media_id: the media ID from the <code>mxc://</code> URI (the path
 *            component)
 * @error: return location for a #GError, or %NULL
 *
 * Download content from the content repository.
 *
 * If @server_name or @media_id is %NULL, this function returns
 * immediately, and fills @error with %MATRIX_API_ERROR_INCOMPLETE.
 */
void
matrix_api_media_download(MatrixAPI *api,
                          MatrixAPICallback callback,
                          gpointer user_data,
                          const gchar *server_name,
                          const gchar *media_id,
                          GError **error)
{
    g_return_if_fail(MATRIX_IS_API(api));

    if (!server_name || !media_id) {
        g_set_error(error,
                    MATRIX_API_ERROR, MATRIX_API_ERROR_INCOMPLETE,
                    "server_name and media_id must be specified.");

        return;
    }

    MATRIX_API_GET_IFACE(api)
        ->media_download(api,
                         callback, user_data,
                         server_name, media_id, error);
}

/**
 * matrix_api_media_thumbnail:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async) (allow-none): a function to call when the
 *            request is finished
 * @user_data: (closure): user data to pass to the callback function
 *             @callback
 * @server_name: the server name from the <code>mxc://</code> URI (the
 *               authority component)
 * @media_id: the media ID from the <code>mxc://</code> URI (the path
 *            component)
 * @width: the desired width of the thumbnail, or 0 to use the default
 * @height: the desired height of the thumbnail, or 0 to use the
 *          default
 * @method: the resizing method to use
 * @error: return location for a #GError, or %NULL
 *
 * Download a thumbnail of the content from the content
 * repository. The actual thumbnail may not match the size
 * specified.
 *
 * If @server_name or @media_id is %NULL, this function returns
 * immediately, and fills @error with %MATRIX_API_ERROR_INCOMPLETE.
 */
void
matrix_api_media_thumbnail(MatrixAPI *api,
                           MatrixAPICallback callback,
                           gpointer user_data,
                           const gchar *server_name,
                           const gchar *media_id,
                           guint width,
                           guint height,
                           MatrixAPIResizeMethod method,
                           GError **error)
{
    g_return_if_fail(MATRIX_IS_API(api));

    if (!server_name || !media_id) {
        g_set_error(error,
                    MATRIX_API_ERROR, MATRIX_API_ERROR_INCOMPLETE,
                    "server_name and media_id must be specified.");

        return;
    }

    MATRIX_API_GET_IFACE(api)
        ->media_thumbnail(api,
                          callback, user_data,
                          server_name, media_id,
                          width, height, method,
                          error);
}

/**
 * matrix_api_media_upload:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async) (allow-none): a function to call when the
 *            request is finished
 * @user_data: (closure): user data to pass to the callback function
 *             @callback
 * @content_type: (allow-none): the content type of the file being
 *                uploaded
 * @content: (transfer full): the content to be uploaded
 * @error: return location for a #GError, or %NULL
 *
 * Upload some content to the content repository.
 *
 * If @content is %NULL, this function returns immediately, and fills
 * @error with %MATRIX_API_ERROR_INCOMPLETE.
 */
void
matrix_api_media_upload(MatrixAPI *api,
                        MatrixAPICallback callback,
                        gpointer user_data,
                        const gchar *content_type,
                        GByteArray *content,
                        GError **error)
{
    g_return_if_fail(MATRIX_IS_API(api));

    if (!content) {
        g_set_error(error,
                    MATRIX_API_ERROR, MATRIX_API_ERROR_INCOMPLETE,
                    "content must be set.");

        return;
    }

    MATRIX_API_GET_IFACE(api)
        ->media_upload(api, callback, user_data, content_type, content, error);
}

/* Presence */

/**
 * matrix_api_get_presence_list:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async) (allow-none): a function to call when the
 *            request is finished
 * @user_data: (closure): user data to pass to the callback function
 *             @callback
 * @user_id: the user whose presence list should be retrieved
 * @error: return location for a #GError, or %NULL
 *
 * Retrieve a list of presence events for every user on this list.
 *
 * If @user_id is %NULL, this function returns immediately, and fills
 * @error with %MATRIX_API_ERROR_INCOMPLETE.
 */
void
matrix_api_get_presence_list(MatrixAPI *api,
                             MatrixAPICallback callback,
                             gpointer user_data,
                             const gchar *user_id,
                             GError **error)
{
    g_return_if_fail(MATRIX_IS_API(api));

    if (!user_id) {
        g_set_error(error,
                    MATRIX_API_ERROR, MATRIX_API_ERROR_INCOMPLETE,
                    "user_id must be set.");

        return;
    }

    MATRIX_API_GET_IFACE(api)
        ->get_presence_list(api, callback, user_data, user_id, error);
}

/**
 * matrix_api_update_presence_list:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async) (allow-none): a function to call when the
 *            request is finished
 * @user_data: (closure): user data to pass to the callback function
 *             @callback
 * @user_id: the user whose presence list is being modified
 * @drop_ids: (element-type utf8): a list of user IDs to remove
 *            from the list
 * @invite_ids: (element-type utf8): a list of user IDs to add to
 *              the list
 * @error: return location for a #GError, or %NULL
 *
 * Add or remove users from the specified user's presence list.
 *
 * If @user_id, or both @drop_ids and @invite_ids are %NULL, this
 * function returns immediately, and fills @error with
 * %MATRIX_API_ERROR_INCOMPLETE.
 */
void
matrix_api_update_presence_list(MatrixAPI *api,
                                MatrixAPICallback callback,
                                gpointer user_data,
                                const gchar *user_id,
                                GList *drop_ids,
                                GList *invite_ids,
                                GError **error)
{
    g_return_if_fail(MATRIX_IS_API(api));

    if (!user_id || (!drop_ids && !invite_ids)) {
        g_set_error(error,
                    MATRIX_API_ERROR, MATRIX_API_ERROR_INCOMPLETE,
                    "user_id must be set, so as at least one of drop_ids or invite_ids");

        return;
    }

    MATRIX_API_GET_IFACE(api)
        ->update_presence_list(api,
                               callback, user_data,
                               user_id, drop_ids, invite_ids, error);
}

/**
 * matrix_api_get_user_presence:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async) (allow-none): a function to call when the
 *            request is finished
 * @user_data: (closure): user data to pass to the callback function
 *             @callback
 * @user_id: the user whose presence list is being modified
 * @error: return location for a #GError, or %NULL
 *
 * Get the given user's presence state.
 *
 * If @user_id is %NULL, this function returns immediately, and fills
 * @error with %MATRIX_API_ERROR_INCOMPLETE.
 */
void
matrix_api_get_user_presence(MatrixAPI *api,
                             MatrixAPICallback callback,
                             gpointer user_data,
                             const gchar *user_id,
                             GError **error)
{
    g_return_if_fail(MATRIX_IS_API(api));

    if (!user_id) {
        g_set_error(error,
                    MATRIX_API_ERROR, MATRIX_API_ERROR_INCOMPLETE,
                    "user_id must be set.");

        return;
    }

    MATRIX_API_GET_IFACE(api)
        ->get_user_presence(api,
                            callback, user_data,
                            user_id, error);
}

/**
 * matrix_api_set_user_presence:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async) (allow-none): a function to call when the
 *            request is finished
 * @user_data: (closure): user data to pass to the callback function
 *             @callback
 * @user_id: the user whose presence list is being modified
 * @presence: the new presence state
 * @status_message: (allow-none): a status message attached to this
 *                  state
 * @error: return location for a #GError, or %NULL
 *
 * Set the given user's presence. You cannot set the presence of
 * another user.
 *
 * If @user_id is %NULL, this function returns immediately, and fills
 * @error with %MATRIX_API_ERROR_INCOMPLETE.
 */
void
matrix_api_set_user_presence(MatrixAPI *api,
                             MatrixAPICallback callback,
                             gpointer user_data,
                             const gchar *user_id,
                             MatrixAPIPresence presence,
                             const gchar *status_message,
                             GError **error)
{
    g_return_if_fail(MATRIX_IS_API(api));

    if (!user_id) {
        g_set_error(error,
                    MATRIX_API_ERROR, MATRIX_API_ERROR_INCOMPLETE,
                    "user_id must be set.");

        return;
    }

    MATRIX_API_GET_IFACE(api)
        ->set_user_presence(api,
                            callback, user_data,
                            user_id, presence, status_message,
                            error);
}

/* Push notifications */

/**
 * matrix_api_update_pusher:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async) (allow-none): a function to call when the
 *            request is finished
 * @user_data: (closure): user data to pass to the callback function
 *             @callback
 * @pusher: the pusher information
 * @error: return location for a #GError, or %NULL
 *
 * Create, update or delete a pusher for the active user on this
 * homeserver.
 *
 * If @pusher is %NULL, this function returns immediately, and fills
 * @error with %MATRIX_API_ERROR_INCOMPLETE.
 */
void
matrix_api_update_pusher(MatrixAPI *api,
                         MatrixAPICallback callback,
                         gpointer user_data,
                         MatrixAPIPusher *pusher,
                         GError **error)
{
    g_return_if_fail(MATRIX_IS_API(api));

    if (!pusher) {
        g_set_error(error,
                    MATRIX_API_ERROR, MATRIX_API_ERROR_INCOMPLETE,
                    "pusher must be set.");

        return;
    }

    MATRIX_API_GET_IFACE(api)
        ->update_pusher(api,
                        callback, user_data,
                        pusher, error);
}

/**
 * matrix_api_get_pushers:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async) (allow-none): a function to call when the
 *            request is finished
 * @user_data: (closure): user data to pass to the callback function
 *             @callback
 * @error: return location for a #GError, or %NULL
 *
 * Retrieve all push rulesets.
 */
void
matrix_api_get_pushers(MatrixAPI *api,
                       MatrixAPICallback callback,
                       gpointer user_data,
                       GError **error)
{
    g_return_if_fail(MATRIX_IS_API(api));

    MATRIX_API_GET_IFACE(api)
        ->get_pushers(api, callback, user_data, error);
}

/**
 * matrix_api_delete_pusher:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async) (allow-none): a function to call when the
 *            request is finished
 * @user_data: (closure): user data to pass to the callback function
 *             @callback
 * @scope: either <code>global</code> to specify global rules, or
 *         <code>device/&lt;profile tag&gt;</code> for rules for a
 *         given <code>profile tag</code>.
 * @kind: the kind of rule
 * @rule_id: an identifier for the rule
 * @error: return location for a #GError, or %NULL
 *
 * Delete a push rule.
 *
 * If @scope or @rule_id is %NULL, this function returns immediately,
 * and fills @error with %MATRIX_API_ERROR_INCOMPLETE.
 */
void
matrix_api_delete_pusher(MatrixAPI *api,
                         MatrixAPICallback callback,
                         gpointer user_data,
                         const gchar *scope,
                         MatrixAPIPusherKind kind,
                         const gchar *rule_id,
                         GError **error)
{
    g_return_if_fail(MATRIX_IS_API(api));

    if (!scope || !rule_id) {
        g_set_error(error,
                    MATRIX_API_ERROR, MATRIX_API_ERROR_INCOMPLETE,
                    "scope and rule_id must be set.");

        return;
    }

    MATRIX_API_GET_IFACE(api)
        ->delete_pusher(api,
                        callback, user_data,
                        scope, kind, rule_id, error);
}

/**
 * matrix_api_get_pusher:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async) (allow-none): a function to call when the
 *            request is finished
 * @user_data: (closure): user data to pass to the callback function
 *             @callback
 * @scope: either <code>global</code> to specify global rules, or
 *         <code>device/&lt;profile tag&gt;</code> for rules for a
 *         given <code>profile tag</code>.
 * @kind: the kind of rule
 * @rule_id: an identifier for the rule
 * @error: return location for a #GError, or %NULL
 *
 * Retrieve a specific push rule.
 *
 * If @scope or @rule_id is %NULL, this function returns immediately,
 * and fills @error with %MATRIX_API_ERROR_INCOMPLETE.
 */
void
matrix_api_get_pusher(MatrixAPI *api,
                      MatrixAPICallback callback,
                      gpointer user_data,
                      const gchar *scope,
                      MatrixAPIPusherKind kind,
                      const gchar *rule_id,
                      GError **error)
{
    g_return_if_fail(MATRIX_IS_API(api));

    if (!scope || !rule_id) {
        g_set_error(error,
                    MATRIX_API_ERROR, MATRIX_API_ERROR_INCOMPLETE,
                    "scope and rule_id must be set.");

        return;
    }

    MATRIX_API_GET_IFACE(api)
        ->get_pusher(api,
                     callback, user_data,
                     scope, kind, rule_id, error);
}

/**
 * matrix_api_add_pusher:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async) (allow-none): a function to call when the
 *            request is finished
 * @user_data: (closure): user data to pass to the callback function
 *             @callback
 * @scope: either <code>global</code> to specify global rules, or
 *         <code>device/&lt;profile tag&gt;</code> for rules for a
 *         given <code>profile tag</code>.
 * @kind: the kind of rule
 * @rule_id: an identifier for the rule
 * @before: (allow-none): make the new rule the next-most important
 *          than this rule ID
 * @after: (allow-none): make the new rule the next-less important
 *         than this rule ID
 * @actions: (element-type utf8): the actions to perform when the
 *           conditions for this rule are met
 * @conditions: (element-type MatrixAPIPusherConditionKind) (allow-none):
 *              the conditions that must hold true for an event for a
 *              rule to be applied. A rule with no conditions always
 *              matches
 * @error: return location for a #GError, or %NULL
 *
 * Add or change a push rule.
 *
 * If either @scope, @rule_id or @actions are %NULL, this
 * function returns immediately, and fills @error with
 * %MATRIX_API_ERROR_INCOMPLETE.
 */
void
matrix_api_add_pusher(MatrixAPI *api,
                      MatrixAPICallback callback,
                      gpointer user_data,
                      const gchar *scope,
                      MatrixAPIPusherKind kind,
                      const gchar *rule_id,
                      const gchar *before,
                      const gchar *after,
                      GList *actions,
                      GList *conditions,
                      GError **error)
{
    g_return_if_fail(MATRIX_IS_API(api));

    if (!scope || !rule_id || !actions) {
        g_set_error(error,
                    MATRIX_API_ERROR, MATRIX_API_ERROR_INCOMPLETE,
                    "scope, rule_id and actions must be set.");

        return;
    }

    MATRIX_API_GET_IFACE(api)
        ->add_pusher(api,
                     callback, user_data,
                     scope, kind, rule_id, before, after,
                     actions, conditions,
                     error);
}

/**
 * matrix_api_toggle_pusher:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async) (allow-none): a function to call when the
 *            request is finished
 * @user_data: (closure): user data to pass to the callback function
 *             @callback
 * @scope: either <code>global</code> to specify global rules, or
 *         <code>device/&lt;profile tag&gt;</code> for rules for a
 *         given <code>profile tag</code>.
 * @kind: the kind of rule
 * @rule_id: an identifier for the rule
 * @enabled: if %TRUE, the rule will be enabled, otherwise it gets
 *           disabled
 * @error: return location for a #GError, or %NULL
 *
 * Enable or disable the specified push rule.
 *
 * If @scope or @rule_id is %NULL, this function returns immediately,
 * and fills @error with %MATRIX_API_ERROR_INCOMPLETE.
 */
void matrix_api_toggle_pusher(MatrixAPI *api,
                              MatrixAPICallback callback,
                              gpointer user_data,
                              const gchar *scope,
                              MatrixAPIPusherKind kind,
                              const gchar *rule_id,
                              gboolean enabled,
                              GError **error)
{
    g_return_if_fail(MATRIX_IS_API(api));

    if (!scope || !rule_id) {
        g_set_error(error,
                    MATRIX_API_ERROR, MATRIX_API_ERROR_INCOMPLETE,
                    "scope and rule_id must be set.");

        return;
    }

    MATRIX_API_GET_IFACE(api)
        ->toggle_pusher(api,
                        callback, user_data,
                        scope, kind, rule_id, enabled, error);
}

/* Room creation */

/**
 * matrix_api_create_room:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async) (allow-none): the function to call when
 *            the request is finished
 * @user_data: (closure): user data to pass to the callback function
 *             @callback
 * @preset: a room preset to use
 * @room_name: (allow-none): the desired display name for the room
 * @room_alias: (allow-none): an alias of the room
 * @topic: (allow-none): the topic of the room
 * @visibility: the initial visibility of the room
 * @creation_content: (allow-none): extra keys to be added to the
 *                    content of m.room.create
 * @initial_state: (element-type MatrixAPIStateEvent) (allow-none): A
 *                 list of state events to set in the new room
 * @invitees: (element-type utf8) (allow-none): list of user IDs to
 *            invite to the new room
 * @invite_3pids: (element-type MatrixAPI3PidCredential) (allow-none):
 *                 a list of 3rd party credentials to invite to the
 *                 new room
 * @error: return location for a #GError, or %NULL
 *
 * Create a new room with the given name and invite the users in
 * @invitees.
 */
void
matrix_api_create_room(MatrixAPI *api,
                       MatrixAPICallback callback,
                       gpointer user_data,
                       MatrixAPIRoomPreset preset,
                       const gchar *room_name,
                       const gchar *room_alias,
                       const gchar *topic,
                       MatrixAPIRoomVisibility visibility,
                       JsonNode *creation_content,
                       GList *initial_state,
                       GList *invitees,
                       GList *invite_3pids,
                       GError **error)
{
    g_return_if_fail(MATRIX_IS_API(api));

    MATRIX_API_GET_IFACE(api)
        ->create_room(api, callback, user_data,
                      preset, room_name, room_alias, topic,
                      visibility, creation_content,
                      initial_state, invitees, invite_3pids,
                      error);
}

/* Room directory */

/**
 * matrix_api_delete_room_alias:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async) (allow-none): the function to call when
 *            the request is finished
 * @user_data: (closure): user data to pass to the callback function
 *             @callback
 * @room_alias: the alias name to remove
 * @error: return location for a #GError, or %NULL
 *
 * Remove the mapping of @room_alias to its room ID
 *
 * Servers may choose to implement additional access control checks
 * here, for instance that room aliases can only be deleted by their
 * creator or a server administrator.
 *
 * If @room_alias is %NULL, this function returns immediately, and
 * fills @error with %MATRIX_API_ERROR_INCOMPLETE.
 */
void
matrix_api_delete_room_alias(MatrixAPI *api,
                             MatrixAPICallback callback,
                             gpointer user_data,
                             const gchar *room_alias,
                             GError **error)
{
    g_return_if_fail(MATRIX_IS_API(api));

    if (!room_alias) {
        g_set_error(error,
                    MATRIX_API_ERROR, MATRIX_API_ERROR_INCOMPLETE,
                    "room_alias must be set.");

        return;
    }

    MATRIX_API_GET_IFACE(api)
        ->delete_room_alias(api, callback, user_data, room_alias, error);
}

/**
 * matrix_api_get_room_id:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async) (allow-none): the function to call when
 *            the request is finished
 * @user_data: (closure) (allow-none): user data to pass to the
 *             callback function
 * @room_alias: the room alias
 * @error: return location for a #GError, or %NULL
 *
 * Get the room ID corresponding to this room alias.
 *
 * If @room_alias is %NULL, this function returns immediately, and
 * fills @error with %MATRIX_API_ERROR_INCOMPLETE.
 */
void
matrix_api_get_room_id(MatrixAPI *api,
                       MatrixAPICallback callback,
                       gpointer user_data,
                       const gchar *room_alias,
                       GError **error)
{
    g_return_if_fail(MATRIX_IS_API(api));

    if (!room_alias) {
        g_set_error(error,
                    MATRIX_API_ERROR, MATRIX_API_ERROR_INCOMPLETE,
                    "room_alias must be set.");

        return;
    }

    MATRIX_API_GET_IFACE(api)
        ->get_room_id(api, callback, user_data, room_alias, error);
}

/**
 * matrix_api_create_room_alias:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async) (allow-none): the function to call when
 *            the request is finished
 * @user_data: (closure): user data to pass to the callback function
 *             @callback
 * @room_id: the room ID to add this alias to
 * @room_alias: the room alias to set
 * @error: return location for a #GError, or %NULL
 *
 * Create a new mapping from room alias to room ID.
 *
 * If @room_alias or @room_id is %NULL, this function returns
 * immediately, and fills @error with %MATRIX_API_ERROR_INCOMPLETE.
 */
void
matrix_api_create_room_alias(MatrixAPI *api,
                             MatrixAPICallback callback,
                             gpointer user_data,
                             const gchar *room_id,
                             const gchar *room_alias,
                             GError **error)
{
    g_return_if_fail(MATRIX_IS_API(api));

    if (!room_alias || !room_id) {
        g_set_error(error,
                    MATRIX_API_ERROR, MATRIX_API_ERROR_INCOMPLETE,
                    "room_alias and room_id must be set.");

        return;
    }

    MATRIX_API_GET_IFACE(api)
        ->create_room_alias(api,
                            callback, user_data,
                            room_id, room_alias,
                            error);
}

/* Room discovery */

/**
 * matrix_api_list_public_rooms:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async) (allow-none): the function to call when
 *            the request is finished
 * @user_data: (closure): user data to pass to the callback function
 *             @callback
 * @error: return location for a #GError, or %NULL
 *
 * List the public rooms on the server.
 */
void
matrix_api_list_public_rooms(MatrixAPI *api,
                             MatrixAPICallback callback,
                             gpointer user_data,
                             GError **error)
{
    g_return_if_fail(MATRIX_IS_API(api));

    MATRIX_API_GET_IFACE(api)
        ->list_public_rooms(api, callback, user_data, error);
}

/* Room membership */

/**
 * matrix_api_ban_user:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async): the function to call when the request is
 *            finished
 * @user_data: (closure): user data to pass to the callback function
 *             @callback
 * @room_id: the room ID where the user should be banned
 * @user_id: the user ID to ban
 * @reason: (allow-none): the reason of the ban
 * @error: return location for a #GError, or %NULL
 *
 * Ban the specified user from the specified room. An optional reason
 * can be specified.
 *
 * If @room_id or @user_id is %NULL, this function returns
 * immediately, and fills @error with %MATRIX_API_ERROR_INCOMPLETE.
 */
void
matrix_api_ban_user(MatrixAPI *api,
                    MatrixAPICallback callback,
                    gpointer user_data,
                    const gchar *room_id,
                    const gchar *user_id,
                    const gchar *reason,
                    GError **error)
{
    g_return_if_fail(MATRIX_IS_API(api));

    if (!room_id || !user_id) {
        g_set_error(error,
                    MATRIX_API_ERROR, MATRIX_API_ERROR_INCOMPLETE,
                    "room_id and user_id must be set.");

        return;
    }

    MATRIX_API_GET_IFACE(api)
        ->ban_user(api, callback, user_data, room_id, user_id, reason, error);
}

/**
 * matrix_api_forget_room:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async): the function to call when the request is
 *            finished
 * @user_data: (closure): user data to pass to the callback function
 *             @callback
 * @room_id: the room ID to forget
 * @error: return location for a #GError, or %NULL
 *
 * Stop the requesting user remembering about a particular room.
 *
 * In general, history is a first class citizen in Matrix. After this
 * API is called, however, a user will no longer be able to retrieve
 * history for this room. If all users on a homeserver forget a room,
 * the room is eligible for deletion from that homeserver.
 *
 * If the user is currently joined to the room, they will implicitly
 * leave the room as part of this API call.
 *
 * If @room_id is %NULL, this function returns immediately, and fills
 * @error with %MATRIX_API_ERROR_INCOMPLETE.
 */
void
matrix_api_forget_room(MatrixAPI *api,
                       MatrixAPICallback callback,
                       gpointer user_data,
                       const gchar *room_id,
                       GError **error)
{
    g_return_if_fail(MATRIX_IS_API(api));

    if (!room_id) {
        g_set_error(error,
                    MATRIX_API_ERROR, MATRIX_API_ERROR_INCOMPLETE,
                    "room_id must be set.");

        return;
    }

    MATRIX_API_GET_IFACE(api)
        ->forget_room(api, callback, user_data, room_id, error);
}

/**
 * matrix_api_invite_user_3rdparty:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async): the function to call when the request is
 *            finished
 * @user_data: (closure): user data to pass to the callback function
 *             @callback
 * @room_id: the room ID to which to invite the user
 * @credential: (transfer none): a #MatrixAPI3PidCredential that
 *              identifies a user to invite
 * @error: return location for a #GError, or %NULL
 *
 * Invite a user to the room by a 3rd party identifier. They do not
 * start participating in the room until they actually join the room.
 *
 * If the identity server does not know a Matrix user identifier for
 * the passed third party identifier, the homeserver will issue an
 * invitation which can be accepted upon providing proof of ownership
 * of the third party identifier.
 *
 * If @credential is %NULL, this function immediately returns, and
 * fills @error with %MATRIX_API_ERROR_INCOMPLETE.
 */
void matrix_api_invite_user_3rdparty(MatrixAPI *api,
                                     MatrixAPICallback callback,
                                     gpointer user_data,
                                     const gchar *room_id,
                                     MatrixAPI3PidCredential *credential,
                                     GError **error)
{
    g_return_if_fail(MATRIX_IS_API(api));

    if (!credential) {
        g_set_error(error,
                    MATRIX_API_ERROR, MATRIX_API_ERROR_INCOMPLETE,
                    "credential must be set.");

        return;
    }

    MATRIX_API_GET_IFACE(api)
        ->invite_user_3rdparty(api,
                               callback, user_data,
                               room_id, credential,
                               error);
}

/**
 * matrix_api_invite_user:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async): the function to call when the request is
 *            finished
 * @user_data: (closure): user data to pass to the callback function
 *             @callback
 * @room_id: the room ID to invite the user to
 * @user_id: the user ID to invite
 * @error: return location for a #GError, or %NULL
 *
 * Invite a user to a room.
 *
 * If @room_id or @user_id is %NULL, this function returns
 * immediately, and fills @error with %MATRIX_API_ERROR_INCOMPLETE.
 */
void
matrix_api_invite_user(MatrixAPI *api,
                       MatrixAPICallback callback,
                       gpointer user_data,
                       const gchar *room_id,
                       const gchar *user_id,
                       GError **error)
{
    g_return_if_fail(MATRIX_IS_API(api));

    if (!room_id || !user_id) {
        g_set_error(error,
                    MATRIX_API_ERROR, MATRIX_API_ERROR_INCOMPLETE,
                    "room_id and user_id must be set.");

        return;
    }

    MATRIX_API_GET_IFACE(api)
        ->invite_user(api, callback, user_data, room_id, user_id, error);
}

/**
 * matrix_api_join_room:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async): the function to call when the request is
 *            finished
 * @user_data: (closure): user data to pass to the callback function
 *             @callback
 * @room_id: the room ID to join to
 * @error: return location for a #GError, or %NULL
 *
 * Join a room.
 *
 * If @room_id is %NULL, this function returns immediately, and fills
 * @error with %MATRIX_API_ERROR_INCOMPLETE.
 */
void
matrix_api_join_room(MatrixAPI *api,
                     MatrixAPICallback callback,
                     gpointer user_data,
                     const gchar *room_id,
                     GError **error)
{
    g_return_if_fail(MATRIX_IS_API(api));

    if (!room_id) {
        g_set_error(error,
                    MATRIX_API_ERROR, MATRIX_API_ERROR_INCOMPLETE,
                    "room_id must be set.");

        return;
    }

    MATRIX_API_GET_IFACE(api)
        ->join_room(api, callback, user_data, room_id, error);
}

/**
 * matrix_api_leave_room:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async): the function to call when the request is
 *            finished
 * @user_data: (closure): user data to pass to the callback function
 *             @callback
 * @room_id: the room ID to kick the user from
 * @error: return location for a #GError, or %NULL
 *
 * Leave a room.
 *
 * If @room_id is %NULL, this function returns immediately, and fills
 * @error with %MATRIX_API_ERROR_INCOMPLETE.
 */
void
matrix_api_leave_room(MatrixAPI *api,
                      MatrixAPICallback callback,
                      gpointer user_data,
                      const gchar *room_id,
                      GError **error)
{
    g_return_if_fail(MATRIX_IS_API(api));

    if (!room_id) {
        g_set_error(error,
                    MATRIX_API_ERROR, MATRIX_API_ERROR_INCOMPLETE,
                    "room_id must be set.");

        return;
    }

    MATRIX_API_GET_IFACE(api)
        ->leave_room(api, callback, user_data, room_id, error);
}

/* Room participation */

/**
 * matrix_api_event_stream:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async): the function to call when the request is
 *            finished
 * @user_data: (closure): user data to pass to the callback function
 *             @callback
 * @from_token: (allow-none): events will be listed from this token
 * @timeout: timeout of the request
 * @error: return location for a #GError, or %NULL
 *
 * Get the event stream, optionally beginning from @from_token.
 */
void
matrix_api_event_stream(MatrixAPI *api,
                        MatrixAPICallback callback,
                        gpointer user_data,
                        const gchar *from_token,
                        gulong timeout,
                        GError **error)
{
    g_return_if_fail(MATRIX_IS_API(api));

    MATRIX_API_GET_IFACE(api)
        ->event_stream(api, callback, user_data, from_token, timeout, error);
}

/**
 * matrix_api_get_event:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async): the function to call when the request is
 *            finished
 * @user_data: (closure): user data to pass to the callback function
 *             @callback
 * @event_id: the event ID to get
 * @error: return location for a #GError, or %NULL
 *
 * Get a single event by event ID.
 *
 * If @event_id is %NULL, this function returns immediately, and fills
 * @error with %MATRIX_API_ERROR_INCOMPLETE.
 */
void
matrix_api_get_event(MatrixAPI *api,
                     MatrixAPICallback callback,
                     gpointer user_data,
                     const gchar *event_id,
                     GError **error)
{
    g_return_if_fail(MATRIX_IS_API(api));

    if (!event_id) {
        g_set_error(error,
                    MATRIX_API_ERROR, MATRIX_API_ERROR_INCOMPLETE,
                    "event_id must be set.");

        return;
    }

    MATRIX_API_GET_IFACE(api)
        ->get_event(api, callback, user_data, event_id, error);
}

/**
 * matrix_api_initial_sync:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async): the function to call when the request is
 *            finished
 * @user_data: (closure): user data to pass to the callback function
 *             @callback
 * @limit: the maximum number of events to get
 * @archived: whether to include rooms that the user has left
 * @error: return location for a #GError, or %NULL
 *
 * perform an initial sync of events
 */
void
matrix_api_initial_sync(MatrixAPI *api,
                        MatrixAPICallback callback,
                        gpointer user_data,
                        guint limit,
                        gboolean archived,
                        GError **error)
{
    g_return_if_fail(MATRIX_IS_API(api));

    MATRIX_API_GET_IFACE(api)
        ->initial_sync(api, callback, user_data, limit, archived, error);
}

/**
 * matrix_api_get_event_context:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async): the function to call when the request is
 *            finished
 * @user_data: (closure): user data to pass to the callback function
 *             @callback
 * @room_id: the room to get events from
 * @event_id: the event to get context around
 * @limit: the maximum number of events to get. If 0, a default value
 *         is used (10, according to the specification)
 * @error: return location for a #GError, or %NULL
 *
 * Gets a number of events that happened just before and after the
 * specified event.
 *
 * If @room_id or @event_id is %NULL, this function returns
 * immediately, and fills @error with %MATRIX_API_ERROR_INCOMPLETE.
 */
void
matrix_api_get_event_context(MatrixAPI *api,
                             MatrixAPICallback callback,
                             gpointer user_data,
                             const gchar *room_id,
                             const gchar *event_id,
                             guint limit,
                             GError **error)
{
    g_return_if_fail(MATRIX_IS_API(api));

    if (!room_id || !event_id) {
        g_set_error(error,
                    MATRIX_API_ERROR, MATRIX_API_ERROR_INCOMPLETE,
                    "room_id and event_id must be set.");

        return;
    }

    MATRIX_API_GET_IFACE(api)
        ->get_event_context(api,
                            callback, user_data,
                            room_id, event_id, limit, error);
}

/**
 * matrix_api_initial_sync_room:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async): the function to call when the request is
 *            finished
 * @user_data: (closure): user data to pass to the callback function
 *             @callback
 * @room_id: the room to get the data for
 * @error: return location for a #GError, or %NULL
 *
 * Get a copy of the current state and the most recent messages in a
 * room.
 *
 * If @room_id is %NULL, this function returns immediately, and fills
 * @error with %MATRIX_API_ERROR_INCOMPLETE.
 */
void
matrix_api_initial_sync_room(MatrixAPI *api,
                             MatrixAPICallback callback,
                             gpointer user_data,
                             const gchar *room_id,
                             GError **error)
{
    g_return_if_fail(MATRIX_IS_API(api));

    if (!room_id) {
        g_set_error(error,
                    MATRIX_API_ERROR, MATRIX_API_ERROR_INCOMPLETE,
                    "room_id must be set.");

        return;
    }

    MATRIX_API_GET_IFACE(api)
        ->initial_sync_room(api, callback, user_data, room_id, error);
}

/**
 * matrix_api_list_room_members:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async): the function to call when the request is
 *            finished
 * @user_data: (closure): user data to pass to the callback function
 *             @callback
 * @room_id: the room to get the member events for
 * @error: return location for a #GError, or %NULL
 *
 * Get the list of members for a room.
 *
 * If @room_id is %NULL, this function returns immediately, and fills
 * @error with %MATRIX_API_ERROR_INCOMPLETE.
 */
void
matrix_api_list_room_members(MatrixAPI *api,
                             MatrixAPICallback callback,
                             gpointer user_data,
                             const gchar *room_id,
                             GError **error)
{
    g_return_if_fail(MATRIX_IS_API(api));

    if (!room_id) {
        g_set_error(error,
                    MATRIX_API_ERROR, MATRIX_API_ERROR_INCOMPLETE,
                    "room_id must be set.");

        return;
    }

    MATRIX_API_GET_IFACE(api)
        ->list_room_members(api, callback, user_data, room_id, error);
}

/**
 * matrix_api_list_room_messages:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async): the function to call when the request is
 *            finished
 * @user_data: (closure): user data to pass to the callback function
 *             @callback
 * @room_id: the room to get the events for
 * @from_token: the token to start returning events from. This token
 *              can be obtained by calling matrix_api_initial_sync()
 *              or matrix_api_initial_sync_room()
 * @direction: the direction of the returned events
 * @limit: the maximum number of events to return. If 0, a default
 *         value will be used (10, according to the specification
 * @error: return location for a #GError, or %NULL
 *
 * Get a list of message and state events for a room.
 *
 * If @room_id or @from_token is %NULL, this function returns
 * immediately, and fills @error with %MATRIX_API_ERROR_INCOMPLETE.
 */
void
matrix_api_list_room_messages(MatrixAPI *api,
                              MatrixAPICallback callback,
                              gpointer user_data,
                              const gchar *room_id,
                              const gchar *from_token,
                              MatrixAPIEventDirection direction,
                              guint limit,
                              GError **error)
{
    g_return_if_fail(MATRIX_IS_API(api));

    if (!room_id || !from_token) {
        g_set_error(error,
                    MATRIX_API_ERROR, MATRIX_API_ERROR_INCOMPLETE,
                    "room_id and from_token must be set.");

        return;
    }

    MATRIX_API_GET_IFACE(api)
        ->list_room_messages(api,
                             callback, user_data,
                             room_id, from_token, direction, limit,
                             error);
}

/**
 * matrix_api_send_event_receipt:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async): the function to call when the request is
 *            finished
 * @user_data: (closure): user data to pass to the callback function
 *             @callback
 * @room_id: the room in which to send the event
 * @receipt_type: type of the receipt
 * @event_id: the event ID to acknowledge up to
 * @receipt: extra receipt information to attach. Note that the server
 *           will automatically attach the <code>ts</code> field
 * @error: return location for a #GError, or %NULL
 *
 * Update the marker for the given receipt type to the event ID
 * specified.
 *
 * If @room_id, @event_id or @receipt is %NULL, this function returns
 * immediately, and fills @error with %MATRIX_API_ERROR_INCOMPLETE.
 */
void
matrix_api_send_event_receipt(MatrixAPI *api,
                              MatrixAPICallback callback,
                              gpointer user_data,
                              const gchar *room_id,
                              MatrixAPIReceiptType receipt_type,
                              const gchar *event_id,
                              JsonNode *receipt,
                              GError **error)
{
    g_return_if_fail(MATRIX_IS_API(api));

    if (!room_id || !event_id || !receipt) {
        g_set_error(error,
                    MATRIX_API_ERROR, MATRIX_API_ERROR_INCOMPLETE,
                    "room_id, event_id and receipt must be set.");

        return;
    }

    MATRIX_API_GET_IFACE(api)
        ->send_event_receipt(api,
                             callback, user_data,
                             room_id, receipt_type, event_id, receipt,
                             error);
}

/**
 * matrix_api_redact_event:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async): the function to call when the request is
 *            finished
 * @user_data: (closure): user data to pass to the callback function
 *             @callback
 * @room_id: the room from which to redact the event
 * @event_id: the event ID to acknowledge up to
 * @txn_id: the transaction ID for this event. Clients should generate
 *          a unique ID; it will be used by the server to ensure
 *          idempotency of requests
 * @reason: (allow-none): the reason for the event being redacted
 * @error: return location for a #GError, or %NULL
 *
 * Strip all information out of an event which isn't critical to the
 * integrity of the server-side representation of the room. This
 * cannot be undone.
 *
 * Users may redact their own events, and any user with a power level
 * greater than or equal to <code>redact</code> power level of the
 * room may redact events there.
 *
 * If @room_id, @event_id or @txn_id is %NULL, this function returns
 * immediately, and fills @error with %MATRIX_API_ERROR_INCOMPLETE.
 */
void
matrix_api_redact_event(MatrixAPI *api,
                        MatrixAPICallback callback,
                        gpointer user_data,
                        const gchar *room_id,
                        const gchar *event_id,
                        const gchar *txn_id,
                        const gchar *reason,
                        GError **error)
{
    g_return_if_fail(MATRIX_IS_API(api));

    if (!room_id || !event_id || !txn_id) {
        g_set_error(error,
                    MATRIX_API_ERROR, MATRIX_API_ERROR_INCOMPLETE,
                    "room_id, event_id and txn_id must be set.");

        return;
    }

    MATRIX_API_GET_IFACE(api)
        ->redact_event(api,
                       callback, user_data,
                       room_id, event_id, txn_id, reason,
                       error);
}

/**
 * matrix_api_send_message_event:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async): the function to call when the request is
 *            finished
 * @user_data: (closure): user data to pass to the callback function
 *             @callback
 * @room_id: the room to send the event to
 * @event_type: the type of event to send
 * @txn_id: the transaction ID for this event. Clients should generate
 *          a unique ID; it will be used by the server to ensure
 *          idempotency of requests
 * @content: (transfer full): the content of the event as a #JsonNode
 * @error: return location for a #GError, or %NULL
 *
 * Send a message event to the room.
 *
 * If @room_id, @event_type, @txn_id or @content is %NULL, this
 * function returns immediately, and fills @error with
 * %MATRIX_API_ERROR_INCOMPLETE.
 */
void
matrix_api_send_message_event(MatrixAPI *api,
                              MatrixAPICallback callback,
                              gpointer user_data,
                              const gchar *room_id,
                              const gchar *event_type,
                              const gchar *txn_id,
                              JsonNode *content,
                              GError **error)
{
    g_return_if_fail(MATRIX_IS_API(api));

    if (!room_id || !event_type || !txn_id || !content) {
        g_set_error(error,
                    MATRIX_API_ERROR, MATRIX_API_ERROR_INCOMPLETE,
                    "room_id, event_type, txn_id and content must be set.");

        return;
    }

    MATRIX_API_GET_IFACE(api)
        ->send_message_event(api,
                             callback, user_data,
                             room_id,
                             event_type, txn_id,
                             content, error);
}

/**
 * matrix_api_get_room_state:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async): the function to call when the request is
 *            finished
 * @user_data: (closure): user data to pass to the callback function
 *             @callback
 * @room_id: the room ID to get a state for
 * @event_type: (allow-none): the type of state to look up
 * @state_key: (allow-none): the key of the state to look up. If
 *             @event_type is %NULL, this parameter is ignored
 * @error: return location for a #GError, or %NULL
 *
 * Look up the contents of a state event in a room. If both
 * @event_type and @state_key are empty, get a list of state events
 * for that room.
 *
 * If @room_id is %NULL, or if @state_key is set with @event_type
 * being %NULL, this function returns immediately, and fills @error
 * with %MATRIX_API_ERROR_INCOMPLETE.
 */
void
matrix_api_get_room_state(MatrixAPI *api,
                          MatrixAPICallback callback,
                          gpointer user_data,
                          const gchar *room_id,
                          const gchar *event_type,
                          const gchar *state_key,
                          GError **error)
{
    g_return_if_fail(MATRIX_IS_API(api));

    if (!room_id || (state_key && !event_type)) {
        g_set_error(error,
                    MATRIX_API_ERROR, MATRIX_API_ERROR_INCOMPLETE,
                    "room_id must be set, and event_type must be set if state_key is set.");

        return;
    }

    MATRIX_API_GET_IFACE(api)
        ->get_room_state(api,
                         callback, user_data,
                         room_id, event_type, state_key, error);
}

/**
 * matrix_api_send_room_event:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async): the function to call when the request is
 *            finished
 * @user_data: (closure): user data to pass to the callback function
 *             @callback
 * @room_id: the room ID to get a state for
 * @event_type: the type of state to look up
 * @state_key: (allow-none): the key of the state to look up. If
 *             @event_type is %NULL, this parameter is ignored
 * @content: (transfer full): the content of the state event
 * @error: return location for a #GError, or %NULL
 *
 * Send a state event to the room. These events will be overwritten if
 * @room_id, @event_type and @state_key all match.
 *
 * This request cannot use transaction IDs.
 *
 * The required fields in the body of the request (@content) vary
 * depending on the type of the event.
 *
 * If @room_id or @content is %NULL, or if @state_key is set with
 * @event_type being %NULL, this function returns immediately, and
 * fills @error with %MATRIX_API_ERROR_INCOMPLETE.
 */
void
matrix_api_send_room_event(MatrixAPI *api,
                           MatrixAPICallback callback,
                           gpointer user_data,
                           const gchar *room_id,
                           const gchar *event_type,
                           const gchar *state_key,
                           JsonNode *content,
                           GError **error)
{
    g_return_if_fail(MATRIX_IS_API(api));

    if (!room_id || (state_key && !event_type) || !content) {
        g_set_error(error,
                    MATRIX_API_ERROR, MATRIX_API_ERROR_INCOMPLETE,
                    "room_id must be set, and event_type must be set if state_key is set.");

        return;
    }

    MATRIX_API_GET_IFACE(api)
        ->send_room_event(api,
                          callback, user_data,
                          room_id, event_type, state_key, content,
                          error);
}

/**
 * matrix_api_notify_room_typing:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async): the function to call when the request is
 *            finished
 * @user_data: (closure): user data to pass to the callback function
 *             @callback
 * @user_id: the user who has started to type
 * @room_id: the room in which the user is typing
 * @timeout: the length of time in milliseconds to mark this user as
 *           typing
 * @typing: whether the user is typing or not. If %FALSE, @timeout can
 *          be omitted (ie. set to 0)
 * @error: return location for a #GError, or %NULL
 *
 * Tell the server the user is typing for the next @timeout
 * milliseconds. If @typing is %FALSE, it tells the server that the
 * user stopped typing.
 *
 * If @user_id or @room_id is %NULL, this function returns
 * immediately, and fills @error with %MATRIX_API_ERROR_INCOMPLETE.
 */
void
matrix_api_notify_room_typing(MatrixAPI *api,
                              MatrixAPICallback callback,
                              gpointer user_data,
                              const gchar *user_id,
                              const gchar *room_id,
                              guint timeout,
                              gboolean typing,
                              GError **error)
{
    g_return_if_fail(MATRIX_IS_API(api));

    if (!user_id || !room_id) {
        g_set_error(error,
                    MATRIX_API_ERROR, MATRIX_API_ERROR_INCOMPLETE,
                    "user_id and room_id must be set.");

        return;
    }

    MATRIX_API_GET_IFACE(api)
        ->notify_room_typing(api,
                             callback, user_data,
                             user_id, room_id, timeout, typing,
                             error);
}

/**
 * matrix_api_sync:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async): the function to call when the request is
 *            finished
 * @user_data: (closure): user data to pass to the callback function
 *             @callback
 * @filter_id: (allow-none): a filter ID created by the filter API
 *             (e.g. matrix_api_create_filter())
 * @filter: (allow-none): a definition on what events to fetch
 * @since: (allow-none): a point in time to continue a sync from
 * @full_state: if %TRUE, all state events will be returned, even if
 *              @since is not empty. If %FALSE, and @since is not
 *              empty, only states which have changed since the point
 *              indicated by @since will be returned
 * @set_presence: controls whether the client is automatically marked
 *                as online by polling this API.
 * @timeout: the maximum time to poll in milliseconds
 * @error: return location for a #GError, or %NULL
 *
 * Synchronize the client's state with the latest state on the
 * server. Clients should use this API when they first log in to get
 * an initial snapshot of the state on the server and then continue to
 * call this API to get incremental details to the state and to
 * receive new messages.
 *
 * Only one of @filter and @filter_id should be specified, or both of
 * them should be set to %NULL to receive all events.
 */
void
matrix_api_sync(MatrixAPI *api,
                MatrixAPICallback callback,
                gpointer user_data,
                const gchar *filter_id,
                const MatrixAPIFilter *filter,
                const gchar *since,
                gboolean full_state,
                gboolean set_presence,
                gulong timeout,
                GError **error)
{
    g_return_if_fail(MATRIX_IS_API(api));

    MATRIX_API_GET_IFACE(api)
        ->sync(api,
               callback, user_data,
               filter_id, filter, since,
               full_state, set_presence, timeout,
               error);
}

/**
 * matrix_api_create_filter:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async): the function to call when the request is
 *            finished
 * @user_data: (closure): user data to pass to the callback function
 *             @callback
 * @user_id: the ID of the user uploading the filter. An access token
 *           must be present (either specifying one with
 *           matrix_api_set_token() or requested from the server via
 *           matrix_api_register_account() or matrix_api_login().
 * @filter: the filter to upload
 * @error: return location for a #GError, or %NULL
 *
 * Upload a new filter definition to the homeserver. It will return a
 * filter ID that may be used in future requests.
 *
 * If @user_id or @filter is %NULL, this function returns immediately,
 * and fills @error with %MATRIX_API_ERROR_INCOMPLETE.
 */
void
matrix_api_create_filter(MatrixAPI *api,
                         MatrixAPICallback callback,
                         gpointer user_data,
                         const gchar *user_id,
                         MatrixAPIFilter *filter,
                         GError **error)
{
    g_return_if_fail(MATRIX_IS_API(api));

    if (!user_id || !filter) {
        g_set_error(error,
                    MATRIX_API_ERROR, MATRIX_API_ERROR_INCOMPLETE,
                    "user_id and filter must be set.");

        return;
    }

    MATRIX_API_GET_IFACE(api)
        ->create_filter(api,
                        callback, user_data,
                        user_id, filter,
                        error);
}

/**
 * matrix_api_download_filter:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async): the function to call when the request is
 *            finished
 * @user_data: (closure): user data to pass to the callback function
 *             @callback
 * @user_id: the user ID to download a filter from
 * @filter_id: the filter ID to download
 * @error: return location for a #GError, or %NULL
 *
 * Download a filter.
 *
 * If @user_id or @filter_id is %NULL, this function returns
 * immediately, and fills @error with %MATRIX_API_ERROR_INCOMPLETE.
 */
void
matrix_api_download_filter(MatrixAPI *api,
                           MatrixAPICallback callback,
                           gpointer user_data,
                           const gchar *user_id,
                           const gchar *filter_id,
                           GError **error)
{
    g_return_if_fail(MATRIX_IS_API(api));

    if (!user_id || !filter_id) {
        g_set_error(error,
                    MATRIX_API_ERROR, MATRIX_API_ERROR_INCOMPLETE,
                    "user_id and filter_id must be set.");

        return;
    }

    MATRIX_API_GET_IFACE(api)
        ->download_filter(api,
                          callback, user_data,
                          user_id, filter_id, error);
}

/* Search */

/* Server administration */

/**
 * matrix_api_whois:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async): the function to call when the request is
 *            finished
 * @user_data: (closure): user data to pass to the callback function
 *             @callback
 * @user_id: the user ID to look up
 * @error: return location for a #GError, or %NULL
 *
 * Get information about a particular user.
 *
 * If @user_id is %NULL, this function returns immediately, and fills
 * @error with %MATRIX_API_ERROR_INCOMPLETE.
 */
void
matrix_api_whois(MatrixAPI *api,
                 MatrixAPICallback callback,
                 gpointer user_data,
                 const gchar *user_id,
                 GError **error)
{
    g_return_if_fail(MATRIX_IS_API(api));

    if (!user_id) {
        g_set_error(error,
                    MATRIX_API_ERROR, MATRIX_API_ERROR_INCOMPLETE,
                    "user_id must be set.");

        return;
    }

    MATRIX_API_GET_IFACE(api)
        ->whois(api, callback, user_data, user_id, error);
}

/**
 * matrix_api_versions:
 * @api: a #MatrixAPI iplementation
 * @callback: (scope async): the function to call when the request is
 *            finished
 * @user_data: (closure): user data to pass to the callback function
 *             @callback
 * @error: return location for a #GError, or %NULL
 *
 * Get the versions of the specification supported by the server.
 */
void
matrix_api_versions(MatrixAPI *api,
                    MatrixAPICallback callback,
                    gpointer user_data,
                    GError **error)
{
    g_return_if_fail(MATRIX_IS_API(api));

    MATRIX_API_GET_IFACE(api)
        ->versions(api, callback, user_data, error);
}

/* Session management */

/**
 * matrix_api_login:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async): the function to call when the request is
 *            finished
 * @user_data: (closure): user data to pass to the callback function
 *             @callback
 * @login_type: the login type to use
 * @content: (allow-none): parameters to pass for the login request
 * @error: return location for a #GError, or %NULL
 *
 * Attempt to login with type @type. Implementations of this method
 * must set the token property on a successful login.
 *
 * If @login_type or @content is %NULL, this function returns
 * immediately, and fills @error with %MATRIX_API_ERROR_INCOMPLETE.
 */
void
matrix_api_login(MatrixAPI *api,
                 MatrixAPICallback callback,
                 gpointer user_data,
                 const gchar *login_type,
                 const JsonNode *content,
                 GError **error)
{
    g_return_if_fail(MATRIX_IS_API(api));

    if (!login_type || !content) {
        g_set_error(error,
                    MATRIX_API_ERROR, MATRIX_API_ERROR_INCOMPLETE,
                    "login_type and content must be set.");

        return;
    }

    MATRIX_API_GET_IFACE(api)
        ->login(api, callback, user_data, login_type, content, error);
}

/**
 * matrix_api_token_refresh:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async): the function to call when the request is
 *            finished
 * @user_data: (closure): user data to pass to the callback function
 *             @callback
 * @refresh_token: (allow-none): the refresh token that was issued by
 *                 the server
 * @error: return location for a #GError, or %NULL
 *
 * Exchanges a refresh token for a new access token. This is intended
 * to be used if the access token has expired. If @refresh_token is
 * %NULL, iplementations MUST send the stored refresh token. If it is
 * not pesent (e.g. because login hasn’t happened yet), this function
 * MUST yield an error.
 */
void
matrix_api_token_refresh(MatrixAPI *api,
                         MatrixAPICallback callback,
                         gpointer user_data,
                         const gchar *refresh_token,
                         GError **error)
{
    g_return_if_fail(MATRIX_IS_API(api));

    MATRIX_API_GET_IFACE(api)
        ->token_refresh(api, callback, user_data, refresh_token, error);
}

/* User data */

/**
 * matrix_api_get_3pids:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async): the function to call when the request is
 *            finished
 * @user_data: (closure): user data to pass to the callback function
 *             @callback
 * @error: return location for a #GError, or %NULL
 *
 * Get a list of the third party identifiers that a homeserver has
 * associated with the user's account.
 *
 * This is not the same as the list of third party identifiers bound
 * to the user's Matrix ID in Identity Servers.
 *
 * Identifiers in this list may be used by the homeserver as, for
 * example, identifiers to accept to reset the user's account
 * password.
 */
void
matrix_api_get_3pids(MatrixAPI *api,
                     MatrixAPICallback callback,
                     gpointer user_data,
                     GError **error)
{
    g_return_if_fail(MATRIX_IS_API(api));

    MATRIX_API_GET_IFACE(api)
        ->get_3pids(api, callback, user_data, error);
}

/**
 * matrix_api_add_3pid:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async): the function to call when the request is
 *            finished
 * @user_data: (closure): user data to pass to the callback function
 *             @callback
 * @bind_creds: whether the homeserver should also bind this third
 *              party identifier to the account's Matrix ID with the
 *              passed Identity Server.
 * @threepid_creds: the credentials to associate with the account
 * @error: return location for a #GError, or %NULL
 *
 * Add contact information to the user's account.
 *
 * If @threepid_creds is %NULL, this function returns immediately, and
 * fills @error with %MATRIX_API_ERROR_INCOMPLETE.
 */
void
matrix_api_add_3pid(MatrixAPI *api,
                    MatrixAPICallback callback,
                    gpointer user_data,
                    gboolean bind_creds,
                    MatrixAPI3PidCredential *threepid_creds,
                    GError **error)
{
    g_return_if_fail(MATRIX_IS_API(api));

    if (!threepid_creds) {
        g_set_error(error,
                    MATRIX_API_ERROR, MATRIX_API_ERROR_INCOMPLETE,
                    "threepid_creds must be set.");

        return;
    }

    MATRIX_API_GET_IFACE(api)
        ->add_3pid(api, callback, user_data, bind_creds, threepid_creds, error);
}

/**
 * matrix_api_change_password:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async): the function to call when the request is
 *            finished
 * @user_data: (closure): user data to pass to the callback function
 *             @callback
 * @new_password: the new password for the account
 * @error: return location for a #GError, or %NULL
 *
 * Change the active user's password.
 *
 * If @new_password is %NULL, this function returns immediately, and
 * fills @error with %MATRIX_API_ERROR_INCOMPLETE.
 */
void
matrix_api_change_password(MatrixAPI *api,
                           MatrixAPICallback callback,
                           gpointer user_data,
                           const gchar *new_password,
                           GError **error)
{
    g_return_if_fail(MATRIX_IS_API(api));

    if (!new_password) {
        g_set_error(error,
                    MATRIX_API_ERROR, MATRIX_API_ERROR_INCOMPLETE,
                    "new_password must be set.");

        return;
    }

    MATRIX_API_GET_IFACE(api)
        ->change_password(api, callback, user_data, new_password, error);
}

/**
 * matrix_api_get_profile:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async): the function to call when the request is
 *            finished
 * @user_data: (closure): user data to pass to the callback function
 *             @callback
 * @user_id: the user whose profile to get
 * @error: return location for a #GError, or %NULL
 *
 * Get a user's profile.
 *
 * If @user_id is %NULL, this function returns immediately, and fills
 * @error with %MATRIX_API_ERROR_INCOMPLETE.
 */
void
matrix_api_get_profile(MatrixAPI *api,
                       MatrixAPICallback callback,
                       gpointer user_data,
                       const gchar *user_id,
                       GError **error)
{
    g_return_if_fail(MATRIX_IS_API(api));

    if (!user_id) {
        g_set_error(error,
                    MATRIX_API_ERROR, MATRIX_API_ERROR_INCOMPLETE,
                    "user_id must be set.");

        return;
    }

    MATRIX_API_GET_IFACE(api)
        ->get_profile(api, callback, user_data, user_id, error);
}

/**
 * matrix_api_get_avatar_url:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async): the function to call when the request is
 *            finished
 * @user_data: (closure): user data to pass to the callback function
 *             @callback
 * @user_id: the user whose avatar URL to get
 * @error: return location for a #GError, or %NULL
 *
 * Get the URL of the specified user's avatar.
 *
 * If @user_id is %NULL, this function returns immediately, and fills
 * @error with %MATRIX_API_ERROR_INCOMPLETE.
 */
void
matrix_api_get_avatar_url(MatrixAPI *api,
                          MatrixAPICallback callback,
                          gpointer user_data,
                          const gchar *user_id,
                          GError **error)
{
    g_return_if_fail(MATRIX_IS_API(api));

    if (!user_id) {
        g_set_error(error,
                    MATRIX_API_ERROR, MATRIX_API_ERROR_INCOMPLETE,
                    "user_id must be set.");

        return;
    }

    MATRIX_API_GET_IFACE(api)
        ->get_avatar_url(api, callback, user_data, user_id, error);
}

/**
 * matrix_api_set_avatar_url:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async): the function to call when the request is
 *            finished
 * @user_data: (closure): user data to pass to the callback function
 *             @callback
 * @user_id: the user whose avatar URL to set
 * @avatar_url: the avatar URL info
 * @error: return location for a #GError, or %NULL
 *
 * Set the user's avatar URL.
 *
 * If @user_id or @avatar_url is %NULL, this function returns
 * immediately, and fills @error with %MATRIX_API_ERROR_INCOMPLETE.
 */
void
matrix_api_set_avatar_url(MatrixAPI *api,
                          MatrixAPICallback callback,
                          gpointer user_data,
                          const gchar *user_id,
                          const gchar *avatar_url,
                          GError **error)
{
    g_return_if_fail(MATRIX_IS_API(api));

    if (!user_id || !avatar_url) {
        g_set_error(error,
                    MATRIX_API_ERROR, MATRIX_API_ERROR_INCOMPLETE,
                    "user_id and avatar_url must be set.");

        return;
    }

    MATRIX_API_GET_IFACE(api)
        ->set_avatar_url(api, callback, user_data, user_id, avatar_url, error);
}

/**
 * matrix_api_get_display_name:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async): the function to call when the request is
 *            finished
 * @user_data: (closure): user data to pass to the callback function
 *             @callback
 * @user_id: the user whose display name to get
 * @error: return location for a #GError, or %NULL
 *
 * Get the user's display name.
 *
 * If @user_id is %NULL, this function returns immediately, and fills
 * @error with %MATRIX_API_ERROR_INCOMPLETE.
 */
void
matrix_api_get_display_name(MatrixAPI *api,
                            MatrixAPICallback callback,
                            gpointer user_data,
                            const gchar *user_id,
                            GError **error)
{
    g_return_if_fail(MATRIX_IS_API(api));

    if (!user_id) {
        g_set_error(error,
                    MATRIX_API_ERROR, MATRIX_API_ERROR_INCOMPLETE,
                    "user_id must be set.");

        return;
    }

    MATRIX_API_GET_IFACE(api)
        ->get_display_name(api, callback, user_data, user_id, error);
}

/**
 * matrix_api_set_display_name:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async): the function to call when the request is
 *            finished
 * @user_data: (closure): user data to pass to the callback function
 *             @callback
 * @user_id: the user whose display name to set
 * @display_name: the display name info
 * @error: return location for a #GError, or %NULL
 *
 * Set the user's display name.
 *
 * If @user_id or @display_name is %NULL, this function returns
 * immediately, and fills @error with %MATRIX_API_ERROR_INCOMPLETE.
 */
void
matrix_api_set_display_name(MatrixAPI *api,
                            MatrixAPICallback callback,
                            gpointer user_data,
                            const gchar *user_id,
                            const gchar *display_name,
                            GError **error)
{
    g_return_if_fail(MATRIX_IS_API(api));

    if (!user_id || !display_name) {
        g_set_error(error,
                    MATRIX_API_ERROR, MATRIX_API_ERROR_INCOMPLETE,
                    "user_id and display_name must be set.");

        return;
    }

    MATRIX_API_GET_IFACE(api)
        ->set_display_name(api,
                           callback, user_data,
                           user_id, display_name, error);
}

/**
 * matrix_api_register_account:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async): the function to call when the request is
 *            finished
 * @user_data: (closure): user data to pass to the callback function
 *             @callback
 * @account_kind: the type of account to register
 * @bind_email: if %TRUE, the server binds the e-mail used for
 *              authentication to the Matrix ID with the ID server
 * @username: (allow-none): the local part of the desired Matrix
 *            ID. If omitted, the server will generate a local part
 * @password: the desired password for the account
 * @error: return location for a #GError, or %NULL
 *
 * Attempt to register an account with the homeserver using @username
 * and @password.
 *
 * Implementations of this method must set the token property on a
 * successful login.
 *
 * If @password is %NULL, this function returns immediately, and fills
 * @error with %MATRIX_API_ERROR_INCOMPLETE.
 */
void
matrix_api_register_account(MatrixAPI *api,
                            MatrixAPICallback callback,
                            gpointer user_data,
                            MatrixAPIAccountKind account_kind,
                            gboolean bind_email,
                            const gchar *username,
                            const gchar *password,
                            GError **error)
{
    g_return_if_fail(MATRIX_IS_API(api));

    if (!password) {
        g_set_error(error,
                    MATRIX_API_ERROR, MATRIX_API_ERROR_INCOMPLETE,
                    "password must be set.");

        return;
    }

    MATRIX_API_GET_IFACE(api)
        ->register_account(api, callback, user_data, account_kind,
                           bind_email, username, password, error);
}

/**
 * matrix_api_set_account_data:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async): the function to call when the request is
 *            finished
 * @user_data: (closure): user data to pass to the callback function
 *             @callback
 * @user_id: the user to set account data for. An access token must be
 *           present and be authorized to make requests for this user
 *           ID
 * @room_id: (allow-none): the room to set account data for. If %NULL,
 *           the account data will be set globally
 * @event_type: the event type of the account data to set. Custom
 *              types should be namespaced to avoid clashes
 * @content: (transfer full): the content of the account data
 * @error: return location for a #GError, or %NULL
 *
 * Set some account data for the client. This config is only visible
 * to the user who set the account data. The config will be synced to
 * clients in the top-level account data.
 *
 * If @user_id, @event_type or @content is %NULL, this
 * function returns immediately, and fills @error with
 * %MATRIX_API_ERROR_INCOMPLETE.
 */
void
matrix_api_set_account_data(MatrixAPI *api,
                            MatrixAPICallback callback,
                            gpointer user_data,
                            const gchar *user_id,
                            const gchar *room_id,
                            const gchar *event_type,
                            JsonNode *content,
                            GError **error)
{
    g_return_if_fail(MATRIX_IS_API(api));

    if (!user_id || !event_type || !content) {
        g_set_error(error,
                    MATRIX_API_ERROR, MATRIX_API_ERROR_INCOMPLETE,
                    "user_id2 event_type and content must be set.");

        return;
    }

    MATRIX_API_GET_IFACE(api)
        ->set_account_data(api,
                           callback, user_data,
                           user_id, room_id, event_type, content, error);
}

/**
 * matrix_api_get_room_tags:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async): the function to call when the request is
 *            finished
 * @user_data: (closure): user data to pass to the callback function
 *             @callback
 * @user_id: the ID of the user to get the tags for. An access token
 *           must be set, and it must be authorised to make requests
 *           for this user ID
 * @room_id: the room to get tags for
 * @error: return location for a #GError, or %NULL
 *
 * List the tags set by a user on a room.
 *
 * If @user_id or @room_id is %NULL, this function returns
 * immediately, and fills @error with %MATRIX_API_ERROR_INCOMPLETE.
 */
void
matrix_api_get_room_tags(MatrixAPI *api,
                         MatrixAPICallback callback,
                         gpointer user_data,
                         const gchar *user_id,
                         const gchar *room_id,
                         GError **error)
{
    g_return_if_fail(MATRIX_IS_API(api));

    if (!user_id || !room_id) {
        g_set_error(error,
                    MATRIX_API_ERROR, MATRIX_API_ERROR_INCOMPLETE,
                    "user_id and room_id must be set.");

        return;
    }

    MATRIX_API_GET_IFACE(api)
        ->get_room_tags(api, callback, user_data, user_id, room_id, error);
}

/**
 * matrix_api_delete_room_tag:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async): the function to call when the request is
 *            finished
 * @user_data: (closure): user data to pass to the callback function
 *             @callback
 * @user_id: the id of the user to remove a tag for
 * @room_id: the id of the room to remove the tag from
 * @tag: the tag to remove
 * @error: return location for a #GError, or %NULL
 *
 * Remove a tag from the room.
 *
 * If @user_id, @room_id or @tag is %NULL, this function returns
 * immediately, and fills @error with %MATRIX_API_ERROR_INCOMPLETE.
 */
void
matrix_api_delete_room_tag(MatrixAPI *api,
                           MatrixAPICallback callback,
                           gpointer user_data,
                           const gchar *user_id,
                           const gchar *room_id,
                           const gchar *tag,
                           GError **error)
{
    g_return_if_fail(MATRIX_IS_API(api));

    if (!user_id || !room_id || !tag) {
        g_set_error(error,
                    MATRIX_API_ERROR, MATRIX_API_ERROR_INCOMPLETE,
                    "user_id, room_id and tag must be set.");

        return;
    }

    MATRIX_API_GET_IFACE(api)
        ->delete_room_tag(api,
                          callback, user_data,
                          user_id, room_id, tag, error);
}

/**
 * matrix_api_add_room_tag:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async): the function to call when the request is
 *            finished
 * @user_data: (closure): user data to pass to the callback function
 *             @callback
 * @user_id: the ID of the user to add the tag for
 * @room_id: the ID of the room to add the tag for
 * @tag: the tag to add
 * @content: (transfer full) (allow-none): extra data for the tag,
 *           e.g. ordering
 * @error: return location for a #GError, or %NULL
 *
 * Add a tag to the room.
 *
 * If @user_id, @room_id or @tag is %NULL, this function returns
 * immediately, and fills @error with %MATRIX_API_ERROR_INCOMPLETE.
 */
void
matrix_api_add_room_tag(MatrixAPI *api,
                        MatrixAPICallback callback,
                        gpointer user_data,
                        const gchar *user_id,
                        const gchar *room_id,
                        const gchar *tag,
                        JsonNode *content,
                        GError **error)
{
    g_return_if_fail(MATRIX_IS_API(api));

    if (!user_id || !room_id || !tag) {
        g_set_error(error,
                    MATRIX_API_ERROR, MATRIX_API_ERROR_INCOMPLETE,
                    "user_id, room_id and tag must be set.");

        return;
    }

    MATRIX_API_GET_IFACE(api)
        ->add_room_tag(api,
                       callback, user_data,
                       user_id, room_id, tag, content, error);
}

/* VoIP */

/**
 * matrix_api_get_turn_server:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async): the function to call when the request is
 *            finished
 * @user_data: (closure): user data to pass to the callback function
 *             @callback
 * @error: return location for a #GError, or %NULL
 *
 * Get credentials for the client to use when initiating calls.
 */
void
matrix_api_get_turn_server(MatrixAPI *api,
                           MatrixAPICallback callback,
                           gpointer user_data,
                           GError **error)
{
    g_return_if_fail(MATRIX_IS_API(api));

    MATRIX_API_GET_IFACE(api)
        ->get_turn_server(api, callback, user_data, error);
}

/* Non-spec methods */

/**
 * matrix_api_abort_pending:
 * @api: a #MatrixAPI implementation
 *
 * Abort all pending requests toward the Matrix server. Be aware that
 * this may leave requests in an incosistent state.
 *
 * Implementations that provide only synchronous requests can choose
 * not to implement this function.
 */
void
matrix_api_abort_pending(MatrixAPI *api)
{
    g_return_if_fail(MATRIX_IS_API(api));

    if (MATRIX_API_GET_IFACE(api)->abort_pending) {
        MATRIX_API_GET_IFACE(api)->abort_pending(api);
    }
}
