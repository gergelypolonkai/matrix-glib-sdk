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
 * Base class for Matrix events.
 */
public abstract class Matrix.Event.Base : GLib.Object,
                                          GLib.Initable,
                                          Json.Serializable {
    private Error? _construct_error = null;
    private bool _inited = false;
    private Json.Node? _json;

    protected string? _event_type = null;

    /**
     * The type of the event. It should be namespaced similar to the
     * Java package naming conventions,
     * e.g. `com.example.subdomain.event.type`. It cannot be changed
     * after object initialization.
     */
    public string? event_type {
        get {
            return _event_type;
        }

        construct {
            _event_type = value;
        }

        default = null;
    }

    /**
     * The event as a JSON node.
     */
    public Json.Node? json {
        get {
            _json = new Json.Node(Json.NodeType.OBJECT);
            _json.set_object(new Json.Object());

            // Add the content node, as all event types must include it.
            var content_root = new Json.Node(Json.NodeType.OBJECT);
            var content_obj = new Json.Object();
            content_root.set_object(content_obj);
            _json.get_object().set_member("content", content_root);

            try {
                to_json(_json);
            }
            catch (Matrix.Error e) {
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

    // Implementation of GLib.Initable

    public bool
    init(GLib.Cancellable? cancellable = null)
        throws Error, Matrix.Error
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

    // Implementation of Json.Serializable


    public unowned ParamSpec
    find_property(string name)
    {
        return get_class().find_property(name);
    }

    public Json.Node
    serialize_property(string property_name,
                       Value value,
                       ParamSpec pspec)
    {
        return default_serialize_property(property_name, value, pspec);
    }

    public bool
    deserialize_property(string property_name,
                         out Value value,
                         ParamSpec pspec,
                         Json.Node property_node)
    {
        value = Value(pspec.value_type);

        return default_deserialize_property(property_name,
                                            value,
                                            pspec,
                                            property_node);
    }

    // Own methods

    private void
    initialize_from_json(Json.Node json_data)
        throws Matrix.Error
    {
        Json.Object root;
        Json.Node node;

        if (json_data.get_node_type() != Json.NodeType.OBJECT) {
            throw new Matrix.Error.INVALID_FORMAT(
                    "The event is not valid");
        }

        root = json_data.get_object();

        if ((node = root.get_member("type")) == null) {
            throw new Matrix.Error.INCOMPLETE(
                    "Event type is not specified");
        }

        if ((_event_type != null)
            && (_event_type != node.get_string())) {
            throw new Matrix.Error.INVALID_TYPE(
                    "Changing event type is not supported");
        }

        var evt_type = node.get_string();

        if ((node = root.get_member("content")) == null) {
            warning("content key is missing from the %s event", evt_type);

            // As event type objects depend on having this node, let’s
            // add it now.
            var content_node = new Json.Node(Json.NodeType.OBJECT);
            content_node.set_object(new Json.Object());
            root.set_member("content", content_node);
        }

        from_json(json_data);

        _event_type = evt_type;
    }

    /**
     * Subclasses should implement this function to initialize
     * themselves from JSON data.
     */
    public virtual void
    from_json(Json.Node json_data)
        throws Matrix.Error
    {
        var root = json_data.get_object();
        Json.Node? node;

        if ((node = root.get_member("type")) != null) {
            _event_type = node.get_string();
        } else if (Config.DEBUG) {
            warning("type is not present in an event");
        }
    }

    /**
     * Subclasses should implement this to export their data to JSON.
     */
    public virtual void
    to_json(Json.Node json_data)
        throws Matrix.Error
    {
        var root = json_data.get_object();

        if (_event_type == null) {
            throw new Matrix.Error.INCOMPLETE(
                    "Won't generate an event without type");
        }

        root.set_string_member("type", _event_type);
    }

    public static Base?
    new_from_json(owned string? event_type = null,
                  Json.Node? json_data = null)
        throws Matrix.Error, GLib.Error
    {
        GLib.Type? event_gtype;
        Base? ret = null;

        if (event_type == null) {
            if (json_data == null) {
                throw new Matrix.Error.INCOMPLETE(
                        "Either event_type or json_data must be set!");
            }

            if (json_data.get_node_type() != Json.NodeType.OBJECT) {
                throw new Matrix.Error.INVALID_FORMAT(
                        "Event is not a JSON object!");
            }

            Json.Node? node;

            if ((node = json_data.get_object().get_member("type")) == null) {
                throw new Matrix.Error.INCOMPLETE(
                        "event_type is null and JSON object doesn't contain type!");
            }

            event_type = node.get_string();
        }

        if ((event_gtype = get_handler(event_type)) == null) {
            throw new Matrix.Error.INVALID_TYPE(
                    "No registered type for event type %s",
                    event_type);
        }

        ret = (Base)Object.new(event_gtype,
                                event_type : event_type,
                                json : json_data);

        ret.init();

        return ret;
    }
}

namespace Matrix.Event {

private HashTable<string, TypeClass>? type_handlers = null;

/**
 * Get the {@link GLib.Type} of the class that is registered to
 * handle events with type @param event_type.
 *
 * @param event_type the event type to look up
 * @return a {@link GLib.Type} or {@link Matrix.Event} if no
 *         handler is registered
 */
public static GLib.Type?
get_handler(string event_type)
{
    unowned GLib.TypeClass? klass = null;

    if ((type_handlers != null)
        && ((klass = type_handlers.get(event_type)) != null)) {
        return klass.get_type();
    }

    if (Config.DEBUG) {
        warning("No registered type for %s", event_type);
    }

    return null;
}

/**
 * Registers @param event_type to be handled by the
 * type @param event_gtype.
 *
 * @param event_type the type of the event
 * @param event_gtype the {@link GLib.Type} of the event’s handler
 */
public static void
register_type(string event_type, GLib.Type event_gtype)
    throws Matrix.Error
{
    if (!event_gtype.is_a(typeof(Matrix.Event.Base))) {
        throw new Matrix.Error.INVALID_TYPE(
                "Invalid event type handler. It must be a subclass of MatrixEvent");
    }

    if (type_handlers == null) {
        type_handlers = new HashTable<string, GLib.TypeClass>(
                str_hash, str_equal);
    }

    type_handlers.replace(event_type, event_gtype.class_ref());
}

/**
 * Unregister @param event_type.
 *
 * @param event_type the event type to remove
 */
public static void
unregister_type(string event_type)
{
    if (type_handlers != null) {
        type_handlers.remove(event_type);
    }
}
}
