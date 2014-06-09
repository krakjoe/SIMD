/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) Joe Watkins 2014                                       |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:  krakjoe@php.net                                             |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_simd.h"
#include "zend_interfaces.h"
#include <x86intrin.h>

zend_class_entry *php_float32x4_ce;
zend_object_handlers php_float32x4_handlers;

#define php_float32x4_fetch_ex(o)    ((php_float32x4_t*) zend_object_store_get_object(o TSRMLS_CC))
#define php_float32x4_fetch()        php_float32x4_fetch_ex(getThis())
#define php_float32x4_empty          {0.0, 0.0, 0.0, 0.0}

#define php_float32x4_method(n)		PHP_METHOD(Float32x4, n) {	\
	zval *zo; \
	php_float32x4_t *op1, *op2, *result; \
	\
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "O", &zo, php_float32x4_ce) != SUCCESS) { \
		return; \
	} \
	\
	op1 = php_float32x4_fetch(); \
	op2 = php_float32x4_fetch_ex(zo); \
	\
	object_init_ex(return_value, php_float32x4_ce); \
	\
	result = php_float32x4_fetch_ex(return_value); \
	\
	posix_memalign((void**) &result->v, 16, sizeof(__m128)); \
	\
	*result->v = _mm_##n##_ps(*op1->v, *op2->v); \
}

#define php_float32x4_operator(returns, result, v1, v2, n) \
	object_init_ex(returns, php_float32x4_ce); \
	\
	result = php_float32x4_fetch_ex(returns); \
	\
	posix_memalign((void**) &result->v, 16, sizeof(__m128)); \
	\
	*result->v = _mm_##n##_ps(*v1->v, *v2->v); \
	\
	return SUCCESS;

typedef struct _php_float32x4_t {
	zend_object std;
	zend_object_handle h;
	__m128 *v;
} php_float32x4_t;

ZEND_BEGIN_ARG_INFO_EX(php_float32x4_construct_arginfo, 0, 0, 4)
	ZEND_ARG_INFO(0, x)
	ZEND_ARG_INFO(0, y)
	ZEND_ARG_INFO(0, z)
	ZEND_ARG_INFO(0, w)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(php_float32x4_op_arginfo, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, op, Float32x4, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(php_float32x4_offsetGet_arginfo, 0, 0, 1)
	ZEND_ARG_INFO(0, offset)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(php_float32x4_offsetSet_arginfo, 0, 0, 1)
	ZEND_ARG_INFO(0, offset)
	ZEND_ARG_INFO(0, data)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(php_float32x4_offsetUnset_arginfo, 0, 0, 1)
	ZEND_ARG_INFO(0, offset)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(php_float32x4_offsetExists_arginfo, 0, 0, 1)
	ZEND_ARG_INFO(0, offset)
ZEND_END_ARG_INFO()

PHP_METHOD(Float32x4, __construct) {
	double lanes[4] = php_float32x4_empty;
	float  flanes[4] = php_float32x4_empty;
	php_float32x4_t *p = php_float32x4_fetch();
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dddd", &lanes[0], &lanes[1], &lanes[2], &lanes[3]) != SUCCESS) {
		return;
	}
	
	flanes[0] = (float) lanes[0];
	flanes[1] = (float) lanes[1];
	flanes[2] = (float) lanes[2];
	flanes[3] = (float) lanes[3];
	
	posix_memalign(
		(void**) &p->v, 16, sizeof(__m128));
	
	*p->v = _mm_load_ps (flanes);
}

php_float32x4_method(add)
php_float32x4_method(sub)
php_float32x4_method(div)
php_float32x4_method(mul)
php_float32x4_method(min)
php_float32x4_method(max)
php_float32x4_method(and)
php_float32x4_method(andnot)
php_float32x4_method(or)
php_float32x4_method(xor)

