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

#include "matrix-event-room-member.h"
#include "config.h"
#include "utils.h"

/**
 * SECTION:matrix-event-room-member
 * @short_description: event representing room membership
 *
 * This is the default event handler for `m.room.member` events.
 *
 * Adjusts the membership state for a user in a room.  It is preferable to use the membership
 * APIs like matrix_api_invite_user() when performing membership actions rather than adjusting
 * the state directly as there are a restricted set of valid transformations.  For example,
 * user A cannot force user B to join a room, and trying to force this state change directly
 * will fail.
 *
 * The following membership states are specified:
 *
 * * `invite` - The user has been invited to join a room, but has not yet joined it.  They may
 *     not participate in the room until they join.
 * * `join` - The user has joined the room (possibly after accepting an invite), and may
 *     participate in it.
 * * `leave` - The user was once joined to the room, but has since left (possibly by choice,
 *     or possibly by being kicked).
 * * `ban` - The user has been banned from the room, and is no longer allowed to join it until
 *     they are un-banned from the room (by having their membership state set to a value other
 *     than ban).
 * * `knock` - This is a reserved word, which currently has no meaning.
 *
 * See also #MatrixRoomMembership for more information.
 *
 * The MatrixEventRoomMember:third-party-invite property will be set if this invite is an
 * invite event and is the successor of an `m.room.third_party_invite` event, and absent
 * otherwise.
 *
 * This event may also include an `invite_room_state` key outside the content key.  If present,
 * this contains an array of stripped state events.  These events provide information on a few
 * select state events such as the room name.
 */

enum  {
    PROP_0,
    PROP_MEMBERSHIP,
    PROP_AVATAR_URL,
    PROP_DISPLAY_NAME,
    PROP_TPI_DISPLAY_NAME,
    PROP_TPI_SIGNED_MXID,
    PROP_TPI_SIGNED_TOKEN,
    PROP_TPI_SIGNATURE,
    PROP_USER_ID,
    NUM_PROPERTIES
};

static GParamSpec* matrix_event_room_member_properties[NUM_PROPERTIES];

typedef struct {
    MatrixRoomMembership _membership;
    gchar* _avatar_url;
    gchar* _display_name;
    gchar* _tpi_display_name;
    gchar* _tpi_signed_mxid;
    gchar* _tpi_signed_token;
    JsonNode* _tpi_signature;
    MatrixEventState** _invite_room_state;
    gint _invite_room_state_len;
    gint __invite_room_state_size_;
} MatrixEventRoomMemberPrivate;

/**
 * MatrixEventRoomMember:
 */
G_DEFINE_TYPE_WITH_PRIVATE(MatrixEventRoomMember, matrix_event_room_member, MATRIX_EVENT_TYPE_STATE);

