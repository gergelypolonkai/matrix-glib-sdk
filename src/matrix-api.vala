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
 * Base interface for all Client/Server API functionality.
 */
public interface Matrix.API : GLib.Object {
    /**
     * The token to use for authorization. The Matrix.API.login() and
     * Matrix.API.register_account() calls MUST set this
     * automatically.
     */
    public abstract string? token { get; set; default = null; }

    /**
     * The token to use for refreshing the authorization token. It is
     * issued by the server after a successful registration, login or
     * token refresh.
     */
    public abstract string? refresh_token { get; set; default = null; }

    /**
     * The Matrix user ID that is currently authenticated with the
     * server. It is set automatically by the registration and login
     * process.
     */
    public abstract string? user_id { get; default = null; }

    /**
     * The name of the Matrix home server as it calls itself. It is
     * set automatically by the registration and login process.
     */
    public abstract string? homeserver { get; default = null; }

    /**
     * A callback function to use with Client API calls. If the
     * response is JSON, @param json_content is set and @param
     * raw_content is not. If the response is not JSON, but still has
     * a body (e.g. with media downloads), @param json_content is
     * {{{null}}} and @raw_content holds the content body.
     *
     * @param api a Matrix.API instance
     * @param content_type the content type of the response
     * @param json_content the JSON content of the response
     * @param raw_content the raw (ie. binary) content of the response
     * @param err a GLib.Error that holds any errors that occured
     *            during the API call, or {{{null}}}
     */
    public delegate void
    Callback(Matrix.API api,
             string content_type,
             Json.Node? json_content,
             GLib.ByteArray? raw_content,
             Matrix.Error? err);

    /**
     * Abort all pending requests toward a Matrix homeserver. Be aware
     * that this may leave requests in an inconsistent state.
     */
    public abstract void
    abort_pending();

    /* Media */

    /**
     * Download content from the content repository.
     *
     * Implementors: If server_name or media_id is {{{null}}},
     * implementations MUST throw Matrix.Error.INCOMPLETE.
     *
     * @param callback a function to call when the request is finished
     * @param server_name the server name from the `mxc:` URI
     * @param media_id the media ID from the `mxc:` URI
     */
    public abstract void
    media_download([CCode (delegate_target_pos = 1.5, scope = "async", destroy_notify_pos = -1)]
                   owned Matrix.API.Callback? @callback,
                   string server_name,
                   string media_id)
        throws Matrix.Error;

    /**
     * Download a thumbnail of the content from the content
     * repository. The actual thumbnail may not match the size
     * specified.
     *
     * If @param server_name or @param media_id is {{{null}}}, this
     * function returns immediately, and throws
     * Matrix.Error.INCOMPLETE.
     *
     * @param callback a function to call when the request is finished
     * @param server_name the server name from the `mxc:` URI
     * @param media_id the media ID from the `mxc:` URI
     */
    public abstract void
    media_thumbnail([CCode (delegate_target_pos = 1.5, scope = "async")]
                    owned Matrix.API.Callback? @callback,
                    string server_name,
                    string media_id,
                    uint width,
                    uint height,
                    Matrix.ResizeMethod method)
        throws Matrix.Error;

    /**
     * Upload some content to the content repository.
     *
     * If @param content is {{{null}}}, this function returns
     * immediately, and throws Matrix.Error.INCOMPLETE.
     *
     * @param content_type the type of the content to be uploaded
     * @param content the content to be uploaded
     */
    public abstract void
    media_upload([CCode (delegate_target_pos = 1.5, scope = "async")]
                 owned Matrix.API.Callback? @callback,
                 string? content_type,
                 owned GLib.ByteArray content)
        throws Matrix.Error;

    /* Presence */

    /**
     * Retrieve a list of presence events for every user on this list.
     *
     * If @param user_id is {{{null}}}, this function returns immediately,
     * and throws Matrix.Error.INCOMPLETE.
     *
     * @param user_id the user whose presence list should be retrieved
     */
    public abstract void
    get_presence_list([CCode (delegate_target_pos = 1.5, scope = "async")]
                      owned Matrix.API.Callback? @callback,
                      string user_id)
        throws Matrix.Error;

    /**
     * Add or remove users from the specified user's presence list.
     *
     * If @param user_id, or both @param drop_ids and @param
     * invite_ids are {{{null}}}, this function returns immediately,
     * and throws Matrix.Error.INCOMPLETE.
     *
     * @param user_id the user whose presence list is being modified
     * @param drop_ids a list of user IDs to remove from the list
     * @param invite_ids a list of user IDs to add to the list
     */
    public abstract void
    update_presence_list([CCode (delegate_target_pos = 1.5, scope = "async")]
                         owned Matrix.API.Callback? @callback,
                         string user_id,
                         GLib.List<string> drop_ids,
                         GLib.List<string> invite_ids)
        throws Matrix.Error;

