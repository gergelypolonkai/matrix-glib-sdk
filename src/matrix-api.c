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
 * @err: a #GError
 *
 * A callback function to use with API calls.
 */

/**
 * MatrixAPIError:
 * @MATRIX_API_ERROR_NONE: no error
 * @MATRIX_API_ERROR_MISSING_TOKEN: authorization token is missing
 *                                  from the request
 * @MATRIX_API_ERROR_FORBIDDEN: access was forbidden (e.g. due to a
 *                              missing/invalid token, or using a bad
 *                              password during login)
 * @MATRIX_API_ERROR_UNKNOWN: an error unknown to the Matrix server
 * @MATRIX_API_ERROR_UNKNOWN_ERROR: an error unknown to this library
 *
 * Value mappings from Matrix.org API error codes
 * (e.g. <code>M_MISSING_TOKEN</code>). They should be set
 * automatically by API calls, if the response contains an error code.
 */

/**
 * MATRIX_API_ERROR:
 *
 * Error domain for Matrix GLib SDK API. See #GError for more
 * information on error domains.
 */

/**
 * MatrixAPIRoomPreset:
 * @MATRIX_API_ROOM_PRESET_NONE: no preset
 * @MATRIX_API_ROOM_PRESET_PRIVATE: preset for private rooms
 * @MATRIX_API_ROOM_PRESET_TRUSTED_PRIVATE: same as private rooms, but
 *                                          all users get the same
 *                                          power level as the room
 *                                          creator
 * @MATRIX_API_ROOM_PRESET_PUBLIC: preset for public rooms
 *
 * Preset values for matrix_api_create_room() calls.
 */

/**
 * MatrixAPIRoomVisibility:
 * @MATRIX_API_ROOM_VISIBILITY_DEFAULT: use a server-assigned value
 *                                      (usually <code>private</code>
 * @MATRIX_API_ROOM_VISIBILITY_PUBLIC: make the room visible in the
 *                                     public room list
 * @MATRIX_API_ROOM_VISIBILITY_PRIVATE: hide the room from the public
 *                                      room list
 *
 * Visibility values for room creation. Not to be confused with join
 * rules.
 */

/**
 * MatrixAPIResizeMethod:
 * @MATRIX_API_RESIZE_METHOD_CROP: crop thumbnail to the requested
 *                                 size
 * @MATRIX_API_RESIZE_METHOD_SCALE: scale thumbnail to the requested
 *                                  size
 *
 * Resizing methods for matrix_api_media_thumbnail().
 */

/**
 * MatrixAPIPresence:
 * @MATRIX_API_PRESENCE_ONLINE: user is online
 * @MATRIX_API_PRESENCE_OFFLINE: user is offline
 * @MATRIX_API_PRESENCE_UNAVAILABLE: user is unavailable (i.e. busy)
 * @MATRIX_API_PRESENCE_FREE_FOR_CHAT: user is free for chat
 *
 * Presence values for matrix_api_set_user_presence() and other
 * presence related queries.
 */

/**
 * MatrixAPIPusherKind:
 * @MATRIX_API_PUSHER_KIND_OVERRIDE: highest priority rules
 * @MATRIX_API_PUSHER_KIND_SENDER: for (unencrypted) messages that
 *                                 match certain patterns
 * @MATRIX_API_PUSHER_KIND_ROOM: for all messages for a given
 *                               room. The rule ID of a room rule is
 *                               always the ID of the room that it
 *                               affects
 * @MATRIX_API_PUSHER_KIND_CONTENT: for messages from a specific
 *                                  Matrix user ID. The rule ID of
 *                                  such rules is always the Matrix ID
 *                                  of the user whose messages they'd
 *                                  apply to
 * @MATRIX_API_PUSHER_KIND_UNDERRIDE: lowest priority rules
 *
 * Pusher types.
 */

/**
 * MatrixAPIPusherConditionKind:
 * @MATRIX_API_PUSHER_CONDITION_KIND_EVENT_MATCH: glob pattern match
 *                                                on a field of the
 *                                                event. Requires a
 *                                                <code>key</code> and
 *                                                a
 *                                                <code>pattern</code>
 *                                                parameter
 * @MATRIX_API_PUSHER_CONDITION_KIND_PROFILE_TAG: matches the profile
 *                                                tag of the device
 *                                                that the
 *                                                notification would
 *                                                be delivered
 *                                                to. Requires a
 *                                                <code>profile_tag</code>
 *                                                parameter
 * @MATRIX_API_PUSHER_CONDITION_KIND_CONTAINS_DISPLAY_NAME: matches
 *                                                          unencrypted
 *                                                          messages
 *                                                          where the
 *                                                          content's
 *                                                          body
 *                                                          contains
 *                                                          the
 *                                                          owner's
 *                                                          display
 *                                                          name in
 *                                                          that room.
 * @MATRIX_API_PUSHER_CONDITION_KIND_ROOM_MEMBER_COUNT: matches the
 *                                                      current number
 *                                                      of members in
 *                                                      the
 *                                                      room. Requires
 *                                                      an
 *                                                      <code>is</code>
 *                                                      parameter,
 *                                                      which must be
 *                                                      an integer,
 *                                                      optionally
 *                                                      prefixed by
 *                                                      <code>==</code>,
 *                                                      <code>&lt;</code>,
 *                                                      <code>&gt;</code>,
 *                                                      <code>&lt;=</code>
 *                                                      or
 *                                                      <code>&gt;=</code>. If
 *                                                      the prefix is
 *                                                      omitted, it
 *                                                      defaults to
 *                                                      <code>==</code>
 *
 * Condition types for pushers.
 */

