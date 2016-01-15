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

#ifndef __MATRIX_API_IFACE_H__
#define __MATRIX_API_IFACE_H__

#include <glib-object.h>
#include <json-glib/json-glib.h>

#include "matrix-api-types.h"

G_BEGIN_DECLS

#define MATRIX_TYPE_API         (matrix_api_get_type())
#define MATRIX_API(o)           (G_TYPE_CHECK_INSTANCE_CAST((o), MATRIX_TYPE_API, MatrixAPI))
#define MATRIX_IS_API(o)        (G_TYPE_CHECK_INSTANCE_TYPE((o), MATRIX_TYPE_API))
#define MATRIX_API_GET_IFACE(o) (G_TYPE_INSTANCE_GET_INTERFACE((o), MATRIX_TYPE_API, MatrixAPIInterface))

typedef struct _MatrixAPIInterface MatrixAPIInterface;
typedef struct _MatrixAPI          MatrixAPI;

typedef void (*MatrixAPICallback)(MatrixAPI *api,
                                  const gchar *content_type,
                                  JsonNode *json_content,
                                  GByteArray *raw_content,
                                  gpointer user_data,
                                  GError *err);

struct _MatrixAPIInterface {
    /*< private >*/
    GTypeInterface g_iface;

    /*< public >*/

    /* Properties */
    void (*set_token)(MatrixAPI *api, const gchar *token);
    const gchar *(*get_token)(MatrixAPI *api);

    void (*set_refresh_token)(MatrixAPI *api, const gchar *refresh_token);
    const gchar *(*get_refresh_token)(MatrixAPI *api);

    const gchar *(*get_user_id)(MatrixAPI *api);

    const gchar *(*get_homeserver)(MatrixAPI *api);

    /*< private >*/
    void *properties_reserved[10];

    /*< public >*/
    /* Media */
    void (*media_download)(MatrixAPI *api,
                           MatrixAPICallback callback,
                           gpointer user_data,
                           const gchar *server_name,
                           const gchar *media_id,
                           GError **error);
    void (*media_thumbnail)(MatrixAPI *api,
                            MatrixAPICallback callback,
                            gpointer user_data,
                            const gchar *server_name,
                            const gchar *media_id,
                            guint width,
                            guint height,
                            MatrixAPIResizeMethod method,
                            GError **error);
    void (*media_upload)(MatrixAPI *api,
                         MatrixAPICallback callback,
                         gpointer user_data,
                         const gchar *content_type,
                         GByteArray *content,
                         GError **error);

    /* Presence */
    void (*get_presence_list)(MatrixAPI *api,
                              MatrixAPICallback callback,
                              gpointer user_data,
                              const gchar *user_id,
                              GError **error);
    void (*update_presence_list)(MatrixAPI *api,
                                 MatrixAPICallback callback,
                                 gpointer user_data,
                                 const gchar *user_id,
                                 GList *drop_ids,
                                 GList *invite_ids,
                                 GError **error);
    void (*get_user_presence)(MatrixAPI *api,
                              MatrixAPICallback callback,
                              gpointer user_data,
                              const gchar *user_id,
                              GError **error);
    void (*set_user_presence)(MatrixAPI *api,
                              MatrixAPICallback callback,
                              gpointer user_data,
                              const gchar *user_id,
                              MatrixAPIPresence presence,
                              const gchar *status_message,
                              GError **error);

