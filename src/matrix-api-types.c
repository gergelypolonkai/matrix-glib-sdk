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
 * MatrixAPIError:
 * @MATRIX_API_ERROR_NONE: no error
 * @MATRIX_API_ERROR_MISSING_TOKEN: authorization token is missing
 *                                  from the request
 * @MATRIX_API_ERROR_FORBIDDEN: access was forbidden (e.g. due to a
 *                              missing/invalid token, or using a bad
 *                              password during login)
 * @MATRIX_API_ERROR_UNKNOWN: an error unknown to the Matrix server
 * @MATRIX_API_ERROR_UNKNOWN_ERROR: an error unknown to this library
 *
 * Value mappings from Matrix.org API error codes
 * (e.g. <code>M_MISSING_TOKEN</code>). They should be set
 * automatically by API calls, if the response contains an error code.
 */

/**
 * MATRIX_API_ERROR:
 *
 * Error domain for Matrix GLib SDK API. See #GError for more
 * information on error domains.
 */

/**
 * matrix_api_error_quark:
 *
 * Gets the Matrix API error #GQuark
 */
G_DEFINE_QUARK(matrix-api-error-quark, matrix_api_error);

/**
 * MatrixAPIEventFormat:
 * @MATRIX_API_EVENT_FORMAT_DEFAULT: event format will be omitted from
 *                                   the filter, so the server will
 *                                   use its default (usually
 *                                   @MATRIX_API_EVENT_FORMAT_FEDERATION)
 * @MATRIX_API_EVENT_FORMAT_CLIENT: return the events in a format
 *                                  suitable for clients
 * @MATRIX_API_EVENT_FORMAT_FEDERATION: return the raw event as
 *                                      receieved over federation
 *
 * Event format received when synchronizing.
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
    g_object_unref(generator);

    return data;
}

/**
 * MatrixAPIRoomFilter:
 *
 * An opaque structure to hold room filters.
 */
struct _MatrixAPIRoomFilter {
    gboolean include_leave;
    MatrixAPIFilterRules *ephemeral;
    MatrixAPIFilterRules *state;
    MatrixAPIFilterRules *timeline;
    guint refcount;
};

G_DEFINE_BOXED_TYPE(MatrixAPIRoomFilter, matrix_api_room_filter,
                    (GBoxedCopyFunc)matrix_api_room_filter_ref,
                    (GBoxedFreeFunc)matrix_api_room_filter_unref);

/**
 * matrix_api_room_filter_new:
 *
 * Create a new #MatrixAPIRoomFilter object with reference count of 1.
 *
 * Returns: (transfer full): a new #MatrixAPIRoomFilter
 */
MatrixAPIRoomFilter *
matrix_api_room_filter_new(void)
{
    MatrixAPIRoomFilter *filter;

    filter = g_new0(MatrixAPIRoomFilter, 1);
    filter->refcount = 1;

    return filter;
}

static void
matrix_api_room_filter_free(MatrixAPIRoomFilter *filter)
{
    if (filter->ephemeral) {
        matrix_api_filter_rules_unref(filter->ephemeral);
    }

    if (filter->state) {
        matrix_api_filter_rules_unref(filter->state);
    }

    if (filter->timeline) {
        matrix_api_filter_rules_unref(filter->timeline);
    }

    g_free(filter);
}

/**
 * matrix_api_room_filter_ref:
 * @filter: a #MatrixAPIRoomFilter
 *
 * Increase reference count of @filter by one.
 *
 * Returns: (transfer none): the same #MatrixAPIRoomFilter
 */
MatrixAPIRoomFilter *
matrix_api_room_filter_ref(MatrixAPIRoomFilter *filter)
{
    filter->refcount++;

    return filter;
}

/**
 * matrix_api_room_filter_unref:
 * @filter: a #MatrixAPIRoomFilter
 *
 * Decrease reference count of @filter by one. If reference count
 * reaches zero, @filter is freed.
 */
void
matrix_api_room_filter_unref(MatrixAPIRoomFilter *filter)
{
    if (--filter->refcount == 0) {
        matrix_api_room_filter_free(filter);
    }
}

/**
 * matrix_api_room_filter_set_include_leave:
 * @filter: a #MatrixAPIRoomFilter
 * @include_leave: %TRUE if events from left rooms should be included
 *
 * Sets if events for rooms that the user has left should be included
 * in the filtered event list.
 */
void
matrix_api_room_filter_set_include_leave(MatrixAPIRoomFilter *filter,
                                         gboolean include_leave)
{
    filter->include_leave = include_leave;
}

