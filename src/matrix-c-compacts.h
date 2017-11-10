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

#ifndef __MATRIX_GLIB_SDK_COMPACTS_H__
# define __MATRIX_GLIB_SDK_COMPACTS_H__

# include <glib-object.h>
# include <json-glib/json-glib.h>

# include "matrix-types.h"

G_BEGIN_DECLS

# define MATRIX_TYPE_JSON_COMPACT matrix_json_compact_get_type()
# define MATRIX_JSON_COMPACT(o) (G_TYPE_CHECK_INSTANCE_CAST((o), MATRIX_TYPE_JSON_COMPACT, MatrixJsonCompact))
# define MATRIX_JSON_COMPACT_CLASS(c) (G_TYPE_CHECK_CLASS_CAST((c), MATRIX_TYPE_JSON_COMPACT, MatrixJsonCompactClass))
# define MATRIX_IS_JSON_COMPACT(o) (G_TYPE_CHECK_INSTANCE_TYPE((o), MATRIX_TYPE_JSON_COMPACT))
# define MATRIX_IS_JSON_COMPACT_CLASS(c) (G_TYPE_CHECK_CLASS_TYPE((c), MATRIX_TYPE_JSON_COMPACT))
# define MATRIX_JSON_COMPACT_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS((o), MATRIX_TYPE_JSON_COMPACT, MatrixJsonCompactClass))

typedef struct _MatrixJsonCompactClass MatrixJsonCompactClass;
typedef struct _MatrixJsonCompact MatrixJsonCompact;
void matrix_json_compact_unref(MatrixJsonCompact *json_compact);
G_DEFINE_AUTOPTR_CLEANUP_FUNC(MatrixJsonCompact, matrix_json_compact_unref);

struct _MatrixJsonCompact {
    GTypeInstance parent_instance;

    /* < private > */
    volatile int refcount;
};

struct _MatrixJsonCompactClass {
    GTypeClass parent_class;

    void (*finalize)(MatrixJsonCompact *json_compact);
    JsonNode *(*get_json_node)(MatrixJsonCompact *json_compact, GError **error);
};

GType matrix_json_compact_get_type(void) G_GNUC_CONST;
MatrixJsonCompact *matrix_json_compact_construct(GType object_type);
MatrixJsonCompact *matrix_json_compact_ref(MatrixJsonCompact *json_compact);
JsonNode *matrix_json_compact_get_json_node(MatrixJsonCompact *json_compact, GError **error);
gchar *matrix_json_compact_get_json_data(MatrixJsonCompact *json_compact, gsize *datalen, GError **error);

#define MATRIX_TYPE_FILTER_RULES matrix_filter_rules_get_type()
G_DECLARE_DERIVABLE_TYPE(MatrixFilterRules, matrix_filter_rules, MATRIX, FILTER_RULES, MatrixJsonCompact);

struct _MatrixFilterRulesClass {
    MatrixJsonCompactClass parent_class;
};

MatrixFilterRules *matrix_filter_rules_new(void);
MatrixFilterRules *matrix_filter_rules_construct(GType object_type);
void matrix_filter_rules_set_limit(MatrixFilterRules *filter_rules, guint limit);
guint matrix_filter_rules_get_limit(MatrixFilterRules *filter_rules);
void matrix_filter_rules_set_types(MatrixFilterRules *filter_rules, gchar **types, int n_types);
gchar **matrix_filter_rules_get_types(MatrixFilterRules *filter_rules, int *n_types);
void matrix_filter_rules_set_excluded_types(MatrixFilterRules *filter_rules, gchar **excluded_types, int n_excluded_types);
gchar **matrix_filter_rules_get_excluded_types(MatrixFilterRules *filter_rules, int *n_excluded_types);
void matrix_filter_rules_set_senders(MatrixFilterRules *filter_rules, gchar **senders, int n_senders);
gchar **matrix_filter_rules_get_senders(MatrixFilterRules *filter_rules, int *n_senders);
void matrix_filter_rules_set_excluded_senders(MatrixFilterRules *filter_rules, gchar **excluded_senders, int n_excluded_senders);
gchar **matrix_filter_rules_get_excluded_senders(MatrixFilterRules *filter_rules, int *n_excluded_senders);
void matrix_filter_rules_set_rooms(MatrixFilterRules *filter_rules, gchar **rooms, int n_rooms);
gchar **matrix_filter_rules_get_rooms(MatrixFilterRules *filter_rules, int *n_rooms);
void matrix_filter_rules_set_excluded_rooms(MatrixFilterRules *filter_rules, gchar **excluded_rooms, int n_excluded_rooms);
gchar **matrix_filter_rules_get_excluded_rooms(MatrixFilterRules *filter_rules, int *n_excluded_rooms);

