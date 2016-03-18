#  Matrix Client SDK for GLib

This is a [Matrix.org](http://matrix.org/) client-server SDK for
GLib >= 2.40. It contains both raw API calls and a signal based
asynchronous client. The API and ABI are both very volatile as of now;
don’t rely on any specific feature until the API is frozen.

The main interfaces are `MatrixAPI` and
`MatrixClient`. `MatrixHTTPAPI` and `MatrixHTTPClient` implement these
interfaces, respectively and can communicate with an HTTP based
homeserver. If a new protocol becomes supported oficially, a new `API`
and `Client` class will be added.

# Usage

The SDK provides two layers of interaction. The low-level layer
(`MatrixAPI` implementations like `MatrixHTTPAPI`) just wraps the raw
API calls. The high-level layer (`MatrixClient` implementations like
`MatrixHTTPClient`) is a `GMainLoop` based asynchronous object that
emits `GObject` signals and uses the low-level layer to provide an
object model to perform actions on.

## Client

For a working example, see [test-client.c](src/test-client.c).

    // Create a client object
    MatrixClient *client = MATRIX_CLIENT(matrix_http_client_new("http://localhost:8008"));

    // Set tokens for the session. Alternatively you may want to login with matrix_api_login() or matrix_client_login_with_password()
    matrix_api_set_token(MATRIX_API(client), "your_access_token");
    matrix_api_set_refresh_token(MATRIX_API(client), "your_refresh_token");

    // Connect a callback that gets called when a m.room.message event arrives
    matrix_client_connect_event(client, MATRIX_EVENT_TYPE_ROOM_MESSAGE, message_callback, NULL);

    // Enter polling mode. This continuously calls the /sync API
    matrix_client_begin_polling(client);

    // Now enter a main loop with g_main_loop_run() so polling for events can actually begin

## API

For a full blown example, see [test-api-client.c](src/test-api-client.c).

# Contribution

See the file [CONTRIBUTING.md](CONTRIBUTING.md) for details.

# Code of Conduct

See the file [CODE_OF_CONDUCT.md](CODE_OF_CONDUCT.md) for details.

# Contact

Should you need any help, join us in Matrix at #matrix-glib-sdk:polonkai.eu.
