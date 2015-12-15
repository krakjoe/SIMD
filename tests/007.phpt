--TEST--
Check for parameter error
--SKIPIF--
<?php if (!extension_loaded("simd")) print "skip"; ?>
--FILE--
<?php 
$vec[0] = new Float32x4("strings", "are", "not", "valid");
?>
--EXPECTF--
Fatal error: Uncaught TypeError: Argument 1 passed to Float32x4::__construct() must be of the type float, string given in %s:2
Stack trace:
#0 %s(2): Float32x4->__construct('strings', 'are', 'not', 'valid')
#1 {main}
  thrown in %s on line 2



