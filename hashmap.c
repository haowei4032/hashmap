#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_hashmap.h"

#define HASHMAP_PROPERTY "list"
#define HASHMAP_EXISTS_METHOD "exists"

PHP_METHOD(hashmap, __construct);
PHP_METHOD(hashmap, put);
PHP_METHOD(hashmap, get);
PHP_METHOD(hashmap, exists);
PHP_METHOD(hashmap, remove);

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

PHP_METHOD(hashmap, __construct)
{
	zval init;
	array_init(&init);
	zend_update_property(hashmap_ce, getThis(), ZEND_STRL(HASHMAP_PROPERTY), &init);
	zval_ptr_dtor(&init);
}

PHP_METHOD(hashmap, put)
{
	zend_string *key;
	zval *property, *value;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "Sz", &key, &value) == FAILURE) {
		RETURN_FALSE;
	}

	property = zend_read_property(hashmap_ce, getThis(), ZEND_STRL(HASHMAP_PROPERTY), 1, NULL);
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
	
	property = zend_read_property(hashmap_ce, getThis(), ZEND_STRL(HASHMAP_PROPERTY), 1, NULL);
	HashTable *ht = Z_ARRVAL_P(property);

	zval param[1];
	ZVAL_STRING(&param[0], ZSTR_VAL(key));
	zval function;
	ZVAL_STRING(&function, HASHMAP_EXISTS_METHOD);
	call_user_function(NULL, getThis(), &function, &value, 1, param TSRMLS_CC);
	//int status = call_user_function(NULL, getThis(), &function, &value, 1, param TSRMLS_CC);
	//php_printf("status: %d; success: %d; failure: %d\n", status, SUCCESS, FAILURE);
	//php_printf("value: %d\n", Z_TYPE_P(&value) == IS_TRUE ? 1 : 0);
	zval_ptr_dtor(param);
	if (Z_TYPE_P(&value) == IS_FALSE) RETURN_NULL();

	RETURN_ZVAL(zend_hash_find(ht, key), 1, 0);
}

PHP_METHOD(hashmap, exists)
{
	zend_string *key;
	zval *property;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "S", &key) == FAILURE) {
		RETURN_FALSE;
	}

	property = zend_read_property(hashmap_ce, getThis(), ZEND_STRL(HASHMAP_PROPERTY), 1, NULL);
	HashTable *ht = Z_ARRVAL_P(property);
	RETURN_BOOL(zend_hash_exists(ht, key));
}

PHP_METHOD(hashmap, remove)
{
	zend_string *key;
	zval *property, value;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "S", &key) == FAILURE) {
		RETURN_FALSE;
	}

	zval param[1];
	ZVAL_STRING(&param[0], ZSTR_VAL(key));
	zval function;
	ZVAL_STRING(&function, HASHMAP_EXISTS_METHOD);
	call_user_function(NULL, getThis(), &function, &value, 1, param TSRMLS_CC);
	zval_ptr_dtor(param);
	if (Z_TYPE_P(&value) == IS_FALSE) RETURN_FALSE;

	property = zend_read_property(hashmap_ce, getThis(), ZEND_STRL(HASHMAP_PROPERTY), 1, NULL);
	HashTable *ht = Z_ARRVAL_P(property);
	RETURN_BOOL(zend_hash_del(ht, key) == SUCCESS ? 1 : 0);
}

PHP_MINIT_FUNCTION(hashmap)
{
	INIT_CLASS_ENTRY(ce, "HashMap", hashmap_methods);
	hashmap_ce = zend_register_internal_class(&ce TSRMLS_CC);
	zend_declare_property_null(hashmap_ce, ZEND_STRL(HASHMAP_PROPERTY), ZEND_ACC_PRIVATE);
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

