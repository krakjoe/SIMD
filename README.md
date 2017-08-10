SIMD
====
*Single Instructions, Multiple Data*

[![Build Status](https://travis-ci.org/krakjoe/SIMD.svg?branch=master)](https://travis-ci.org/krakjoe/SIMD)

This extension exposes SIMD SSE instructions for use in PHP ... for whatever reason you like ...

```php
<?php
class Float32x4 {
	public function __construct(float $x, float $y, float $z, float $w)
	public function add(Float32x4 $v);
	public function sub(Float32x4 $v);
	public function div(Float32x4 $v);
	public function mul(Float32x4 $v);
	public function min(Float32x4 $v);
	public function max(Float32x4 $v);
	public function and(Float32x4 $v);
	public function andNot(Float32x4 $v);
	public function or(Float32x4 $v);
	public function xor(Float32x4 $v);
	
	public $x;
	public $y;
	public $z;
	public $w;
}
?>
```

Operators
=========

The following operators will work on Float32x4 vectors as expected:

 * + -> ```addition```
 * / -> ```division```
 * * -> ```multiplication```
 * - -> ```subtraction```
 * | -> ```bitwise or```
 * ^ -> ```bitwise xor```
 * & -> ```bitwise and```
 * ~ -> ```bitwise not```
 
Enjoy folks ;)

http://en.wikipedia.org/wiki/SIMD
