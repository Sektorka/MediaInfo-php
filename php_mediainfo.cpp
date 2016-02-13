//by Sektor | sektor@sektor.hu

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php_mediainfo.h"
#include "zend_exceptions.h"

zend_module_entry mediainfo_module_entry = {
    #if ZEND_MODULE_API_NO >= 20010901
    STANDARD_MODULE_HEADER,
    #endif
    PHP_MEDIAINFO_EXTNAME,
    NULL,
    PHP_MINIT(mediainfo),
    NULL,
    NULL,
    NULL,
    PHP_MINFO(mediainfo),
    #if ZEND_MODULE_API_NO >= 20010901
    PHP_MEDIAINFO_VERSION,
    #endif
    STANDARD_MODULE_PROPERTIES
};

zend_class_entry *mediainfo_entry;
zend_class_entry *mediainfolist_entry;

zend_object_handlers mediainfo_object_handlers;
zend_object_handlers mediainfolist_object_handlers;

struct mediainfo_object {
    zend_object std;
    MediaInfoDLL::MediaInfo *mediainfo;
};

struct mediainfolist_object {
    zend_object std;
    MediaInfoDLL::MediaInfoList *mediainfolist;
};

zend_object_value mediainfo_create_handler(zend_class_entry *type TSRMLS_DC);
zend_object_value mediainfolist_create_handler(zend_class_entry *type TSRMLS_DC);

static stream_t getStreamByID(int id){
    switch(id){
        case 0:
            return Stream_General;
        case 1:
            return Stream_Video;
        case 2:
            return Stream_Audio;
        case 3:
            return Stream_Text;
        case 4:
            return Stream_Other;
        case 5:
            return Stream_Image;
        case 6:
            return Stream_Menu;
        case 7:
            return Stream_Max;
    }

    return Stream_General;
}

static info_t getInfoByID(int id){
    switch(id){
        case 0:
            return Info_Name;
        case 1:
            return Info_Text;
        case 2:
            return Info_Measure;
        case 3:
            return Info_Options;
        case 4:
            return Info_Name_Text;
        case 5:
            return Info_Measure_Text;
        case 6:
            return Info_Info;
        case 7:
            return Info_HowTo;
        case 8:
            return Info_Max;
    }

    return Info_Name;
}

static infooptions_t getInfoOptionByID(int id){
    switch(id){
        case 0:
            return InfoOption_ShowInInform;
        case 1:
            return InfoOption_Reserved;
        case 2:
            return InfoOption_ShowInSupported;
        case 3:
            return InfoOption_TypeOfValue;
        case 4:
            return InfoOption_Max;
    }

    return InfoOption_ShowInInform;
}

static fileoptions_t getFileOptionByID(int id){
    switch(id){
        case 0:
            return FileOption_Nothing;
        case 1:
            return FileOption_NoRecursive;
        case 2:
            return FileOption_CloseAll;
        case 4:
            return FileOption_Max;
    }

    return FileOption_Nothing;
}

static wchar_t* charToWChar(const char* text){
    size_t size = strlen(text) + 1;
    wchar_t* wa = new wchar_t[size];
    mbstowcs(wa,text,size);
    return wa;
}

static char* wCharToChar(const wchar_t* text){
    size_t size = wcslen(text) + 1;
    char* cha = new char[size];
    wcstombs(cha,text,size);
    return cha;
}

static char* unicodeToAnsi(const char * text){
    wchar_t * wtext = charToWChar(text);
    return wCharToChar(wtext);
}

#include "classes/mediainfo.h"
#include "classes/mediainfolist.h"
#include "ext/standard/info.h"

PHP_MINFO_FUNCTION(mediainfo)
{
	php_info_print_table_start();
	php_info_print_table_row(2, concat(PHP_MEDIAINFO_EXTNAME, " support"), "enabled");
	php_info_print_table_row(2, "Version",  PHP_MEDIAINFO_VERSION);
	php_info_print_table_row(2, "Library", unicodeToAnsi(MediaInfoDLL::MediaInfo::Option_Static("Info_Version").c_str()));
	php_info_print_table_row(2, "Author",   "Sektor");
	php_info_print_table_end();
}

