--TEST--
Check for addition
--SKIPIF--
<?php if (!extension_loaded("simd")) print "skip"; ?>
--FILE--
<?php 
$vec[0] = new \SIMD\Float32x4(1, 2, 3, 4);
$vec[1] = new \SIMD\Float32x4(10, 20, 30, 40);
$vec[2] = $vec[0] + $vec[1];

var_dump($vec[2]);
?>
--EXPECTF--
object(SIMD\Float32x4)#%d (%d) {
  [0]=>
  float(11)
  [1]=>
  float(22)
  [2]=>
  float(33)
  [3]=>
  float(44)
}

