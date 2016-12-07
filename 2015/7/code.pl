#!/usr/bin/perl -w
use strict;

my %labels;
my %values;

open(IN, "input.txt") || die;
while(my $line=<IN>) {
	chomp($line);
	my($src, $dest)=split(' -> ', $line);
	$labels{$dest}=$src;
}
close(IN);

my $val = getvalue('a');
print "Initial value of a is $val\n";
%values=('b' => $val);
print "Second value of a is ", getvalue('a'), "\n";

sub setlbl {
	my($l, $v)=@_;
	$values{$l}=$v & 0xFFFF;
	return $values{$l};
}

sub getvalue {
	my $label=shift(@_);

	if($label=~/^\d+/) {
		return $label;
	}

	if(exists $values{$label}) {
		return $values{$label};
	}

	if(!exists $labels{$label}) {
		die "No info on label $label\n";
	}

	my $src = $labels{$label};

	if(my($l)=$src=~/NOT (\S+)/) {
		my $v1=getvalue($l);
		#print "\tNOT $l ($v1) -> $label\n";
		return setlbl($label, $v1^0xFFFF);
	} elsif(my($l1,$op,$l2)=$src=~/^(\S+)\s(\S+)\s(\S+)/) {
		my $v1=getvalue($l1);
		my $v2=getvalue($l2);
		#print "\t$op $l1 ($v1), $l2 ($v2) -> $label\n";
		if($op eq 'AND') {
			return setlbl($label, $v1 & $v2);
		} elsif($op eq 'OR') {
			return setlbl($label, $v1 | $v2);
		} elsif($op eq 'LSHIFT') {
			return setlbl($label, ($v1 << $v2));
		} elsif($op eq 'RSHIFT') {
			return setlbl($label, ($v1 >> $v2));
		} else {
			die "Unknown operator '$op'\n";
		}
	} elsif(my($const)=$src=~/^(\S+)$/) {
		my $v=getvalue($const);
		#print "\tMOV $const ($v) -> $label\n";
		return setlbl($label, $v);
	}
	die "Unknown line $label: '$src'\n";
}
