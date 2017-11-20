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

#include "matrix-message-image.h"
#include "config.h"

/**
 * SECTION:matrix-message-image
 * @short_description: message type representing image uploads
 *
 * This is the default message handler for `m.image` messages.
 */
enum  {
    PROP_0,
    PROP_URL,
    PROP_THUMBNAIL_URL,
    PROP_INFO,
    PROP_THUMBNAIL_INFO,
    NUM_PROPERTIES
};

static GParamSpec *matrix_message_image_properties[NUM_PROPERTIES];

typedef struct {
    gchar* _url;
    gchar* _thumbnail_url;
    MatrixImageInfo* _info;
    MatrixImageInfo* _thumbnail_info;
} MatrixMessageImagePrivate;

G_DEFINE_TYPE_WITH_PRIVATE(MatrixMessageImage, matrix_message_image, MATRIX_MESSAGE_TYPE_BASE);

static void
matrix_message_image_real_from_json(MatrixMessageBase *matrix_message_base, JsonNode *json_data, GError **error)
{
    MatrixMessageImagePrivate *priv;
    JsonObject *root;
    JsonNode *node;

    g_return_if_fail(json_data != NULL);

    priv = matrix_message_image_get_instance_private(MATRIX_MESSAGE_IMAGE(matrix_message_base));
    root = json_node_get_object(json_data);

    if ((node = json_object_get_member(root, "url")) != NULL) {
        g_free(priv->_url);
        priv->_url = g_strdup(json_node_get_string(node));
    } else if (DEBUG) {
        g_warning("url is missing from a m.image message");
    }

    if ((node = json_object_get_member(root, "info")) != NULL) {
        matrix_image_info_unref(priv->_info);
        priv->_info = matrix_image_info_new();
        matrix_image_info_set_from_json(priv->_info, node);
    }

    if ((node = json_object_get_member(root, "thumbnail_url")) != NULL) {
        g_free(priv->_thumbnail_url);
        priv->_thumbnail_url = g_strdup(json_node_get_string(node));
    }

    if ((node = json_object_get_member(root, "thumbnail_info")) != NULL) {
        matrix_image_info_unref(priv->_thumbnail_info);
        priv->_thumbnail_info = matrix_image_info_new();
        matrix_image_info_set_from_json(priv->_thumbnail_info, node);
    }

    MATRIX_MESSAGE_BASE_CLASS(matrix_message_image_parent_class)->from_json(matrix_message_base, json_data, error);
}

static void
matrix_message_image_real_to_json(MatrixMessageBase *matrix_message_base, JsonNode *json_data, GError **error)
{
    MatrixMessageImagePrivate *priv;
    JsonObject *root;
    JsonNode *node;
    GError *inner_error = NULL;

    g_return_if_fail(json_data != NULL);

    priv = matrix_message_image_get_instance_private(MATRIX_MESSAGE_IMAGE(matrix_message_base));

    if (priv->_url == NULL) {
        g_set_error(error, MATRIX_ERROR, MATRIX_ERROR_INCOMPLETE,
                    "Won't generate a m.image message without url");

        return;
    }

    root = json_node_get_object(json_data);

    json_object_set_string_member(root, "url", priv->_url);

    if (priv->_info != NULL) {
        node = matrix_image_info_get_json_node(priv->_info, &inner_error);

        if (inner_error != NULL) {
            g_propagate_error(error, inner_error);

            return;
        }

        json_object_set_member(root, "info", node);
    }

    if (priv->_thumbnail_url != NULL) {
        json_object_set_string_member(root, "thumbnail_url", priv->_thumbnail_url);
    }

    if (priv->_thumbnail_info != NULL) {
        node = matrix_image_info_get_json_node(priv->_thumbnail_info, &inner_error);

        if (inner_error != NULL) {
            g_propagate_error(error, inner_error);

            return;
        }

        json_object_set_member(root, "thumbnail_info", node);
    }

    MATRIX_MESSAGE_BASE_CLASS(matrix_message_image_parent_class)->to_json(matrix_message_base, json_data, error);
}

MatrixMessageImage *
matrix_message_image_new(void)
{
    return (MatrixMessageImage *)matrix_message_base_construct(MATRIX_MESSAGE_TYPE_IMAGE);
}