/**
 * matrix_api_room_filter_get_include_leave:
 * @filter: a #MatrixAPIRoomFilter
 *
 * If %TRUE, events from rooms that the user has left will be included
 * in the filtered event list.
 *
 * Returns: the value of <code>include_leave</code>
 */
gboolean
matrix_api_room_filter_get_include_leave(MatrixAPIRoomFilter *filter)
{
    return filter->include_leave;
}

/**
 * matrix_api_room_filter_set_ephemeral:
 * @filter: a #MatrixAPIRoomFilter
 * @rules: (transfer none): filtering rules for ephemeral events
 *
 * Set filtering rules for ephemeral events, i.e. events that are not
 * recorded in the room history (typing notifications, receipts, etc.)
 * @filter gets a reference on @rules, so the caller may unref it.
 */
void
matrix_api_room_filter_set_ephemeral(MatrixAPIRoomFilter *filter,
                                     MatrixAPIFilterRules *rules)
{
    if (filter->ephemeral) {
        matrix_api_filter_rules_unref(filter->ephemeral);
    }

    filter->ephemeral = matrix_api_filter_rules_ref(rules);
}

/**
 * matrix_api_room_filter_get_ephemeral:
 * @filter: a #MatrixAPIRoomFilter
 *
 * Gets the filtering rules for ephemeral events. See
 * matrix_api_room_filter_set_ephemeral() for details.
 *
 * Returns: (transfer none): the filtering rules for ephemeral events.
 */
MatrixAPIFilterRules *
matrix_api_room_filter_get_ephemeral(MatrixAPIRoomFilter *filter)
{
    return filter->ephemeral;
}

/**
 * matrix_api_room_filter_set_state:
 * @filter: a #MatrixAPIRoomFilter
 * @rules: (transfer none): filtering rules for state events
 *
 * Sets filtering rules for state events. @filter obtains a reference
 * on @rules, so the caller may unref it safely.
 */
void
matrix_api_room_filter_set_state(MatrixAPIRoomFilter *filter,
                                 MatrixAPIFilterRules *rules)
{
    if (filter->state) {
        matrix_api_filter_rules_unref(filter->state);
    }

    filter->state = matrix_api_filter_rules_ref(rules);
}

/**
 * matrix_api_room_filter_get_state:
 * @filter: a #MatrixAPIRoomFilter
 *
 * Gets the filtering rules for state events.
 *
 * Returns: (transfer none): the filtering rules for state events
 */
MatrixAPIFilterRules *
matrix_api_room_filter_get_state(MatrixAPIRoomFilter *filter)
{
    return filter->state;
}

/**
 * matrix_api_room_filter_set_timeline:
 * @filter: a #MatrixAPIRoomFilter
 * @rules: (transfer none): filtering rules for timeline events
 *         (messages and state events from rooms)
 *
 * Set filtering rules for timeline events.
 */
void
matrix_api_room_filter_set_timeline(MatrixAPIRoomFilter *filter,
                                    MatrixAPIFilterRules *rules)
{
    if (filter->timeline) {
        matrix_api_filter_rules_unref(filter->timeline);
    }

    filter->timeline = matrix_api_filter_rules_ref(rules);
}

/**
 * matrix_api_room_filter_get_timeline:
 * @filter: a #MatrixAPIRoomFilter
 *
 * Gets filtering rules for timeline events (see
 * matrix_api_room_filter_set_timeline() for details.
 *
 * Returns: (transfer none): the filtering rules for timeline events
 */
MatrixAPIFilterRules *
matrix_api_room_filter_get_timeline(MatrixAPIRoomFilter *filter)
{
    return filter->timeline;
}

/**
 * matrix_api_room_filter_get_json_node:
 * @filter: a #MatrixAPIRoomFilter
 *
 * Gets the #JsonNode representation of the filtering ruleset.
 *
 * Returns: (transfer full): the JSON representation of the filtering
 *          ruleset as a #JsonNode
 */
JsonNode *
matrix_api_room_filter_get_json_node(MatrixAPIRoomFilter *filter)
{
    JsonBuilder *builder;
    JsonNode *node;

    builder = json_builder_new();
    json_builder_begin_object(builder);

    json_builder_set_member_name(builder, "include_leave");
    json_builder_add_boolean_value(builder, filter->include_leave);

    if (filter->ephemeral) {
        json_builder_set_member_name(builder, "ephemeral");
        json_builder_add_value(builder,
                               matrix_api_filter_rules_get_json_node(
                                       filter->ephemeral));
    }

    if (filter->state) {
        json_builder_set_member_name(builder, "state");
        json_builder_add_value(builder,
                               matrix_api_filter_rules_get_json_node(
                                       filter->state));
    }

    if (filter->timeline) {
        json_builder_set_member_name(builder, "timeline");
        json_builder_add_value(builder,
                               matrix_api_filter_rules_get_json_node(
                                       filter->timeline));
    }

    json_builder_end_object(builder);

    node = json_builder_get_root(builder);
    g_object_unref(builder);

    return node;
}

