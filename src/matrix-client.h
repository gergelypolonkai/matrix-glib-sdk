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

#ifndef __MATRIX_GLIB_SDK_CLIENT_H__
# define __MATRIX_GLIB_SDK_CLIENT_H__

# include <glib-object.h>
# include "matrix-enumtypes.h"
# include "matrix-profile.h"
# include "matrix-room.h"
# include "matrix-event-base.h"

G_BEGIN_DECLS

# define MATRIX_TYPE_CLIENT matrix_client_get_type()
G_DECLARE_INTERFACE(MatrixClient, matrix_client, MATRIX, CLIENT, GObject);

typedef void (*MatrixClientSendCallback)(MatrixClient *client, const gchar *event_id, GError *error, void *user_data);
typedef void (*MatrixClientEventCallback)(MatrixClient *client, const gchar *room_id, JsonNode *raw_event, MatrixEventBase *matrix_event, void *user_data);

struct _MatrixClientInterface {
    GTypeInterface parent_iface;
    void (*login_with_password)(MatrixClient *client,
                                const gchar *username,
                                const gchar *password,
                                GError **error);
    void (*register_with_password)(MatrixClient *client,
                                   const gchar *username,
                                   const gchar *password,
                                   GError **error);
    void (*logout)(MatrixClient *client, GError **error);
    void (*begin_polling)(MatrixClient *client, GError **error);
    void (*stop_polling)(MatrixClient *client, gboolean cancel_ongoing, GError **error);
    MatrixProfile *(*get_user_profile)(MatrixClient *client,
                                       const gchar *user_id,
                                       const gchar *room_id,
                                       GError **error);
    MatrixPresence (*get_user_presence)(MatrixClient *client,
                                        const gchar *user_id,
                                        const gchar *room_id,
                                        GError **error);
    MatrixRoom *(*get_room_by_id)(MatrixClient *client, const gchar *room_id, GError **error);
    MatrixRoom *(*get_room_by_alias)(MatrixClient *client,
                                     const gchar *room_alias,
                                     GError **error);
    void (*send)(MatrixClient *client,
                 const gchar *room_id,
                 MatrixEventBase *evt,
                 MatrixClientSendCallback cb, void *cb_target,
                 gulong txn_id,
                 GError **error);
    void (*save_state)(MatrixClient *client,
                       const gchar *filename,
                       GError **error);
    void (*load_state)(MatrixClient *client,
                       const gchar *filename,
                       GError **error);
    void (*login_finished)(MatrixClient *client, gboolean success);
    void (*event)(MatrixClient *client,
                  const gchar *room_id,
                  JsonNode *raw_event,
                  MatrixEventBase *matrix_event);
};

GType matrix_client_get_type(void) G_GNUC_CONST;
void matrix_client_login_with_password(MatrixClient *client,
                                       const gchar *username,
                                       const gchar *password,
                                       GError **error);
void matrix_client_register_with_password(MatrixClient *client,
                                          const gchar *username,
                                          const gchar *password,
                                          GError **error);
void matrix_client_logout(MatrixClient *client, GError **error);
void matrix_client_begin_polling(MatrixClient *client, GError **error);
void matrix_client_stop_polling(MatrixClient *client, gboolean cancel_ongoing, GError **error);
void matrix_client_emit_login_finished(MatrixClient *client, gboolean success);
void matrix_client_incoming_event(MatrixClient *client,
                                  const gchar *room_id,
                                  JsonNode *raw_event,
                                  MatrixEventBase *matrix_event);
void matrix_client_connect_event(MatrixClient *client,
                                 GType event_gtype,
                                 MatrixClientEventCallback event_callback,
                                 gpointer user_data,
                                 GDestroyNotify destroy_notify);
MatrixProfile *matrix_client_get_user_profile(MatrixClient *client,
                                              const gchar *user_id,
                                              const gchar *room_id,
                                              GError **error);
MatrixPresence matrix_client_get_user_presence(MatrixClient *client,
                                               const gchar *user_id,
                                               const gchar *room_id,
                                               GError **error);
MatrixRoom *matrix_client_get_room_by_id(MatrixClient *client,
                                         const gchar *room_id,
                                         GError **error);
MatrixRoom *matrix_client_get_room_by_alias(MatrixClient *client,
                                            const gchar *room_alias,
                                            GError **error);
void matrix_client_send(MatrixClient *client,
                        const gchar *room_id,
                        MatrixEventBase *evt,
                        gulong txn_id,
                        MatrixClientSendCallback callback,
                        gpointer user_data,
                        GError **error);
void matrix_client_save_state(MatrixClient *client, const gchar *filename, GError **error);
void matrix_client_load_state(MatrixClient *client, const gchar *filename, GError **error);
void matrix_client_connect_event(MatrixClient *client,
                                 GType event_gtype,
                                 MatrixClientEventCallback callback,
                                 gpointer user_data,
                                 GDestroyNotify destroy_notify);

G_END_DECLS

#endif  /*__MATRIX_GLIB_SDK_CLIENT_H__ */
