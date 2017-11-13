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

#include "matrix-event-room-base.h"
#include "config.h"

/**
 * SECTION:matrix-event-room-base
 * @short_description: abstract base class for room events
 * @title: room event base class
 *
 * #MatrixEventRoom is the abstract base class for room events.  It handles the `event_id`,
 * `room_id`, `sender`, `age`, `redacted_because`, and `transaction_id` field of an event’s
 * JSON representation, so subclasses don’t have to care about them.
 */

enum  {
    PROP_0,
    PROP_EVENT_ID,
    PROP_ROOM_ID,
    PROP_SENDER,
    PROP_AGE,
    PROP_REDACTED_BECAUSE,
    PROP_TRANSACTION_ID,
    NUM_PROPS
};

static GParamSpec* matrix_event_room_properties[NUM_PROPS];

typedef struct {
    gchar* _event_id;
    gchar* _room_id;
    gchar* _sender;
    glong _age;
    gchar* _redacted_because;
    gchar* _transaction_id;
} MatrixEventRoomPrivate;

/**
 * MatrixEventRoom:
 *
 * Abstract base class for room events.
 */
G_DEFINE_ABSTRACT_TYPE_WITH_PRIVATE(MatrixEventRoom, matrix_event_room, MATRIX_EVENT_TYPE_BASE);

static void
matrix_event_room_real_from_json(MatrixEventBase *matrix_event_base, JsonNode *json_data, GError **error)
{
    MatrixEventRoom *matrix_event_room;
    MatrixEventRoomPrivate *priv;
    JsonObject *root;
    JsonNode *node;
    GError *inner_error = NULL;

    g_return_if_fail(json_data != NULL);

    matrix_event_room = MATRIX_EVENT_ROOM(matrix_event_base);
    priv = matrix_event_room_get_instance_private(matrix_event_room);

    root = json_node_get_object(json_data);

    if ((node = json_object_get_member (root, "event_id")) != NULL) {
        g_free(priv->_event_id);
        priv->_event_id = g_strdup(json_node_get_string(node));
    } else if (DEBUG) {
        g_warning ("matrix-event-room-base.vala:71: event_id is missing from a Room event");
    }

    if ((node = json_object_get_member(root, "room_id")) != NULL) {
        g_free(priv->_room_id);
        priv->_room_id = g_strdup(json_node_get_string(node));
    } else if (DEBUG) {
        g_warning ("matrix-event-room-base.vala:77: room_id is missing from a Room event");
    }

    if ((node = json_object_get_member(root, "sender")) != NULL) {
        g_free(priv->_sender);
        priv->_sender = g_strdup(json_node_get_string(node));
    } else if (DEBUG) {
        g_warning ("matrix-event-room-base.vala:83: sender is missing from a Room event");
    }

    if ((node = json_object_get_member(root, "unsigned")) != NULL) {
        JsonObject* unsigned_root = NULL;

        unsigned_root = json_node_get_object(node);

        if ((node = json_object_get_member (unsigned_root, "age")) != NULL) {
            priv->_age = json_node_get_int(node);
        }

        if ((node = json_object_get_member(unsigned_root, "redacted_because")) != NULL) {
            g_free(priv->_redacted_because);
            priv->_redacted_because = g_strdup(json_node_get_string(node));
        }

        if ((node = json_object_get_member(unsigned_root, "transaction_id")) != NULL) {
            g_free(priv->_transaction_id);
            priv->_transaction_id = g_strdup(json_node_get_string(node));
        }
    }

    MATRIX_EVENT_BASE_CLASS(matrix_event_room_parent_class)->from_json(matrix_event_base, json_data, &inner_error);

    json_object_unref(root);
    json_node_unref(node);

    if (inner_error != NULL) {
        g_propagate_error(error, inner_error);

        return;
    }
}

