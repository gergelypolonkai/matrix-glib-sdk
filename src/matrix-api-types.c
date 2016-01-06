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

#include "matrix-api-types.h"

/**
 * SECTION:matrix-api-types
 * @title: Generic types
 * @short_description: Generic types for all #MatrixAPI
 *                     implementations
 *
 * These are the types used by all #MatrixAPI implementations for
 * communication with the homeserver.
 */

/**
 * MatrixAPIFilterRules: (ref-func matrix_api_filter_rules_ref) (unref-func matrix_api_filter_rules_unref)
 *
 * An opaque structure to hold filter rules that can be used to filter
 * events in the event stream. It is possible to filter by event type,
 * room and sender ID.
 */

struct _MatrixAPIFilterRules {
    guint limit;
    GList *rooms;
    GList *excluded_rooms;
    GList *senders;
    GList *excluded_senders;
    GList *types;
    GList *excluded_types;
    guint refcount;
};

G_DEFINE_BOXED_TYPE(MatrixAPIFilterRules, matrix_api_filter_rules,
                    (GBoxedCopyFunc)matrix_api_filter_rules_ref,
                    (GBoxedFreeFunc)matrix_api_filter_rules_unref);

/**
 * matrix_api_filter_rules_new:
 *
 * Create a new #MatrixAPIFilterRules object with reference count of
 * 1.
 *
 * Returns: (transfer full): a new #MatrixAPIFilterRules
 */
MatrixAPIFilterRules *
matrix_api_filter_rules_new(void)
{
    MatrixAPIFilterRules *rules;

    rules = g_new0(MatrixAPIFilterRules, 1);
    rules->refcount = 1;

    return rules;
}

static void
matrix_api_filter_rules_free(MatrixAPIFilterRules *rules)
{
    g_list_free_full(rules->rooms, g_free);
    g_list_free_full(rules->excluded_rooms, g_free);
    g_list_free_full(rules->senders, g_free);
    g_list_free_full(rules->excluded_senders, g_free);
    g_list_free_full(rules->types, g_free);
    g_list_free_full(rules->excluded_types, g_free);

    g_free(rules);
}

/**
 * matrix_api_filter_rules_ref:
 * @rules: a #MatrixAPIFilterRules
 *
 * Increase reference count of @rules by one.
 *
 * Returns: (transfer none): the same #MatrixAPIFilterRules
 */
MatrixAPIFilterRules *
matrix_api_filter_rules_ref(MatrixAPIFilterRules *rules)
{
    rules->refcount++;

    return rules;
}

/**
 * matrix_api_filter_rules_unref:
 * @rules: a #MatrixAPIFilterRules
 *
 * Decrease reference count of @rules by one. If reference count
 * reaches zero, @rules is freed.
 */
void
matrix_api_filter_rules_unref(MatrixAPIFilterRules *rules)
{
    if (--rules->refcount == 0) {
        matrix_api_filter_rules_free(rules);
    }
}

/**
 * matrix_api_filter_rules_set_limit:
 * @rules: a #MatrixAPIFilterRules
 * @limit: (in): the maximum number of events to return.
 *
 * Set the maximum number of events to return by the filter. If @limit
 * is <code>0</code>, no limit will be applied.
 */
void
matrix_api_filter_rules_set_limit(MatrixAPIFilterRules *rules, guint limit)
{
    rules->limit = limit;
}

/**
 * matrix_api_filter_rules_get_limit:
 * @rules: a #MatrixAPIFilterRules
 *
 * Get the current limit set in @rules.
 *
 * Returns: the limit currently set
 */
guint
matrix_api_filter_rules_get_limit(MatrixAPIFilterRules *rules)
{
    return rules->limit;
}

/**
 * matrix_api_filter_rules_set_senders:
 * @rules: a #MatrixAPIFilterRules
 * @senders: (in) (element-type utf8) (transfer full) (allow-none):
 *           a list of Matrix user IDs. Events from these users will
 *           be included in the filtered event list.If %NULL then all
 *           senders are included. See
 *           matrix_api_filter_rules_add_sender() for wildcarding
 *           possibilities
 *
 * Set the list of user IDs to include in the filtered events. @rules
 * takes ownership of @senders, so it should not be freed nor modified
 * directly after this call.
 */
