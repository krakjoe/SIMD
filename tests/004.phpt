--TEST--
Check for division
--SKIPIF--
<?php if (!extension_loaded("simd")) print "skip"; ?>
--FILE--
<?php 
$vec[0] = new Float32x4(1, 2, 3, 4);
$vec[1] = new Float32x4(10, 20, 30, 40);
$vec[2] = $vec[1] / $vec[0];

var_dump
	($vec[2]);
?>
--EXPECTF--
object(Float32x4)#%d (%d) {
  [0]=>
  float(10)
  [1]=>
  float(10)
  [2]=>
  float(10)
  [3]=>
  float(10)
}


