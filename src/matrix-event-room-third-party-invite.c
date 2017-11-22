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

#include "matrix-event-room-third-party-invite.h"
#include "matrix-types.h"
#include "config.h"

/**
 * SECTION:matrix-event-room-third-party-invite
 * @short_description: event to hold 3rd party invites
 *
 * This is the default event handler for `m.room.third_party_invite` events.
 *
 * Acts as an `m.room.member` invite event (see #MatrixEventRoomMember), where there isn’t a
 * target Matrix ID to invite.  This event contains a token and a public key whose private key
 * must be used to sign the token.  Any user who can present that signature may use this
 * invitation to join the target room.
 */
struct _MatrixThirdPartyInvitePublicKey {
    gchar* key;
    gchar* validity_url;

    guint refcount;
};

/**
 * MatrixThirdPartyInvitePublicKey:
 */
G_DEFINE_BOXED_TYPE(MatrixThirdPartyInvitePublicKey, matrix_third_party_invite_public_key, (GBoxedCopyFunc)matrix_third_party_invite_public_key_ref, (GBoxedFreeFunc)matrix_third_party_invite_public_key_unref);

MatrixThirdPartyInvitePublicKey *
matrix_third_party_invite_public_key_new(void)
{
    MatrixThirdPartyInvitePublicKey *ret = g_new0(MatrixThirdPartyInvitePublicKey, 1);

    ret->refcount = 1;

    return ret;
}

MatrixThirdPartyInvitePublicKey *
matrix_third_party_invite_public_key_ref(MatrixThirdPartyInvitePublicKey *matrix_third_party_invite_public_key)
{
    g_return_val_if_fail(matrix_third_party_invite_public_key != NULL, NULL);

    ++matrix_third_party_invite_public_key->refcount;

    return matrix_third_party_invite_public_key;
}

void
matrix_third_party_invite_public_key_unref(MatrixThirdPartyInvitePublicKey *matrix_third_party_invite_public_key)
{
    g_return_if_fail(matrix_third_party_invite_public_key != NULL);

    if (--(matrix_third_party_invite_public_key->refcount) == 0) {
        g_free(matrix_third_party_invite_public_key->key);
        g_free(matrix_third_party_invite_public_key->validity_url);
        g_free(matrix_third_party_invite_public_key);
    }
}

/**
 * matrix_third_party_invite_public_key_get_key:
 * @third_party_invite_public_key: a #MatrixThirdPartyInvitePublicKey
 *
 * Get the public key from the 3rd party invite.
 *
 * Returns: (nullable) (transfer none): the public key
 */
const gchar *
matrix_third_party_invite_public_key_get_key(MatrixThirdPartyInvitePublicKey *third_party_invite_public_key)
{
    g_return_val_if_fail(third_party_invite_public_key != NULL, NULL);

    return third_party_invite_public_key->key;
}

void
matrix_third_party_invite_public_key_set_key(MatrixThirdPartyInvitePublicKey *matrix_third_party_invite_public_key, const gchar *key)
{
    g_return_if_fail(matrix_third_party_invite_public_key != NULL);

    g_free(matrix_third_party_invite_public_key->key);
    matrix_third_party_invite_public_key->key = g_strdup(key);
}

const gchar *
matrix_third_party_invite_public_key_get_validity_url(MatrixThirdPartyInvitePublicKey *matrix_third_party_invite_public_key)
{
    g_return_val_if_fail(matrix_third_party_invite_public_key != NULL, NULL);

    return matrix_third_party_invite_public_key->validity_url;
}

void
matrix_third_party_invite_public_key_set_validity_url(MatrixThirdPartyInvitePublicKey *matrix_third_party_invite_public_key, const gchar *validity_url)
{
    g_return_if_fail(matrix_third_party_invite_public_key != NULL);

    g_free(matrix_third_party_invite_public_key->validity_url);
    matrix_third_party_invite_public_key->validity_url = g_strdup(validity_url);
}

