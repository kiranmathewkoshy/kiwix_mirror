AM_CPPFLAGS=-I$(top_srcdir)/src/ctpp2/include -I$(top_srcdir)/src/ctpp2/include/functions

noinst_LTLIBRARIES = libctpp2.la

libctpp2_la_SOURCES = \
	CTPP2Logger.cpp \
	CTPP2VMExecutable.cpp \
	CTPP2JSONFileParser.cpp \
	CTPP2FileSourceLoader.cpp \
	CTPP2VMMemoryCore.cpp \
	CTPP2VMOpcodeCollector.cpp \
	CTPP2VMArgStack.cpp \
	CTPP2FileOutputCollector.cpp \
	CTPP2Compiler.cpp \
	CTPP2VMFileLoader.cpp \
	CTPP2BitIndex.cpp \
	CTPP2VMException.cpp \
	CTPP2StreamOutputCollector.cpp \
	CTPP2StringBuffer.cpp \
	CDTSortRoutines.cpp \
	CTPP2VMDebugInfo.cpp \
	CTPP2SyscallFactory.cpp \
	CTPP2HashTable.cpp \
	CTPP2Exception.cpp \
	CTPP2Sprintf.cpp \
	CTPP2StaticData.cpp \
	CDT.cpp \
	CTPP2VMSTDLib.cpp \
	CTPP2VMSyscall.cpp \
	CTPP2ParserException.cpp \
	CTPP2VM.cpp \
	CTPP2FileLogger.cpp \
	CTPP2Parser.cpp \
	CTPP2Error.cpp \
	CTPP2StaticText.cpp \
	CTPP2StringOutputCollector.cpp \
	CTPP2VMCodeStack.cpp \
	CTPP2TextLoader.cpp \
	functions/FnBase64Encode.cpp \
	functions/FnInSet.cpp \
	functions/FnSubstring.cpp \
	functions/FnGetText.cpp \
	functions/FnMin.cpp \
	functions/FnMBSubstring.cpp \
	functions/FnVersion.cpp \
	functions/FnListElement.cpp \
	functions/FnMax.cpp \
	functions/FnConcat.cpp \
	functions/FnSize.cpp \
	functions/FnMBSize.cpp \
	functions/FnBase64Decode.cpp \
	functions/FnFormParam.cpp \
	functions/FnXMLEscape.cpp \
	functions/FnWMLEscape.cpp \
	functions/FnHMACMD5.cpp \
	functions/FnDateFormat.cpp \
	functions/FnHTMLEscape.cpp \
	functions/FnTruncate.cpp \
	functions/FnURLEscape.cpp \
	functions/FnIconv.cpp \
	functions/FnURIEscape.cpp \
	functions/FnNumFormat.cpp \
	functions/FnMD5.cpp \
	functions/FnGetText_.cpp \
	functions/FnJSEscape.cpp \
	functions/FnDefined.cpp \
	functions/FnSprintf.cpp \
	functions/FnHrefParam.cpp \
	functions/FnCast.cpp \
	functions/FnJSON.cpp \
	functions/FnRandom.cpp \
	functions/FnLog.cpp \
	functions/FnObjDump.cpp \
	functions/FnMBTruncate.cpp \
	functions/FnDefault.cpp \
	functions/FnNumeral.cpp \
	functions/FnAvg.cpp \
	functions/FnEmitter.cpp \
	functions/FnError.cpp \
	functions/FnHashElement.cpp \
	functions/FnArrayElement.cpp \
	CTPP2VMDumper.cpp \
	CTPP2JSONParser.cpp \
	CTPP2DTOA.cpp \
	CTPP2Util.cpp

libctpp2_la_CXXFLAGS=-fPIC -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE

if IS_OSX
libctpp2_la_CXXFLAGS+= -I/opt/local/include
endif

libctpp2_la_LDFLAGS=$(CHECK_LDFLAGS)

CLEANFILES=*.obj *.lib *.dll