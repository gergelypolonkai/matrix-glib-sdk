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

#include "matrix-message-audio.h"
#include "matrix-enumtypes.h"
#include "config.h"

/**
 * SECTION:matrix-message-audio
 * @short_description: message representing a single audio clip
 *
 * This is the default message handler for `m.audio` messages.
 */
enum  {
    PROP_0,
    PROP_URL,
    PROP_INFO,
    NUM_PROPERTIES
};

static GParamSpec *matrix_message_audio_properties[NUM_PROPERTIES];

typedef struct {
    gchar* _url;
    MatrixAudioInfo* _info;
} MatrixMessageAudioPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(MatrixMessageAudio, matrix_message_audio, MATRIX_MESSAGE_TYPE_BASE);

static void
matrix_message_audio_real_from_json(MatrixMessageBase *matrix_message_base, JsonNode *json_data, GError **error)
{
    MatrixMessageAudioPrivate *priv;
    JsonObject *root;
    JsonNode *node;

    g_return_if_fail(json_data != NULL);

    priv = matrix_message_audio_get_instance_private(MATRIX_MESSAGE_AUDIO(matrix_message_base));
    root = json_node_get_object(json_data);

    if ((node = json_object_get_member(root, "url")) != NULL) {
        g_free(priv->_url);
        priv->_url = g_strdup(json_node_get_string(node));
    } else if (DEBUG) {
        g_warning("url is missing from a m.audio message");
    }

    if ((node = json_object_get_member(root, "info")) != NULL) {
        matrix_audio_info_unref(priv->_info);
        priv->_info = matrix_audio_info_new();
        matrix_audio_info_set_from_json(priv->_info, node);
    }

    MATRIX_MESSAGE_BASE_CLASS(matrix_message_audio_parent_class)->from_json(matrix_message_base, json_data, error);
}

static void
matrix_message_audio_real_to_json(MatrixMessageBase *matrix_message_base, JsonNode *json_data, GError **error)
{
    MatrixMessageAudioPrivate *priv;
    JsonObject *root;

    g_return_if_fail(json_data != NULL);

    priv = matrix_message_audio_get_instance_private(MATRIX_MESSAGE_AUDIO(matrix_message_base));

    if (priv->_url == NULL) {
        g_set_error(error, MATRIX_ERROR, MATRIX_ERROR_INCOMPLETE,
                    "Won't generate a m.audio message without url");

        return;
    }

    root = json_node_get_object(json_data);

    json_object_set_string_member(root, "url", priv->_url);

    if (priv->_info != NULL) {
        GError *inner_error = NULL;
        JsonNode *node = matrix_audio_info_get_json_node(priv->_info, &inner_error);

        if (inner_error != NULL) {
            g_propagate_error(error, inner_error);

            return;
        }

        json_object_set_member(root, "info", node);
    }

    MATRIX_MESSAGE_BASE_CLASS(matrix_message_audio_parent_class)->to_json(matrix_message_base, json_data, error);
}

MatrixMessageAudio *
matrix_message_audio_new(void)
{
    return (MatrixMessageAudio *)matrix_message_base_construct(MATRIX_MESSAGE_TYPE_AUDIO);
}

const gchar *
matrix_message_audio_get_url(MatrixMessageAudio *matrix_message_audio)
{
    MatrixMessageAudioPrivate *priv;

    g_return_val_if_fail(matrix_message_audio != NULL, NULL);

    priv = matrix_message_audio_get_instance_private(matrix_message_audio);

    return priv->_url;
}

void
matrix_message_audio_set_url(MatrixMessageAudio *matrix_message_audio, const gchar *url)
{
    MatrixMessageAudioPrivate *priv;

    g_return_if_fail(matrix_message_audio != NULL);

    priv = matrix_message_audio_get_instance_private(matrix_message_audio);

    if (g_strcmp0(url, priv->_url) != 0) {
        g_free(priv->_url);
        priv->_url = g_strdup(url);

        g_object_notify_by_pspec((GObject *)matrix_message_audio, matrix_message_audio_properties[PROP_URL]);
    }
}

