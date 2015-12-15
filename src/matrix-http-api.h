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

#ifndef __MATRIX_HTTP_API_H__
#define __MATRIX_HTTP_API_H__

#include <glib-object.h>

#include "matrix-api.h"

G_BEGIN_DECLS

#define MATRIX_TYPE_HTTP_API         (matrix_http_api_get_type())
#define MATRIX_HTTP_API(o)           (G_TYPE_CHECK_INSTANCE_CAST((o), MATRIX_TYPE_HTTP_API, MatrixHTTPAPI))
#define MATRIX_HTTP_API_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST((k), MATRIX_TYPE_HTTP_API, MatrixHTTPAPIClass))
#define MATRIX_IS_HTTP_API(o)        (G_TYPE_CHECK_INSTANCE_TYPE((o), MATRIX_TYPE_HTTP_API))
#define MATRIX_IS_HTTP_API_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE((k), MATRIX_TYPE_HTTP_API))
#define MATRIX_HTTP_API_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS((o), MATRIX_TYPE_HTTP_API, MatrixHTTPAPIClass))

typedef struct _MatrixHTTPAPI      MatrixHTTPAPI;
typedef struct _MatrixHTTPAPIClass MatrixHTTPAPIClass;

struct _MatrixHTTPAPI {
    /* Parent instance structure */
    GObject parent_instance;

    /* Instance members */
};

struct _MatrixHTTPAPIClass {
    GObjectClass parent_class;
};

GType matrix_http_api_get_type(void) G_GNUC_CONST;
void matrix_http_api_set_validate_certificate(MatrixHTTPAPI *api,
                                              gboolean validate_certificate);
gboolean matrix_http_api_get_validate_certificate(MatrixHTTPAPI *api);
const gchar *matrix_http_api_get_base_url(MatrixHTTPAPI *api);

MatrixHTTPAPI *matrix_http_api_new(const gchar *base_url, const gchar *token);

/* Media */
void matrix_http_api_media_download(MatrixAPI *api,
                                    MatrixAPICallback callback,
                                    gpointer user_data,
                                    const gchar *server_name,
                                    const gchar *media_id);
void matrix_http_api_media_thumbnail(MatrixAPI *api,
                                     MatrixAPICallback callback,
                                     gpointer user_data,
                                     const gchar *server_name,
                                     const gchar *media_id,
                                     guint width,
                                     guint height,
                                     MatrixAPIResizeMethod method);
void matrix_http_api_media_upload(MatrixAPI *api,
                                  MatrixAPICallback callback,
                                  gpointer user_data,
                                  const gchar *content_type,
                                  const GByteArray *content);

/* Presence */
void matrix_http_api_get_presence_list(MatrixAPI *api,
                                       MatrixAPICallback callback,
                                       gpointer user_data,
                                       const gchar *user_id);
void matrix_http_api_update_presence_list(MatrixAPI *api,
                                          MatrixAPICallback callback,
                                          gpointer user_data,
                                          const gchar *user_id,
                                          GList *drop_ids,
                                          GList *invite_ids);
void matrix_http_api_get_user_presence(MatrixAPI *api,
                                       MatrixAPICallback callback,
                                       gpointer user_data,
                                       const gchar *user_id);
void matrix_http_api_set_user_presence(MatrixAPI *api,
                                       MatrixAPICallback callback,
                                       gpointer user_data,
                                       const gchar *user_id,
                                       MatrixAPIPresence presence,
                                       const gchar *status_message);

/* Push notifications */
void matrix_http_api_set_pushers(MatrixAPI *api,
                                 MatrixAPICallback callback,
                                 gpointer user_data,
                                 MatrixAPIPusher *pusher);
void matrix_http_api_get_pushers(MatrixAPI *api,
                                 MatrixAPICallback callback,
                                 gpointer user_data);
void matrix_http_api_delete_pusher(MatrixAPI *api,
                                   MatrixAPICallback callback,
                                   gpointer user_data,
                                   const gchar *scope,
                                   MatrixAPIPusherKind kind,
                                   const gchar *rule_id);
void matrix_http_api_get_pusher(MatrixAPI *api,
                                MatrixAPICallback callback,
                                gpointer user_data,
                                const gchar *scope,
                                MatrixAPIPusherKind kind,
                                const gchar *rule_id);
void matrix_http_api_add_pusher(MatrixAPI *api,
                                MatrixAPICallback callback,
                                gpointer user_data,
                                const gchar *scope,
                                MatrixAPIPusherKind kind,
                                const gchar *rule_id,
                                const gchar *before,
                                const gchar *after,
                                GList *actions,
                                GList *conditions);
void matrix_http_api_toggle_pusher(MatrixAPI *api,
                                   MatrixAPICallback callback,
                                   gpointer user_data,
                                   const gchar *scope,
                                   MatrixAPIPusherKind kind,
                                   const gchar *rule_id,
                                   gboolean enabled);

