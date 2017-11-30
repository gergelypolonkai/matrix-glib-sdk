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

#include "matrix-http-client.h"
#include "matrix-client.h"
#include "matrix-event-room-base.h"
#include "matrix-event-presence.h"
#include "matrix-event-room-member.h"
#include "matrix-event-room-aliases.h"
#include "matrix-event-room-avatar.h"
#include "matrix-event-room-canonical-alias.h"
#include "matrix-event-room-create.h"
#include "matrix-event-room-guest-access.h"
#include "matrix-event-room-history-visibility.h"
#include "matrix-event-room-join-rules.h"
#include "matrix-event-room-name.h"
#include "matrix-event-room-power-levels.h"
#include "matrix-event-room-topic.h"

/**
 * SECTION:matrix-http-client
 * @short_description: event-driven communication with Matrix.org homeserver via HTTP.
 *
 * An event-driven client class to communicate with HTTP based Matrix.org servers.
 */
static void matrix_http_client_matrix_client_interface_init (MatrixClientInterface * iface);

typedef struct {
    gboolean _polling;
    gulong _event_timeout;
    gchar* _last_sync_token;
    GHashTable* _user_global_profiles;
    GHashTable* _user_global_presence;
    GHashTable* _rooms;
    gulong _last_txn_id;
} MatrixHTTPClientPrivate;

G_DEFINE_TYPE_EXTENDED(MatrixHTTPClient, matrix_http_client, MATRIX_TYPE_HTTP_API, 0, G_ADD_PRIVATE(MatrixHTTPClient) G_IMPLEMENT_INTERFACE(MATRIX_TYPE_CLIENT, matrix_http_client_matrix_client_interface_init));

MatrixHTTPClient *
matrix_http_client_new(const gchar* base_url)
{
    g_return_val_if_fail(base_url != NULL, NULL);

    return (MatrixHTTPClient *)g_object_new(MATRIX_TYPE_HTTP_CLIENT,
                                            "base-url", base_url,
                                            NULL);
}

static void
login_callback(MatrixAPI *matrix_api, const gchar *content_type, JsonNode *json_content, GByteArray *raw_content, GError *err, gpointer user_data)
{
    g_signal_emit_by_name((MatrixClient *)matrix_api,
                          "login-finished",
                          g_error_matches(err, MATRIX_ERROR, MATRIX_ERROR_NONE));
}

static void
matrix_http_client_real_login_with_password(MatrixClient *matrix_client, const gchar *username, const gchar *password, GError **error)
{
    JsonBuilder *builder;
    JsonNode *root_node;

    g_return_if_fail (username != NULL);
    g_return_if_fail (password != NULL);

    builder = json_builder_new();

    json_builder_begin_object(builder);

    json_builder_set_member_name(builder, "user");
    json_builder_add_string_value(builder, username);

    json_builder_set_member_name(builder, "password");
    json_builder_add_string_value(builder, password);

    json_builder_end_object(builder);
    root_node = json_builder_get_root(builder);

    matrix_api_login(MATRIX_API(matrix_client), "m.login.password", root_node, login_callback, NULL, error);

    json_node_unref(root_node);
    g_object_unref(builder);
}

static void
matrix_http_client_real_register_with_password(MatrixClient *matrix_client, const gchar *username, const gchar *password, GError **error)
{
    matrix_api_register_account(MATRIX_API(matrix_client),
                                MATRIX_ACCOUNT_KIND_USER, FALSE, username, password,
                                login_callback, NULL,
                                error);
}

static void
logout_callback(MatrixAPI *matrix_api, const gchar *content_type, JsonNode *json_content, GByteArray *raw_content, GError *err, gpointer user_data)
{
    matrix_api_abort_pending(matrix_api);
    matrix_api_set_token(matrix_api, NULL);
    matrix_api_set_refresh_token(matrix_api, NULL);
}

static void
matrix_http_client_real_logout(MatrixClient *matrix_client, GError **error)
{
    matrix_api_logout(MATRIX_API(matrix_client), logout_callback, NULL, error);
}

static MatrixRoom *
_get_or_create_room(MatrixHTTPClient *matrix_http_client, const gchar *room_id)
{
    MatrixRoom *room = NULL;
    MatrixHTTPClientPrivate *priv;

    g_return_val_if_fail(matrix_http_client != NULL, NULL);
    g_return_val_if_fail(room_id != NULL, NULL);

    priv = matrix_http_client_get_instance_private(matrix_http_client);

    if ((room = g_hash_table_lookup(priv->_rooms, room_id)) == NULL) {
        room = matrix_room_new(room_id);
        g_hash_table_insert(priv->_rooms, g_strdup(room_id), room);
    }

    return room;
}

