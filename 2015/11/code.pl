#!/usr/bin/perl
use strict;

my $start = 'cqjxjnds';
my $working=$start;
my $numtoget=2;

my @letters=(("a".."z"),"a");
my @runs=grep { $_!~/[iol]/ } map { $_ . plusone($_) . plusone(plusone($_)) } ("a".."x");
@letters=grep { $_!~/[iol]/ } (("a".."z"),"a");

for(;;) {
	$working = incstr($working);
	if(validate($working)) {
		print "Next password '$working'\n";
		last if(--$numtoget==0);
	}
}

sub incstr {
	my $str=shift(@_);
	my @s=split(//, $str);
	for(my $i=7;$i>=0;$i--) {
		$s[$i]=plusone($s[$i]);
		last if($s[$i] ne 'a');
	}
	return join('', @s);
}

sub validate {
	my $str=shift(@_);

	if(length $str != 8) {
		#print "$str\tBad length\n";
		return 0;
	}

	if($str=~/[iol]/) {
		#print "$str\tHas i, o, l\n";
		return 0;
	}
	
	if(!grep { $str=~/$_/ } (@runs)) {
		#print "$str\tNo run\n";
		return 0;
	}

	unless($str=~/(.)\1.*(.)\2/) {
		#print "$str\tNo doubles\n";
		return 0;
	}
	#print "$str\tOK!\n";
	return 1;
}

sub plusone {
	my $char=shift(@_);
	for(my $i=0;;$i++) {
		if($letters[$i] eq $char) {
			return $letters[$i+1];
		}
	}
	die "Unable to find next for char '$char'\n";
}
	
