
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

#include "kernel/array.h"
#include "kernel/fcall.h"
#include "kernel/operators.h"
#include "kernel/concat.h"
#include "kernel/exception.h"

/**
 * Phalcon\Tag\Select
 *
 * Generates a SELECT html tag using a static array of values or a Phalcon\Mvc\Model resultset
 */


/**
 * Phalcon\Tag\Select initializer
 */
PHALCON_INIT_CLASS(Phalcon_Tag_Select){

	PHALCON_REGISTER_CLASS(Phalcon\\Tag, Select, tag_select, phalcon_tag_select_method_entry, ZEND_ACC_EXPLICIT_ABSTRACT_CLASS);

	return SUCCESS;
}

/**
 * Generates a SELECT tag
 *
 * @param array $parameters
 * @param array $data
 */
PHP_METHOD(Phalcon_Tag_Select, selectField){

	zval *parameters, *data = NULL, *params = NULL, *eol = NULL, *id = NULL, *name, *value = NULL;
	zval *use_empty = NULL, *empty_value = NULL, *empty_text = NULL, *code;
	zval *avalue = NULL, *key = NULL, *close_option, *options = NULL, *using;
	zval *resultset_options, *array_options;
	HashTable *ah0;
	HashPosition hp0;
	zval **hd;

	PHALCON_MM_GROW();

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z|z", &parameters, &data) == FAILURE) {
		RETURN_MM_NULL();
	}

	if (!data) {
		PHALCON_INIT_VAR(data);
	}
	
	if (Z_TYPE_P(parameters) != IS_ARRAY) { 
		PHALCON_INIT_VAR(params);
		array_init_size(params, 2);
		phalcon_array_append(&params, parameters, PH_SEPARATE TSRMLS_CC);
		phalcon_array_append(&params, data, PH_SEPARATE TSRMLS_CC);
	} else {
		PHALCON_CPY_WRT(params, parameters);
	}
	
	PHALCON_INIT_VAR(eol);
	
	PHALCON_INIT_NVAR(eol);
	ZVAL_STRING(eol, PHP_EOL, 1);
	if (!phalcon_array_isset_long(params, 0)) {
		PHALCON_OBS_VAR(id);
		phalcon_array_fetch_string(&id, params, SL("id"), PH_NOISY_CC);
		phalcon_array_update_long(&params, 0, &id, PH_COPY | PH_SEPARATE TSRMLS_CC);
	}
	
	PHALCON_OBS_NVAR(id);
	phalcon_array_fetch_long(&id, params, 0, PH_NOISY_CC);
	if (!phalcon_array_isset_string(params, SS("name"))) {
		phalcon_array_update_string(&params, SL("name"), &id, PH_COPY | PH_SEPARATE TSRMLS_CC);
	} else {
		PHALCON_OBS_VAR(name);
		phalcon_array_fetch_string(&name, params, SL("name"), PH_NOISY_CC);
		if (!zend_is_true(name)) {
			phalcon_array_update_string(&params, SL("name"), &id, PH_COPY | PH_SEPARATE TSRMLS_CC);
		}
	}
	
	if (!phalcon_array_isset_string(params, SS("id"))) {
		phalcon_array_update_string(&params, SL("id"), &id, PH_COPY | PH_SEPARATE TSRMLS_CC);
	}
	
	if (!phalcon_array_isset_string(params, SS("value"))) {
		PHALCON_INIT_VAR(value);
		PHALCON_CALL_STATIC_PARAMS_2(value, "phalcon\\tag", "getvalue", id, params);
	} else {
		PHALCON_OBS_NVAR(value);
		phalcon_array_fetch_string(&value, params, SL("value"), PH_NOISY_CC);
		phalcon_array_unset_string(&params, SS("value"), PH_SEPARATE);
	}
	
	PHALCON_INIT_VAR(use_empty);
	ZVAL_BOOL(use_empty, 0);
	if (phalcon_array_isset_string(params, SS("useEmpty"))) {
		if (!phalcon_array_isset_string(params, SS("emptyValue"))) {
			PHALCON_INIT_VAR(empty_value);
			ZVAL_STRING(empty_value, "", 1);
		} else {
			PHALCON_OBS_NVAR(empty_value);
			phalcon_array_fetch_string(&empty_value, params, SL("emptyValue"), PH_NOISY_CC);
			phalcon_array_unset_string(&params, SS("emptyValue"), PH_SEPARATE);
		}
		if (!phalcon_array_isset_string(params, SS("emptyText"))) {
			PHALCON_INIT_VAR(empty_text);
			ZVAL_STRING(empty_text, "Choose...", 1);
		} else {
			PHALCON_OBS_NVAR(empty_text);
			phalcon_array_fetch_string(&empty_text, params, SL("emptyText"), PH_NOISY_CC);
			phalcon_array_unset_string(&params, SS("emptyText"), PH_SEPARATE);
		}
	
		PHALCON_OBS_NVAR(use_empty);
		phalcon_array_fetch_string(&use_empty, params, SL("useEmpty"), PH_NOISY_CC);
		phalcon_array_unset_string(&params, SS("useEmpty"), PH_SEPARATE);
	}
	
	PHALCON_INIT_VAR(code);
	ZVAL_STRING(code, "<select", 1);
	if (Z_TYPE_P(params) == IS_ARRAY) { 
	
		if (!phalcon_is_iterable(params, &ah0, &hp0, 0, 0 TSRMLS_CC)) {
			return;
		}
	
		while (zend_hash_get_current_data_ex(ah0, (void**) &hd, &hp0) == SUCCESS) {
	
			PHALCON_GET_FOREACH_KEY(key, ah0, hp0);
			PHALCON_GET_FOREACH_VALUE(avalue);
	
			if (Z_TYPE_P(key) != IS_LONG) {
				if (Z_TYPE_P(avalue) != IS_ARRAY) { 
					PHALCON_SCONCAT_SVSVS(code, " ", key, "=\"", avalue, "\"");
				}
			}
	
			zend_hash_move_forward_ex(ah0, &hp0);
		}
	
	}
	
	PHALCON_SCONCAT_SV(code, ">", eol);
	
	PHALCON_INIT_VAR(close_option);
	PHALCON_CONCAT_SV(close_option, "</option>", eol);
	if (zend_is_true(use_empty)) {
		/** 
		 * Create an empty value
		 */
		PHALCON_SCONCAT_SVSVV(code, "\t<option value=\"", empty_value, "\">", empty_text, close_option);
		phalcon_array_unset_string(&params, SS("useEmpty"), PH_SEPARATE);
	}
	
	if (phalcon_array_isset_long(params, 1)) {
		PHALCON_OBS_VAR(options);
		phalcon_array_fetch_long(&options, params, 1, PH_NOISY_CC);
	} else {
		PHALCON_CPY_WRT(options, data);
	}
	
	if (Z_TYPE_P(options) == IS_OBJECT) {
	
		/** 
		 * The options is a resultset
		 */
		if (!phalcon_array_isset_string(params, SS("using"))) {
			PHALCON_THROW_EXCEPTION_STR(phalcon_tag_exception_ce, "The 'using' parameter is required");
			return;
		} else {
			PHALCON_OBS_VAR(using);
			phalcon_array_fetch_string(&using, params, SL("using"), PH_NOISY_CC);
			if (Z_TYPE_P(using) != IS_ARRAY) { 
				PHALCON_THROW_EXCEPTION_STR(phalcon_tag_exception_ce, "The 'using' parameter should be an Array");
				return;
			}
		}
	
		/** 
		 * Create the SELECT's option from a resultset
		 */
		PHALCON_INIT_VAR(resultset_options);
		PHALCON_CALL_SELF_PARAMS_4(resultset_options, this_ptr, "_optionsfromresultset", options, using, value, close_option);
		phalcon_concat_self(&code, resultset_options TSRMLS_CC);
	} else {
		if (Z_TYPE_P(options) == IS_ARRAY) { 
			/** 
			 * Create the SELECT's option from an array
			 */
			PHALCON_INIT_VAR(array_options);
			PHALCON_CALL_SELF_PARAMS_3(array_options, this_ptr, "_optionsfromarray", options, value, close_option);
			phalcon_concat_self(&code, array_options TSRMLS_CC);
		} else {
			PHALCON_THROW_EXCEPTION_STR(phalcon_tag_exception_ce, "Invalid data provided to SELECT helper");
			return;
		}
	}
	
	phalcon_concat_self_str(&code, SL("</select>") TSRMLS_CC);
	
	RETURN_CTOR(code);
}