    /* Push notifications */
    void (*update_pusher)(MatrixAPI *api,
                          MatrixAPICallback callback,
                          gpointer user_data,
                          MatrixAPIPusher *pusher,
                          GError **error);
    void (*get_pushers)(MatrixAPI *api,
                        MatrixAPICallback callback,
                        gpointer user_data,
                        GError **error);
    void (*delete_pusher)(MatrixAPI *api,
                          MatrixAPICallback callback,
                          gpointer user_data,
                          const gchar *scope,
                          MatrixAPIPusherKind kind,
                          const gchar *rule_id,
                          GError **error);
    void (*get_pusher)(MatrixAPI *api,
                       MatrixAPICallback callback,
                       gpointer user_data,
                       const gchar *scope,
                       MatrixAPIPusherKind kind,
                       const gchar *rule_id,
                       GError **error);
    void (*add_pusher)(MatrixAPI *api,
                       MatrixAPICallback callback,
                       gpointer user_data,
                       const gchar *scope,
                       MatrixAPIPusherKind kind,
                       const gchar *rule_id,
                       const gchar *before,
                       const gchar *after,
                       GList *actions,
                       GList *conditions,
                       GError **error);
    void (*toggle_pusher)(MatrixAPI *api,
                          MatrixAPICallback callback,
                          gpointer user_data,
                          const gchar *scope,
                          MatrixAPIPusherKind kind,
                          const gchar *rule_id,
                          gboolean enabled,
                          GError **error);

    /* Room creation */
    void (*create_room)(MatrixAPI *api,
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
                        GError **error);

    /* Room directory */
    void (*delete_room_alias)(MatrixAPI *api,
                              MatrixAPICallback callback,
                              gpointer user_data,
                              const gchar *room_alias,
                              GError **error);
    void (*get_room_id)(MatrixAPI *api,
                        MatrixAPICallback callback,
                        gpointer user_data,
                        const gchar *room_alias,
                        GError **error);
    void (*create_room_alias)(MatrixAPI *api,
                              MatrixAPICallback callback,
                              gpointer user_data,
                              const gchar *room_id,
                              const gchar *room_alias,
                              GError **error);

    /* Room discovery */
    void (*list_public_rooms)(MatrixAPI *api,
                              MatrixAPICallback callback,
                              gpointer user_data,
                              GError **error);

    /* Room membership */

    void (*ban_user)(MatrixAPI *api,
                     MatrixAPICallback callback,
                     gpointer user_data,
                     const gchar *room_id,
                     const gchar *user_id,
                     const gchar *reason,
                     GError **error);
    void (*forget_room)(MatrixAPI *api,
                        MatrixAPICallback callback,
                        gpointer user_data,
                        const gchar *room_id,
                        GError **error);
    void (*invite_user_3rdparty)(MatrixAPI *api,
                                 MatrixAPICallback callback,
                                 gpointer user_data,
                                 const gchar *room_id,
                                 const gchar *address,
                                 const gchar *medium,
                                 const gchar *id_server,
                                 GError **error);
    void (*invite_user)(MatrixAPI *api,
                        MatrixAPICallback callback,
                        gpointer user_data,
                        const gchar *room_id,
                        const gchar *user_id,
                        GError **error);
    void (*join_room)(MatrixAPI *api,
                      MatrixAPICallback callback,
                      gpointer user_data,
                      const gchar *room_id,
                      GError **error);
    void (*leave_room)(MatrixAPI *api,
                       MatrixAPICallback callback,
                       gpointer user_data,
                       const gchar *room_id,
                       GError **error);

    /* Room participation */

