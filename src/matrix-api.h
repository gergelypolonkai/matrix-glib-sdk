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

#ifndef __MATRIX_GLIB_SDK_API_H__
# define __MATRIX_GLIB_SDK_API_H__

# include <glib-object.h>
# include <json-glib/json-glib.h>
# include "matrix-compacts.h"
# include "matrix-event-state-base.h"

G_BEGIN_DECLS

# ifdef __MATRIX_GLIB_SDK_COMPILATION
#  define MATRIX_DEPRECATED_FOR(f)
# else
#  define MATRIX_DEPRECATED_FOR(f) G_DEPRECATED_FOR(f)
# endif

# define MATRIX_TYPE_API matrix_api_get_type()
G_DECLARE_INTERFACE(MatrixAPI, matrix_api, MATRIX, API, GObject);

typedef void (*MatrixAPICallback)(MatrixAPI *api,
                                  const gchar *content_type,
                                  JsonNode *json_content,
                                  GByteArray *raw_content,
                                  GError *err,
                                  gpointer user_data);

struct _MatrixAPIInterface {
    GTypeInterface parent_iface;
    void (*abort_pending)(MatrixAPI *api);
    void (*get_3pids)(MatrixAPI *api,
                      MatrixAPICallback callback,
                      gpointer user_data,
                      GError **error);
    void (*add_3pid)(MatrixAPI *api,
                     MatrixAPICallback callback,
                     gpointer user_data,
                     gboolean bind_creds,
                     Matrix3PidCredential *threepid_creds,
                     GError **error);
    void (*deactivate_account)(MatrixAPI *api,
                               MatrixAPICallback callback,
                               gpointer user_data,
                               const gchar *session,
                               const gchar *login_type,
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
                             MatrixAccountKind account_kind,
                             gboolean bind_email,
                             const gchar *username,
                             const gchar *password,
                             GError **error);
    void (*set_account_data)(MatrixAPI *api,
                             MatrixAPICallback callback,
                             gpointer user_data,
                             const gchar *user_id,
                             const gchar *room_id,
                             const gchar *event_type,
                             JsonNode *content,
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
                         JsonNode *content,
                         GError **error);
    void (*whois)(MatrixAPI *api,
                  MatrixAPICallback callback,
                  gpointer user_data,
                  const gchar *user_id,
                  GError **error);
    void (*versions)(MatrixAPI *api,
                     MatrixAPICallback callback,
                     gpointer user_data,
                     GError **error);
    void (*create_room)(MatrixAPI *api,
                        MatrixAPICallback callback,
                        gpointer user_data,
                        MatrixRoomPreset preset,
                        const gchar *room_name,
                        const gchar *room_alias,
                        const gchar *topic,
                        MatrixRoomVisibility visibility,
                        JsonNode *creation_content,
                        MatrixEventState **initial_state,
                        int initial_state_length1,
                        gchar **invitees,
                        int invitees_length1,
                        Matrix3PidCredential **invite_3pids,
                        int invite_3pids_length1,
                        GError **error);
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
                               MatrixEventDirection direction,
                               guint limit,
                               GError **error);
    void (*send_event_receipt)(MatrixAPI *api,
                               MatrixAPICallback callback,
                               gpointer user_data,
                               const gchar *room_id,
                               MatrixReceiptType receipt_type,
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
    void (*send_event)(MatrixAPI *api,
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
    void (*send_state_event)(MatrixAPI *api,
                             MatrixAPICallback callback,
                             gpointer user_data,
                             const gchar *room_id,
                             const gchar *event_type,
                             const gchar *state_key,
                             JsonNode *content,
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
                 MatrixFilter *filter,
                 const gchar *since,
                 gboolean full_state,
                 gboolean set_presence,
                 gulong timeout,
                 GError **error);
    void (*create_filter)(MatrixAPI *api,
                          MatrixAPICallback callback,
                          gpointer user_data,
                          const gchar *user_id,
                          MatrixFilter *filter,
                          GError **error);
    void (*download_filter)(MatrixAPI *api,
                            MatrixAPICallback callback,
                            gpointer user_data,
                            const gchar *user_id,
                            const gchar *filter_id,
                            GError **error);
    void (*join_room_id_or_alias)(MatrixAPI *api,
                                  MatrixAPICallback callback,
                                  gpointer user_data,
                                  const gchar *room_id_or_alias,
                                  GError **error);
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
                                 Matrix3PidCredential *credential,
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
    void (*kick_user)(MatrixAPI *api,
                      MatrixAPICallback callback,
                      gpointer user_data,
                      const gchar *room_id,
                      const gchar *user_id,
                      const gchar *reason,
                      GError **error);
    void (*leave_room)(MatrixAPI *api,
                       MatrixAPICallback callback,
                       gpointer user_data,
                       const gchar *room_id,
                       GError **error);
    void (*unban_user)(MatrixAPI *api,
                       MatrixAPICallback callback,
                       gpointer user_data,
                       const gchar *room_id,
                       const gchar *user_id,
                       GError **error);
    void (*login)(MatrixAPI *api,
                  MatrixAPICallback callback,
                  gpointer user_data,
                  const gchar *login_type,
                  JsonNode *content,
                  GError **error);
    void (*logout)(MatrixAPI *api,
                   MatrixAPICallback callback,
                   gpointer user_data,
                   GError **error);
    void (*token_refresh)(MatrixAPI *api,
                          MatrixAPICallback callback,
                          gpointer user_data,
                          const gchar *refresh_token,
                          GError **error);
    void (*get_presence_list)(MatrixAPI *api,
                              MatrixAPICallback callback,
                              gpointer user_data,
                              const gchar *user_id,
                              GError **error);
    void (*update_presence_list)(MatrixAPI *api,
                                 MatrixAPICallback callback,
                                 gpointer user_data,
                                 const gchar *user_id,
                                 gchar **drop_ids,
                                 int drop_ids_length1,
                                 gchar **invite_ids,
                                 int invite_ids_length1,
                                 GError **error);
    void (*get_presence)(MatrixAPI *api,
                         MatrixAPICallback callback,
                         gpointer user_data,
                         const gchar *user_id,
                         GError **error);
    void (*set_presence)(MatrixAPI *api,
                         MatrixAPICallback callback,
                         gpointer user_data,
                         const gchar *user_id,
                         MatrixPresence presence,
                         const gchar *status_message,
                         GError **error);
    void (*list_public_rooms)(MatrixAPI *api,
                              MatrixAPICallback callback,
                              gpointer user_data,
                              GError **error);
    void (*get_pushers)(MatrixAPI *api,
                        MatrixAPICallback callback,
                        gpointer user_data,
                        GError **error);
    void (*update_pusher)(MatrixAPI *api,
                          MatrixAPICallback callback,
                          gpointer user_data,
                          MatrixPusher *pusher,
                          GError **error);
    void (*get_pushrules)(MatrixAPI *api,
                          MatrixAPICallback callback,
                          gpointer user_data,
                          GError **error);
    void (*delete_pushrule)(MatrixAPI *api,
                            MatrixAPICallback callback,
                            gpointer user_data,
                            const gchar *scope,
                            MatrixPusherKind kind,
                            const gchar *rule_id,
                            GError **error);
    void (*get_pushrule)(MatrixAPI *api,
                         MatrixAPICallback callback,
                         gpointer user_data,
                         const gchar *scope,
                         MatrixPusherKind kind,
                         const gchar *rule_id,
                         GError **error);
    void (*add_pushrule)(MatrixAPI *api,
                         MatrixAPICallback callback,
                         gpointer user_data,
                         const gchar *scope,
                         MatrixPusherKind kind,
                         const gchar *rule_id,
                         const gchar *before,
                         const gchar *after,
                         gchar **actions,
                         int actions_length1,
                         MatrixPusherConditionKind *conditions,
                         int conditions_length1,
                         GError **error);
    void (*toggle_pushrule)(MatrixAPI *api,
                            MatrixAPICallback callback,
                            gpointer user_data,
                            const gchar *scope,
                            MatrixPusherKind kind,
                            const gchar *rule_id,
                            gboolean enabled,
                            GError **error);
    void (*search)(MatrixAPI *api,
                   MatrixAPICallback callback,
                   gpointer user_data,
                   const gchar *next_batch,
                   MatrixSearchCategories *search_categories,
                   GError **error);
    void (*get_turn_server)(MatrixAPI *api,
                            MatrixAPICallback callback,
                            gpointer user_data,
                            GError **error);
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
                            MatrixResizeMethod method,
                            GError **error);
    void (*media_upload)(MatrixAPI *api,
                         MatrixAPICallback callback,
                         gpointer user_data,
                         const gchar *content_type,
                         GByteArray *content,
                         GError **error);
    const gchar *(*get_token)(MatrixAPI *api);
    void (*set_token)(MatrixAPI *api, const gchar *value);
    const gchar *(*get_refresh_token)(MatrixAPI *api);
    void (*set_refresh_token)(MatrixAPI *api, const gchar *value);
    const gchar *(*get_user_id)(MatrixAPI *api);
    const gchar *(*get_homeserver)(MatrixAPI *api);
};