    /**
     * Get the given user's presence state.
     *
     * If @param user_id is {{{null}}}, this function returns
     * immediately, and throws Matrix.Error.INCOMPLETE.
     *
     * @param user_id the user whose presence list is being modified
     */
    public abstract void
    get_user_presence([CCode (delegate_target_pos = 1.5, scope = "async")]
                      owned Matrix.API.Callback? @callback,
                      string user_id)
        throws Matrix.Error;

    /**
     * Set the given user's presence. You cannot set the presence of
     * another user.
     *
     * If @param user_id is {{{null}}}, this function returns
     * immediately, and throws Matrix.Error.INCOMPLETE.
     *
     * @param user_id  the user whose presence list is being modified
     * @param presence the new presence state
     * @param status_message a status message attached to this state
     */
    public abstract void
    set_user_presence([CCode (delegate_target_pos = 1.5, scope = "async")]
                      owned Matrix.API.Callback? @callback,
                      string user_id,
                      Matrix.Presence presence,
                      string? status_message)
        throws Matrix.Error;

    /* Push notifications */

    /**
     * Create, update or delete a pusher for the active user on this
     * homeserver.
     *
     * If @param pusher is {{{null}}}, this function returns
     * immediately, and throws Matrix.Error.INCOMPLETE.
     */
    public abstract void
    update_pusher([CCode (delegate_target_pos = 1.5, scope = "async")]
                  owned Matrix.API.Callback? @callback,
                  Matrix.Pusher pusher)
        throws Matrix.Error;

    /**
     * Retrieve all push rulesets.
     */
    public abstract void
    get_pushers([CCode (scope = "async")]
                owned Matrix.API.Callback? @callback)
        throws Matrix.Error;

    /**
     * Delete a push rule.
     *
     * If @param scope or @param rule_id is {{{null}}}, this function
     * returns immediately, and throws
     * Matrix.Error.INCOMPLETE.
     *
     * @param callback a function to call when the request is finished
     * @param scope either {{{global}}} to specify global rules, or
     *              {{{device/&lt;profile tag&gt;}}} for rules for a
     *              given {{{profile tag}}}
     * @param kind the kind of rule
     * @param rule_id an identifier for the rule
     */
    public abstract void
    delete_pusher([CCode (delegate_target_pos = 1.5, scope = "async")]
                  owned Matrix.API.Callback? @callback,
                  string scope,
                  Matrix.PusherKind kind,
                  string rule_id)
        throws Matrix.Error;

    /**
     * Retrieve a specific push rule.
     *
     * If @param scope or @param rule_id is {{{null}}}, this function
     * returns immediately, and throws
     * Matrix.Error.INCOMPLETE.
     *
     * @param callback a function to call when the request is finished
     * @param scope either {{{global}}} to specify global rules, or
     *              {{{device/&lt;profile tag&gt;}}} for rules for a
     *              given {{{profile tag}}}.
     * @param kind the kind of rule
     * @param rule_id an identifier for the rule
     */
    public abstract void
    get_pusher([CCode (delegate_target_pos = 1.5, scope = "async")]
               owned Matrix.API.Callback? @callback,
               string scope,
               Matrix.PusherKind kind,
               string rule_id)
        throws Matrix.Error;

    /**
     * Add or change a push rule.
     *
     * If either @param scope, @param rule_id or @param actions are
     * {{{null}}}, this function returns immediately, and throws
     * Matrix.Error.INCOMPLETE.
     *
     * @param callback a function to call when the request is finished
     * @param scope either {{{global}}} to specify global rules, or
     *              {{{device/&lt;profile tag&gt;}}} for rules for a
     *              given {{{profile tag}}}
     * @param kind the kind of rule
     * @param rule_id an identifier for the rule
     * @param before make the new rule the next-most important than
     *               this rule ID
     * @param after make the new rule the next-less important than
     *              this rule ID
     * @param actions the actions to perform when the conditions for
     *                this rule are met
     * @param conditions the conditions that must hold true for an
     *                   event for a rule to be applied. A rule with
     *                   no conditions always matches
     */
    public abstract void
    add_pusher([CCode (delegate_target_pos = 1.5, scope = "async")]
               owned Matrix.API.Callback? @callback,
               string scope,
               Matrix.PusherKind kind,
               string rule_id,
               string? before,
               string? after,
               GLib.List<string> actions,
               GLib.List<Matrix.PusherConditionKind>? conditions)
        throws Matrix.Error;

    /**
     * Enable or disable the specified push rule.
     *
     * If @scope or @rule_id is %NULL, this function returns immediately,
     * and fills @error with %MATRIX_ERROR_INCOMPLETE.
     *
     * @param callback a function to call when the request is finished
     * @param scope either {{{global}}} to specify global rules, or
     *             {{{device/&lt;profile tag&gt;}}} for rules for a
     *             given {{{profile tag}}}
     * @param kind the kind of rule
     * @param rule_id an identifier for the rule
     * @param enabled if {{{true}}}, the rule will be enabled,
     *                otherwise it gets disabled
     */
    public abstract void
    toggle_pusher([CCode (delegate_target_pos = 1.5, scope = "async")]
                  owned Matrix.API.Callback? @callback,
                  string scope,
                  Matrix.PusherKind kind,
                  string rule_id,
                  bool enabled)
        throws Matrix.Error;

