VALA_PKG_LIST = \
	--pkg=gio-2.0 \
	--pkg=Json-1.0 \
	--pkg=libsoup-2.4 \
	--pkg=gee-0.8 \
	$(NULL)

VALADOC_FLAGS = \
	--driver $(VALAC_VERSION) \
	--force \
	--package-name=Matrix-$(MATRIX_GLIB_API_VERSION) \
	--package-version=$(PACKAGE_VERSION) \
	-b $(top_srcdir)/src \
	$(VALA_PKG_LIST) \
	--vapidir=$(top_srcdir)/src \
	$(top_srcdir)/src/c-api.vapi \
	$(NULL)
