<?php
namespace SIMD {

	class Float32x4
	{
		public $x;
		public $y;
		public $z;
		public $w;

		public function __construct(float $x, float $y, float $z, float $w)
		{

		}

		public function add   (Float32x4 $v) { return $v; }
		public function sub   (Float32x4 $v) { return $v; }
		public function div   (Float32x4 $v) { return $v; }
		public function mul   (Float32x4 $v) { return $v; }
		public function min   (Float32x4 $v) { return $v; }
		public function max   (Float32x4 $v) { return $v; }
		public function and   (Float32x4 $v) { return $v; }
		public function andNot(Float32x4 $v) { return $v; }
		public function or    (Float32x4 $v) { return $v; }
		public function xor   (Float32x4 $v) { return $v; }
	}
}