enum  {
    PROP_0,
    PROP_DISPLAY_NAME,
    PROP_KEY_VALIDITY_URL,
    PROP_PUBLIC_KEY,
    PROP_TOKEN,
    NUM_PROPERTIES
};

static GParamSpec *matrix_event_room_third_party_invite_properties[NUM_PROPERTIES];

typedef struct {
    gchar* _display_name;
    gchar* _key_validity_url;
    gchar* _public_key;
    MatrixThirdPartyInvitePublicKey **_public_keys;
    gint _public_keys_len;
} MatrixEventRoomThirdPartyInvitePrivate;

/**
 * MatrixEventRoomThirdPartyInvite:
 */
G_DEFINE_TYPE_WITH_PRIVATE(MatrixEventRoomThirdPartyInvite, matrix_event_room_third_party_invite, MATRIX_EVENT_TYPE_STATE);

static void
matrix_event_room_third_party_invite_real_from_json(MatrixEventBase *matrix_event_base, JsonNode *json_data, GError **error)
{
    MatrixEventRoomThirdPartyInvitePrivate *priv;
    JsonObject *root;
    JsonObject *content_root;
    JsonNode *content_node;
    JsonNode *node;

    priv = matrix_event_room_third_party_invite_get_instance_private(MATRIX_EVENT_ROOM_THIRD_PARTY_INVITE(matrix_event_base));
    root = json_node_get_object(json_data);
    content_node = json_object_get_member(root, "content");
    content_root = json_node_get_object(content_node);

    if ((node = json_object_get_member(content_root, "display_name")) != NULL) {
        g_free(priv->_display_name);
        priv->_display_name = g_strdup(json_node_get_string(node));
    } else {
        g_warning("content.display_name is missing from a m.room.third_party_invite event");
    }

    if ((node = json_object_get_member(content_root, "public_key")) != NULL) {
        g_free(priv->_public_key);
        priv->_public_key = g_strdup(json_node_get_string(node));
    } else {
        g_warning("content.public_key is missing from a m.room.third_party_invite event");
    }

    if ((node = json_object_get_member(content_root, "key_validity_url")) != NULL) {
        g_free(priv->_key_validity_url);
        priv->_key_validity_url = g_strdup(json_node_get_string(node));
    } else {
        g_warning("content.key_validity_url is missing from a m.room.third_party_invite event");
    }

    if ((node = json_object_get_member(content_root, "public_keys")) != NULL) {
        JsonArray *public_key_list = json_node_get_array(node);
        priv->_public_keys_len = json_array_get_length(public_key_list);
        priv->_public_keys = g_new0(MatrixThirdPartyInvitePublicKey *, priv->_public_keys_len);

        for (gint i = 0; i < priv->_public_keys_len; i++) {
            JsonNode *member_node = json_array_get_element(public_key_list, i);
            JsonObject *member_root = json_node_get_object(member_node);
            priv->_public_keys[i] = matrix_third_party_invite_public_key_new();

            if ((node = json_object_get_member(member_root, "public_key")) != NULL) {
                priv->_public_keys[i]->key = g_strdup(json_node_get_string(node));
            } else {
                g_warning("content.public_keys[].public_key is missing from an m.room.third_party_invite event");
            }

            if ((node = json_object_get_member(member_root, "validity_url")) != NULL) {
                priv->_public_keys[i]->validity_url = g_strdup(json_node_get_string(node));
            } else {
                g_warning("content.public_keys[].validity_url is missing from an m.room.third_party_invite event");
            }
        }
    }

    MATRIX_EVENT_BASE_CLASS(matrix_event_room_third_party_invite_parent_class)->from_json((MatrixEventBase*)matrix_event_base, json_data, error);
}

