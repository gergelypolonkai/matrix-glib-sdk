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

#include "matrix-event-call-invite.h"
#include "utils.h"
#include "config.h"
#include "matrix-enumtypes.h"

/**
 * SECTION:matrix-event-call-invite
 * @short_description: event to signal a call request
 *
 * This event is sent by the caller when they wish to establish a call.
 */
enum  {
    PROP_0,
    PROP_OFFER_TYPE,
    PROP_SDP,
    PROP_LIFETIME,
    NUM_PROPERTIES
};

static GParamSpec *matrix_event_call_invite_properties[NUM_PROPERTIES];

typedef struct {
    MatrixCallOfferType _offer_type;
    gchar* _sdp;
    gint _lifetime;
} MatrixEventCallInvitePrivate;

/**
 * MatrixEventCallInvite:
 */
G_DEFINE_TYPE_WITH_PRIVATE(MatrixEventCallInvite, matrix_event_call_invite, MATRIX_EVENT_TYPE_CALL);

static void
matrix_event_call_invite_real_from_json(MatrixEventBase *matrix_event_base, JsonNode *json_data, GError **error)
{
    MatrixEventCallInvitePrivate *priv;
    JsonObject *root;
    JsonObject *content_root;
    JsonNode *content_node;
    JsonNode *node;

    g_return_if_fail (json_data != NULL);

    priv = matrix_event_call_invite_get_instance_private(MATRIX_EVENT_CALL_INVITE(matrix_event_base));

    root = json_node_get_object(json_data);
    content_node = json_object_get_member(root, "content");
    content_root = json_node_get_object(content_node);

    if ((node = json_object_get_member(content_root, "offer")) != NULL) {
        JsonObject *offer_root = json_node_get_object(node);

        if ((node = json_object_get_member(offer_root, "type")) != NULL) {
            GError *inner_error = NULL;
            MatrixCallOfferType offer_type = _matrix_g_enum_nick_to_value(MATRIX_TYPE_CALL_OFFER_TYPE, json_node_get_string(node), &inner_error);

            if (inner_error != NULL) {
                priv->_offer_type = MATRIX_CALL_OFFER_TYPE_UNKNOWN;

#ifdef DEBUG
                g_warning("Unknown value %s in content.offer.type of a m.call.invite event", json_node_get_string(node));
#endif
            } else {
                priv->_offer_type = offer_type;
            }
        } else {
            g_warning("content.offer.type is missing from a m.call.invite event");
        }

        if ((node = json_object_get_member(offer_root, "sdp")) != NULL) {
            g_free(priv->_sdp);
            priv->_sdp = g_strdup(json_node_get_string(node));
        } else {
            g_warning("content.offer.sdp is missing from a m.call.invite event");
        }
    }

    if ((node = json_object_get_member(content_root, "lifetime")) != NULL) {
        priv->_lifetime = json_node_get_int(node);
    } else {
        g_warning("content.lifetime is missing from a m.call.invite event");
    }

    MATRIX_EVENT_BASE_CLASS(matrix_event_call_invite_parent_class)->from_json(matrix_event_base, json_data, error);
}

static void
matrix_event_call_invite_real_to_json(MatrixEventBase *matrix_event_base, JsonNode *json_data, GError **error)
{
    MatrixEventCallInvitePrivate *priv;
    JsonObject *root;
    JsonObject *content_root;
    JsonObject *offer_root;
    JsonNode *content_node;
    JsonNode *offer_node;
    gchar *offer_type;

    g_return_if_fail(json_data != NULL);

    priv = matrix_event_call_invite_get_instance_private(MATRIX_EVENT_CALL_INVITE(matrix_event_base));

    if (priv->_offer_type == MATRIX_CALL_OFFER_TYPE_UNKNOWN) {
        g_set_error(error, MATRIX_ERROR, MATRIX_ERROR_INCOMPLETE,
                    "Won't generate a m.call.invite without a valid offer.type");

        return;
    }

    if (priv->_sdp == NULL) {
        g_set_error(error, MATRIX_ERROR, MATRIX_ERROR_INCOMPLETE,
                    "Won't generate a m.call.invite without offer.sdp");

        return;
    }

    if (priv->_lifetime < 0) {
        g_set_error(error, MATRIX_ERROR, MATRIX_ERROR_INCOMPLETE,
                    "Won't generate a m.call.invite without lifetime");

        return;
    }

    root = json_node_get_object(json_data);
    content_node = json_object_get_member(root, "content");
    content_root = json_node_get_object(content_node);

    json_object_set_int_member(content_root, "lifetime", priv->_lifetime);

    offer_root = json_object_new();
    offer_node = json_node_new(JSON_NODE_OBJECT);
    json_node_set_object(offer_node, offer_root);

    offer_type = _matrix_g_enum_to_string(MATRIX_TYPE_CALL_OFFER_TYPE, priv->_offer_type, '_');
    json_object_set_string_member(offer_root, "type", offer_type);
    g_free(offer_type);

    json_object_set_string_member(offer_root, "sdp", priv->_sdp);

    json_object_set_member(content_root, "offer", offer_node);

    MATRIX_EVENT_BASE_CLASS(matrix_event_call_invite_parent_class)->to_json(matrix_event_base, json_data, error);
}

