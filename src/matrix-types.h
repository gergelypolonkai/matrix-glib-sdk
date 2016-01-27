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

    /* Add Matrix-defined error codes under here, changing `M_` to
     * `MATRIX_ERROR`, i.e. `M_FORBIDDEN` =>
     * `MATRIX_ERROR_FORBIDDEN` */
    MATRIX_ERROR_MISSING_TOKEN = 500,
    MATRIX_ERROR_FORBIDDEN,
    MATRIX_ERROR_UNKNOWN,
    MATRIX_ERROR_UNKNOWN_TOKEN,
    MATRIX_ERROR_NOT_JSON,
    MATRIX_ERROR_UNRECOGNIZED,

    /* Allow for a lot of Matrix.org defined codes
       Do not define error codes after this! */
    MATRIX_ERROR_UNSPECIFIED = 16383,
    MATRIX_ERROR_UNKNOWN_ERROR
} MatrixError;

#define MATRIX_ERROR matrix_error_quark()
GQuark matrix_error_quark(void);

typedef enum {
    MATRIX_EVENT_FORMAT_DEFAULT,
    MATRIX_EVENT_FORMAT_CLIENT,
    MATRIX_EVENT_FORMAT_FEDERATION
} MatrixEventFormat;

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

typedef struct _MatrixFilterRules MatrixFilterRules;

GType matrix_filter_rules_get_type(void);
#define MATRIX_TYPE_FILTER_RULES (matrix_filter_rules_get_type())

MatrixFilterRules *matrix_filter_rules_new(void);
MatrixFilterRules *matrix_filter_rules_ref(MatrixFilterRules *rules);
void matrix_filter_rules_unref(MatrixFilterRules *rules);
void matrix_filter_rules_set_limit(MatrixFilterRules *rules, guint limit);
guint matrix_filter_rules_get_limit(MatrixFilterRules *rules);
void matrix_filter_rules_set_rooms(MatrixFilterRules *rules, GList *rooms);
void matrix_filter_rules_add_room(MatrixFilterRules *rules, const gchar *room);
void matrix_filter_rules_delete_room(MatrixFilterRules *rules,
                                     const gchar *room);
const GList *matrix_filter_rules_get_rooms(MatrixFilterRules *rules);
void matrix_filter_rules_set_excluded_rooms(MatrixFilterRules *rules,
                                            GList *rooms);
void matrix_filter_rules_add_excluded_room(MatrixFilterRules *rules,
                                           const gchar *room);
void matrix_filter_rules_delete_excluded_room(MatrixFilterRules *rules,
                                              const gchar *room);
const GList *matrix_filter_rules_get_excluded_rooms(MatrixFilterRules *rules);
void matrix_filter_rules_set_senders(MatrixFilterRules *rules, GList *senders);
void matrix_filter_rules_add_sender(MatrixFilterRules *rules,
                                    const gchar *sender);
void matrix_filter_rules_delete_sender(MatrixFilterRules *rules,
                                       const gchar *sender);
const GList *matrix_filter_rules_get_senders(MatrixFilterRules *rules);
void matrix_filter_rules_set_excluded_senders(MatrixFilterRules *rules,
                                              GList *senders);
void matrix_filter_rules_add_excluded_sender(MatrixFilterRules *rules,
                                             const gchar *sender);
void matrix_filter_rules_delete_excluded_sender(MatrixFilterRules *rules,
                                                const gchar *sender);
const GList *matrix_filter_rules_get_excluded_senders(MatrixFilterRules *rules);
void matrix_filter_rules_set_types(MatrixFilterRules *rules, GList *types);
void matrix_filter_rules_add_type(MatrixFilterRules *rules, const gchar *type);
void matrix_filter_rules_delete_type(MatrixFilterRules *rules,
                                     const gchar *type);
const GList *matrix_filter_rules_get_types(MatrixFilterRules *rules);
void matrix_filter_rules_set_excluded_types(MatrixFilterRules *rules,
                                            GList *types);
void matrix_filter_rules_add_excluded_type(MatrixFilterRules *rules,
                                           const gchar *type);
void matrix_filter_rules_delete_excluded_type(MatrixFilterRules *rules,
                                              const gchar *type);
const GList *matrix_filter_rules_get_excluded_types(MatrixFilterRules *rules);
JsonNode *matrix_filter_rules_get_json_node(MatrixFilterRules *rules);
gchar *matrix_filter_rules_get_json_data(MatrixFilterRules *rules,
                                         gsize *datalen);