/* Room creation */
void matrix_http_api_create_room(MatrixAPI *api,
                                 MatrixAPICallback callback,
                                 gpointer user_data,
                                 MatrixAPIRoomPreset preset,
                                 const gchar *room_name,
                                 const gchar *room_alias,
                                 const gchar *topic,
                                 MatrixAPIRoomVisibility visibility,
                                 JsonNode *creation_content,
                                 GList *initial_state,
                                 GList *invitees);

/* Room directory */
void matrix_http_api_delete_room_alias(MatrixAPI *api,
                                       MatrixAPICallback callback,
                                       gpointer user_data,
                                       const gchar *room_alias);
void matrix_http_api_get_room_id(MatrixAPI *api,
                                 MatrixAPICallback callback,
                                 gpointer user_data,
                                 const gchar *room_alias);
void matrix_http_api_create_room_alias(MatrixAPI *api,
                                       MatrixAPICallback callback,
                                       gpointer user_data,
                                       const gchar *room_id,
                                       const gchar *room_alias);

/* Room discovery */
void matrix_http_api_list_public_rooms(MatrixAPI *api,
                                       MatrixAPICallback callback,
                                       gpointer user_data);

/* Room membership */

void matrix_http_api_ban_user(MatrixAPI *api,
                              MatrixAPICallback callback,
                              gpointer user_data,
                              const gchar *room_id,
                              const gchar *user_id,
                              const gchar *reason);
void matrix_http_api_forget_room(MatrixAPI *api,
                                 MatrixAPICallback callback,
                                 gpointer user_data,
                                 const gchar *room_id);
void matrix_http_api_invite_user_3rdparty(MatrixAPI *api,
                                          MatrixAPICallback callback,
                                          gpointer user_data,
                                          const gchar *room_id,
                                          const gchar *address,
                                          const gchar *medium,
                                          const gchar *id_server);
void matrix_http_api_invite_user(MatrixAPI *api,
                                 MatrixAPICallback callback,
                                 gpointer user_data,
                                 const gchar *room_id,
                                 const gchar *user_id);
void matrix_http_api_join_room(MatrixAPI *api,
                               MatrixAPICallback callback,
                               gpointer user_data,
                               const gchar *room_id_or_alias);
void matrix_http_api_leave_room(MatrixAPI *api,
                                MatrixAPICallback callback,
                                gpointer user_data,
                                const gchar *room_id);

/* Room participation */

void matrix_http_api_event_stream(MatrixAPI *api,
                                  MatrixAPICallback callback,
                                  gpointer user_data,
                                  const gchar *from_token,
                                  gulong timeout);
void matrix_http_api_get_event(MatrixAPI *api,
                               MatrixAPICallback callback,
                               gpointer user_data,
                               const gchar *event_id);
void matrix_http_api_initial_sync(MatrixAPI *api,
                                  MatrixAPICallback callback,
                                  gpointer user_data,
                                  guint limit,
                                  gboolean archived);
void matrix_http_api_get_event_context(MatrixAPI *api,
                                       MatrixAPICallback callback,
                                       gpointer user_data,
                                       const gchar *room_id,
                                       const gchar *event_id,
                                       guint limit);
void matrix_http_api_initial_sync_room(MatrixAPI *api,
                                       MatrixAPICallback callback,
                                       gpointer user_data,
                                       const gchar *room_id);
void matrix_http_api_list_room_members(MatrixAPI *api,
                                       MatrixAPICallback callback,
                                       gpointer user_data,
                                       const gchar *room_id);
void matrix_http_api_list_room_messages(MatrixAPI *api,
                                        MatrixAPICallback callback,
                                        gpointer user_data,
                                        const gchar *room_id,
                                        const gchar *from_token,
                                        MatrixAPIEventDirection direction,
                                        guint limit);
void matrix_http_api_send_event_receipt(MatrixAPI *api,
                                        MatrixAPICallback callback,
                                        gpointer user_data,
                                        const gchar *room_id,
                                        MatrixAPIReceiptType type,
                                        const gchar *event_id,
                                        JsonNode *receipt);
void matrix_http_api_redact_event(MatrixAPI *api,
                                  MatrixAPICallback callback,
                                  gpointer user_data,
                                  const gchar *room_id,
                                  const gchar *event_id,
                                  const gchar *txn_id,
                                  const gchar *reason);
void matrix_http_api_send_message_event(MatrixAPI *api,
                                        MatrixAPICallback callback,
                                        gpointer user_data,
                                        const gchar *room_id,
                                        const gchar *event_type,
                                        const gchar *txn_id,
                                        const JsonNode *content);
void matrix_http_api_get_room_state(MatrixAPI *api,
                                    MatrixAPICallback callback,
                                    gpointer user_data,
                                    const gchar *room_id,
                                    const gchar *event_type,
                                    const gchar *state_key);