static void
matrix_event_room_third_party_invite_real_to_json(MatrixEventBase *matrix_event_base, JsonNode *json_data, GError **error)
{
    MatrixEventRoomThirdPartyInvitePrivate *priv;
    JsonObject *root;
    JsonObject *content_root;
    JsonArray *key_list;
    JsonNode *content_node;

    priv = matrix_event_room_third_party_invite_get_instance_private(MATRIX_EVENT_ROOM_THIRD_PARTY_INVITE(matrix_event_base));
    root = json_node_get_object(json_data);
    content_node = json_object_get_member(root, "content");
    content_root = json_node_get_object(content_node);

    if (priv->_display_name == NULL) {
        g_set_error(error, MATRIX_ERROR, MATRIX_ERROR_INCOMPLETE,
                    "Won't generate a m.room.third_party_invite event without display_name");

        return;
    }

    if (priv->_public_key == NULL) {
        g_set_error(error, MATRIX_ERROR, MATRIX_ERROR_INCOMPLETE,
                    "Won't generate a m.room.third_party_invite without public_key");

        return;
    }

    if (priv->_key_validity_url == NULL) {
        g_set_error(error, MATRIX_ERROR, MATRIX_ERROR_INCOMPLETE,
                    "Won't generate a m.room.third_party_invite without key_validity_url");

        return;
    }

    json_object_set_string_member(content_root, "display_name", priv->_display_name);
    json_object_set_string_member(content_root, "public_key", priv->_public_key);
    json_object_set_string_member(content_root, "key_validity_url", priv->_key_validity_url);

    key_list = json_array_new();

    for (gint i = 0; i < priv->_public_keys_len; i++) {
        JsonObject *key_root;
        JsonNode *key_node;
        MatrixThirdPartyInvitePublicKey *entry = priv->_public_keys[i];

        if (entry->key == NULL) {
            g_set_error(error, MATRIX_ERROR, MATRIX_ERROR_INCOMPLETE,
                        "Won't generate a m.room.third_party_invite with a missing key under public_keys");

            return;
        }

        key_root = json_object_new();
        key_node = json_node_new(JSON_NODE_OBJECT);
        json_node_set_object(key_node, key_root);
        json_object_set_string_member(key_root, "public_key", entry->key);

        if (entry->validity_url != NULL) {
            json_object_set_string_member(key_root, "key_validity_url", entry->validity_url);
        }

        json_array_add_element(key_list, key_node);
    }

    if (json_array_get_length(key_list) > 0) {
        JsonNode *keys_node = json_node_new(JSON_NODE_ARRAY);

        json_node_set_array(keys_node, key_list);
        json_object_set_member(content_root, "public_keys", keys_node);
    }

    MATRIX_EVENT_BASE_CLASS(matrix_event_room_third_party_invite_parent_class)->to_json(matrix_event_base, json_data, error);
}

MatrixEventRoomThirdPartyInvite *matrix_event_room_third_party_invite_new(void)
{
    return (MatrixEventRoomThirdPartyInvite *)matrix_event_state_construct(MATRIX_EVENT_TYPE_ROOM_THIRD_PARTY_INVITE);
}

const gchar *
matrix_event_room_third_party_invite_get_display_name(MatrixEventRoomThirdPartyInvite *matrix_event_room_third_party_invite)
{
    MatrixEventRoomThirdPartyInvitePrivate *priv;

    g_return_val_if_fail(matrix_event_room_third_party_invite != NULL, NULL);

    priv = matrix_event_room_third_party_invite_get_instance_private(matrix_event_room_third_party_invite);

    return priv->_display_name;
}

void
matrix_event_room_third_party_invite_set_display_name(MatrixEventRoomThirdPartyInvite *matrix_event_room_third_party_invite, const gchar *display_name)
{
    MatrixEventRoomThirdPartyInvitePrivate *priv;

    g_return_if_fail(matrix_event_room_third_party_invite != NULL);

    priv = matrix_event_room_third_party_invite_get_instance_private(matrix_event_room_third_party_invite);

    if (g_strcmp0(display_name, priv->_display_name) != 0) {
        g_free(priv->_display_name);
        priv->_display_name = g_strdup(display_name);

        g_object_notify_by_pspec((GObject *)matrix_event_room_third_party_invite, matrix_event_room_third_party_invite_properties[PROP_DISPLAY_NAME]);
    }
}

