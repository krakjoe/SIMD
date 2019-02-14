--TEST--
Check for minimum
--SKIPIF--
<?php if (!extension_loaded("simd")) print "skip"; ?>
--FILE--
<?php 
$vec[0] = new \SIMD\Float32x4(1, 8, 3, 4);
$vec[1] = new \SIMD\Float32x4(10, 2, 30, 40);
$vec[2] = $vec[1]->min($vec[0]);

var_dump
	($vec[2]);
?>
--EXPECTF--
object(SIMD\Float32x4)#%d (%d) {
  [0]=>
  float(1)
  [1]=>
  float(2)
  [2]=>
  float(3)
  [3]=>
  float(4)
}


