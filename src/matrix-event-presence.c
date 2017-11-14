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

#include "matrix-event-presence.h"
#include "matrix-event-room-base.h"
#include "matrix-enumtypes.h"
#include "config.h"
#include "utils.h"

/**
 * SECTION:matrix-event-presence
 * @short_description: event to inform the client of a users’s presence change
 *
 * Informs the client of a user's presence state change.
 */
enum  {
    PROP_0,
    PROP_AVATAR_URL,
    PROP_DISPLAY_NAME,
    PROP_LAST_ACTIVE_AGO,
    PROP_USER_ID,
    PROP_PRESENCE,
    NUM_PROPS
};

static GParamSpec *matrix_event_presence_properties[NUM_PROPS];

typedef struct {
    gchar *_avatar_url;
    gchar *_display_name;
    glong _last_active_ago;
    gchar *_user_id;
    gchar *_event_id;
    MatrixPresence _presence;
} MatrixEventPresencePrivate;

/**
 * MatrixEventPresence:
 */
G_DEFINE_TYPE_WITH_PRIVATE(MatrixEventPresence, matrix_event_presence, MATRIX_EVENT_TYPE_BASE);

static void
matrix_event_presence_real_from_json(MatrixEventBase *matrix_event_base, JsonNode *json_data, GError **error)
{
    MatrixEventPresencePrivate *priv;
    JsonNode *content_node;
    JsonNode *node;
    JsonObject *root;
    JsonObject *content_root;
    GError *inner_error = NULL;

    g_return_if_fail(json_data != NULL);

    priv = matrix_event_presence_get_instance_private(MATRIX_EVENT_PRESENCE(matrix_event_base));

    root = json_node_get_object(json_data);
    content_node = json_object_get_member(root, "content");
    content_root = json_node_get_object(content_node);

    if ((node = json_object_get_member(root, "event_id")) != NULL) {
        g_free(priv->_event_id);
        priv->_event_id = g_strdup(json_node_get_string(node));
    } else if (DEBUG) {
        g_warning("event_id is missing from a m.presence event");
    }

    if ((node = json_object_get_member(content_root, "user_id")) != NULL) {
        g_free(priv->_user_id);
        priv->_user_id = g_strdup(json_node_get_string(node));
    } else if (DEBUG) {
        g_warning("content.user_id is missing from the m.presence event");

        // Workaround for having sender instead of content.user_id
        // in most (room-dependent) presence events
        if ((node = json_object_get_member(root, "sender")) != NULL) {
            g_free(priv->_user_id);
            priv->_user_id = g_strdup(json_node_get_string(node));
        }
    }

    if ((node = json_object_get_member(content_root, "last_active_ago")) != NULL) {
        priv->_last_active_ago = json_node_get_int(node);
    }

    if ((node = json_object_get_member(content_root, "avatar_url")) != NULL) {
        g_free(priv->_avatar_url);
        priv->_avatar_url = g_strdup(json_node_get_string(node));
    }

    if ((node = json_object_get_member(content_root, "displayname")) != NULL) {
        g_free(priv->_display_name);
        priv->_display_name = g_strdup(json_node_get_string(node));
    }

    if ((node = json_object_get_member(content_root, "presence")) != NULL) {
        MatrixPresence presence;

        presence = _matrix_g_enum_nick_to_value(MATRIX_TYPE_PRESENCE, json_node_get_string(node), &inner_error);

        if (inner_error != NULL) {
            g_clear_error(&inner_error);
            priv->_presence = MATRIX_PRESENCE_UNKNOWN;

            if (DEBUG) {
                g_warning("Unknown value %s for content.presence in a m.presence event", json_node_get_string(node));
            }
        } else {
            priv->_presence = presence;
        }
    } else if (DEBUG) {
        g_warning("content.presence is missing from the m.presence event");
    }

    MATRIX_EVENT_BASE_CLASS(matrix_event_presence_parent_class)->from_json(matrix_event_base, json_data, &inner_error);

    if ((inner_error != NULL)) {
        g_propagate_error(error, inner_error);
    }
}