    /* Room creation */

    /**
     * Create a new room with the given name and invite the users
     * in @param invitees.
     *
     * @param callback the function to call when the request is
     *                 finished
     * @param preset a room preset to use
     * @param room_name the desired display name for the room
     * @param room_alias an alias of the room
     * @param topic the topic of the room
     * @param visibility the initial visibility of the room
     * @param creation_content extra keys to be added to the content
     *                         of m.room.create
     * @param initial_state a list of state events to set in the new
     *                      room
     * @param invitees list of user IDs to invite to the new room
     * @param invite_3pids a list of 3rd party credentials to invite
     *                     to the new room
     */
    public abstract void
    create_room([CCode (delegate_target_pos = 1.5, scope = "async")]
                owned Matrix.API.Callback? @callback,
                Matrix.RoomPreset preset,
                string? room_name,
                string? room_alias,
                string? topic,
                Matrix.RoomVisibility visibility,
                Json.Node? creation_content,
                GLib.List<Matrix.StateEvent>? initial_state,
                GLib.List<string>? invitees,
                GLib.List<Matrix.3PidCredential>? invite_3pids)
        throws Matrix.Error;

    /* Room directory */

    /**
     * Remove the mapping of @room_alias to its room ID
     *
     * Servers may choose to implement additional access control checks
     * here, for instance that room aliases can only be deleted by their
     * creator or a server administrator.
     *
     * If @room_alias is %NULL, this function returns immediately, and
     * fills @error with %MATRIX_ERROR_INCOMPLETE.
     *
     * @param callback the function to call when the request is
     *                 finished
     * @param room_alias the alias name to remove
     */
    public abstract void
    delete_room_alias([CCode (delegate_target_pos = 1.5, scope = "async")]
                      owned Matrix.API.Callback? @callback,
                      string room_alias)
        throws Matrix.Error;

    /**
     * Get the room ID corresponding to this room alias.
     *
     * If @param room_alias is {{{null}}}, this function returns
     * immediately, throws Matrix.Error.INCOMPLETE.
     *
     * @param callback the function to call when the request is
     *                 finished
     * @param room_alias the room alias
     */
    public abstract void
    get_room_id([CCode (delegate_target_pos = 1.5, scope = "async")]
                owned Matrix.API.Callback? @callback,
                string room_alias)
        throws Matrix.Error;

    /**
     * Create a new mapping from room alias to room ID.
     *
     * If @param room_alias or @param room_id is {{{null}}}, this
     * function returns immediately, and throws
     * Matrix.Error.INCOMPLETE.
     *
     * @param callback the function to call when the request is
     *                 finished
     * @param room_id the room ID to add this alias to
     * @param room_alias the room alias to set
     */
    public abstract void
    create_room_alias([CCode (delegate_target_pos = 1.5, scope = "async")]
                      owned Matrix.API.Callback? @callback,
                      string room_id,
                      string room_alias)
        throws Matrix.Error;

    /* Room discovery */

    /**
     * List the public rooms on the server.
     *
     * @param callback the function to call when the request is
     *                 finished
     */
    public abstract void
    list_public_rooms([CCode (scope = "async")]
                      owned Matrix.API.Callback? @callback)
        throws Matrix.Error;

    /* Room membership */

    /**
     * Ban the specified user from the specified room. An optional reason
     * can be specified.
     *
     * If @param room_id or @param user_id is {{{null}}}, this
     * function returns immediately, and throws
     * Matrix.Error.INCOMPLETE.
     *
     * @param callback the function to call when the request is
     *                 finished
     * @param room_id the room ID where the user should be banned
     * @param user_id the user ID to ban
     * @param reason the reason of the ban
     */
    public abstract void
    ban_user([CCode (delegate_target_pos = 1.5, scope = "async")]
             owned Matrix.API.Callback? @callback,
             string room_id,
             string user_id, string? reason)
        throws Matrix.Error;

    /**
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
     * If @param room_id is {{{null}}}, this function returns
     * immediately, and throws Matrix.Error.INCOMPLETE.
     *
     * @param callback the function to call when the request is
     *                 finished
     * @param room_id the room ID to forget
     */
    public abstract void
    forget_room([CCode (delegate_target_pos = 1.5, scope = "async")]
                owned Matrix.API.Callback? @callback,
                string room_id)
        throws Matrix.Error;

