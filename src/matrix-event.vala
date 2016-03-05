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

private HashTable<string, TypeClass>? event_type_handlers = null;

/**
 * Base class for Matrix events.
 */
public class Matrix.Event : GLib.Object, GLib.Initable {
    private Error? _construct_error = null;
    private bool _inited = false;
    private Json.Node? _json;
    private Gee.HashMap<string, Json.Node> _custom_fields =
        new Gee.HashMap<string, Json.Node>();

    protected string? _sender = null;
    protected string? _room_id = null;

    /**
     * The Matrix ID of the events sender.
     */
    public string? sender {
        get {
            return _sender;
        }

        set {
            _sender = value;
        }

        default = null;
    }

    /**
     * The ID of the room this event is associated with.
     */
    public string? room_id {
        get {
            return _room_id;
        }

        set {
            _room_id = value;
        }

        default = null;
    }

    /**
     * The type of the event.
     */
    public string? event_type { get; construct; default = null; }

    /**
     * The timestamp of the event on the originating server.
     */
    public ulong? origin_server_ts { get; set; default = null; }

    /**
     * The unsigned part of the event.
     */
    public Matrix.UnsignedEventData unsigned_data {
        get; set;
        default = null;
    }

    /**
     * The event as a JSON node.
     */
    public Json.Node? json {
        get {
            if (_json == null) {
                _json = new Json.Node(Json.NodeType.OBJECT);
                _json.set_object(new Json.Object());

            }

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

        from_json(json_data);

        _event_type = node.get_string();

        if ((node = root.get_member("origin_server_ts")) != null) {
            _origin_server_ts = (ulong)node.get_int();
        }

        if ((node = root.get_member("sender")) != null) {
            _sender = node.get_string();
        }

        if ((node = root.get_member("unsigned")) != null) {
            _unsigned_data = new Matrix.UnsignedEventData.from_json(node);
        }
    }

    /**
     * Subclasses should implement this function to initialize
     * themselves from JSON data.
     */
    public virtual void
    from_json(Json.Node json_data)
        throws Matrix.Error
    {}

    /**
     * Subclasses should implement this to export their data to JSON.
     */
    public virtual void
    to_json(Json.Node json_data)
        throws Matrix.Error
    {
        Json.Object root = json_data.get_object();

        foreach (var entry in _custom_fields.entries) {
            root.set_member(entry.key, _json_node_deep_copy(entry.value));
        }

        if (event_type != null) {
            root.set_string_member("type", event_type);
        }

        if (origin_server_ts != null) {
            root.set_int_member("origin_server_ts", origin_server_ts);
        }

        if (sender != null) {
            root.set_string_member("sender", sender);
        }

        if (unsigned_data != null) {
            root.set_member("unsigned", unsigned_data.get_json_node());
        }
    }

    public static Event?
    new_from_json(owned string? event_type = null,
                  string? room_id = null,
                  Json.Node? json_data = null)
        throws Matrix.Error, GLib.Error
    {
        GLib.Type event_gtype;
        Event? ret = null;

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

        event_gtype = get_handler(event_type);

        ret = (Event)Object.new(event_gtype,
                                event_type : event_type,
                                room_id : room_id,
                                json : json_data);

        ret.init();

        return ret;
    }

    /**
     * Set a value for a custom field.
     */
    public void
    set_custom_field(string field_name, Json.Node value)
    {
        _custom_fields.set(field_name, value);
    }

    /**
     * Get the value of a custom field.
     */
    public Json.Node?
    get_custom_field (string field_name)
    {
        return _custom_fields.get(field_name);
    }

    /**
     * Delete a custom field.
     */
    public void
    delete_custom_field (string field_name)
    {
        _custom_fields.unset(field_name);
    }

    /**
     * Get the {@link GLib.Type} of the class that is registered to
     * handle events with type @param event_type.
     *
     * @param event_type the event type to look up
     * @return a {@link GLib.Type} or {@link Matrix.Event} if no
     *         handler is registered
     */
    public static GLib.Type
    get_handler(string event_type)
    {
        unowned GLib.TypeClass? klass = null;

        if ((event_type_handlers != null)
            && ((klass = event_type_handlers.get(event_type)) != null)) {
            return klass.get_type();
        }

        if (Config.DEBUG) {
            warning("Using Matrix.Event for %s", event_type);
        }

        return typeof(Matrix.Event);
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
        if (!event_gtype.is_a(typeof(Matrix.Event))) {
            throw new Matrix.Error.INVALID_TYPE(
                    "Invalid event type handler. It must be a subclass of MatrixEvent");
        }

        if (event_type_handlers == null) {
            event_type_handlers = new HashTable<string, GLib.TypeClass>(
                    str_hash, str_equal);
        }

        event_type_handlers.replace(event_type, event_gtype.class_ref());
    }

    /**
     * Unregister @param event_type.
     *
     * @param event_type the event type to remove
     */
    public static void
    unregister_type(string event_type)
    {
        if (event_type_handlers != null) {
            event_type_handlers.remove(event_type);
        }
    }
}