typedef struct _MatrixRoomFilter MatrixRoomFilter;

GType matrix_room_filter_get_type(void);
#define MATRIX_TYPE_ROOM_FILTER (matrix_room_filter_get_type())

MatrixRoomFilter *matrix_room_filter_new(void);
MatrixRoomFilter *matrix_room_filter_ref(MatrixRoomFilter *filter);
void matrix_room_filter_unref(MatrixRoomFilter *filter);
void matrix_room_filter_set_include_leave(MatrixRoomFilter *filter,
                                          gboolean include_leave);
gboolean matrix_room_filter_get_include_leave(MatrixRoomFilter *filter);
void matrix_room_filter_set_ephemeral(MatrixRoomFilter *filter,
                                      MatrixFilterRules *rules);
MatrixFilterRules *matrix_room_filter_get_ephemeral(MatrixRoomFilter *filter);
void matrix_room_filter_set_state(MatrixRoomFilter *filter,
                                  MatrixFilterRules *rules);
MatrixFilterRules *matrix_room_filter_get_state(MatrixRoomFilter *filter);
void matrix_room_filter_set_timeline(MatrixRoomFilter *filter,
                                     MatrixFilterRules *rules);
MatrixFilterRules *matrix_room_filter_get_timeline(MatrixRoomFilter *filter);
JsonNode *matrix_room_filter_get_json_node(MatrixRoomFilter *filter);
gchar *matrix_room_filter_get_json_data(MatrixRoomFilter *filter,
                                        gsize *datalen);

typedef struct _MatrixFilter MatrixFilter;

GType matrix_filter_get_type(void);
#define MATRIX_TYPE_FILTER (matrix_filter_get_type())

MatrixFilter *matrix_filter_new(void);
MatrixFilter *matrix_filter_ref(MatrixFilter *filter);
void matrix_filter_unref(MatrixFilter *filter);
void matrix_filter_set_event_fields(MatrixFilter *filter, GList *event_fields);
void matrix_filter_add_event_field(MatrixFilter *filter,
                                   const gchar *event_field);
void matrix_filter_delete_event_field(MatrixFilter *filter,
                                      const gchar *event_field);
const GList *matrix_filter_get_event_fields(MatrixFilter *filter);
void matrix_filter_set_event_format(MatrixFilter *filter,
                                    MatrixEventFormat event_format);
MatrixEventFormat matrix_filter_get_event_format(MatrixFilter *filter);
void matrix_filter_set_presence_filter(MatrixFilter *filter,
                                       MatrixFilterRules *presence_filter);
MatrixFilterRules *matrix_filter_get_presence_filter(MatrixFilter *filter);
void matrix_filter_set_room_filter(MatrixFilter *filter,
                                   MatrixRoomFilter *room_filter);
MatrixRoomFilter *matrix_filter_get_room_filter(MatrixFilter *filter);
JsonNode *matrix_filter_get_json_node(MatrixFilter *filter);
gchar *matrix_filter_get_json_data(MatrixFilter *filter, gsize *datalen);

typedef struct _Matrix3PidCredential Matrix3PidCredential;

GType matrix_3pid_credential_get_type(void);
#define MATRIX_TYPE_3PID_CREDENTIAL (matrix_3pid_credential_get_type())

Matrix3PidCredential *matrix_3pid_credential_new(void);
Matrix3PidCredential *matrix_3pid_credential_ref(Matrix3PidCredential *credential);
void matrix_3pid_credential_unref(Matrix3PidCredential *credential);
void matrix_3pid_credential_set_id_server(Matrix3PidCredential *credential,
                                          const gchar *id_server);
const gchar *matrix_3pid_credential_get_id_server(Matrix3PidCredential *credential);
void matrix_3pid_credential_set_session_id(Matrix3PidCredential *credential,
                                           const gchar *session_id);
const gchar *matrix_3pid_credential_get_session_id(Matrix3PidCredential *credential);
void matrix_3pid_credential_set_client_secret(Matrix3PidCredential *credential,
                                              const gchar *client_secret);
const gchar *matrix_3pid_credential_get_client_secret(Matrix3PidCredential *credential);
JsonNode *matrix_3pid_credential_get_json_node(Matrix3PidCredential *credential,
                                               GError **error);
gchar *matrix_3pid_credential_get_json_data(Matrix3PidCredential *credential,
                                            gsize *datalen,
                                            GError **error);

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
