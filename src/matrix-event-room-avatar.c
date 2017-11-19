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

#include "matrix-event-room-avatar.h"
#include "config.h"

/**
 * SECTION:matrix-event-room-avatar
 * @short_description: event holding the room avatar
 *
 * This is the default handler for `m.room.avatar` events.
 *
 * A picture that is associated with the room.  This can be displayed alongside the room
 * information.
 */
enum  {
    PROP_0,
    PROP_URL,
    PROP_THUMBNAIL_URL,
    PROP_INFO,
    PROP_THUMBNAIL_INFO,
    NUM_PROPERTIES
};

static GParamSpec *matrix_event_room_avatar_properties[NUM_PROPERTIES];

typedef struct {
    gchar* _url;
    gchar* _thumbnail_url;
    MatrixImageInfo* _info;
    MatrixImageInfo* _thumbnail_info;
} MatrixEventRoomAvatarPrivate;

/**
 * MatrixEventRoomAvatar:
 */
G_DEFINE_TYPE_WITH_PRIVATE(MatrixEventRoomAvatar, matrix_event_room_avatar, MATRIX_EVENT_TYPE_STATE);

static void
matrix_event_room_avatar_real_from_json(MatrixEventBase *matrix_event_base, JsonNode *json_data, GError **error)
{
    MatrixEventRoomAvatarPrivate *priv;
    JsonObject *root;
    JsonObject *content_root;
    JsonNode *content_node;
    JsonNode *node;

    g_return_if_fail(json_data != NULL);

    priv = matrix_event_room_avatar_get_instance_private(MATRIX_EVENT_ROOM_AVATAR(matrix_event_base));
    root = json_node_get_object(json_data);
    content_node = json_object_get_member(root, "content");
    content_root = json_node_get_object(content_node);

    if (DEBUG) {
        if ((node = json_object_get_member(root, "state_key")) != NULL) {
            const char *sk = json_node_get_string(node);

            if ((sk != NULL) && (*sk != 0)) {
                g_warning("state_key of a m.room.avatar event is non-empty");
            }
        }
    }

    if ((node = json_object_get_member(content_root, "url")) != NULL) {
        g_free(priv->_url);
        priv->_url = g_strdup(json_node_get_string(node));
    } else {
        g_warning("content.url is missing from a m.room.avatar event");
    }

    if ((node = json_object_get_member(content_root, "thumbnail_url")) != NULL) {
        g_free(priv->_thumbnail_url);
        priv->_thumbnail_url = g_strdup(json_node_get_string(node));
    }

    if ((node = json_object_get_member(content_root, "info")) != NULL) {
        matrix_image_info_unref(priv->_info);
        priv->_info = matrix_image_info_new();
        matrix_image_info_set_from_json(priv->_info, node);
    }

    if ((node = json_object_get_member(content_root, "thumbnail_info")) != NULL) {
        matrix_image_info_unref(priv->_thumbnail_info);
        priv->_thumbnail_info = matrix_image_info_new();
        matrix_image_info_set_from_json(priv->_thumbnail_info, node);
    }

    MATRIX_EVENT_BASE_CLASS(matrix_event_room_avatar_parent_class)->from_json(matrix_event_base, json_data, error);
}

static void
matrix_event_room_avatar_real_to_json(MatrixEventBase *matrix_event_base, JsonNode *json_data, GError **error)
{
    MatrixEventRoomAvatarPrivate *priv;
    JsonObject *root;
    JsonObject *content_root;
    JsonNode *content_node;
    JsonNode *node;
    const gchar *state_key;
    GError *inner_error = NULL;

    g_return_if_fail (json_data != NULL);

    priv = matrix_event_room_avatar_get_instance_private(MATRIX_EVENT_ROOM_AVATAR(matrix_event_base));

    if (priv->_url == NULL) {
        g_set_error(error, MATRIX_ERROR, MATRIX_ERROR_INCOMPLETE,
                    "Won't generate a m.room.avatar event without url");

        return;
    }

    root = json_node_get_object(json_data);
    content_node = json_object_get_member(root, "content");
    content_root = json_node_get_object(content_node);

    state_key = matrix_event_state_get_state_key(MATRIX_EVENT_STATE(matrix_event_base));

    if ((state_key == NULL) || (*state_key == 0)) {
        g_set_error(error, MATRIX_ERROR, MATRIX_ERROR_INCOMPLETE,
                    "Won't generate a m.room.avatar event with a non-empty state_key");

        return;
    }

    json_object_set_string_member(content_root, "url", priv->_url);

    if (priv->_thumbnail_url != NULL) {
        json_object_set_string_member(content_root, "thumbnail_url", priv->_thumbnail_url);
    }

    if (priv->_info != NULL) {
        node = matrix_image_info_get_json_node(priv->_info, &inner_error);

        if (inner_error != NULL) {
            g_propagate_error(error, inner_error);

            return;
        }

        json_object_set_member(content_root, "info", node);
    }

    if (priv->_thumbnail_info != NULL) {
        node = matrix_image_info_get_json_node(priv->_thumbnail_info, &inner_error);

        if (inner_error != NULL) {
            g_propagate_error(error, inner_error);

            return;
        }

        json_object_set_member(content_root, "thumbnail_info", node);
    }

    MATRIX_EVENT_BASE_CLASS(matrix_event_room_avatar_parent_class)->to_json(matrix_event_base, json_data, error);
}

