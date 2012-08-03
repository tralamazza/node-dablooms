var dablooms = require('..');

var bloom = dablooms.scalingBloom(1000, 0.05, '/tmp/bloom.bin', 0);
console.log(bloom.add('foo', 2));
console.log(bloom.check('bar'));
console.log(bloom.remove('foo', 2));
console.log(bloom.check('foo'));
console.log(bloom.flush());
