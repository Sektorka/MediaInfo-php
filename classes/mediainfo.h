#ifndef MEDIAINFO_H_INCLUDED
#define MEDIAINFO_H_INCLUDED

PHP_METHOD(MediaInfo, __construct);
PHP_METHOD(MediaInfo, Option);
PHP_METHOD(MediaInfo, OptionStatic);
PHP_METHOD(MediaInfo, Open);
PHP_METHOD(MediaInfo, OpenBufferInit);
PHP_METHOD(MediaInfo, OpenBufferContinue);
PHP_METHOD(MediaInfo, OpenBufferContinueGoToGet);
PHP_METHOD(MediaInfo, OpenBufferFinalize);
PHP_METHOD(MediaInfo, OpenNextPacket);
PHP_METHOD(MediaInfo, Inform);
PHP_METHOD(MediaInfo, Get);
PHP_METHOD(MediaInfo, IsReady);
PHP_METHOD(MediaInfo, StateGet);
PHP_METHOD(MediaInfo, CountGet);
PHP_METHOD(MediaInfo, Close);

ZEND_BEGIN_ARG_INFO_EX(MediaInfo_construct, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(MediaInfo__Option, 0, 0, 2)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(MediaInfo__OptionStatic, 0, 0, 2)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(MediaInfo__Open, 0, 0, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(MediaInfo__OpenBufferInit, 0, 0, 2)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(MediaInfo__OpenBufferContinue, 0, 0, 2)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(MediaInfo__OpenBufferContinueGoToGet, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(MediaInfo__OpenBufferFinalize, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(MediaInfo__OpenNextPacket, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(MediaInfo__Inform, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(MediaInfo__Get, 0, 0, 5)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(MediaInfo__IsReady, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(MediaInfo__StateGet, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(MediaInfo__CountGet, 0, 0, 2)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(MediaInfo__Close, 0, 0, 0)
ZEND_END_ARG_INFO()

extern zend_function_entry mediainfo_methods[];

zend_function_entry mediainfo_methods[] = {
    PHP_ME(MediaInfo, __construct, MediaInfo_construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR|ZEND_ACC_FINAL)
	PHP_ME(MediaInfo, Option, MediaInfo__Option, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(MediaInfo, OptionStatic, MediaInfo__OptionStatic, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC|ZEND_ACC_FINAL)
	PHP_ME(MediaInfo, Open, MediaInfo__Open, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(MediaInfo, OpenBufferInit, MediaInfo__OpenBufferInit, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(MediaInfo, OpenBufferContinue, MediaInfo__OpenBufferContinue, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(MediaInfo, OpenBufferContinueGoToGet, MediaInfo__OpenBufferContinueGoToGet, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(MediaInfo, OpenBufferFinalize, MediaInfo__OpenBufferFinalize, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(MediaInfo, OpenNextPacket, MediaInfo__OpenNextPacket, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(MediaInfo, Inform, MediaInfo__Inform, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(MediaInfo, Get, MediaInfo__Get, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(MediaInfo, IsReady, MediaInfo__IsReady, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(MediaInfo, StateGet, MediaInfo__StateGet, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(MediaInfo, CountGet, MediaInfo__CountGet, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(MediaInfo, Close, MediaInfo__Close, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	{NULL, NULL, NULL}
};

PHP_METHOD(MediaInfo, __construct){
    mediainfo_object *obj = (mediainfo_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
    obj->mediainfo = new MediaInfo();
}

PHP_METHOD(MediaInfo, Option){
    char *option, *value;
    int option_len = -1, value_len = -1;
    const char *retval;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|s", &option, &option_len, &value, &value_len) == FAILURE) {
        zend_error(E_COMPILE_ERROR, "MediaInfo::Option() must take exactly 1 argument (option name)");
    }

    mediainfo_object *obj = (mediainfo_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

    if(option_len != -1 && value_len != -1){
        retval = unicodeToAnsi(obj->mediainfo->Option(option,value).c_str());
        RETURN_STRINGL( retval, strlen(retval), 1 );
    }

    else if(option_len != -1){
        retval = unicodeToAnsi(obj->mediainfo->Option(option).c_str());
        RETURN_STRINGL( retval, strlen(retval), 1 );
    }

    else{
        RETURN_NULL();
    }
}

PHP_METHOD(MediaInfo, OptionStatic){
    char *option, *value;
    int option_len = -1, value_len = -1;
    const char *retval;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|s", &option, &option_len, &value, &value_len) == FAILURE) {
        zend_error(E_COMPILE_ERROR, "MediaInfo::Option() must take exactly 1 argument (option name)");
    }

    if(option_len != -1 && value_len != -1){
        retval = unicodeToAnsi(MediaInfo::Option_Static(option,value).c_str());
        RETURN_STRINGL( retval, strlen(retval), 1 );
    }

    else if(option_len != -1){
        retval = unicodeToAnsi(MediaInfo::Option_Static(option).c_str());
        RETURN_STRINGL( retval, strlen(retval), 1 );
    }

    else{
        RETURN_NULL();
    }
}

PHP_METHOD(MediaInfo, Open){
    char *filename, *resolved_path;
    int filename_len = -1;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "p", &filename, &filename_len) == FAILURE) {
        zend_error(E_COMPILE_ERROR, "MediaInfo::Open() must take exactly 1 argument (filename)");
    }

    if ((resolved_path = expand_filepath(filename, NULL TSRMLS_CC)) == NULL) {
        zend_error(E_WARNING, "MediaInfo: Failed to resolve file path from: \"%s\"",filename);
        RETURN_FALSE;
    }

    if( access( resolved_path, F_OK ) == -1 ) {
        zend_error(E_WARNING, "MediaInfo: Can't access resolved path \"%s\"",resolved_path);
        RETURN_FALSE;
    }

    mediainfo_object *obj = (mediainfo_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

    if(filename_len > 0){
        if(obj->mediainfo->Open(resolved_path)){
            RETURN_TRUE;
        }
        else{
            zend_error(E_WARNING, "MediaInfo: Failed to open file \"%s\"",resolved_path);
            RETURN_FALSE;
        }
    }
    else{
        zend_error(E_WARNING, "MediaInfo: Empty file name!",resolved_path);
        RETURN_FALSE;
    }
}

PHP_METHOD(MediaInfo, OpenBufferInit){
    double fileSize, fileOffset;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|dd", &fileSize, &fileOffset) == FAILURE) {
        zend_error(E_COMPILE_ERROR, "MediaInfo::OpenBufferInit() failed to get arguments!");
    }

    mediainfo_object *obj = (mediainfo_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

    RETURN_LONG(obj->mediainfo->Open_Buffer_Init((MediaInfo_int64u)fileSize, (MediaInfo_int64u)fileOffset));
}

PHP_METHOD(MediaInfo, OpenBufferContinue){
    MediaInfo_int8u* buffer;
    size_t bufferSize;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sl", &buffer, &bufferSize) == FAILURE) {
        zend_error(E_COMPILE_ERROR, "MediaInfo::OpenBufferContinue() must take exactly 2 argument (buffer, buffer size)!");
    }

    mediainfo_object *obj = (mediainfo_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

    RETURN_LONG(obj->mediainfo->Open_Buffer_Continue(buffer, bufferSize));
}

PHP_METHOD(MediaInfo, OpenBufferContinueGoToGet){
    mediainfo_object *obj = (mediainfo_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
    RETURN_DOUBLE(obj->mediainfo->Open_Buffer_Continue_GoTo_Get());
}

PHP_METHOD(MediaInfo, OpenBufferFinalize){
    mediainfo_object *obj = (mediainfo_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
    RETURN_LONG(obj->mediainfo->Open_Buffer_Finalize());
}

PHP_METHOD(MediaInfo, OpenNextPacket){
    mediainfo_object *obj = (mediainfo_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
    RETURN_LONG(obj->mediainfo->Open_NextPacket());
}

PHP_METHOD(MediaInfo, Inform){
    const char *retval;

    mediainfo_object *obj = (mediainfo_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
    retval = unicodeToAnsi(obj->mediainfo->Inform().c_str());

    RETURN_STRINGL( retval, strlen(retval), 1 );
}

PHP_METHOD(MediaInfo, Get){
    char *parameter;
    int streamKind, infoKind, searchKind,  parameter_len = -1;
    size_t streamNumber, iparameter = 0;
    const char *retval;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lls|ll", &streamKind, &streamNumber, &parameter, &parameter_len, &infoKind, &searchKind) == FAILURE) {
        zend_error(E_COMPILE_ERROR, "MediaInfo::Get() must take exactly 3 argument (Stream kind const, Stream number, Parameter text)");
    }

    mediainfo_object *obj = (mediainfo_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

    iparameter = atoi(parameter);
    if((iparameter > 0 || iparameter == 0 && strcmp(parameter,"0") == 0) && ZEND_NUM_ARGS() < 5){
        retval = unicodeToAnsi(obj->mediainfo->Get(getStreamByID(streamKind), streamNumber, iparameter, getInfoByID(infoKind)).c_str());
        RETURN_STRINGL( retval, strlen(retval), 1 );
    }
    else{
        retval = unicodeToAnsi(obj->mediainfo->Get(getStreamByID(streamKind), streamNumber, parameter, getInfoByID(infoKind), getInfoByID(searchKind)).c_str());
        RETURN_STRINGL( retval, strlen(retval), 1 );
    }
}

PHP_METHOD(MediaInfo, IsReady){
    mediainfo_object *obj = (mediainfo_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
    if(obj->mediainfo->IsReady()){
        RETURN_TRUE;
    }
    else{
        RETURN_FALSE;
    }
}

PHP_METHOD(MediaInfo, StateGet){
    mediainfo_object *obj = (mediainfo_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
    RETURN_LONG(obj->mediainfo->State_Get());
}

PHP_METHOD(MediaInfo, CountGet){
    int streamKind;
    size_t streamNumber = -1;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l|l", &streamKind, &streamNumber) == FAILURE) {
        zend_error(E_COMPILE_ERROR, "MediaInfo::CountGet() call with minimum 1 parameter!");
    }

    mediainfo_object *obj = (mediainfo_object *)zend_object_store_get_object(getThis() TSRMLS_CC);

    RETURN_LONG(obj->mediainfo->Count_Get(getStreamByID(streamKind), streamNumber));
}

PHP_METHOD(MediaInfo, Close){
    mediainfo_object *obj = (mediainfo_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
    obj->mediainfo->Close();
}

#endif