static void
matrix_event_room_member_real_from_json(MatrixEventBase *matrix_event_base, JsonNode *json_data, GError **error)
{
    MatrixEventRoomMemberPrivate *priv;
    JsonObject *root;
    JsonObject *content_root;
    JsonNode *content_node;
    JsonNode *node;
    GError *inner_error = NULL;

    g_return_if_fail(json_data != NULL);

    priv = matrix_event_room_member_get_instance_private(MATRIX_EVENT_ROOM_MEMBER(matrix_event_base));
    root = json_node_get_object(json_data);
    content_node = json_object_get_member(root, "content");
    content_root = json_node_get_object(content_node);

    // Even though the state_key is handled by the parent class,
    // in this event type this actually means the sender
    if ((node = json_object_get_member(root, "state_key")) != NULL) {
        matrix_event_state_set_state_key(MATRIX_EVENT_STATE(matrix_event_base), json_node_get_string(node));
    } else {
        g_warning("state_key is missing from a m.room.member event");
    }

    if ((node = json_object_get_member(content_root, "membership")) != NULL) {
        MatrixRoomMembership membership;

        membership = _matrix_g_enum_nick_to_value(MATRIX_TYPE_ROOM_MEMBERSHIP, json_node_get_string(node), &inner_error);

        if (inner_error != NULL) {
            g_clear_error(&inner_error);
            priv->_membership = MATRIX_ROOM_MEMBERSHIP_UNKNOWN;

            if (DEBUG) {
                g_warning("Unknown membership value %s", json_node_get_string(node));
            }
        } else {
            priv->_membership = membership;
        }
    } else if (DEBUG) {
        g_warning("membership key is missing from the m.room.member event");
    }

    if ((node = json_object_get_member(content_root, "avatar_url")) != NULL) {
        g_free(priv->_avatar_url);
        priv->_avatar_url = g_strdup(json_node_get_string(node));
    }

    if ((node = json_object_get_member(content_root, "displayname")) != NULL) {
        g_free(priv->_display_name);
        priv->_display_name = g_strdup(json_node_get_string(node));
    }

    if ((node = json_object_get_member(content_root, "third_party_invite")) != NULL) {
        JsonObject *tpi_root = json_node_get_object(node);

        if ((node = json_object_get_member(tpi_root, "display_name")) != NULL) {
            g_free(priv->_tpi_display_name);
            priv->_tpi_display_name = g_strdup(json_node_get_string(node));
        } else {
            g_warning("content.third_party_invite.display_name is missing from a m.room.member event");
        }

        if ((node = json_object_get_member(tpi_root, "signed")) != NULL) {
            JsonObject *signed_root = json_node_get_object(node);

            if ((node = json_object_get_member(signed_root, "mxid")) != NULL) {
                g_free(priv->_tpi_signed_mxid);
                priv->_tpi_signed_mxid = g_strdup(json_node_get_string(node));
            } else {
                g_warning("content.third_party_invit.signed.mxid is missing from a m.room.member event");
            }

            if ((node = json_object_get_member(signed_root, "token")) != NULL) {
                g_free(priv->_tpi_signed_token);
                priv->_tpi_signed_token = g_strdup(json_node_get_string(node));
            } else {
                g_warning("content.third_party_invite.signed.token is missing from a m.room.member event");
            }

            if ((node = json_object_get_member(signed_root, "signatures")) != NULL) {
                json_node_unref(priv->_tpi_signature);
                priv->_tpi_signature = json_node_ref(node);
            } else {
                g_warning("content.third_party_invite.signed.signatures is missing from a m.room.member event");
            }
        } else {
            g_warning("content.third_party_invite.signed is missing from a m.room.member event");
        }
    }

    if ((node = json_object_get_member(root, "invite_room_state")) != NULL) {
        JsonArray *events = json_node_get_array(node);
        gint events_len;

        if ((events_len = json_array_get_length(events)) > 0) {
            for (gint i = 0; i < priv->_invite_room_state_len; i++) {
                g_object_unref(priv->_invite_room_state[i]);
            }

            g_free(priv->_invite_room_state);

            priv->_invite_room_state_len = events_len;
            priv->_invite_room_state = g_new(MatrixEventState *, events_len);

            for (gint i = 0; i < events_len; i++) {
                JsonNode *member_node = json_array_get_element(events, i);
                MatrixEventState *event;
                GError *error = NULL;

                event = (MatrixEventState *)matrix_event_base_new_from_json(NULL, member_node, &error);

                if (error == NULL) {
                    priv->_invite_room_state[i] = g_object_ref(event);
                }

                g_object_unref(event);
            }
        }
    }

    // Chain up
    MATRIX_EVENT_BASE_CLASS(matrix_event_room_member_parent_class)->from_json(matrix_event_base, json_data, &inner_error);

    if (inner_error != NULL) {
        g_propagate_error(error, inner_error);
    }
}

