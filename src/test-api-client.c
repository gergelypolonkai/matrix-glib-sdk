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

#include <glib/gprintf.h>
#include <json-glib/json-glib.h>
#include <libsoup/soup.h>

#include "matrix-glib.h"

#define LOG_DOMAIN "Matrix-Test-Client"

static gchar *user;
static gchar *password;
static gboolean no_validate_certs = FALSE;
static gchar **homeserver = NULL;

static GOptionEntry entries[] = {
    {"user", 'u', 0, G_OPTION_ARG_STRING, &user},
    {"password", 'p', 0, G_OPTION_ARG_STRING, &password},
    {"no-validate-certs", 'n', 0, G_OPTION_ARG_NONE, &no_validate_certs},
    {G_OPTION_REMAINING, 0, 0, G_OPTION_ARG_FILENAME_ARRAY, &homeserver, "The homeserver to connect to", "homeserver"},
};

static void
initial_sync_finished(MatrixAPI *api,
                      const gchar *content_type,
                      JsonNode *json_content,
                      GByteArray *raw_content,
                      GError *err,
                      gpointer user_data)
{
    g_printf("initialSync finished\n");

    matrix_api_event_stream(api, NULL, NULL, NULL, 0, NULL);
}

static void
create_room_finished(MatrixAPI *api,
                     const gchar *content_type,
                     JsonNode *json_content,
                     GByteArray *raw_content,
                     GError *err,
                     gpointer data)
{
    if (err) {
        g_debug("Error: %s", err->message);
    } else {
        g_printf("Room registered\n");
    }

    matrix_api_join_room(api, NULL, NULL, "#matrix-glib-sdk-test:elxa4y5sd12", NULL);
    matrix_api_list_public_rooms(api, NULL, NULL, NULL);
}

static void
get_presence_finished(MatrixAPI *api,
                           const gchar *content_type,
                           JsonNode *json_content,
                           GByteArray *raw_content,
                           GError *err,
                           gpointer data)
{
    JsonObject *root_obj;
    const gchar *avatar_url;
    SoupURI *avatar_uri;

    root_obj = json_node_get_object(json_content);
    avatar_url = json_object_get_string_member(root_obj, "avatar_url");

    g_info("Avatar URL: %s", avatar_url);

    avatar_uri = soup_uri_new(avatar_url);
    g_info("Scheme: %s; authority: %s; path: %s",
           soup_uri_get_scheme(avatar_uri),
           soup_uri_get_host(avatar_uri),
           soup_uri_get_path(avatar_uri));
    matrix_api_media_download(api,
                              NULL, NULL,
                              soup_uri_get_host(avatar_uri),
                              soup_uri_get_path(avatar_uri) + 1,
                              NULL);
    soup_uri_free(avatar_uri);
}

static void
login_finished(MatrixAPI *api,
               const gchar *content_type,
               JsonNode *json_content,
               GByteArray *raw_content,
               GError *err,
               gpointer data)
{
    JsonPath *path = json_path_new();
    JsonNode *result;
    GMainLoop *loop = data;

    if (err) {
        g_printf("ERROR: %s\n", err->message);

        g_main_loop_quit(loop);

        return;
    }

    json_path_compile(path, "$.user_id", NULL);

    if ((result = json_path_match(path, json_content)) != NULL) {
        JsonArray *array = json_node_get_array(result);
        const gchar *user_id;

        if (json_array_get_length(array) != 1) {
            g_printf("Result is not one long?\n");
        }

        user_id = matrix_api_get_user_id(MATRIX_API(api));

        g_printf("Logged in as %s\n", user_id);

        matrix_api_initial_sync(api,
                                initial_sync_finished,
                                data, 10, TRUE,
                                NULL);
        matrix_api_create_room(api,
                               create_room_finished, NULL,
                               MATRIX_ROOM_PRESET_PUBLIC,
                               "GLib SDK test room", "matrix-glib-sdk-test",
                               "GLib SDK test room",
                               MATRIX_ROOM_VISIBILITY_DEFAULT,
                               NULL, NULL, NULL, NULL, NULL);
        matrix_api_get_presence_list(api, NULL, NULL, user_id, NULL);
        matrix_api_get_presence(api,
                                     get_presence_finished, NULL,
                                     user_id, NULL);
    } else {
        g_printf("Login unsuccessful!\n");

        g_main_loop_quit(loop);
    }
}

int
main(int argc, char *argv[])
{
    MatrixAPI *api;
    GMainLoop *loop = g_main_loop_new(NULL, FALSE);
    GOptionContext *opts;
    GError *err = NULL;
    JsonBuilder *builder;
    JsonNode *login_content;

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

    api = MATRIX_API(matrix_http_api_new(*homeserver, NULL));
    matrix_http_api_set_validate_certificate(MATRIX_HTTP_API(api),
                                             !no_validate_certs);
    builder = json_builder_new();
    json_builder_begin_object(builder);
    json_builder_set_member_name(builder, "user");
    json_builder_add_string_value(builder, user);
    json_builder_set_member_name(builder, "password");
    json_builder_add_string_value(builder, password);
    json_builder_end_object(builder);
    login_content = json_builder_get_root(builder);

    g_debug("Logging in");
    matrix_api_login(api,
                     login_finished, loop,
                     "m.login.password",
                     login_content,
                     &err);

    if (err) {
        g_warning("Error: %s", err->message);

        return 1;
    }

    g_info("Entering main loop");
    g_main_loop_run(loop);

    return 0;
}