/**
 * MatrixAPIEventDirection:
 * @MATRIX_API_EVENT_DIRECTION_FORWARD: List events after the
 *                                      specified one
 * @MATRIX_API_EVENT_DIRECTION_BACKWARD: List events before the
 *                                       specified one
 *
 * Direction of events when requesting an event context.
 */

/**
 * MatrixAPIReceiptType:
 * @MATRIX_API_RECEIPT_TYPE_READ: indicate that the message has been
 *                                read
 *
 * Receipt types of acknowledgment.
 */

/**
 * MatrixAPIEventFormat:
 * @MATRIX_API_EVENT_FORMAT_DEFAULT: event format will be omitted from
 *                                   the filter, so the server will
 *                                   use its default (usually
 *                                   @MATRIX_API_EVENT_FORMAT_FEDERATION)
 * @MATRIX_API_EVENT_FORMAT_CLIENT: return the events in a format
 *                                  suitable for clients
 * @MATRIX_API_EVENT_FORMAT_FEDERATION: return the raw event as
 *                                      receieved over federation
 *
 * Event format received when synchronizing.
 */

/**
 * MatrixAPIPusher:
 * @app_display_name: a string that will allow the user to identify
 *                    what application owns the pusher
 * @app_id: a reverse DNS style identifier for the application. It is
 *          recommended that this ends with the platform, such that
 *          different platform versions get different app
 *          identifiers. Maximum length is 64 characters, which is
 *          currently not enforced by this SDK
 * @append: if %TRUE, the homeserver should add another pusher with
 *          the given push key and app ID in addition to any others
 *          with different user IDs. Otherwise, the homeserver must
 *          remove any other pushers with the same App ID and pushkey
 *          for different users
 * @device_display_name: a string that will allow the user to identify
 *                       what device owns this pusher
 * @kind: the kind of pusher to configure. <code>http</code> makes a
 *        pusher that sends HTTP pokes. %NULL deletes the pusher.
 * @lang: the preferred language for receiving notifications,
 *        e.g. <code>en</code> or <code>en-US</code>
 * @profile_tag: a string that determines what set of device rules
 *               will be matched when evaluating push rules for this
 *               pusher. It is an arbitrary string. Multiple devices
 *               may use the same <code>profile_tag</code>. It is
 *               advised that when an app's data is copied or restored
 *               to a different device, this value remain the
 *               same. Client apps should offer ways to change the
 *               profile_tag, optionally copying rules from the old
 *               profile tag. Maximum length is 32 bytes, which is
 *               currently not enforced by this SDK
 * @pushkey: a unique identifier for this pusher. The value you should
 *           use for this is the routing or destination address
 *           information for the notification, for example, the APNS
 *           token for APNS or the Registration ID for GCM. If your
 *           notification client has no such concept, use any unique
 *           identifier. Maximum length is 512 bytes, which is
 *           currently not enforced by this SDK
 * @data: A dictionary of information for the pusher implementation
 *        itself. For example, if kind is <code>http</code>, this
 *        should contain <code>url</code> which is the URL to use to
 *        send notifications to.
 *
 * Ruleset for creating pushers.
 */

/**
 * MatrixAPI3PidCredential:
 * @client_secret: the client secret used in the session with the
 *                 Identity Server
 * @id_server: the Identity Server to use
 * @session_id: the session identifier given by the Identity Server
 *
 * Structure to store credentials to use with Identity Server
 * communication.
 */

/**
 * MatrixAPIEventFilter:
 * @rooms: (element-type GString): a list of room IDs to include. If
 *         %NULL, all rooms will be included. A <code>*</code> can be
 *         used as a wildcard to match any sequence of characters
 * @not_rooms: (element-type GString): a list of room IDs to
 *             exclude. If %NULL, no rooms are excluded. A matching
 *             room will be excluded even if it is listed in @rooms. A
 *             <code>*</code> can be used as a wildcard to match any
 *             sequence of characters
 * @limit: the maximum number of events to return. If <code>0</code>,
 *         no limit is applied
 * @senders: (element-type GString): a list of senders IDs to
 *           include. If %NULL then all senders are included. A
 *           <code>*</code> can be used as a wildcard to match any
 *           sequence of characters
 * @not_senders: (element-type GString): a list of sender IDs to
 *               exclude. If %NULL then no senders are excluded. A
 *               matching sender will be excluded even if it is listed
 *               in the @senders filter. A <code>*</code> can be used
 *               as a wildcard to match any sequence of characters
 * @types: (element-type GString): a list of event types to
 *         include. If %NULL then all event types are included. A
 *         <code>*</code> can be used as a wildcard to match any
 *         sequence of characters
 * @not_types: (element-type GString): a list of event types to
 *             exclude. If this list is absent then no event types are
 *             excluded. A matching type will be excluded even if it
 *             is listed in the @types filter. A <code>*</code> can be
 *             used as a wildcard to match any sequence of characters
 *
 * A struct to hold event filters.
 */

/**
 * MatrixAPIRoomFilter:
 * @ephemeral: the events that aren't recorded in the room history,
 *             e.g. typing and receipts, to include for rooms
 * @include_leave: include rooms that the user has left in the sync
 * @state: the state events to include for rooms
 * @timeline: the message and state update events to include for rooms
 *
 * A struct to hold a room event filter
 */

