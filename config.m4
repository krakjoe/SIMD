dnl $Id$
dnl config.m4 for extension simd

PHP_ARG_ENABLE(simd, whether to enable simd support,
[  --enable-simd           Enable simd support])

if test "$PHP_SIMD" != "no"; then
  PHP_NEW_EXTENSION(simd, simd.c, $ext_shared,, -msse2 -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)
fi
