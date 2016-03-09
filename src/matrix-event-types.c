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

#include "matrix-glib.h"
#include "matrix-marshalers.h"

/*
  Borrowed from GLib

  If G_HAS_CONSTRUCTORS is true then the compiler support *both*
  constructors and destructors, in a sane way, including e.g. on
  library unload. If not you're on your own.

  Some compilers need #pragma to handle this, which does not work with macros,
  so the way you need to use this is (for constructors):

  #ifdef G_DEFINE_CONSTRUCTOR_NEEDS_PRAGMA
  #pragma G_DEFINE_CONSTRUCTOR_PRAGMA_ARGS(my_constructor)
  #endif
  G_DEFINE_CONSTRUCTOR(my_constructor)
  static void my_constructor(void) {
   ...
  }

*/

#ifndef __GTK_DOC_IGNORE__

#if  __GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ >= 7)

#define G_HAS_CONSTRUCTORS 1

#define G_DEFINE_CONSTRUCTOR(_func) static void __attribute__((constructor)) _func (void);
#define G_DEFINE_DESTRUCTOR(_func) static void __attribute__((destructor)) _func (void);

#elif defined (_MSC_VER) && (_MSC_VER >= 1500)
/* Visual studio 2008 and later has _Pragma */

#define G_HAS_CONSTRUCTORS 1

/* We do some weird things to avoid the constructors being optimized
 * away on VS2015 if WholeProgramOptimization is enabled. First we
 * make a reference to the array from the wrapper to make sure its
 * references. Then we use a pragma to make sure the wrapper function
 * symbol is always included at the link stage. Also, the symbols
 * need to be extern (but not dllexport), even though they are not
 * really used from another object file.
 */

/* We need to account for differences between the mangling of symbols
 * for Win32 (x86) and x64 programs, as symbols on Win32 are prefixed
 * with an underscore but symbols on x64 are not.
 */
#ifdef _WIN64
#define G_MSVC_SYMBOL_PREFIX ""
#else
#define G_MSVC_SYMBOL_PREFIX "_"
#endif

#define G_DEFINE_CONSTRUCTOR(_func) G_MSVC_CTOR (_func, G_MSVC_SYMBOL_PREFIX)
#define G_DEFINE_DESTRUCTOR(_func) G_MSVC_DTOR (_func, G_MSVC_SYMBOL_PREFIX)