void matrix_api_abort_pending(MatrixAPI *api);
void matrix_api_get_3pids(MatrixAPI *api,
                          MatrixAPICallback callback,
                          gpointer user_data,
                          GError **error);
void matrix_api_add_3pid(MatrixAPI *api,
                         gboolean bind_creds,
                         Matrix3PidCredential *threepid_creds,
                         MatrixAPICallback callback,
                         gpointer user_data,
                         GError **error);
void matrix_api_deactivate_account(MatrixAPI *api,
                                   const gchar *session,
                                   const gchar *login_type,
                                   MatrixAPICallback callback,
                                   gpointer user_data,
                                   GError **error);
void matrix_api_change_password(MatrixAPI *api,
                                const gchar *new_password,
                                MatrixAPICallback callback,
                                gpointer user_data,
                                GError **error);
void matrix_api_get_profile(MatrixAPI *api,
                            const gchar *user_id,
                            MatrixAPICallback callback,
                            gpointer user_data,
                            GError **error);
void matrix_api_get_avatar_url(MatrixAPI *api,
                               const gchar *user_id,
                               MatrixAPICallback callback,
                               gpointer user_data,
                               GError **error);
void matrix_api_set_avatar_url(MatrixAPI *api,
                               const gchar *user_id,
                               const gchar *avatar_url,
                               MatrixAPICallback callback,
                               gpointer user_data,
                               GError **error);
