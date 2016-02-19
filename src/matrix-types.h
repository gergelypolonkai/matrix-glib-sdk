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

#ifndef __MATRIX_TYPES_H__
#define __MATRIX_TYPES_H__

#include <glib-object.h>
#include <json-glib/json-glib.h>

G_BEGIN_DECLS

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

    /* Allow for a lot of Matrix.org defined codes
       Do not define error codes after this! */
    MATRIX_ERROR_UNSPECIFIED = 16383,
    MATRIX_ERROR_UNKNOWN_ERROR
} MatrixError;

#define MATRIX_ERROR matrix_error_quark()
GQuark matrix_error_quark(void);

typedef enum {
    MATRIX_RESIZE_METHOD_DEFAULT,
    MATRIX_RESIZE_METHOD_CROP,
    MATRIX_RESIZE_METHOD_SCALE
} MatrixResizeMethod;

typedef enum {
    MATRIX_PRESENCE_UNKNOWN,
    MATRIX_PRESENCE_ONLINE,
    MATRIX_PRESENCE_OFFLINE,
    MATRIX_PRESENCE_UNAVAILABLE,
    MATRIX_PRESENCE_FREE_FOR_CHAT
} MatrixPresence;

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
    MATRIX_EVENT_DIRECTION_FORWARD,
    MATRIX_EVENT_DIRECTION_BACKWARD
} MatrixEventDirection;

typedef enum {
    MATRIX_RECEIPT_TYPE_READ
} MatrixReceiptType;

typedef enum {
    MATRIX_PUSHER_KIND_OVERRIDE,
    MATRIX_PUSHER_KIND_SENDER,
    MATRIX_PUSHER_KIND_ROOM,
    MATRIX_PUSHER_KIND_CONTENT,
    MATRIX_PUSHER_KIND_UNDERRIDE
} MatrixPusherKind;

typedef enum {
    MATRIX_PUSHER_CONDITION_KIND_EVENT_MATCH,
    MATRIX_PUSHER_CONDITION_KIND_PROFILE_TAG,
    MATRIX_PUSHER_CONDITION_KIND_CONTAINS_DISPLAY_NAME,
    MATRIX_PUSHER_CONDITION_KIND_ROOM_MEMBER_COUNT
} MatrixPusherConditionKind;

typedef enum {
    MATRIX_ACCOUNT_KIND_DEFAULT,
    MATRIX_ACCOUNT_KIND_USER,
    MATRIX_ACCOUNT_KIND_GUEST
} MatrixAccountKind;

typedef struct _MatrixPusher MatrixPusher;

GType matrix_pusher_get_type(void);
#define MATRIX_TYPE_PUSHER (matrix_pusher_get_type())

MatrixPusher *matrix_pusher_new(void);
MatrixPusher *matrix_pusher_ref(MatrixPusher *pusher);
void matrix_pusher_unref(MatrixPusher *pusher);
void matrix_pusher_set_device_display_name(MatrixPusher *pusher,
                                           const gchar *device_display_name);
const gchar *matrix_pusher_get_device_display_name(MatrixPusher *pusher);
void matrix_pusher_set_app_display_name(MatrixPusher *pusher,
                                        const gchar *app_display_name);
const gchar *matrix_pusher_get_app_display_name(MatrixPusher *pusher);
void matrix_pusher_set_app_id(MatrixPusher *pusher, const gchar *app_id);
const gchar *matrix_pusher_get_app_id(MatrixPusher *pusher);
void matrix_pusher_set_append(MatrixPusher *pusher, gboolean append);
gboolean matrix_pusher_get_append(MatrixPusher *pusher);
void matrix_pusher_set_kind(MatrixPusher *pusher, const gchar *kind);
const gchar *matrix_pusher_get_kind(MatrixPusher *pusher);
void matrix_pusher_set_lang(MatrixPusher *pusher, const gchar *lang);
const gchar *matrix_pusher_get_lang(MatrixPusher *pusher);
void matrix_pusher_set_profile_tag(MatrixPusher *pusher,
                                   const gchar *profile_tag);
const gchar *matrix_pusher_get_profile_tag(MatrixPusher *pusher);
void matrix_pusher_set_pushkey(MatrixPusher *pusher, const gchar *pushkey);
const gchar *matrix_pusher_get_pushkey(MatrixPusher *pusher);
void matrix_pusher_set_data(MatrixPusher *pusher, const JsonNode *data);
void matrix_pusher_take_data(MatrixPusher *pusher, JsonNode *data);
const JsonNode *matrix_pusher_get_data(MatrixPusher *pusher);
JsonNode *matrix_pusher_get_json_node(MatrixPusher *pusher, GError **err);
gchar *matrix_pusher_get_json_data(MatrixPusher *pusher,
                                   gsize *datalen,
                                   GError **err);

typedef struct _MatrixStateEvent MatrixStateEvent;

GType matrix_state_event_get_type(void);
#define MATRIX_TYPE_STATE_EVENT (matrix_state_event_get_type())

MatrixStateEvent *matrix_state_event_new(void);
MatrixStateEvent *matrix_state_event_ref(MatrixStateEvent *event);
void matrix_state_event_unref(MatrixStateEvent *event);
void matrix_state_event_set_event_type(MatrixStateEvent *event,
                                       const gchar *event_type);
const gchar *matrix_state_event_get_event_type(MatrixStateEvent *event);
void matrix_state_event_set_state_key(MatrixStateEvent *event,
                                      const gchar *state_key);
const gchar *matrix_state_event_get_state_key(MatrixStateEvent *event);
void matrix_state_event_set_content(MatrixStateEvent *event,
                                    const JsonNode *content);
const JsonNode *matrix_state_event_get_content(MatrixStateEvent *event);
JsonNode *matrix_state_event_get_json_node(MatrixStateEvent *event);
gchar *matrix_state_event_get_json_data(MatrixStateEvent *event,
                                        gsize *datalen);

typedef struct _MatrixPresenceEvent MatrixPresenceEvent;

GType matrix_presence_event_get_type(void);
#define MATRIX_TYPE_PRESENCE_EVENT (matrix_presence_event_get_type())

MatrixPresenceEvent *matrix_presence_event_new(void);
MatrixPresenceEvent *matrix_presence_event_new_from_json(JsonNode *json_data);
MatrixPresenceEvent *matrix_presence_event_ref(MatrixPresenceEvent *event);
void matrix_presence_event_unref(MatrixPresenceEvent *event);
void matrix_presence_event_set_user_id(MatrixPresenceEvent *event,
                                       const gchar *user_id);
const gchar *matrix_presence_event_get_user_id(MatrixPresenceEvent *event);
void matrix_presence_event_set_display_name(MatrixPresenceEvent *event,
                                            const gchar *display_name);
const gchar *matrix_presence_event_get_display_name(MatrixPresenceEvent *event);
void matrix_presence_event_set_avatar_url(MatrixPresenceEvent *event,
                                          const gchar *avatar_url);
const gchar *matrix_presence_event_get_avatar_url(MatrixPresenceEvent *event);
void matrix_presence_event_set_last_active_ago(MatrixPresenceEvent *event,
                                               gulong last_active_ago);
gulong matrix_presence_event_get_last_active_ago(MatrixPresenceEvent *event);
void matrix_presence_event_set_presence(MatrixPresenceEvent *event,
                                        MatrixPresence presence);
MatrixPresence matrix_presence_event_get_presence(MatrixPresenceEvent *event);

G_END_DECLS

#endif /* __MATRIX_TYPES_H__ */