    /**
     * Invite a user to the room by a 3rd party identifier. They do not
     * start participating in the room until they actually join the room.
     *
     * If the identity server does not know a Matrix user identifier for
     * the passed third party identifier, the homeserver will issue an
     * invitation which can be accepted upon providing proof of ownership
     * of the third party identifier.
     *
     * If @param credential is {{{null}}}, this function immediately
     * returns, and throws Matrix.Error.INCOMPLETE.
     *
     * @param callback the function to call when the request is
     *                 finished
     * @param room_id the room ID to which to invite the user
     * @param credential a {@link Matrix.3PidCredential} that
     *                   identifies a user to invite
     */
    public abstract void
    invite_user_3rdparty([CCode (delegate_target_pos = 1.5, scope = "async")]
                         owned Matrix.API.Callback? @callback,
                         string room_id,
                         Matrix.3PidCredential credential)
        throws Matrix.Error;

    /**
     * Invite a user to a room.
     *
     * If @room_id or @user_id is %NULL, this function returns
     * immediately, and fills @error with %MATRIX_ERROR_INCOMPLETE.
     *
     * @param callback the function to call when the request is
     *                 finished
     * @param room_id the room ID to invite the user to
     * @param user_id the user ID to invite
     */
    public abstract void
    invite_user([CCode (delegate_target_pos = 1.5, scope = "async")]
                owned Matrix.API.Callback? @callback,
                string room_id,
                string user_id)
        throws Matrix.Error;

    /**
     * Join a room.
     *
     * If @param room_id is {{{null}}}, this function returns
     * immediately, and throws Matrix.Error.INCOMPLETE.
     *
     * @param callback the function to call when the request is
     *                 finished
     * @param room_id the room ID to join to
     */
    public abstract void
    join_room([CCode (delegate_target_pos = 1.5, scope = "async")]
              owned Matrix.API.Callback? @callback,
              string room_id)
        throws Matrix.Error;

    /**
     * Leave a room.
     *
     * If @param room_id is {{{null}}}, this function returns
     * immediately, and throws Matrix.Error.INCOMPLETE.
     *
     * @param callback the function to call when the request is
     *                 finished
     * @param room_id the room ID to kick the user from
     */
    public abstract void
    leave_room([CCode (delegate_target_pos = 1.5, scope = "async")]
               owned Matrix.API.Callback? @callback,
               string room_id)
        throws Matrix.Error;

    /* Room participation */

    /**
     * Get the event stream, optionally beginning from @from_token.
     *
     * @param callback the function to call when the request is
     *                 finished
     * @param from_token events will be listed from this token
     * @param timeout timeout of the request
     */
    public abstract void
    event_stream([CCode (delegate_target_pos = 1.5, scope = "async")]
                 owned Matrix.API.Callback? @callback,
                 string? from_token,
                 ulong timeout)
        throws Matrix.Error;

    /**
     * Get a single event by event ID.
     *
     * If @param event_id is {{{null}}}, this function returns
     * immediately, and throws Matrix.Error.INCOMPLETE.
     *
     * @param callback the function to call when the request is
     *                 finished
     * @param event_id the event ID to get
     */
    public abstract void
    get_event([CCode (delegate_target_pos = 1.5, scope = "async")]
              owned Matrix.API.Callback? @callback,
              string event_id)
        throws Matrix.Error;

    /**
     * Perform an initial sync of events
     *
     * @param callback the function to call when the request is
     *                 finished
     * @param limit the maximum number of events to get
     * @param archived whether to include rooms that the user has left
     */
    public abstract void
    initial_sync([CCode (delegate_target_pos = 1.5, scope = "async")]
                 owned Matrix.API.Callback? @callback,
                 uint limit,
                 bool archived)
        throws Matrix.Error;

    /**
     * Gets a number of events that happened just before and after the
     * specified event.
     *
     * If @param room_id or @param event_id is {{{null}}}, this
     * function returns immediately, and throws
     * Matrix.Error.INCOMPLETE.
     *
     * @param callback the function to call when the request is
     *                 finished
     * @param room_id the room to get events from
     * @param event_id the event to get context around
     * @param limit the maximum number of events to get. If 0, a
     *              default value is used (10, according to the
     *              specification)
     */
    public abstract void
    get_event_context([CCode (delegate_target_pos = 1.5, scope = "async")]
                      owned Matrix.API.Callback? callback,
                      string room_id,
                      string event_id,
                      uint limit)
        throws Matrix.Error;

    /**
     * Get a copy of the current state and the most recent messages in a
     * room.
     *
     * If @param room_id is {{{null}}}, this function returns
     * immediately, and throws Matrix.Error.INCOMPLETE.
     *
     * @param callback the function to call when the request is
     *                 finished
     * @param room_id the room to get the data for
     */
    public abstract void
    initial_sync_room([CCode (delegate_target_pos = 1.5, scope = "async")]
                      owned Matrix.API.Callback? @callback,
                      string room_id)
        throws Matrix.Error;

    /**
     * Get the list of members for a room.
     *
     * If @param room_id is {{{null}}}, this function returns
     * immediately, and throws Matrix.Error.INCOMPLETE.
     *
     * @param callback the function to call when the request is
     *                 finished
     * @param room_id the room to get the member events for
     */
    public abstract void
    list_room_members([CCode (delegate_target_pos = 1.5, scope = "async")]
                      owned Matrix.API.Callback? @callback,
                      string room_id)
        throws Matrix.Error;

