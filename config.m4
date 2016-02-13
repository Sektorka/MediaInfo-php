dnl
dnl $Id$ 
dnl

PHP_ARG_ENABLE(mediainfo, mediainfo support,
[  --enable-mediainfo     Enable mediainfo])

if test "$PHP_MEDIAINFO" != "no"; then
	AC_DEFINE(HAVE_MEDIAINFO, 1, [mediainfo enabled]) 
	PHP_REQUIRE_CXX()
  
	PHP_NEW_EXTENSION(mediainfo, php_mediainfo.cpp, $ext_shared)
	PHP_ADD_BUILD_DIR($ext_builddir/src)
	PHP_ADD_MAKEFILE_FRAGMENT
fi
