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

G_BEGIN_DECLS

#define MATRIX_TYPE_API         (matrix_api_get_type())
#define MATRIX_API(o)           (G_TYPE_CHECK_INSTANCE_CAST((o), MATRIX_TYPE_API, MatrixAPI))
#define MATRIX_IS_API(o)        (G_TYPE_CHECK_INSTANCE_TYPE((o), MATRIX_TYPE_API))
#define MATRIX_API_GET_IFACE(o) (G_TYPE_INSTANCE_GET_INTERFACE((o), MATRIX_TYPE_API, MatrixAPIInterface))

typedef struct _MatrixAPIInterface MatrixAPIInterface;
typedef struct _MatrixAPI          MatrixAPI;

typedef void (*MatrixAPICallback)(MatrixAPI *api, JsonNode *content, gpointer data);

struct _MatrixAPIInterface {
    /*< private >*/
    GTypeInterface g_iface;

    /*< public >*/

   void (*register_account)(MatrixAPI *api,
                             MatrixAPICallback callback,
                             gpointer user_data,
                             gchar *login_type,
                             GHashTable *parameters);
    void (*login)(MatrixAPI *api,
                  MatrixAPICallback callback,
                  gpointer user_data,
                  gchar *login_type,
                  GHashTable *parameters);
    void (*initial_sync)(MatrixAPI *api,
                         MatrixAPICallback callback,
                         gpointer user_data,
                         guint limit);
    void (*event_stream)(MatrixAPI *api,
                         MatrixAPICallback callback,
                         gpointer user_data,
                         gchar *from_token,
                         gulong timeout);
    void (*create_room)(MatrixAPI *api,
                        MatrixAPICallback callback,
                        gpointer user_data,
                        gchar *room_alias,
                        gboolean is_public,
                        GStrv invitees);
    void (*join_room)(MatrixAPI *api,
                      MatrixAPICallback callback,
                      gpointer user_data,
                      gchar *room_id_or_alias);
    void (*send_state_event)(MatrixAPI *api,
                             MatrixAPICallback callback,
                             gpointer user_data,
                             gchar *room_id,
                             gchar *event_type,
                             JsonNode *content,
                             gchar *state_key);
    void (*send_message_event)(MatrixAPI *api,
                               MatrixAPICallback callback,
                               gpointer user_data,
                               gchar *room_id,
                               gchar *event_type,
                               JsonNode *content);
    void (*send_message)(MatrixAPI *api,
                         MatrixAPICallback callback,
                         gpointer user_data,
                         gchar *room_id,
                         gchar *text_content,
                         gchar *msg_type);
    void (*send_emote)(MatrixAPI *api,
                       MatrixAPICallback callback,
                       gpointer user_data,
                       gchar *room_id,
                       gchar *text_content);
    void (*get_room_name)(MatrixAPI *api,
                          MatrixAPICallback callback,
                          gpointer user_data,
                          gchar *room_id);
    void (*get_room_topic)(MatrixAPI *api,
                           MatrixAPICallback callback,
                           gpointer user_data,
                           gchar *room_id);
    void (*leave_room)(MatrixAPI *api,
                       MatrixAPICallback callback,
                       gpointer user_data,
                       gchar *room_id);
    void (*invite_user)(MatrixAPI *api,
                        MatrixAPICallback callback,
                        gpointer user_data,
                        gchar *room_id,
                        gchar *user_id);
    void (*kick_user)(MatrixAPI *api,
                      MatrixAPICallback callback,
                      gpointer user_data,
                      gchar *room_id,
                      gchar *user_id,
                      gchar *reason);
    void (*set_membership)(MatrixAPI *api,
                           MatrixAPICallback callback,
                           gpointer user_data,
                           gchar *room_id,
                           gchar *user_id,
                           gchar *membership,
                           gchar *reason);
    void (*ban_user)(MatrixAPI *api,
                     MatrixAPICallback callback,
                     gpointer user_data,
                     gchar *room_id,
                     gchar *user_id,
                     gchar *reason);
    void (*get_room_state)(MatrixAPI *api,
                           MatrixAPICallback callback,
                           gpointer user_data,
                           gchar *room_id);
    void (*get_text_body)(MatrixAPI *api,
                          MatrixAPICallback callback,
                          gpointer user_data,
                          gchar *text,
                          gchar *msgtype);
    void (*get_html_body)(MatrixAPI *api,
                          MatrixAPICallback callback,
                          gpointer user_data,
                          gchar *html,
                          gchar *msgtype);
    void (*get_emote_body)(MatrixAPI *api,
                           MatrixAPICallback callback,
                           gpointer user_data,
                           gchar *text);
    void (*_send)(MatrixAPI *api,
                  MatrixAPICallback callback,
                  gpointer user_data,
                  gchar *method,
                  gchar *path,
                  gchar *content,
                  gchar *query_params,
                  gchar *headers);

