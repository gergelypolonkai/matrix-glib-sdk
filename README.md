#  Matrix Client SDK for GLib

This is a Matrix client-server SDK for GLib >= 2.40. As of now it
mostly mimics the official Python SDK.

# Usage

The SDK provides 2 layers of interaction. The low-level layer just wraps the
raw HTTP API calls. The high-level layer wraps the low-level layer and provides
an object model to perform actions on.

## Client

``` c

#include <matrix-glib/matrix-client.h>

gchar *token = NULL;
MatrixClient *client = matrix_client_new("http://localhost:8008", NULL);
matrix_client_register_with_password(client, "foobar", "monkey”);
g_object_get(client, "token", &token, NULL);
room = matrix_client_create_room(client, "my_room_alias");
matrix_room_send_text(room, "Hello!");
```

## API

```c
#include <matrix-glib/matrix-api.h>

static void
sync_finished(MatrixAPI *api,
              MatrixAPIResponse *response,
              gpointer data)
{
    /* Do something with the response */
}

static void
message_sent(MatrixAPI *api,
             MatrixAPIResponse *response,
             gpointer data)
{
    /* Do something with the response */
}

matrix = matrix_api_new("https://matrix.org", "some_token");
g_signal_connect(G_OBJECT(api),
                 "request-finished",
                 G_CALLBACK(sync_finished),
                 NULL);
matrix_api_initial_sync(matrix, sync_finished);
matrix_api_send_message(matrix, "!roomid:matrix.org", "Hello!", message_sent);
```

# Structure

The SDK is split into two modules: `MatrixAPI` and `MatrixClient`.

## API

This contains the raw HTTP API calls and has minimal business logic. You can
set the access token (`token`) to use for requests as well as set a custom
transaction ID (`txn_id`) which will be incremented for each request.

## Client

This encapsulates the API module and provides object models such as `Room`.