    void (*event_stream)(MatrixAPI *api,
                         MatrixAPICallback callback,
                         gpointer user_data,
                         const gchar *from_token,
                         gulong timeout,
                         GError **error);
    void (*get_event)(MatrixAPI *api,
                      MatrixAPICallback callback,
                      gpointer user_data,
                      const gchar *event_id,
                      GError **error);
    void (*initial_sync)(MatrixAPI *api,
                         MatrixAPICallback callback,
                         gpointer user_data,
                         guint limit,
                         gboolean archived,
                         GError **error);
    void (*get_event_context)(MatrixAPI *api,
                              MatrixAPICallback callback,
                              gpointer user_data,
                              const gchar *room_id,
                              const gchar *event_id,
                              guint limit,
                              GError **error);
    void (*initial_sync_room)(MatrixAPI *api,
                              MatrixAPICallback callback,
                              gpointer user_data,
                              const gchar *room_id,
                              GError **error);
    void (*list_room_members)(MatrixAPI *api,
                              MatrixAPICallback callback,
                              gpointer user_data,
                              const gchar *room_id,
                              GError **error);
    void (*list_room_messages)(MatrixAPI *api,
                               MatrixAPICallback callback,
                               gpointer user_data,
                               const gchar *room_id,
                               const gchar *from_token,
                               MatrixAPIEventDirection direction,
                               guint limit,
                               GError **error);
    void (*send_event_receipt)(MatrixAPI *api,
                               MatrixAPICallback callback,
                               gpointer user_data,
                               const gchar *room_id,
                               MatrixAPIReceiptType receipt_type,
                               const gchar *event_id,
                               JsonNode *receipt,
                               GError **error);
    void (*redact_event)(MatrixAPI *api,
                         MatrixAPICallback callback,
                         gpointer user_data,
                         const gchar *room_id,
                         const gchar *event_id,
                         const gchar *txn_id,
                         const gchar *reason,
                         GError **error);
    void (*send_message_event)(MatrixAPI *api,
                               MatrixAPICallback callback,
                               gpointer user_data,
                               const gchar *room_id,
                               const gchar *event_type,
                               const gchar *txn_id,
                               JsonNode *content,
                               GError **error);
    void (*get_room_state)(MatrixAPI *api,
                           MatrixAPICallback callback,
                           gpointer user_data,
                           const gchar *room_id,
                           const gchar *event_type,
                           const gchar *state_key,
                           GError **error);
    void (*send_room_event)(MatrixAPI *api,
                            MatrixAPICallback callback,
                            gpointer user_data,
                            const gchar *room_id,
                            const gchar *event_type,
                            const gchar *state_key,
                            const JsonNode *content,
                            GError **error);
    void (*notify_room_typing)(MatrixAPI *api,
                               MatrixAPICallback callback,
                               gpointer user_data,
                               const gchar *user_id,
                               const gchar *room_id,
                               guint timeout,
                               gboolean typing,
                               GError **error);
    void (*sync)(MatrixAPI *api,
                 MatrixAPICallback callback,
                 gpointer user_data,
                 const gchar *filter_id,
                 const MatrixAPIFilter *filter,
                 const gchar *since,
                 gboolean full_state,
                 gboolean set_presence,
                 gulong timeout,
                 GError **error);
    void (*create_filter)(MatrixAPI *api,
                          MatrixAPICallback callback,
                          gpointer user_data,
                          const gchar *user_id,
                          MatrixAPIFilter *filter,
                          GError **error);
    void (*download_filter)(MatrixAPI *api,
                            MatrixAPICallback callback,
                            gpointer user_data,
                            const gchar *user_id,
                            const gchar *filter_id,
                            GError **error);

    /*< private >*/
    /* Search */
    void *search_reserved;

    /*< public >*/
    /* Server administration */

    void (*whois)(MatrixAPI *api,
                  MatrixAPICallback callback,
                  gpointer user_data,
                  const gchar *user_id,
                  GError **error);

    /* Session management */

    void (*login)(MatrixAPI *api,
                  MatrixAPICallback callback,
                  gpointer user_data,
                  const gchar *type,
                  const JsonNode *content,
                  GError **error);
    void (*token_refresh)(MatrixAPI *api,
                          MatrixAPICallback callback,
                          gpointer user_data,
                          const gchar *refresh_token,
                          GError **error);

    /* User data */