static void
matrix_event_presence_real_to_json(MatrixEventBase *matrix_event_base, JsonNode *json_data, GError **error)
{
    MatrixEventPresencePrivate *priv;
    JsonNode *content_node;
    JsonObject *root;
    JsonObject *content_root;
    GError *inner_error = NULL;

    g_return_if_fail(json_data != NULL);

    priv = matrix_event_presence_get_instance_private(MATRIX_EVENT_PRESENCE(matrix_event_base));

    if (priv->_presence == MATRIX_PRESENCE_UNKNOWN) {
        g_set_error(error, MATRIX_ERROR, MATRIX_ERROR_UNKNOWN_VALUE,
                    "Won't generate a m.presence event with an unkwnown presence");

        return;
    }

    if (priv->_user_id == NULL) {
        g_set_error(error, MATRIX_ERROR, MATRIX_ERROR_INCOMPLETE,
                    "Won't generate a m.presence event without sender");

        return;
    }

    if (priv->_event_id == NULL) {
        g_set_error(error, MATRIX_ERROR, MATRIX_ERROR_INCOMPLETE,
                    "Won't generate a m.presence event without event_id");

        return;
    }

    root = json_node_get_object(json_data);
    content_node = json_object_get_member(root, "content");
    content_root = json_node_get_object(content_node);

    json_object_set_string_member(root, "event_id", priv->_event_id);

    json_object_set_string_member(content_root, "user_id", priv->_user_id);
    json_object_set_string_member(content_root, "presence", _matrix_g_enum_to_string(MATRIX_TYPE_PRESENCE, priv->_presence, '_'));

    if (priv->_last_active_ago >= 0) {
        json_object_set_int_member(content_root, "last_active_ago", priv->_last_active_ago);
    }

    if (priv->_avatar_url != NULL) {
        json_object_set_string_member(content_root, "avatar_url", priv->_avatar_url);
    }

    if (priv->_display_name != NULL) {
        json_object_set_string_member(content_root, "displayname", priv->_display_name);
    }

    MATRIX_EVENT_BASE_CLASS(matrix_event_presence_parent_class)->to_json(matrix_event_base, json_data, &inner_error);

    if (inner_error != NULL) {
        g_propagate_error(error, inner_error);
    }
}

/**
 * matrix_event_presence_new:
 *
 * Create a new #MatrixEventPresence object.
 *
 * Returns: (transfer full): a new #MatrixEventPresence object
 */
MatrixEventPresence *
matrix_event_presence_new(void)
{
    return (MatrixEventPresence *)matrix_event_base_construct(MATRIX_EVENT_TYPE_PRESENCE);
}

/**
 * matrix_event_presence_get_avatar_url:
 * @event: a #MatrixEventPresence
 *
 * Get the URL of the user’s avatar in @event.
 *
 * The returned value is owned by @event and should not be freed.
 *
 * Returns: (transfer none): the avatar URL
 */
const gchar *
matrix_event_presence_get_avatar_url(MatrixEventPresence *matrix_event_presence)
{
    MatrixEventPresencePrivate *priv;

    g_return_val_if_fail(matrix_event_presence != NULL, NULL);

    priv = matrix_event_presence_get_instance_private(matrix_event_presence);

    return priv->_avatar_url;
}

/**
 * matrix_event_presence_set_avatar_url:
 * @event: a #MatrixEventPresence
 * @avatar_url: (transfer none): an URL to the media of the user’s avatar
 *
 * Set the URL af the user’s avatar in @event.
 */
void
matrix_event_presence_set_avatar_url(MatrixEventPresence *matrix_event_presence, const gchar *avatar_url)
{
    MatrixEventPresencePrivate *priv;

    g_return_if_fail(matrix_event_presence != NULL);

    priv = matrix_event_presence_get_instance_private(matrix_event_presence);

    if (g_strcmp0(avatar_url, priv->_avatar_url) != 0) {
        g_free(priv->_avatar_url);
        priv->_avatar_url = g_strdup (avatar_url);

        g_object_notify_by_pspec((GObject *)matrix_event_presence, matrix_event_presence_properties[PROP_AVATAR_URL]);
    }
}

/**
 * matrix_event_presence_get_display_name:
 * @event: a #MatrixEventPresence
 *
 * Get the display name of the user in @event.
 *
 * The returned value is owned by @event and should not be freed.
 *
 * Returns: (transfer none): a display name
 */
const gchar *
matrix_event_presence_get_display_name(MatrixEventPresence *matrix_event_presence)
{
    MatrixEventPresencePrivate *priv;

    g_return_val_if_fail(matrix_event_presence != NULL, NULL);

    priv = matrix_event_presence_get_instance_private(matrix_event_presence);

    return priv->_display_name;
}

/**
 * matrix_event_presence_set_display_name:
 * @event: a #MatrixEventPresence
 * @display_name: (transfer none): a display name
 *
 * Set the display name of the user in @event.
 */
