#!/usr/bin/perl
use strict;
use Data::Dumper;
use vars qw/$s/;

my $totalpacked=0;
my $totalunpacked=0;
my $totalrepacked=0;

open(IN, 'input2.txt') || die;
while(<IN>) {
	chomp;
	my $unpacked=$_;
	my $ul=length $unpacked;

	my $evalstr = "\$s=$unpacked;";
	eval $evalstr;
	if($@) {
		die "Unable to eval '$evalstr': $@\n";
	}
	my $packed=$s;
	my $pl=length $packed;
	
	my $repacked=Dumper($_);
	($repacked)=$repacked=~/'(.+)'/;
	$repacked=~s/"/\\"/g;
	$repacked=~s/\\'/'/g;
	$repacked=~s/^'/"/g;
	$repacked=~s/'$/"/g;
	$repacked="\"$repacked\"";
	my $rl=length $repacked;

	print "$ul: $unpacked\n";
	print "$pl: $packed\n";
	print "$rl: $repacked\n";
	print "\n";

	$totalunpacked+=$ul;
	$totalpacked+=$pl;
	$totalrepacked+=$rl;
}
close(IN);

my $sp=$totalunpacked - $totalpacked;
my $nr=$totalrepacked - $totalunpacked;

print "Total of $totalunpacked unpacked, $totalpacked packed, $totalrepacked repacked\n";
print "Save $sp by packing\n";
print "Need $nr by repacking\n";