/**
 * MatrixAPIPresenceFilter:
 * @limit: the maximum number of events to return. If <code>0</code>,
 *         no limit will be applied
 * @senders: (element-type GString): a list of senders IDs to
 *           include. If %NULL then all senders are included. A
 *           <code>*</code> can be used as a wildcard to match any
 *           sequence of characters
 * @not_senders: (element-type GString): a list of sender IDs to
 *               exclude. If %NULL then no senders are excluded. A
 *               matching sender will be excluded even if it is listed
 *               in the @senders filter. A <code>*</code> can be used
 *               as a wildcard to match any sequence of characters
 * @types: (element-type GString): a list of event types to
 *         include. If %NULL then all event types are included. A
 *         <code>*</code> can be used as a wildcard to match any
 *         sequence of characters
 * @not_types: (element-type GString): a list of event types to
 *             exclude. If %NULL then no event types are excluded. A
 *             matching type will be excluded even if it is listed in
 *             the @types filter. A <code>*</code> can be used as a
 *             wildcard to match any sequence of characters
 *
 * A struct to hold a presence filter.
 */

/**
 * MatrixAPIFilter:
 * @event_fields: (element-type GString): list of event fields to
 *                include. If %NULL then all fields are included. The
 *                entries may include <code>.</code> charaters to
 *                indicate sub-fields. So
 *                <code>['content.body']</code> will include the
 *                <code>body</code> field of the <code>content</code>
 *                object. A literal <code>.</code> character in a
 *                field name may be escaped using a <code>\</code>. A
 *                server may include more fields than were requested
 * @event_format: the format to use for
 *                events. %MATRIX_API_EVENT_FORMAT_CLIENT will return
 *                the events in a format suitable for
 *                clients. %MATRIX_API_EVENT_FORMAT_FEDERATION will
 *                return the raw event as receieved over
 *                federation. The default is
 *                %MATRIX_API_EVENT_FORMAT_CLIENT
 * @presence: the presence updates to include
 * @room: room filters
 *
 * A struct to define an event filter.
 */

/**
 * MatrixAPIStateEvent:
 * @type: the event type
 * @state_key: the key of the state event
 * @content: the contents of the state event
 *
 * A struct to hold a state event filter.
 */

/**
 * matrix_api_error_quark:
 *
 * Gets the Matrix API error #GQuark
 */
G_DEFINE_QUARK(matrix-api-error-quark, matrix_api_error);

G_DEFINE_INTERFACE(MatrixAPI, matrix_api, G_TYPE_OBJECT);

static void
matrix_api_default_init(MatrixAPIInterface *iface)
{
    /**
     * MatrixAPI:token:
     *
     * The token to use for authorization. The matrix_http_api_login()
     * and matrix_http_api_register_account() calls set this
     * automatically.
     */
    g_object_interface_install_property(
            iface,
            g_param_spec_string("token", "Authorization token",
                                "The authorization token to use in requests",
                                NULL,
                                G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));
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
    g_return_if_fail(MATRIX_IS_API(api));

    return MATRIX_API_GET_IFACE(api)
        ->get_token(api);
}

/**
 * matrix_api_set_token:
 * @api: a #MatrixAPI implementation
 * @token: the authorization token to set
 *
 * Set the authorization token to use in subsequent requests.
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

/* Media */

/**
 * matrix_api_media_download:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async) (allow-none): a function to call when the
 *            request is finished
 * @user_data: user data to pass to the callback function @callback
 * @server_name: the server name from the <code>mxc://</code> URI (the
 *               authority component)
 * @media_id: the media ID from the <code>mxc://</code> URI (the path
 *            component)
 * @error: (allow-none): a #GError
 *
 * Download content from the content repository.
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
 * @user_data: user data to pass to the callback function @callback
 * @server_name: the server name from the <code>mxc://</code> URI (the
 *               authority component)
 * @media_id: the media ID from the <code>mxc://</code> URI (the path
 *            component)
 * @width: the desired width of the thumbnail, or 0 to use the default
 * @height: the desired height of the thumbnail, or 0 to use the
 *          default
 * @method: the resizing method to use
 * @error: (allow-none): a #GError
 *
 * Download a thumbnail of the content from the content
 * repository. The actual thumbnail may not match the size specified.
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
 * @user_data: user data to pass to the callback function @callback
 * @content_type: (allow-none): the content type of the file being
 *                uploaded
 * @content: the content to be uploaded
 * @error: a #GError
 *
 * Upload some content to the content repository.
 */
void
matrix_api_media_upload(MatrixAPI *api,
                        MatrixAPICallback callback,
                        gpointer user_data,
                        const gchar *content_type,
                        const GByteArray *content,
                        GError **error)
{
    g_return_if_fail(MATRIX_IS_API(api));

    MATRIX_API_GET_IFACE(api)
        ->media_upload(api, callback, user_data, content_type, content, error);
}

/* Presence */

/**
 * matrix_api_get_presence_list:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async) (allow-none): a function to call when the
 *            request is finished
 * @user_data: user data to pass to the callback function @callback
 * @user_id: the user whose presence list should be retrieved
 * @error: a #GError
 *
 * Retrieve a list of presence events for every user on this list.
 */
void
matrix_api_get_presence_list(MatrixAPI *api,
                             MatrixAPICallback callback,
                             gpointer user_data,
                             const gchar *user_id,
                             GError **error)
{
    g_return_if_fail(MATRIX_IS_API(api));

    MATRIX_API_GET_IFACE(api)
        ->get_presence_list(api, callback, user_data, user_id, error);
}