void matrix_api_get_display_name(MatrixAPI *api,
                                 const gchar *user_id,
                                 MatrixAPICallback callback,
                                 gpointer user_data,
                                 GError **error);
void matrix_api_set_display_name(MatrixAPI *api,
                                 const gchar *user_id,
                                 const gchar *display_name,
                                 MatrixAPICallback callback,
                                 gpointer user_data,
                                 GError **error);
void matrix_api_register_account(MatrixAPI *api,
                                 MatrixAccountKind account_kind,
                                 gboolean bind_email,
                                 const gchar *username,
                                 const gchar *password,
                                 MatrixAPICallback callback,
                                 gpointer user_data,
                                 GError **error);
void matrix_api_set_account_data(MatrixAPI *api,
                                 const gchar *user_id,
                                 const gchar *room_id,
                                 const gchar *event_type,
                                 JsonNode *content,
                                 MatrixAPICallback callback,
                                 gpointer user_data,
                                 GError **error);
void matrix_api_get_room_tags(MatrixAPI *api,
                              const gchar *user_id,
                              const gchar *room_id,
                              MatrixAPICallback callback,
                              gpointer user_data,
                              GError **error);
void matrix_api_delete_room_tag(MatrixAPI *api,
                                const gchar *user_id,
                                const gchar *room_id,
                                const gchar *tag,
                                MatrixAPICallback callback,
                                gpointer user_data,
                                GError **error);