PHP_MINIT_FUNCTION(mediainfo)
{
    zend_class_entry mediainfo_ce, mediainfolist_ce;

    //Mediainfo
    INIT_CLASS_ENTRY(mediainfo_ce, "MediaInfo", mediainfo_methods);

    mediainfo_entry = zend_register_internal_class(&mediainfo_ce TSRMLS_CC);
    mediainfo_entry->create_object = mediainfo_create_handler;

    memcpy(&mediainfo_object_handlers,
        zend_get_std_object_handlers(), sizeof(zend_object_handlers));
    mediainfo_object_handlers.clone_obj = NULL;

    //MediaInfoList
    INIT_CLASS_ENTRY(mediainfolist_ce, "MediaInfoList", mediainfolist_methods);

    mediainfolist_entry = zend_register_internal_class(&mediainfolist_ce TSRMLS_CC);
    mediainfolist_entry->create_object = mediainfolist_create_handler;

    memcpy(&mediainfolist_object_handlers,
        zend_get_std_object_handlers(), sizeof(zend_object_handlers));
    mediainfolist_object_handlers.clone_obj = NULL;

    #define REGISTER_MEDIAINFO_CLASS_CONST_LONG(const_name, value) \
	zend_declare_class_constant_long(mediainfo_entry, const_name, sizeof(const_name)-1, value TSRMLS_CC); \
	zend_declare_class_constant_long(mediainfolist_entry, const_name, sizeof(const_name)-1, value TSRMLS_CC);

    REGISTER_MEDIAINFO_CLASS_CONST_LONG("STREAM_GENERAL"                ,0);
    REGISTER_MEDIAINFO_CLASS_CONST_LONG("STREAM_VIDEO"                  ,1);
    REGISTER_MEDIAINFO_CLASS_CONST_LONG("STREAM_AUDIO"                  ,2);
    REGISTER_MEDIAINFO_CLASS_CONST_LONG("STREAM_TEXT"                   ,3);
    REGISTER_MEDIAINFO_CLASS_CONST_LONG("STREAM_OTHER"                  ,4);
    REGISTER_MEDIAINFO_CLASS_CONST_LONG("STREAM_IMAGE"                  ,5);
    REGISTER_MEDIAINFO_CLASS_CONST_LONG("STREAM_MENU"                   ,6);
    REGISTER_MEDIAINFO_CLASS_CONST_LONG("STREAM_MAX"                    ,7);

    REGISTER_MEDIAINFO_CLASS_CONST_LONG("INFO_NAME"                     ,0);
    REGISTER_MEDIAINFO_CLASS_CONST_LONG("INFO_TEXT"                     ,1);
    REGISTER_MEDIAINFO_CLASS_CONST_LONG("INFO_MEASURE"                  ,2);
    REGISTER_MEDIAINFO_CLASS_CONST_LONG("INFO_OPTIONS"                  ,3);
    REGISTER_MEDIAINFO_CLASS_CONST_LONG("INFO_NAME_TEXT"                ,4);
    REGISTER_MEDIAINFO_CLASS_CONST_LONG("INFO_MEASURE_TEXT"             ,5);
    REGISTER_MEDIAINFO_CLASS_CONST_LONG("INFO_INFO"                     ,6);
    REGISTER_MEDIAINFO_CLASS_CONST_LONG("INFO_HOWTO"                    ,7);
    REGISTER_MEDIAINFO_CLASS_CONST_LONG("INFO_MAX"                      ,8);

    REGISTER_MEDIAINFO_CLASS_CONST_LONG("INFO_OPTION_SHOW_IN_FORM"      ,0);
    REGISTER_MEDIAINFO_CLASS_CONST_LONG("INFO_OPTION_RESERVED"          ,1);
    REGISTER_MEDIAINFO_CLASS_CONST_LONG("INFO_OPTION_SHOW_IN_SUPPORTED" ,2);
    REGISTER_MEDIAINFO_CLASS_CONST_LONG("INFO_OPTION_TYPE_OF_VALUE"     ,3);
    REGISTER_MEDIAINFO_CLASS_CONST_LONG("INFO_OPTION_MAX"               ,4);

    REGISTER_MEDIAINFO_CLASS_CONST_LONG("FILE_OPTION_NOTHING"           ,0);
    REGISTER_MEDIAINFO_CLASS_CONST_LONG("FILE_OPTION_NORECURSIVE"       ,1);
    REGISTER_MEDIAINFO_CLASS_CONST_LONG("FILE_OPTION_CLOSEALL"          ,2);
    REGISTER_MEDIAINFO_CLASS_CONST_LONG("FILE_OPTION_MAX"               ,4);

    return SUCCESS;
}