void
matrix_api_filter_rules_set_senders(MatrixAPIFilterRules *rules, GList *senders)
{
    g_list_free_full(rules->senders, g_free);
    rules->senders = senders;
}

/**
 * matrix_api_filter_rules_add_sender:
 * @rules: a #MatrixAPIFilterRules
 * @sender: (in): a Matrix user ID to add to the included senders
 *          list. A <code>*</code> can be used as a wildcard to match
 *          any sequence of characters
 *
 * Add @sender to the list of user IDs to include in the filtered
 * event list. If @sender is already included in the senders list,
 * nothing happens.
 */
void
matrix_api_filter_rules_add_sender(MatrixAPIFilterRules *rules,
                                   const gchar *sender)
{
    g_return_if_fail(sender != NULL);

    if (!g_list_find_custom(rules->senders, sender, (GCompareFunc)g_strcmp0)) {
        rules->senders = g_list_prepend(rules->senders, g_strdup(sender));
    }
}

/**
 * matrix_api_filter_rules_delete_sender:
 * @rules: a #MatrixAPIFilterRules
 * @sender: (in): the user ID to remove from the senders list
 *
 * Remove @sender from the list of user IDs to include in the filtered
 * event list.
 */
void
matrix_api_filter_rules_delete_sender(MatrixAPIFilterRules *rules,
                                      const gchar *sender)
{
    GList *sender_element;

    g_return_if_fail(sender != NULL);

    while (sender_element = g_list_find_custom(rules->senders, sender,
                                               (GCompareFunc)g_strcmp0)) {
        rules->senders = g_list_remove_link(rules->senders, sender_element);
        g_list_free_full(sender_element, g_free);
    }
}

/**
 * matrix_api_filter_rules_get_senders:
 * @rules: a #MatrixAPIFilterRules
 *
 * Get the list of user IDs that will be included in the filtered
 * events.
 *
 * Returns: (element-type utf8) (allow-none) (transfer none): the
 *          list of user IDs. The returned value is owned by @rules
 *          and should not be freed nor modified
 */
const GList *
matrix_api_filter_rules_get_senders(MatrixAPIFilterRules *rules)
{
    return rules->senders;
}

/**
 * matrix_api_filter_rules_set_excluded_senders:
 * @rules: a #MatrixAPIFilterRules
 * @senders: (in) (element-type utf8) (transfer full) (allow-none):
 *           a list of Matrix user IDs. Events from these users will
 *           be included in the filtered event list.If %NULL then all
 *           senders are included. See
 *           matrix_api_filter_rules_add_sender() for wildcarding
 *           possibilities
 *
 * Set the list of Matrix user IDs to exclude from the filtered
 * events. A matching sender will be excluded even if it is listed in
 * the senders list (specified by
 * e.g. matrix_api_filter_rules_set_senders()). @rules takes
 * ownership of @senders, so it should not be freed nor modified
 * directly after this call.
 */
void
matrix_api_filter_rules_set_excluded_senders(MatrixAPIFilterRules *rules,
                                             GList *senders)
{
    g_list_free_full(rules->excluded_senders, g_free);
    rules->excluded_senders = senders;
}

/**
 * matrix_api_filter_rules_add_excluded_sender:
 * @rules: a #MatrixAPIFilterRules
 * @sender: (in): a Matrix user ID to add to the excluded senders
 *          list. See matrix_api_filter_rules_add_sender() for
 *          wildcarding possibilities
 *
 * Add @sender to the list of user IDs to exclude from the filtered
 * event list. If @sender is already in the excluded senders list,
 * nothing happens.
 */