void matrix_api_add_room_tag(MatrixAPI *api,
                             const gchar *user_id,
                             const gchar *room_id,
                             const gchar *tag,
                             JsonNode *content,
                             MatrixAPICallback callback,
                             gpointer user_data,
                             GError **error);
void matrix_api_whois(MatrixAPI *api,
                      const gchar *user_id,
                      MatrixAPICallback callback,
                      gpointer user_data,
                      GError **error);
void matrix_api_versions(MatrixAPI *api,
                         MatrixAPICallback callback,
                         gpointer user_data,
                         GError **error);
void matrix_api_create_room(MatrixAPI *api,
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
                            GError **error);
void matrix_api_delete_room_alias(MatrixAPI *api,
                                  const gchar *room_alias,
                                  MatrixAPICallback callback,
                                  gpointer user_data,
                                  GError **error);
void matrix_api_get_room_id(MatrixAPI *api,
                            const gchar *room_alias,
                            MatrixAPICallback callback,
                            gpointer user_data,
                            GError **error);
void matrix_api_create_room_alias(MatrixAPI *api,
                                  const gchar *room_id,
                                  const gchar *room_alias,
                                  MatrixAPICallback callback,
                                  gpointer user_data,
                                  GError **error);
void matrix_api_event_stream(MatrixAPI *api,
                             const gchar *from_token,
                             gulong timeout,
                             MatrixAPICallback callback,
                             gpointer user_data,
                             GError **error)
    MATRIX_DEPRECATED_FOR(matrix_api_sync);
void matrix_api_get_event(MatrixAPI *api,
                          const gchar *event_id,
                          MatrixAPICallback callback,
                          gpointer user_data,
                          GError **error)
    MATRIX_DEPRECATED_FOR(matrix_api_sync);
void matrix_api_initial_sync(MatrixAPI *api,
                             guint limit,
                             gboolean archived,
                             MatrixAPICallback callback,
                             gpointer user_data,
                             GError **error)
    MATRIX_DEPRECATED_FOR(matrix_api_sync);
void matrix_api_get_event_context(MatrixAPI *api,
                                  const gchar *room_id,
                                  const gchar *event_id,
                                  guint limit,
                                  MatrixAPICallback callback,
                                  gpointer user_data,
                                  GError **error);
void matrix_api_initial_sync_room(MatrixAPI *api,
                                  const gchar *room_id,
                                  MatrixAPICallback callback,
                                  gpointer user_data,
                                  GError **error)
    MATRIX_DEPRECATED_FOR(matrix_api_sync);
void matrix_api_list_room_members(MatrixAPI *api,
                                  const gchar *room_id,
                                  MatrixAPICallback callback,
                                  gpointer user_data,
                                  GError **error);
void matrix_api_list_room_messages(MatrixAPI *api,
                                   const gchar *room_id,
                                   const gchar *from_token,
                                   MatrixEventDirection direction,
                                   guint limit,
                                   MatrixAPICallback callback,
                                   gpointer user_data,
                                   GError **error);
void matrix_api_send_event_receipt(MatrixAPI *api,
                                   const gchar *room_id,
                                   MatrixReceiptType receipt_type,
                                   const gchar *event_id,
                                   JsonNode *receipt,
                                   MatrixAPICallback callback,
                                   gpointer user_data,
                                   GError **error);
void matrix_api_redact_event(MatrixAPI *api,
                             const gchar *room_id,
                             const gchar *event_id,
                             const gchar *txn_id,
                             const gchar *reason,
                             MatrixAPICallback callback,
                             gpointer user_data,
                             GError **error);
void matrix_api_send_event(MatrixAPI *api,
                           const gchar *room_id,
                           const gchar *event_type,
                           const gchar *txn_id,
                           JsonNode *content,
                           MatrixAPICallback callback,
                           gpointer user_data,
                           GError **error);
void matrix_api_get_room_state(MatrixAPI *api,
                               const gchar *room_id,
                               const gchar *event_type,
                               const gchar *state_key,
                               MatrixAPICallback callback,
                               gpointer user_data,
                               GError **error);
