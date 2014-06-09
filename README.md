SIMD
====
*Simple Instructions, Multiple Data*

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
	
	public function cmpeq(Float32x4 $v);
	public function cmpneq(Float32x4 $v);
	public function cmplt(Float32x4 $v);
	public function cmple(Float32x4 $v);
	public function cmpgt(Float32x4 $v);
	public function cmpge(Float32x4 $v);
	public function cmpnlt(Float32x4 $v);
	public function cmpnle(Float32x4 $v);
	public function cmpngt(Float32x4 $v);
	public function cmpnge(Float32x4 $v);
	public function cmpord(Float32x4 $v);
	public function cmpunord(Float32x4 $v);
	
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
 * == -> ```equal```
 * != -> ```not equal```
 * >  -> ```larger than```
 * <  -> ```smaller than```
 * <= -> ```smaller than or equal```
 * >= -> ```greater than or equal```
 
Enjoy folks ;)

http://en.wikipedia.org/wiki/SIMD