/**
 * matrix_api_room_filter_get_json_data:
 * @filter: a #MatrixAPIRoomFilter
 * @datalen: (out) (allow-none): storage for the length of the JSON
 *           data, or %NULL
 *
 * Get the string representation of the filtering ruleset as a JSON
 * object.
 *
 * Returns: the JSON object representation of the filtering ruleset
 *          data as a string
 */
gchar *
matrix_api_room_filter_get_json_data(MatrixAPIRoomFilter *filter,
                                     gsize *datalen)
{
    JsonGenerator *generator;
    JsonNode *node = matrix_api_room_filter_get_json_node(filter);
    gchar *data;

    generator = json_generator_new();
    json_generator_set_root(generator, node);
    json_node_free(node);

    data = json_generator_to_data(generator, datalen);
    g_object_unref(generator);

    return data;
}

/**
 * MatrixAPIFilter:
 *
 * An opaque structure to hold an event filter.
 */
struct _MatrixAPIFilter {
    GList *event_fields;
    MatrixAPIEventFormat event_format;
    MatrixAPIFilterRules *presence;
    MatrixAPIRoomFilter *room;
    guint refcount;
};

G_DEFINE_BOXED_TYPE(MatrixAPIFilter, matrix_api_filter,
                    (GBoxedCopyFunc)matrix_api_filter_ref,
                    (GBoxedFreeFunc)matrix_api_filter_unref);

/**
 * matrix_api_filter_new:
 *
 * Create a new #MatrixAPIFilter object with reference count of 1.
 *
 * Returns: (transfer full): a new #MatrixAPIFilter
 */
MatrixAPIFilter *
matrix_api_filter_new(void)
{
    MatrixAPIFilter *filter;

    filter = g_new0(MatrixAPIFilter, 1);
    filter->refcount = 1;

    return filter;
}

static void
matrix_api_filter_free(MatrixAPIFilter *filter)
{
    g_list_free_full(filter->event_fields, g_free);

    if (filter->presence) {
        matrix_api_filter_rules_unref(filter->presence);
    }

    if (filter->room) {
        matrix_api_room_filter_unref(filter->room);
    }

    g_free(filter);
}

/**
 * matrix_api_filter_ref:
 * @filter: a #MatrixAPIFilter
 *
 * Increase reference count of @filter by one.
 *
 * Returns: (transfer none): the same #MatrixAPIFilter
 */
MatrixAPIFilter *
matrix_api_filter_ref(MatrixAPIFilter *filter)
{
    filter->refcount++;

    return filter;
}

/**
 * matrix_api_filter_unref:
 * @filter: a #MatrixAPIFilter
 *
 * Decrease reference count of @filter by one. If reference count
 * reaches zero, @filter is freed.
 */
void
matrix_api_filter_unref(MatrixAPIFilter *filter)
{
    if (--filter->refcount == 0) {
        matrix_api_filter_free(filter);
    }
}

/**
 * matrix_api_filter_set_event_fields:
 * @filter: a #MatrixAPIFilter
 * @event_fields: (in) (element-type utf8) (transfer full) (allow-none):
 *                a list of event fields to include. If %NULL then all
 *                fields are included. The entries may include
 *                <code>.</code> charaters to indicate sub-fields. So
 *                <code>['content.body']</code> will include the
 *                <code>body</code> field of the <code>content</code>
 *                object. A literal <code>.</code> character in a
 *                field name may be escaped using a <code>\</code>. A
 *                server may include more fields than were requested
 *
 * Set the event fields to include in the filtered events.
 */
void
matrix_api_filter_set_event_fields(MatrixAPIFilter *filter,
                                   GList *event_fields)
{
    g_list_free_full(filter->event_fields, g_free);
    filter->event_fields = event_fields;
}

/**
 * matrix_api_filter_add_event_field:
 * @filter: a #MatrixAPIFilter
 * @event_field: an event field to add to the list. See
 *               matrix_api_filter_set_event_fields() for details
 *
 * Add an event field to the list of fields that will be present in
 * the filtered events.
 */
