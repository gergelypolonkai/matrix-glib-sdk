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

#include "matrix-client.h"
#include "matrix-http-client.h"
#include "matrix-event-presence.h"
#include "matrix-event-room-member.h"
#include "matrix-event-room-message.h"

static gchar *user = NULL;
static gchar *password = NULL;
static gboolean no_validate_certs = FALSE;
static gchar **homeserver = NULL;

static GOptionEntry entries[] = {
    {"user", 'u', 0, G_OPTION_ARG_STRING, &user, "The username to login with", "username"},
    {"password", 'p', 0, G_OPTION_ARG_STRING, &password, "The password to login with", "password"},
    {"no-validate-certs", 'n', 0, G_OPTION_ARG_NONE, &no_validate_certs, "If set, certificates won’t be validated", NULL},
    {G_OPTION_REMAINING, 0, 0, G_OPTION_ARG_FILENAME_ARRAY, &homeserver, "The homeserver to connect to", "homeserver"},
};

static gchar *
save_file_name(MatrixClient *client)
{
    const gchar *base_url = matrix_http_api_get_base_url(
            MATRIX_HTTP_API(client));
    gchar *filename = g_compute_checksum_for_string(
            G_CHECKSUM_SHA1, base_url, -1);
    gchar *dir = g_strdup_printf("%s/matrix-glib-sdk", g_get_user_cache_dir());
    gchar *full_fn = g_strdup_printf("%s/%s", dir, filename);

    g_mkdir_with_parents(dir, 0700);

    g_free(dir);
    g_free(filename);

    return full_fn;
}

static void
save_state(MatrixClient *client)
{
    gchar *filename = save_file_name(client);

    matrix_client_save_state(client, filename, NULL);
    g_free(filename);
}

static void
load_state(MatrixClient *client)
{
    gchar *filename = save_file_name(client);

    matrix_client_load_state(client, filename, NULL);
    g_printf("Token loaded: %s\n", matrix_api_get_token(MATRIX_API(client)));
    g_free(filename);
}

void
login_finished(MatrixClient *client, gboolean success, GMainLoop *loop)
{
    g_printerr("Login %s.\n", (success) ? "succesful" : "failed");

    if (success) {
        save_state(client);
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
    MatrixMessageBase *message;

    if ((message = matrix_event_room_message_get_message(
                 MATRIX_EVENT_ROOM_MESSAGE(event))) != NULL) {
        g_printf("Message from %s in %s: %s\n",
                 matrix_event_room_get_sender(MATRIX_EVENT_ROOM(event)),
                 matrix_event_room_get_room_id(MATRIX_EVENT_ROOM(event)),
                 matrix_message_base_get_body(message));
    } else {
        g_printf("Unknown message received from %s in %s\n",
                 matrix_event_room_get_sender(MATRIX_EVENT_ROOM(event)),
                 matrix_event_room_get_room_id(MATRIX_EVENT_ROOM(event)));
    }
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

    load_state(client);

    if (matrix_api_get_token(MATRIX_API(client)) == NULL) {
        matrix_client_login_with_password(client, user, password, NULL);
    } else {
        matrix_client_begin_polling(client, NULL);
    }

    g_main_loop_run(loop);

    return 0;
}