const gchar *
matrix_message_image_get_url(MatrixMessageImage *matrix_message_image)
{
    MatrixMessageImagePrivate *priv;

    g_return_val_if_fail(matrix_message_image != NULL, NULL);

    priv = matrix_message_image_get_instance_private(matrix_message_image);

    return priv->_url;
}

void
matrix_message_image_set_url(MatrixMessageImage *matrix_message_image, const gchar *url)
{
    MatrixMessageImagePrivate *priv;

    g_return_if_fail(matrix_message_image != NULL);

    priv = matrix_message_image_get_instance_private(matrix_message_image);

    if (g_strcmp0(url, priv->_url) != 0) {
        g_free(priv->_url);
        priv->_url = g_strdup(url);

        g_object_notify_by_pspec((GObject *)matrix_message_image, matrix_message_image_properties[PROP_URL]);
    }
}

const gchar *
matrix_message_image_get_thumbnail_url(MatrixMessageImage *matrix_message_image)
{
    MatrixMessageImagePrivate *priv;

    g_return_val_if_fail(matrix_message_image != NULL, NULL);

    priv = matrix_message_image_get_instance_private(matrix_message_image);

    return priv->_thumbnail_url;
}


void
matrix_message_image_set_thumbnail_url(MatrixMessageImage *matrix_message_image, const gchar *thumbnail_url)
{
    MatrixMessageImagePrivate *priv;

    g_return_if_fail(matrix_message_image != NULL);

    priv = matrix_message_image_get_instance_private(matrix_message_image);

    if (g_strcmp0(thumbnail_url, priv->_thumbnail_url) != 0) {
        g_free(priv->_thumbnail_url);
        priv->_thumbnail_url = g_strdup(thumbnail_url);

        g_object_notify_by_pspec((GObject *)matrix_message_image, matrix_message_image_properties[PROP_THUMBNAIL_URL]);
    }
}

MatrixImageInfo *
matrix_message_image_get_info(MatrixMessageImage *matrix_message_image)
{
    MatrixMessageImagePrivate *priv;

    g_return_val_if_fail(matrix_message_image != NULL, NULL);

    priv = matrix_message_image_get_instance_private(matrix_message_image);

    return priv->_info;
}

void
matrix_message_image_set_info(MatrixMessageImage *matrix_message_image, MatrixImageInfo *info)
{
    MatrixMessageImagePrivate *priv;

    g_return_if_fail (matrix_message_image != NULL);

    priv = matrix_message_image_get_instance_private(matrix_message_image);

    if (priv->_info != info) {
        matrix_image_info_unref(priv->_info);
        priv->_info = matrix_image_info_ref(info);

        g_object_notify_by_pspec((GObject *)matrix_message_image, matrix_message_image_properties[PROP_INFO]);
    }
}

MatrixImageInfo *
matrix_message_image_get_thumbnail_info(MatrixMessageImage *matrix_message_image)
{
    MatrixMessageImagePrivate *priv;

    g_return_val_if_fail(matrix_message_image != NULL, NULL);

    priv = matrix_message_image_get_instance_private(matrix_message_image);

    return priv->_thumbnail_info;
}

void
matrix_message_image_set_thumbnail_info(MatrixMessageImage *matrix_message_image, MatrixImageInfo *thumbnail_info)
{
    MatrixMessageImagePrivate *priv;

    g_return_if_fail(matrix_message_image != NULL);

    priv = matrix_message_image_get_instance_private(matrix_message_image);

    if (priv->_thumbnail_info != thumbnail_info) {
        matrix_image_info_unref(priv->_thumbnail_info);
        priv->_thumbnail_info = matrix_image_info_ref(priv->_thumbnail_info);

        g_object_notify_by_pspec((GObject *)matrix_message_image, matrix_message_image_properties[PROP_THUMBNAIL_INFO]);
    }
}

static void
matrix_message_image_finalize(GObject *gobject)
{
    MatrixMessageImagePrivate *priv = matrix_message_image_get_instance_private(MATRIX_MESSAGE_IMAGE(gobject));

    g_free(priv->_url);
    g_free(priv->_thumbnail_url);
    matrix_image_info_unref(priv->_info);
    matrix_image_info_unref(priv->_thumbnail_info);

    G_OBJECT_CLASS(matrix_message_image_parent_class)->finalize(gobject);
}