void
matrix_api_filter_add_event_field(MatrixAPIFilter *filter,
                                  const gchar *event_field)
{
    g_return_if_fail(event_field != NULL);

    if (!g_list_find_custom(filter->event_fields, event_field,
                            (GCompareFunc)g_strcmp0)) {
        filter->event_fields = g_list_prepend(filter->event_fields,
                                              g_strdup(event_field));
    }
}

/**
 * matrix_api_filter_delete_event_field:
 * @filter: a #MatrixAPIFilter
 * @event_field: an event field to remove from the list
 *
 * Remove @event_field from the list of fields that will be present in
 * the filtered events.
 */
void
matrix_api_filter_delete_event_field(MatrixAPIFilter *filter,
                                     const gchar *event_field)
{
    GList *event_field_element;

    g_return_if_fail(event_field != NULL);

    while (event_field_element = g_list_find_custom(filter->event_fields,
                                                    event_field,
                                                    (GCompareFunc)g_strcmp0)) {
        filter->event_fields = g_list_remove_link(filter->event_fields,
                                                  event_field_element);
        g_list_free_full(event_field_element, g_free);
    }
}

/**
 * matrix_api_filter_get_event_fields:
 * @filter: a #MatrixAPIFilter
 *
 * Get the list of event fields that will be present in the filtered
 * events.
 *
 * Returns: (transfer none) (element-type utf8) (allow-none): the list
 *          of event fields. The returned value is owned by @filter
 *          and should not be freed nor modified.
 */
const GList *
matrix_api_filter_get_event_fields(MatrixAPIFilter *filter)
{
    return filter->event_fields;
}

/**
 * matrix_api_filter_set_event_format:
 * @filter: a #MatrixAPIFilter
 * @event_format: the desired event format for filtered events
 *
 * Set the desired event format for the filtered events (e.g. for
 * matrix_api_sync())
 */
void
matrix_api_filter_set_event_format(MatrixAPIFilter *filter,
                                   MatrixAPIEventFormat event_format)
{
    filter->event_format = event_format;
}

/**
 * matrix_api_filter_get_event_format:
 * @filter: a #MatrixAPIFilter
 *
 * Get the desired event format set in @filter.
 *
 * Returns: the event format currently set
 */
MatrixAPIEventFormat
matrix_api_filter_get_event_format(MatrixAPIFilter *filter)
{
    return filter->event_format;
}

/**
 * matrix_api_filter_set_presence_filter:
 * @filter: a #MatrixAPIFilter
 * @presence_filter: (transfer none): the desired filters to use
 *
 * Set a filtering ruleset for presence events.
 */
void
matrix_api_filter_set_presence_filter(MatrixAPIFilter *filter,
                                      MatrixAPIFilterRules *presence_filter)
{
    if (filter->presence) {
        matrix_api_filter_rules_unref(filter->presence);
    }

    filter->presence = matrix_api_filter_rules_ref(presence_filter);
}

/**
 * matrix_api_filter_get_presence_filter:
 * @filter: a #MatrixAPIFilter
 *
 * Get the current filtering ruleset for presence events.
 *
 * Returns: (transfer none): the current ruleset. The returned value
 *          is owned by @filter; if the callee wants to use it
 *          separately, it should create a reference for it
 */
MatrixAPIFilterRules *
matrix_api_filter_get_presence_filter(MatrixAPIFilter *filter)
{
    return filter->presence;
}

/**
 * matrix_api_filter_set_room_filter:
 * @filter: a #MatrixAPIFilter
 * @room_filter: the desired room filters to use in @filter
 *
 * Set a new filtering ruleset for room events in @filter.
 */
void
matrix_api_filter_set_room_filter(MatrixAPIFilter *filter,
                                  MatrixAPIRoomFilter *room_filter)
{
    if (filter->room) {
        matrix_api_room_filter_unref(filter->room);
    }

    filter->room = matrix_api_room_filter_ref(room_filter);
}

/**
 * matrix_api_filter_get_room_filter:
 * @filter: a #MatrixAPIFilter
 *
 * Get the filtering ruleset for room events in @filter.
 *
 * Returns: (transfer none): the current filtering ruleset for room
 * events
 */
MatrixAPIRoomFilter *
matrix_api_filter_get_room_filter(MatrixAPIFilter *filter)
{
    return filter->room;
}

/**
 * MatrixAPI3PidCredential:
 *
 * An opaque structure to store credentials to use with Identity
 * Server communication.
 */
struct _MatrixAPI3PidCredential {
    gchar *id_server;
    gchar *session_id;
    gchar *client_secret;
    guint refcount;
};