const gchar *
matrix_event_room_third_party_invite_get_key_validity_url(MatrixEventRoomThirdPartyInvite *matrix_event_room_third_party_invite)
{
    MatrixEventRoomThirdPartyInvitePrivate *priv;

    g_return_val_if_fail(matrix_event_room_third_party_invite != NULL, NULL);

    priv = matrix_event_room_third_party_invite_get_instance_private(matrix_event_room_third_party_invite);

    return priv->_key_validity_url;
}

void
matrix_event_room_third_party_invite_set_key_validity_url(MatrixEventRoomThirdPartyInvite *matrix_event_room_third_party_invite, const gchar *key_validity_url)
{
    MatrixEventRoomThirdPartyInvitePrivate *priv;

    g_return_if_fail(matrix_event_room_third_party_invite != NULL);

    priv = matrix_event_room_third_party_invite_get_instance_private(matrix_event_room_third_party_invite);

    if (g_strcmp0(key_validity_url, priv->_key_validity_url) != 0) {
        g_free(priv->_key_validity_url);
        priv->_key_validity_url = g_strdup(key_validity_url);

        g_object_notify_by_pspec((GObject *)matrix_event_room_third_party_invite, matrix_event_room_third_party_invite_properties[PROP_KEY_VALIDITY_URL]);
    }
}

const gchar *
matrix_event_room_third_party_invite_get_public_key(MatrixEventRoomThirdPartyInvite *matrix_event_room_third_party_invite)
{
    MatrixEventRoomThirdPartyInvitePrivate *priv;

    g_return_val_if_fail(matrix_event_room_third_party_invite != NULL, NULL);

    priv = matrix_event_room_third_party_invite_get_instance_private(matrix_event_room_third_party_invite);

    return priv->_public_key;
}

void
matrix_event_room_third_party_invite_set_public_key(MatrixEventRoomThirdPartyInvite *matrix_event_room_third_party_invite, const gchar *public_key)
{
    MatrixEventRoomThirdPartyInvitePrivate *priv;

    g_return_if_fail(matrix_event_room_third_party_invite != NULL);

    priv = matrix_event_room_third_party_invite_get_instance_private(matrix_event_room_third_party_invite);

    if (g_strcmp0(public_key, priv->_public_key) != 0) {
        g_free(priv->_public_key);
        priv->_public_key = g_strdup(public_key);

        g_object_notify_by_pspec((GObject *)matrix_event_room_third_party_invite, matrix_event_room_third_party_invite_properties[PROP_PUBLIC_KEY]);
    }
}

const MatrixThirdPartyInvitePublicKey **
matrix_event_room_third_party_invite_get_public_keys(MatrixEventRoomThirdPartyInvite *matrix_event_room_third_party_invite, int *n_public_keys)
{
    MatrixEventRoomThirdPartyInvitePrivate *priv;

    g_return_val_if_fail(matrix_event_room_third_party_invite != NULL, NULL);

    priv = matrix_event_room_third_party_invite_get_instance_private(matrix_event_room_third_party_invite);

    if (n_public_keys != NULL) {
        *n_public_keys = priv->_public_keys_len;
    }

    return (const MatrixThirdPartyInvitePublicKey **)priv->_public_keys;
}

