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

#ifndef __MATRIX_API_TYPES_H__
#define __MATRIX_API_TYPES_H__

#include <glib-object.h>
#include <json-glib/json-glib.h>

G_BEGIN_DECLS

typedef enum {
    MATRIX_API_ERROR_NONE,
    MATRIX_API_ERROR_MISSING_TOKEN,
    MATRIX_API_ERROR_FORBIDDEN,
    MATRIX_API_ERROR_UNKNOWN,
    /* Allow for a lot of Matrix.org defined codes
       Do not define error codes after this! */
    MATRIX_API_ERROR_UNKNOWN_ERROR = 16384
} MatrixAPIError;

#define MATRIX_API_ERROR matrix_api_error_quark()
GQuark matrix_api_error_quark(void);

typedef enum {
    MATRIX_API_EVENT_FORMAT_DEFAULT,
    MATRIX_API_EVENT_FORMAT_CLIENT,
    MATRIX_API_EVENT_FORMAT_FEDERATION
} MatrixAPIEventFormat;

typedef struct _MatrixAPIFilterRules MatrixAPIFilterRules;

GType matrix_api_filter_rules_get_type(void);
#define MATRIX_TYPE_API_FILTER_RULES (matrix_api_filter_rules_get_type())

MatrixAPIFilterRules *matrix_api_filter_rules_new(void);
MatrixAPIFilterRules *matrix_api_filter_rules_ref(MatrixAPIFilterRules *rules);
void matrix_api_filter_rules_unref(MatrixAPIFilterRules *rules);
void matrix_api_filter_rules_set_limit(MatrixAPIFilterRules *rules,
                                       guint limit);
guint matrix_api_filter_rules_get_limit(MatrixAPIFilterRules *rules);
void matrix_api_filter_rules_set_rooms(MatrixAPIFilterRules *rules,
                                       GList *rooms);
void matrix_api_filter_rules_add_room(MatrixAPIFilterRules *rules,
                                      const gchar *room);
void matrix_api_filter_rules_delete_room(MatrixAPIFilterRules *rules,
                                         const gchar *room);
const GList *matrix_api_filter_rules_get_rooms(MatrixAPIFilterRules *rules);
void matrix_api_filter_rules_set_excluded_rooms(MatrixAPIFilterRules *rules,
                                                GList *rooms);
void matrix_api_filter_rules_add_excluded_room(MatrixAPIFilterRules *rules,
                                               const gchar *room);
void matrix_api_filter_rules_delete_excluded_room(MatrixAPIFilterRules *rules,
                                                  const gchar *room);
const GList *matrix_api_filter_rules_get_excluded_rooms(MatrixAPIFilterRules *rules);
void matrix_api_filter_rules_set_senders(MatrixAPIFilterRules *rules,
                                         GList *senders);
void matrix_api_filter_rules_add_sender(MatrixAPIFilterRules *rules,
                                        const gchar *sender);
void matrix_api_filter_rules_delete_sender(MatrixAPIFilterRules *rules,
                                           const gchar *sender);
const GList *matrix_api_filter_rules_get_senders(MatrixAPIFilterRules *rules);
void matrix_api_filter_rules_set_excluded_senders(MatrixAPIFilterRules *rules,
                                                  GList *senders);
void matrix_api_filter_rules_add_excluded_sender(MatrixAPIFilterRules *rules,
                                                 const gchar *sender);
void matrix_api_filter_rules_delete_excluded_sender(MatrixAPIFilterRules *rules,
                                                    const gchar *sender);
const GList *matrix_api_filter_rules_get_excluded_senders(MatrixAPIFilterRules *rules);
void matrix_api_filter_rules_set_types(MatrixAPIFilterRules *rules,
                                       GList *types);
void matrix_api_filter_rules_add_type(MatrixAPIFilterRules *rules,
                                      const gchar *type);
void matrix_api_filter_rules_delete_type(MatrixAPIFilterRules *rules,
                                         const gchar *type);
const GList *matrix_api_filter_rules_get_types(MatrixAPIFilterRules *rules);
void matrix_api_filter_rules_set_excluded_types(MatrixAPIFilterRules *rules,
                                                GList *types);
void matrix_api_filter_rules_add_excluded_type(MatrixAPIFilterRules *rules,
                                               const gchar *type);
void matrix_api_filter_rules_delete_excluded_type(MatrixAPIFilterRules *rules,
                                                  const gchar *type);
const GList *matrix_api_filter_rules_get_excluded_types(MatrixAPIFilterRules *rules);
JsonNode *matrix_api_filter_rules_get_json_node(MatrixAPIFilterRules *rules);
gchar *matrix_api_filter_rules_get_json_data(MatrixAPIFilterRules *rules,
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
                                          MatrixAPIFilterRules *rules);
MatrixAPIFilterRules *matrix_api_room_filter_get_ephemeral(MatrixAPIRoomFilter *filter);
void matrix_api_room_filter_set_state(MatrixAPIRoomFilter *filter,
                                      MatrixAPIFilterRules *rules);
MatrixAPIFilterRules *matrix_api_room_filter_get_state(MatrixAPIRoomFilter *filter);
void matrix_api_room_filter_set_timeline(MatrixAPIRoomFilter *filter,
                                         MatrixAPIFilterRules *rules);
MatrixAPIFilterRules *matrix_api_room_filter_get_timeline(MatrixAPIRoomFilter *filter);
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
                                        MatrixAPIEventFormat event_format);
MatrixAPIEventFormat matrix_api_filter_get_event_format(MatrixAPIFilter *filter);
void matrix_api_filter_set_presence_filter(MatrixAPIFilter *filter,
                                           MatrixAPIFilterRules *presence_filter);
MatrixAPIFilterRules *matrix_api_filter_get_presence_filter(MatrixAPIFilter *filter);
void matrix_api_filter_set_room_filter(MatrixAPIFilter *filter,
                                       MatrixAPIRoomFilter *room_filter);
MatrixAPIRoomFilter *matrix_api_filter_get_room_filter(MatrixAPIFilter *filter);

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

G_END_DECLS

#endif /* __MATRIX_API_TYPES_H__ */