/**
 * Generate the OPTION tags based on a resulset
 *
 * @param Phalcon\Mvc\Model $resultset
 * @param array $using
 * @param mixed value
 * @param string $closeOption
 */
PHP_METHOD(Phalcon_Tag_Select, _optionsFromResultset){

	zval *resultset, *using, *value, *close_option;
	zval *code, *using_zero = NULL, *using_one = NULL, *option = NULL, *option_value = NULL;
	zval *option_text = NULL, *is_in_array = NULL;
	zval *r0 = NULL;

	PHALCON_MM_GROW();

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zzzz", &resultset, &using, &value, &close_option) == FAILURE) {
		RETURN_MM_NULL();
	}

	PHALCON_INIT_VAR(code);
	ZVAL_STRING(code, "", 1);
	PHALCON_CALL_METHOD_NORETURN(resultset, "rewind");
	
	while (1) {
	
		PHALCON_INIT_NVAR(r0);
		PHALCON_CALL_METHOD(r0, resultset, "valid");
		if (PHALCON_IS_NOT_FALSE(r0)) {
		} else {
			break;
		}
	
		PHALCON_OBS_NVAR(using_zero);
		phalcon_array_fetch_long(&using_zero, using, 0, PH_NOISY_CC);
	
		PHALCON_OBS_NVAR(using_one);
		phalcon_array_fetch_long(&using_one, using, 1, PH_NOISY_CC);
	
		/** 
		 * Get the current option
		 */
		PHALCON_INIT_NVAR(option);
		PHALCON_CALL_METHOD(option, resultset, "current");
	
		/** 
		 * Read the value attribute from the model
		 */
		PHALCON_INIT_NVAR(option_value);
		PHALCON_CALL_METHOD_PARAMS_1(option_value, option, "readattribute", using_zero);
	
		/** 
		 * Read the text attribute from the model
		 */
		PHALCON_INIT_NVAR(option_text);
		PHALCON_CALL_METHOD_PARAMS_1(option_text, option, "readattribute", using_one);
	
		/** 
		 * If the value is equal to the option's value we mark it as selected
		 */
		if (Z_TYPE_P(value) == IS_ARRAY) { 
	
			PHALCON_INIT_NVAR(is_in_array);
			PHALCON_CALL_FUNC_PARAMS_2(is_in_array, "in_array", option_value, value);
			if (zend_is_true(is_in_array)) {
				PHALCON_SCONCAT_SVSVV(code, "\t<option selected=\"selected\" value=\"", option_value, "\">", option_text, close_option);
			} else {
				PHALCON_SCONCAT_SVSVV(code, "\t<option value=\"", option_value, "\">", option_text, close_option);
			}
		} else {
			if (PHALCON_IS_EQUAL(option_value, value)) {
				PHALCON_SCONCAT_SVSVV(code, "\t<option selected=\"selected\" value=\"", option_value, "\">", option_text, close_option);
			} else {
				PHALCON_SCONCAT_SVSVV(code, "\t<option value=\"", option_value, "\">", option_text, close_option);
			}
		}
	
		PHALCON_CALL_METHOD_NORETURN(resultset, "next");
	}
	
	RETURN_CTOR(code);
}