void
matrix_event_room_third_party_invite_set_public_keys(MatrixEventRoomThirdPartyInvite *matrix_event_room_third_party_invite, MatrixThirdPartyInvitePublicKey **public_keys, int n_public_keys)
{
    MatrixEventRoomThirdPartyInvitePrivate *priv;

    g_return_if_fail(matrix_event_room_third_party_invite != NULL);

    priv = matrix_event_room_third_party_invite_get_instance_private(matrix_event_room_third_party_invite);

    for (gint i = 0; i < priv->_public_keys_len; i++) {
        matrix_third_party_invite_public_key_unref(priv->_public_keys[i]);
    }

    g_free(priv->_public_keys);

    priv->_public_keys = g_new0(MatrixThirdPartyInvitePublicKey *, n_public_keys);

    for (gint i = 0; i < n_public_keys; i++) {
        priv->_public_keys[i] = matrix_third_party_invite_public_key_ref(public_keys[i]);
    }

    priv->_public_keys_len = n_public_keys;
}

const gchar *
matrix_event_room_third_party_invite_get_token(MatrixEventRoomThirdPartyInvite *matrix_event_room_third_party_invite)
{
    g_return_val_if_fail(matrix_event_room_third_party_invite != NULL, NULL);

    return matrix_event_state_get_state_key(MATRIX_EVENT_STATE(matrix_event_room_third_party_invite));
}

void
matrix_event_room_third_party_invite_set_token(MatrixEventRoomThirdPartyInvite *matrix_event_room_third_party_invite, const gchar *token)
{
    const gchar *state_key;

    g_return_if_fail(matrix_event_room_third_party_invite != NULL);

    state_key = matrix_event_state_get_state_key(MATRIX_EVENT_STATE(matrix_event_room_third_party_invite));

    if (g_strcmp0(token, state_key) != 0) {
        matrix_event_state_set_state_key(MATRIX_EVENT_STATE(matrix_event_room_third_party_invite), token);

        g_object_notify_by_pspec ((GObject *) matrix_event_room_third_party_invite, matrix_event_room_third_party_invite_properties[PROP_TOKEN]);
    }
}

static void
matrix_event_room_third_party_invite_finalize(GObject *gobject)
{
    MatrixEventRoomThirdPartyInvitePrivate *priv = matrix_event_room_third_party_invite_get_instance_private(MATRIX_EVENT_ROOM_THIRD_PARTY_INVITE(gobject));

    g_free(priv->_display_name);
    g_free(priv->_key_validity_url);
    g_free(priv->_public_key);

    for (gint i = 0; i < priv->_public_keys_len; i++) {
        matrix_third_party_invite_public_key_unref(priv->_public_keys[i]);
    }

    g_free(priv->_public_keys);

    G_OBJECT_CLASS (matrix_event_room_third_party_invite_parent_class)->finalize(gobject);
}

static void
matrix_event_room_third_party_invite_get_property(GObject *gobject, guint property_id, GValue *value, GParamSpec *pspec)
{
    MatrixEventRoomThirdPartyInvite *matrix_event_room_third_party_invite = MATRIX_EVENT_ROOM_THIRD_PARTY_INVITE(gobject);

    switch (property_id) {
        case PROP_DISPLAY_NAME:
            g_value_set_string(value, matrix_event_room_third_party_invite_get_display_name(matrix_event_room_third_party_invite));

            break;
        case PROP_KEY_VALIDITY_URL:
            g_value_set_string(value, matrix_event_room_third_party_invite_get_key_validity_url(matrix_event_room_third_party_invite));

            break;
        case PROP_PUBLIC_KEY:
            g_value_set_string(value, matrix_event_room_third_party_invite_get_public_key(matrix_event_room_third_party_invite));

            break;
        case PROP_TOKEN:
            g_value_set_string(value, matrix_event_room_third_party_invite_get_token(matrix_event_room_third_party_invite));

            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, property_id, pspec);
        break;
    }
}