static void
matrix_message_image_get_property(GObject *gobject, guint property_id, GValue *value, GParamSpec *pspec)
{
    MatrixMessageImage *matrix_message_image = MATRIX_MESSAGE_IMAGE(gobject);

    switch (property_id) {
        case PROP_URL:
            g_value_set_string(value, matrix_message_image_get_url(matrix_message_image));

            break;
        case PROP_THUMBNAIL_URL:
            g_value_set_string(value, matrix_message_image_get_thumbnail_url(matrix_message_image));

            break;
        case PROP_INFO:
            g_value_set_boxed(value, matrix_message_image_get_info(matrix_message_image));

            break;
        case PROP_THUMBNAIL_INFO:
            g_value_set_boxed(value, matrix_message_image_get_thumbnail_info(matrix_message_image));

            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, property_id, pspec);

            break;
    }
}


static void
matrix_message_image_set_property(GObject *gobject, guint property_id, const GValue *value, GParamSpec *pspec)
{
    MatrixMessageImage *matrix_message_image = MATRIX_MESSAGE_IMAGE(gobject);

    switch (property_id) {
        case PROP_URL:
            matrix_message_image_set_url(matrix_message_image, g_value_get_string(value));

            break;
        case PROP_THUMBNAIL_URL:
            matrix_message_image_set_thumbnail_url(matrix_message_image, g_value_get_string(value));

            break;
        case PROP_INFO:
            matrix_message_image_set_info(matrix_message_image, g_value_get_boxed(value));

            break;
        case PROP_THUMBNAIL_INFO:
            matrix_message_image_set_thumbnail_info(matrix_message_image, g_value_get_boxed(value));

            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, property_id, pspec);

            break;
    }
}

static void
matrix_message_image_class_init(MatrixMessageImageClass *klass)
{
    ((MatrixMessageBaseClass *)klass)->from_json = matrix_message_image_real_from_json;
    ((MatrixMessageBaseClass *)klass)->to_json = matrix_message_image_real_to_json;
    G_OBJECT_CLASS(klass)->get_property = matrix_message_image_get_property;
    G_OBJECT_CLASS(klass)->set_property = matrix_message_image_set_property;
    G_OBJECT_CLASS(klass)->finalize = matrix_message_image_finalize;

    /**
     * MatrixMessageImage:url:
     *
     * The URL to the image.
     */
    matrix_message_image_properties[PROP_URL] = g_param_spec_string(
            "url", "url", "url",
            NULL,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_URL, matrix_message_image_properties[PROP_URL]);

    /**
     * MatrixMessageImage:thumbnail-url:
     *
     * The URL to the thumbnail of the image.
     */
    matrix_message_image_properties[PROP_THUMBNAIL_URL] = g_param_spec_string(
            "thumbnail-url", "thumbnail-url", "thumbnail-url",
            NULL,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_THUMBNAIL_URL, matrix_message_image_properties[PROP_THUMBNAIL_URL]);

    /**
     * MatrixMessageImage:info:
     *
     * A #MatrixImageInfo describing #MatrixMessageImage:url.
     */
    matrix_message_image_properties[PROP_INFO] = g_param_spec_boxed(
            "info", "info", "info",
            MATRIX_TYPE_IMAGE_INFO,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_INFO, matrix_message_image_properties[PROP_INFO]);

    /**
     * MatrixMessageImage:thumbnail-info:
     *
     * A #MatrixImageInfo describing MatrixMessageImage:thumbnail-url.
     */
    matrix_message_image_properties[PROP_THUMBNAIL_INFO] = g_param_spec_boxed(
            "thumbnail-info", "thumbnail-info", "thumbnail-info",
            MATRIX_TYPE_IMAGE_INFO,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_THUMBNAIL_INFO, matrix_message_image_properties[PROP_THUMBNAIL_INFO]);
}

static void
matrix_message_image_init(MatrixMessageImage *matrix_message_image)
{
    MatrixMessageImagePrivate *priv = matrix_message_image_get_instance_private(matrix_message_image);

    priv->_url = NULL;
    priv->_thumbnail_url = NULL;
    priv->_info = NULL;
    priv->_thumbnail_info = NULL;
}