    void (*get_3pids)(MatrixAPI *api,
                      MatrixAPICallback callback,
                      gpointer user_data,
                      GError **error);
    void (*add_3pid)(MatrixAPI *api,
                     MatrixAPICallback callback,
                     gpointer user_data,
                     gboolean bind_creds,
                     MatrixAPI3PidCredential *threepid_creds,
                     GError **error);
    void (*change_password)(MatrixAPI *api,
                            MatrixAPICallback callback,
                            gpointer user_data,
                            const gchar *new_password,
                            GError **error);
    void (*get_profile)(MatrixAPI *api,
                        MatrixAPICallback callback,
                        gpointer user_data,
                        const gchar *user_id,
                        GError **error);
    void (*get_avatar_url)(MatrixAPI *api,
                           MatrixAPICallback callback,
                           gpointer user_data,
                           const gchar *user_id,
                           GError **error);
    void (*set_avatar_url)(MatrixAPI *api,
                           MatrixAPICallback callback,
                           gpointer user_data,
                           const gchar *user_id,
                           const gchar *avatar_url,
                           GError **error);
    void (*get_display_name)(MatrixAPI *api,
                             MatrixAPICallback callback,
                             gpointer user_data,
                             const gchar *user_id,
                             GError **error);
    void (*set_display_name)(MatrixAPI *api,
                             MatrixAPICallback callback,
                             gpointer user_data,
                             const gchar *user_id,
                             const gchar *display_name,
                             GError **error);
    void (*register_account)(MatrixAPI *api,
                             MatrixAPICallback callback,
                             gpointer user_data,
                             gboolean bind_email,
                             const gchar *username,
                             const gchar *password,
                             GError **error);
    void (*set_account_data)(MatrixAPI *api,
                             MatrixAPICallback callback,
                             gpointer user_data,
                             const gchar *user_id,
                             const gchar *room_id,
                             const gchar *type,
                             const JsonNode *content,
                             GError **error);
    void (*get_room_tags)(MatrixAPI *api,
                          MatrixAPICallback callback,
                          gpointer user_data,
                          const gchar *user_id,
                          const gchar *room_id,
                          GError **error);
    void (*delete_room_tag)(MatrixAPI *api,
                            MatrixAPICallback callback,
                            gpointer user_data,
                            const gchar *user_id,
                            const gchar *room_id,
                            const gchar *tag,
                            GError **error);
    void (*add_room_tag)(MatrixAPI *api,
                         MatrixAPICallback callback,
                         gpointer user_data,
                         const gchar *user_id,
                         const gchar *room_id,
                         const gchar *tag,
                         const JsonNode *content,
                         GError **error);

    /* VoIP */

    void (*get_turn_server)(MatrixAPI *api,
                            MatrixAPICallback callback,
                            gpointer user_data,
                            GError **error);

    /*< private >*/
    /* Leave room for endpoint expansion */
    void *padding[50];
};

GType matrix_api_get_type(void) G_GNUC_CONST;

/* Property getters and setters */

void matrix_api_set_token(MatrixAPI *api, const gchar *token);
const gchar *matrix_api_get_token(MatrixAPI *api);
void matrix_api_set_refresh_token(MatrixAPI *api, const gchar *refresh_token);
const gchar *matrix_api_get_refresh_token(MatrixAPI *api);
const gchar *matrix_api_get_user_id(MatrixAPI *api);
const gchar *matrix_api_get_homeserver(MatrixAPI *api);

/* API definition */

/* Media */
void matrix_api_media_download(MatrixAPI *api,
                               MatrixAPICallback callback,
                               gpointer user_data,
                               const gchar *server_name,
                               const gchar *media_id,
                               GError **error);
void matrix_api_media_thumbnail(MatrixAPI *api,
                                MatrixAPICallback callback,
                                gpointer user_data,
                                const gchar *server_name,
                                const gchar *media_id,
                                guint width,
                                guint height,
                                MatrixAPIResizeMethod method,
                                GError **error);
void matrix_api_media_upload(MatrixAPI *api,
                             MatrixAPICallback callback,
                             gpointer user_data,
                             const gchar *content_type,
                             GByteArray *content,
                             GError **error);

/* Presence */
void matrix_api_get_presence_list(MatrixAPI *api,
                                  MatrixAPICallback callback,
                                  gpointer user_data,
                                  const gchar *user_id,
                                  GError **error);