static void
matrix_event_room_real_to_json(MatrixEventBase *matrix_event_base, JsonNode *json_data, GError **error)
{
    MatrixEventRoom *matrix_event_room;
    MatrixEventRoomPrivate *priv;
    JsonObject *root_obj;
    JsonObject *unsigned_obj;
    GError *inner_error = NULL;

    g_return_if_fail(json_data != NULL);

    matrix_event_room = MATRIX_EVENT_ROOM(matrix_event_base);
    priv = matrix_event_room_get_instance_private(matrix_event_room);

    root_obj = json_node_get_object (json_data);

    if (priv->_event_id != NULL) {
        json_object_set_string_member (root_obj, "event_id", priv->_event_id);
    }

    if (priv->_room_id!= NULL) {
        json_object_set_string_member(root_obj, "room_id", priv->_room_id);
    }

    if (priv->_sender != NULL) {
        json_object_set_string_member (root_obj, "sender", priv->_sender);
    }

    unsigned_obj = json_object_new();

    if (priv->_age >= 0) {
        json_object_set_int_member (unsigned_obj, "age", priv->_age);
    }

    if (priv->_redacted_because != NULL) {
        json_object_set_string_member (unsigned_obj, "redacted_because", priv->_redacted_because);
    }

    if (priv->_transaction_id != NULL) {
        json_object_set_string_member (unsigned_obj, "transaction_id", priv->_transaction_id);
    }

    if (json_object_get_size(unsigned_obj) > 0) {
        JsonNode* unsigned_node = NULL;

        unsigned_node = json_node_new (JSON_NODE_OBJECT);
        json_node_set_object (unsigned_node, unsigned_obj);
        json_object_set_member (root_obj, "unsigned", unsigned_node);
    }

    MATRIX_EVENT_BASE_CLASS(matrix_event_room_parent_class)->to_json(MATRIX_EVENT_BASE(matrix_event_base), json_data, &inner_error);

    json_object_unref(unsigned_obj);
    json_object_unref(root_obj);

    if (inner_error != NULL) {
        g_propagate_error(error, inner_error);

        return;
    }
}

/**
 * matrix_event_room_construct:
 * @object_type: the #GType of the object to be created
 *
 * Returns: (transfer full): a new instance of @object_type
 */
MatrixEventRoom *
matrix_event_room_construct(GType object_type)
{
    return (MatrixEventRoom *)matrix_event_base_construct(object_type);
}

/**
 * matrix_event_room_get_event_id:
 * @event: a #MatrixEventRoom derived object
 *
 * Get the event ID.
 *
 * The returned value is owned by @event and should not be freed.
 *
 * Returns: (transfer none) (nullable): the event ID
 */
const gchar *
matrix_event_room_get_event_id(MatrixEventRoom *matrix_event_room)
{
    MatrixEventRoomPrivate *priv;

    g_return_val_if_fail(matrix_event_room != NULL, NULL);

    priv = matrix_event_room_get_instance_private(matrix_event_room);

    return priv->_event_id;
}

/**
 * matrix_event_room_set_event_id:
 * @event: a #MatrixEventRoom derived object
 * @event_id: (transfer none) (nullable): an event ID
 *
 * Set the event ID in @event.
 *
 * As the event sending process will strip away this field, setting the event ID is usually
 * useless.
 */
void
matrix_event_room_set_event_id(MatrixEventRoom *matrix_event_room, const gchar *event_id)
{
    MatrixEventRoomPrivate *priv;

    g_return_if_fail(matrix_event_room != NULL);

    priv = matrix_event_room_get_instance_private(matrix_event_room);

    if (g_strcmp0(event_id, priv->_event_id) != 0) {
        g_free(priv->_event_id);
        priv->_event_id = g_strdup(event_id);

        g_object_notify_by_pspec((GObject *)matrix_event_room, matrix_event_room_properties[PROP_EVENT_ID]);
    }
}

/**
 * matrix_event_room_get_room_id:
 * @event: a #MatrixEventRoom derived object
 *
 * Get the room ID from @event.
 *
 * The returned value is owned by @event, and should not be freed.
 *
 * Returns: (transfer none) (nullable): the room ID
 */
const gchar *
matrix_event_room_get_room_id(MatrixEventRoom *matrix_event_room)
{
    MatrixEventRoomPrivate *priv;

    g_return_val_if_fail(matrix_event_room != NULL, NULL);

    priv = matrix_event_room_get_instance_private(matrix_event_room);

    return priv->_room_id;
}

/**
 * matrix_event_room_set_room_id:
 * @event: a #MatrixEventRoom derived object
 * @room_id: (transfer none) (nullable): a room ID
 *
 * Set the room ID for @event.
 *
 * This field is actually the target of any message, so it is really important to set
 * correctly.  Room IDs, as per the Matrix specification, look like
 * `!random-string:homeserver`.  This function doesn’t check for validity, though.
 */