void
matrix_api_filter_rules_add_excluded_sender(MatrixAPIFilterRules *rules,
                                            const gchar *sender)
{
    g_return_if_fail(sender != NULL);

    if (!g_list_find_custom(rules->excluded_senders, sender,
                            (GCompareFunc)g_strcmp0)) {
        rules->excluded_senders = g_list_prepend(rules->excluded_senders,
                                                 g_strdup(sender));
    }
}

/**
 * matrix_api_filter_rules_delete_excluded_sender:
 * @rules: a #MatrixAPIFilterRules
 * @sender: (in): the Matrix user ID to remove from the excluded
 *          senders list
 *
 * Remove @sender from the list of user IDs to exclude from the
 * filtered event list.
 */
void
matrix_api_filter_rules_delete_excluded_sender(MatrixAPIFilterRules *rules,
                                               const gchar *sender)
{
    GList *sender_element;

    g_return_if_fail(sender != NULL);

    while (sender_element = g_list_find_custom(rules->excluded_senders, sender,
                                               (GCompareFunc)g_strcmp0)) {
        rules->excluded_senders = g_list_remove_link(rules->excluded_senders,
                                                     sender_element);
        g_list_free_full(sender_element, g_free);
    }
}

/**
 * matrix_api_filter_rules_get_excluded_senders:
 * @rules: a #MatrixAPIFilterRules
 *
 * Get the list of user IDs that will be excluded in the filtered
 * events.
 *
 * Returns: (element-type utf8) (allow-none) (transfer none): the
 *          list of user IDs to be excluded. The returned value is
 *          owned by @rules and should not be freed nor modified.
 */
const GList *
matrix_api_filter_rules_get_excluded_senders(MatrixAPIFilterRules *rules)
{
    return rules->excluded_senders;
}

/**
 * matrix_api_filter_rules_set_rooms:
 * @rules: a #MatrixAPIFilterRules
 * @rooms: (in) (element-type utf8) (transfer full) (allow-none): a
 *         list of room IDs. Events from these rooms will be included
 *         in the filtered event list.If %NULL then all rooms are
 *         included. See matrix_api_filter_rules_add_sender() for
 *         wildcarding possibilities
 *
 * Set the list of room IDs to include in the filtered events. @rules
 * takes ownership of @rooms, so it should not be freed nor modified
 * directly after this call.
 */
void
matrix_api_filter_rules_set_rooms(MatrixAPIFilterRules *rules, GList *rooms)
{
    g_list_free_full(rules->rooms, g_free);
    rules->rooms = rooms;
}

/**
 * matrix_api_filter_rules_add_room:
 * @rules: a #MatrixAPIFilterRules
 * @room: (in): a room ID to add to the included rooms list. See
 *        matrix_api_filter_rules_add_sender() for wildcarding
 *        possibilities
 *
 * Add @room to the list of room IDs to include in the filtered
 * event list. If @room is already included in the rooms list,
 * nothing happens.
 */
void
matrix_api_filter_rules_add_room(MatrixAPIFilterRules *rules,
                                 const gchar *room)
{
    g_return_if_fail(room != NULL);

    if (!g_list_find_custom(rules->rooms, room, (GCompareFunc)g_strcmp0)) {
        rules->rooms = g_list_prepend(rules->rooms, g_strdup(room));
    }
}

/**
 * matrix_api_filter_rules_delete_room:
 * @rules: a #MatrixAPIFilterRules
 * @room: (in): the room ID to remove from the rooms list
 *
 * Remove @room from the list of room IDs to include in the filtered
 * event list.
 */
void
matrix_api_filter_rules_delete_room(MatrixAPIFilterRules *rules,
                                    const gchar *room)
{
    GList *room_element;

    g_return_if_fail(room != NULL);

    while (room_element = g_list_find_custom(rules->rooms, room,
                                             (GCompareFunc)g_strcmp0)) {
        rules->rooms = g_list_remove_link(rules->rooms, room_element);
        g_list_free_full(room_element, g_free);
    }
}