    /**
     * Get a list of message and state events for a room.
     *
     * If @param room_id or @param from_token is {{{null}}}, this
     * function returns immediately, and throws
     * Matrix.Error.INCOMPLETE.
     *
     * @param callback the function to call when the request is
     *                 finished
     * @param room_id the room to get the events for
     * @param from_token the token to start returning events
     *                   from. This token can be obtained by calling
     *                   matrix_api_initial_sync() or
     *                   matrix_api_initial_sync_room()
     * @param direction the direction of the returned events
     * @param limit the maximum number of events to return. If 0, a
     *              default value will be used (10, according to the
     *              specification
     */
    public abstract void
    list_room_messages([CCode (delegate_target_pos = 1.5, scope = "async")]
                       owned Matrix.API.Callback? @callback,
                       string room_id,
                       string from_token,
                       Matrix.EventDirection direction,
                       uint limit)
        throws Matrix.Error;

    /**
     * Update the marker for the given receipt type to the event ID
     * specified.
     *
     * If @param room_id, @param event_id or @param receipt is
     * {{{null}}}, this function returns immediately, and throws
     * Matrix.Error.INCOMPLETE.
     *
     * @param callback the function to call when the request is
     *                 finished
     * @param room_id the room in which to send the event
     * @param receipt_type type of the receipt
     * @param event_id the event ID to acknowledge up to
     * @param receipt extra receipt information to attach. Note that
     *                the server will automatically attach the
     *                {{{ts}}} field
     */
    public abstract void
    send_event_receipt([CCode (delegate_target_pos = 1.5, scope = "async")]
                       owned Matrix.API.Callback? @callback,
                       string room_id,
                       Matrix.ReceiptType receipt_type,
                       string event_id,
                       Json.Node receipt)
        throws Matrix.Error;

    /**
     * Strip all information out of an event which isn't critical to
     * the integrity of the server-side representation of the
     * room. This cannot be undone.
     *
     * Users may redact their own events, and any user with a power
     * level greater than or equal to {{{redact}}} power level of the
     * room may redact events there.
     *
     * If @param room_id, @param event_id or @param txn_id is
     * {{{null}}}, this function returns immediately, and throws
     * Matrix.Error.INCOMPLETE.
     *
     * @param callback the function to call when the request is
     *                 finished
     * @param room_id the room from which to redact the event
     * @param event_id the event ID to acknowledge up to
     * @param txn_id the transaction ID for this event. Clients should
     *               generate a unique ID; it will be used by the
     *               server to ensure idempotency of requests
     * @param reason the reason for the event being redacted
     */
    public abstract void
    redact_event([CCode (delegate_target_pos = 1.5, scope = "async")]
                 owned Matrix.API.Callback? @callback,
                 string room_id,
                 string event_id,
                 string txn_id,
                 string? reason)
        throws Matrix.Error;

    /**
     * Send a message event to the room.
     *
     * If @param room_id, @param event_type, @param txn_id or @content
     * is {{{null}}}, this function returns immediately, and
     * throws Matrix.Error.INCOMPLETE.
     *
     * @param callback the function to call when the request is
     *                 finished
     * @param room_id the room to send the event to
     * @param event_type the type of event to send
     * @param txn_id the transaction ID for this event. Clients should
     *               generate a unique ID; it will be used by the
     *               server to ensure idempotency of requests
     * @param content the content of the event as a {@link Json.Node}
     */
    public abstract void
    send_message_event([CCode (delegate_target_pos = 1.5, scope = "async")]
                       owned Matrix.API.Callback? @callback,
                       string room_id,
                       string event_type,
                       string txn_id,
                       owned Json.Node content)
        throws Matrix.Error;

    /**
     * Look up the contents of a state event in a room. If
     * both @param event_type and @param state_key are empty, get a
     * list of state events for that room.
     *
     * If @param room_id is {{{null}}}, or if @param state_key is set
     * with @param event_type being {{{null}}}, this function returns
     * immediately, and throws Matrix.Error.INCOMPLETE.
     *
     * @param callback the function to call when the request is
     *                 finished
     * @param room_id the room ID to get a state for
     * @param event_type the type of state to look up
     * @param state_key the key of the state to look up.
     *                  If @param event_type is {{{null}}}, this
     *                  parameter is ignored
     */
    public abstract void
    get_room_state([CCode (delegate_target_pos = 1.5, scope = "async")]
                   owned Matrix.API.Callback? @callback,
                   string room_id,
                   string? event_type,
                   string? state_key)
        throws Matrix.Error;