#define G_MSVC_CTOR(_func,_sym_prefix) \
  static void _func(void); \
  extern int (* _array ## _func)(void);              \
  int _func ## _wrapper(void) { _func(); g_slist_find (NULL,  _array ## _func); return 0; } \
  __pragma(comment(linker,"/include:" _sym_prefix # _func "_wrapper")) \
  __pragma(section(".CRT$XCU",read)) \
  __declspec(allocate(".CRT$XCU")) int (* _array ## _func)(void) = _func ## _wrapper;

#define G_MSVC_DTOR(_func,_sym_prefix) \
  static void _func(void); \
  extern int (* _array ## _func)(void);              \
  int _func ## _constructor(void) { atexit (_func); g_slist_find (NULL,  _array ## _func); return 0; } \
   __pragma(comment(linker,"/include:" _sym_prefix # _func "_constructor")) \
  __pragma(section(".CRT$XCU",read)) \
  __declspec(allocate(".CRT$XCU")) int (* _array ## _func)(void) = _func ## _constructor;

#elif defined (_MSC_VER)

#define G_HAS_CONSTRUCTORS 1

/* Pre Visual studio 2008 must use #pragma section */
#define G_DEFINE_CONSTRUCTOR_NEEDS_PRAGMA 1
#define G_DEFINE_DESTRUCTOR_NEEDS_PRAGMA 1

#define G_DEFINE_CONSTRUCTOR_PRAGMA_ARGS(_func) \
  section(".CRT$XCU",read)
#define G_DEFINE_CONSTRUCTOR(_func) \
  static void _func(void); \
  static int _func ## _wrapper(void) { _func(); return 0; } \
  __declspec(allocate(".CRT$XCU")) static int (*p)(void) = _func ## _wrapper;

#define G_DEFINE_DESTRUCTOR_PRAGMA_ARGS(_func) \
  section(".CRT$XCU",read)
#define G_DEFINE_DESTRUCTOR(_func) \
  static void _func(void); \
  static int _func ## _constructor(void) { atexit (_func); return 0; } \
  __declspec(allocate(".CRT$XCU")) static int (* _array ## _func)(void) = _func ## _constructor;

#elif defined(__SUNPRO_C)

/* This is not tested, but i believe it should work, based on:
 * http://opensource.apple.com/source/OpenSSL098/OpenSSL098-35/src/fips/fips_premain.c
 */

#define G_HAS_CONSTRUCTORS 1

#define G_DEFINE_CONSTRUCTOR_NEEDS_PRAGMA 1
#define G_DEFINE_DESTRUCTOR_NEEDS_PRAGMA 1

#define G_DEFINE_CONSTRUCTOR_PRAGMA_ARGS(_func) \
  init(_func)
#define G_DEFINE_CONSTRUCTOR(_func) \
  static void _func(void);

#define G_DEFINE_DESTRUCTOR_PRAGMA_ARGS(_func) \
  fini(_func)
#define G_DEFINE_DESTRUCTOR(_func) \
  static void _func(void);

#else

/* constructors not supported for this compiler */

#errer "Constructors are not supported by your compiler, but are needed for this library"
#endif

#endif /* __GTK_DOC_IGNORE__ */

#ifdef G_DEFINE_CONSTRUCTOR_NEEDS_PRAGMA
#pragma G_DEFINE_CONSTRUCTOR_PRAGMA_ARGS(matrix_event_types_ctor)
#endif

G_DEFINE_CONSTRUCTOR(matrix_event_types_ctor);

static void
matrix_event_types_ctor(void)
{
    matrix_event_register_type("m.room.member",
                               MATRIX_EVENT_TYPE_ROOM_MEMBER,
                               NULL);
    matrix_event_register_type("m.presence",
                               MATRIX_EVENT_TYPE_PRESENCE,
                               NULL);
    matrix_event_register_type("m.room.message",
                               MATRIX_EVENT_TYPE_ROOM_MESSAGE,
                               NULL);
    matrix_event_register_type("m.room.topic",
                               MATRIX_EVENT_TYPE_ROOM_TOPIC,
                               NULL);
    matrix_event_register_type("m.typing",
                               MATRIX_EVENT_TYPE_TYPING,
                               NULL);
    matrix_event_register_type("m.room.aliases",
                               MATRIX_EVENT_TYPE_ROOM_ALIASES,
                               NULL);
    matrix_event_register_type("m.receipt",
                               MATRIX_EVENT_TYPE_RECEIPT,
                               NULL);
    matrix_event_register_type("m.room.history_visibility",
                               MATRIX_EVENT_TYPE_ROOM_HISTORY_VISIBILITY,
                               NULL);
    matrix_event_register_type("m.room.join_rules",
                               MATRIX_EVENT_TYPE_ROOM_JOIN_RULES,
                               NULL);
    matrix_event_register_type("m.room.name",
                               MATRIX_EVENT_TYPE_ROOM_NAME,
                               NULL);
    matrix_event_register_type("m.tag",
                               MATRIX_EVENT_TYPE_TAG,
                               NULL);
    matrix_event_register_type("m.room.canonical_alias",
                               MATRIX_EVENT_TYPE_ROOM_CANONICAL_ALIAS,
                               NULL);
    matrix_event_register_type("m.room.create",
                               MATRIX_EVENT_TYPE_ROOM_CREATE,
                               NULL);
    matrix_event_register_type("m.room.power_levels",
                               MATRIX_EVENT_TYPE_ROOM_POWER_LEVELS,
                               NULL);
    matrix_event_register_type("m.room.avatar",
                               MATRIX_EVENT_TYPE_ROOM_AVATAR,
                               NULL);
    matrix_event_register_type("m.room.message.feedback",
                               MATRIX_EVENT_TYPE_ROOM_MESSAGE_FEEDBACK,
                               NULL);
    matrix_event_register_type("m.room.guest_access",
                               MATRIX_EVENT_TYPE_ROOM_GUEST_ACCESS,
                               NULL);
    matrix_event_register_type("m.room.redaction",
                               MATRIX_EVENT_TYPE_ROOM_REDACTION,
                               NULL);
    matrix_event_register_type("m.room.third_party_invite",
                               MATRIX_EVENT_TYPE_ROOM_THIRD_PARTY_INVITE,
                               NULL);
    matrix_event_register_type("m.call.invite",
                               MATRIX_EVENT_TYPE_CALL_INVITE,
                               NULL);
    matrix_event_register_type("m.call.candidates",
                               MATRIX_EVENT_TYPE_CALL_CANDIDATES,
                               NULL);
}

void
matrix_client_connect_event(MatrixClient *client,
                            GType event_gtype,
                            MatrixClientEventCallback callback,
                            gpointer user_data,
                            GDestroyNotify destroy_notify)
{
    GClosure *closure;
    GQuark equark;
    MatrixEventBaseClass *event_class = MATRIX_EVENT_BASE_CLASS(
            g_type_class_ref(event_gtype));
    guint event_signal_id = g_signal_lookup("event", MATRIX_TYPE_CLIENT);

    if (!MATRIX_EVENT_IS_BASE_CLASS(event_class)) {
        g_warning("Trying to connect to a type that is not derived from MatrixEvent");
        g_type_class_unref(event_class);

        return;
    }

    g_type_class_unref(event_class);
    equark = g_type_qname(event_gtype);

    closure = g_closure_ref(g_cclosure_new(G_CALLBACK(callback),
                                           user_data,
                                           (GClosureNotify)destroy_notify));
    g_closure_set_marshal(closure,
                          _matrix_marshal_VOID__STRING_BOXED_OBJECT);
    g_closure_sink(closure);

    g_signal_connect_closure_by_id(client,
                                   event_signal_id, equark,
                                   closure, FALSE);
}