/**
 * matrix_api_filter_rules_get_rooms:
 * @rules: a #MatrixAPIFilterRules
 *
 * Get the list of room IDs that will be included in the filtered
 * events.
 *
 * Returns: (element-type utf8) (allow-none) (transfer none): the
 *          list of room IDs. The returned value is owned by @rules
 *          and should not be freed nor modified
 */
const GList *
matrix_api_filter_rules_get_rooms(MatrixAPIFilterRules *rules)
{
    return rules->rooms;
}

/**
 * matrix_api_filter_rules_set_excluded_rooms:
 * @rules: a #MatrixAPIFilterRules
 * @rooms: (in) (element-type utf8) (transfer full) (allow-none): a
 *         list of room IDs. Events from these rooms will be included
 *         in the filtered event list.If %NULL, then all rooms are
 *         included. See matrix_api_filter_rules_add_sender() for
 *         wildcarding possibilities
 *
 * Set the list of room IDs to exclude from the filtered events. A
 * matching room will be excluded even if it is listed in the rooms
 * list (specified by
 * e.g. matrix_api_filter_rules_set_rooms()). @rules takes ownership
 * of @rooms, so it should not be freed nor modified directly after
 * this call.
 */
void
matrix_api_filter_rules_set_excluded_rooms(MatrixAPIFilterRules *rules,
                                           GList *rooms)
{
    g_list_free_full(rules->excluded_rooms, g_free);
    rules->excluded_rooms = rooms;
}

/**
 * matrix_api_filter_rules_add_excluded_room:
 * @rules: a #MatrixAPIFilterRules
 * @room: (in): a room ID to add to the excluded rooms list. See
 *        matrix_api_filter_rules_add_sender() for wildcarding
 *        possibilities
 *
 * Add @room to the list of room IDs to exclude from the filtered
 * event list. If @room is already in the excluded rooms list, nothing
 * happens.
 */
void
matrix_api_filter_rules_add_excluded_room(MatrixAPIFilterRules *rules,
                                          const gchar *room)
{
    g_return_if_fail(room != NULL);

    if (!g_list_find_custom(rules->excluded_rooms, room,
                            (GCompareFunc)g_strcmp0)) {
        rules->excluded_rooms = g_list_prepend(rules->excluded_rooms,
                                               g_strdup(room));
    }
}

/**
 * matrix_api_filter_rules_delete_excluded_room:
 * @rules: a #MatrixAPIFilterRules
 * @room: (in): the room ID to remove from the excluded rooms list
 *
 * Remove @room from the list of room IDs to exclude from the filtered
 * event list.
 */
void
matrix_api_filter_rules_delete_excluded_room(MatrixAPIFilterRules *rules,
                                             const gchar *room)
{
    GList *room_element;

    g_return_if_fail(room != NULL);

    while (room_element = g_list_find_custom(rules->excluded_rooms, room,
                                             (GCompareFunc)g_strcmp0)) {
        rules->excluded_rooms = g_list_remove_link(rules->excluded_rooms,
                                                   room_element);
        g_list_free_full(room_element, g_free);
    }
}

/**
 * matrix_api_filter_rules_get_excluded_rooms:
 * @rules: a #MatrixAPIFilterRules
 *
 * Get the list of room IDs that will be excluded in the filtered
 * events.
 *
 * Returns: (element-type utf8) (allow-none) (transfer none): the
 *          list of room IDs to be excluded. The returned value is
 *          owned by @rules and should not be freed nor modified.
 */
const GList *
matrix_api_filter_rules_get_excluded_rooms(MatrixAPIFilterRules *rules)
{
    return rules->excluded_rooms;
}

/**
 * matrix_api_filter_rules_set_types:
 * @rules: a #MatrixAPIFilterRules
 * @types: (in) (element-type utf8) (transfer full) (allow-none): a
 *         list of event types to include. If %NULL then all event
 *         types are included. See
 *         matrix_api_filter_rules_add_sender() for wildcarding
 *         possibilities
 *
 * Set the list of event types to be included in the filtered events.
 */
void
matrix_api_filter_rules_set_types(MatrixAPIFilterRules *rules, GList *types)
{
    g_list_free_full(rules->types, g_free);
    rules->types = types;
}