static void
matrix_event_room_member_real_to_json(MatrixEventBase *matrix_event_base, JsonNode *json_data, GError **error)
{
    MatrixEventRoomMemberPrivate *priv;
    const gchar *state_key;
    gchar *membership;
    JsonObject *root;
    JsonObject *content_root;
    JsonObject *tpi_root;
    JsonObject *tpi_signed_root;
    JsonNode *content_node;
    GError *inner_error = NULL;

    priv = matrix_event_room_member_get_instance_private(MATRIX_EVENT_ROOM_MEMBER(matrix_event_base));

    if (priv->_membership == MATRIX_ROOM_MEMBERSHIP_UNKNOWN) {
        g_set_error(error, MATRIX_ERROR, MATRIX_ERROR_UNKNOWN_VALUE,
                    "Unknown membership value cannot be added to a room member event");

        return;
    }

    state_key = matrix_event_state_get_state_key(MATRIX_EVENT_STATE(matrix_event_base));

    if ((state_key == NULL) || (*state_key == 0)) {
        g_set_error(error, MATRIX_ERROR, MATRIX_ERROR_INCOMPLETE,
                    "Won't generate a m.room.member event with an empty state_key");

        return;
    }

    root = json_node_get_object(json_data);
    content_node = json_object_get_member(root, "content");
    content_root = json_node_get_object(content_node);
    membership = _matrix_g_enum_to_string(MATRIX_TYPE_ROOM_MEMBERSHIP, priv->_membership, '_');

    if (membership != NULL) {
        json_object_set_string_member(content_root, "membership", membership);
        g_free(membership);
    } else {
        g_critical("Won't generate a m.room.member event with an unknown membership");
    }

    if (priv->_avatar_url != NULL) {
        json_object_set_string_member(content_root, "avatar_url", priv->_avatar_url);
    }

    if (priv->_display_name != NULL) {
        json_object_set_string_member(content_root, "displayname", priv->_display_name);
    }

    tpi_root = json_object_new();

    if (priv->_tpi_display_name != NULL) {
        json_object_set_string_member(tpi_root, "display_name", priv->_tpi_display_name);
    }

    tpi_signed_root = json_object_new();

    if (priv->_tpi_signed_mxid != NULL) {
        json_object_set_string_member(tpi_signed_root, "mxid", priv->_tpi_signed_mxid);
    }

    if (priv->_tpi_signed_token != NULL) {
        json_object_set_string_member(tpi_signed_root, "token", priv->_tpi_signed_token);
    }

    if (priv->_tpi_signature != NULL) {
        json_object_set_member(tpi_signed_root, "signature", priv->_tpi_signature);
    }

    if ((json_object_get_size(tpi_signed_root) != 3) &&
        (json_object_get_size(tpi_signed_root) != 0)) {
        g_warning("3rd party invite data is not filled; ignoring");

        tpi_signed_root = (json_object_unref(tpi_signed_root), NULL);
    }

    if ((tpi_signed_root != NULL) && (priv->_tpi_display_name != NULL)) {
        JsonNode *tpi_signed_node = json_node_new(JSON_NODE_OBJECT);

        json_node_set_object(tpi_signed_node, tpi_signed_root);
        json_object_set_member(tpi_root, "signed", tpi_signed_node);
    }

    if (json_object_get_size(tpi_root) == 2) {
        JsonNode *tpi_node = json_node_new(JSON_NODE_OBJECT);

        json_node_set_object(tpi_node, tpi_root);
        json_object_set_member(content_root, "third_party_invite", tpi_node);
    } else if (json_object_get_size(tpi_root) != 0) {
        tpi_root = (json_object_unref(tpi_root), NULL);
        g_warning("3rd party invite data is incomplete; ignoring");
    }

    if (priv->_invite_room_state != NULL) {
        JsonArray *state_ary = json_array_new();

        for (gint i = 0; i < priv->_invite_room_state_len; i++) {
            JsonNode *state_node = matrix_event_state_get_stripped_node(priv->_invite_room_state[i]);

            json_array_add_element(state_ary, state_node);
        }

        if (json_array_get_length(state_ary) > 0) {
            JsonNode *state_node = json_node_new(JSON_TYPE_ARRAY);

            json_node_set_array(state_node, state_ary);
            json_object_set_member(root, "invite_room_state", state_node);
        }
    }

    MATRIX_EVENT_BASE_CLASS(matrix_event_room_member_parent_class)->to_json(matrix_event_base, json_data, &inner_error);

    if (inner_error != NULL) {
        g_propagate_error(error, inner_error);
    }
}

MatrixEventRoomMember *
matrix_event_room_member_construct(GType object_type)
{
    return (MatrixEventRoomMember *)matrix_event_state_construct(object_type);
}

/**
 * matrix_event_room_member_new:
 *
 * Create a new #MatrixEventRoomMember object.
 *
 * Returns: (transfer full): a new #MatrixEventRoomMember object
 */
MatrixEventRoomMember *
matrix_event_room_member_new(void)
{
    return matrix_event_room_member_construct(MATRIX_EVENT_TYPE_ROOM_MEMBER);
}

/**
 * matrix_event_room_member_get_membership:
 * @event: a #MatrixEventRoomMember
 *
 * Get the membership status from @event.
 *
 * Returns: a #MatrixRoomMembership value
 */
MatrixRoomMembership
matrix_event_room_member_get_membership(MatrixEventRoomMember *matrix_event_room_member)
{
    MatrixEventRoomMemberPrivate *priv;

    g_return_val_if_fail(matrix_event_room_member != NULL, 0);

    priv = matrix_event_room_member_get_instance_private(matrix_event_room_member);

    return priv->_membership;
}

/**
 * matrix_event_room_member_set_membership:
 * @event: a #MatrixEventRoomMember
 * @membership: a #MatrixRoomMembership value
 *
 * Set the membership value in @event.
 */
