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

#include "matrix-event-receipt.h"
#include "matrix-enumtypes.h"
#include "config.h"

/**
 * SECTION:matrix-event-receipt
 * @short_description: event to inform clients of new receipts
 *
 * This is the default event handler for `m.receipt` events.
 */
enum  {
    PROP_0,
    PROP_ROOM_ID,
    NUM_PROPS
};
static GParamSpec* matrix_event_receipt_properties[NUM_PROPS];

typedef struct {
    gchar *_room_id;
    GHashTable *_receipt_data;
} MatrixEventReceiptPrivate;

/**
 * MatrixEventReceipt:
 */
G_DEFINE_TYPE_WITH_PRIVATE(MatrixEventReceipt, matrix_event_receipt, MATRIX_EVENT_TYPE_BASE);

typedef struct {
    gchar *event_id;
    gchar *typ;
    gchar *user;
    guint refcount;
} ReceiptData;

static gboolean
_rd_equal(ReceiptData *k1, ReceiptData *k2) {
    if ((k1 == NULL) && (k2 == NULL)) {
        return TRUE;
    }

    if ((k1 == NULL) || (k2 == NULL)) {
        return FALSE;
    }

    return ((g_strcmp0(k1->event_id, k2->event_id) == 0) &&
            (g_strcmp0(k1->typ, k2->typ) == 0) &&
            (g_strcmp0(k1->user, k2->user) == 0));
}

static void
_rd_free(ReceiptData *receipt_data) {
    g_return_if_fail(receipt_data != NULL);

    if ( --receipt_data->refcount == 0) {
        g_free(receipt_data->event_id);
        g_free(receipt_data->typ);
        g_free(receipt_data->user);

        g_free(receipt_data);
    }
}

static void
process_event(JsonObject *obj, const gchar *key, JsonNode *member_node, gpointer user_data)
{
    JsonNode *node;
    MatrixEventReceiptPrivate *priv = user_data;

    if ((node = json_object_get_member(json_node_get_object(member_node), "m.read")) != NULL) {
        JsonObject *read_obj = json_node_get_object(node);
        JsonObjectIter inner_iter;
        JsonNode *inner_node;
        const gchar *inner_key;

        json_object_iter_init(&inner_iter, read_obj);

        while (json_object_iter_next(&inner_iter, &inner_key, &inner_node)) {
            gulong *value;
            ReceiptData *rd_key;

            if (priv->_receipt_data == NULL) {
                priv->_receipt_data = g_hash_table_new_full(g_direct_hash, (GEqualFunc)_rd_equal, (GDestroyNotify)_rd_free, g_free);
            }

            rd_key = g_new(ReceiptData, 1);
            rd_key->event_id = g_strdup(key);
            rd_key->typ = g_strdup("m.read");
            rd_key->user = g_strdup(inner_key);

            value = g_new(gulong, 1);
            *value = json_node_get_int(json_object_get_member(json_node_get_object(inner_node), "ts"));

            g_hash_table_replace(priv->_receipt_data, rd_key, value);
        }
    } else {
        g_warning("content.$event-id.m.read is missing from a m.presence event");
    }
}

static void
matrix_event_receipt_real_from_json(MatrixEventBase *matrix_event_base, JsonNode *json_data, GError **error)
{
    MatrixEventReceiptPrivate *priv;
    JsonObject *root;
    JsonObject *content_root;
    JsonNode *content_node;
    JsonNode *node;
    GError *inner_error = NULL;

    g_return_if_fail(json_data != NULL);

    priv = matrix_event_receipt_get_instance_private(MATRIX_EVENT_RECEIPT(matrix_event_base));

    root = json_node_get_object(json_data);
    content_node = json_object_get_member(root, "content");
    content_root = json_node_get_object(content_node);

    if ((node = json_object_get_member(root, "room_id")) != NULL) {
        g_free(priv->_room_id);
        priv->_room_id = g_strdup(json_node_get_string(node));
    } else if (DEBUG) {
        g_warning("room_id is missing from a m.receipt event");
    }

    json_object_foreach_member(content_root, process_event, priv);

    MATRIX_EVENT_BASE_CLASS(matrix_event_receipt_parent_class)->from_json(matrix_event_base, json_data, &inner_error);

    if (inner_error != NULL) {
        g_propagate_error(error, inner_error);
    }
}