void
matrix_event_presence_set_display_name (MatrixEventPresence *matrix_event_presence, const gchar *display_name)
{
    MatrixEventPresencePrivate *priv;

    g_return_if_fail(matrix_event_presence != NULL);

    priv = matrix_event_presence_get_instance_private(matrix_event_presence);

    if (g_strcmp0(display_name, priv->_display_name) != 0) {
        g_free(priv->_display_name);
        priv->_display_name = g_strdup(display_name);

        g_object_notify_by_pspec((GObject *)matrix_event_presence, matrix_event_presence_properties[PROP_DISPLAY_NAME]);
    }
}

/**
 * matrix_event_presence_get_last_active_ago:
 * @event: a #MatrixEventPresence
 *
 * Get the number of milliseconds since this user was last active.
 *
 * Returns: the number of milliseconds
 */
glong
matrix_event_presence_get_last_active_ago(MatrixEventPresence *matrix_event_presence)
{
    MatrixEventPresencePrivate *priv;

    g_return_val_if_fail(matrix_event_presence != NULL, -1);

    priv = matrix_event_presence_get_instance_private(matrix_event_presence);

    return priv->_last_active_ago;
}

/**
 * matrix_event_presence_set_last_active_ago:
 * @event: a #MatrixEventPresence
 * @last_active_ago: the number of milliseconds since the user is inactive
 *
 * Set the number of milliseconds since the user in @event is inactive.
 */
void
matrix_event_presence_set_last_active_ago(MatrixEventPresence *matrix_event_presence, glong last_active_ago)
{
    MatrixEventPresencePrivate *priv;

    g_return_if_fail(matrix_event_presence != NULL);

    priv = matrix_event_presence_get_instance_private(matrix_event_presence);

    if (priv->_last_active_ago != last_active_ago) {
        priv->_last_active_ago = last_active_ago;

        g_object_notify_by_pspec((GObject *)matrix_event_presence, matrix_event_presence_properties[PROP_LAST_ACTIVE_AGO]);
    }
}

/**
 * matrix_event_presence_get_user_id:
 * @event: a #MatrixEventPresence
 *
 * Get the user ID @event belongs to.
 *
 * The returned value is owned by @event and should not be freed.
 *
 * Returns: (transfer none): a user ID
 */
const gchar *
matrix_event_presence_get_user_id(MatrixEventPresence *event)
{
    return matrix_event_room_get_sender(MATRIX_EVENT_ROOM(event));
}

/**
 * matrix_event_presence_set_user_id:
 * @event: a #MatrixEventPresence
 * @user_id: (transfer none): the user ID @event should belong to
 *
 * Set the user ID in @event.
 */
void
matrix_event_presence_set_user_id(MatrixEventPresence *event, const gchar *user_id)
{
    matrix_event_room_set_sender(MATRIX_EVENT_ROOM(event), user_id);

    // TODO: Send this only if the property actually changed
    g_object_notify_by_pspec((GObject *)event, matrix_event_presence_properties[PROP_USER_ID]);
}

/**
 * matrix_event_presence_get_presence:
 * @event: a #MatrixEventPresence
 *
 * Get the presence state of the user in @event.
 *
 * Returns: the presence state in @event
 */
MatrixPresence
matrix_event_presence_get_presence (MatrixEventPresence *matrix_event_presence) {
    MatrixEventPresencePrivate *priv;

    g_return_val_if_fail(matrix_event_presence != NULL, MATRIX_PRESENCE_UNKNOWN);

    priv = matrix_event_presence_get_instance_private(matrix_event_presence);

    return priv->_presence;
}

/**
 * matrix_event_presence_set_presence:
 * @event: a #MatrixEventPresence
 * @presence: a #MatrixPresence value
 *
 * Set the presence state in @event.
 */
void
matrix_event_presence_set_presence(MatrixEventPresence *matrix_event_presence, MatrixPresence presence)
{
    MatrixEventPresencePrivate *priv;

    g_return_if_fail(matrix_event_presence != NULL);

    priv = matrix_event_presence_get_instance_private(matrix_event_presence);

    if (priv->_presence != presence) {
        priv->_presence = presence;

        g_object_notify_by_pspec((GObject *)matrix_event_presence, matrix_event_presence_properties[PROP_PRESENCE]);
    }
}

static void
matrix_event_presence_get_property(GObject *gobject, guint property_id, GValue* value, GParamSpec* pspec)
{
    MatrixEventPresencePrivate *priv = matrix_event_presence_get_instance_private(MATRIX_EVENT_PRESENCE(gobject));

    switch (property_id) {
        case PROP_AVATAR_URL:
            g_value_set_string(value, priv->_avatar_url);

            break;
        case PROP_DISPLAY_NAME:
            g_value_set_string(value, priv->_display_name);

            break;
        case PROP_LAST_ACTIVE_AGO:
            g_value_set_long(value, priv->_last_active_ago);

            break;
        case PROP_USER_ID:
            g_value_set_string(value, priv->_user_id);

            break;
        case PROP_PRESENCE:
            g_value_set_enum(value, priv->_presence);

            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, property_id, pspec);

            break;
    }
}

