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

#include "matrix-message-location.h"
#include "matrix-types.h"
#include "config.h"

/**
 * SECTION:matrix-message-location
 * @short_description: message type representing a real-world location
 *
 * This is the defaul message handler for `m.location` messages.
 */
enum  {
    PROP_0,
    PROP_GEO_URI,
    PROP_THUMBNAIL_URL,
    PROP_THUMBNAIL_INFO,
    NUM_PROPERTIES
};

static GParamSpec *matrix_message_location_properties[NUM_PROPERTIES];

typedef struct {
    gchar* _geo_uri;
    gchar* _thumbnail_url;
    MatrixImageInfo* _thumbnail_info;
} MatrixMessageLocationPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(MatrixMessageLocation, matrix_message_location, MATRIX_MESSAGE_TYPE_BASE);

static void
matrix_message_location_real_from_json(MatrixMessageBase *matrix_message_base, JsonNode *json_data, GError **error)
{
    MatrixMessageLocationPrivate *priv;
    JsonObject *root;
    JsonNode *node;
    GError *inner_error = NULL;

    g_return_if_fail(json_data != NULL);

    priv = matrix_message_location_get_instance_private(MATRIX_MESSAGE_LOCATION(matrix_message_base));
    root = json_node_get_object(json_data);

    if ((node = json_object_get_member(root, "geo_uri")) != NULL) {
        g_free(priv->_geo_uri);
        priv->_geo_uri = g_strdup(json_node_get_string(node));
    } else if (DEBUG) {
        g_warning("geo_uri is missing from a m.location message");
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

    MATRIX_MESSAGE_BASE_CLASS(matrix_message_location_parent_class)->from_json(matrix_message_base, json_data, &inner_error);

    if (inner_error != NULL) {
        g_propagate_error(error, inner_error);
    }
}

static void
matrix_message_location_real_to_json(MatrixMessageBase *matrix_message_base, JsonNode *json_data, GError **error)
{
    MatrixMessageLocationPrivate *priv;
    JsonObject *root;
    GError *inner_error = NULL;

    g_return_if_fail (json_data != NULL);

    priv = matrix_message_location_get_instance_private(MATRIX_MESSAGE_LOCATION(matrix_message_base));

    if (priv->_geo_uri == NULL) {
        g_set_error(error, MATRIX_ERROR, MATRIX_ERROR_INCOMPLETE,
                    "Won't generate a m.location message without geo_uri");

        return;
    }

    root = json_node_get_object(json_data);

    json_object_set_string_member(root, "geo_uri", priv->_geo_uri);

    if (priv->_thumbnail_url != NULL) {
        json_object_set_string_member(root, "thumbnail_url", priv->_thumbnail_url);
    }

    if (priv->_thumbnail_info != NULL) {
        JsonNode *node = matrix_image_info_get_json_node(priv->_thumbnail_info, &inner_error);

        if (inner_error != NULL) {
            g_propagate_error(error, inner_error);

            return;
        }

        json_object_set_member(root, "thumbnail_info", node);
    }

    MATRIX_MESSAGE_BASE_CLASS(matrix_message_location_parent_class)->to_json(matrix_message_base, json_data, &inner_error);

    if (inner_error != NULL) {
        g_propagate_error(error, inner_error);
    }
}

MatrixMessageLocation *
matrix_message_location_construct(GType object_type)
{
    return (MatrixMessageLocation *)matrix_message_base_construct(object_type);
}

MatrixMessageLocation *
matrix_message_location_new(void)
{
    return matrix_message_location_construct(MATRIX_MESSAGE_TYPE_LOCATION);
}

const gchar *
matrix_message_location_get_geo_uri(MatrixMessageLocation *matrix_message_location)
{
    MatrixMessageLocationPrivate *priv;

    g_return_val_if_fail(matrix_message_location != NULL, NULL);

    priv = matrix_message_location_get_instance_private(matrix_message_location);

    return priv->_geo_uri;
}


void
matrix_message_location_set_geo_uri(MatrixMessageLocation *matrix_message_location, const gchar *geo_uri)
{
    MatrixMessageLocationPrivate *priv;

    g_return_if_fail(matrix_message_location != NULL);

    priv = matrix_message_location_get_instance_private(matrix_message_location);

    if (g_strcmp0(geo_uri, priv->_geo_uri) != 0) {
        g_free(priv->_geo_uri);
        priv->_geo_uri = g_strdup(geo_uri);

        g_object_notify_by_pspec((GObject *)matrix_message_location, matrix_message_location_properties[PROP_GEO_URI]);
    }
}

const gchar *
matrix_message_location_get_thumbnail_url(MatrixMessageLocation *matrix_message_location)
{
    MatrixMessageLocationPrivate *priv;

    g_return_val_if_fail(matrix_message_location != NULL, NULL);

    priv = matrix_message_location_get_instance_private(matrix_message_location);

    return priv->_thumbnail_url;
}

void
matrix_message_location_set_thumbnail_url(MatrixMessageLocation *matrix_message_location, const gchar *thumbnail_url)
{
    MatrixMessageLocationPrivate *priv;

    g_return_if_fail(matrix_message_location != NULL);

    priv = matrix_message_location_get_instance_private(matrix_message_location);

    if (g_strcmp0(thumbnail_url, priv->_thumbnail_url) != 0) {
        g_free(priv->_thumbnail_url);
        priv->_thumbnail_url = g_strdup(thumbnail_url);

        g_object_notify_by_pspec((GObject *)matrix_message_location, matrix_message_location_properties[PROP_THUMBNAIL_URL]);
    }
}

MatrixImageInfo *
matrix_message_location_get_thumbnail_info(MatrixMessageLocation *matrix_message_location)
{
    MatrixMessageLocationPrivate *priv;

    g_return_val_if_fail(matrix_message_location != NULL, NULL);

    priv = matrix_message_location_get_instance_private(matrix_message_location);

    return priv->_thumbnail_info;
}

void
matrix_message_location_set_thumbnail_info(MatrixMessageLocation *matrix_message_location, MatrixImageInfo *thumbnail_info)
{
    MatrixMessageLocationPrivate *priv;

    g_return_if_fail(matrix_message_location != NULL);

    priv = matrix_message_location_get_instance_private(matrix_message_location);

    if (thumbnail_info != priv->_thumbnail_info) {
        matrix_image_info_unref(priv->_thumbnail_info);
        priv->_thumbnail_info = matrix_image_info_ref(thumbnail_info);

        g_object_notify_by_pspec((GObject *)matrix_message_location, matrix_message_location_properties[PROP_THUMBNAIL_INFO]);
    }
}

static void
matrix_message_location_finalize(GObject *gobject)
{
    MatrixMessageLocationPrivate *priv = matrix_message_location_get_instance_private(MATRIX_MESSAGE_LOCATION(gobject));

    g_free(priv->_geo_uri);
    g_free(priv->_thumbnail_url);
    matrix_image_info_unref(priv->_thumbnail_info);

    G_OBJECT_CLASS(matrix_message_location_parent_class)->finalize(gobject);
}

static void
matrix_message_location_get_property(GObject *gobject, guint property_id, GValue *value, GParamSpec *pspec)
{
    MatrixMessageLocation *matrix_message_location = MATRIX_MESSAGE_LOCATION(gobject);

    switch (property_id) {
        case PROP_GEO_URI:
            g_value_set_string(value, matrix_message_location_get_geo_uri(matrix_message_location));

            break;
        case PROP_THUMBNAIL_URL:
            g_value_set_string(value, matrix_message_location_get_thumbnail_url(matrix_message_location));

            break;
        case PROP_THUMBNAIL_INFO:
            g_value_set_boxed(value, matrix_message_location_get_thumbnail_info(matrix_message_location));

            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, property_id, pspec);

            break;
    }
}

