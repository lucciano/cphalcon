
/*
  +------------------------------------------------------------------------+
  | Phalcon Framework                                                      |
  +------------------------------------------------------------------------+
  | Copyright (c) 2011-2013 Phalcon Team (http://www.phalconphp.com)       |
  +------------------------------------------------------------------------+
  | This source file is subject to the New BSD License that is bundled     |
  | with this package in the file docs/LICENSE.txt.                        |
  |                                                                        |
  | If you did not receive a copy of the license and are unable to         |
  | obtain it through the world-wide-web, please send an email             |
  | to license@phalconphp.com so we can send you a copy immediately.       |
  +------------------------------------------------------------------------+
  | Authors: Andres Gutierrez <andres@phalconphp.com>                      |
  |          Eduar Carvajal <eduar@phalconphp.com>                         |
  +------------------------------------------------------------------------+
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_phalcon.h"
#include "phalcon.h"

#include "Zend/zend_operators.h"
#include "Zend/zend_exceptions.h"
#include "Zend/zend_interfaces.h"

#include "kernel/main.h"
#include "kernel/memory.h"

#include "kernel/object.h"
#include "kernel/array.h"
#include "kernel/exception.h"
#include "kernel/fcall.h"
#include "kernel/operators.h"

/**
 * Phalcon\Paginator\Adapter\NativeArray
 *
 * Component of pagination by array data
 *
 */


/**
 * Phalcon\Paginator\Adapter\NativeArray initializer
 */
PHALCON_INIT_CLASS(Phalcon_Paginator_Adapter_NativeArray){

	PHALCON_REGISTER_CLASS(Phalcon\\Paginator\\Adapter, NativeArray, paginator_adapter_nativearray, phalcon_paginator_adapter_nativearray_method_entry, 0);

	zend_declare_property_null(phalcon_paginator_adapter_nativearray_ce, SL("_limitRows"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(phalcon_paginator_adapter_nativearray_ce, SL("_config"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(phalcon_paginator_adapter_nativearray_ce, SL("_page"), ZEND_ACC_PROTECTED TSRMLS_CC);

	zend_class_implements(phalcon_paginator_adapter_nativearray_ce TSRMLS_CC, 1, phalcon_paginator_adapterinterface_ce);

	return SUCCESS;
}

/**
 * Phalcon\Paginator\Adapter\NativeArray constructor
  *
 * @param array $config
 */
PHP_METHOD(Phalcon_Paginator_Adapter_NativeArray, __construct){

	zval *config, *limit, *page;

	PHALCON_MM_GROW();

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &config) == FAILURE) {
		RETURN_MM_NULL();
	}

	phalcon_update_property_zval(this_ptr, SL("_config"), config TSRMLS_CC);
	if (phalcon_array_isset_string(config, SS("limit"))) {
		PHALCON_OBS_VAR(limit);
		phalcon_array_fetch_string(&limit, config, SL("limit"), PH_NOISY_CC);
		phalcon_update_property_zval(this_ptr, SL("_limitRows"), limit TSRMLS_CC);
	}
	
	if (phalcon_array_isset_string(config, SS("page"))) {
		PHALCON_OBS_VAR(page);
		phalcon_array_fetch_string(&page, config, SL("page"), PH_NOISY_CC);
		phalcon_update_property_zval(this_ptr, SL("_page"), page TSRMLS_CC);
	}
	
	PHALCON_MM_RESTORE();
}

/**
 * Set the current page number
 *
 * @param int $page
 */
PHP_METHOD(Phalcon_Paginator_Adapter_NativeArray, setCurrentPage){

	zval *page;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &page) == FAILURE) {
		RETURN_NULL();
	}

	phalcon_update_property_zval(this_ptr, SL("_page"), page TSRMLS_CC);
	
}

/**
 * Returns a slice of the resultset to show in the pagination
 *
 * @return stdClass
 */