/**
 * matrix_api_update_presence_list:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async) (allow-none): a function to call when the
 *            request is finished
 * @user_data: user data to pass to the callback function @callback
 * @user_id: the user whose presence list is being modified
 * @drop_ids: (element-type GString): a list of user IDs to remove
 *            from the list
 * @invite_ids: (element-type GString): a list of user IDs to add to
 *              the list
 * @error: a #GError
 *
 * Add or remove users from the specified user's presence list.
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
 * @user_data: user data to pass to the callback function @callback
 * @user_id: the user whose presence list is being modified
 * @error: a #GError
 *
 * Get the given user's presence state.
 */
void
matrix_api_get_user_presence(MatrixAPI *api,
                             MatrixAPICallback callback,
                             gpointer user_data,
                             const gchar *user_id,
                             GError **error)
{
    g_return_if_fail(MATRIX_IS_API(api));

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
 * @user_data: user data to pass to the callback function @callback
 * @user_id: the user whose presence list is being modified
 * @presence: the new presence state
 * @status_message: a status message attached to this state
 * @error: a #GError
 *
 * Set the given user's presence. You cannot set the presence of
 * another user.
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

    MATRIX_API_GET_IFACE(api)
        ->set_user_presence(api,
                            callback, user_data,
                            user_id, presence, status_message,
                            error);
}

/* Push notifications */

/**
 * matrix_api_modify_pusher:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async) (allow-none): a function to call when the
 *            request is finished
 * @user_data: user data to pass to the callback function @callback
 * @pusher: the pusher information
 * @error: a #GError
 *
 * Modify a pushers for the active user on this homeserver.
 */
void
matrix_api_modify_pusher(MatrixAPI *api,
                         MatrixAPICallback callback,
                         gpointer user_data,
                         MatrixAPIPusher *pusher,
                         GError **error)
{
    g_return_if_fail(MATRIX_IS_API(api));

    MATRIX_API_GET_IFACE(api)
        ->modify_pusher(api,
                        callback, user_data,
                        pusher, error);
}

/**
 * matrix_api_get_pushers:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async) (allow-none): a function to call when the
 *            request is finished
 * @user_data: user data to pass to the callback function @callback
 * @error: a #GError
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
 * @user_data: user data to pass to the callback function @callback
 * @scope: either <code>global</code> to specify global rules, or
 *         <code>device/&lt;profile tag&gt;</code> for rules for a
 *         given <code>profile tag</code>.
 * @kind: the kind of rule
 * @rule_id: an identifier for the rule
 * @error: a #GError
 *
 * Delete a push rule.
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
 * @user_data: user data to pass to the callback function @callback
 * @scope: either <code>global</code> to specify global rules, or
 *         <code>device/&lt;profile tag&gt;</code> for rules for a
 *         given <code>profile tag</code>.
 * @kind: the kind of rule
 * @rule_id: an identifier for the rule
 * @error: a #GError
 *
 * Retrieve a specific push rule.
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
 * @user_data: user data to pass to the callback function @callback
 * @scope: either <code>global</code> to specify global rules, or
 *         <code>device/&lt;profile tag&gt;</code> for rules for a
 *         given <code>profile tag</code>.
 * @kind: the kind of rule
 * @rule_id: an identifier for the rule
 * @before: (allow-none): make the new rule the next-most important
 *          than this rule ID
 * @after: (allow-none): make the new rule the next-less important
 *         than this rule ID
 * @actions: (element-type GString): the actions to perform when the
 *           conditions for this rule are met
 * @conditions: (element-type MatrixAPIPusherConditionKind) (allow-none):
 *              the conditions that must hold true for an event for a
 *              rule to be applied. A rule with no conditions always
 *              matches
 * @error: a #GError
 *
 * Add or change a push rule.
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
 * @user_data: user data to pass to the callback function @callback
 * @scope: either <code>global</code> to specify global rules, or
 *         <code>device/&lt;profile tag&gt;</code> for rules for a
 *         given <code>profile tag</code>.
 * @kind: the kind of rule
 * @rule_id: an identifier for the rule
 * @enabled: if %TRUE, the rule will be enabled, otherwise it gets
 *           disabled
 * @error: a #GError
 *
 * Enable or disable the specified push rule.
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
 * @user_data: (allow-none): user data to pass to the callback function
 * @preset: a room preset to use
 * @room_name: (allow-none): the desired name for the room
 * @room_alias: (allow-none): the alias of the room
 * @topic: (allow-none): the topic of the room
 * @visibility: the initial visibility of the room
 * @creation_content: (allow-none): extra keys to be added to the
 *                    content of m.room.create
 * @initial_state: (element-type MatrixAPIStateEvent) (allow-none): A
 *                 list of state events to set in the new room
 * @invitees: (element-type GString) (allow-none): list of user IDs to
 *            invite to the new room
 * @error: (allow-none): a #GError
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
                       GError **error)
{
    g_return_if_fail(MATRIX_IS_API(api));

    MATRIX_API_GET_IFACE(api)
        ->create_room(api, callback, user_data,
                      preset, room_name, room_alias, topic,
                      visibility, creation_content,
                      initial_state, invitees,
                      error);
}

/* Room directory */

/**
 * matrix_api_delete_room_alias:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async) (allow-none): the function to call when
 *            the request is finished
 * @user_data: (allow-none): user data to pass to the callback function
 * @room_alias: the alias name to remove
 * @error: a #GError
 *
 * Remove the mapping of @room_alias to its room ID
 *
 * Servers may choose to implement additional access control checks
 * here, for instance that room aliases can only be deleted by their
 * creator or a server administrator.
 */
