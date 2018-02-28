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

#ifndef __MATRIX_TYPE_H__
# define __MATRIX_TYPE_H__

# include <glib-object.h>
# include <json-glib/json-glib.h>

typedef enum {
    MATRIX_ERROR_NONE,
    MATRIX_ERROR_COMMUNICATION_ERROR,
    MATRIX_ERROR_INCOMPLETE,
    MATRIX_ERROR_BAD_REQUEST,
    MATRIX_ERROR_BAD_RESPONSE,
    MATRIX_ERROR_INVALID_ROOM_ID,
    MATRIX_ERROR_UNKNOWN_VALUE,
    MATRIX_ERROR_INVALID_TYPE,
    MATRIX_ERROR_UNSUPPORTED,
    MATRIX_ERROR_INVALID_FORMAT,
    MATRIX_ERROR_UNAVAILABLE,
    MATRIX_ERROR_NOT_FOUND,
    MATRIX_ERROR_ALREADY_EXISTS,

    /* Add Matrix-defined error codes under here, prefixing them with
     * `MATRIX_ERROR_`, i.e. `M_FORBIDDEN` =>
     * `MATRIX_ERROR_M_FORBIDDEN` */

    MATRIX_ERROR_M_MISSING_TOKEN = 500,
    MATRIX_ERROR_M_FORBIDDEN,
    MATRIX_ERROR_M_UNKNOWN,
    MATRIX_ERROR_M_UNKNOWN_TOKEN,
    MATRIX_ERROR_M_NOT_JSON,
    MATRIX_ERROR_M_UNRECOGNIZED,
    MATRIX_ERROR_M_UNAUTHORIZED,
    MATRIX_ERROR_M_BAD_JSON,
    MATRIX_ERROR_M_USER_IN_USE,
    MATRIX_ERROR_M_ROOM_IN_USE,
    MATRIX_ERROR_M_BAD_PAGINATION,
    MATRIX_ERROR_M_BAD_STATE,
    MATRIX_ERROR_M_NOT_FOUND,
    MATRIX_ERROR_M_GUEST_ACCESS_FORBIDDEN,
    MATRIX_ERROR_M_LIMIT_EXCEEDED,
    MATRIX_ERROR_M_CAPTCHA_NEEDED,
    MATRIX_ERROR_M_CAPTCHA_INVALID,
    MATRIX_ERROR_M_MISSING_PARAM,
    MATRIX_ERROR_M_TOO_LARGE,
    MATRIX_ERROR_M_EXCLUSIVE,
    MATRIX_ERROR_M_THREEPID_AUTH_FAILED,
    MATRIX_ERROR_M_THREEPID_IN_USE,
    MATRIX_ERROR_M_INVALID_USERNAME,

    /* Allow for a lot of Matrix.org defined codes. Do not define
     * Matrix-specific error codes after this! */

    MATRIX_ERROR_UNSPECIFIED = 16383,
    MATRIX_ERROR_UNKNOWN_ERROR
} MatrixError;

# define MATRIX_ERROR matrix_error_quark()
GQuark matrix_error_quark(void);

typedef enum {
    MATRIX_ACCOUNT_KIND_DEFAULT,
    MATRIX_ACCOUNT_KIND_USER,
    MATRIX_ACCOUNT_KIND_GUEST
} MatrixAccountKind;

typedef enum {
    MATRIX_EVENT_DIRECTION_FORWARD,
    MATRIX_EVENT_DIRECTION_BACKWARD
} MatrixEventDirection;

typedef enum {
    MATRIX_EVENT_FORMAT_DEFAULT,
    MATRIX_EVENT_FORMAT_CLIENT,
    MATRIX_EVENT_FORMAT_FEDERATION
} MatrixEventFormat;

typedef enum {
    MATRIX_PRESENCE_UNKNOWN,
    MATRIX_PRESENCE_ONLINE,
    MATRIX_PRESENCE_OFFLINE,
    MATRIX_PRESENCE_UNAVAILABLE,
    MATRIX_PRESENCE_FREE_FOR_CHAT
} MatrixPresence;

typedef enum {
    MATRIX_PUSHER_CONDITION_KIND_EVENT_MATCH,
    MATRIX_PUSHER_CONDITION_KIND_PROFILE_TAG,
    MATRIX_PUSHER_CONDITION_KIND_CONTAINS_DISPLAY_NAME,
    MATRIX_PUSHER_CONDITION_KIND_ROOM_MEMBER_COUNT
} MatrixPusherConditionKind;

