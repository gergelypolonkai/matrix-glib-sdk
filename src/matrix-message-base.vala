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
 * Base class for message handlers.
 *
 * Message handler base class.
 */
public abstract class Matrix.Message.Base : Object, Initable {
    /**
     * The message type.
     */
    public string? message_type { get; set; default = null; }

    /**
     * The body of the message.
     */
    public string? body { get; set; default = null; }

    public Json.Node? json {
        get {
            _json = new Json.Node(Json.NodeType.OBJECT);
            _json.set_object(new Json.Object());

            try {
                to_json(_json);
            } catch (Matrix.Error e) {
                return null;
            }

            return _json;
        }

        construct {
            if (value != null) {
                try {
                    initialize_from_json(value);
                } catch (Matrix.Error e) {}
            }
        }
    }

    private bool _inited = false;
    private Error? _construct_error = null;
    private Json.Node? _json = null;

    public bool
    init(Cancellable? cancellable = null)
        throws GLib.Error
    {
        if (cancellable != null) {
            throw new Matrix.Error.UNSUPPORTED(
                    "Cancellable initialization not supported");
        }

        if (_construct_error != null) {
            throw _construct_error;
        }

        _inited = true;

        return true;
    }

    private void
    initialize_from_json(Json.Node json_data)
        throws Matrix.Error
    {
        if (json_data.get_node_type() != Json.NodeType.OBJECT) {
            throw new Matrix.Error.INVALID_FORMAT(
                    "The message is not valid");
        }

        from_json(json_data);
    }

    public static Matrix.Message.Base?
    new_from_json(Json.Node json_data)
        throws Matrix.Error, GLib.Error
    {
        var root = json_data.get_object();
        Json.Node? node;

        if ((node = root.get_member("msgtype")) == null) {
            throw new Matrix.Error.INCOMPLETE(
                    "Can not process a message without msgtype");
        }

        Type? msg_gtype;

        if ((msg_gtype = get_handler(node.get_string())) == null) {
            return null;
        }

        var ret = (Base)Object.new(msg_gtype,
                                   json : json_data);

        ret.init();

        return ret;
    }

    /**
     * Subclasses should override this method to initialize themselves
     * from JSON data while chaining up to allow parent classes to do
     * the same.
     */
    public virtual void
    from_json(Json.Node json_data)
        throws Matrix.Error
    {
        var root = json_data.get_object();
        Json.Node? node;

        if ((node = root.get_member("msgtype")) != null) {
            _message_type = node.get_string();
        } else if (Config.DEBUG) {
            warning("msgtype is not present in a message");
        }
    }

    /**
     * Subclasses should override this method to export their data to
     * JSON, while chaining up to allow parent classes to do the same.
     */
    public virtual void
    to_json(Json.Node json_data)
        throws Matrix.Error
    {
        var root = json_data.get_object();

        if (_message_type == null) {
            throw new Matrix.Error.INCOMPLETE(
                    "Won't generate a message with an empty msgtype");
        }

        root.set_string_member("msgtype", _message_type);
    }
}

namespace Matrix.Message {

private HashTable<string, TypeClass>? type_handlers = null;

/**
 * Get the {@link GLib.Type} of the class that is registered to handle
 * messages with type @param message_type.
 *
 * @param message_type the message type to look up
 * @return a {@link GLib.Type} or `null` if no handler is registered
 */
public static GLib.Type?
get_handler(string message_type)
{
    unowned GLib.TypeClass? klass = null;

    if ((type_handlers != null)
        && ((klass = type_handlers.get(message_type)) != null)) {
        return klass.get_type();
    }

    if (Config.DEBUG) {
        warning("No registered type for message %s", message_type);
    }

    return null;
}

/**
 * Register @param message_type to be handled by the
 * type @param message_gtype.
 *
 * @param message_type the type of the message
 * @param message_gtype the {@link GLib.Type} of the event's handler
 */
public static void
register_type(string message_type, GLib.Type message_gtype)
    throws Matrix.Error
{
    if (!message_gtype.is_a(typeof(Matrix.Message.Base))) {
        throw new Matrix.Error.INVALID_TYPE(
                "Invalid message handler type");
    }

    if (type_handlers == null) {
        type_handlers = new HashTable<string, GLib.TypeClass>(
                str_hash, str_equal);
    }

    type_handlers.replace(message_type, message_gtype.class_ref());
}

/**
 * Unregister @param message_type.
 *
 * @param message_type the message type to remove
 */
public void
unregister_type(string message_type)
{
    if (type_handlers != null) {
        type_handlers.remove(message_type);
    }
}
}