static void
_process_event(MatrixHTTPClient *matrix_http_client, JsonNode *event_node, const gchar *room_id)
{
    MatrixHTTPClientPrivate *priv;
    JsonObject *root;
    JsonNode *node;
    const gchar *event_type;
    MatrixEventBase *evt = NULL;
    GError *inner_error = NULL;

    g_return_if_fail(matrix_http_client != NULL);
    g_return_if_fail(event_node != NULL);

    if (json_node_get_node_type(event_node) != JSON_NODE_OBJECT) {
#if DEBUG
        g_warning("Received event that is not an object.");
#endif

        return;
    }

    root = json_node_get_object(event_node);

    if ((node = json_object_get_member(root, "type")) == NULL) {
#if DEBUG
        g_warning("Received event without type.");
#endif

        return;
    }

    priv = matrix_http_client_get_instance_private(matrix_http_client);
    event_type = json_node_get_string(node);
    evt = matrix_event_base_new_from_json(event_type, event_node, &inner_error);

    if (inner_error != NULL) {
        evt = NULL;
        g_clear_error(&inner_error);
    }

    if (evt != NULL) {
        if (MATRIX_EVENT_IS_ROOM(evt)) {
            MatrixEventRoom *revt = MATRIX_EVENT_ROOM(evt);

            // Make sure Room events have room_id set, even if it was stripped by the HS
            if (matrix_event_room_get_room_id(revt) == NULL) {
                matrix_event_room_set_room_id(revt, room_id);
            }
        }

        else if (MATRIX_EVENT_IS_PRESENCE(evt)) {
            MatrixEventPresence *pevt = MATRIX_EVENT_PRESENCE(evt);
            const gchar *user_id = matrix_event_presence_get_user_id(pevt);
            MatrixProfile *profile;

            g_hash_table_replace(priv->_user_global_presence, g_strdup(user_id), GINT_TO_POINTER(matrix_event_presence_get_presence(pevt)));

            profile = g_hash_table_lookup(priv->_user_global_profiles, user_id);

            if (profile == NULL) {
                profile = matrix_profile_new();
                g_hash_table_insert(priv->_user_global_profiles, g_strdup(user_id), profile);
            }

            matrix_profile_set_avatar_url(profile, matrix_event_presence_get_avatar_url(pevt));
            matrix_profile_set_display_name(profile, matrix_event_presence_get_display_name(pevt));
        } else if (MATRIX_EVENT_IS_ROOM(evt)) {
            MatrixEventRoom *revt = MATRIX_EVENT_ROOM(evt);
            MatrixRoom *room = _get_or_create_room(matrix_http_client, matrix_event_room_get_room_id(revt));

            if (MATRIX_EVENT_IS_ROOM_MEMBER(evt)) {
                MatrixEventRoomMember *mevt = MATRIX_EVENT_ROOM_MEMBER(evt);
                const gchar *user_id = matrix_event_room_member_get_user_id(mevt);
                MatrixProfile *profile = matrix_room_get_or_add_member(room, user_id, matrix_event_room_member_get_tpi_display_name(mevt) != NULL, NULL);

                matrix_profile_set_avatar_url(profile, matrix_event_room_member_get_avatar_url(mevt));
                matrix_profile_set_display_name(profile, matrix_event_room_member_get_display_name(mevt));
            } else if (MATRIX_EVENT_IS_ROOM_ALIASES(evt)) {
                gint n_aliases;
                const gchar **aliases;
                MatrixEventRoomAliases *aevt = MATRIX_EVENT_ROOM_ALIASES(evt);

                aliases = matrix_event_room_aliases_get_aliases(aevt, &n_aliases);
                matrix_room_set_aliases(room, aliases, n_aliases);
            } else if (MATRIX_EVENT_IS_ROOM_AVATAR(evt)) {
                MatrixEventRoomAvatar *aevt = MATRIX_EVENT_ROOM_AVATAR(evt);

                matrix_room_set_avatar_url(room, matrix_event_room_avatar_get_url(aevt));
                matrix_room_set_avatar_info(room, matrix_event_room_avatar_get_info(aevt));
                matrix_room_set_avatar_thumbnail_url(room, matrix_event_room_avatar_get_thumbnail_url(aevt));
                matrix_room_set_avatar_thumbnail_info(room, matrix_event_room_avatar_get_thumbnail_info(aevt));
            } else if (MATRIX_EVENT_IS_ROOM_CANONICAL_ALIAS(evt)) {
                MatrixEventRoomCanonicalAlias *cevt = MATRIX_EVENT_ROOM_CANONICAL_ALIAS(evt);

                matrix_room_set_canonical_alias(room, matrix_event_room_canonical_alias_get_canonical_alias(cevt));
            } else if (MATRIX_EVENT_IS_ROOM_CREATE(evt)) {
                MatrixEventRoomCreate *cevt = MATRIX_EVENT_ROOM_CREATE(evt);

                matrix_room_set_creator(room, matrix_event_room_create_get_creator(cevt));
                matrix_room_set_federate(room, matrix_event_room_create_get_federate(cevt));
            } else if (MATRIX_EVENT_IS_ROOM_GUEST_ACCESS(evt)) {
                MatrixEventRoomGuestAccess *gevt = MATRIX_EVENT_ROOM_GUEST_ACCESS(evt);

                matrix_room_set_guest_access(room, matrix_event_room_guest_access_get_guest_access(gevt));
            } else if (MATRIX_EVENT_IS_ROOM_HISTORY_VISIBILITY(evt)) {
                MatrixEventRoomHistoryVisibility *hevt = MATRIX_EVENT_ROOM_HISTORY_VISIBILITY(evt);

                matrix_room_set_history_visibility(room, matrix_event_room_history_visibility_get_visibility(hevt));
            } else if (MATRIX_EVENT_IS_ROOM_JOIN_RULES(evt)) {
                MatrixEventRoomJoinRules *jevt = MATRIX_EVENT_ROOM_JOIN_RULES(evt);

                matrix_room_set_join_rules(room, matrix_event_room_join_rules_get_join_rules(jevt));
            } else if (MATRIX_EVENT_ROOM_NAME(evt)) {
                MatrixEventRoomName *nevt = MATRIX_EVENT_ROOM_NAME(evt);

                matrix_room_set_name(room, matrix_event_room_name_get_name(nevt));
            } else if (MATRIX_EVENT_ROOM_POWER_LEVELS(evt)) {
                MatrixEventRoomPowerLevels *levt = MATRIX_EVENT_ROOM_POWER_LEVELS(evt);
                GHashTable *user_levels;
                GHashTable *event_levels;
                GHashTableIter iter;
                gchar *key;
                gpointer level;

                matrix_room_set_default_power_level(room, matrix_event_room_power_levels_get_users_default(levt));
                matrix_room_set_default_event_level(room, matrix_event_room_power_levels_get_events_default(levt));
                matrix_room_set_default_state_level(room, matrix_event_room_power_levels_get_state_default(levt));
                matrix_room_set_ban_level(room, matrix_event_room_power_levels_get_ban(levt));
                matrix_room_set_kick_level(room, matrix_event_room_power_levels_get_kick(levt));
                matrix_room_set_redact_level(room, matrix_event_room_power_levels_get_redact(levt));
                matrix_room_set_invite_level(room, matrix_event_room_power_levels_get_invite(levt));
                matrix_room_clear_user_levels(room);
                matrix_room_clear_event_levels(room);

                user_levels = matrix_event_room_power_levels_get_user_levels(levt);
                g_hash_table_iter_init(&iter, user_levels);

                while (g_hash_table_iter_next(&iter, (gpointer *)&key, &level)) {
                    matrix_room_set_user_level(room, key, GPOINTER_TO_INT(level));
                }

                event_levels = matrix_event_room_power_levels_get_event_levels(levt);
                g_hash_table_iter_init(&iter, event_levels);

                while (g_hash_table_iter_next(&iter, (gpointer *)&key, &level)) {
                    matrix_room_set_event_level(room, key, GPOINTER_TO_INT(level));
                }
            } else if (MATRIX_EVENT_IS_ROOM_TOPIC(evt)) {
                MatrixEventRoomTopic *tevt = MATRIX_EVENT_ROOM_TOPIC(evt);

                matrix_room_set_topic(room, matrix_event_room_topic_get_topic(tevt));
            }
        }
    }

    matrix_client_incoming_event(MATRIX_CLIENT(matrix_http_client), room_id, event_node, evt);
    g_object_unref(evt);
    json_node_unref(node);
    json_object_unref(root);
}

