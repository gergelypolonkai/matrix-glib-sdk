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

typedef struct _MatrixAPIRoomFilter MatrixAPIRoomFilter;

GType matrix_api_room_filter_get_type(void);
#define MATRIX_TYPE_API_ROOM_FILTER (matrix_api_room_filter_get_type())

MatrixAPIRoomFilter *matrix_api_room_filter_new(void);
MatrixAPIRoomFilter *matrix_api_room_filter_ref(MatrixAPIRoomFilter *filter);
void matrix_api_room_filter_unref(MatrixAPIRoomFilter *filter);
void matrix_api_room_filter_set_include_leave(MatrixAPIRoomFilter *filter,
                                              gboolean include_leave);
gboolean matrix_api_room_filter_get_include_leave(MatrixAPIRoomFilter *filter);
void matrix_api_room_filter_set_ephemeral(MatrixAPIRoomFilter *filter,
                                          MatrixFilterRules *rules);
MatrixFilterRules *matrix_api_room_filter_get_ephemeral(MatrixAPIRoomFilter *filter);
void matrix_api_room_filter_set_state(MatrixAPIRoomFilter *filter,
                                      MatrixFilterRules *rules);
MatrixFilterRules *matrix_api_room_filter_get_state(MatrixAPIRoomFilter *filter);
void matrix_api_room_filter_set_timeline(MatrixAPIRoomFilter *filter,
                                         MatrixFilterRules *rules);
MatrixFilterRules *matrix_api_room_filter_get_timeline(MatrixAPIRoomFilter *filter);
JsonNode *matrix_api_room_filter_get_json_node(MatrixAPIRoomFilter *filter);
gchar *matrix_api_room_filter_get_json_data(MatrixAPIRoomFilter *filter,
                                            gsize *datalen);

typedef struct _MatrixAPIFilter MatrixAPIFilter;

GType matrix_api_filter_get_type(void);
#define MATRIX_TYPE_API_FILTER (matrix_api_filter_get_type())

MatrixAPIFilter *matrix_api_filter_new(void);
MatrixAPIFilter *matrix_api_filter_ref(MatrixAPIFilter *filter);
void matrix_api_filter_unref(MatrixAPIFilter *filter);
void matrix_api_filter_set_event_fields(MatrixAPIFilter *filter,
                                        GList *event_fields);
void matrix_api_filter_add_event_field(MatrixAPIFilter *filter,
                                       const gchar *event_field);
void matrix_api_filter_delete_event_field(MatrixAPIFilter *filter,
                                          const gchar *event_field);
const GList *matrix_api_filter_get_event_fields(MatrixAPIFilter *filter);
void matrix_api_filter_set_event_format(MatrixAPIFilter *filter,
                                        MatrixEventFormat event_format);
MatrixEventFormat matrix_api_filter_get_event_format(MatrixAPIFilter *filter);
void matrix_api_filter_set_presence_filter(MatrixAPIFilter *filter,
                                           MatrixFilterRules *presence_filter);
MatrixFilterRules *matrix_api_filter_get_presence_filter(MatrixAPIFilter *filter);
void matrix_api_filter_set_room_filter(MatrixAPIFilter *filter,
                                       MatrixAPIRoomFilter *room_filter);
MatrixAPIRoomFilter *matrix_api_filter_get_room_filter(MatrixAPIFilter *filter);
JsonNode *matrix_api_filter_get_json_node(MatrixAPIFilter *filter);
gchar *matrix_api_filter_get_json_data(MatrixAPIFilter *filter, gsize *datalen);

typedef struct _MatrixAPI3PidCredential MatrixAPI3PidCredential;

GType matrix_api_3pid_credential_get_type(void);
#define MATRIX_TYPE_API_3PID_CREDENTIAL (matrix_api_3pid_credential_get_type())

MatrixAPI3PidCredential *matrix_api_3pid_credential_new(void);
MatrixAPI3PidCredential *matrix_api_3pid_credential_ref(MatrixAPI3PidCredential *credential);
void matrix_api_3pid_credential_unref(MatrixAPI3PidCredential *credential);
void matrix_api_3pid_credential_set_id_server(MatrixAPI3PidCredential *credential,
                                              const gchar *id_server);
const gchar *matrix_api_3pid_credential_get_id_server(MatrixAPI3PidCredential *credential);
void matrix_api_3pid_credential_set_session_id(MatrixAPI3PidCredential *credential,
                                               const gchar *session_id);
const gchar *matrix_api_3pid_credential_get_session_id(MatrixAPI3PidCredential *credential);
void matrix_api_3pid_credential_set_client_secret(MatrixAPI3PidCredential *credential,
                                                  const gchar *client_secret);