static void
matrix_message_location_set_property(GObject *gobject, guint property_id, const GValue *value, GParamSpec *pspec)
{
    MatrixMessageLocation *matrix_message_location = MATRIX_MESSAGE_LOCATION(gobject);

    switch (property_id) {
        case PROP_GEO_URI:
            matrix_message_location_set_geo_uri(matrix_message_location, g_value_get_string(value));

            break;
        case PROP_THUMBNAIL_URL:
            matrix_message_location_set_thumbnail_url(matrix_message_location, g_value_get_string(value));

            break;
        case PROP_THUMBNAIL_INFO:
            matrix_message_location_set_thumbnail_info(matrix_message_location, g_value_get_boxed(value));

            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, property_id, pspec);

            break;
    }
}

static void
matrix_message_location_class_init(MatrixMessageLocationClass *klass)
{
    ((MatrixMessageBaseClass *)klass)->from_json = matrix_message_location_real_from_json;
    ((MatrixMessageBaseClass *)klass)->to_json = matrix_message_location_real_to_json;
    G_OBJECT_CLASS(klass)->get_property = matrix_message_location_get_property;
    G_OBJECT_CLASS(klass)->set_property = matrix_message_location_set_property;
    G_OBJECT_CLASS(klass)->finalize = matrix_message_location_finalize;

    /**
     * MatrixMessageLocation:geo-uri:
     *
     * A geo URI representing this location.
     */
    matrix_message_location_properties[PROP_GEO_URI] = g_param_spec_string(
            "geo-uri", "geo-uri", "geo-uri",
            NULL,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_GEO_URI, matrix_message_location_properties[PROP_GEO_URI]);

    /**
     * MatrixMessageLocation:thumbnail-url:
     *
     * The URL to a thumbnail of the location being represented.
     */
    matrix_message_location_properties[PROP_THUMBNAIL_URL] = g_param_spec_string(
            "thumbnail-url", "thumbnail-url", "thumbnail-url",
            NULL,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_THUMBNAIL_URL, matrix_message_location_properties[PROP_THUMBNAIL_URL]);

    /**
     * MatrixMessageLocation:thumbnail-info:
     *
     * A #MatrixImageInfo object describing #MatrixMessageLocation:thumbnail-url.
     */
    matrix_message_location_properties[PROP_THUMBNAIL_INFO] = g_param_spec_boxed(
            "thumbnail-info", "thumbnail-info", "thumbnail-info",
            MATRIX_TYPE_IMAGE_INFO,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_THUMBNAIL_INFO, matrix_message_location_properties[PROP_THUMBNAIL_INFO]);
}

static void
matrix_message_location_init(MatrixMessageLocation *matrix_message_location)
{
    MatrixMessageLocationPrivate *priv = matrix_message_location_get_instance_private(matrix_message_location);

    priv->_geo_uri = NULL;
    priv->_thumbnail_url = NULL;
    priv->_thumbnail_info = NULL;
}