void matrix_api_send_state_event(MatrixAPI *api,
                                 const gchar *room_id,
                                 const gchar *event_type,
                                 const gchar *state_key,
                                 JsonNode *content,
                                 MatrixAPICallback callback,
                                 gpointer user_data,
                                 GError **error);
void matrix_api_notify_room_typing(MatrixAPI *api,
                                   const gchar *user_id,
                                   const gchar *room_id,
                                   guint timeout,
                                   gboolean typing,
                                   MatrixAPICallback callback,
                                   gpointer user_data,
                                   GError **error);
void matrix_api_sync(MatrixAPI *api,
                     const gchar *filter_id,
                     MatrixFilter *filter,
                     const gchar *since,
                     gboolean full_state,
                     gboolean set_presence,
                     gulong timeout,
                     MatrixAPICallback callback,
                     gpointer user_data,
                     GError **error);
void matrix_api_create_filter(MatrixAPI *api,
                              const gchar *user_id,
                              MatrixFilter *filter,
                              MatrixAPICallback callback,
                              gpointer user_data,
                              GError **error);
void matrix_api_download_filter(MatrixAPI *api,
                                const gchar *user_id,
                                const gchar *filter_id,
                                MatrixAPICallback callback,
                                gpointer user_data,
                                GError **error);
void matrix_api_join_room_id_or_alias(MatrixAPI *api,
                                      const gchar *room_id_or_alias,
                                      MatrixAPICallback callback,
                                      gpointer user_data,
                                      GError **error);
void matrix_api_ban_user(MatrixAPI *api,
                         const gchar *room_id,
                         const gchar *user_id,
                         const gchar *reason,
                         MatrixAPICallback callback,
                         gpointer user_data,
                         GError **error);
void matrix_api_forget_room(MatrixAPI *api,
                            const gchar *room_id,
                            MatrixAPICallback callback,
                            gpointer user_data,
                            GError **error);
void matrix_api_invite_user_3rdparty(MatrixAPI *api,
                                     const gchar *room_id,
                                     Matrix3PidCredential *credential,
                                     MatrixAPICallback callback,
                                     gpointer user_data,
                                     GError **error);
void matrix_api_invite_user(MatrixAPI *api,
                            const gchar *room_id,
                            const gchar *user_id,
                            MatrixAPICallback callback,
                            gpointer user_data,
                            GError **error);
void matrix_api_join_room(MatrixAPI *api,
                          const gchar *room_id,
                          MatrixAPICallback callback,
                          gpointer user_data,
                          GError **error);
void matrix_api_kick_user(MatrixAPI *api,
                          const gchar *room_id,
                          const gchar *user_id,
                          const gchar *reason,
                          MatrixAPICallback callback,
                          gpointer user_data,
                          GError **error);
void matrix_api_leave_room(MatrixAPI *api,
                           const gchar *room_id,
                           MatrixAPICallback callback,
                           gpointer user_data,
                           GError **error);
void matrix_api_unban_user(MatrixAPI *api,
                           const gchar *room_id,
                           const gchar *user_id,
                           MatrixAPICallback callback,
                           gpointer user_data,
                           GError **error);
void matrix_api_login(MatrixAPI *api,
                      const gchar *login_type,
                      JsonNode *content,
                      MatrixAPICallback callback,
                      gpointer user_data,
                      GError **error);
void matrix_api_logout(MatrixAPI *api,
                       MatrixAPICallback callback,
                       gpointer user_data,
                       GError **error);
void matrix_api_token_refresh(MatrixAPI *api,
                              const gchar *refresh_token,
                              MatrixAPICallback callback,
                              gpointer user_data,
                              GError **error);
void matrix_api_get_presence_list(MatrixAPI *api,
                                  const gchar *user_id,
                                  MatrixAPICallback callback,
                                  gpointer user_data,
                                  GError **error);