    /*< private >*/
    void *padding[20];
};

void matrix_api_initial_sync(MatrixAPI *api,
                             MatrixAPICallback callback,
                             gpointer user_data,
                             guint limit);
void matrix_api_register_account(MatrixAPI *api,
                                 MatrixAPICallback callback,
                                 gpointer user_data,
                                 gchar *login_type,
                                 GHashTable *parameters);
void matrix_api_login(MatrixAPI *api,
                      MatrixAPICallback callback,
                      gpointer user_data,
                      gchar *login_type,
                      GHashTable *parameters);
void matrix_api_create_room(MatrixAPI *api,
                            MatrixAPICallback callback,
                            gpointer user_data,
                            gchar *alias,
                            gboolean is_public,
                            GStrv invitees);
void matrix_api_join_room(MatrixAPI *api,
                          MatrixAPICallback callback,
                          gpointer user_data,
                          gchar *room_id_or_alias);
void matrix_api_event_stream(MatrixAPI *api,
                             MatrixAPICallback callback,
                             gpointer user_data,
                             gchar *from_token,
                             gulong timeout);
void matrix_api_send_state_event(MatrixAPI *api,
                                 MatrixAPICallback callback,
                                 gpointer user_data,
                                 gchar *room_id,
                                 gchar *event_type,
                                 JsonNode *content,
                                 gchar *state_key);
void matrix_api_send_message_event(MatrixAPI *api,
                                   MatrixAPICallback callback,
                                   gpointer user_data,
                                   gchar *room_id,
                                   gchar *event_type,
                                   JsonNode *content);
void matrix_api_send_message(MatrixAPI *api,
                             MatrixAPICallback callback,
                             gpointer user_data,
                             gchar *room_id,
                             gchar *text_content,
                             gchar *msg_type);
void matrix_api_send_emote(MatrixAPI *api,
                           MatrixAPICallback callback,
                           gpointer user_data,
                           gchar *room_id,
                           gchar *text_content);
void matrix_api_get_room_name(MatrixAPI *api,
                              MatrixAPICallback callback,
                              gpointer user_data,
                              gchar *room_id);
void matrix_api_get_room_topic(MatrixAPI *api,
                               MatrixAPICallback callback,
                               gpointer user_data,
                               gchar *room_id);
void matrix_api_leave_room(MatrixAPI *api,
                           MatrixAPICallback callback,
                           gpointer user_data,
                           gchar *room_id);
void matrix_api_invite_user(MatrixAPI *api,
                            MatrixAPICallback callback,
                            gpointer user_data,
                            gchar *room_id,
                            gchar *user_id);
void matrix_api_kick_user(MatrixAPI *api,
                          MatrixAPICallback callback,
                          gpointer user_data,
                          gchar *room_id,
                          gchar *user_id,
                          gchar *reason);
void matrix_api_set_membership(MatrixAPI *api,
                               MatrixAPICallback callback,
                               gpointer user_data,
                               gchar *room_id,
                               gchar *user_id,
                               gchar *membership,
                               gchar *reason);
void matrix_api_ban_user(MatrixAPI *api,
                         MatrixAPICallback callback,
                         gpointer user_data,
                         gchar *room_id,
                         gchar *user_id,
                         gchar *reason);
void matrix_api_get_room_state(MatrixAPI *api,
                               MatrixAPICallback callback,
                               gpointer user_data,
                               gchar *room_id);
void matrix_api_get_text_body(MatrixAPI *api, gchar *text, gchar *msgtype);
void matrix_api_get_html_body(MatrixAPI *api, gchar *html, gchar *msgtype);
void matrix_api_get_emote_body(MatrixAPI *api, gchar *text);
void _send(MatrixAPI *api,
           gchar *method,
           gchar *path,
           gchar *content,
           GHashTable *query_params,
           GHashTable *headers);

G_END_DECLS

#endif /* __MATRIX_API_IFACE_H__ */