/**
 * matrix_api_filter_rules_add_type:
 * @rules: a #MatrixAPIFilterRules
 * @type: (in): an event type to add to the list of included
 *        events. See matrix_api_filter_rules_add_sender() for
 *        wildcarding possibilities
 *
 * Add @type to the list of event types to include in the filtered
 * event list. If @type is already included in the types list, nothing
 * happens.
 */
void
matrix_api_filter_rules_add_type(MatrixAPIFilterRules *rules, const gchar *type)
{
    g_return_if_fail(type != NULL);

    if (g_list_find_custom(rules->types, type, (GCompareFunc)g_strcmp0)) {
        rules->types = g_list_prepend(rules->types, g_strdup(type));
    }
}

/**
 * matrix_api_filter_rules_delete_type:
 * @rules: a #MatrixAPIFilterRules
 * @type: (in): a type to remove from the list of included event
 *        types. See matrix_api_filter_rules_add_sender() for
 *        wildcarding possibilities
 *
 * Remove @type from the list of excluded event type list.
 */
void
matrix_api_filter_rules_delete_type(MatrixAPIFilterRules *rules,
                                    const gchar *type)
{
    GList *type_element;

    g_return_if_fail(type != NULL);

    while (type_element = g_list_find_custom(rules->types, type,
                                             (GCompareFunc)g_strcmp0)) {
        rules->types = g_list_remove_link(rules->types, type_element);
        g_list_free_full(type_element, g_free);
    }
}

/**
 * matrix_api_filter_rules_get_types:
 * @rules: a #MatrixAPIFilterRules
 *
 * Get the list of event types that will be included in the filtered
 * events.
 *
 * Returns: (element-type utf8) (allow-none) (transfer none): the
 *          list of event types. The returned values is owned by
 *          @rules and should not be freed nor modified
 */
const GList *
matrix_api_filter_rules_get_types(MatrixAPIFilterRules *rules)
{
    return rules->types;
}

/**
 * matrix_api_filter_rules_set_excluded_types:
 * @rules: a #MatrixAPIFilterRules
 * @types: (in) (element-type utf8) (transfer full) (allow-none): a
 *         list of event types to exclude. If %NULL then no event
 *         types are excluded. A matching type will be excluded even
 *         if it is listed in the included types. See
 *         matrix_api_filter_rules_add_sender() for wildcarding
 *         possibilities
 *
 * Set the list of event types to be excluded from the filtered
 * events. A matching type will be excluded even if it is listed in
 * the types list (specified by
 * e.g. matrix_api_filter_rules_set_types()).
 */
void
matrix_api_filter_rules_set_excluded_types(MatrixAPIFilterRules *rules,
                                           GList *types)
{
    g_list_free_full(rules->excluded_types, g_free);
    rules->excluded_types = types;
}

/**
 * matrix_api_filter_rules_add_excluded_type:
 * @rules: a #MatrixAPIFilterRules
 * @type: (in): an event type to add to the excluded event type
 *        list. See matrix_api_filter_rules_add_sender() for
 *        wildcarding possibilities
 *
 * Add @type to the list of excluded event types.
 */
void
matrix_api_filter_rules_add_excluded_type(MatrixAPIFilterRules *rules,
                                          const gchar *type)
{
    g_return_if_fail(type != NULL);

    if (!g_list_find_custom(rules->excluded_types, type,
                            (GCompareFunc)g_strcmp0)) {
        rules->excluded_types = g_list_prepend(rules->excluded_types,
                                               g_strdup(type));
    }
}

/**
 * matrix_api_filter_rules_delete_excluded_type:
 * @rules: a #MatrixAPIFilterRules
 * @type: (in): the event type to be removed from the excluded types
 *        list
 *
 * Remove @type from the list of event types to be excluded from the
 * filtered event list.
 */