MatrixAudioInfo *
matrix_message_audio_get_info(MatrixMessageAudio *matrix_message_audio)
{
    MatrixMessageAudioPrivate *priv;

    g_return_val_if_fail(matrix_message_audio != NULL, NULL);

    priv = matrix_message_audio_get_instance_private(matrix_message_audio);

    return priv->_info;
}

void
matrix_message_audio_set_info(MatrixMessageAudio *matrix_message_audio, MatrixAudioInfo *info)
{
    MatrixMessageAudioPrivate *priv;

    g_return_if_fail (matrix_message_audio != NULL);

    priv = matrix_message_audio_get_instance_private(matrix_message_audio);

    if (priv->_info != info) {
        matrix_audio_info_unref(priv->_info);
        priv->_info = matrix_audio_info_ref(info);

        g_object_notify_by_pspec((GObject *)matrix_message_audio, matrix_message_audio_properties[PROP_INFO]);
    }
}
static void
matrix_message_audio_finalize(GObject *gobject)
{
    MatrixMessageAudioPrivate *priv;

    priv = matrix_message_audio_get_instance_private(MATRIX_MESSAGE_AUDIO(gobject));

    g_free(priv->_url);
    matrix_audio_info_unref(priv->_info);

    G_OBJECT_CLASS(matrix_message_audio_parent_class)->finalize(gobject);
}

static void
matrix_message_audio_get_property(GObject *gobject, guint property_id, GValue *value, GParamSpec *pspec)
{
    MatrixMessageAudio *matrix_message_audio = MATRIX_MESSAGE_AUDIO(gobject);

    switch (property_id) {
        case PROP_URL:
            g_value_set_string(value, matrix_message_audio_get_url(matrix_message_audio));

            break;
        case PROP_INFO:
            g_value_set_boxed(value, matrix_message_audio_get_info(matrix_message_audio));

            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, property_id, pspec);

            break;
    }
}

static void
matrix_message_audio_set_property(GObject *gobject, guint property_id, const GValue *value, GParamSpec *pspec)
{
    MatrixMessageAudio * matrix_message_audio = MATRIX_MESSAGE_AUDIO(gobject);

    switch (property_id) {
        case PROP_URL:
            matrix_message_audio_set_url(matrix_message_audio, g_value_get_string(value));

            break;
        case PROP_INFO:
            matrix_message_audio_set_info(matrix_message_audio, g_value_get_boxed(value));

            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, property_id, pspec);

            break;
    }
}

static void
matrix_message_audio_class_init(MatrixMessageAudioClass *klass)
{
    ((MatrixMessageBaseClass *)klass)->from_json = matrix_message_audio_real_from_json;
    ((MatrixMessageBaseClass *)klass)->to_json = matrix_message_audio_real_to_json;
    G_OBJECT_CLASS(klass)->get_property = matrix_message_audio_get_property;
    G_OBJECT_CLASS(klass)->set_property = matrix_message_audio_set_property;
    G_OBJECT_CLASS(klass)->finalize = matrix_message_audio_finalize;

    /**
     * MatrixMessageAudio:url:
     *
     * The URL to the audio clip.
     */
    matrix_message_audio_properties[PROP_URL] = g_param_spec_string(
            "url", "url", "url",
            NULL,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_URL, matrix_message_audio_properties[PROP_URL]);

    /**
     * MatrixMessageAudio:info:
     *
     * A #MatrixAudioInfo describing #MatrixMessageAudio:url.
     */
    matrix_message_audio_properties[PROP_INFO] = g_param_spec_boxed(
            "info", "info", "info",
            MATRIX_TYPE_AUDIO_INFO,
            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
    g_object_class_install_property(G_OBJECT_CLASS(klass), PROP_INFO, matrix_message_audio_properties[PROP_INFO]);
}

static void
matrix_message_audio_init(MatrixMessageAudio *matrix_message_audio)
{
    MatrixMessageAudioPrivate *priv = matrix_message_audio_get_instance_private(matrix_message_audio);

    priv->_url = NULL;
    priv->_info = NULL;
}
