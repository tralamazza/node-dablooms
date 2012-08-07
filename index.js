var dablooms = require('./build/Release/node_dablooms');

dablooms.createScalingBloom = function(capacity, error_rate, filename, id) {
  return new dablooms.ScalingBloom(capacity, error_rate, filename, id);
};

dablooms.createScalingBloomFromFile = function(capacity, error_rate, filename) {
  return new dablooms.ScalingBloom(capacity, error_rate, filename);
};

dablooms.createCountingBloom = function(capacity, error_rate, filename) {
  return new dablooms.CountingBloom(capacity, error_rate, filename);
};

module.exports = dablooms;