# define MATRIX_TYPE_ROOM_FILTER matrix_room_filter_get_type()
G_DECLARE_DERIVABLE_TYPE(MatrixRoomFilter, matrix_room_filter, MATRIX, ROOM_FILTER, MatrixJsonCompact)

struct _MatrixRoomFilterClass {
    MatrixJsonCompactClass parent_class;
};

MatrixRoomFilter *matrix_room_filter_new(void);
MatrixRoomFilter *matrix_room_filter_construct(GType object_type);
gboolean matrix_room_filter_get_include_leave(MatrixRoomFilter *room_filter);
void matrix_room_filter_set_include_leave(MatrixRoomFilter *room_filter, gboolean include_leave);
MatrixFilterRules *matrix_room_filter_get_ephemeral(MatrixRoomFilter *room_filter);
void matrix_room_filter_set_ephemeral(MatrixRoomFilter *room_filter, MatrixFilterRules *ephemeral_rules);
MatrixFilterRules *matrix_room_filter_get_state(MatrixRoomFilter *room_filter);
void matrix_room_filter_set_state(MatrixRoomFilter *room_filter, MatrixFilterRules *state_rules);
MatrixFilterRules *matrix_room_filter_get_timeline(MatrixRoomFilter *room_filter);
void matrix_room_filter_set_timeline(MatrixRoomFilter *room_filter, MatrixFilterRules *timeline_rules);

# define MATRIX_TYPE_FILTER matrix_filter_get_type()
G_DECLARE_DERIVABLE_TYPE(MatrixFilter, matrix_filter, MATRIX, FILTER, MatrixJsonCompact);

struct _MatrixFilterClass {
    MatrixJsonCompactClass parent_class;
};

MatrixFilter *matrix_filter_new(void);
MatrixFilter *matrix_filter_construct(GType object_type);
gchar **matrix_filter_get_event_fields(MatrixFilter *filter, int *n_event_fields);
void matrix_filter_set_event_fields(MatrixFilter *filter, gchar **event_fields, int n_event_fields);
MatrixEventFormat matrix_filter_get_event_format(MatrixFilter *filter);
void matrix_filter_set_event_format(MatrixFilter *filter, MatrixEventFormat event_format);
MatrixFilterRules *matrix_filter_get_presence_filter(MatrixFilter *filter);
void matrix_filter_set_presence_filter(MatrixFilter *filter, MatrixFilterRules *presence_filter);
MatrixRoomFilter *matrix_filter_get_room_filter(MatrixFilter *filter);
void matrix_filter_set_room_filter(MatrixFilter *filter, MatrixRoomFilter *room_filter);

# define MATRIX_TYPE_3PID_CREDENTIAL matrix_3pid_credential_get_type()
G_DECLARE_DERIVABLE_TYPE(Matrix3PidCredential, matrix_3pid_credential, MATRIX, 3PID_CREDENTIAL, MatrixJsonCompact);

struct _Matrix3PidCredentialClass {
    MatrixJsonCompactClass parent_class;
};

Matrix3PidCredential *matrix_3pid_credential_new(void);
Matrix3PidCredential *matrix_3pid_credential_construct(GType object_type);
const gchar *matrix_3pid_credential_get_id_server(Matrix3PidCredential *credential);
void matrix_3pid_credential_set_id_server(Matrix3PidCredential *credential, const gchar *id_server);
const gchar *matrix_3pid_credential_get_session_id(Matrix3PidCredential *credential);
void matrix_3pid_credential_set_session_id(Matrix3PidCredential *credential, const gchar *session_id);
const gchar *matrix_3pid_credential_get_client_secret(Matrix3PidCredential *credential);
void matrix_3pid_credential_set_client_secret(Matrix3PidCredential *credential, const gchar *client_secret);

G_END_DECLS

#endif  /* __MATRIX_GLIB_SDK_COMPACTS_H__ */
