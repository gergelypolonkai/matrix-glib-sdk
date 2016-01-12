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

#include <json-glib/json-glib.h>

#include "matrix-http-api.h"

#define LOG_DOMAIN "Matrix-Test-Client"

static gchar *user;
static gchar *password;

static GOptionEntry entries[] = {
    {"user", 'u', 0, G_OPTION_ARG_STRING, &user},
    {"password", 'p', 0, G_OPTION_ARG_STRING, &password},
};

static void
initial_sync_finished(MatrixAPI *api,
                      JsonNode *content,
                      gpointer data,
                      GError *err)
{
    g_printf("initialSync finished\n");

    matrix_api_event_stream(MATRIX_API(api), NULL, NULL, NULL, 0, NULL);
}

static void
create_room_finished(MatrixAPI *api,
                     JsonNode *content,
                     gpointer data,
                     GError *err)
{
    if (err) {
        g_debug("Error: %s", err->message);
    } else {
        g_printf("Room registered\n");
    }

    matrix_api_initial_sync(MATRIX_API(api),
                            initial_sync_finished,
                            data, 10, TRUE,
                            NULL);
}

static void
login_finished(MatrixAPI *api, JsonNode *content, gpointer data, GError *err)
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

    if ((result = json_path_match(path, content)) != NULL) {
        JsonArray *array = json_node_get_array(result);
        const gchar *user_id;

        if (json_array_get_length(array) != 1) {
            g_printf("Result is not one long?\n");
        }

        user_id = json_array_get_string_element(array, 0);

        g_printf("Logged in as %s\n", user_id);

        matrix_api_create_room(api,
                               create_room_finished, NULL,
                               MATRIX_API_ROOM_PRESET_PUBLIC,
                               "GLib SDK test room", "matrix-glib-sdk-test",
                               "GLib SDK test room",
                               MATRIX_API_ROOM_VISIBILITY_DEFAULT,
                               NULL, NULL, NULL, NULL);
    } else {
        g_printf("Login unsuccessful!\n");
    }
}

int
main(int argc, char *argv[])
{
    MatrixHTTPAPI *api;
    GHashTable *params;
    GMainLoop *loop = g_main_loop_new(NULL, FALSE);
    GOptionContext *opts;
    GError *err = NULL;
    gchar *url;
    JsonBuilder *builder;
    JsonNode *login_content;

    opts = g_option_context_new(NULL);
    g_option_context_add_main_entries(opts, entries, NULL);

    if (!g_option_context_parse(opts, &argc, &argv, &err)) {
        g_printerr("Could not parse arguments: %s\n", err->message);
        g_printerr("%s", g_option_context_get_help(opts, TRUE, NULL));

        return 1;
    }

    if (argc != 2) {
        g_printerr("%s", g_option_context_get_help(opts, TRUE, NULL));

        return 1;
    }

    g_option_context_free(opts);

    url = argv[1];

    g_info("Starting up: %s with %s:%s", url, user, password);

    api = matrix_http_api_new(url, NULL);
    builder = json_builder_new();
    json_builder_begin_object(builder);
    json_builder_set_member_name(builder, "user");
    json_builder_add_string_value(builder, user);
    json_builder_set_member_name(builder, "password");
    json_builder_add_string_value(builder, password);
    json_builder_end_object(builder);
    login_content = json_builder_get_root(builder);

    matrix_api_login(MATRIX_API(api),
                     login_finished, loop,
                     "m.login.password",
                     login_content,
                     &err);

    if (err) {
        g_warning("Error: %s", err->message);

        return;
    }

    g_info("Entering main loop");
    g_main_loop_run(loop);

    return 0;
}
