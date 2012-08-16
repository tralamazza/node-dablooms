var dablooms = require('..');
var assert = require('assert');

describe('Dablooms', function() {
  describe('ScalingBloom', function() {
    var bloom = null;
    before(function() {
      bloom = dablooms.createScalingBloom(1000, 0.05, '/tmp/bloom.bin');
    });
    it('should have a valid constructor', function() {
      assert(bloom);
    });
    it('should have an error_rate of 0.5', function() {
      assert.equal(bloom.error_rate, 0.05);
    });
    it('should have capacity of 1000', function() {
      assert.equal(bloom.capacity, 1000);
    });
    it('should add a value', function() {
      assert.equal(bloom.add('foo', 2), 1);
    });
    it('should check this value membership', function() {
      assert.equal(bloom.check('foo', 2), 1);
      assert.equal(bloom.check('bar', 2), 0); // false positive ???
    });
    it('should remove a value', function() {
      assert.equal(bloom.remove('foo', 2), 1);
    });
    it('should check the removal', function() {
      assert.equal(bloom.check('foo', 2), 0);
    });
    it('should have a size in bytes larger than zero', function() {
      assert(bloom.num_bytes > 0);
    });
  });
});