static void
_process_event_list_obj(MatrixHTTPClient *matrix_http_client, JsonNode* node, const gchar* room_id)
{
    JsonObject *root;
    JsonNode *events_node;

    root = json_node_get_object(node);

    if ((events_node = json_object_get_member(root, "events")) != NULL) {
        if (json_node_get_node_type(events_node) == JSON_NODE_ARRAY) {
            JsonArray *events_array = json_node_get_array(events_node);
            gint len = json_array_get_length(events_array);

            for (gint idx = 0; idx < len; idx++) {
                JsonNode *event_node = json_array_get_element(events_array, idx);

                _process_event(matrix_http_client, event_node, room_id);
            }
        }
    }
}

static void
refresh_callback(MatrixAPI *matrix_api, const gchar *content_type, JsonNode *json_content, GByteArray *raw_content, GError *err, gpointer user_data)
{
    g_signal_emit_by_name(MATRIX_CLIENT(matrix_api), "login-finished", g_error_matches(err, MATRIX_ERROR, MATRIX_ERROR_NONE));

    if (matrix_api_get_token(matrix_api) == NULL) {
        matrix_api_set_refresh_token(matrix_api, NULL);
        g_signal_emit_by_name(MATRIX_CLIENT(matrix_api), "polling-stopped", err);

        matrix_client_stop_polling(MATRIX_CLIENT(matrix_api), FALSE, NULL);
    }
}