void
matrix_event_room_set_room_id(MatrixEventRoom *matrix_event_room, const gchar *room_id)
{
    MatrixEventRoomPrivate *priv;

    g_return_if_fail(matrix_event_room != NULL);

    priv = matrix_event_room_get_instance_private(matrix_event_room);

    if (g_strcmp0(room_id, priv->_room_id) != 0) {
        g_free(priv->_room_id);
        priv->_room_id = g_strdup(room_id);

        g_object_notify_by_pspec((GObject *)matrix_event_room, matrix_event_room_properties[PROP_ROOM_ID]);
    }
}

/**
 * matrix_event_room_get_sender:
 * @event: a #MatrixEventRoom derived object
 *
 * Get the user ID of @event’s sender.
 *
 * The returned value is owned by @event and should not be freed.
 *
 * Returns: (transfer none) (nullable): a user ID in MXID format
 */
const gchar *
matrix_event_room_get_sender(MatrixEventRoom *matrix_event_room)
{
    MatrixEventRoomPrivate *priv;

    g_return_val_if_fail(matrix_event_room != NULL, NULL);

    priv = matrix_event_room_get_instance_private(matrix_event_room);

    return priv->_sender;
}

/**
 * matrix_event_room_set_sender:
 * @event: a #MatrixEventRoom derived object
 * @sender: (transfer none) (nullable): a user ID in MXID format
 *
 * Set the sender of @event.
 *
 * @sender must be in MXID format (`\@user_id:homeserver`).  This function doesn’t check for
 * user ID validity.
 */
void
matrix_event_room_set_sender(MatrixEventRoom *matrix_event_room, const gchar *sender)
{
    MatrixEventRoomPrivate *priv;

    g_return_if_fail(matrix_event_room != NULL);

    priv = matrix_event_room_get_instance_private(matrix_event_room);

    if (g_strcmp0(sender, priv->_sender) != 0) {
        g_free(priv->_sender);
        priv->_sender = g_strdup(sender);

        g_object_notify_by_pspec((GObject *)matrix_event_room, matrix_event_room_properties[PROP_SENDER]);
    }
}

/**
 * matrix_event_room_get_age:
 * @event: a #MatrixEventRoom derived object
 *
 * Get the age of the event.
 *
 * The age is reported by the homeserver, not calculated by this library.
 *
 * Returns: the age, in milliseconds
 */
glong
matrix_event_room_get_age (MatrixEventRoom *matrix_event_room) {
    MatrixEventRoomPrivate *priv;

    g_return_val_if_fail(matrix_event_room != NULL, -1);

    priv = matrix_event_room_get_instance_private(matrix_event_room);

    return priv->_age;
}

/**
 * matrix_event_room_set_age:
 * @event: a #MatrixEventRoom derived object
 * @age: the age of the event, in milliseconds
 *
 * Set the age of @event.
 *
 * As age is calculated by the homeserver based on the timestamp it received the event,
 * setting this property explicitly has no point.
 */
void
matrix_event_room_set_age(MatrixEventRoom *matrix_event_room, glong age)
{
    MatrixEventRoomPrivate *priv;

    g_return_if_fail(matrix_event_room != NULL);

    priv = matrix_event_room_get_instance_private(matrix_event_room);

    if (priv->_age != age) {
        priv->_age = age;

        g_object_notify_by_pspec((GObject *)matrix_event_room, matrix_event_room_properties[PROP_AGE]);
    }
}

/**
 * matrix_event_room_get_redacted_because:
 * @event: a #MatrixEventRoom derived object
 *
 * Get the redaction reason of @event.
 *
 * This will be %NULL if the event is not redacted, but may also be %NULL if the event is
 * redacted without reason.
 *
 * The value returned is owned by @event and should not be freed.
 *
 * Returns: (transfer none) (nullable): the redaction reason
 */
const gchar *
matrix_event_room_get_redacted_because(MatrixEventRoom *matrix_event_room) {
    MatrixEventRoomPrivate *priv;

    g_return_val_if_fail(matrix_event_room != NULL, NULL);

    priv = matrix_event_room_get_instance_private(matrix_event_room);

    return priv->_redacted_because;
}

