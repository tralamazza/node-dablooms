var dablooms = require('./build/Release/node_dablooms');

dablooms.createScalingBloom = function(capacity, error_rate, filename) {
  return new dablooms.ScalingBloom(capacity, error_rate, filename, false);
};

dablooms.createScalingBloomFromFile = function(capacity, error_rate, filename) {
  return new dablooms.ScalingBloom(capacity, error_rate, filename, true);
};

dablooms.createCountingBloom = function(capacity, error_rate, filename) {
  return new dablooms.CountingBloom(capacity, error_rate, filename);
};

module.exports = dablooms;
