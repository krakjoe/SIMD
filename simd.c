/*
  +----------------------------------------------------------------------+
  | PHP Version 7                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) Joe Watkins 2015                                       |
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

typedef struct _php_float32x4_t {
	__m128 *v;
	zend_object std;
} php_float32x4_t;

#define php_float32x4_from(o)		 (php_float32x4_t*) ((char*)o - XtOffsetOf(php_float32x4_t, std))
#define php_float32x4_fetch_ex(o)    php_float32x4_from(Z_OBJ_P(o))
#define php_float32x4_fetch()        php_float32x4_fetch_ex(getThis())
#define php_float32x4_empty          {0.0, 0.0, 0.0, 0.0}

#define php_float32x4_method(n)		PHP_METHOD(Float32x4, n) {	\
	zval *zo; \
	php_float32x4_t *op1, *op2, *result; \
	\
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "O", &zo, php_float32x4_ce) != SUCCESS) { \
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

ZEND_BEGIN_ARG_INFO_EX(php_float32x4_construct_arginfo, 0, 0, 4)
	ZEND_ARG_TYPE_INFO(0, x, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, y, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, z, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, w, IS_DOUBLE, 0)
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
	
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "dddd", &lanes[0], &lanes[1], &lanes[2], &lanes[3]) != SUCCESS) {
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
	
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "l", &offset) != SUCCESS) {
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

static inline void php_float32x4_free(zend_object *zobject) {
	php_float32x4_t *p = php_float32x4_from(zobject);

	zend_object_std_dtor(&p->std);

	free(p->v);
}

static inline zend_object* php_float32x4_create(zend_class_entry *ce) {
	php_float32x4_t *p = 
		(php_float32x4_t*) ecalloc(1, sizeof(php_float32x4_t) + zend_object_properties_size(ce));
	
	zend_object_std_init(&p->std, ce);
	object_properties_init(&p->std, ce);

	p->std.handlers = &php_float32x4_handlers;
	
	return &p->std;
}

static HashTable *php_float32x4_dump(zval *obj, int *is_temp) /* {{{ */
{
	php_float32x4_t *p = php_float32x4_fetch_ex(obj);
	zval zv;
	
	array_init(&zv);
	add_next_index_double(&zv, (*p->v)[0]);
	add_next_index_double(&zv, (*p->v)[1]);
	add_next_index_double(&zv, (*p->v)[2]);
	add_next_index_double(&zv, (*p->v)[3]);

	*is_temp = 1;

	return Z_ARRVAL(zv);
}
/* }}} */

static int php_float32x4_operation(zend_uchar op, zval *returns, zval *op1, zval *op2) /* {{{ */
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

static zval *php_float32x4_read( zval *object, zval *member, int type, void **cache_slot, zval *rv) /* {{{ */
{
	php_float32x4_t *p = php_float32x4_fetch_ex(object);
	zval *property = &EG(uninitialized_zval);

	if (!member || Z_TYPE_P(member) != IS_STRING || !Z_STRLEN_P(member)) {
		return property;
	}

	switch (Z_STRVAL_P(member)[0]) {
		case 'x': ZVAL_DOUBLE(rv, (*p->v)[0]); break;
		case 'y': ZVAL_DOUBLE(rv, (*p->v)[1]); break;
		case 'z': ZVAL_DOUBLE(rv, (*p->v)[2]); break;
		case 'w': ZVAL_DOUBLE(rv, (*p->v)[3]); break;
	}
	
	return property;
} /* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(simd)
{
	zend_class_entry ce;
	
	INIT_CLASS_ENTRY(ce, "Float32x4", php_float32x4_methods);
	php_float32x4_ce = zend_register_internal_class(&ce);
	php_float32x4_ce->create_object = php_float32x4_create;
	zend_class_implements(php_float32x4_ce, 1, zend_ce_arrayaccess);

	memcpy(
		&php_float32x4_handlers,
		zend_get_std_object_handlers(), 
		sizeof(php_float32x4_handlers));
	
	php_float32x4_handlers.do_operation     = php_float32x4_operation;
	php_float32x4_handlers.get_debug_info   = php_float32x4_dump;
	php_float32x4_handlers.read_property    = php_float32x4_read;
	php_float32x4_handlers.free_obj			= php_float32x4_free;
	php_float32x4_handlers.offset			= XtOffsetOf(php_float32x4_t, std);
	return SUCCESS;
}
/* }}} */

PHP_RINIT_FUNCTION(simd) 
{
#ifdef ZTS
	ZEND_TSRMLS_CACHE_UPDATE();
#endif
}

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
	PHP_SIMD_EXTNAME,
	NULL,
	PHP_MINIT(simd),
	NULL,
	PHP_RINIT(simd),
	NULL,
	PHP_MINFO(simd),
	PHP_SIMD_VERSION,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_SIMD
ZEND_GET_MODULE(simd)
#if ZTS && defined(COMPILE_DL_SIMD)
	ZEND_TSRMLS_CACHE_DEFINE();
#endif
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
