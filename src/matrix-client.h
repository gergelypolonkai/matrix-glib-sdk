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

#ifndef __MATRIX_CLIENT_H__
#define __MATRIX_CLIENT_H__

#include <glib-object.h>

G_BEGIN_DECLS

#define MATRIX_TYPE_CLIENT         (matrix_client_get_type())
#define MATRIX_CLIENT(o)           (G_TYPE_CHECK_INSTANCE_CAST((o), MATRIX_TYPE_CLIENT, MatrixClient))
#define MATRIX_IS_CLIENT(o)        (G_TYPE_CHECK_INSTANCE_TYPE((o), MATRIX_TYPE_CLIENT))
#define MATRIX_CLIENT_GET_IFACE(o) (G_TYPE_INSTANCE_GET_INTERFACE((o), MATRIX_TYPE_CLIENT, MatrixClientInterface))

typedef struct _MatrixClientInterface MatrixClientInterface;
typedef struct _MatrixClient          MatrixClient;

struct _MatrixClientInterface {
    /*< private >*/
    /* Parent instance structure */
    GTypeInterface g_iface;

    /*< public >*/
    /* Instance members */

    /* Virtual table */
    void (*login_with_password)(MatrixClient *client,
                                const gchar *username,
                                const gchar *password);
    void (*register_with_password)(MatrixClient *client,
                                   const gchar *username,
                                   const gchar *password);
    void (*logout)(MatrixClient *client);
    void (*refresh_token)(MatrixClient *client);

    void (*begin_polling)(MatrixClient *client);
    void (*stop_polling)(MatrixClient *client, gboolean cancel_ongoing);

    void (*get_room)(MatrixClient *client, const gchar *room_id_or_alias);
    void (*get_user)(MatrixClient *client, const gchar *user_id);
};

struct _MatrixClientClass {
    /* Parent class */
    GObjectClass parent_class;
};

GType matrix_client_get_type(void) G_GNUC_CONST;

void matrix_client_login_with_password(MatrixClient *client,
                                       const gchar *username,
                                       const gchar *password);
void matrix_client_register_with_password(MatrixClient *client,
                                          const gchar *username,
                                          const gchar *password);
void matrix_client_logout(MatrixClient *client);
void matrix_client_refresh_token(MatrixClient *client);

void matrix_client_begin_polling(MatrixClient *client);
void matrix_client_stop_polling(MatrixClient *client, gboolean cancel_ongoing);

void matrix_client_get_room(MatrixClient *client,
                            const gchar *room_id_or_alias);
void matrix_client_get_user(MatrixClient *client, const gchar *user_id);

G_END_DECLS

#endif /* __MATRIX_CLIENT_H__ */