/**
 * Generate the OPTION tags based on an array
 *
 * @param Phalcon\Mvc\ModelInterface $resultset
 * @param array $using
 * @param mixed value
 * @param string $closeOption
 */
PHP_METHOD(Phalcon_Tag_Select, _optionsFromArray){

	zval *data, *value, *close_option, *code, *option_text = NULL;
	zval *option_value = NULL, *is_in_array = NULL;
	HashTable *ah0;
	HashPosition hp0;
	zval **hd;

	PHALCON_MM_GROW();

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zzz", &data, &value, &close_option) == FAILURE) {
		RETURN_MM_NULL();
	}

	PHALCON_INIT_VAR(code);
	ZVAL_STRING(code, "", 1);
	
	if (!phalcon_is_iterable(data, &ah0, &hp0, 0, 0 TSRMLS_CC)) {
		return;
	}
	
	while (zend_hash_get_current_data_ex(ah0, (void**) &hd, &hp0) == SUCCESS) {
	
		PHALCON_GET_FOREACH_KEY(option_value, ah0, hp0);
		PHALCON_GET_FOREACH_VALUE(option_text);
	
		if (Z_TYPE_P(value) == IS_ARRAY) { 
	
			PHALCON_INIT_NVAR(is_in_array);
			PHALCON_CALL_FUNC_PARAMS_2(is_in_array, "in_array", option_value, value);
			if (zend_is_true(is_in_array)) {
				PHALCON_SCONCAT_SVSVV(code, "\t<option selected=\"selected\" value=\"", option_value, "\">", option_text, close_option);
			} else {
				PHALCON_SCONCAT_SVSVV(code, "\t<option value=\"", option_value, "\">", option_text, close_option);
			}
		} else {
			if (PHALCON_IS_EQUAL(option_value, value)) {
				PHALCON_SCONCAT_SVSVV(code, "\t<option selected=\"selected\" value=\"", option_value, "\">", option_text, close_option);
			} else {
				PHALCON_SCONCAT_SVSVV(code, "\t<option value=\"", option_value, "\">", option_text, close_option);
			}
		}
	
		zend_hash_move_forward_ex(ah0, &hp0);
	}
	
	
	RETURN_CTOR(code);
}