void
matrix_api_delete_room_alias(MatrixAPI *api,
                             MatrixAPICallback callback,
                             gpointer user_data,
                             const gchar *room_alias,
                             GError **error)
{
    g_return_if_fail(MATRIX_IS_API(api));

    MATRIX_API_GET_IFACE(api)
        ->delete_room_alias(api, callback, user_data, room_alias, error);
}

/**
 * matrix_api_get_room_id:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async) (allow-none): the function to call when
 *            the request is finished
 * @user_data: (allow-none): user data to pass to the callback function
 * @room_alias: the room alias
 * @error: a #GError
 *
 * Get the room ID corresponding to this room alias.
 */
void
matrix_api_get_room_id(MatrixAPI *api,
                       MatrixAPICallback callback,
                       gpointer user_data,
                       const gchar *room_alias,
                       GError **error)
{
    g_return_if_fail(MATRIX_IS_API(api));

    MATRIX_API_GET_IFACE(api)
        ->get_room_id(api, callback, user_data, room_alias, error);
}

/**
 * matrix_api_create_room_alias:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async) (allow-none): the function to call when
 *            the request is finished
 * @user_data: (allow-none): user data to pass to the callback function
 * @room_id: the room ID to add this alias to
 * @room_alias: the room alias to set
 * @error: a #GError
 *
 * Create a new mapping from room alias to room ID.
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
 * @user_data: (allow-none): user data to pass to the callback function
 * @error: a #GError
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
 * @user_data: user data to pass to the callback function
 * @room_id: the room ID where the user should be banned
 * @user_id: the user ID to ban
 * @reason: (allow-none): the reason of the ban
 * @error: (allow-none): a #GError
 *
 * Ban the specified user from the specified room. An optional reason
 * can be specified.
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

    MATRIX_API_GET_IFACE(api)
        ->ban_user(api, callback, user_data, room_id, user_id, reason, error);
}

/**
 * matrix_api_forget_room:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async): the function to call when the request is
 *            finished
 * @user_data: user data to pass to the callback function
 * @room_id: the room ID to forget
 * @error: a #GError
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
 */
void
matrix_api_forget_room(MatrixAPI *api,
                       MatrixAPICallback callback,
                       gpointer user_data,
                       const gchar *room_id,
                       GError **error)
{
    g_return_if_fail(MATRIX_IS_API(api));

    MATRIX_API_GET_IFACE(api)
        ->forget_room(api, callback, user_data, room_id, error);
}

/**
 * matrix_api_invite_user_3rdparty:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async): the function to call when the request is
 *            finished
 * @user_data: user data to pass to the callback function
 * @room_id: the room ID to which to invite the user
 * @address: the invitee's 3rd party identifier
 * @medium: the kind of address being passed in the address field,
 *          e.g. <code>email</code>
 * @id_server: the hostname+port of the identity server which should
 *             be used for 3rd party identifier lookups
 * @error: a #GError
 *
 * Invite a user to the room by a 3rd party identifier. They do not
 * start participating in the room until they actually join the room.
 *
 * If the identity server does not know a Matrix user identifier for
 * the passed third party identifier, the homeserver will issue an
 * invitation which can be accepted upon providing proof of ownership
 * of the third party identifier.
 */
void matrix_api_invite_user_3rdparty(MatrixAPI *api,
                                     MatrixAPICallback callback,
                                     gpointer user_data,
                                     const gchar *room_id,
                                     const gchar *address,
                                     const gchar *medium,
                                     const gchar *id_server,
                                     GError **error)
{
    g_return_if_fail(MATRIX_IS_API(api));

    MATRIX_API_GET_IFACE(api)
        ->invite_user_3rdparty(api,
                               callback, user_data,
                               room_id, address, medium, id_server,
                               error);
}

/**
 * matrix_api_invite_user:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async): the function to call when the request is
 *            finished
 * @user_data: user data to pass to the callback function
 * @room_id: the room ID to invite the user to
 * @user_id: the user ID to invite
 * @error: a #GError
 *
 * Invite a user to a room.
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

    MATRIX_API_GET_IFACE(api)
        ->invite_user(api, callback, user_data, room_id, user_id, error);
}

/**
 * matrix_api_join_room:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async): the function to call when the request is
 *            finished
 * @user_data: user data to pass to the callback function
 * @room_id_or_alias: the room ID or room alias to join to
 * @error: a #GError
 *
 * Join a room.
 */
void
matrix_api_join_room(MatrixAPI *api,
                     MatrixAPICallback callback,
                     gpointer user_data,
                     const gchar *room_id_or_alias,
                     GError **error)
{
    g_return_if_fail(MATRIX_IS_API(api));

    MATRIX_API_GET_IFACE(api)
        ->join_room(api, callback, user_data, room_id_or_alias, error);
}

/**
 * matrix_api_leave_room:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async): the function to call when the request is
 *            finished
 * @user_data: user data to pass to the callback function
 * @room_id: the room ID to kick the user from
 * @error: a #GError
 *
 * Leave a room
 */
void
matrix_api_leave_room(MatrixAPI *api,
                      MatrixAPICallback callback,
                      gpointer user_data,
                      const gchar *room_id,
                      GError **error)
{
    g_return_if_fail(MATRIX_IS_API(api));

    MATRIX_API_GET_IFACE(api)
        ->leave_room(api, callback, user_data, room_id, error);
}

