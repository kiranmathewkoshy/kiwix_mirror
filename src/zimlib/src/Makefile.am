AM_CPPFLAGS=-I$(top_srcdir)/src/zimlib/include

noinst_LTLIBRARIES = libzim.la

libzim_la_SOURCES = \
	article.cpp \
	articlesearch.cpp \
	cluster.cpp \
	dirent.cpp \
	file.cpp \
	fileheader.cpp \
	fileimpl.cpp \
	indexarticle.cpp \
	ptrstream.cpp \
	search.cpp \
	template.cpp \
	unicode.cpp \
	uuid.cpp \
	zintstream.cpp \
	envvalue.cpp \
	lzmastream.cpp \
	unlzmastream.cpp \
	fstream.cpp \
	md5.cpp \
	md5stream.cpp \
	ptrstream.h \
	log.h \
	envvalue.h \
	md5stream.h \
	md5.h

libzim_la_CXXFLAGS=${LZMA_CFLAGS} -fPIC -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE
libzim_la_LDFLAGS=$(CHECK_LDFLAGS) ${LZMA_STATIC_LDFLAGS}

CLEANFILES=*.obj *.lib *.dll