typedef enum {
    MATRIX_PUSHER_KIND_OVERRIDE,
    MATRIX_PUSHER_KIND_SENDER,
    MATRIX_PUSHER_KIND_ROOM,
    MATRIX_PUSHER_KIND_CONTENT,
    MATRIX_PUSHER_KIND_UNDERRIDE
} MatrixPusherKind;

typedef enum {
    MATRIX_RECEIPT_TYPE_UNKNOWN,
    MATRIX_RECEIPT_TYPE_READ
} MatrixReceiptType;

typedef enum {
    MATRIX_RESIZE_METHOD_DEFAULT,
    MATRIX_RESIZE_METHOD_CROP,
    MATRIX_RESIZE_METHOD_SCALE
} MatrixResizeMethod;

typedef enum {
    MATRIX_ROOM_MEMBERSHIP_UNKNOWN,
    MATRIX_ROOM_MEMBERSHIP_INVITE,
    MATRIX_ROOM_MEMBERSHIP_JOIN,
    MATRIX_ROOM_MEMBERSHIP_LEAVE,
    MATRIX_ROOM_MEMBERSHIP_BAN,
    MATRIX_ROOM_MEMBERSHIP_KNOCK
} MatrixRoomMembership;

typedef enum {
    MATRIX_ROOM_PRESET_NONE,
    MATRIX_ROOM_PRESET_PRIVATE,
    MATRIX_ROOM_PRESET_TRUSTED_PRIVATE,
    MATRIX_ROOM_PRESET_PUBLIC
} MatrixRoomPreset;

typedef enum {
    MATRIX_ROOM_VISIBILITY_DEFAULT,
    MATRIX_ROOM_VISIBILITY_PUBLIC,
    MATRIX_ROOM_VISIBILITY_PRIVATE
} MatrixRoomVisibility;

typedef enum {
    MATRIX_JOIN_RULES_UNKNOWN,
    MATRIX_JOIN_RULES_PUBLIC,
    MATRIX_JOIN_RULES_INVITE,
    MATRIX_JOIN_RULES_PRIVATE,
    MATRIX_JOIN_RULES_KNOCK
} MatrixJoinRules;

typedef enum {
    MATRIX_SEARCH_ORDER_RECENT,
    MATRIX_SEARCH_ORDER_RANK
} MatrixSearchOrder;

typedef enum {
    MATRIX_SEARCH_KEY_CONTENT_BODY,
    MATRIX_SEARCH_KEY_CONTENT_NAME,
    MATRIX_SEARCH_KEY_CONTENT_TOPIC
} MatrixSearchKey;

typedef enum {
    MATRIX_SEARCH_GROUP_BY_NONE,
    MATRIX_SEARCH_GROUP_BY_ROOM_ID,
    MATRIX_SEARCH_GROUP_BY_SENDER
} MatrixSearchGroupBy;

typedef enum {
    MATRIX_HISTORY_VISIBILITY_UNKNOWN,
    MATRIX_HISTORY_VISIBILITY_INVITED,
    MATRIX_HISTORY_VISIBILITY_JOINED,
    MATRIX_HISTORY_VISIBILITY_SHARED,
    MATRIX_HISTORY_VISIBILITY_WORLD_READABLE
} MatrixHistoryVisibility;

typedef enum {
    MATRIX_GUEST_ACCESS_UNKNOWN,
    MATRIX_GUEST_ACCESS_CAN_JOIN,
    MATRIX_GUEST_ACCESS_FORBIDDEN
} MatrixGuestAccess;

typedef enum {
    MATRIX_CALL_OFFER_TYPE_UNKNOWN,
    MATRIX_CALL_OFFER_TYPE_OFFER
} MatrixCallOfferType;

typedef enum {
    MATRIX_CALL_ANSWER_TYPE_UNKNOWN,
    MATRIX_CALL_ANSWER_TYPE_ANSWER
} MatrixCallAnswerType;

typedef struct _MatrixFileInfo MatrixFileInfo;

GType matrix_file_info_get_type(void);
# define MATRIX_TYPE_FILE_INFO matrix_file_info_get_type()

MatrixFileInfo *matrix_file_info_new(void);
MatrixFileInfo *matrix_file_info_ref(MatrixFileInfo *file_info);
void matrix_file_info_unref(MatrixFileInfo *file_info);
void matrix_file_info_set_size(MatrixFileInfo *file_info, gssize size);
gssize matrix_file_info_get_size(MatrixFileInfo *file_info);
void matrix_file_info_set_mimetype(MatrixFileInfo *file_info, const gchar *mimetype);
const gchar *matrix_file_info_get_mimetype(MatrixFileInfo *file_info);
void matrix_file_info_set_from_json(MatrixFileInfo *file_info, JsonNode *json_data);
JsonNode *matrix_file_info_get_json_node(MatrixFileInfo *file_info, GError **error);

