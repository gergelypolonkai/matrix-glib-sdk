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

#include <glib.h>
#include <glib/gprintf.h>

#include "matrix-glib.h"

static gchar *user = NULL;
static gchar *password = NULL;
static gboolean no_validate_certs = FALSE;
static gchar **homeserver = NULL;

static GOptionEntry entries[] = {
    {"user", 'u', 0, G_OPTION_ARG_STRING, &user},
    {"password", 'p', 0, G_OPTION_ARG_STRING, &password},
    {"no-validate-certs", 'n', 0, G_OPTION_ARG_NONE, &no_validate_certs},
    {G_OPTION_REMAINING, 0, 0, G_OPTION_ARG_FILENAME_ARRAY, &homeserver, "The homeserver to connect to", "homeserver"},
};

void
login_finished(MatrixClient *client, gboolean success, GMainLoop *loop)
{
    g_printerr("Login %s.\n", (success) ? "succesful" : "failed");

    if (success) {
        matrix_client_begin_polling(client, NULL);
    } else {
        g_main_loop_quit(loop);
    }
}

static void
cb_presence_event(MatrixClient *client,
                  const gchar *room_id,
                  JsonNode *raw_event,
                  MatrixEventBase *event,
                  gpointer user_data)
{
    g_printerr("Incoming presence event from %s!\n",
               matrix_event_presence_get_user_id(MATRIX_EVENT_PRESENCE(event)));
}

static void
cb_room_member_event(MatrixClient *client,
                     const gchar *room_id,
                     JsonNode *raw_event,
                     MatrixEventBase *event,
                     gpointer user_data)
{
    g_printerr("Incoming room member event from %s via room %s\n",
               matrix_event_room_member_get_user_id(MATRIX_EVENT_ROOM_MEMBER(event)),
               room_id);
}

static void
cb_room_message_event(MatrixClient *client,
                      const gchar *room_id,
                      JsonNode *raw_event,
                      MatrixEventBase *event,
                      gpointer user_data)
{
    g_printf("Message from %s: %s\n",
             matrix_event_room_get_sender(MATRIX_EVENT_ROOM(event)),
             matrix_event_room_message_get_body(
                     MATRIX_EVENT_ROOM_MESSAGE(event)));
}

int
main(int argc, char *argv[])
{
    GError *err = NULL;
    GOptionContext *opts;
    MatrixClient *client;
    GMainLoop *loop;

    opts = g_option_context_new(NULL);
    g_option_context_add_main_entries(opts, entries, NULL);

    if (!g_option_context_parse(opts, &argc, &argv, &err)) {
        g_printerr("Could not parse arguments: %s\n", err->message);
        g_printerr("%s", g_option_context_get_help(opts, TRUE, NULL));

        return 1;
    }

    if ((argc != 1) || !homeserver) {
        g_printerr("%s", g_option_context_get_help(opts, TRUE, NULL));

        return 1;
    }

    g_option_context_free(opts);

    g_info("Starting up: %s with %s:%s", *homeserver, user, password);

    loop = g_main_loop_new(NULL, FALSE);

    client = MATRIX_CLIENT(matrix_http_client_new(*homeserver));
    matrix_http_api_set_validate_certificate(MATRIX_HTTP_API(client),
                                             !no_validate_certs);
    g_signal_connect(G_OBJECT(client),
                     "login-finished",
                     G_CALLBACK(login_finished),
                     loop);
    matrix_client_connect_event(client, MATRIX_EVENT_TYPE_PRESENCE,
                                cb_presence_event, NULL, NULL);
    matrix_client_connect_event(client, MATRIX_EVENT_TYPE_ROOM_MEMBER,
                                cb_room_member_event, NULL, NULL);
    matrix_client_connect_event(client, MATRIX_EVENT_TYPE_ROOM_MESSAGE,
                                cb_room_message_event, NULL, NULL);

    matrix_client_login_with_password(client, user, password, NULL);

    g_main_loop_run(loop);

    return 0;
}