static void
cb_sync(MatrixAPI *matrix_api, const gchar *content_type, JsonNode *json_content, GByteArray *raw_content, GError *error, gpointer user_data)
{
    MatrixHTTPClientPrivate *priv = matrix_http_client_get_instance_private(MATRIX_HTTP_CLIENT(matrix_api));

    if (error == NULL) {
        JsonObject *root = json_node_get_object(json_content);
        JsonNode *node;

#if DEBUG
        g_debug("Processing account data");
#endif

        _process_event_list_obj(MATRIX_HTTP_CLIENT(matrix_api), json_object_get_member(root, "account_data"), NULL);

#if DEBUG
        g_debug("Processing presence");
#endif

        _process_event_list_obj(MATRIX_HTTP_CLIENT(matrix_api), json_object_get_member(root, "presence"), NULL);

        if ((node = json_object_get_member(root, "rooms")) != NULL) {
            if (json_node_get_node_type(node) == JSON_NODE_OBJECT) {
                JsonObject *rooms_root = json_node_get_object(node);
                JsonNode *rooms_node;
                        MatrixHTTPClient *matrix_http_client = MATRIX_HTTP_CLIENT(matrix_api);

#if DEBUG
                g_debug("Processing rooms");
#endif

                if ((rooms_node = json_object_get_member(rooms_root, "invite")) != NULL) {
                    JsonObjectIter iter;
                    const gchar *room_id;
                    JsonNode *room_node;

                    json_object_iter_init(&iter, json_node_get_object(rooms_node));

                    while (json_object_iter_next(&iter, &room_id, &room_node)) {
                        JsonObject *room_root;

                        if (json_node_get_node_type(room_node) != JSON_NODE_OBJECT) {
                            continue;
                        }

                        room_root = json_node_get_object(room_node);

                        _process_event_list_obj(MATRIX_HTTP_CLIENT(matrix_api), json_object_get_member(room_root, "invite_state"), room_id);
                    }
                }

                if ((rooms_node = json_object_get_member(rooms_root, "join")) != NULL) {
                    JsonObjectIter iter;
                    const gchar *room_id;
                    JsonNode *room_node;

                    json_object_iter_init(&iter, json_node_get_object(rooms_node));

                    while (json_object_iter_next(&iter, &room_id, &room_node)) {
                        JsonObject *room_root;

                        if (json_node_get_node_type(room_node) != JSON_NODE_OBJECT) {
                            continue;
                        }

                        room_root = json_node_get_object(room_node);

                        _process_event_list_obj(matrix_http_client, json_object_get_member(room_root, "timeline"), room_id);
                        _process_event_list_obj(matrix_http_client, json_object_get_member(room_root, "state"), room_id);
                        _process_event_list_obj(matrix_http_client, json_object_get_member(room_root, "account_data"), room_id);
                        _process_event_list_obj(matrix_http_client, json_object_get_member(room_root, "ephemeral"), room_id);
                    }
                }

                if ((rooms_node = json_object_get_member(rooms_root, "leave")) != NULL) {
                    JsonObjectIter iter;
                    const gchar *room_id;
                    JsonNode *room_node;

                    json_object_iter_init(&iter, json_node_get_object(rooms_node));

                    while (json_object_iter_next(&iter, &room_id, &room_node)) {
                        JsonObject *room_root;

                        if (json_node_get_node_type(room_node) != JSON_NODE_OBJECT) {
                            continue;
                        }

                        room_root = json_node_get_object(room_node);

                        _process_event_list_obj(matrix_http_client, json_object_get_member(room_root, "timeline"), room_id);
                        _process_event_list_obj(matrix_http_client, json_object_get_member(room_root, "state"), room_id);
                    }
                }
            }
        }

        if ((node = json_object_get_member(root, "next_batch")) != NULL) {
            g_free(priv->_last_sync_token);
            priv->_last_sync_token = g_strdup(json_node_get_string(node));
        }
    } else if ((error->domain == MATRIX_ERROR) &&
               ((error->code == MATRIX_ERROR_M_FORBIDDEN) ||
                (error->code == MATRIX_ERROR_M_UNKNOWN_TOKEN) ||
                (error->code == MATRIX_ERROR_M_UNAUTHORIZED))) {
        matrix_api_set_token(matrix_api, NULL);
        matrix_api_token_refresh(matrix_api,
                                 NULL,
                                 refresh_callback, NULL,
                                 NULL);
    }

    // It is possible that polling has been disabled while we were processing events. Don’t
    // continue polling if that is the case.
    if (priv->_polling) {
        if ((error == NULL) || (error->code < MATRIX_ERROR_M_MISSING_TOKEN)) {
            matrix_client_begin_polling(MATRIX_CLIENT(matrix_api), NULL);
        } else if ((error != NULL) && (error->code >= MATRIX_ERROR_M_MISSING_TOKEN)) {
            g_signal_emit_by_name(MATRIX_CLIENT(matrix_api), "polling-stopped", error);
            matrix_client_stop_polling(MATRIX_CLIENT(matrix_api), FALSE, NULL);
        }
    }
}

