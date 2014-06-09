--TEST--
Check for maximum
--SKIPIF--
<?php if (!extension_loaded("simd")) print "skip"; ?>
--FILE--
<?php 
$vec[0] = new Float32x4(1, 8, 3, 4);
$vec[1] = new Float32x4(10, 2, 30, 40);
$vec[2] = $vec[1]->max($vec[0]);

var_dump
	($vec[2]);
?>
--EXPECTF--
object(Float32x4)#%d (%d) {
  [0]=>
  float(10)
  [1]=>
  float(8)
  [2]=>
  float(30)
  [3]=>
  float(40)
}