MatrixEventRoomAvatar *
matrix_event_room_avatar_construct(GType object_type)
{
    return (MatrixEventRoomAvatar *)matrix_event_state_construct(object_type);
}

/**
 * matrix_event_room_avatar_new:
 *
 * Create a new #MatrixEventRoomAvatar object.
 *
 * Returns: (transfer full): a new #MatrixEventRoomAvatar object
 */
MatrixEventRoomAvatar *
matrix_event_room_avatar_new(void)
{
    return matrix_event_room_avatar_construct(MATRIX_EVENT_TYPE_ROOM_AVATAR);
}

/**
 * matrix_event_room_avatar_get_url:
 * @event: a #MatrixEventRoomAvatar
 *
 * Get the URL of the room avatar from @event.
 *
 * The returned value is owned by @event and should no be freed.
 *
 * Returns: (transfer none) (nullable): an avatar URL
 */
const gchar *
matrix_event_room_avatar_get_url(MatrixEventRoomAvatar *matrix_event_room_avatar)
{
    MatrixEventRoomAvatarPrivate *priv;

    g_return_val_if_fail(matrix_event_room_avatar != NULL, NULL);

    priv = matrix_event_room_avatar_get_instance_private(matrix_event_room_avatar);

    return priv->_url;
}

/**
 * matrix_event_room_avatar_set_url:
 * @event: a #MatrixEventRoomAvatar
 * @url: (transfer none) (nullable): an URL to set as the room avatar
 *
 * Set the room avatar URL.
 */
void
matrix_event_room_avatar_set_url(MatrixEventRoomAvatar *matrix_event_room_avatar, const gchar *url)
{
    MatrixEventRoomAvatarPrivate *priv;

    g_return_if_fail(matrix_event_room_avatar != NULL);

    priv = matrix_event_room_avatar_get_instance_private(matrix_event_room_avatar);

    if (g_strcmp0(url, priv->_url) != 0) {
        g_free(priv->_url);
        priv->_url = g_strdup(url);

        g_object_notify_by_pspec((GObject *)matrix_event_room_avatar, matrix_event_room_avatar_properties[PROP_URL]);
    }
}

/**
 * matrix_event_room_avatar_get_thumbnail_url:
 * @event: a #MatrixEventRoomAvatar
 *
 * Get the URL of the avatar’s thumbnail.
 *
 * The returned value is owned by @event and should not be freed.
 *
 * Returns: (transfer none) (nullable): the URL of the avatar thumbnail
 */
const gchar *
matrix_event_room_avatar_get_thumbnail_url(MatrixEventRoomAvatar *matrix_event_room_avatar)
{
    MatrixEventRoomAvatarPrivate *priv;

    g_return_val_if_fail(matrix_event_room_avatar != NULL, NULL);

    priv = matrix_event_room_avatar_get_instance_private(matrix_event_room_avatar);

    return priv->_thumbnail_url;
}

/**
 * matrix_event_room_avatar_set_thumbnail_url:
 * @event: a #MatrixEventRoomAvatar
 * @thumbnail_url: (transfer none) (nullable): the URL of the room avatar’s thumbnail
 *
 * Set the URL of the room avatar’s thumbnail.
 */