    /**
     * Send a state event to the room. These events will be overwritten
     * if @param room_id, @param event_type and @param state_key all
     * match.
     *
     * This request cannot use transaction IDs.
     *
     * The required fields in the body of the request (@param content)
     * vary depending on the type of the event.
     *
     * If @param room_id or @param content is {{{null}}}, or if @param
     * state_key is set with @param event_type being {{{null}}}, this
     * function returns immediately, and throw
     * Matrix.Error.INCOMPLETE.
     *
     * @param callback the function to call when the request is
     *                 finished
     * @param room_id the room ID to get a state for
     * @param event_type the type of state to look up
     * @param state_key the key of the state to look up.
     *                  If @param event_type is {{{null}}}, this
     *                  parameter is ignored
     * @param content the content of the state event
     */
    public abstract void
    send_room_event([CCode (delegate_target_pos = 1.5, scope = "async")]
                    owned Matrix.API.Callback? @callback,
                    string room_id,
                    string event_type,
                    string? state_key,
                    owned Json.Node content)
        throws Matrix.Error;

    /**
     * Tell the server the user is typing for the next @param timeout
     * milliseconds. If @param typing is {{{false}}}, it tells the
     * server that the user stopped typing.
     *
     * If @param user_id or @param room_id is {{{null}}}, this
     * function returns immediately, and throws
     * Matrix.Error.INCOMPLETE.
     *
     * @param callback the function to call when the request is
     *                 finished
     * @param user_id the user who has started to type
     * @param room_id the room in which the user is typing
     * @param timeout the length of time in milliseconds to mark this
     *                user as typing
     * @param typing whether the user is typing or not. If
     *               {{{false}}}, @param timeout can be omitted
     *               (ie. set to 0)
     */
    public abstract void
    notify_room_typing([CCode (delegate_target_pos = 1.5, scope = "async")]
                       owned Matrix.API.Callback? @callback,
                       string user_id,
                       string room_id,
                       uint timeout,
                       bool typing)
        throws Matrix.Error;

    /**
     * Synchronize the client's state with the latest state on the
     * server. Clients should use this API when they first log in to
     * get an initial snapshot of the state on the server and then
     * continue to call this API to get incremental details to the
     * state and to receive new messages.
     *
     * Only one of @param filter and @param filter_id should be
     * specified, or both of them should be set to {{{null}}} to
     * receive all events.
     *
     * @param callback the function to call when the request is
     *                 finished
     * @param filter_id a filter ID created by the filter API
     *                  (e.g. matrix_api_create_filter())
     * @param filter a definition on what events to fetch
     * @param since a point in time to continue a sync from
     * @param full_state if {{{true}}}, all state events will be
     *                   returned, even if @param since is not
     *                   empty. If {{{false}}}, and @param since is
     *                   not empty, only states which have changed
     *                   since the point indicated by @param since
     *                   will be returned
     * @param set_presence controls whether the client is
     *                     automatically marked as online by polling
     *                     this API.
     * @param timeout the maximum time to poll in milliseconds
     */
    public abstract void
    sync([CCode (delegate_target_pos = 1.5, scope = "async")]
         owned Matrix.API.Callback? @callback,
         string? filter_id,
         Matrix.Filter? filter,
         string? since,
         bool full_state,
         bool set_presence,
         ulong timeout)
        throws Matrix.Error;

    /**
     * Upload a new filter definition to the homeserver. It will return a
     * filter ID that may be used in future requests.
     *
     * If @param user_id or @param filter is {{{null}}}, this function
     * returns immediately, and throws
     * Matrix.Error.INCOMPLETE.
     *
     * @param callback the function to call when the request is
     *                 finished
     * @param user_id the ID of the user uploading the filter. An
     *                access token must be present (either specifying
     *                one with matrix_api_set_token() or requested
     *                from the server via
     *                matrix_api_register_account() or
     *                matrix_api_login()).
     * @param filter the filter to upload
     */
    public abstract void
    create_filter([CCode (delegate_target_pos = 1.5, scope = "async")]
                  owned Matrix.API.Callback? @callback,
                  string user_id,
                  Matrix.Filter filter)
        throws Matrix.Error;

    /**
     * Download a filter.
     *
     * If @param user_id or @param filter_id is {{{null}}}, this
     * function returns immediately, and throws
     * Matrix.Error.INCOMPLETE.
     *
     * @param callback the function to call when the request is
     *                 finished
     * @param user_id the user ID to download a filter from
     * @param filter_id the filter ID to download
     */
    public abstract void
    download_filter([CCode (delegate_target_pos = 1.5, scope = "async")]
                    owned Matrix.API.Callback? @callback,
                    string user_id,
                    string filter_id)
        throws Matrix.Error;

    /* Search */

    /* TODO: implement search! */

    /* Server administration */

    /**
     * Get information about a particular user.
     *
     * If @param user_id is {{{null}}}, this function returns
     * immediately, and throws Matrix.Error.INCOMPLETE.
     *
     * @param callback the function to call when the request is
     *                 finished
     * @param user_id the user ID to look up
     */
    public abstract void
    whois([CCode (delegate_target_pos = 1.5, scope = "async")]
          owned Matrix.API.Callback? @callback,
          string user_id)
        throws Matrix.Error;

    /**
     * Get the versions of the specification supported by the server.
     *
     * @param callback the function to call when the request is
     *                 finished
     */
    public abstract void
    versions([CCode (scope = "async")]
             owned Matrix.API.Callback? @callback)
        throws Matrix.Error;