typedef struct _MatrixAudioInfo MatrixAudioInfo;

GType matrix_audio_info_get_type(void);
# define MATRIX_TYPE_AUDIO_INFO matrix_audio_info_get_type()

MatrixAudioInfo *matrix_audio_info_new(void);
MatrixAudioInfo *matrix_audio_info_ref(MatrixAudioInfo *audio_info);
void matrix_audio_info_unref(MatrixAudioInfo *audio_info);
void matrix_audio_info_set_size(MatrixAudioInfo *audio_info, gssize size);
gssize matrix_audio_info_get_size(MatrixAudioInfo *audio_info);
void matrix_audio_info_set_mimetype(MatrixAudioInfo *audio_info, const gchar *mimetype);
const gchar *matrix_audio_info_get_mimetype(MatrixAudioInfo *audio_info);
void matrix_audio_info_set_duration(MatrixAudioInfo *audio_info, gint duration);
gint matrix_audio_info_get_duration(MatrixAudioInfo *audio_info);
void matrix_audio_info_set_from_json(MatrixAudioInfo *audio_info, JsonNode *json_data);
JsonNode *matrix_audio_info_get_json_node(MatrixAudioInfo *audio_info, GError **error);

typedef struct _MatrixImageInfo MatrixImageInfo;

GType matrix_image_info_get_type(void);
# define MATRIX_TYPE_IMAGE_INFO matrix_image_info_get_type()

MatrixImageInfo *matrix_image_info_new(void);
MatrixImageInfo *matrix_image_info_ref(MatrixImageInfo *image_info);
void matrix_image_info_unref(MatrixImageInfo *image_info);
void matrix_image_info_set_size(MatrixImageInfo *image_info, gssize size);
gssize matrix_image_info_get_size(MatrixImageInfo *image_info);
void matrix_image_info_set_height(MatrixImageInfo *image_info, gint height);
gint matrix_image_info_get_height(MatrixImageInfo *image_info);
void matrix_image_info_set_width(MatrixImageInfo *image_info, gint width);
gint matrix_image_info_get_width(MatrixImageInfo *image_info);
void matrix_image_info_set_mimetype(MatrixImageInfo *image_info, const gchar *mimetype);
const gchar *matrix_image_info_get_mimetype(MatrixImageInfo *image_info);
void matrix_image_info_set_from_json(MatrixImageInfo *image_info, JsonNode *json_data);
JsonNode *matrix_image_info_get_json_node(MatrixImageInfo *image_info, GError **error);
gboolean matrix_image_info_differs(MatrixImageInfo *image_info, MatrixImageInfo *other);

typedef struct _MatrixVideoInfo MatrixVideoInfo;

GType matrix_video_info_get_type(void) G_GNUC_CONST;
# define MATRIX_TYPE_VIDEO_INFO matrix_video_info_get_type()

MatrixVideoInfo *matrix_video_info_new(void);
MatrixVideoInfo *matrix_video_info_ref(MatrixVideoInfo *video_info);
void matrix_video_info_unref(MatrixVideoInfo *video_info);
void matrix_video_info_set_size(MatrixVideoInfo *video_info, gsize size);
gsize matrix_video_info_get_size(MatrixVideoInfo *video_info);
void matrix_video_info_set_mimetype(MatrixVideoInfo *video_info, const gchar *mimetype);
const gchar *matrix_video_info_get_mimetype(MatrixVideoInfo *video_info);
void matrix_video_info_set_duration(MatrixVideoInfo *video_info, gint duration);
gint matrix_video_info_get_duration(MatrixVideoInfo *video_info);
void matrix_video_info_set_width(MatrixVideoInfo *video_info, gint width);
gint matrix_video_info_get_width(MatrixVideoInfo *video_info);
void matrix_video_info_set_height(MatrixVideoInfo *video_info, gint height);
gint matrix_video_info_get_height(MatrixVideoInfo *video_info);
void matrix_video_info_set_thumbnail_url(MatrixVideoInfo *video_info, const gchar *thumbnail_url);
const gchar *matrix_video_info_get_thumbnail_url(MatrixVideoInfo *video_info);
void matrix_video_info_set_thumbnail_info(MatrixVideoInfo *video_info, const MatrixImageInfo *thumbnail_info);
const MatrixImageInfo *matrix_video_info_get_thumbnail_info(MatrixVideoInfo *video_info);
void matrix_video_info_set_from_json(MatrixVideoInfo *video_info, JsonNode *json_data);
JsonNode *matrix_video_info_get_json_node(MatrixVideoInfo *video_info, GError **error);

#endif  /* __MATRIX_TYPE_H__ */
