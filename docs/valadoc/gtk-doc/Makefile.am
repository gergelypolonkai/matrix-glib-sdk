include $(top_srcdir)/vala-globals.mk
CLEANFILES =

sources = $(top_srcdir)/src/*.vala

gtkdocdir = $(datadir)/gtk-doc/html/matrix-glib
gtkdoc_DATA = \
	gtk-doc/matrix-glib/*.txt \
	gtk-doc/matrix-glib/*.xml \
	gtk-doc/matrix-glib/xml/*.xml \
	gtk-doc/matrix-glib/xml/*.top \
	gtk-doc/matrix-glib/xml/*.bottom \
	gtk-doc/matrix-glib/*.types \
	gtk-doc/matrix-glib/sgml.stamp \
	$(NULL)

$(gtkdoc_DATA): matrix-glib-gtk-doc

matrix-glib-gtk-doc:
	$(VALADOC) -o gtk-doc/matrix-glib --doclet=gtkdoc \
	    -X $(top_builddir)/src/matrix-glib.h \
	    $(VALADOC_FLAGS) $(sources)

all: matrix-glib-gtk-doc

clean-local:
	rm -rf gtk-doc
	rm -rf matrix-glib
