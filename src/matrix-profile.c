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

#include "matrix-profile.h"
#include <stdlib.h>
#include <string.h>

// List of object properties
enum  {
    PROP_NONE,
    PROP_AGE,
    PROP_DISPLAY_NAME,
    PROP_AVATAR_URL,
    NUM_PROPERTIES
};

typedef struct {
    GDateTime* last_update;
    gchar* display_name;
    gchar* _avatar_url;
} MatrixProfilePrivate;

G_DEFINE_TYPE(MatrixProfile, matrix_profile, G_TYPE_OBJECT);

static GParamSpec* matrix_profile_properties[NUM_PROPERTIES];

MatrixProfile *
matrix_profile_construct(GType object_type)
{
    return (MatrixProfile *)g_object_new(object_type, NULL);
}

/**
 * matrix_profile_new:
 *
 * Create a new #MatrixProfile object.
 *
 * Returns: (transfer full): a new #MatrixProfile object
 */
MatrixProfile *
matrix_profile_new (void)
{
    return matrix_profile_construct(MATRIX_TYPE_PROFILE);
}

/**
 * matrix_profile_get_age:
 * @profile: a #MatrixProfile
 *
 * Get the age of the profile.
 *
 * Returns: the profile age, in seconds
 */
GTimeSpan
matrix_profile_get_age (MatrixProfile *matrix_profile)
{
    GTimeSpan result;
    GDateTime* utcnow;
    MatrixProfilePrivate *priv = matrix_profile_get_instance_private(matrix_profile);

    if (G_UNLIKELY(matrix_profile == NULL)) {
        return (GTimeSpan)0;
    }

    utcnow = g_date_time_new_now_utc ();
    result = g_date_time_difference (utcnow, priv->last_update);
    g_date_time_unref(utcnow);

    return result;
}

/**
 * matrix_profile_get_display_name:
 * @profile: a #MatrixProfile
 *
 * Get the display name from @profile.
 *
 * The returned value is owned by @profile and should not be modified.
 *
 * Returns: (transfer none) (nullable): a display name
 */
const gchar*
matrix_profile_get_display_name (MatrixProfile *matrix_profile)
{
    MatrixProfilePrivate *priv = matrix_profile_get_instance_private(matrix_profile);

    if (G_UNLIKELY(matrix_profile == NULL)) {
        return NULL;
    }

    return priv->display_name;
}

/**
 * matrix_profile_set_display_name:
 * @profile: a #MatrixProfile
 * @display_name: (transfer none) (nullable): a display name
 *
 * Set the display name in @profile.
 */
void
matrix_profile_set_display_name (MatrixProfile *matrix_profile, const gchar* display_name)
{
    MatrixProfilePrivate *priv = matrix_profile_get_instance_private(matrix_profile);

    if (G_UNLIKELY(matrix_profile == NULL)) {
        return;
    }

    g_free(priv->display_name);
    priv->display_name = g_strdup (display_name);

    g_date_time_unref(priv->last_update);
    priv->last_update = g_date_time_new_now_utc ();

    g_object_notify_by_pspec((GObject *)matrix_profile,
                             matrix_profile_properties[PROP_DISPLAY_NAME]);
}

/**
 * matrix_profile_get_avatar_url:
 * @profile: a #MatrixProfile
 *
 * Get the avatar URL from @profile.
 *
 * The returned value is owned by @profile and should not be freed.
 *
 * Returns: (transfer none) (nullable): an avatar URL
 */
const gchar *
matrix_profile_get_avatar_url (MatrixProfile *matrix_profile)
{
    MatrixProfilePrivate *priv = matrix_profile_get_instance_private(matrix_profile);

    if (G_UNLIKELY(matrix_profile == NULL)) {
        return NULL;
    }

    return priv->_avatar_url;
}

/**
 * matrix_profile_set_avatar_url:
 * @profile: a #MatrixProfile
 * @avatar_url: (transfer none) (nullable): the URL to the avatar
 *
 * Set the avatar URL in @profile.
 */
void
matrix_profile_set_avatar_url (MatrixProfile *matrix_profile, const gchar* avatar_url)
{
    MatrixProfilePrivate *priv = matrix_profile_get_instance_private(matrix_profile);

    if (G_UNLIKELY(matrix_profile == NULL)) {
        return;
    }

    g_free(priv->_avatar_url);
    priv->_avatar_url = g_strdup (avatar_url);

    g_date_time_unref(priv->last_update);
    priv->last_update = g_date_time_new_now_utc ();

    g_object_notify_by_pspec ((GObject *)matrix_profile,
                              matrix_profile_properties[PROP_AVATAR_URL]);
}