/**
 * matrix_event_room_set_redacted_because:
 * @event: a #MatrixEventRoom derived object
 * @redacted_because: (transfer none) (nullable): the reason @event got redacted
 *
 * Set the redaction reason for @event.
 *
 * Redacting events must be done via the corresponding API (eg. matrix_api_redact_event()).
 * Merely setting this field has no effect on the Matrix network.
 */
void
matrix_event_room_set_redacted_because(MatrixEventRoom *matrix_event_room, const gchar *redacted_because)
{
    MatrixEventRoomPrivate *priv;

    g_return_if_fail(matrix_event_room != NULL);

    priv = matrix_event_room_get_instance_private(matrix_event_room);

    if (g_strcmp0(redacted_because, priv->_redacted_because) != 0) {
        g_free(priv->_redacted_because);
        priv->_redacted_because = g_strdup(redacted_because);

        g_object_notify_by_pspec((GObject *)matrix_event_room, matrix_event_room_properties[PROP_REDACTED_BECAUSE]);
    }
}

/**
 * matrix_event_room_get_transaction_id:
 * @event: a #MatrixEventRoom derived object
 *
 * Get the transaction ID of @event.
 *
 * The returned value is owned by @event and should not be freed.
 *
 * Returns: (transfer none) (nullable): the transaction ID
 */
const gchar *
matrix_event_room_get_transaction_id(MatrixEventRoom *matrix_event_room)
{
    MatrixEventRoomPrivate *priv;

    g_return_val_if_fail(matrix_event_room != NULL, NULL);

    priv = matrix_event_room_get_instance_private(matrix_event_room);

    return priv->_transaction_id;
}

/**
 * matrix_event_room_set_transaction_id:
 * @event: a #MatrixEventRoom derived object
 * @transaction_id: (transfer none) (nullable): a transction ID
 *
 * Set the transaction ID of the event.
 */
void
matrix_event_room_set_transaction_id(MatrixEventRoom *matrix_event_room, const gchar *transaction_id)
{
    MatrixEventRoomPrivate *priv;

    g_return_if_fail(matrix_event_room != NULL);

    priv = matrix_event_room_get_instance_private(matrix_event_room);

    if (g_strcmp0(transaction_id, priv->_transaction_id) != 0) {
        g_free(priv->_transaction_id);
        priv->_transaction_id = g_strdup(transaction_id);

        g_object_notify_by_pspec((GObject *)matrix_event_room, matrix_event_room_properties[PROP_TRANSACTION_ID]);
    }
}

static void
matrix_event_room_get_property(GObject *gobject, guint property_id, GValue* value, GParamSpec* pspec)
{
    MatrixEventRoomPrivate *priv = matrix_event_room_get_instance_private(MATRIX_EVENT_ROOM(gobject));

    switch (property_id) {
        case PROP_EVENT_ID:
            g_value_set_string(value, priv->_event_id);

            break;
        case PROP_ROOM_ID:
            g_value_set_string(value, priv->_room_id);

            break;
        case PROP_SENDER:
            g_value_set_string(value, priv->_sender);

            break;
        case PROP_AGE:
            g_value_set_long(value, priv->_age);

            break;
        case PROP_REDACTED_BECAUSE:
            g_value_set_string(value, priv->_redacted_because);

            break;
        case PROP_TRANSACTION_ID:
            g_value_set_string(value, priv->_transaction_id);

            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, property_id, pspec);

            break;
    }
}

static void
matrix_event_room_set_property(GObject *gobject, guint property_id, const GValue* value, GParamSpec* pspec)
{
    MatrixEventRoom *matrix_event_room = MATRIX_EVENT_ROOM(gobject);

    switch (property_id) {
        case PROP_EVENT_ID:
            matrix_event_room_set_event_id(matrix_event_room, g_value_get_string(value));

            break;
        case PROP_ROOM_ID:
            matrix_event_room_set_room_id(matrix_event_room, g_value_get_string(value));

            break;
        case PROP_SENDER:
            matrix_event_room_set_sender(matrix_event_room, g_value_get_string(value));

            break;
        case PROP_AGE:
            matrix_event_room_set_age(matrix_event_room, g_value_get_long(value));

            break;
        case PROP_REDACTED_BECAUSE:
            matrix_event_room_set_redacted_because(matrix_event_room, g_value_get_string(value));

            break;
        case PROP_TRANSACTION_ID:
            matrix_event_room_set_transaction_id(matrix_event_room, g_value_get_string(value));

            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, property_id, pspec);

            break;
    }
}