static void
matrix_http_client_real_begin_polling(MatrixClient *matrix_client, GError **error)
{
    MatrixHTTPClientPrivate *priv = matrix_http_client_get_instance_private(MATRIX_HTTP_CLIENT(matrix_client));
    GError *inner_error = NULL;

    matrix_api_sync(MATRIX_API(matrix_client),
                    NULL, NULL,
                    priv->_last_sync_token, FALSE, FALSE,
                    priv->_event_timeout,
                    cb_sync, NULL,
                    &inner_error);

    if (inner_error != NULL) {
        g_propagate_error(error, inner_error);

        return;
    }

    if (priv->_polling == FALSE) {
        g_signal_emit_by_name (matrix_client, "polling-started");
    }

    priv->_polling = TRUE;
}

static void
matrix_http_client_real_stop_polling (MatrixClient *matrix_client, gboolean cancel_ongoing, GError **error)
{
    MatrixHTTPClientPrivate *priv = matrix_http_client_get_instance_private(MATRIX_HTTP_CLIENT(matrix_client));

    priv->_polling = FALSE;

    if (cancel_ongoing) {
        matrix_api_abort_pending(MATRIX_API(matrix_client));
    }
}

static MatrixProfile *
matrix_http_client_real_get_user_profile(MatrixClient *matrix_client, const gchar *user_id, const gchar *room_id, GError **error)
{
    MatrixHTTPClientPrivate *priv = matrix_http_client_get_instance_private(MATRIX_HTTP_CLIENT(matrix_client));
    MatrixRoom *room;

    if (room_id == NULL) {
        MatrixProfile *profile = g_hash_table_lookup(priv->_user_global_profiles, user_id);

        if (profile == NULL) {
            g_set_error(error, MATRIX_ERROR, MATRIX_ERROR_UNAVAILABLE,
                        "Global profile for %s is not cached yet.",
                        user_id);
        }

        return profile;
    }

    if ((room = g_hash_table_lookup(priv->_rooms, room_id)) == NULL) {
        g_set_error(error, MATRIX_ERROR, MATRIX_ERROR_UNAVAILABLE,
                    "Room data for %s is not cached yet.", room_id);

        return NULL;
    }

    return matrix_room_get_member(room, user_id, NULL, error);
}