void
matrix_event_room_member_set_membership(MatrixEventRoomMember *matrix_event_room_member, MatrixRoomMembership membership)
{
    MatrixEventRoomMemberPrivate *priv;

    g_return_if_fail(matrix_event_room_member != NULL);

    priv = matrix_event_room_member_get_instance_private(matrix_event_room_member);

    if (priv->_membership != membership) {
        priv->_membership = membership;

        g_object_notify_by_pspec((GObject *)matrix_event_room_member, matrix_event_room_member_properties[PROP_MEMBERSHIP]);
    }
}

/**
 * matrix_event_room_member_get_avatar_url:
 * @event: a #MatrixEventRoomMember
 *
 * Get the avatar URL from @event.
 *
 * The returned value is owned by @event and should not be freed.
 *
 * Returns: (transfer none) (nullable): an avatar URL
 */
const gchar *
matrix_event_room_member_get_avatar_url(MatrixEventRoomMember *matrix_event_room_member)
{
    MatrixEventRoomMemberPrivate *priv;

    g_return_val_if_fail(matrix_event_room_member != NULL, NULL);

    priv = matrix_event_room_member_get_instance_private(matrix_event_room_member);

    return priv->_avatar_url;
}

/**
 * matrix_event_room_member_set_avatar_url:
 * @event: a #MatrixEventRoomMember
 * @avatar_url: (transfer none) (nullable): the avatar URL for @event
 *
 * Set the avatar URL in @event.
 */
void
matrix_event_room_member_set_avatar_url(MatrixEventRoomMember *matrix_event_room_member, const gchar *avatar_url)
{
    MatrixEventRoomMemberPrivate *priv;

    g_return_if_fail(matrix_event_room_member != NULL);

    priv = matrix_event_room_member_get_instance_private(matrix_event_room_member);

    if (g_strcmp0(avatar_url, priv->_avatar_url) != 0) {
        g_free(priv->_avatar_url);
        priv->_avatar_url = g_strdup(avatar_url);

        g_object_notify_by_pspec((GObject *)matrix_event_room_member, matrix_event_room_member_properties[PROP_AVATAR_URL]);
    }
}

/**
 * matrix_event_room_member_get_display_name:
 * @event: a #MatrixEventRoomMember
 *
 * Get the display name from @event.
 *
 * The returned value is owned by @event and should not be freed.
 *
 * Returns: (transfer none) (nullable): a display name
 */
const gchar *
matrix_event_room_member_get_display_name(MatrixEventRoomMember *matrix_event_room_member)
{
    MatrixEventRoomMemberPrivate *priv;

    g_return_val_if_fail(matrix_event_room_member != NULL, NULL);

    priv = matrix_event_room_member_get_instance_private(matrix_event_room_member);

    return priv->_display_name;
}

/**
 * matrix_event_room_member_set_display_name:
 * @event: a #MatrixEventRoomMember
 * @display_name: (transfer none) (nullable): a display name
 *
 * Set the display name in @event.
 */
void
matrix_event_room_member_set_display_name(MatrixEventRoomMember *matrix_event_room_member, const gchar *display_name)
{
    MatrixEventRoomMemberPrivate *priv;

    g_return_if_fail(matrix_event_room_member != NULL);

    priv = matrix_event_room_member_get_instance_private(matrix_event_room_member);

    if (g_strcmp0(display_name, priv->_display_name) != 0) {
        g_free(priv->_display_name);
        priv->_display_name = g_strdup(display_name);

        g_object_notify_by_pspec((GObject *)matrix_event_room_member, matrix_event_room_member_properties[PROP_DISPLAY_NAME]);
    }
}

/**
 * matrix_event_room_member_get_tpi_display_name:
 * @event: a #MatrixEventRoomMember
 *
 * Get the 3rd party display name from @event.  It will be %NULL if the member was not invited
 * by a 3rd party ID.
 *
 * The returned value is owned by @event, and should not be freed.
 *
 * Returns: (transfer none) (nullable): a 3rd party display name
 */
const gchar *
matrix_event_room_member_get_tpi_display_name(MatrixEventRoomMember *matrix_event_room_member)
{
    MatrixEventRoomMemberPrivate *priv;

    g_return_val_if_fail(matrix_event_room_member != NULL, NULL);

    priv = matrix_event_room_member_get_instance_private(matrix_event_room_member);

    return priv->_tpi_display_name;
}

/**
 * matrix_event_room_member_set_tpi_display_name:
 * @event: a #MatrixEventRoomMember
 * @tpi_display_name: a 3rd party display name
 *
 * Set the 3rd party display name in @event.
 */
