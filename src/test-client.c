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
login_finished(MatrixAPI *api, JsonNode *content, gpointer data)
{
    JsonPath *path = json_path_new();
    JsonNode *result;

    json_path_compile(path, "$.user_id", NULL);

    if ((result = json_path_match(path, content)) != NULL) {
        JsonArray *array = json_node_get_array(result);
        const gchar *user_id;

        if (json_array_get_length(array) != 1) {
            g_printf("Result is not one long?\n");
        }

        user_id = json_array_get_string_element(array, 0);

        g_printf("Node type: %s\n", json_node_type_name(result));

        g_printf("Logged in as %s\n", user_id);
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
    params = matrix_http_api_gen_parameters(
            "user", user,
            "password", password,
            NULL);
    matrix_http_api_login(MATRIX_API(api),
                          login_finished, NULL,
                          "m.login.password",
                          params);

    g_info("Entering main loop");
    g_main_loop_run(loop);

    return 0;
}