static MatrixPresence
matrix_http_client_real_get_user_presence(MatrixClient *matrix_client, const gchar *user_id, const gchar *room_id, GError **error)
{
    MatrixHTTPClientPrivate *priv = matrix_http_client_get_instance_private(MATRIX_HTTP_CLIENT(matrix_client));
    gpointer value;

    if (room_id != NULL) {
        g_set_error(error, MATRIX_ERROR, MATRIX_ERROR_UNSUPPORTED,
                    "Per-room presences are not supported yet.");

        return MATRIX_PRESENCE_UNKNOWN;
    }

    if (!g_hash_table_lookup_extended(priv->_user_global_presence, user_id, NULL, &value)) {
        g_set_error(error, MATRIX_ERROR, MATRIX_ERROR_UNAVAILABLE,
                    "Global presence for %s is not cached yet.",
                    user_id);

        return MATRIX_PRESENCE_UNKNOWN;
    }

    return GPOINTER_TO_INT(value);
}

static MatrixRoom *
matrix_http_client_real_get_room_by_id(MatrixClient *matrix_client, const gchar *room_id, GError **error)
{
    MatrixHTTPClientPrivate *priv;
    MatrixRoom *room;

    g_return_val_if_fail(room_id != NULL, NULL);

    priv = matrix_http_client_get_instance_private(MATRIX_HTTP_CLIENT(matrix_client));

    if ((room = g_hash_table_lookup(priv->_rooms, room_id)) == NULL) {
        g_set_error(error, MATRIX_ERROR, MATRIX_ERROR_UNAVAILABLE,
                    "Room data for %s is not cached yet.", room_id);

        return NULL;
    }

    return room;
}

static MatrixRoom *
matrix_http_client_real_get_room_by_alias(MatrixClient *matrix_client, const gchar *room_alias, GError **error)
{
    MatrixHTTPClientPrivate *priv;
    MatrixRoom *room_found = NULL;
    GHashTableIter iter;
    gpointer key;
    gpointer value;

    g_return_val_if_fail (room_alias != NULL, NULL);

    priv = matrix_http_client_get_instance_private(MATRIX_HTTP_CLIENT(matrix_client));

    g_hash_table_iter_init(&iter, priv->_rooms);

    while (g_hash_table_iter_next(&iter, &key, &value)) {
        MatrixRoom *room = value;
        gchar **aliases;
        gint n_aliases;

        if (g_strcmp0(room_alias, matrix_room_get_canonical_alias(room)) == 0) {
            room_found = room;

            break;
        }

        aliases = matrix_room_get_aliases(room, &n_aliases);

        for (gint i = 0; i < n_aliases; i++) {
            if (g_strcmp0(room_alias, aliases[i]) == 0) {
                room_found = room;

                // Whee!
                goto found_room;
            }
        }
    }
  found_room:

    if (room_found == NULL) {
        g_set_error(error, MATRIX_ERROR, MATRIX_ERROR_UNAVAILABLE,
                    "No room data found for alias %s", room_alias);
    }

    return room_found;
}

/**
 * matrix_http_client_next_txn_id:
 * @client: a #MatrixHTTPClient
 *
 * Get the next transaction ID to use.  It increments the internally stored value and returns
 * that, so it is guaranteed to be unique until we run out of #ulong boundaries.
 *
 * It is called internally by send().
 */
gulong
matrix_http_client_next_txn_id(MatrixHTTPClient *matrix_http_client)
{
    MatrixHTTPClientPrivate *priv;

    g_return_val_if_fail(matrix_http_client != NULL, 0UL);

    priv = matrix_http_client_get_instance_private(matrix_http_client);

    return ++(priv->_last_txn_id);
}

typedef struct {
    MatrixClientSendCallback cb;
    gpointer callback_target;
} SendCallbackData;

static void
send_callback(MatrixAPI *matrix_api, const gchar *content_type, JsonNode *json_content, GByteArray *raw_content, GError *err, gpointer user_data)
{
    const gchar *event_id = NULL;
    GError *new_err = err;
    SendCallbackData *cb_data = user_data;

    g_return_if_fail(matrix_api != NULL);

    // If there is no callback, there is no point to continue
    if (cb_data->cb == NULL) {
        return;
    }

    if (err == NULL) {
        JsonObject *root = json_node_get_object(json_content);

        if (json_object_has_member(root, "event_id")) {
            event_id = json_object_get_string_member(root, "event_id");
        } else {
            new_err = g_error_new_literal(MATRIX_ERROR, MATRIX_ERROR_BAD_RESPONSE,
                                          "event_id is missing from an event response");
        }
    }

    cb_data->cb(MATRIX_CLIENT(matrix_api), event_id, new_err, cb_data->callback_target);
}

