bin_PROGRAMS=kiwix-serve
AM_CPPFLAGS=-I$(top_srcdir)/src/common -I$(top_srcdir)/src/zimlib/include -I$(top_srcdir)/src/ctpp2/include -I$(top_srcdir)/src/pugixml
kiwix_serve_SOURCES= \
	kiwix-serve.cpp \
	../common/kiwix/reader.cpp \
	../common/kiwix/searcher.cpp \
	../common/kiwix/manager.cpp \
	../common/kiwix/library.cpp \
	../common/kiwix/xapianSearcher.cpp \
	../common/kiwix/cluceneSearcher.cpp \
	../common/base64.cpp \
	../common/pathTools.cpp \
	../common/regexTools.cpp \
	../common/resourceTools.cpp \
	../common/stringTools.cpp \
	../common/kiwix/reader.h \
	../common/kiwix/searcher.h \
	../common/kiwix/manager.h \
	../common/kiwix/library.h \
	../common/kiwix/xapianSearcher.h \
	../common/kiwix/cluceneSearcher.h \
	../common/base64.h \
	../common/pathTools.h \
	../common/regexTools.h \
	../common/resourceTools.h \
	../common/stringTools.h
kiwix_serve_CXXFLAGS=${MICROHTTPD_CFLAGS} -I${CURRENT_PATH}/src/ctpp2/include $(CLUCENE_CFLAGS) $(XAPIAN_CFLAGS) $(ICU_CFLAGS) $(LZMA_CFLAGS) $(ZLIB_CFLAGS) -I../common/

kiwix_serve_LDFLAGS=$(CHECK_LDFLAGS)

if STATIC_BINS
kiwix_serve_LDFLAGS+= $(XAPIAN_STATIC_LDFLAGS) $(ICU_STATIC_LDFLAGS) $(MICROHTTPD_STATIC_LDFLAGS) ../zimlib/src/.libs/libzim.a ../ctpp2/src/.libs/libctpp2.a ../pugixml/.libs/libpugixml.a $(LZMA_STATIC_LDFLAGS) $(CLUCENE_STATIC_LDFLAGS) $(UUID_STATIC_LDFLAGS) $(ZLIB_STATIC_LDFLAGS) $(STATIC_LIBM_ARCHIVE) $(STATIC_LIBSTDCPP_ARCHIVE) $(STATIC_LIBDL_ARCHIVE) $(STATIC_LIBGCC_ARCHIVE) $(STATIC_LIBPTHREAD_ARCHIVE) --static
else
kiwix_serve_LDFLAGS+= $(XAPIAN_LDFLAGS) $(ICU_LDFLAGS) $(MICROHTTPD_LDFLAGS) ../zimlib/src/.libs/libzim.a ../ctpp2/src/.libs/libctpp2.a ../pugixml/.libs/libpugixml.a $(LZMA_LDFLAGS) $(CLUCENE_LDFLAGS) -ldl $(UUID_LDFLAGS) $(ZLIB_LDFLAGS) -lpthread
endif

if IS_OSX
kiwix_serve_LDFLAGS+= /opt/local/lib/libiconv.a /opt/local/lib/libintl.a -framework IOKit -framework CoreFoundation
endif