G_DEFINE_BOXED_TYPE(MatrixAPI3PidCredential, matrix_api_3pid_credential,
                    (GBoxedCopyFunc)matrix_api_3pid_credential_ref,
                    (GBoxedFreeFunc)matrix_api_3pid_credential_unref);

/**
 * matrix_api_3pid_credential_new:
 *
 * Create a new #MatrixAPI3PidCredential object with reference count
 * of 1.
 *
 * Returns: (transfer full): a new #MatrixAPI3PidCredential
 */
MatrixAPI3PidCredential *
matrix_api_3pid_credential_new(void)
{
    MatrixAPI3PidCredential *credential;

    credential = g_new0(MatrixAPI3PidCredential, 1);
    credential->refcount = 1;

    return credential;
}

static void
matrix_api_3pid_credential_free(MatrixAPI3PidCredential *credential)
{
    g_free(credential->id_server);
    g_free(credential->session_id);
    g_free(credential->client_secret);
    g_free(credential);
}

/**
 * matrix_api_3pid_credential_ref:
 * @credential: a #MatrixAPI3PidCredential
 *
 * Increase reference count of @credential by one.
 *
 * Returns: (transfer none): the same #MatrixAPI3PidCredential
 */
MatrixAPI3PidCredential *
matrix_api_3pid_credential_ref(MatrixAPI3PidCredential *credential)
{
    credential->refcount++;

    return credential;
}

/**
 * matrix_api_3pid_credential_unref:
 * @credential: a #MatrixAPI3PidCredential
 *
 * Decrease reference count of @credential by one. If reference count
 * reaches zero, @credential gets freed.
 */
void
matrix_api_3pid_credential_unref(MatrixAPI3PidCredential *credential)
{
    if (--credential->refcount == 0) {
        matrix_api_3pid_credential_free(credential);
    }
}

/**
 * matrix_api_3pid_credential_set_id_server:
 * @credential: a #MatrixAPI3PidCredential
 * @id_server: the Identity Server to use
 *
 * Set the Identity Server to use for this credential.
 */
void
matrix_api_3pid_credential_set_id_server(MatrixAPI3PidCredential *credential,
                                         const gchar *id_server)
{
    g_free(credential->id_server);
    credential->id_server = g_strdup(id_server);
}

/**
 * matrix_api_3pid_credential_get_id_server:
 * @credential: a #MatrixAPI3PidCredential
 *
 * Get the Identity Server used for this credential.
 *
 * Returns: the Identity Server's name. The returned value is owned by
 *          @credential and should not be freed nor modified
 */
const gchar *
matrix_api_3pid_credential_get_id_server(MatrixAPI3PidCredential *credential)
{
    return credential->id_server;
}

/**
 * matrix_api_3pid_credential_set_session_id:
 * @credential: a #MatrixAPI3PidCredential
 * @session_id: the session identifier given by the Identity Server
 *
 * Set the session identifier got from the Identity Server.
 */
void
matrix_api_3pid_credential_set_session_id(MatrixAPI3PidCredential *credential,
                                          const gchar *session_id)
{
    g_free(credential->session_id);
    credential->session_id = g_strdup(session_id);
}

/**
 * matrix_api_3pid_credential_get_session_id:
 * @credential: a #MatrixAPI3PidCredential
 *
 * Get the session identifier for this credential.
 *
 * Returns: the session identifier. The returned value is owned by
 *          @credential and should not be freed nor modified.
 */
const gchar *
matrix_api_3pid_credential_get_session_id(MatrixAPI3PidCredential *credential)
{
    return credential->session_id;
}

/**
 * matrix_api_3pid_credential_set_client_secret:
 * @credential: a #MatrixAPI3PidCredential
 * @client_secret: the client secret used in the session with the
 *                 Identity Server
 *
 * Set the client secret that was used in the session with the
 * Identity Server.
 */
void
matrix_api_3pid_credential_set_client_secret(MatrixAPI3PidCredential *credential,
                                             const gchar *client_secret)
{
    g_free(credential->client_secret);
    credential->client_secret = g_strdup(client_secret);
}

/**
 * matrix_api_3pid_credential_get_client_secret:
 * @credential: a #MatrixAPI3PidCredential
 *
 * Get the client secret that was used in the session with the
 * Identity Server.
 *
 * Returns: the client secret. The returned value is owned by
 *          @credential and should not be freed nor modified.
 */
const gchar *
matrix_api_3pid_credential_get_client_secret(MatrixAPI3PidCredential *credential)
{
    return credential->client_secret;
}