static void
matrix_http_client_real_send(MatrixClient *matrix_client, const gchar *room_id, MatrixEventBase *evt, MatrixClientSendCallback cb, void *cb_target, gulong txn_id, GError **error)
{
    JsonNode *evt_node;
    JsonObject *evt_root;
    const gchar *state_key = NULL;
    SendCallbackData cb_data;

    g_return_if_fail (room_id != NULL);
    g_return_if_fail (evt != NULL);

    evt_node = matrix_event_base_get_json(evt);
    evt_root = json_node_get_object(evt_node);

    if (json_object_has_member(evt_root, "state_key")) {
        state_key = json_object_get_string_member(evt_root, "state_key");
    }

    cb_data.cb = cb;
    cb_data.callback_target = cb_target;

    if (state_key != NULL) {
        matrix_api_send_state_event(MATRIX_API(matrix_client),
                                    room_id,
                                    matrix_event_base_get_event_type(evt),
                                    state_key,
                                    json_object_get_member(evt_root, "content"),
                                    send_callback, &cb_data,
                                    error);
    } else {
        txn_id = matrix_http_client_next_txn_id(MATRIX_HTTP_CLIENT(matrix_client));
        gchar *txn_id_str = g_strdup_printf("%lu", txn_id);

        matrix_api_send_event(MATRIX_API(matrix_client),
                              room_id,
                              matrix_event_base_get_event_type(evt),
                              txn_id_str,
                              json_object_get_member(evt_root, "content"),
                              send_callback, &cb_data,
                              error);
        g_free(txn_id_str);
    }
}

static void
matrix_http_client_real_save_state(MatrixClient *matrix_client, const gchar *filename, GError * *error)
{
    JsonObject *root;
    const gchar *user_id;
    const gchar *homeserver;
    const gchar *token;
    const gchar *refresh_token;
    JsonNode *node;
    JsonGenerator *generator;

    g_return_if_fail(filename != NULL);

    root = json_object_new();

    json_object_set_string_member(root, "base_url", matrix_http_api_get_base_url(MATRIX_HTTP_API(matrix_client)));

    json_object_set_boolean_member(root, "validate_certificate", matrix_http_api_get_validate_certificate(MATRIX_HTTP_API(matrix_client)));

    if ((user_id = matrix_api_get_user_id(MATRIX_API(matrix_client))) != NULL) {
        json_object_set_string_member(root, "user_id", user_id);
    }

    if ((homeserver = matrix_api_get_homeserver(MATRIX_API(matrix_client))) != NULL) {
        json_object_set_string_member(root, "homeserver_name", homeserver);
    }

    if ((token = matrix_api_get_token(MATRIX_API(matrix_client))) != NULL) {
        json_object_set_string_member(root, "access_token", token);
    }

    if ((refresh_token = matrix_api_get_refresh_token(MATRIX_API(matrix_client))) != NULL) {
        json_object_set_string_member(root, "refresh_token", refresh_token);
    }

    node = json_node_new(JSON_NODE_OBJECT);
    json_node_set_object(node, root);

#if DEBUG
    g_debug("Saving state to %s\n", filename);
#endif

    generator = json_generator_new();
    json_generator_set_root(generator, node);
    json_generator_to_file(generator, filename, error);
}

