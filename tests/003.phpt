--TEST--
Check for subtraction
--SKIPIF--
<?php if (!extension_loaded("simd")) print "skip"; ?>
--FILE--
<?php 
$vec[0] = new Float32x4(1, 2, 3, 4);
$vec[1] = new Float32x4(10, 20, 30, 40);
$vec[2] = $vec[1] - $vec[0];

var_dump($vec[2]);
?>
--EXPECTF--
object(Float32x4)#%d (%d) {
  ["x"]=>
  float(9)
  ["y"]=>
  float(18)
  ["z"]=>
  float(27)
  ["w"]=>
  float(36)
}



