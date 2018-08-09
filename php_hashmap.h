/*
  +----------------------------------------------------------------------+
  | PHP Version 7                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2018 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: eastwood<boss@haowei.me>                                     |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include <ext/standard/info.h>

#ifndef PHP_HASHMAP_H
#define PHP_HASHMAP_H

extern zend_module_entry hashmap_module_entry;
#define phpext_hashmap_ptr &hashmap_module_entry

#define PHP_HASHMAP_VERSION "0.1.0"

#ifdef PHP_WIN32
#	define PHP_HASHMAP_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_HASHMAP_API __attribute__ ((visibility("default")))
#else
#	define PHP_HASHMAP_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif


#define HASHMAP_G(v) ZEND_MODULE_GLOBALS_ACCESSOR(hashmap, v)

#if defined(ZTS) && defined(COMPILE_DL_HASHMAP)
ZEND_TSRMLS_CACHE_EXTERN()
#endif

#endif

#define HASHMAP_CLASS_PROPERTY      "list"
#define HASHMAP_CLASS_NAME          "EastWood\\Helper\\HashMap"
#define HASHMAP_CLASS_ALIAS_NAME    "HashMap"

PHP_METHOD(hashmap, __construct);
PHP_METHOD(hashmap, put);
PHP_METHOD(hashmap, get);
PHP_METHOD(hashmap, exists);
PHP_METHOD(hashmap, remove);


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