static void
matrix_event_receipt_real_to_json(MatrixEventBase *matrix_event_base, JsonNode *json_data, GError **error)
{
    MatrixEventReceiptPrivate *priv;
    JsonObject *root;
    JsonObject *content_root;
    JsonNode *content_node;
    GHashTableIter iter;
    gpointer gh_key;
    gpointer gh_value;
    gint i = 0;
    GError *inner_error = NULL;

    g_return_if_fail(json_data != NULL);

    priv = matrix_event_receipt_get_instance_private(MATRIX_EVENT_RECEIPT(matrix_event_base));

    if (priv->_room_id == NULL) {
        g_set_error(error, MATRIX_ERROR, MATRIX_ERROR_INCOMPLETE,
                    "Won't generate a m.receipt without room_id");

        return;
    }

    root = json_node_get_object(json_data);
    content_node = json_object_get_member(root, "content");
    content_root = json_node_get_object(content_node);

    g_hash_table_iter_init(&iter, priv->_receipt_data);

    while (g_hash_table_iter_next(&iter, &gh_key, &gh_value)) {
        ReceiptData *key = gh_key;
        gulong value = *(gulong *)gh_value;
        JsonObject *event_object;
        JsonObject *type_object;
        JsonObject *user_object;
        JsonNode *node;

        if (key->event_id == NULL) {
            g_set_error(error, MATRIX_ERROR, MATRIX_ERROR_INCOMPLETE,
                        "Won't generate a m.receipt event with an empty event ID");


            return;
        }

        if (key->typ == NULL) {
            g_set_error(error, MATRIX_ERROR, MATRIX_ERROR_INCOMPLETE,
                        "Won't generate a m.receipt event with an empty receipt type");

            return;
        }

        if (key->user == NULL) {
            g_set_error(error, MATRIX_ERROR, MATRIX_ERROR_INCOMPLETE,
                        "Won't generate a m.receipt event with an empty user ID");

            return;
        }

        i++;

        if ((node = json_object_get_member(content_root, key->event_id)) == NULL) {
            event_object = json_object_new();
            node = json_node_new(JSON_TYPE_OBJECT);
            json_node_set_object(node, event_object);
            json_object_set_member(content_root, key->event_id, node);
        } else {
            event_object = json_node_get_object(node);
        }

        if ((node = json_object_get_member(event_object, key->typ)) == NULL) {
            type_object = json_object_new();
            node = json_node_new(JSON_TYPE_OBJECT);
            json_node_set_object(node, type_object);
            json_object_set_member(event_object, key->typ, node);
        } else {
            type_object = json_node_get_object(node);
        }

        if ((node = json_object_get_member(type_object, key->user)) == NULL) {
            user_object = json_object_new();
            node = json_node_new(JSON_TYPE_OBJECT);
            json_node_set_object(node, user_object);
            json_object_set_member(type_object, key->user, node);
        } else {
            user_object = json_node_get_object(node);
        }

        json_object_set_int_member(user_object, "ts", value);
    }

    if (i == 0) {
        g_set_error(error, MATRIX_ERROR, MATRIX_ERROR_INCOMPLETE,
                    "Won't generate a m.receipt event with no receipts");

        return;
    }

    json_object_set_string_member(root, "room_id", priv->_room_id);

    MATRIX_EVENT_BASE_CLASS(matrix_event_receipt_parent_class)->to_json(matrix_event_base, json_data, &inner_error);

    if (inner_error != NULL) {
        g_propagate_error(error, inner_error);
    }
}

MatrixEventReceipt *
matrix_event_receipt_construct(GType object_type)
{
    return (MatrixEventReceipt *)matrix_event_base_construct(object_type);
}

/**
 * matrix_event_receipt_new:
 *
 * Create a new #MatrixEventReceipt object.
 *
 * Returns: (transfer full): a new #MatrixEventReceipt object
 */
