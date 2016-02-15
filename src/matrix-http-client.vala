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

/**
 * An event-driven client class to communicate with HTTP based
 * Matrix.org servers.
 */
public class Matrix.HTTPClient : Matrix.HTTPAPI, Matrix.Client {
    private bool _polling = false;
    private ulong _event_timeout = 30000;

    public
    HTTPClient(string base_url)
    {
        Object(base_url : base_url);
    }

    public void
    login_with_password(string username, string password)
        throws Matrix.Error
    {
        var builder = new Json.Builder();

        builder.begin_object();

        builder.set_member_name("user");
        builder.add_string_value(username);

        builder.set_member_name("password");
        builder.add_string_value(password);

        builder.end_object();

        login((i, content_type, json_content, raw_content, error) => {
                login_finished((error == null) || (error is Matrix.Error.NONE));
            }
            ,"m.login.password",
            builder.get_root());
    }

    public void
    register_with_password(string? username, string password)
        throws Matrix.Error
    {
        register_account(
                (i, content_type, json_content, raw_content, error) => {
                    login_finished((error is Matrix.Error.NONE));
                },
                Matrix.AccountKind.USER,
                false, username, password);
    }

    public void
    logout()
        throws Matrix.Error
    {
        token = null;
        refresh_token = null;
        abort_pending();
    }

    private void
    cb_event_stream(string content_type,
                    Json.Node? json_content,
                    ByteArray? raw_content,
                    Matrix.Error? error)
    {
        string? end_token = null;

        if (error == null) {
            var root_obj = json_content.get_object();
            Json.Node? node;

            if ((node = root_obj.get_member("chunk")) != null) {
                var chunks = node.get_array();

                chunks.foreach_element((ary, idx, member_node) => {});
            }

            if ((node = root_obj.get_member("end")) != null) {
                end_token = node.get_string();
            }
        }

        // Only continue polling if polling is still enabled, and
        // there was no communication error during the last call

        if (_polling && ((error == null) || (error.code <= 500))) {
            _polling = false;

            try {
                event_stream(
                        (API.Callback)cb_event_stream,
                        end_token,
                        _event_timeout);
            } catch (Matrix.Error e) {}
        }
    }

    public void
    begin_polling()
        throws Matrix.Error
    {
        try {
            event_stream((API.Callback)cb_event_stream, null, _event_timeout);
            _polling = true;
        } catch (Matrix.Error e) {
            throw e;
        }
    }

    public void
    stop_polling(bool cancel_ongoing)
        throws Matrix.Error
    {
        _polling = false;

        if (cancel_ongoing) {
            abort_pending();
        }
    }
}