void matrix_http_api_send_room_event(MatrixAPI *api,
                                     MatrixAPICallback callback,
                                     gpointer user_data,
                                     const gchar *room_id,
                                     const gchar *event_type,
                                     const gchar *state_key,
                                     JsonNode *content);
void matrix_http_api_notify_room_typing(MatrixAPI *api,
                                        MatrixAPICallback callback,
                                        gpointer user_data,
                                        const gchar *user_id,
                                        const gchar *room_id,
                                        guint timeout,
                                        gboolean typing);
void matrix_http_api_sync(MatrixAPI *api,
                          MatrixAPICallback callback,
                          gpointer user_data,
                          const gchar *filter_id,
                          const JsonNode *filter,
                          const gchar *since,
                          gboolean full_state,
                          gboolean set_presence,
                          gulong timeout);
void matrix_http_api_create_filter(MatrixAPI *api,
                                   MatrixAPICallback callback,
                                   gpointer user_data,
                                   const gchar *user_id,
                                   MatrixAPIFilter *filter);
void matrix_http_api_download_filter(MatrixAPI *api,
                                     MatrixAPICallback callback,
                                     gpointer user_data,
                                     const gchar *user_id,
                                     const gchar *filter_id);

/* Search */
void *search_reserved;

/* Server administration */

void matrix_http_api_whois(MatrixAPI *api,
                           MatrixAPICallback callback,
                           gpointer user_data,
                           const gchar *user_id);

/* Session management */

void matrix_http_api_login(MatrixAPI *api,
                           MatrixAPICallback callback,
                           gpointer user_data,
                           const gchar *type,
                           const JsonNode *content,
                           GError **error);
void matrix_http_api_token_refresh(MatrixAPI *api,
                                   MatrixAPICallback callback,
                                   gpointer user_data,
                                   const gchar *refresh_token);

/* User data */

void matrix_http_api_get_3pids(MatrixAPI *api,
                               MatrixAPICallback callback,
                               gpointer user_data);
void matrix_http_api_add_3pid(MatrixAPI *api,
                              MatrixAPICallback callback,
                              gpointer user_data,
                              gboolean bind_creds,
                              GList *threepid_creds);
void matrix_http_api_change_password(MatrixAPI *api,
                                     MatrixAPICallback callback,
                                     gpointer user_data,
                                     const gchar *new_password);
void matrix_http_api_get_profile(MatrixAPI *api,
                                 MatrixAPICallback callback,
                                 gpointer user_data,
                                 const gchar *user_id);
void matrix_http_api_get_avatar_url(MatrixAPI *api,
                                    MatrixAPICallback callback,
                                    gpointer user_data,
                                    const gchar *user_id);
void matrix_http_api_set_avatar_url(MatrixAPI *api,
                                    MatrixAPICallback callback,
                                    gpointer user_data,
                                    const gchar *user_id,
                                    const gchar *avatar_url);
void matrix_http_api_get_display_name(MatrixAPI *api,
                                      MatrixAPICallback callback,
                                      gpointer user_data,
                                      const gchar *user_id);
void matrix_http_api_set_display_name(MatrixAPI *api,
                                      MatrixAPICallback callback,
                                      gpointer user_data,
                                      const gchar *user_id,
                                      const gchar *display_name);
void matrix_http_api_register_account(MatrixAPI *api,
                                      MatrixAPICallback callback,
                                      gpointer user_data,
                                      gboolean bind_email,
                                      const gchar *username,
                                      const gchar *password);
void matrix_http_api_set_account_data(MatrixAPI *api,
                                      MatrixAPICallback callback,
                                      gpointer user_data,
                                      const gchar *user_id,
                                      const gchar *type,
                                      JsonNode *content);
void matrix_http_api_set_room_account_data(MatrixAPI *api,
                                           MatrixAPICallback callback,
                                           gpointer user_data,
                                           const gchar *user_id,
                                           const gchar *room_id,
                                           const gchar *type,
                                           JsonNode *content);
void matrix_http_api_get_room_tags(MatrixAPI *api,
                                   MatrixAPICallback callback,
                                   gpointer user_data,
                                   const gchar *user_id,
                                   const gchar *room_id);
void matrix_http_api_delete_room_tag(MatrixAPI *api,
                                     MatrixAPICallback callback,
                                     gpointer user_data,
                                     const gchar *user_id,
                                     const gchar *room_id,
                                     const gchar *tag);
void matrix_http_api_add_room_tag(MatrixAPI *api,
                                  MatrixAPICallback callback,
                                  gpointer user_data,
                                  const gchar *user_id,
                                  const gchar *room_id,
                                  const gchar *tag,
                                  JsonNode *content);

/* VoIP */

void matrix_http_api_get_turn_server(MatrixAPI *api,
                                     MatrixAPICallback callback,
                                     gpointer user_data);

G_END_DECLS

#endif /* __MATRIX_HTTP_API_H__ */
