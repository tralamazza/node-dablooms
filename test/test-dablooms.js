var dablooms = require('..');
var assert = require('assert');

var bloom = new dablooms.ScalingBloom(1000, 0.05, '/tmp/bloom.bin', 0);
assert.equal(bloom.add('foo', 2), 1);
assert.equal(bloom.check('bar'), 0);
assert.equal(bloom.remove('foo', 2), 1);
assert.equal(bloom.check('foo'), 0);
assert.equal(bloom.flush(), 0);