void
matrix_event_room_avatar_set_thumbnail_url(MatrixEventRoomAvatar *matrix_event_room_avatar, const gchar *thumbnail_url)
{
    MatrixEventRoomAvatarPrivate *priv;

    g_return_if_fail(matrix_event_room_avatar != NULL);

    priv = matrix_event_room_avatar_get_instance_private(matrix_event_room_avatar);

    if (g_strcmp0(thumbnail_url, priv->_thumbnail_url) != 0) {
        g_free(priv->_thumbnail_url);
        priv->_thumbnail_url = g_strdup(thumbnail_url);

        g_object_notify_by_pspec((GObject *)matrix_event_room_avatar, matrix_event_room_avatar_properties[PROP_THUMBNAIL_URL]);
    }
}

/**
 * matrix_event_room_avatar_get_info:
 * @event: a #MatrixEventRoomAvatar
 *
 * Get the image info for the room avatar.
 *
 * The returned value is owned by @event and should not be freed.
 *
 * Returns: (transfer none) (nullable): the image info for the room avatar image.
 */
MatrixImageInfo *
matrix_event_room_avatar_get_info(MatrixEventRoomAvatar *matrix_event_room_avatar)
{
    MatrixEventRoomAvatarPrivate *priv;

    g_return_val_if_fail(matrix_event_room_avatar != NULL, NULL);

    priv = matrix_event_room_avatar_get_instance_private(matrix_event_room_avatar);

    return priv->_info;
}

/**
 * matrix_event_room_avatar_set_info:
 * @event: a #MatrixEventRoomAvatar
 * @info: a #MatrixImageInfo
 *
 * Set the image information for the avatar image.
 */
void
matrix_event_room_avatar_set_info(MatrixEventRoomAvatar *matrix_event_room_avatar, MatrixImageInfo *info)
{
    MatrixEventRoomAvatarPrivate *priv;

    g_return_if_fail(matrix_event_room_avatar != NULL);

    priv = matrix_event_room_avatar_get_instance_private(matrix_event_room_avatar);

    if (priv->_info != info) {
        matrix_image_info_unref(priv->_info);
        priv->_info = matrix_image_info_ref(info);

        g_object_notify_by_pspec((GObject *)matrix_event_room_avatar, matrix_event_room_avatar_properties[PROP_INFO]);
    }
}

/**
 * matrix_event_room_avatar_get_thumbnail_info:
 * @event: a #MatrixEventRoomAvatar
 *
 * Get the image info for the room avatar’s thumbnail.
 *
 * The returned value is owned by @event and should not be freed.
 *
 * Returns: (transfer none) (nullable): the image info for the avatar’s thumbnail
 */
MatrixImageInfo *
matrix_event_room_avatar_get_thumbnail_info(MatrixEventRoomAvatar *matrix_event_room_avatar)
{
    MatrixEventRoomAvatarPrivate *priv;

    g_return_val_if_fail(matrix_event_room_avatar != NULL, NULL);

    priv = matrix_event_room_avatar_get_instance_private(matrix_event_room_avatar);

    return priv->_thumbnail_info;
}

/**
 * matrix_event_room_avatar_set_thumbnail_info:
 * @event: a #MatrixEventRoomAvatar
 * @thumbnail_info: (transfer none) (nullable): a #MatrixImageInfo
 *
 * Set the image info for the room avatar’s thumbnail.
 */
void
matrix_event_room_avatar_set_thumbnail_info(MatrixEventRoomAvatar *matrix_event_room_avatar, MatrixImageInfo *thumbnail_info)
{
    MatrixEventRoomAvatarPrivate *priv;

    g_return_if_fail(matrix_event_room_avatar != NULL);

    priv = matrix_event_room_avatar_get_instance_private(matrix_event_room_avatar);

    if (priv->_thumbnail_info != thumbnail_info) {
        matrix_image_info_unref(priv->_thumbnail_info);
        priv->_thumbnail_info = matrix_image_info_ref(thumbnail_info);

        g_object_notify_by_pspec((GObject *)matrix_event_room_avatar, matrix_event_room_avatar_properties[PROP_THUMBNAIL_INFO]);
    }
}

static void
matrix_event_room_avatar_finalize(GObject *gobject)
{
    MatrixEventRoomAvatarPrivate *priv = matrix_event_room_avatar_get_instance_private(MATRIX_EVENT_ROOM_AVATAR(gobject));

    g_free(priv->_url);
    g_free(priv->_thumbnail_url);
    matrix_image_info_unref(priv->_info);
    matrix_image_info_unref(priv->_thumbnail_info);

    G_OBJECT_CLASS(matrix_event_room_avatar_parent_class)->finalize(gobject);
}

