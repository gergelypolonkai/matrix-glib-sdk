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
                                const gchar *password,
                                GError **error);
    void (*register_with_password)(MatrixClient *client,
                                   const gchar *username,
                                   const gchar *password,
                                   GError **error);
    void (*logout)(MatrixClient *client, GError **error);

    void (*begin_polling)(MatrixClient *client, GError **error);
    void (*stop_polling)(MatrixClient *client,
                         gboolean cancel_ongoing,
                         GError **error);
};

struct _MatrixClientClass {
    /* Parent class */
    GObjectClass parent_class;
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
void matrix_client_stop_polling(MatrixClient *client,
                                gboolean cancel_ongoing,
                                GError **error);

G_END_DECLS

#endif /* __MATRIX_CLIENT_H__ */