void
matrix_event_room_member_set_tpi_display_name(MatrixEventRoomMember *matrix_event_room_member, const gchar *tpi_display_name)
{
    MatrixEventRoomMemberPrivate *priv;

    g_return_if_fail(matrix_event_room_member != NULL);

    priv = matrix_event_room_member_get_instance_private(matrix_event_room_member);

    if (g_strcmp0(tpi_display_name, priv->_tpi_display_name) != 0) {
        g_free(priv->_tpi_display_name);
        priv->_tpi_display_name = g_strdup(tpi_display_name);

        g_object_notify_by_pspec((GObject *)matrix_event_room_member, matrix_event_room_member_properties[PROP_TPI_DISPLAY_NAME]);
    }
}

/**
 * matrix_event_room_member_get_tpi_signed_mxid:
 * @event: a #MatrixEventRoomMember
 *
 * Get the signed 3rd party Matrix ID.
 *
 * The returned value is owned by @event and should not be freed.
 *
 * Returns: (transfer none) (nullable): a 3rd party Matrix ID
 */
const gchar *
matrix_event_room_member_get_tpi_signed_mxid(MatrixEventRoomMember *matrix_event_room_member)
{
    MatrixEventRoomMemberPrivate *priv;

    g_return_val_if_fail(matrix_event_room_member != NULL, NULL);

    priv = matrix_event_room_member_get_instance_private(matrix_event_room_member);

    return priv->_tpi_signed_mxid;
}

/**
 * matrix_event_room_member_set_tpi_signed_mxid:
 * @event: a #MatrixEventRoomMember
 * @tpi_signed_mxid: a 3rd party signed Matrix ID
 *
 * Set the signed 3rd party Matrix ID.
 */
void
matrix_event_room_member_set_tpi_signed_mxid(MatrixEventRoomMember *matrix_event_room_member, const gchar *tpi_signed_mxid)
{
    MatrixEventRoomMemberPrivate *priv;

    g_return_if_fail(matrix_event_room_member != NULL);

    priv = matrix_event_room_member_get_instance_private(matrix_event_room_member);

    if (g_strcmp0(tpi_signed_mxid, priv->_tpi_signed_mxid) != 0) {
        g_free(priv->_tpi_signed_mxid);
        priv->_tpi_signed_mxid = g_strdup(tpi_signed_mxid);

        g_object_notify_by_pspec((GObject *)matrix_event_room_member, matrix_event_room_member_properties[PROP_TPI_SIGNED_MXID]);
    }
}

/**
 * matrix_event_room_member_get_tpi_signed_token:
 * @event: a #MatrixEventRoomMember
 *
 * Get the signed 3rd party token.
 *
 * The returned value is owned by @event and should not be freed.
 *
 * Returns: (transfer none) (nullable): a signed 3rd party token
 */
const gchar *
matrix_event_room_member_get_tpi_signed_token(MatrixEventRoomMember *matrix_event_room_member)
{
    MatrixEventRoomMemberPrivate *priv;

    g_return_val_if_fail(matrix_event_room_member != NULL, NULL);

    priv = matrix_event_room_member_get_instance_private(matrix_event_room_member);

    return priv->_tpi_signed_token;
}

/**
 * matrix_event_room_member_set_tpi_signed_token:
 * @event: a #MatrixEventRoomMember
 * @tpi_signed_token: a signed 3rd party token
 *
 * Set the signed 3rd party token in @event.
 */
void
matrix_event_room_member_set_tpi_signed_token(MatrixEventRoomMember *matrix_event_room_member, const gchar *tpi_signed_token)
{
    MatrixEventRoomMemberPrivate *priv;

    g_return_if_fail(matrix_event_room_member != NULL);

    priv = matrix_event_room_member_get_instance_private(matrix_event_room_member);

    if (g_strcmp0(tpi_signed_token, priv->_tpi_signed_token) != 0) {
        g_free(priv->_tpi_signed_token);
        priv->_tpi_signed_token = g_strdup(tpi_signed_token);

        g_object_notify_by_pspec((GObject *)matrix_event_room_member, matrix_event_room_member_properties[PROP_TPI_SIGNED_TOKEN]);
    }
}

/**
 * matrix_event_room_member_get_tpi_signature:
 * @event: a #MatrixEventRoomMember
 *
 * Get the 3rd party signature from @event.
 *
 * The returned value is owned by @event and should not be freed.
 *
 * Returns: (transfer none) (nullable): a 3rd party signature.
 */