PHP_METHOD(Float32x4, offsetGet)    {
	long offset = -1;
	php_float32x4_t *p = php_float32x4_fetch();
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &offset) != SUCCESS) {
		return;
	}
	
	/* check boundaries and return double */
	if (offset < 4 && offset > -1) {
		RETURN_DOUBLE((*p->v)[offset]);
	}
	
	RETURN_DOUBLE(-1);
}

/* none of these are legal, yet */
PHP_METHOD(Float32x4, offsetSet)    {}
PHP_METHOD(Float32x4, offsetExists) {}
PHP_METHOD(Float32x4, offsetUnset)  {}

zend_function_entry php_float32x4_methods[] = {
	PHP_ME(Float32x4, __construct,         php_float32x4_construct_arginfo, ZEND_ACC_PUBLIC)
	PHP_ME(Float32x4, add,                 php_float32x4_op_arginfo, ZEND_ACC_PUBLIC)
	PHP_ME(Float32x4, sub,                 php_float32x4_op_arginfo, ZEND_ACC_PUBLIC)
	PHP_ME(Float32x4, mul,                 php_float32x4_op_arginfo, ZEND_ACC_PUBLIC)
	PHP_ME(Float32x4, div,                 php_float32x4_op_arginfo, ZEND_ACC_PUBLIC)
	PHP_ME(Float32x4, min,                 php_float32x4_op_arginfo, ZEND_ACC_PUBLIC)
	PHP_ME(Float32x4, max,                 php_float32x4_op_arginfo, ZEND_ACC_PUBLIC)
	PHP_ME(Float32x4, and,                 php_float32x4_op_arginfo, ZEND_ACC_PUBLIC)
	PHP_ME(Float32x4, andnot,              php_float32x4_op_arginfo, ZEND_ACC_PUBLIC)
	PHP_ME(Float32x4, or,                  php_float32x4_op_arginfo, ZEND_ACC_PUBLIC)
	PHP_ME(Float32x4, xor,                 php_float32x4_op_arginfo, ZEND_ACC_PUBLIC)
	PHP_ME(Float32x4, offsetGet,           php_float32x4_offsetGet_arginfo, ZEND_ACC_PUBLIC)
	PHP_ME(Float32x4, offsetSet,           php_float32x4_offsetSet_arginfo, ZEND_ACC_PUBLIC)
	PHP_ME(Float32x4, offsetUnset,         php_float32x4_offsetUnset_arginfo, ZEND_ACC_PUBLIC)
	PHP_ME(Float32x4, offsetExists,        php_float32x4_offsetExists_arginfo, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

static inline void php_float32x4_destroy(void *zobject, zend_object_handle handle TSRMLS_DC) {
	zend_objects_destroy_object(zobject, handle TSRMLS_CC);
}

static inline void php_float32x4_free(void *zobject TSRMLS_DC) {
	php_float32x4_t *p = 
		(php_float32x4_t *) zobject;

	zend_object_std_dtor(&p->std TSRMLS_CC);

	free(p->v);
	efree(p);
}

static inline zend_object_value php_float32x4_create(zend_class_entry *ce TSRMLS_DC) {
	zend_object_value value;
	php_float32x4_t *p = 
		(php_float32x4_t*) ecalloc(1, sizeof(php_float32x4_t));
	
	zend_object_std_init(&p->std, ce TSRMLS_CC);
	object_properties_init(&p->std, ce);

	p->h = zend_objects_store_put(
		p, 
		php_float32x4_destroy, 
		php_float32x4_free, NULL TSRMLS_CC);
	
	value.handle   = p->h;
	value.handlers = &php_float32x4_handlers;
	
	return value;
}

static HashTable *php_float32x4_dump(zval *obj, int *is_temp TSRMLS_DC) /* {{{ */
{
	php_float32x4_t *p = php_float32x4_fetch_ex(obj);
	zval zv;

	*is_temp = 1;
	
	array_init(&zv);
	add_next_index_double(&zv, (*p->v)[0]);
	add_next_index_double(&zv, (*p->v)[1]);
	add_next_index_double(&zv, (*p->v)[2]);
	add_next_index_double(&zv, (*p->v)[3]);
	return Z_ARRVAL(zv);
}
/* }}} */

static int php_float32x4_operation(zend_uchar op, zval *returns, zval *op1, zval *op2 TSRMLS_DC) /* {{{ */
{
	php_float32x4_t *p1 = php_float32x4_fetch_ex(op1),
					*p2 = php_float32x4_fetch_ex(op2),
					*result  = NULL;
					
	if (!p1 || !p2) {
		return FAILURE;
	}
	
	switch (op)
	{
		case ZEND_ADD:
			php_float32x4_operator(returns, result, p1, p2, add);
		case ZEND_SUB:
			php_float32x4_operator(returns, result, p1, p2, sub);
		case ZEND_MUL:
			php_float32x4_operator(returns, result, p1, p2, mul);
		case ZEND_DIV:
			php_float32x4_operator(returns, result, p1, p2, div);
		case ZEND_BW_OR:
			php_float32x4_operator(returns, result, p1, p2, or);
		case ZEND_BW_AND:
			php_float32x4_operator(returns, result, p1, p2, and);
		case ZEND_BW_XOR:
			php_float32x4_operator(returns, result, p1, p2, xor);
		case ZEND_BW_NOT:
			php_float32x4_operator(returns, result, p1, p2, andnot);
		
		default:
			return FAILURE;
	}
}
/* }}} */

static zval *php_float32x4_read( zval *object, zval *member, int type, const struct _zend_literal *key TSRMLS_DC ) /* {{{ */
{
	php_float32x4_t *p = php_float32x4_fetch_ex(object);
	zval *property = &EG(uninitialized_zval);
	
	if (!member || Z_TYPE_P(member) != IS_STRING || !Z_STRLEN_P(member)) {
		return property;
	}
		
	switch (Z_STRVAL_P(member)[0]) {
		case 'x':
			ALLOC_INIT_ZVAL(property);
			ZVAL_DOUBLE(property, (*p->v)[0]);
		break;
		
		case 'y':
			ALLOC_INIT_ZVAL(property);
			ZVAL_DOUBLE(property, (*p->v)[1]);
		break;
		
		case 'z':
			ALLOC_INIT_ZVAL(property);
			ZVAL_DOUBLE(property, (*p->v)[2]);
		break;
		
		case 'w':
			ALLOC_INIT_ZVAL(property);
			ZVAL_DOUBLE(property, (*p->v)[3]);
		break;
	}
	
	Z_SET_REFCOUNT_P(property, 0);
	
	return property;
} /* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(simd)
{
	zend_class_entry ce;
	
	INIT_CLASS_ENTRY(ce, "Float32x4", php_float32x4_methods);
	php_float32x4_ce = zend_register_internal_class(&ce TSRMLS_CC);
	php_float32x4_ce->create_object = php_float32x4_create;
	zend_class_implements(php_float32x4_ce TSRMLS_CC, 1, zend_ce_arrayaccess);

	memcpy(
		&php_float32x4_handlers,
		zend_get_std_object_handlers(), 
		sizeof(php_float32x4_handlers));
	
	php_float32x4_handlers.do_operation     = php_float32x4_operation;
	php_float32x4_handlers.get_debug_info   = php_float32x4_dump;
	php_float32x4_handlers.read_property    = php_float32x4_read;

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(simd)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "SIMD support", "enabled");
	php_info_print_table_end();
}
/* }}} */

/* {{{ simd_module_entry
 */
zend_module_entry simd_module_entry = {
	STANDARD_MODULE_HEADER,
	"simd",
	NULL,
	PHP_MINIT(simd),
	NULL,
	NULL,
	NULL,
	PHP_MINFO(simd),
	PHP_SIMD_VERSION,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_SIMD
ZEND_GET_MODULE(simd)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