static void
matrix_event_room_finalize(GObject *gobject)
{
    MatrixEventRoomPrivate *priv = matrix_event_room_get_instance_private(MATRIX_EVENT_ROOM(gobject));

    g_free(priv->_event_id);
    g_free(priv->_room_id);
    g_free(priv->_sender);
    g_free(priv->_redacted_because);
    g_free(priv->_transaction_id);

    G_OBJECT_CLASS(matrix_event_room_parent_class)->finalize(gobject);
}

static void
matrix_event_room_class_init(MatrixEventRoomClass *klass)
{
    ((MatrixEventBaseClass *)klass)->from_json = matrix_event_room_real_from_json;
    ((MatrixEventBaseClass *)klass)->to_json = matrix_event_room_real_to_json;
    G_OBJECT_CLASS (klass)->get_property = matrix_event_room_get_property;
    G_OBJECT_CLASS (klass)->set_property = matrix_event_room_set_property;
    G_OBJECT_CLASS (klass)->finalize = matrix_event_room_finalize;

    /**
     * MatrixEventRoom:event-id:
     *
     * A globally unique event ID. Required.
     */
    matrix_event_room_properties[PROP_EVENT_ID] = g_param_spec_string(
            "event-id", "event-id", "event-id",
            NULL,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_EVENT_ID, matrix_event_room_properties[PROP_EVENT_ID]);

    /**
     * MatrixEventRoom:room-id:
     *
     * The ID of the room associated with this event. Required, but it
     * may be stripped by HS implementations from some APIs if they
     * reside under a key marked with the room ID.
     */
    matrix_event_room_properties[PROP_ROOM_ID] = g_param_spec_string(
            "room-id", "room-id", "room-id",
            NULL,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_ROOM_ID, matrix_event_room_properties[PROP_ROOM_ID]);

    /**
     * MatrixEventRoom:sender:
     *
     * The fully qualified Matrix ID of the user who sent the
     * event. Required.
     */
    matrix_event_room_properties[PROP_SENDER] = g_param_spec_string(
            "sender", "sender", "sender",
            NULL,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_SENDER, matrix_event_room_properties[PROP_SENDER]);

    /**
     * MatrixEventRoom:age:
     *
     * The time, in milliseconds, that has elapsed since the event was sent.  This is part of
     * the unsigned event data.
     *
     * This value will be omitted from the generated event JSON if less than zero.
     */
    matrix_event_room_properties[PROP_AGE] = g_param_spec_long(
            "age", "age", "age",
            G_MINLONG, G_MAXLONG, 0,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_AGE, matrix_event_room_properties[PROP_AGE]);

    /**
     * MatrixEventRoom:redacted-because:
     *
     * The reason this event was redacted, if it was redacted.
     */
    matrix_event_room_properties[PROP_REDACTED_BECAUSE] = g_param_spec_string(
            "redacted-because", "redacted-because", "redacted-because",
            NULL,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_REDACTED_BECAUSE, matrix_event_room_properties[PROP_REDACTED_BECAUSE]);

    /**
     * MatrixEventRoom:transaction-id:
     *
     * The client-supplied transaction ID. This should only be set if
     * the client being given the event is the same one which sent it.
     */
    matrix_event_room_properties[PROP_TRANSACTION_ID] = g_param_spec_string(
            "transaction-id", "transaction-id", "transaction-id",
            NULL,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_TRANSACTION_ID, matrix_event_room_properties[PROP_TRANSACTION_ID]);
}

static void
matrix_event_room_init(MatrixEventRoom *matrix_event_room)
{
    MatrixEventRoomPrivate *priv = matrix_event_room_get_instance_private(matrix_event_room);

    priv->_event_id = NULL;
    priv->_room_id = NULL;
    priv->_sender = NULL;
    priv->_age = 0;
    priv->_redacted_because = NULL;
    priv->_transaction_id = NULL;
}