MatrixEventReceipt *
matrix_event_receipt_new(void)
{
    return matrix_event_receipt_construct(MATRIX_EVENT_TYPE_RECEIPT);
}

/**
 * matrix_event_receipt_get_room_id:
 * @event: a #MatrixEventReceipt
 *
 * Get the room ID @event belongs to.
 *
 * The returned value is owned by @event and should not be freed.
 *
 * Returns: (transfer none) (nullable): a room ID
 */
const gchar *
matrix_event_receipt_get_room_id(MatrixEventReceipt *matrix_event_receipt)
{
    MatrixEventReceiptPrivate *priv;

    g_return_val_if_fail(matrix_event_receipt != NULL, NULL);

    priv = matrix_event_receipt_get_instance_private(matrix_event_receipt);

    return priv->_room_id;
}

/**
 * matrix_event_receipt_set_room_id:
 * @event: a #MatrixEventReceipt
 * @room_id: (transfer none) (nullable): a room ID
 *
 * Set the room ID for @event.
 */
void
matrix_event_receipt_set_room_id(MatrixEventReceipt *matrix_event_receipt, const gchar *room_id)
{
    MatrixEventReceiptPrivate *priv;

    g_return_if_fail(matrix_event_receipt != NULL);

    priv = matrix_event_receipt_get_instance_private(matrix_event_receipt);

    if (g_strcmp0(room_id, priv->_room_id) != 0) {
        g_free(priv->_room_id);
        priv->_room_id = g_strdup(room_id);

        g_object_notify_by_pspec((GObject *)matrix_event_receipt, matrix_event_receipt_properties[PROP_ROOM_ID]);
    }
}

static void
matrix_event_receipt_finalize(GObject *gobject)
{
    MatrixEventReceiptPrivate *priv = matrix_event_receipt_get_instance_private(MATRIX_EVENT_RECEIPT(gobject));

    g_free(priv->_room_id);
    g_hash_table_unref(priv->_receipt_data);

    G_OBJECT_CLASS(matrix_event_receipt_parent_class)->finalize(gobject);
}

static void
matrix_event_receipt_get_property(GObject *gobject, guint property_id, GValue *value, GParamSpec *pspec)
{
    MatrixEventReceipt *matrix_event_receipt = MATRIX_EVENT_RECEIPT(gobject);

    switch (property_id) {
        case PROP_ROOM_ID:
            g_value_set_string(value, matrix_event_receipt_get_room_id(matrix_event_receipt));

            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, property_id, pspec);

            break;
    }
}

static void
matrix_event_receipt_set_property(GObject *gobject, guint property_id, const GValue *value, GParamSpec *pspec)
{
    MatrixEventReceipt *matrix_event_receipt = MATRIX_EVENT_RECEIPT(gobject);

    switch (property_id) {
        case PROP_ROOM_ID:
            matrix_event_receipt_set_room_id(matrix_event_receipt, g_value_get_string(value));

            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, property_id, pspec);

            break;
    }
}

static void
matrix_event_receipt_class_init(MatrixEventReceiptClass *klass)
{
    ((MatrixEventBaseClass *)klass)->from_json = matrix_event_receipt_real_from_json;
    ((MatrixEventBaseClass *)klass)->to_json = matrix_event_receipt_real_to_json;

    G_OBJECT_CLASS(klass)->get_property = matrix_event_receipt_get_property;
    G_OBJECT_CLASS(klass)->set_property = matrix_event_receipt_set_property;
    G_OBJECT_CLASS(klass)->finalize = matrix_event_receipt_finalize;

    /**
     * MatrixEventReceipt:room-id:
     *
     * The room ID.
     */
    matrix_event_receipt_properties[PROP_ROOM_ID] = g_param_spec_string(
            "room-id", "room-id", "room-id",
            NULL,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_ROOM_ID, matrix_event_receipt_properties[PROP_ROOM_ID]);
}

static void
matrix_event_receipt_init(MatrixEventReceipt *matrix_event_receipt)
{
    MatrixEventReceiptPrivate *priv;

    priv = matrix_event_receipt_get_instance_private(matrix_event_receipt);

    priv->_room_id = NULL;
    priv->_receipt_data = NULL;
}