const gchar *matrix_api_3pid_credential_get_client_secret(MatrixAPI3PidCredential *credential);
JsonNode *matrix_api_3pid_credential_get_json_node(MatrixAPI3PidCredential *credential,
                                                   GError **error);
gchar *matrix_api_3pid_credential_get_json_data(MatrixAPI3PidCredential *credential,
                                                gsize *datalen,
                                                GError **error);

typedef struct _MatrixAPIPusher MatrixAPIPusher;

GType matrix_api_pusher_get_type(void);
#define MATRIX_TYPE_API_PUSHER (matrix_api_pusher_get_type())

MatrixAPIPusher *matrix_api_pusher_new(void);
MatrixAPIPusher *matrix_api_pusher_ref(MatrixAPIPusher *pusher);
void matrix_api_pusher_unref(MatrixAPIPusher *pusher);
void matrix_api_pusher_set_device_display_name(MatrixAPIPusher *pusher,
                                               const gchar *device_display_name);
const gchar *matrix_api_pusher_get_device_display_name(MatrixAPIPusher *pusher);
void matrix_api_pusher_set_app_display_name(MatrixAPIPusher *pusher,
                                            const gchar *app_display_name);
const gchar *matrix_api_pusher_get_app_display_name(MatrixAPIPusher *pusher);
void matrix_api_pusher_set_app_id(MatrixAPIPusher *pusher,
                                  const gchar *app_id);
const gchar *matrix_api_pusher_get_app_id(MatrixAPIPusher *pusher);
void matrix_api_pusher_set_append(MatrixAPIPusher *pusher,
                                  gboolean append);
gboolean matrix_api_pusher_get_append(MatrixAPIPusher *pusher);
void matrix_api_pusher_set_kind(MatrixAPIPusher *pusher,
                                const gchar *kind);
const gchar *matrix_api_pusher_get_kind(MatrixAPIPusher *pusher);
void matrix_api_pusher_set_lang(MatrixAPIPusher *pusher,
                                const gchar *lang);
const gchar *matrix_api_pusher_get_lang(MatrixAPIPusher *pusher);
void matrix_api_pusher_set_profile_tag(MatrixAPIPusher *pusher,
                                       const gchar *profile_tag);
const gchar *matrix_api_pusher_get_profile_tag(MatrixAPIPusher *pusher);
void matrix_api_pusher_set_pushkey(MatrixAPIPusher *pusher,
                                   const gchar *pushkey);
const gchar *matrix_api_pusher_get_pushkey(MatrixAPIPusher *pusher);
void matrix_api_pusher_set_data(MatrixAPIPusher *pusher, const JsonNode *data);
void matrix_api_pusher_take_data(MatrixAPIPusher *pusher, JsonNode *data);
const JsonNode *matrix_api_pusher_get_data(MatrixAPIPusher *pusher);
JsonNode *matrix_api_pusher_get_json_node(MatrixAPIPusher *pusher,
                                          GError **err);
gchar *matrix_api_pusher_get_json_data(MatrixAPIPusher *pusher,
                                       gsize *datalen,
                                       GError **err);

typedef struct _MatrixAPIStateEvent MatrixAPIStateEvent;

GType matrix_api_state_event_get_type(void);
#define MATRIX_TYPE_API_STATE_EVENT (matrix_api_state_event_get_type())

MatrixAPIStateEvent *matrix_api_state_event_new(void);
MatrixAPIStateEvent *matrix_api_state_event_ref(MatrixAPIStateEvent *event);
void matrix_api_state_event_unref(MatrixAPIStateEvent *event);
void matrix_api_state_event_set_event_type(MatrixAPIStateEvent *event,
                                           const gchar *event_type);
const gchar *matrix_api_state_event_get_event_type(MatrixAPIStateEvent *event);
void matrix_api_state_event_set_state_key(MatrixAPIStateEvent *event,
                                          const gchar *state_key);
const gchar *matrix_api_state_event_get_state_key(MatrixAPIStateEvent *event);
void matrix_api_state_event_set_content(MatrixAPIStateEvent *event,
                                        const JsonNode *content);
const JsonNode *matrix_api_state_event_get_content(MatrixAPIStateEvent *event);
JsonNode *matrix_api_state_event_get_json_node(MatrixAPIStateEvent *event);
gchar *matrix_api_state_event_get_json_data(MatrixAPIStateEvent *event,
                                            gsize *datalen);

G_END_DECLS

#endif /* __MATRIX_TYPES_H__ */