/**
 * matrix_event_call_invite_new:
 *
 * Create a new #MatrixEventCallInvite object.
 *
 * Returns: (transfer full): a new #MatrixEventCallInvite object
 */
MatrixEventCallInvite *
matrix_event_call_invite_new(void) {
    return (MatrixEventCallInvite *)matrix_event_call_construct(MATRIX_EVENT_TYPE_CALL_INVITE);
}

/**
 * matrix_event_call_invite_get_offer_type:
 * @event: a #MatrixEventCallInvite
 *
 * Get the offer type of @event.
 *
 * Returns: the call offer type
 */
MatrixCallOfferType
matrix_event_call_invite_get_offer_type(MatrixEventCallInvite *matrix_event_call_invite)
{
    MatrixEventCallInvitePrivate *priv;

    g_return_val_if_fail(matrix_event_call_invite != NULL, 0);

    priv = matrix_event_call_invite_get_instance_private(matrix_event_call_invite);

    return priv->_offer_type;
}

/**
 * matrix_event_call_invite_set_offer_type:
 * @event: a #MatrixEventCallInvite
 * @offer_type: the type of the call offer
 *
 * Set the offer type of the call in @event.
 */
void
matrix_event_call_invite_set_offer_type(MatrixEventCallInvite *matrix_event_call_invite, MatrixCallOfferType offer_type)
{
    MatrixEventCallInvitePrivate *priv;

    g_return_if_fail(matrix_event_call_invite != NULL);


    priv = matrix_event_call_invite_get_instance_private(matrix_event_call_invite);

    if (priv->_offer_type != offer_type) {
        priv->_offer_type = offer_type;

        g_object_notify_by_pspec((GObject *)matrix_event_call_invite, matrix_event_call_invite_properties[PROP_OFFER_TYPE]);
    }
}

/**
 * matrix_event_call_invite_get_sdp:
 * @event: a #MatrixEventCallInvite
 *
 * Get the SDP line of the call.
 *
 * The returned value is owned by @event and should not by freed.
 *.
 * Returns: (transfer none): the SDP line of the call
 */
const gchar *
matrix_event_call_invite_get_sdp(MatrixEventCallInvite *matrix_event_call_invite)
{
    MatrixEventCallInvitePrivate *priv;

    g_return_val_if_fail(matrix_event_call_invite != NULL, NULL);

    priv = matrix_event_call_invite_get_instance_private(matrix_event_call_invite);

    return priv->_sdp;
}

/**
 * matrix_event_call_invite_set_sdp:
 * @event: a #MatrixEventCallInvite
 * @sdp: an SDP line
 *
 * Set the SDP line ID of the call.
 */
void
matrix_event_call_invite_set_sdp(MatrixEventCallInvite *matrix_event_call_invite, const gchar *sdp)
{
    MatrixEventCallInvitePrivate *priv;

    g_return_if_fail(matrix_event_call_invite != NULL);

    priv = matrix_event_call_invite_get_instance_private(matrix_event_call_invite);

    if (g_strcmp0(sdp, priv->_sdp) != 0) {
        g_free(priv->_sdp);
        priv->_sdp = g_strdup(sdp);

        g_object_notify_by_pspec((GObject *)matrix_event_call_invite, matrix_event_call_invite_properties[PROP_SDP]);
    }
}

/**
 * matrix_event_call_invite_get_lifetime:
 * @event: a #MatrixEventCallInvite
 *
 * Get the lifetime of the call in @event.
 *
 * Returns: the lifetime, in milliseconds.
 */
gint
matrix_event_call_invite_get_lifetime(MatrixEventCallInvite *matrix_event_call_invite)
{
    MatrixEventCallInvitePrivate *priv;

    g_return_val_if_fail(matrix_event_call_invite != NULL, 0);

    priv = matrix_event_call_invite_get_instance_private(matrix_event_call_invite);

    return priv->_lifetime;
}

/**
 * matrix_event_call_invite_set_lifetime:
 * @event: a #MatrixEventCallInvite
 * @lifetime: the lifetime of @event
 *
 * Set the lifetime of @event, in milliseconds.
 */
