SIMD
====
*Simple Instructions, Multiple Data*

This extension exposes SIMD SSE instructions for use in PHP ... for whatever reason you like ...

```php
<?php
class Float32x4 implements ArrayAccess {
	public function __construct(float $z, float $y, float $x, float $w)
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
}
?>
```

Operators
=========

The following operators will work on Float32x4 vectors as expected:

 * + -> ```add```
 * / -> ```div```
 * * -> ```mul```
 * - -> ```sub```
 * | -> ```or```
 * ^ -> ```xor```
 * & -> ```and```
 * ~ -> ```andNot```

Enjoy folks ;)

http://en.wikipedia.org/wiki/SIMD