JsonNode *
matrix_event_room_member_get_tpi_signature(MatrixEventRoomMember *matrix_event_room_member)
{
    MatrixEventRoomMemberPrivate *priv;

    g_return_val_if_fail(matrix_event_room_member != NULL, NULL);

    priv = matrix_event_room_member_get_instance_private(matrix_event_room_member);

    return priv->_tpi_signature;
}

/**
 * matrix_event_room_member_set_tpi_signature:
 * @event: a #MatrixEventRoomMember
 * @tpi_signature: a 3rd party signature
 *
 * Set the 3rd party signature in @event.
 */
void
matrix_event_room_member_set_tpi_signature(MatrixEventRoomMember *matrix_event_room_member, JsonNode *tpi_signature)
{
    MatrixEventRoomMemberPrivate *priv;

    g_return_if_fail(matrix_event_room_member != NULL);

    priv = matrix_event_room_member_get_instance_private(matrix_event_room_member);

    if (priv->_tpi_signature != tpi_signature) {
        json_node_unref(priv->_tpi_signature);
        priv->_tpi_signature = json_node_ref(tpi_signature);

        g_object_notify_by_pspec((GObject *)matrix_event_room_member, matrix_event_room_member_properties[PROP_TPI_SIGNATURE]);
    }
}

/**
 * matrix_event_room_member_get_invite_room_state:
 * @event: a #MatrixEventRoomMember
 * @n_invite_room_state: placeholder for the returned list, or %NULL to ignore
 *
 * Get the initial state events from @event.
 *
 * The returned value is owned by @event and should not be freed.
 *
 * Returns: (transfer none) (nullable): the list of initial state events
 */
MatrixEventState **
matrix_event_room_member_get_invite_room_state(MatrixEventRoomMember *matrix_event_room_member, int *n_invite_room_state)
{
    MatrixEventRoomMemberPrivate *priv;

    g_return_val_if_fail(matrix_event_room_member != NULL, NULL);

    priv = matrix_event_room_member_get_instance_private(matrix_event_room_member);

    if (n_invite_room_state != NULL) {
        *n_invite_room_state = priv->_invite_room_state_len;
    }

    return priv->_invite_room_state;
}

/**
 * matrix_event_room_member_set_invite_room_state:
 * @event: a #MatrixEventRoomMember
 * @invite_room_state: a list of initial state events
 * @n_invite_room_state: the length of @invite_room_state
 *
 * Set the initial state events in @event.
 */
void
matrix_event_room_member_set_invite_room_state(MatrixEventRoomMember *matrix_event_room_member, MatrixEventState **invite_room_state, int n_invite_room_state)
{
    MatrixEventRoomMemberPrivate *priv;

    g_return_if_fail(matrix_event_room_member != NULL);

    priv = matrix_event_room_member_get_instance_private(matrix_event_room_member);

    for (gint i = 0; i < priv->_invite_room_state_len; i++) {
        g_object_unref(priv->_invite_room_state[i]);
    }

    g_free(priv->_invite_room_state);

    priv->_invite_room_state = g_new(MatrixEventState *, n_invite_room_state);

    for (gint i = 0; i < n_invite_room_state; i++) {
        priv->_invite_room_state[i] = g_object_ref(invite_room_state[i]);
    }

    priv->_invite_room_state_len = n_invite_room_state;
}

/**
 * matrix_event_room_member_get_user_id:
 * @event: a #MatrixEventRoomMember
 *
 * Get the user ID from @event.
 *
 * The returned value is owned by @event and should not be freed.
 *
 * Returns: (transfer none) (nullable): a user ID
 */
const gchar *
matrix_event_room_member_get_user_id(MatrixEventRoomMember *matrix_event_room_member)
{
    g_return_val_if_fail(matrix_event_room_member != NULL, NULL);

    return matrix_event_state_get_state_key(MATRIX_EVENT_STATE(matrix_event_room_member));
}

/**
 * matrix_event_room_member_set_user_id:
 * @event: a #MatrixEventRoomMember
 * @user_id: a Matrix user ID
 *
 * Set the user ID in @event.
 */
void
matrix_event_room_member_set_user_id(MatrixEventRoomMember *matrix_event_room_member, const gchar *user_id)
{
    const gchar *state_key = matrix_event_state_get_state_key(MATRIX_EVENT_STATE(matrix_event_room_member));

    if (g_strcmp0(state_key, user_id) != 0) {
        matrix_event_state_set_state_key(MATRIX_EVENT_STATE(matrix_event_room_member), user_id);

        g_object_notify_by_pspec((GObject *)matrix_event_room_member, matrix_event_room_member_properties[PROP_USER_ID]);
    }
}

