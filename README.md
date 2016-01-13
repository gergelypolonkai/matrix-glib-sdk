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

The `MatrixClient` interface is not fully planned yet.

## API

For a full blown example, see [test-client.c](src/test-client.c).