static void
matrix_http_client_real_load_state(MatrixClient *matrix_client, const gchar *filename, GError **error)
{
    JsonParser *parser;
    JsonNode *node;
    JsonNode *root_node;
    JsonObject *root;

    g_return_if_fail(filename != NULL);

    parser = json_parser_new();

#if DEBUG
    g_debug("Loading state from %s\n", filename);
#endif

    if (!json_parser_load_from_file(parser, filename, error)) {
        g_object_unref(parser);

        return;
    }

    root_node = json_node_ref(json_parser_get_root(parser));
    g_object_unref(parser);

    if (json_node_get_node_type(root_node) != JSON_NODE_OBJECT) {
        g_set_error(error, MATRIX_ERROR, MATRIX_ERROR_INVALID_FORMAT,
                    "Save data must be a JSON object.");

        json_node_unref(root_node);

        return;
    }

    root = json_node_get_object(root_node);

    if ((node = json_object_get_member(root, "base_url")) == NULL) {
        g_set_error(error, MATRIX_ERROR, MATRIX_ERROR_INVALID_FORMAT,
                    "Save data has no base_url key");

        json_node_unref(root_node);

        return;
    }

    matrix_http_api_set_base_url(MATRIX_HTTP_API(matrix_client), json_node_get_string(node));

#if DEBUG
    g_debug("Loaded base URL %s", json_node_get_string(node));
#endif

    if ((node = json_object_get_member(root, "validate_certificate")) == NULL) {
        g_set_error(error, MATRIX_ERROR, MATRIX_ERROR_INVALID_FORMAT,
                    "Save data has no validate_certificate key");

        json_node_unref(root_node);

        return;
    }

    matrix_http_api_set_validate_certificate(MATRIX_HTTP_API(matrix_client), json_node_get_boolean(node));

    if ((node = json_object_get_member(root, "user_id")) != NULL) {
        g_free(MATRIX_HTTP_API(matrix_client)->_user_id);
        MATRIX_HTTP_API(matrix_client)->_user_id = g_strdup(json_node_get_string(node));

#if DEBUG
        g_debug("Loaded user ID %s", MATRIX_HTTP_API(matrix_client)->_user_id);
#endif
    }

    if ((node = json_object_get_member(root, "homeserver_name")) != NULL) {
        g_free(MATRIX_HTTP_API(matrix_client)->_homeserver);
        MATRIX_HTTP_API(matrix_client)->_homeserver = g_strdup(json_node_get_string(node));

#if DEBUG
        g_debug("Loaded homeserver name %s", MATRIX_HTTP_API(matrix_client)->_homeserver);
#endif
    }

    if ((node = json_object_get_member(root, "access_token")) != NULL) {
        matrix_api_set_token(MATRIX_API(matrix_client), json_node_get_string(node));

#if DEBUG
        g_debug("Loaded access token %s", matrix_api_get_token(MATRIX_API(matrix_client)));
#endif
    }

    if ((node = json_object_get_member(root, "refresh_token")) != NULL) {
        matrix_api_set_refresh_token(MATRIX_API(matrix_client), json_node_get_string(node));

#if DEBUG
        g_debug("Loaded refresh token %s", matrix_api_get_refresh_token(MATRIX_API(matrix_client)));
#endif
    }

    json_node_unref(root_node);
}

static void
matrix_http_client_finalize(GObject *gobject)
{
    MatrixHTTPClientPrivate *priv = matrix_http_client_get_instance_private(MATRIX_HTTP_CLIENT(gobject));

    g_free(priv->_last_sync_token);
    g_hash_table_unref(priv->_user_global_profiles);
    g_hash_table_unref(priv->_user_global_presence);
    g_hash_table_unref(priv->_rooms);

    G_OBJECT_CLASS(matrix_http_client_parent_class)->finalize(gobject);
}

static void
matrix_http_client_class_init(MatrixHTTPClientClass *klass)
{
    G_OBJECT_CLASS(klass)->finalize = matrix_http_client_finalize;
}

static void
matrix_http_client_matrix_client_interface_init(MatrixClientInterface * iface)
{
    iface->login_with_password = matrix_http_client_real_login_with_password;
    iface->register_with_password = matrix_http_client_real_register_with_password;
    iface->logout = matrix_http_client_real_logout;
    iface->begin_polling = matrix_http_client_real_begin_polling;
    iface->stop_polling = matrix_http_client_real_stop_polling;
    iface->get_user_profile = matrix_http_client_real_get_user_profile;
    iface->get_user_presence = matrix_http_client_real_get_user_presence;
    iface->get_room_by_id = matrix_http_client_real_get_room_by_id;
    iface->get_room_by_alias = matrix_http_client_real_get_room_by_alias;
    iface->send = matrix_http_client_real_send;
    iface->save_state = matrix_http_client_real_save_state;
    iface->load_state = matrix_http_client_real_load_state;
}

static void
matrix_http_client_init(MatrixHTTPClient *matrix_http_client)
{
    MatrixHTTPClientPrivate *priv = matrix_http_client_get_instance_private(matrix_http_client);

    priv->_polling = FALSE;
    priv->_event_timeout = (gulong)30000;
    priv->_user_global_profiles = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, g_object_unref);
    priv->_user_global_presence = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, NULL);
    priv->_rooms = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, g_object_unref);
    priv->_last_txn_id = (gulong)0;
}
