var dablooms = require('..');

var bloom = new dablooms.ScalingBloom(1000, 0.05, '/tmp/bloom.bin', 0);
console.log("bloom.add('foo', 2) => ", bloom.add('foo', 2));
console.log("bloom.check('bar') => ", bloom.check('bar'));
console.log("bloom.remove('foo', 2) => ", bloom.remove('foo', 2));
console.log("bloom.check('foo') => ", bloom.check('foo'));
console.log("bloom.flush() => ", bloom.flush());