void
matrix_api_filter_rules_delete_excluded_type(MatrixAPIFilterRules *rules,
                                             const gchar *type)
{
    GList *type_element;

    g_return_if_fail(type != NULL);

    while (type_element = g_list_find_custom(rules->excluded_types, type,
                                             (GCompareFunc)g_strcmp0)) {
        rules->excluded_types = g_list_remove_link(rules->excluded_types,
                                                   type_element);
        g_list_free_full(type_element, g_free);
    }
}

/**
 * matrix_api_filter_rules_get_excluded_types:
 * @rules: a #MatrixAPIFilterRules
 *
 * Get the list of event types that will be excluded from the filtered
 * events.
 *
 * Returns: (element-type utf8) (allow-none) (transfer none): the
 *          list of event types. The returned value is owned by
 *          @rules and should not be freed nor modified
 */
const GList *
matrix_api_filter_rules_get_excluded_types(MatrixAPIFilterRules *rules)
{
    return rules->excluded_types;
}

static void
json_add_string(gchar *str, JsonBuilder *builder)
{
    json_builder_add_string_value(builder, str);
}

/**
 * matrix_api_filter_rules_get_json_node:
 * @rules: a #MatrixAPIFilterRules
 *
 * Gets the #JsonNode representation of this filtering ruleset.
 *
 * Returns: (transfer full): the JSON representation of the filtering
 *          data as a #JsonNode
 */
JsonNode *
matrix_api_filter_rules_get_json_node(MatrixAPIFilterRules *rules)
{
    JsonBuilder *builder;
    JsonNode *node;

    builder = json_builder_new();
    json_builder_begin_object(builder);

    json_builder_set_member_name(builder, "limit");
    json_builder_add_int_value(builder, rules->limit);

    if (rules->rooms) {
        json_builder_set_member_name(builder, "rooms");
        json_builder_begin_array(builder);
        g_list_foreach(rules->rooms, (GFunc)json_add_string, builder);
        json_builder_end_array(builder);
    }

    if (rules->excluded_rooms) {
        json_builder_set_member_name(builder, "not_rooms");
        json_builder_begin_array(builder);
        g_list_foreach(rules->excluded_rooms, (GFunc)json_add_string, builder);
        json_builder_end_array(builder);
    }

    if (rules->senders) {
        json_builder_set_member_name(builder, "senders");
        json_builder_begin_array(builder);
        g_list_foreach(rules->senders, (GFunc)json_add_string, builder);
        json_builder_end_array(builder);
    }

    if (rules->excluded_senders) {
        json_builder_set_member_name(builder, "not_senders");
        json_builder_begin_array(builder);
        g_list_foreach(rules->excluded_senders,
                       (GFunc)json_add_string, builder);
        json_builder_end_array(builder);
    }

    if (rules->types) {
        json_builder_set_member_name(builder, "types");
        json_builder_begin_array(builder);
        g_list_foreach(rules->types, (GFunc)json_add_string, builder);
        json_builder_end_array(builder);
    }

    if (rules->excluded_types) {
        json_builder_set_member_name(builder, "not_types");
        json_builder_begin_array(builder);
        g_list_foreach(rules->excluded_types, (GFunc)json_add_string, builder);
        json_builder_end_array(builder);
    }

    json_builder_end_object(builder);

    node = json_builder_get_root(builder);
    g_object_unref(builder);

    return node;
}

/**
 * matrix_api_filter_rules_get_json_data:
 * @rules: a #MatrixAPIFilterRules
 * @datalen: (out): storage for the the length of the JSON data or
 *           %NULL
 *
 * Gets the string representation of these filtering rules, as a JSON
 * object.
 *
 * Returns: (transfer full): the JSON representation of the filtering
 *          rule data as a string
 */
gchar *
matrix_api_filter_rules_get_json_data(MatrixAPIFilterRules *rules,
                                      gsize *datalen)
{
    JsonGenerator *generator;
    JsonNode *node = matrix_api_filter_rules_get_json_node(rules);
    gchar *data;

    generator = json_generator_new();
    json_generator_set_root(generator, node);
    json_node_free(node);

    data = json_generator_to_data(generator, datalen);

    return data;
}