/* Room participation */

/**
 * matrix_api_event_stream:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async): the function to call when the request is
 *            finished
 * @user_data: user data to pass to the callback function
 * @from_token: (allow-none): events will be listed from this token
 * @timeout: timeout of the request
 * @error: a #GError
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
 * @user_data: user data to pass to the callback function
 * @event_id: the event ID to get
 * @error: a #GError
 *
 * Get a single event by event ID.
 */
void
matrix_api_get_event(MatrixAPI *api,
                     MatrixAPICallback callback,
                     gpointer user_data,
                     const gchar *event_id,
                     GError **error)
{
    g_return_if_fail(MATRIX_IS_API(api));

    MATRIX_API_GET_IFACE(api)
        ->get_event(api, callback, user_data, event_id, error);
}

/**
 * matrix_api_initial_sync:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async): the function to call when the request is
 *            finished
 * @user_data: user data to pass to the callback function
 * @limit: the maximum number of events to get
 * @archived: whether to include rooms that the user has left
 * @error: a #GError
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
 * @user_data: user data to pass to the callback function
 * @room_id: the room to get events from
 * @event_id: the event to get context around
 * @limit: the maximum number of events to get. If 0, a default value
 *         is used (10, according to the specification)
 * @error: a #GError
 *
 * Gets a number of events that happened just before and after the
 * specified event.
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
 * @user_data: user data to pass to the callback function
 * @room_id: the room to get the data for
 * @error: a #GError
 *
 * Get a copy of the current state and the most recent messages in a
 * room.
 */
void
matrix_api_initial_sync_room(MatrixAPI *api,
                             MatrixAPICallback callback,
                             gpointer user_data,
                             const gchar *room_id,
                             GError **error)
{
    g_return_if_fail(MATRIX_IS_API(api));

    MATRIX_API_GET_IFACE(api)
        ->initial_sync_room(api, callback, user_data, room_id, error);
}

/**
 * matrix_api_list_room_members:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async): the function to call when the request is
 *            finished
 * @user_data: user data to pass to the callback function
 * @room_id: the room to get the member events for
 * @error: a #GError
 *
 * Get the list of members for a room.
 */
void
matrix_api_list_room_members(MatrixAPI *api,
                             MatrixAPICallback callback,
                             gpointer user_data,
                             const gchar *room_id,
                             GError **error)
{
    g_return_if_fail(MATRIX_IS_API(api));

    MATRIX_API_GET_IFACE(api)
        ->list_room_members(api, callback, user_data, room_id, error);
}

/**
 * matrix_api_list_room_messages:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async): the function to call when the request is
 *            finished
 * @user_data: user data to pass to the callback function
 * @room_id: the room to get the events for
 * @from_token: the token to start returning events from. This token
 *              can be obtained by calling matrix_api_initial_sync()
 *              or matrix_api_initial_sync_room()
 * @direction: the direction of the returned events
 * @limit: the maximum number of events to return. If 0, a default
 *         value will be used (10, according to the specification
 * @error: a #GError
 *
 * Get a list of message and state events for a room.
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
 * @user_data: user data to pass to the callback function
 * @room_id: the room in which to send the event
 * @type: type of the receipt
 * @event_id: the event ID to acknowledge up to
 * @receipt: extra receipt information to attach. Note that the server
 *           will automatically attach the <code>ts</code> field
 * @error: a #GError
 *
 * Update the marker for the given receipt type to the event ID specified.
 */
void
matrix_api_send_event_receipt(MatrixAPI *api,
                              MatrixAPICallback callback,
                              gpointer user_data,
                              const gchar *room_id,
                              MatrixAPIReceiptType type,
                              const gchar *event_id,
                              JsonNode *receipt,
                              GError **error)
{
    g_return_if_fail(MATRIX_IS_API(api));

    MATRIX_API_GET_IFACE(api)
        ->send_event_receipt(api,
                             callback, user_data,
                             room_id, type, event_id, receipt,
                             error);
}

/**
 * matrix_api_redact_event:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async): the function to call when the request is
 *            finished
 * @user_data: user data to pass to the callback function
 * @room_id: the room from which to redact the event
 * @event_id: the event ID to acknowledge up to
 * @txn_id: the transaction ID for this event. Clients should generate
 *          a unique ID; it will be used by the server to ensure
 *          idempotency of requests
 * @reason: (allow-none): the reason for the event being redacted
 * @error: a #GError
 *
 * Strip all information out of an event which isn't critical to the
 * integrity of the server-side representation of the room. This
 * cannot be undone.
 *
 * Users may redact their own events, and any user with a power level
 * greater than or equal to <code>redact</code> power level of the
 * room may redact events there.
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
 * @user_data: user data to pass to the callback function
 * @room_id: the room to send the event to
 * @event_type: the type of event to send
 * @txn_id: the transaction ID for this event. Clients should generate
 *          a unique ID; it will be used by the server to ensure
 *          idempotency of requests
 * @content: the content of the event as a #JsonNode
 * @error: a #GError
 *
 * Send a message event to the room.
 */