static void
matrix_profile_finalize(GObject *gobject)
{
    MatrixProfile *matrix_profile;
    MatrixProfilePrivate *priv;

    matrix_profile = G_TYPE_CHECK_INSTANCE_CAST(gobject, MATRIX_TYPE_PROFILE, MatrixProfile);
    priv = matrix_profile_get_instance_private(matrix_profile);

    g_date_time_unref(priv->last_update);
    g_free(priv->display_name);
    g_free(priv->_avatar_url);

    priv->last_update = NULL;
    priv->display_name = NULL;
    priv->_avatar_url = NULL;

    G_OBJECT_CLASS (matrix_profile_parent_class)->finalize(gobject);
}

static void
matrix_profile_get_property (GObject *gobject, guint property_id, GValue *value, GParamSpec *pspec)
{
    MatrixProfile *matrix_profile;

    matrix_profile = G_TYPE_CHECK_INSTANCE_CAST(gobject, MATRIX_TYPE_PROFILE, MatrixProfile);

    switch (property_id) {
        case PROP_AGE:
            g_value_set_int64 (value, matrix_profile_get_age (matrix_profile));

            break;
        case PROP_DISPLAY_NAME:
            g_value_set_string (value, matrix_profile_get_display_name (matrix_profile));

            break;
        case PROP_AVATAR_URL:
            g_value_set_string(value, matrix_profile_get_avatar_url (matrix_profile));

            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, property_id, pspec);

            break;
    }
}

static void
matrix_profile_set_property(GObject *gobject,
                            guint property_id,
                            const GValue *value,
                            GParamSpec *pspec)
{
    MatrixProfile *matrix_profile;

    matrix_profile = G_TYPE_CHECK_INSTANCE_CAST(gobject, MATRIX_TYPE_PROFILE, MatrixProfile);

    switch (property_id) {
        case PROP_DISPLAY_NAME:
            matrix_profile_set_display_name(matrix_profile, g_value_get_string(value));

            break;
        case PROP_AVATAR_URL:
            matrix_profile_set_avatar_url(matrix_profile, g_value_get_string(value));

            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, property_id, pspec);

            break;
    }
}

static void
matrix_profile_class_init(MatrixProfileClass *klass)
{
    matrix_profile_parent_class = g_type_class_peek_parent(klass);

    G_OBJECT_CLASS(klass)->get_property = matrix_profile_get_property;
    G_OBJECT_CLASS(klass)->set_property = matrix_profile_set_property;
    G_OBJECT_CLASS(klass)->finalize = matrix_profile_finalize;

    /**
     * MatrixProfile:age:
     *
     * The age of the profile, in seconds.
     */
    matrix_profile_properties[PROP_AGE] = g_param_spec_int64(
            "age", "Age",
            "Age of the profile",
            0, G_MAXINT64, 0,
            G_PARAM_STATIC_STRINGS | G_PARAM_READABLE);
    g_object_class_install_property(
            G_OBJECT_CLASS (klass),
            PROP_AGE,
            matrix_profile_properties[PROP_AGE]);

    /**
     * MatrixProfile:display-name:
     *
     * The display name of the user in this profile.
     */
    matrix_profile_properties[PROP_DISPLAY_NAME] = g_param_spec_string(
            "display-name", "display-name",
            "The human readable name of the usser",
            NULL,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(
            G_OBJECT_CLASS(klass),
            PROP_DISPLAY_NAME,
            matrix_profile_properties[PROP_DISPLAY_NAME]);

    /**
     * MatrixProfile:avatar-url:
     *
     * The avatar URL in this profile.
     */
    matrix_profile_properties[PROP_AVATAR_URL] = g_param_spec_string (
            "avatar-url", "avatar-url",
            "The URL of the user's avatar",
            NULL,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(
            G_OBJECT_CLASS (klass),
            PROP_AVATAR_URL,
            matrix_profile_properties[PROP_AVATAR_URL]);
}

static void
matrix_profile_init(MatrixProfile *matrix_profile)
{
    MatrixProfilePrivate *priv = matrix_profile_get_instance_private(matrix_profile);

    priv->display_name = NULL;
    priv->_avatar_url = NULL;

    matrix_profile_set_display_name(matrix_profile, NULL);
    matrix_profile_set_avatar_url(matrix_profile, NULL);
}
