#!/usr/bin/perl
use strict;

open(IN, "input.txt") || die "Unable to open input: $!\n";

my @forbidden=(qw/ab cd pq xy/);
my $nice1=0;
my $nice2=0;
while(my $line=<IN>) {
	chomp($line);

	if(
		$line=~/[aeiou].*[aeiou].*[aeiou]/
	     && $line=~/(.)\1/
	     && !grep { $line=~/$_/ } (@forbidden)
	) {
		$nice1++;
	}
	
	if(	$line=~/(..).*\1/
	     && $line=~/(.).\1/
	) {
		$nice2++;
	}
}
close(IN);

print "Day 1: $nice1 nice strings\n";
print "Day 2: $nice2 nice strings\n";
