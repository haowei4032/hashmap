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

#include "php_hashmap.h"

static zend_class_entry *hashmap_ce, ce;
static zend_function_entry hashmap_functions[] = {
	PHP_FE_END
};

static zend_function_entry hashmap_methods[] = {
	PHP_ME(hashmap, __construct, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(hashmap, put, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(hashmap, get, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(hashmap, exists, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(hashmap, remove, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

static zend_bool hashmap_key_exists(INTERNAL_FUNCTION_PARAMETERS)
{
	zend_string *key;
	zval *property;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "S", &key) == FAILURE) {
		RETURN_FALSE;
	}

	//php_printf("%s\n", ZSTR_VAL(key));
	property = zend_read_property(hashmap_ce, getThis(), ZEND_STRL(HASHMAP_CLASS_PROPERTY), 1, NULL);
	RETURN_BOOL(zend_hash_exists(Z_ARRVAL_P(property), key));
}

PHP_METHOD(hashmap, __construct)
{
	zval init;
	array_init(&init);
	zend_update_property(hashmap_ce, getThis(), ZEND_STRL(HASHMAP_CLASS_PROPERTY), &init);
	zval_ptr_dtor(&init);
}

PHP_METHOD(hashmap, put)
{
	zend_string *key;
	zval *property, *value;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "Sz", &key, &value) == FAILURE) {
		RETURN_FALSE;
	}

	property = zend_read_property(hashmap_ce, getThis(), ZEND_STRL(HASHMAP_CLASS_PROPERTY), 1, NULL);
	if (zend_hash_update(Z_ARRVAL_P(property), key, value) != NULL) {
		Z_TRY_ADDREF_P(value);
		RETURN_TRUE;
	}
	RETURN_FALSE;
}

PHP_METHOD(hashmap, get)
{
	zend_string *key;
	zval *property, value;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "S", &key) == FAILURE) {
		RETURN_FALSE;
	}
	
	property = zend_read_property(hashmap_ce, getThis(), ZEND_STRL(HASHMAP_CLASS_PROPERTY), 1, NULL);
	if (!hashmap_key_exists(INTERNAL_FUNCTION_PARAM_PASSTHRU)) RETURN_NULL();
	
	RETURN_ZVAL(zend_hash_find(Z_ARRVAL_P(property), key), 1, 0);
}

PHP_METHOD(hashmap, exists)
{
	hashmap_key_exists(INTERNAL_FUNCTION_PARAM_PASSTHRU);
}

PHP_METHOD(hashmap, remove)
{
	zend_string *key;
	zval *property, value;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "S", &key) == FAILURE) {
		RETURN_FALSE;
	}
	if (!hashmap_key_exists(INTERNAL_FUNCTION_PARAM_PASSTHRU)) RETURN_FALSE;
	property = zend_read_property(hashmap_ce, getThis(), ZEND_STRL(HASHMAP_CLASS_PROPERTY), 1, NULL);
	RETURN_BOOL(zend_hash_del(Z_ARRVAL_P(property), key) == SUCCESS ? 1 : 0);
}

PHP_MINIT_FUNCTION(hashmap)
{
	INIT_CLASS_ENTRY(ce, HASHMAP_CLASS_NAME, hashmap_methods);
	hashmap_ce = zend_register_internal_class(&ce TSRMLS_CC);
	zend_register_class_alias(HASHMAP_CLASS_ALIAS_NAME, hashmap_ce);
	zend_declare_property_null(hashmap_ce, ZEND_STRL(HASHMAP_CLASS_PROPERTY), ZEND_ACC_PRIVATE);
	return SUCCESS;
}


PHP_MSHUTDOWN_FUNCTION(hashmap)
{
	return SUCCESS;
}

PHP_RINIT_FUNCTION(hashmap)
{
#if defined(COMPILE_DL_HASHMAP) && defined(ZTS)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif
	return SUCCESS;
}

PHP_RSHUTDOWN_FUNCTION(hashmap)
{
	return SUCCESS;
}

PHP_MINFO_FUNCTION(hashmap)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "hashmap support", "enabled");
	php_info_print_table_row(2, "hashmap version", PHP_HASHMAP_VERSION);
	php_info_print_table_row(2, "hashmap author", "eastwood<boss@haowei.me>");
	php_info_print_table_end();
}

zend_module_entry hashmap_module_entry = {
	STANDARD_MODULE_HEADER,
	"hashmap",
	hashmap_functions,
	PHP_MINIT(hashmap),
	PHP_MSHUTDOWN(hashmap),
	PHP_RINIT(hashmap),	
	PHP_RSHUTDOWN(hashmap),
	PHP_MINFO(hashmap),
	PHP_HASHMAP_VERSION,
	STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_HASHMAP
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif
ZEND_GET_MODULE(hashmap)
#endif


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