static void
matrix_event_room_third_party_invite_set_property(GObject *gobject, guint property_id, const GValue *value, GParamSpec *pspec) {
    MatrixEventRoomThirdPartyInvite *matrix_event_room_third_party_invite = MATRIX_EVENT_ROOM_THIRD_PARTY_INVITE(gobject);

    switch(property_id) {
        case PROP_DISPLAY_NAME:
            matrix_event_room_third_party_invite_set_display_name(matrix_event_room_third_party_invite, g_value_get_string(value));

            break;
        case PROP_KEY_VALIDITY_URL:
            matrix_event_room_third_party_invite_set_key_validity_url(matrix_event_room_third_party_invite, g_value_get_string(value));

            break;
        case PROP_PUBLIC_KEY:
            matrix_event_room_third_party_invite_set_public_key(matrix_event_room_third_party_invite, g_value_get_string(value));

            break;
        case PROP_TOKEN:
            matrix_event_room_third_party_invite_set_token(matrix_event_room_third_party_invite, g_value_get_string(value));

            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, property_id, pspec);

            break;
    }
}

static void
matrix_event_room_third_party_invite_class_init(MatrixEventRoomThirdPartyInviteClass *klass)
{
    ((MatrixEventBaseClass *)klass)->from_json = matrix_event_room_third_party_invite_real_from_json;
    ((MatrixEventBaseClass *)klass)->to_json = matrix_event_room_third_party_invite_real_to_json;
    G_OBJECT_CLASS(klass)->get_property = matrix_event_room_third_party_invite_get_property;
    G_OBJECT_CLASS(klass)->set_property = matrix_event_room_third_party_invite_set_property;
    G_OBJECT_CLASS(klass)->finalize = matrix_event_room_third_party_invite_finalize;

    /**
     * MatrixEventRoomThirdPartyInvite:display-name:
     *
     * A user-readable string which represents the user who has been
     * invited. This should not contain the user's third party ID, as
     * otherwise when the invite is accepted it would leak the
     * association between the matrix ID and the third party ID.
     */
    matrix_event_room_third_party_invite_properties[PROP_DISPLAY_NAME] = g_param_spec_string(
            "display-name", "display-name", "display-name",
            NULL,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_DISPLAY_NAME, matrix_event_room_third_party_invite_properties[PROP_DISPLAY_NAME]);

    /**
     * MatrixEventRoomThirdPartyInvite:key-validity-url:
     *
     * A URL which can be fetched, with querystring
     * public_key=public_key, to validate whether the key has been
     * revoked. The URL must return a JSON object containing a boolean
     * property named valid.
     */
    matrix_event_room_third_party_invite_properties[PROP_KEY_VALIDITY_URL] = g_param_spec_string(
            "key-validity-url", "key-validity-url", "key-validity-url",
            NULL,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_KEY_VALIDITY_URL, matrix_event_room_third_party_invite_properties[PROP_KEY_VALIDITY_URL]);

    /**
     * MatrixEventRoomThirdPartyInvite:public-key:
     *
     * A base64-encoded ed25519 key with which token must be signed
     * (though a signature from any entry in public_keys is also
     * sufficient). This exists for backwards compatibility.
     */
    matrix_event_room_third_party_invite_properties[PROP_PUBLIC_KEY] = g_param_spec_string(
            "public-key", "public-key", "public-key",
            NULL,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_PUBLIC_KEY, matrix_event_room_third_party_invite_properties[PROP_PUBLIC_KEY]);

    /**
     * MatrixEventRoomThirdPartyInvite:token:
     *
     * The token, of which a signature must be produced in order to
     * join the room.
     */
    matrix_event_room_third_party_invite_properties[PROP_TOKEN] = g_param_spec_string(
            "token", "token", "token",
            NULL,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_TOKEN, matrix_event_room_third_party_invite_properties[PROP_TOKEN]);
}

static void
matrix_event_room_third_party_invite_init(MatrixEventRoomThirdPartyInvite *matrix_event_room_third_party_invite)
{
    MatrixEventRoomThirdPartyInvitePrivate *priv = matrix_event_room_third_party_invite_get_instance_private(matrix_event_room_third_party_invite);

    priv->_display_name = NULL;
    priv->_key_validity_url = NULL;
    priv->_public_key = NULL;
}