static void
matrix_event_room_member_finalize(GObject *gobject)
{
    MatrixEventRoomMemberPrivate *priv = matrix_event_room_member_get_instance_private(MATRIX_EVENT_ROOM_MEMBER(gobject));

    g_free(priv->_avatar_url);
    g_free(priv->_display_name);
    g_free(priv->_tpi_display_name);
    g_free(priv->_tpi_signed_mxid);
    g_free(priv->_tpi_signed_token);
    json_node_unref(priv->_tpi_signature);

    for (gint i = 0; i < priv->_invite_room_state_len; i++) {
        g_object_unref(priv->_invite_room_state[i]);
    }

    g_free(priv->_invite_room_state);

    G_OBJECT_CLASS(matrix_event_room_member_parent_class)->finalize(gobject);
}

static void
matrix_event_room_member_get_property(GObject *gobject, guint property_id, GValue* value, GParamSpec* pspec)
{
    MatrixEventRoomMember *matrix_event_room_member = MATRIX_EVENT_ROOM_MEMBER(gobject);

    switch (property_id) {
        case PROP_MEMBERSHIP:
            g_value_set_enum(value, matrix_event_room_member_get_membership(matrix_event_room_member));

            break;
        case PROP_AVATAR_URL:
            g_value_set_string(value, matrix_event_room_member_get_avatar_url(matrix_event_room_member));

            break;
        case PROP_DISPLAY_NAME:
            g_value_set_string(value, matrix_event_room_member_get_display_name(matrix_event_room_member));

            break;
        case PROP_TPI_DISPLAY_NAME:
            g_value_set_string(value, matrix_event_room_member_get_tpi_display_name(matrix_event_room_member));

            break;
        case PROP_TPI_SIGNED_MXID:
            g_value_set_string(value, matrix_event_room_member_get_tpi_signed_mxid(matrix_event_room_member));

            break;
        case PROP_TPI_SIGNED_TOKEN:
            g_value_set_string(value, matrix_event_room_member_get_tpi_signed_token(matrix_event_room_member));

            break;
        case PROP_TPI_SIGNATURE:
            g_value_set_boxed(value, matrix_event_room_member_get_tpi_signature(matrix_event_room_member));

            break;
        case PROP_USER_ID:
            g_value_set_string(value, matrix_event_room_member_get_user_id(matrix_event_room_member));

            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, property_id, pspec);

            break;
    }
}

static void
matrix_event_room_member_set_property(GObject *gobject, guint property_id, const GValue* value, GParamSpec* pspec)
{
    MatrixEventRoomMember * matrix_event_room_member = MATRIX_EVENT_ROOM_MEMBER(gobject);

    switch (property_id) {
        case PROP_MEMBERSHIP:
            matrix_event_room_member_set_membership (matrix_event_room_member, g_value_get_enum (value));

            break;
        case PROP_AVATAR_URL:
            matrix_event_room_member_set_avatar_url (matrix_event_room_member, g_value_get_string (value));

            break;
        case PROP_DISPLAY_NAME:
            matrix_event_room_member_set_display_name (matrix_event_room_member, g_value_get_string (value));

            break;
        case PROP_TPI_DISPLAY_NAME:
            matrix_event_room_member_set_tpi_display_name (matrix_event_room_member, g_value_get_string (value));

            break;
        case PROP_TPI_SIGNED_MXID:
            matrix_event_room_member_set_tpi_signed_mxid (matrix_event_room_member, g_value_get_string (value));

            break;
        case PROP_TPI_SIGNED_TOKEN:
            matrix_event_room_member_set_tpi_signed_token (matrix_event_room_member, g_value_get_string (value));

            break;
        case PROP_TPI_SIGNATURE:
            matrix_event_room_member_set_tpi_signature (matrix_event_room_member, g_value_get_boxed (value));

            break;
        case PROP_USER_ID:
            matrix_event_room_member_set_user_id (matrix_event_room_member, g_value_get_string (value));

            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, property_id, pspec);

            break;
    }
}