void matrix_api_update_presence_list(MatrixAPI *api,
                                     MatrixAPICallback callback,
                                     gpointer user_data,
                                     const gchar *user_id,
                                     GList *drop_ids,
                                     GList *invite_ids,
                                     GError **error);
void matrix_api_get_user_presence(MatrixAPI *api,
                                  MatrixAPICallback callback,
                                  gpointer user_data,
                                  const gchar *user_id,
                                  GError **error);
void matrix_api_set_user_presence(MatrixAPI *api,
                                  MatrixAPICallback callback,
                                  gpointer user_data,
                                  const gchar *user_id,
                                  MatrixAPIPresence presence,
                                  const gchar *status_message,
                                  GError **error);

/* Push notifications */
void matrix_api_update_pusher(MatrixAPI *api,
                              MatrixAPICallback callback,
                              gpointer user_data,
                              MatrixAPIPusher *pusher,
                              GError **error);
void matrix_api_get_pushers(MatrixAPI *api,
                            MatrixAPICallback callback,
                            gpointer user_data,
                            GError **error);
void matrix_api_delete_pusher(MatrixAPI *api,
                              MatrixAPICallback callback,
                              gpointer user_data,
                              const gchar *scope,
                              MatrixAPIPusherKind kind,
                              const gchar *rule_id,
                              GError **error);
void matrix_api_get_pusher(MatrixAPI *api,
                           MatrixAPICallback callback,
                           gpointer user_data,
                           const gchar *scope,
                           MatrixAPIPusherKind kind,
                           const gchar *rule_id,
                           GError **error);
void matrix_api_add_pusher(MatrixAPI *api,
                           MatrixAPICallback callback,
                           gpointer user_data,
                           const gchar *scope,
                           MatrixAPIPusherKind kind,
                           const gchar *rule_id,
                           const gchar *before,
                           const gchar *after,
                           GList *actions,
                           GList *conditions,
                           GError **error);
void matrix_api_toggle_pusher(MatrixAPI *api,
                              MatrixAPICallback callback,
                              gpointer user_data,
                              const gchar *scope,
                              MatrixAPIPusherKind kind,
                              const gchar *rule_id,
                              gboolean enabled,
                              GError **error);

/* Room creation */
void matrix_api_create_room(MatrixAPI *api,
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
                            GError **error);

/* Room directory */
void matrix_api_delete_room_alias(MatrixAPI *api,
                                  MatrixAPICallback callback,
                                  gpointer user_data,
                                  const gchar *room_alias,
                                  GError **error);
void matrix_api_get_room_id(MatrixAPI *api,
                            MatrixAPICallback callback,
                            gpointer user_data,
                            const gchar *room_alias,
                            GError **error);
void matrix_api_create_room_alias(MatrixAPI *api,
                                  MatrixAPICallback callback,
                                  gpointer user_data,
                                  const gchar *room_id,
                                  const gchar *room_alias,
                                  GError **error);

/* Room discovery */
void matrix_api_list_public_rooms(MatrixAPI *api,
                                  MatrixAPICallback callback,
                                  gpointer user_data,
                                  GError **error);

/* Room membership */

void matrix_api_ban_user(MatrixAPI *api,
                         MatrixAPICallback callback,
                         gpointer user_data,
                         const gchar *room_id,
                         const gchar *user_id,
                         const gchar *reason,
                         GError **error);
void matrix_api_forget_room(MatrixAPI *api,
                            MatrixAPICallback callback,
                            gpointer user_data,
                            const gchar *room_id,
                            GError **error);
void matrix_api_invite_user_3rdparty(MatrixAPI *api,
                                     MatrixAPICallback callback,
                                     gpointer user_data,
                                     const gchar *room_id,
                                     const gchar *address,
                                     const gchar *medium,
                                     const gchar *id_server,
                                     GError **error);
void matrix_api_invite_user(MatrixAPI *api,
                            MatrixAPICallback callback,
                            gpointer user_data,
                            const gchar *room_id,
                            const gchar *user_id,
                            GError **error);