static void
matrix_event_presence_set_property(GObject *gobject, guint property_id, const GValue* value, GParamSpec* pspec)
{
    MatrixEventPresence *matrix_event_presence = MATRIX_EVENT_PRESENCE(gobject);

    switch (property_id) {
        case PROP_AVATAR_URL:
            matrix_event_presence_set_avatar_url(matrix_event_presence, g_value_get_string(value));

            break;
        case PROP_DISPLAY_NAME:
            matrix_event_presence_set_display_name(matrix_event_presence, g_value_get_string(value));

            break;
        case PROP_LAST_ACTIVE_AGO:
            matrix_event_presence_set_last_active_ago(matrix_event_presence, g_value_get_long(value));

            break;
        case PROP_USER_ID:
            matrix_event_presence_set_user_id(matrix_event_presence, g_value_get_string(value));

            break;
        case PROP_PRESENCE:
            matrix_event_presence_set_presence(matrix_event_presence, g_value_get_enum(value));

            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, property_id, pspec);

            break;
    }
}

static void
matrix_event_presence_finalize(GObject *gobject)
{
    MatrixEventPresencePrivate *priv = matrix_event_presence_get_instance_private(MATRIX_EVENT_PRESENCE(gobject));

    g_free(priv->_avatar_url);
    g_free(priv->_display_name);
    g_free(priv->_user_id);
    g_free(priv->_event_id);

    G_OBJECT_CLASS(matrix_event_presence_parent_class)->finalize(gobject);
}

static void
matrix_event_presence_class_init(MatrixEventPresenceClass *klass)
{
    ((MatrixEventBaseClass *)klass)->from_json = matrix_event_presence_real_from_json;
    ((MatrixEventBaseClass *)klass)->to_json = matrix_event_presence_real_to_json;
    G_OBJECT_CLASS (klass)->get_property = matrix_event_presence_get_property;
    G_OBJECT_CLASS (klass)->set_property = matrix_event_presence_set_property;
    G_OBJECT_CLASS (klass)->finalize = matrix_event_presence_finalize;

    /**
     * MatrixEventPresence:avatar-url:
     *
     * The current avatar URL for this user, if any.
     */
    matrix_event_presence_properties[PROP_AVATAR_URL] = g_param_spec_string(
            "avatar-url", "avatar-url", "avatar-url",
            NULL,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_AVATAR_URL, matrix_event_presence_properties[PROP_AVATAR_URL]);

    /**
     * MatrixEventPresence:display-name:
     *
     * The current display name for this user, if any.
     */
    matrix_event_presence_properties[PROP_DISPLAY_NAME] = g_param_spec_string(
            "display-name", "display-name", "display-name",
            NULL,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_DISPLAY_NAME, matrix_event_presence_properties[PROP_DISPLAY_NAME]);

    /**
     * MatrixEventPresence:last-active-ago:
     *
     * The last time since this used performed some action, in milliseconds.
     *
     * This won’t get into the generated event JSON if negative.
     */
    matrix_event_presence_properties[PROP_LAST_ACTIVE_AGO] = g_param_spec_long(
            "last-active-ago", "last-active-ago", "last-active-ago",
            G_MINLONG, G_MAXLONG, -1,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_LAST_ACTIVE_AGO, matrix_event_presence_properties[PROP_LAST_ACTIVE_AGO]);

    /**
     * MatrixEventPresence:user-id:
     *
     * The user's ID.
     */
    matrix_event_presence_properties[PROP_USER_ID] = g_param_spec_string(
            "user-id", "user-id", "user-id",
            NULL,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_USER_ID, matrix_event_presence_properties[PROP_USER_ID]);

    /**
     * MatrixEventPresence:presence:
     *
     * The presence state for this user.
     */
    matrix_event_presence_properties[PROP_PRESENCE] = g_param_spec_enum(
            "presence", "presence", "presence",
            MATRIX_TYPE_PRESENCE, MATRIX_PRESENCE_UNKNOWN,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_PRESENCE, matrix_event_presence_properties[PROP_PRESENCE]);
}

static void
matrix_event_presence_init(MatrixEventPresence *matrix_event_presence) {
    MatrixEventPresencePrivate *priv = matrix_event_presence_get_instance_private(matrix_event_presence);

    priv->_last_active_ago = -1;
    priv->_user_id = NULL;
    priv->_event_id = NULL;
    priv->_presence = MATRIX_PRESENCE_UNKNOWN;
}
