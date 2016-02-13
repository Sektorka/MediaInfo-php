//by Sektor | sektor@sektor.hu

#ifndef PHP_MEDIAINFO_H_INCLUDED
#define PHP_MEDIAINFO_H_INCLUDED

#define PHP_MEDIAINFO_EXTNAME "MediaInfo"
#define PHP_MEDIAINFO_VERSION "1.0.2"

#define concat(strFirst, strSecond) strFirst strSecond
extern "C" {
#include "php.h"
}

//#ifdef PHP_WIN32
#include "src/MediaInfoDLL.h"
//#else
//#include <MediaInfoDLL/MediaInfoDLL.h>
//#endif
#include "fopen_wrappers.h"

using namespace MediaInfoDLL;

#ifdef ZTS
#include "TSRM.h"
#endif

PHP_MINIT_FUNCTION(mediainfo);
PHP_MSHUTDOWN_FUNCTION(mediainfo);
PHP_MINFO_FUNCTION(mediainfo);

static stream_t getStreamByID(int id);
static info_t getInfoByID(int id);
static infooptions_t getInfoOptionByID(int id);
static fileoptions_t getFileOptionByID(int id);
static wchar_t* charToWChar(const char* text);
static char* wCharToChar(const wchar_t* text);
static char* unicodeToAnsi(const char * text);

#ifdef PHP_WIN32
#define MEDIAINFO_API __declspec(dllexport)
#else
#define MEDIAINFO_API
#endif

extern zend_module_entry mediainfo_module_entry;
#define phpext_mediainfo__ptr &mediainfo_module_entry

#endif // PHP_MEDIAINFO_H_INCLUDED
