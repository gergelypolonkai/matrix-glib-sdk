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

#endif /* __MATRIX_API_TYPES_H__ */