//mediainfo destructor
void mediainfo_free_storage(void *object TSRMLS_DC)
{
    mediainfo_object *obj = (mediainfo_object *)object;
    delete obj->mediainfo;

    zend_hash_destroy(obj->std.properties);
    FREE_HASHTABLE(obj->std.properties);

    efree(obj);
}

//mediainfo constructor
zend_object_value mediainfo_create_handler(zend_class_entry *type TSRMLS_DC)
{
    zend_object_value retval;

    mediainfo_object *obj = (mediainfo_object *)emalloc(sizeof(mediainfo_object));
    memset(obj, 0, sizeof(mediainfo_object));
    obj->std.ce = type;

    ALLOC_HASHTABLE(obj->std.properties);
    zend_hash_init(obj->std.properties, 0, NULL, ZVAL_PTR_DTOR, 0);

#if PHP_VERSION_ID < 50400
		{
			zval *temp;

			zend_hash_copy(
				obj->std.properties,
				&type->default_properties,
				(copy_ctor_func_t) zval_add_ref,
				&temp, sizeof(zval*)
			);
		}
#else
		object_properties_init(&(obj->std), type);
#endif

    retval.handle = zend_objects_store_put(obj, NULL,
        mediainfo_free_storage, NULL TSRMLS_CC);
    retval.handlers = &mediainfo_object_handlers;

    return retval;
}

//mediainfolist destructor
void mediainfolist_free_storage(void *object TSRMLS_DC)
{
    mediainfolist_object *obj = (mediainfolist_object *)object;
    delete obj->mediainfolist;

    zend_hash_destroy(obj->std.properties);
    FREE_HASHTABLE(obj->std.properties);

    efree(obj);
}

//mediainfolist constructor
zend_object_value mediainfolist_create_handler(zend_class_entry *type TSRMLS_DC)
{
    zend_object_value retval;

    mediainfolist_object *obj = (mediainfolist_object *)emalloc(sizeof(mediainfolist_object));
    memset(obj, 0, sizeof(mediainfolist_object));
    obj->std.ce = type;

    ALLOC_HASHTABLE(obj->std.properties);
    zend_hash_init(obj->std.properties, 0, NULL, ZVAL_PTR_DTOR, 0);

#if PHP_VERSION_ID < 50400
    {
        zval *temp;

        zend_hash_copy(
            obj->std.properties,
            &type->default_properties,
            (copy_ctor_func_t) zval_add_ref,
            &temp, sizeof(zval*)
        );
    }
#else
		object_properties_init(&(obj->std), type);
#endif

    retval.handle = zend_objects_store_put(obj, NULL,
        mediainfolist_free_storage, NULL TSRMLS_CC);
    retval.handlers = &mediainfolist_object_handlers;

    return retval;
}

#ifdef COMPILE_DL_MEDIAINFO
extern "C" {
ZEND_GET_MODULE(mediainfo)
}
#endif
