var dablooms = require('..');
var fs = require('fs');

var FILEPATH = '/tmp/bloom.bin';
var CAPACITY = 100000;
var ERROR_RATE = 0.05;

console.log('dablooms lib version: %s', dablooms.version);

function test_scale(filename) {

  if (fs.existsSync(FILEPATH))
    fs.unlinkSync(FILEPATH);
  var bloom = dablooms.createScalingBloom(CAPACITY, ERROR_RATE, FILEPATH);

  var words = [];
  fs.readFileSync(filename, 'ascii').split('\n').forEach(function(w) { if (w) words.push(w.trim()); });

  var i = 0;
  words.forEach(function(word) {
    bloom.add(word, i++);
  });
  i = 0;
  words.forEach(function(word) {
    if (i++ % 5 === 0)
      bloom.remove(word, i);
  });
  bloom.flush();

  bloom = dablooms.createScalingBloomFromFile(CAPACITY, ERROR_RATE, FILEPATH);

  var results = { fp: 0, fn: 0, tp: 0, tn: 0, fp_rate: 0.0 };
  i = 0;
  words.forEach(function(word) {
    var exists = bloom.check(word);
    if (i++ % 5 === 0) {
      if (exists)
        results.fp++;
      else
        results.tn++;
    } else {
      if (exists)
        results.tp++;
      else {
        results.fn++;
        console.error('ERROR: False negative: ', word);
      }
    }
  });

  results.fp_rate = results.fp / (results.fp + results.tn);

  console.log(
    "\n" +
    "Elements added:   %d\n" +
    "Elements removed: %d\n" +
    "\n" +
    "True positives:   %d\n" +
    "True negatives:   %d\n" +
    "False positives:  %d\n" +
    "False negatives:  %d\n" +
    "\n" +
    "False positive rate: %d\n" +
    "Total size: %d KiB",
    i, Math.round(i / 5),
    results.tp, results.tn,
    results.fp, results.fn,
    results.fp_rate,
    Math.round(bloom.num_bytes / 1024)
  );

  if (results.fn> 0)
    console.log("TEST FAIL (false negatives exist)");
  else if (results.fp_rate > ERROR_RATE)
    console.log("TEST FAIL (false positive rate too high)");
  else 
    console.log("TEST PASS");
}

if (process.argv.length !== 3)
  console.error('Usage: ' + process.argv[2] + ' <words_file>');
else
  test_scale(process.argv[2]);
