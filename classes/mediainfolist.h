#ifndef MEDIAINFOLIST_H_INCLUDED
#define MEDIAINFOLIST_H_INCLUDED

PHP_METHOD(MediaInfoList, __construct);
PHP_METHOD(MediaInfoList, Option);
PHP_METHOD(MediaInfoList, OptionStatic);
PHP_METHOD(MediaInfoList, Open);
PHP_METHOD(MediaInfoList, Inform);
PHP_METHOD(MediaInfoList, Get);
PHP_METHOD(MediaInfoList, IsReady);
PHP_METHOD(MediaInfoList, StateGet);
PHP_METHOD(MediaInfoList, CountGet);
PHP_METHOD(MediaInfoList, Close);

ZEND_BEGIN_ARG_INFO_EX(MediaInfoList_construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(MediaInfoList__Option, 0, 0, 2)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(MediaInfoList__OptionStatic, 0, 0, 2)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(MediaInfoList__Open, 0, 0, 2)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(MediaInfoList__Inform, 0, 0, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(MediaInfoList__Get, 0, 0, 6)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(MediaInfoList__IsReady, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(MediaInfoList__StateGet, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(MediaInfoList__CountGet, 0, 0, 3)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(MediaInfoList__Close, 0, 0, 1)
ZEND_END_ARG_INFO()

extern zend_function_entry MediaInfoList_methods[];

zend_function_entry mediainfolist_methods[] = {
    PHP_ME(MediaInfoList, __construct, MediaInfoList_construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR|ZEND_ACC_FINAL)
	PHP_ME(MediaInfoList, Option, MediaInfoList__Option, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(MediaInfoList, OptionStatic, MediaInfoList__OptionStatic, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC|ZEND_ACC_FINAL)
	PHP_ME(MediaInfoList, Open, MediaInfoList__Open, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(MediaInfoList, Inform, MediaInfoList__Inform, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(MediaInfoList, Get, MediaInfoList__Get, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(MediaInfoList, IsReady, MediaInfoList__IsReady, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(MediaInfoList, StateGet, MediaInfoList__StateGet, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(MediaInfoList, CountGet, MediaInfoList__CountGet, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(MediaInfoList, Close, MediaInfoList__Close, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	{NULL, NULL, NULL}
};

PHP_METHOD(MediaInfoList, __construct){
    mediainfolist_object *obj = (mediainfolist_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
    obj->mediainfolist = new MediaInfoList();
}

PHP_METHOD(MediaInfoList, Option){
    char *option, *value;
    int option_len = -1, value_len = -1;
    const char *retval;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|s", &option, &option_len, &value, &value_len) == FAILURE) {
        zend_error(E_COMPILE_ERROR, "MediaInfoList::Option() must take exactly 1 argument (option name)");
    }

    mediainfolist_object *obj = (mediainfolist_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

    if(option_len != -1 && value_len != -1){
        retval = unicodeToAnsi(obj->mediainfolist->Option(option,value).c_str());
        RETURN_STRINGL( retval, strlen(retval), 1 );
    }

    else if(option_len != -1){
        retval = unicodeToAnsi(obj->mediainfolist->Option(option).c_str());
        RETURN_STRINGL( retval, strlen(retval), 1 );
    }

    else{
        RETURN_NULL();
    }
}

PHP_METHOD(MediaInfoList, OptionStatic){
    char *option, *value;
    int option_len = -1, value_len = -1;
    const char *retval;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|s", &option, &option_len, &value, &value_len) == FAILURE) {
        zend_error(E_COMPILE_ERROR, "MediaInfoList::Option() must take exactly 1 argument (option name)");
    }

    if(option_len != -1 && value_len != -1){
        retval = unicodeToAnsi(MediaInfoList::Option_Static(option,value).c_str());
        RETURN_STRINGL( retval, strlen(retval), 1 );
    }

    else if(option_len != -1){
        retval = unicodeToAnsi(MediaInfoList::Option_Static(option).c_str());
        RETURN_STRINGL( retval, strlen(retval), 1 );
    }

    else{
        RETURN_NULL();
    }
}

PHP_METHOD(MediaInfoList, Open){
    char *filename, *resolved_path;
    int filename_len = -1, fileoption = -1;
    bool opened;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "p|l", &filename, &filename_len, &fileoption) == FAILURE) {
        zend_error(E_COMPILE_ERROR, "MediaInfoList::Open() must take exactly 1 argument (filename)");
    }

    if ((resolved_path = expand_filepath(filename, NULL TSRMLS_CC)) == NULL) {
        zend_error(E_WARNING, "MediaInfo: Failed to resolve file path from: \"%s\"",filename);
        RETURN_FALSE;
    }

    if( access( resolved_path, F_OK ) == -1 ) {
        zend_error(E_WARNING, "MediaInfo: Can't access resolved path \"%s\"",resolved_path);
        RETURN_FALSE;
    }

    mediainfolist_object *obj = (mediainfolist_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

    if(filename_len > 0){
        if(fileoption == -1){
            opened = (bool)obj->mediainfolist->Open(resolved_path);
        }
        else{
            opened = (bool)obj->mediainfolist->Open(resolved_path, getFileOptionByID(fileoption));
        }

        if(opened){
            RETURN_TRUE;
        }
        else{
            zend_error(E_WARNING, "MediaInfoList: Failed to open file \"%s\"",resolved_path);
            RETURN_FALSE;
        }
    }
    else{
        zend_error(E_WARNING, "MediaInfoList: Empty file name!",resolved_path);
        RETURN_FALSE;
    }
}

PHP_METHOD(MediaInfoList, Inform){
    const char *retval;
    size_t filepos = -1;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|l", &filepos) == FAILURE) {
        zend_error(E_COMPILE_ERROR, "MediaInfoList::Inform() failed to get parameters!");
    }

    mediainfolist_object *obj = (mediainfolist_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

    retval = unicodeToAnsi(obj->mediainfolist->Inform(filepos).c_str());

    RETURN_STRINGL( retval, strlen(retval), 1 );
}

PHP_METHOD(MediaInfoList, Get){
    char *parameter;
    int streamKind, infoKind, searchKind, parameter_len = -1;
    const char *retval;
    size_t filepos = -1, streamNumber, iparameter = 0;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "llls|ll", &filepos, &streamKind, &streamNumber, &parameter, &parameter_len, &infoKind, &searchKind) == FAILURE) {
        zend_error(E_COMPILE_ERROR, "MediaInfoList::Get() must take exactly 3 argument (Stream kind const, Stream number, Parameter text)");
    }

    mediainfolist_object *obj = (mediainfolist_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

    iparameter = atoi(parameter);
    if((iparameter > 0 || iparameter == 0 && strcmp(parameter,"0") == 0) && ZEND_NUM_ARGS() < 6){
        retval = unicodeToAnsi(obj->mediainfolist->Get(filepos,getStreamByID(streamKind), streamNumber, iparameter, getInfoByID(infoKind)).c_str());
        RETURN_STRINGL( retval, strlen(retval), 1 );
    }
    else{
        retval = unicodeToAnsi(obj->mediainfolist->Get(filepos,getStreamByID(streamKind), streamNumber, parameter, getInfoByID(infoKind), getInfoByID(searchKind)).c_str());
        RETURN_STRINGL( retval, strlen(retval), 1 );
    }
}

PHP_METHOD(MediaInfoList, IsReady){
    mediainfolist_object *obj = (mediainfolist_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

    if(obj->mediainfolist->IsReady()){
        RETURN_TRUE;
    }
    else{
        RETURN_FALSE;
    }
}

PHP_METHOD(MediaInfoList, StateGet){
    mediainfolist_object *obj = (mediainfolist_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
    RETURN_LONG(obj->mediainfolist->State_Get());
}

PHP_METHOD(MediaInfoList, CountGet){
    int streamKind;
    size_t filepos, streamNumber = -1;

    mediainfolist_object *obj = (mediainfolist_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

    if(ZEND_NUM_ARGS() == 0){
        RETURN_LONG(obj->mediainfolist->Count_Get());
    }
    else if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll|l", &filepos, &streamKind, &streamNumber) == FAILURE) {
        zend_error(E_COMPILE_ERROR, "MediaInfoList::CountGet() call with minimum 2 parameters or call with 0 parameters!");
    }

    RETURN_LONG(obj->mediainfolist->Count_Get(filepos, getStreamByID(streamKind), streamNumber));
}

PHP_METHOD(MediaInfoList, Close){
    size_t filepos = -1;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|l", &filepos) == FAILURE) {
        zend_error(E_COMPILE_ERROR, "MediaInfoList::Close() failed to get parameters!");
    }

    mediainfolist_object *obj = (mediainfolist_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
    obj->mediainfolist->Close(filepos);
}

#endif