void matrix_api_update_presence_list(MatrixAPI *api,
                                     const gchar *user_id,
                                     gchar **drop_ids,
                                     int n_drop_ids,
                                     gchar **invite_ids,
                                     int n_invite_ids,
                                     MatrixAPICallback callback,
                                     gpointer user_data,
                                     GError **error);
void matrix_api_get_presence(MatrixAPI *api,
                             const gchar *user_id,
                             MatrixAPICallback callback,
                             gpointer user_data,
                             GError **error);
void matrix_api_set_presence(MatrixAPI *api,
                             const gchar *user_id,
                             MatrixPresence presence,
                             const gchar *status_message,
                             MatrixAPICallback callback,
                             gpointer user_data,
                             GError **error);
void matrix_api_list_public_rooms(MatrixAPI *api,
                                  MatrixAPICallback callback,
                                  gpointer user_data,
                                  GError **error);
void matrix_api_get_pushers(MatrixAPI *api,
                            MatrixAPICallback callback,
                            gpointer user_data,
                            GError **error);
void matrix_api_update_pusher(MatrixAPI *api,
                              MatrixPusher *pusher,
                              MatrixAPICallback callback,
                              gpointer user_data,
                              GError **error);
void matrix_api_get_pushrules(MatrixAPI *api,
                              MatrixAPICallback callback,
                              gpointer user_data,
                              GError **error);
void matrix_api_delete_pushrule(MatrixAPI *api,
                                const gchar *scope,
                                MatrixPusherKind kind,
                                const gchar *rule_id,
                                MatrixAPICallback callback,
                                gpointer user_data,
                                GError **error);
void matrix_api_get_pushrule(MatrixAPI *api,
                             const gchar *scope,
                             MatrixPusherKind kind,
                             const gchar *rule_id,
                             MatrixAPICallback callback,
                             gpointer user_data,
                             GError **error);
void matrix_api_add_pushrule(MatrixAPI *api,
                             const gchar *scope,
                             MatrixPusherKind kind,
                             const gchar *rule_id,
                             const gchar *before,
                             const gchar *after,
                             gchar **actions,
                             int n_actions,
                             MatrixPusherConditionKind *conditions,
                             int n_conditions,
                             MatrixAPICallback callback,
                             gpointer user_data,
                             GError **error);
void matrix_api_toggle_pushrule(MatrixAPI *api,
                                const gchar *scope,
                                MatrixPusherKind kind,
                                const gchar *rule_id,
                                gboolean enabled,
                                MatrixAPICallback callback,
                                gpointer user_data,
                                GError **error);
void matrix_api_search(MatrixAPI *api,
                       const gchar *next_batch,
                       MatrixSearchCategories *search_categories,
                       MatrixAPICallback callback,
                       gpointer user_data,
                       GError **error);
void matrix_api_get_turn_server(MatrixAPI *api,
                                MatrixAPICallback callback,
                                gpointer user_data,
                                GError **error);
void matrix_api_media_download(MatrixAPI *api,
                               const gchar *server_name,
                               const gchar *media_id,
                               MatrixAPICallback callback,
                               gpointer user_data,
                               GError **error);
void matrix_api_media_thumbnail(MatrixAPI *api,
                                const gchar *server_name,
                                const gchar *media_id,
                                guint width,
                                guint height,
                                MatrixResizeMethod method,
                                MatrixAPICallback callback,
                                gpointer user_data,
                                GError **error);
void matrix_api_media_upload(MatrixAPI *api,
                             const gchar *content_type,
                             GByteArray *content,
                             MatrixAPICallback callback,
                             gpointer user_data,
                             GError **error);
const gchar *matrix_api_get_token(MatrixAPI *api);
void matrix_api_set_token(MatrixAPI *api, const gchar *token);
const gchar *matrix_api_get_refresh_token(MatrixAPI *api);
void matrix_api_set_refresh_token(MatrixAPI *api, const gchar *refresh_token);
const gchar *matrix_api_get_user_id(MatrixAPI *api);
const gchar *matrix_api_get_homeserver(MatrixAPI *api);

G_END_DECLS

#endif  /* __MATRIX_GLIB_SDK_API_H__ */
