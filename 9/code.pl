#!/usr/bin/perl -w
use strict;

my %distances;
my @cities;

open(IN, 'input.txt') || die;
while(<IN>) {
	chomp;
	my($start, $end, $dist)=$_=~/^(\S+) to (\S+) = (\d+)/;
	addtolist(\@cities, $start);
	addtolist(\@cities, $end);

	if(!ref $distances{$start}) {
		$distances{$start}={};
	}
	if(!ref $distances{$end}) {
		$distances{$end}={};
	}

	$distances{$start}{$end}=$dist;
	$distances{$end}{$start}=$dist;
}
close(IN);

my @bestroute;
my $bestdist=findroute(\@bestroute, 0, 0);
my @worstroute;
my $worstdist=findroute(\@worstroute, 0, 1);

print "Best route:\n";
print join('->', @bestroute), " ($bestdist)\n";
print "Worst route:\n";
print join('->', @worstroute), " ($worstdist)\n";

sub findroute {
	my($route, $totaldist, $longest)=@_;

	my @bestroute=@$route;
	my $bestdist=$totaldist;
	my $fromcity = undef;
	if(@$route) {
		$fromcity = pop(@$route);
		push(@$route, $fromcity);
	}

	#print join("->", @$route), " ($totaldist)\n";

	foreach my $city (@cities) {
		next if(grep { $_ eq $city } (@$route));
		my @workroute=(@$route, $city);
		my $step = 0;

		if($fromcity) {
			$step = $distances{$fromcity}{$city};
		}

		my $dist=findroute(\@workroute, $totaldist + $step, $longest);

		next if($dist == -1);

		if($bestdist == $totaldist || ($longest && $dist > $bestdist) || (!$longest && $dist < $bestdist)) {
			@bestroute=@workroute;
			$bestdist = $dist;
		}
	}
	@$route = @bestroute;
	return $bestdist;
}


sub addtolist {
	my($ref, $item)=@_;
	if(!grep { $_ eq $item } (@$ref)) {
		push @$ref, $item;
	}
}
