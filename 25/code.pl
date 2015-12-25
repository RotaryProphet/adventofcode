#!/usr/bin/perl
use strict;

my $start = 20151125;
my $mult = 252533;
my $remain = 33554393;
my $target_y = 2947;
my $target_x = 3029;

my $x=1;
my $y=1;
my $lastnum = $start;
#print "1,1 = $start\n";

for(;;) {
	$x++;
	$y--;
	if($y <= 0) {
		$y = $x;
		$x = 1;
	}

	my $newnum = ($lastnum * $mult) % $remain;
	#print "$x,$y = $newnum\n";

	if($x == $target_x && $y == $target_y) {
		print "$x,$y = $newnum\n";
		last;
	}
	$lastnum = $newnum;
}
	