void matrix_api_join_room(MatrixAPI *api,
                          MatrixAPICallback callback,
                          gpointer user_data,
                          const gchar *room_id,
                          GError **error);
void matrix_api_leave_room(MatrixAPI *api,
                           MatrixAPICallback callback,
                           gpointer user_data,
                           const gchar *room_id,
                           GError **error);

/* Room participation */

void matrix_api_event_stream(MatrixAPI *api,
                             MatrixAPICallback callback,
                             gpointer user_data,
                             const gchar *from_token,
                             gulong timeout,
                             GError **error);
void matrix_api_get_event(MatrixAPI *api,
                          MatrixAPICallback callback,
                          gpointer user_data,
                          const gchar *event_id,
                          GError **error);
void matrix_api_initial_sync(MatrixAPI *api,
                             MatrixAPICallback callback,
                             gpointer user_data,
                             guint limit,
                             gboolean archived,
                             GError **error);
void matrix_api_get_event_context(MatrixAPI *api,
                                  MatrixAPICallback callback,
                                  gpointer user_data,
                                  const gchar *room_id,
                                  const gchar *event_id,
                                  guint limit,
                                  GError **error);
void matrix_api_initial_sync_room(MatrixAPI *api,
                                  MatrixAPICallback callback,
                                  gpointer user_data,
                                  const gchar *room_id,
                                  GError **error);
void matrix_api_list_room_members(MatrixAPI *api,
                                  MatrixAPICallback callback,
                                  gpointer user_data,
                                  const gchar *room_id,
                                  GError **error);
void matrix_api_list_room_messages(MatrixAPI *api,
                                   MatrixAPICallback callback,
                                   gpointer user_data,
                                   const gchar *room_id,
                                   const gchar *from_token,
                                   MatrixAPIEventDirection direction,
                                   guint limit,
                                   GError **error);
void matrix_api_send_event_receipt(MatrixAPI *api,
                                   MatrixAPICallback callback,
                                   gpointer user_data,
                                   const gchar *room_id,
                                   MatrixAPIReceiptType receipt_type,
                                   const gchar *event_id,
                                   JsonNode *receipt,
                                   GError **error);
void matrix_api_redact_event(MatrixAPI *api,
                             MatrixAPICallback callback,
                             gpointer user_data,
                             const gchar *room_id,
                             const gchar *event_id,
                             const gchar *txn_id,
                             const gchar *reason,
                             GError **error);
void matrix_api_send_message_event(MatrixAPI *api,
                                   MatrixAPICallback callback,
                                   gpointer user_data,
                                   const gchar *room_id,
                                   const gchar *event_type,
                                   const gchar *txn_id,
                                   JsonNode *content,
                                   GError **error);
void matrix_api_get_room_state(MatrixAPI *api,
                               MatrixAPICallback callback,
                               gpointer user_data,
                               const gchar *room_id,
                               const gchar *event_type,
                               const gchar *state_key,
                               GError **error);
void matrix_api_send_room_event(MatrixAPI *api,
                                MatrixAPICallback callback,
                                gpointer user_data,
                                const gchar *room_id,
                                const gchar *event_type,
                                const gchar *state_key,
                                const JsonNode *content,
                                GError **error);
void matrix_api_notify_room_typing(MatrixAPI *api,
                                   MatrixAPICallback callback,
                                   gpointer user_data,
                                   const gchar *user_id,
                                   const gchar *room_id,
                                   guint timeout,
                                   gboolean typing,
                                   GError **error);
void matrix_api_sync(MatrixAPI *api,
                     MatrixAPICallback callback,
                     gpointer user_data,
                     const gchar *filter_id,
                     const MatrixAPIFilter *filter,
                     const gchar *since,
                     gboolean full_state,
                     gboolean set_presence,
                     gulong timeout,
                     GError **error);
void matrix_api_create_filter(MatrixAPI *api,
                              MatrixAPICallback callback,
                              gpointer user_data,
                              const gchar *user_id,
                              MatrixAPIFilter *filter,
                              GError **error);
