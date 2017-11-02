VALA_PKG_LIST = \
	--pkg=gio-2.0 \
	--pkg=json-glib-1.0 \
	--pkg=libsoup-2.4 \
	--pkg=c-api \
	--pkg=config \
	$(NULL)

VALADOC_FLAGS = \
	--driver $(VALAC_VERSION) \
	--force \
	--package-name=Matrix-$(MATRIX_GLIB_API_VERSION) \
	--package-version=$(PACKAGE_VERSION) \
	-b $(top_srcdir)/src \
	$(VALA_PKG_LIST) \
	--vapidir=$(top_srcdir)/vapi \
	$(NULL)