PHP_METHOD(Phalcon_Paginator_Adapter_NativeArray, getPaginate){

	zval *one, *show, *items, *page_number = NULL, *n, *page, *before_page_number;
	zval *start, *nn, *rounded_total, *total_pages = NULL;
	zval *div, *to_show, *compare = NULL, *next = NULL, *before = NULL, *res, *t_pages = NULL;
	zval *t0 = NULL;
	zval *r0 = NULL, *r1 = NULL;

	PHALCON_MM_GROW();

	/** 
	 * TODO: Rewrite the whole method!
	 */
	PHALCON_INIT_VAR(one);
	ZVAL_LONG(one, 1);
	
	PHALCON_OBS_VAR(show);
	phalcon_read_property(&show, this_ptr, SL("_limitRows"), PH_NOISY_CC);
	
	PHALCON_OBS_VAR(t0);
	phalcon_read_property(&t0, this_ptr, SL("_config"), PH_NOISY_CC);
	
	PHALCON_OBS_VAR(items);
	phalcon_array_fetch_string(&items, t0, SL("data"), PH_NOISY_CC);
	
	PHALCON_OBS_VAR(page_number);
	phalcon_read_property(&page_number, this_ptr, SL("_page"), PH_NOISY_CC);
	if (Z_TYPE_P(page_number) == IS_NULL) {
		PHALCON_CPY_WRT(page_number, one);
	}
	
	if (Z_TYPE_P(items) != IS_ARRAY) { 
		PHALCON_THROW_EXCEPTION_STR(phalcon_paginator_exception_ce, "Invalid data for paginator");
		return;
	}
	
	PHALCON_INIT_VAR(n);
	phalcon_fast_count(n, items TSRMLS_CC);
	
	PHALCON_INIT_VAR(page);
	object_init(page);
	
	PHALCON_INIT_VAR(before_page_number);
	sub_function(before_page_number, page_number, one TSRMLS_CC);
	
	PHALCON_INIT_VAR(start);
	mul_function(start, show, before_page_number TSRMLS_CC);
	
	PHALCON_INIT_VAR(nn);
	sub_function(nn, n, one TSRMLS_CC);
	
	PHALCON_INIT_VAR(rounded_total);
	div_function(rounded_total, nn, show TSRMLS_CC);
	
	PHALCON_INIT_VAR(total_pages);
	PHALCON_CALL_FUNC_PARAMS_1(total_pages, "round", rounded_total);
	
	PHALCON_INIT_VAR(r0);
	PHALCON_CALL_FUNC_PARAMS_3(r0, "array_slice", items, start, show);
	phalcon_update_property_zval(page, SL("items"), r0 TSRMLS_CC);
	phalcon_update_property_zval(page, SL("first"), one TSRMLS_CC);
	
	PHALCON_INIT_VAR(div);
	div_function(div, n, show TSRMLS_CC);
	
	PHALCON_INIT_VAR(to_show);
	phalcon_add_function(to_show, start, show TSRMLS_CC);
	
	PHALCON_INIT_VAR(compare);
	is_smaller_function(compare, to_show, n TSRMLS_CC);
	if (PHALCON_IS_TRUE(compare)) {
		PHALCON_INIT_VAR(next);
		phalcon_add_function(next, page_number, one TSRMLS_CC);
	} else {
		is_equal_function(compare, to_show, n TSRMLS_CC);
		if (PHALCON_IS_TRUE(compare)) {
			PHALCON_CPY_WRT(next, n);
		} else {
			if (PHALCON_IS_EQUAL(to_show, n)) {
				PHALCON_CPY_WRT(next, n);
			} else {
				PHALCON_INIT_NVAR(next);
				phalcon_add_function(next, div, one TSRMLS_CC);
			}
	
			PHALCON_INIT_VAR(r1);
			PHALCON_CALL_FUNC_PARAMS_1(r1, "intval", next);
			PHALCON_CPY_WRT(next, r1);
		}
	}
	
	is_smaller_function(compare, total_pages, next TSRMLS_CC);
	if (PHALCON_IS_TRUE(compare)) {
		PHALCON_CPY_WRT(next, total_pages);
	}
	
	phalcon_update_property_zval(page, SL("next"), next TSRMLS_CC);
	
	is_smaller_function(compare, one, page_number TSRMLS_CC);
	if (PHALCON_IS_TRUE(compare)) {
		PHALCON_INIT_VAR(before);
		sub_function(before, page_number, one TSRMLS_CC);
	} else {
		PHALCON_CPY_WRT(before, one);
	}
	
	phalcon_update_property_zval(page, SL("before"), before TSRMLS_CC);
	phalcon_update_property_zval(page, SL("current"), page_number TSRMLS_CC);
	
	PHALCON_INIT_VAR(res);
	mod_function(res, n, show TSRMLS_CC);
	if (zend_is_true(res)) {
		PHALCON_INIT_VAR(t_pages);
		phalcon_add_function(t_pages, div, one TSRMLS_CC);
	} else {
		PHALCON_CPY_WRT(t_pages, div);
	}
	
	PHALCON_INIT_NVAR(total_pages);
	PHALCON_CALL_FUNC_PARAMS_1(total_pages, "intval", t_pages);
	phalcon_update_property_zval(page, SL("last"), total_pages TSRMLS_CC);
	phalcon_update_property_zval(page, SL("total_pages"), total_pages TSRMLS_CC);
	
	RETURN_CTOR(page);
}