    /* Session management */

    /**
     * Attempt to login with type @param type. Implementations of this
     * method must set the token property on a successful login.
     *
     * If @param login_type or @param content is {{{null}}}, this
     * function returns immediately, and throws
     * Matrix.Error.INCOMPLETE.
     *
     * @param callback the function to call when the request is
     *                 finished
     * @param login_type the login type to use
     * @param content parameters to pass for the login request
     */
    public abstract void
    login([CCode (delegate_target_pos = 1.5, scope = "async")]
          owned Matrix.API.Callback? @callback,
          string login_type,
          Json.Node? content)
        throws Matrix.Error;

    /**
     * Exchanges a refresh token for a new access token. This is
     * intended to be used if the access token has expired. If @param
     * refresh_token is {{{null}}}, implementations MUST send the
     * stored refresh token. If it is not pesent (e.g. because login
     * hasn’t happened yet), this function MUST throw an error.
     *
     * @param callback the function to call when the request is
     *                 finished
     * @param refresh_token the refresh token that was issued by the
     *                      server
     */
    public abstract void
    token_refresh([CCode (delegate_target_pos = 1.5, scope = "async")]
                  owned Matrix.API.Callback? @callback,
                  string? refresh_token)
        throws Matrix.Error;

    /* User data */

    /**
     * Get a list of the third party identifiers that a homeserver has
     * associated with the user's account.
     *
     * This is not the same as the list of third party identifiers bound
     * to the user's Matrix ID in Identity Servers.
     *
     * Identifiers in this list may be used by the homeserver as, for
     * example, identifiers to accept to reset the user's account
     * password.
     *
     * @param callback the function to call when the request is
     *                 finished
     */
    public abstract void
    get_3pids([CCode (scope = "async")]
              owned Matrix.API.Callback? @callback)
        throws Matrix.Error;

    /**
     * Add contact information to the user's account.
     *
     * If @param threepid_creds is {{{null}}}, this function returns
     * immediately, and throws Matrix.Error.INCOMPLETE.
     *
     * @param callback the function to call when the request is
     *                 finished
     * @param bind_creds whether the homeserver should also bind this
     *                   third party identifier to the account's
     *                   Matrix ID with the passed Identity Server.
     * @param threepid_creds the credentials to associate with the
     *                       account
     */
    public abstract void
    add_3pid([CCode (delegate_target_pos = 1.5, scope = "async")]
             owned Matrix.API.Callback? @callback,
             bool bind_creds,
             Matrix.3PidCredential threepid_creds)
        throws Matrix.Error;

    /**
     * Change the active user's password.
     *
     * If @param new_password is {{{null}}}, this function returns
     * immediately, and throws Matrix.Error.INCOMPLETE.
     *
     * @param callback the function to call when the request is
     *                 finished
     * @param new_password the new password for the account
     */
    public abstract void
    change_password([CCode (delegate_target_pos = 1.5, scope = "async")]
                    owned Matrix.API.Callback? @callback,
                    string new_password)
        throws Matrix.Error;

    /**
     * Get a user's profile.
     *
     * If @param user_id is {{{null}}}, this function returns
     * immediately, and throws Matrix.Error.INCOMPLETE.
     *
     * @param callback the function to call when the request is
     *                 finished
     * @param user_id the user whose profile to get
     */
    public abstract void
    get_profile([CCode (delegate_target_pos = 1.5, scope = "async")]
                owned Matrix.API.Callback? @callback,
                string user_id)
        throws Matrix.Error;

    /**
     * Get the URL of the specified user's avatar.
     *
     * If @param user_id is {{{null}}}, this function returns
     * immediately, and throws Matrix.Error.INCOMPLETE.
     *
     * @param callback the function to call when the request is
     *                 finished
     * @param user_id the user whose avatar URL to get
     */
    public abstract void
    get_avatar_url([CCode (delegate_target_pos = 1.5, scope = "async")]
                   owned Matrix.API.Callback? @callback,
                   string user_id)
        throws Matrix.Error;

    /**
     * Set the user's avatar URL.
     *
     * If @param user_id or @param avatar_url is {{{null}}}, this
     * function returns immediately, and throws
     * Matrix.Error.INCOMPLETE.
     *
     * @param callback the function to call when the request is
     *                 finished
     * @param user_id the user whose avatar URL to set
     * @param avatar_url the avatar URL info
     */
    public abstract void
    set_avatar_url([CCode (delegate_target_pos = 1.5, scope = "async")]
                   owned Matrix.API.Callback? @callback,
                   string user_id,
                   string avatar_url)
        throws Matrix.Error;

    /**
     * Get the user's display name.
     *
     * If @param user_id is {{{null}}}, this function returns
     * immediately, and throws Matrix.Error.INCOMPLETE.
     *
     * @param callback the function to call when the request is
     *                 finished
     * @param user_id the user whose display name to get
     */
    public abstract void
    get_display_name([CCode (delegate_target_pos = 1.5, scope = "async")]
                     owned Matrix.API.Callback? @callback,
                     string user_id)
        throws Matrix.Error;