void
matrix_api_send_message_event(MatrixAPI *api,
                              MatrixAPICallback callback,
                              gpointer user_data,
                              const gchar *room_id,
                              const gchar *event_type,
                              const gchar *txn_id,
                              const JsonNode *content,
                              GError **error)
{
    g_return_if_fail(MATRIX_IS_API(api));

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
 * @user_data: user data to pass to the callback function
 * @room_id: the room ID to get a state for
 * @event_type: (allow-none): the type of state to look up
 * @state_key: (allow-none): the key of the state to look up. If
 *             @event_type is %NULL, this parameter is ignored
 * @error: a #GError
 *
 * Look up the contents of a state event in a room. If both
 * @event_type and @state_key are empty, get a list of state events
 * for that room.
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
 * @user_data: user data to pass to the callback function
 * @room_id: the room ID to get a state for
 * @event_type: the type of state to look up
 * @state_key: (allow-none): the key of the state to look up. If
 *             @event_type is %NULL, this parameter is ignored
 * @content: the content of the state event
 * @error: a #GError
 *
 * Send a state event to the room. These events will be overwritten if
 * @room_id, @event_type and @state_key all match.
 *
 * This request cannot use transaction IDs.
 *
 * The required fields in the body of the request (@content) vary
 * depending on the type of the event.
 */
void
matrix_api_send_room_event(MatrixAPI *api,
                           MatrixAPICallback callback,
                           gpointer user_data,
                           const gchar *room_id,
                           const gchar *event_type,
                           const gchar *state_key,
                           const JsonNode *content,
                           GError **error)
{
    g_return_if_fail(MATRIX_IS_API(api));

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
 * @user_data: user data to pass to the callback function
 * @user_id: the user who has started to type
 * @room_id: the room in which the user is typing
 * @timeout: the length of time in milliseconds to mark this user as
 *           typing
 * @typing: whether the user is typing or not. If %FALSE, @timeout can
 *          be omitted (ie. set to 0)
 * @error: a #GError
 *
 * Tell the server the user is typing for the next @timeout
 * milliseconds. If @typing is %FALSE, it tells the server that the
 * user stopped typing.
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
 * @user_data: user data to pass to the callback function
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
 * @error: a #GError
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
 * @user_data: user data to pass to the callback function
 * @user_id: the ID of the user uploading the filter. An access token
 *           must be present (either specifying one with
 *           matrix_api_set_token() or requested from the server via
 *           matrix_api_register_account() or matrix_api_login().
 * @filter: the filter to upload
 * @error: a #GError
 *
 * Upload a new filter definition to the homeserver. It will return a
 * filter ID that may be used in future requests.
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
 * @user_data: user data to pass to the callback function
 * @user_id: the user ID to download a filter from
 * @filter_id: the filter ID to download
 * @error: a #GError
 *
 * Download a filter.
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
 * @user_data: user data to pass to the callback function
 * @user_id: the user ID to look up
 * @error: a #GError
 *
 * Get information about a particular user.
 */
void
matrix_api_whois(MatrixAPI *api,
                 MatrixAPICallback callback,
                 gpointer user_data,
                 const gchar *user_id,
                 GError **error)
{
    g_return_if_fail(MATRIX_IS_API(api));

    MATRIX_API_GET_IFACE(api)
        ->whois(api, callback, user_data, user_id, error);
}

/* Session management */

/**
 * matrix_api_login:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async): the function to call when the request is
 *            finished
 * @user_data: user data to pass to the callback function
 * @type: the login type to use
 * @content: (allow-none): parameters to pass for the login request
 * @error: (allow-none): a #GError
 *
 * Attempt to login with type @type. Implementations of this method
 * must set the token property on a successful login.
 */
void
matrix_api_login(MatrixAPI *api,
                 MatrixAPICallback callback,
                 gpointer user_data,
                 const gchar *type,
                 const JsonNode *content,
                 GError **error)
{
    g_return_if_fail(MATRIX_IS_API(api));

    MATRIX_API_GET_IFACE(api)
        ->login(api, callback, user_data, type, content, error);
}

/**
 * matrix_api_token_refresh:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async): the function to call when the request is
 *            finished
 * @user_data: user data to pass to the callback function
 * @refresh_token: the refresh token that was issued by the server
 * @error: a #GError
 *
 * Exchanges a refresh token for a new access token. This is intended
 * to be used if the access token has expired.
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
 * @user_data: user data to pass to the callback function
 * @error: a #GError
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
 * @user_data: user data to pass to the callback function
 * @bind_creds: whether the homeserver should also bind this third
 *              party identifier to the account's Matrix ID with the
 *              passed Identity Server.
 * @threepid_creds: the credentials to associate with the account
 * @error: a #GError
 *
 * Add contact information to the user's account.
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

    MATRIX_API_GET_IFACE(api)
        ->add_3pid(api, callback, user_data, bind_creds, threepid_creds, error);
}

/**
 * matrix_api_change_password:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async): the function to call when the request is
 *            finished
 * @user_data: user data to pass to the callback function
 * @new_password: the new password for the account
 * @error: a #GError
 *
 * Change the active user's password.
 */
void
matrix_api_change_password(MatrixAPI *api,
                           MatrixAPICallback callback,
                           gpointer user_data,
                           const gchar *new_password,
                           GError **error)
{
    g_return_if_fail(MATRIX_IS_API(api));

    MATRIX_API_GET_IFACE(api)
        ->change_password(api, callback, user_data, new_password, error);
}

/**
 * matrix_api_get_profile:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async): the function to call when the request is
 *            finished
 * @user_data: user data to pass to the callback function
 * @user_id: the user whose profile to get
 * @error: a #GError
 *
 * Get a user's profile.
 */
void
matrix_api_get_profile(MatrixAPI *api,
                       MatrixAPICallback callback,
                       gpointer user_data,
                       const gchar *user_id,
                       GError **error)
{
    g_return_if_fail(MATRIX_IS_API(api));

    MATRIX_API_GET_IFACE(api)
        ->get_profile(api, callback, user_data, user_id, error);
}

/**
 * matrix_api_get_avatar_url:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async): the function to call when the request is
 *            finished
 * @user_data: user data to pass to the callback function
 * @user_id: the user whose avatar URL to get
 * @error: a #GError
 *
 * Get the URL of the specified user's avatar.
 */
void
matrix_api_get_avatar_url(MatrixAPI *api,
                          MatrixAPICallback callback,
                          gpointer user_data,
                          const gchar *user_id,
                          GError **error)
{
    g_return_if_fail(MATRIX_IS_API(api));

    MATRIX_API_GET_IFACE(api)
        ->get_avatar_url(api, callback, user_data, user_id, error);
}

/**
 * matrix_api_set_avatar_url:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async): the function to call when the request is
 *            finished
 * @user_data: user data to pass to the callback function
 * @user_id: the user whose avatar URL to set
 * @avatar_url: the avatar URL info
 * @error: a #GError
 *
 * Set the user's avatar URL.
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

    MATRIX_API_GET_IFACE(api)
        ->set_avatar_url(api, callback, user_data, user_id, avatar_url, error);
}

/**
 * matrix_api_get_display_name:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async): the function to call when the request is
 *            finished
 * @user_data: user data to pass to the callback function
 * @user_id: the user whose display name to get
 * @error: a #GError
 *
 * Get the user's display name.
 */
void
matrix_api_get_display_name(MatrixAPI *api,
                            MatrixAPICallback callback,
                            gpointer user_data,
                            const gchar *user_id,
                            GError **error)
{
    g_return_if_fail(MATRIX_IS_API(api));

    MATRIX_API_GET_IFACE(api)
        ->get_display_name(api, callback, user_data, user_id, error);
}

/**
 * matrix_api_set_display_name:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async): the function to call when the request is
 *            finished
 * @user_data: user data to pass to the callback function
 * @user_id: the user whose display name to set
 * @display_name: the display name info
 * @error: a #GError
 *
 * Set the user's display name.
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
 * @user_data: user data to pass to the callback function
 * @bind_email: if %TRUE, the server binds the e-mail used for
 *              authentication to the Matrix ID with the ID server
 * @username: (allow-none): the local part of the desired Matrix
 *            ID. If omitted, the server will generate a local part
 * @password: (allow-none): the desired password for the account
 * @error: a #GError
 *
 * Attempt to register with type @login_type. Implementations of this
 * method must set the token property on a successful login.
 */
void
matrix_api_register_account(MatrixAPI *api,
                            MatrixAPICallback callback,
                            gpointer user_data,
                            gboolean bind_email,
                            const gchar *username,
                            const gchar *password,
                            GError **error)
{
    g_return_if_fail(MATRIX_IS_API(api));

    MATRIX_API_GET_IFACE(api)
        ->register_account(api, callback, user_data, bind_email,
                           username, password, error);
}

/**
 * matrix_api_set_account_data:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async): the function to call when the request is
 *            finished
 * @user_data: user data to pass to the callback function
 * @user_id: the user to set account data for. An access token must be
 *           present and be authorized to make requests for this user
 *           ID
 * @room_id: (allow-none): the room to set account data for. If %NULL,
 *           the account data will be set globally
 * @type: the event type of the account data to set. Custom types
 *        should be namespaced to avoid clashes.
 * @content: the content of the account data
 * @error: a #GError
 *
 * Set some account data for the client. This config is only visible
 * to the user who set the account data. The config will be synced to
 * clients in the top-level account data.
 */
void
matrix_api_set_account_data(MatrixAPI *api,
                            MatrixAPICallback callback,
                            gpointer user_data,
                            const gchar *user_id,
                            const gchar *room_id,
                            const gchar *type,
                            const JsonNode *content,
                            GError **error)
{
    g_return_if_fail(MATRIX_IS_API(api));

    MATRIX_API_GET_IFACE(api)
        ->set_account_data(api,
                           callback, user_data,
                           user_id, room_id, type, content, error);
}

/**
 * matrix_api_get_room_tags:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async): the function to call when the request is
 *            finished
 * @user_data: user data to pass to the callback function
 * @user_id: the ID of the user to get the tags for. An access token
 *           must be set, and it must be authorised to make requests
 *           for this user ID
 * @room_id: the room to get tags for
 * @error: a #GError
 *
 * List the tags set by a user on a room.
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

    MATRIX_API_GET_IFACE(api)
        ->get_room_tags(api, callback, user_data, user_id, room_id, error);
}

/**
 * matrix_api_delete_room_tag:
 * @api: a #MatrixAPI implementation
 * @callback: (scope async): the function to call when the request is
 *            finished
 * @user_data: user data to pass to the callback function
 * @user_id: the id of the user to remove a tag for
 * @room_id: the id of the room to remove the tag from
 * @tag: the tag to remove
 * @error: a #GError
 *
 * Remove a tag from the room.
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
 * @user_data: user data to pass to the callback function
 * @user_id: the ID of the user to add the tag for
 * @room_id: the ID of the room to add the tag for
 * @tag: the tag to add
 * @content: extra data for the tag, e.g. ordering
 * @error: a #GError
 *
 * Add a tag to the room.
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
 * @user_data: user data to pass to the callback function
 * @error: a #GError
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