static void
matrix_event_room_member_class_init (MatrixEventRoomMemberClass *klass)
{
    ((MatrixEventBaseClass *)klass)->from_json = matrix_event_room_member_real_from_json;
    ((MatrixEventBaseClass *)klass)->to_json = matrix_event_room_member_real_to_json;
    G_OBJECT_CLASS(klass)->get_property = matrix_event_room_member_get_property;
    G_OBJECT_CLASS(klass)->set_property = matrix_event_room_member_set_property;
    G_OBJECT_CLASS(klass)->finalize = matrix_event_room_member_finalize;

    /**
     * MatrixEventRoomMember:membership:
     *
     * The membership state of the user.
     */
    matrix_event_room_member_properties[PROP_MEMBERSHIP] = g_param_spec_enum(
            "membership", "membership", "membership",
            MATRIX_TYPE_ROOM_MEMBERSHIP, MATRIX_ROOM_MEMBERSHIP_UNKNOWN,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_MEMBERSHIP, matrix_event_room_member_properties[PROP_MEMBERSHIP]);

    /**
     * MatrixEventRoomMember:avatar-url:
     *
     * The avatar URL for this user, if any.  This is added by the homeserver.
     */
    matrix_event_room_member_properties[PROP_AVATAR_URL] = g_param_spec_string(
            "avatar-url", "avatar-url", "avatar-url",
            NULL,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_AVATAR_URL, matrix_event_room_member_properties[PROP_AVATAR_URL]);

    /**
     * MatrixEventRoomMember:display-name:
     *
     * The display name for this user, if any.  This is added by the homeserver.
     */
    matrix_event_room_member_properties[PROP_DISPLAY_NAME] = g_param_spec_string(
            "display-name", "display-name", "display-name",
            NULL,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_DISPLAY_NAME, matrix_event_room_member_properties[PROP_DISPLAY_NAME]);

    /**
     * MatrixEventRoomMember:tpi-display-name:
     *
     * A name which can be displayed to represent the user instead of their third party identifier.
     */
    matrix_event_room_member_properties[PROP_TPI_DISPLAY_NAME] = g_param_spec_string(
            "tpi-display-name", "tpi-display-name", "tpi-display-name",
            NULL,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_TPI_DISPLAY_NAME, matrix_event_room_member_properties[PROP_TPI_DISPLAY_NAME]);

    /**
     * MatrixEventRoomMember:tpi-signed-mxid:
     * The invited matrix user ID.  Must be equal to the `user_id` property of the event.
     */
    matrix_event_room_member_properties[PROP_TPI_SIGNED_MXID] = g_param_spec_string(
            "tpi-signed-mxid", "tpi-signed-mxid", "tpi-signed-mxid",
            NULL,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_TPI_SIGNED_MXID, matrix_event_room_member_properties[PROP_TPI_SIGNED_MXID]);

    /**
     * MatrixEventRoomMember:tpi-signed-token:
     *
     * The token property of the containing third_party_invite object.
     */
    matrix_event_room_member_properties[PROP_TPI_SIGNED_TOKEN] = g_param_spec_string(
            "tpi-signed-token", "tpi-signed-token", "tpi-signed-token",
            NULL,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_TPI_SIGNED_TOKEN, matrix_event_room_member_properties[PROP_TPI_SIGNED_TOKEN]);

    /**
     * MatrixEventRoomMember:tpi-signature:
     *
     * A single signature from the verifying server, in the format specified by the Signing
     * Events section of the server-server API.
     */
    matrix_event_room_member_properties[PROP_TPI_SIGNATURE] = g_param_spec_boxed(
            "tpi-signature", "tpi-signature", "tpi-signature",
            JSON_TYPE_NODE,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_TPI_SIGNATURE, matrix_event_room_member_properties[PROP_TPI_SIGNATURE]);

    /**
     * MatrixEventRoomMember:user-id:
     *
     * The user ID whom this event relates to.
     */
    matrix_event_room_member_properties[PROP_USER_ID] = g_param_spec_string(
            "user-id", "user-id", "user-id",
            NULL,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_USER_ID, matrix_event_room_member_properties[PROP_USER_ID]);
}

static void
matrix_event_room_member_init(MatrixEventRoomMember *matrix_event_room_member)
{
    MatrixEventRoomMemberPrivate *priv = matrix_event_room_member_get_instance_private(matrix_event_room_member);

    priv->_membership = MATRIX_ROOM_MEMBERSHIP_UNKNOWN;
    priv->_avatar_url = NULL;
    priv->_display_name = NULL;
    priv->_tpi_display_name = NULL;
    priv->_tpi_signed_mxid = NULL;
    priv->_tpi_signed_token = NULL;
    priv->_tpi_signature = NULL;

    matrix_event_room_member_set_user_id(matrix_event_room_member, NULL);
}
