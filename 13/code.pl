#!/usr/bin/perl
use strict;
use List::Util 'shuffle';

my %seating;
my @names;
open(IN, "input.txt") || die;
while(<IN>) {
	my($name1, $neg, $num, $name2)=$_=~/^(\S+) would (\S+) (\d+) happiness units by sitting next to (\S+)\./;
	if(!ref $seating{$name1}) {
		$seating{$name1}={};
	}
	$seating{$name1}{$name2}=($neg eq 'gain' ? $num : $num * -1);
	push @names, $name1 unless(grep { $name1 eq $_ } (@names));
	push @names, $name2 unless(grep { $name2 eq $_ } (@names));

}
close(IN);

my @best;
my $besth;

for(my $i=0;$i<10000;$i++) {
	my @working=shuffle(@names);
	my $h=gethappiness(@working);
	if($h > $besth) {
		@best = @working;
		$besth = $h;
	}
}
print "Best Order:\n";
print join(',',@best), "\n";
print "Happiness: $besth\n";

$seating{'Me'}={};
foreach my $name (@names) {
	$seating{'Me'}{$name}=0;
	$seating{$name}{'Me'}=0;
}
push @names, "Me";

$besth=0;
for(my $i=0;$i<500000;$i++) {
        my @working=shuffle(@names);
        my $h=gethappiness(@working);
        if($h > $besth) {
                @best = @working;
                $besth = $h;
        }
}
print "Day 2 Best Order:\n";
print join(',',@best), "\n";
print "Happiness: $besth\n";


sub gethappiness {
	my @order=@_;
	my $h=0;
	my $i;

	for($i=0;$i<=$#order;$i++) {
		my $n = ($i == $#order ? 0 : $i+1);
		my $p = ($i == 0 ? $#order : $i-1);
		$h += $seating{$order[$i]}{$order[$n]};
		$h += $seating{$order[$i]}{$order[$p]};
	}

	return $h;
}

	
