#!/usr/bin/perl

print "Tests to run: ".(@SPECtests+0)."\n";
$SPECcount = 0;
foreach my $SPECtest (@SPECtests) {
  $SPECcount++;
  print "$SPECcount: Running $SPECtest\n";
  my $SPECbasename = $SPECtest;
  $SPECbasename =~ s/\.t$//o;
  my $pid = fork();
  die "Couldn't fork: $!\nStopped" unless defined($pid);
  if ($pid) {
    $rc = waitpid($pid, 0);
    print "$SPECcount: Finished $SPECtest\n";
  } else {
    print "$SPECcount: $SPECtest child start\n";
    print STDERR "$SPECcount: $SPECtest child start at ".scalar(localtime)."\n";
#    eval { $0 = "400.perlbench: $SPECtest"; };
    open(OFH, ">$SPECbasename.out") or die "Can't open $SPECbasename.out for writing: $!\n";
    open(EFH, ">$SPECbasename.err") or die "Can't open $SPECbasename.err for writing: $!\n";
    open(OLDOUT, ">&STDOUT") or die "Can't dup STDOUT: $!\n";
    open(OLDERR, ">&STDERR") or die "Can't dup STDERR: $!\n";
    open(STDOUT, ">&OFH") or die "Can't dup OFH: $!\n";
    open(STDERR, ">&EFH") or die "Can't dup EFH: $!\n";
    do $SPECtest;
    close(OFH);
    close(EFH);
    open(STDOUT, ">&OLDOUT") or die "Can't dup OLDOUT: $!\n";
    open(STDERR, ">&OLDERR") or die "Can't dup OLDERR: $!\n";
    print "$SPECcount: $SPECtest child exit\n";
    print STDERR "$SPECcount: $SPECtest child exit at ".scalar(localtime)."\n";
    if ($@) {
      print "$SPECcount: $SPECtest: $@\n";
      exit 1;
    } else {
      exit 0;
    }
  }
}
print "Tests run: $SPECcount\n";

BEGIN {
@SPECtests = (qw(
append.t
args.t
arith.t
array.t
auto.t
base_cond.t
base_pat.t
base_term.t
bless.t
bop.t
bproto.t
chars.t
chop.t
cmdopt.t
cmp.t
comp_term.t
concat.t
context.t
decl.t
defins.t
delete.t
die.t
do.t
each.t
eval.t
exists_sub.t
exp.t
fh.t
grep.t
hashwarn.t
if.t
inc.t
index.t
int.t
join.t
length.t
lex.t
list.t
loopctl.t
lop.t
method.t
my.t
nothr5005.t
oct.t
op_cond.t
op_pat.t
ord.t
override.t
package.t
pos.t
push.t
quotemeta.t
range.t
recurse.t
regexp.t
regexp_noamp.t
repeat.t
reverse.t
rs.t
sleep.t
sort.t
splice.t
study.t
sub_lval.t
subst.t
subst_amp.t
subst_wamp.t
tr.t
undef.t
unshift.t
vec.t
wantarray.t
));
}