void matrix_api_download_filter(MatrixAPI *api,
                                MatrixAPICallback callback,
                                gpointer user_data,
                                const gchar *user_id,
                                const gchar *filter_id,
                                GError **error);

/* Search */

/* Add the search function here */

/* Server administration */

void matrix_api_whois(MatrixAPI *api,
                      MatrixAPICallback callback,
                      gpointer user_data,
                      const gchar *user_id,
                      GError **error);

/* Session management */

void matrix_api_login(MatrixAPI *api,
                      MatrixAPICallback callback,
                      gpointer user_data,
                      const gchar *type,
                      const JsonNode *content,
                      GError **error);
void matrix_api_token_refresh(MatrixAPI *api,
                              MatrixAPICallback callback,
                              gpointer user_data,
                              const gchar *refresh_token,
                              GError **error);

/* User data */

void matrix_api_get_3pids(MatrixAPI *api,
                          MatrixAPICallback callback,
                          gpointer user_data,
                          GError **error);
void matrix_api_add_3pid(MatrixAPI *api,
                         MatrixAPICallback callback,
                         gpointer user_data,
                         gboolean bind_creds,
                         MatrixAPI3PidCredential *threepid_creds,
                         GError **error);
void matrix_api_change_password(MatrixAPI *api,
                                MatrixAPICallback callback,
                                gpointer user_data,
                                const gchar *new_password,
                                GError **error);
void matrix_api_get_profile(MatrixAPI *api,
                            MatrixAPICallback callback,
                            gpointer user_data,
                            const gchar *user_id,
                            GError **error);
void matrix_api_get_avatar_url(MatrixAPI *api,
                               MatrixAPICallback callback,
                               gpointer user_data,
                               const gchar *user_id,
                               GError **error);
void matrix_api_set_avatar_url(MatrixAPI *api,
                               MatrixAPICallback callback,
                               gpointer user_data,
                               const gchar *user_id,
                               const gchar *avatar_url,
                               GError **error);
void matrix_api_get_display_name(MatrixAPI *api,
                                 MatrixAPICallback callback,
                                 gpointer user_data,
                                 const gchar *user_id,
                                 GError **error);
void matrix_api_set_display_name(MatrixAPI *api,
                                 MatrixAPICallback callback,
                                 gpointer user_data,
                                 const gchar *user_id,
                                 const gchar *display_name,
                                 GError **error);
void matrix_api_register_account(MatrixAPI *api,
                                 MatrixAPICallback callback,
                                 gpointer user_data,
                                 gboolean bind_email,
                                 const gchar *username,
                                 const gchar *password,
                                 GError **error);
void matrix_api_set_account_data(MatrixAPI *api,
                                 MatrixAPICallback callback,
                                 gpointer user_data,
                                 const gchar *user_id,
                                 const gchar *room_id,
                                 const gchar *type,
                                 const JsonNode *content,
                                 GError **error);
void matrix_api_get_room_tags(MatrixAPI *api,
                              MatrixAPICallback callback,
                              gpointer user_data,
                              const gchar *user_id,
                              const gchar *room_id,
                              GError **error);
void matrix_api_delete_room_tag(MatrixAPI *api,
                                MatrixAPICallback callback,
                                gpointer user_data,
                                const gchar *user_id,
                                const gchar *room_id,
                                const gchar *tag,
                                GError **error);
void matrix_api_add_room_tag(MatrixAPI *api,
                             MatrixAPICallback callback,
                             gpointer user_data,
                             const gchar *user_id,
                             const gchar *room_id,
                             const gchar *tag,
                             JsonNode *content,
                             GError **error);

/* VoIP */

void matrix_api_get_turn_server(MatrixAPI *api,
                                MatrixAPICallback callback,
                                gpointer user_data,
                                GError **error);

G_END_DECLS

#endif /* __MATRIX_API_IFACE_H__ */