static void
matrix_event_room_avatar_get_property(GObject *gobject, guint property_id, GValue *value, GParamSpec *pspec)
{
    MatrixEventRoomAvatar *matrix_event_room_avatar = MATRIX_EVENT_ROOM_AVATAR(gobject);

    switch (property_id) {
        case PROP_URL:
            g_value_set_string(value, matrix_event_room_avatar_get_url(matrix_event_room_avatar));

            break;
        case PROP_THUMBNAIL_URL:
            g_value_set_string(value, matrix_event_room_avatar_get_thumbnail_url(matrix_event_room_avatar));

            break;
        case PROP_INFO:
            g_value_set_boxed(value, matrix_event_room_avatar_get_info(matrix_event_room_avatar));

            break;
        case PROP_THUMBNAIL_INFO:
            g_value_set_boxed(value, matrix_event_room_avatar_get_thumbnail_info(matrix_event_room_avatar));

            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, property_id, pspec);

            break;
    }
}

static void
matrix_event_room_avatar_set_property(GObject *gobject, guint property_id, const GValue *value, GParamSpec *pspec)
{
    MatrixEventRoomAvatar *matrix_event_room_avatar = MATRIX_EVENT_ROOM_AVATAR(gobject);

    switch (property_id) {
        case PROP_URL:
            matrix_event_room_avatar_set_url(matrix_event_room_avatar, g_value_get_string(value));

            break;
        case PROP_THUMBNAIL_URL:
            matrix_event_room_avatar_set_thumbnail_url(matrix_event_room_avatar, g_value_get_string(value));

            break;
        case PROP_INFO:
            matrix_event_room_avatar_set_info(matrix_event_room_avatar, g_value_get_boxed(value));

            break;
        case PROP_THUMBNAIL_INFO:
            matrix_event_room_avatar_set_thumbnail_info(matrix_event_room_avatar, g_value_get_boxed(value));

            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, property_id, pspec);

            break;
    }
}

static void
matrix_event_room_avatar_class_init(MatrixEventRoomAvatarClass *klass)
{
    ((MatrixEventBaseClass *)klass)->from_json = matrix_event_room_avatar_real_from_json;
    ((MatrixEventBaseClass *)klass)->to_json = matrix_event_room_avatar_real_to_json;
    G_OBJECT_CLASS(klass)->get_property = matrix_event_room_avatar_get_property;
    G_OBJECT_CLASS(klass)->set_property = matrix_event_room_avatar_set_property;
    G_OBJECT_CLASS(klass)->finalize = matrix_event_room_avatar_finalize;

    /**
     * MatrixEventRoomAvatar:url:
     *
     * The URL to the image.
     */
    matrix_event_room_avatar_properties[PROP_URL] = g_param_spec_string(
            "url", "url", "url",
            NULL,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_URL, matrix_event_room_avatar_properties[PROP_URL]);

    /**
     * MatrixEventRoomAvatar:thumbnail-url:
     *
     * The URL to the thumbnail of the image.
     */
    matrix_event_room_avatar_properties[PROP_THUMBNAIL_URL] = g_param_spec_string(
            "thumbnail-url", "thumbnail-url", "thumbnail-url",
            NULL,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_THUMBNAIL_URL, matrix_event_room_avatar_properties[PROP_THUMBNAIL_URL]);

    /**
     * MatrixEventRoomAvatar:info:
     *
     * The image info for the room avatar.
     */
    matrix_event_room_avatar_properties[PROP_INFO] = g_param_spec_boxed(
            "info", "info", "info",
            MATRIX_TYPE_IMAGE_INFO,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_INFO, matrix_event_room_avatar_properties[PROP_INFO]);

    /**
     * MatrixEventRoomAvatar:thumbnail-info:
     *
     * The image info for the room avatar’s thumbnail.
     */
    matrix_event_room_avatar_properties[PROP_INFO] = g_param_spec_boxed(
            "thumbnail-info", "thumbnail-info", "thumbnail-info",
            MATRIX_TYPE_IMAGE_INFO,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_INFO, matrix_event_room_avatar_properties[PROP_INFO]);
}

static void
matrix_event_room_avatar_init(MatrixEventRoomAvatar *matrix_event_room_avatar)
{
    MatrixEventRoomAvatarPrivate *priv = matrix_event_room_avatar_get_instance_private(matrix_event_room_avatar);

    priv->_url = NULL;
    priv->_thumbnail_url = NULL;
    priv->_info = NULL;
    priv->_thumbnail_info = NULL;
}
