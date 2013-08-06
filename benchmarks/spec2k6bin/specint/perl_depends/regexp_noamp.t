#!./perl

$skip_amp = 1;
for $file ('regexp.t', './op/regexp.t', './t/op/regexp.t', ':op:regexp.t') {
  if (-r $file) {
    do $file;
    last;
  }
}
#die "Cannot find ./op/regexp.t or ./t/op/regexp.t\n";
1;