void
matrix_event_call_invite_set_lifetime(MatrixEventCallInvite *matrix_event_call_invite, gint lifetime)
{
    MatrixEventCallInvitePrivate *priv;

    g_return_if_fail(matrix_event_call_invite != NULL);

    priv = matrix_event_call_invite_get_instance_private(matrix_event_call_invite);

    if (priv->_lifetime != lifetime) {
        priv->_lifetime = lifetime;

        g_object_notify_by_pspec((GObject *)matrix_event_call_invite, matrix_event_call_invite_properties[PROP_LIFETIME]);
    }
}

static void
matrix_event_call_invite_finalize(GObject *gobject)
{
    MatrixEventCallInvitePrivate *priv = matrix_event_call_invite_get_instance_private(MATRIX_EVENT_CALL_INVITE(gobject));

    g_free(priv->_sdp);

    G_OBJECT_CLASS(matrix_event_call_invite_parent_class)->finalize(gobject);
}

static void
matrix_event_call_invite_get_property(GObject *gobject, guint property_id, GValue *value, GParamSpec *pspec)
{
    MatrixEventCallInvite *matrix_event_call_invite = MATRIX_EVENT_CALL_INVITE(gobject);

    switch (property_id) {
        case PROP_OFFER_TYPE:
            g_value_set_enum(value, matrix_event_call_invite_get_offer_type(matrix_event_call_invite));

            break;
        case PROP_SDP:
            g_value_set_string(value, matrix_event_call_invite_get_sdp(matrix_event_call_invite));

            break;
        case PROP_LIFETIME:
            g_value_set_int(value, matrix_event_call_invite_get_lifetime(matrix_event_call_invite));

            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, property_id, pspec);

            break;
    }
}

static void
matrix_event_call_invite_set_property(GObject *gobject, guint property_id, const GValue *value, GParamSpec *pspec)
{
    MatrixEventCallInvite *matrix_event_call_invite = MATRIX_EVENT_CALL_INVITE(gobject);

    switch (property_id) {
        case PROP_OFFER_TYPE:
            matrix_event_call_invite_set_offer_type(matrix_event_call_invite, g_value_get_enum(value));

            break;
        case PROP_SDP:
            matrix_event_call_invite_set_sdp(matrix_event_call_invite, g_value_get_string(value));

            break;
        case PROP_LIFETIME:
            matrix_event_call_invite_set_lifetime(matrix_event_call_invite, g_value_get_int(value));

            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, property_id, pspec);

            break;
    }
}

static void
matrix_event_call_invite_class_init(MatrixEventCallInviteClass *klass)
{
    ((MatrixEventBaseClass *)klass)->from_json = matrix_event_call_invite_real_from_json;
    ((MatrixEventBaseClass *)klass)->to_json = matrix_event_call_invite_real_to_json;
    G_OBJECT_CLASS (klass)->get_property = matrix_event_call_invite_get_property;
    G_OBJECT_CLASS (klass)->set_property = matrix_event_call_invite_set_property;
    G_OBJECT_CLASS (klass)->finalize = matrix_event_call_invite_finalize;

    /**
     * MatrixEventCallInvite:offer-type:
     *
     * The type of session description.
     */
    matrix_event_call_invite_properties[PROP_OFFER_TYPE] = g_param_spec_enum(
            "offer-type", "offer-type", "offer-type",
            MATRIX_TYPE_CALL_OFFER_TYPE, MATRIX_CALL_OFFER_TYPE_UNKNOWN,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_OFFER_TYPE, matrix_event_call_invite_properties[PROP_OFFER_TYPE]);

    /**
     * MatrixEventCallInvite:sdp:
     *
     * The SDP text of the session description.
     */
    matrix_event_call_invite_properties[PROP_SDP] = g_param_spec_string(
            "sdp", "sdp", "sdp",
            NULL,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_SDP, matrix_event_call_invite_properties[PROP_SDP]);

    /**
     * MatrixEventCallInvite:lifetime:
     *
     * The time in milliseconds that the invite is valid for. Once the invite age exceeds this
     * value, clients should discard it.  They should also no longer show the call as awaiting
     * an answer in the UI.
     */
    matrix_event_call_invite_properties[PROP_LIFETIME] = g_param_spec_int(
            "lifetime", "lifetime", "lifetime",
            G_MININT, G_MAXINT, -1,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_LIFETIME, matrix_event_call_invite_properties[PROP_LIFETIME]);
}

static void
matrix_event_call_invite_init(MatrixEventCallInvite *matrix_event_call_invite)
{
    MatrixEventCallInvitePrivate *priv = matrix_event_call_invite_get_instance_private(matrix_event_call_invite);

    priv->_offer_type = MATRIX_CALL_OFFER_TYPE_UNKNOWN;
    priv->_sdp = NULL;
    priv->_lifetime = -1;
}