    /**
     * Set the user's display name.
     *
     * If @param user_id or @param display_name is {{{null}}}, this
     * function returns immediately, and throws
     * Matrix.Error.INCOMPLETE.
     *
     * @param callback the function to call when the request is
     *                 finished
     * @param user_id the user whose display name to set
     * @param display_name the display name info
     */
    public abstract void
    set_display_name([CCode (delegate_target_pos = 1.5, scope = "async")]
                     owned Matrix.API.Callback? @callback,
                     string user_id,
                     string display_name)
        throws Matrix.Error;

    /**
     * Attempt to register an account with the homeserver using @param
     * username and @param password.
     *
     * Implementations of this method must set the token property on a
     * successful login.
     *
     * If @param password is {{{null}}}, this function returns
     * immediately, and throws Matrix.Error.INCOMPLETE.
     *
     * @param callback the function to call when the request is
     *                 finished
     * @param account_kind the type of account to register
     * @param bind_email if {{{true}}}, the server binds the e-mail
     *                   used for authentication to the Matrix ID with
     *                   the ID server
     * @param username the local part of the desired Matrix ID. If
     *                 omitted, the server will generate a local part
     * @param password the desired password for the account
     */
    public abstract void
    register_account([CCode (delegate_target_pos = 1.5, scope = "async")]
                     owned Matrix.API.Callback? @callback,
                     Matrix.AccountKind account_kind,
                     bool bind_email,
                     string? username,
                     string password)
        throws Matrix.Error;

    /**
     * Set some account data for the client. This config is only
     * visible to the user who set the account data. The config will
     * be synced to clients in the top-level account data.
     *
     * If @param user_id, @param event_type or @param content is
     * {{{null}}}, this function returns immediately, and throws
     * Matrix.Error.INCOMPLETE.
     *
     * @param callback the function to call when the request is
     *                 finished
     * @param user_id the user to set account data for. An access
     *                token must be present and be authorized to make
     *                requests for this user ID
     * @param room_id the room to set account data for. If {{{null}}},
     *                the account data will be set globally
     * @param event_type the event type of the account data to
     *                   set. Custom types should be namespaced to
     *                   avoid clashes
     * @param content the content of the account data
     */
    public abstract void
    set_account_data([CCode (delegate_target_pos = 1.5, scope = "async")]
                     owned Matrix.API.Callback? @callback,
                     string user_id,
                     string? room_id,
                     string event_type,
                     owned Json.Node content)
        throws Matrix.Error;

    /**
     * List the tags set by a user on a room.
     *
     * If @param user_id or @param room_id is {{{null}}}, this
     * function returns immediately, and throws
     * Matrix.Error.INCOMPLETE.
     *
     * @param callback the function to call when the request is
     *                 finished
     * @param user_id the ID of the user to get the tags for. An
     *                access token must be set, and it must be
     *                authorised to make requests for this user ID
     * @param room_id the room to get tags for
     */
    public abstract void
    get_room_tags([CCode (delegate_target_pos = 1.5, scope = "async")]
                  owned Matrix.API.Callback? @callback,
                  string user_id,
                  string room_id)
        throws Matrix.Error;

    /**
     * Remove a tag from the room.
     *
     * If @param user_id, @param room_id or @param tag is {{{null}}},
     * this function returns immediately, and throws
     * Matrix.Error.INCOMPLETE.
     *
     * @param callback the function to call when the request is
     *                 finished
     * @param user_id the id of the user to remove a tag for
     * @param room_id the id of the room to remove the tag from
     * @param tag the tag to remove
     */
    public abstract void
    delete_room_tag([CCode (delegate_target_pos = 1.5, scope = "async")]
                    owned Matrix.API.Callback? @callback,
                    string user_id,
                    string room_id,
                    string tag)
        throws Matrix.Error;

    /**
     * Add a tag to the room.
     *
     * If @param user_id, @param room_id or @param tag is {{{null}}},
     * this function returns immediately, and throws
     * Matrix.Error.INCOMPLETE.
     *
     * @param callback the function to call when the request is
     *                 finished
     * @param user_id the ID of the user to add the tag for
     * @param room_id the ID of the room to add the tag for
     * @param tag the tag to add
     * @param content extra data for the tag, e.g. ordering
     */
    public abstract void
    add_room_tag([CCode (delegate_target_pos = 1.5, scope = "async")]
                 owned Matrix.API.Callback? @callback,
                 string user_id,
                 string room_id,
                 string tag,
                 owned Json.Node? content)
        throws Matrix.Error;

    /* VoIP */

    /**
     * Get credentials for the client to use when initiating calls.
     *
     * @param callback the function to call when the request is
     *                 finished
     */
    public abstract void
    get_turn_server([CCode (scope = "async")]
                    owned Matrix.API.Callback? @callback)
        throws Matrix.Error;
}
