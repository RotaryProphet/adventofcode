#!/usr/bin/perl
use strict;
use JSON;
use Data::Dumper;

my $total;
my $total2;

open(IN, "input.txt") || die;
my $data=join('',<IN>);;
close(IN);

my $ref=decode_json($data);
print Dumper($ref);
$total = summ($ref);
$total2 = summ($ref, 'red');

print "Total of numbers is $total\n";
print "Day2 total is $total2\n";

sub summ {
	my($ref, $skip)=@_;
	my $val;

	if(ref $ref && $ref=~/ARRAY/) {
		map { $val+=summ($_, $skip) } (@$ref);
	} elsif(ref $ref && $ref=~/HASH/) {
		if($skip && grep { lc($_) eq $skip } (values %$ref)) {
			return 0;
		}
		map { $val+=summ($_, $skip) } (values %$ref);
	} elsif(my($v)=$ref=~/(-?\d+)/) {
		$val=$v;
	}
	return $val;
}
