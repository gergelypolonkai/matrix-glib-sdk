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

#ifndef __MATRIX_GLIB_SDK_EVENT_RECEIPT_H__
# define __MATRIX_GLIB_SDK_EVENT_RECEIPT_H__

# include <glib-object.h>
# include "matrix-event-base.h"

G_BEGIN_DECLS

# define MATRIX_EVENT_TYPE_RECEIPT matrix_event_receipt_get_type()
G_DECLARE_DERIVABLE_TYPE(MatrixEventReceipt, matrix_event_receipt, MATRIX_EVENT, RECEIPT, MatrixEventBase);

struct _MatrixEventReceiptClass {
    MatrixEventBaseClass parent_class;
};

GType matrix_event_receipt_get_type (void) G_GNUC_CONST;
MatrixEventReceipt* matrix_event_receipt_new (void);
MatrixEventReceipt* matrix_event_receipt_construct (GType object_type);
const gchar* matrix_event_receipt_get_room_id (MatrixEventReceipt *event);
void matrix_event_receipt_set_room_id (MatrixEventReceipt *event, const gchar *room_id);

G_END_DECLS

#endif  /* __MATRIX_GLIB_SDK_EVENT_RECEIPT_H__ */
