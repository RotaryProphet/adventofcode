#!/usr/bin/perl
use strict;
use Data::Compare;
use Clone qw/clone/;
use threads;
use threads::shared;

my $found :shared;
my @queue :shared;
push @queue, MakeShared(
	{
		'Steps'		=> 0,
		'Elevator'	=> 0,
		'Floors'	=> [
			['PG','PM','SG','SM'],
			['CG','CM','RG','RM','TG'],
			['TM'],
			[],
		],
	}
);

my $numthreads = 20;
my $nt=0;

for(my $i=0;$i<@queue;$i++) {
	do {
		foreach my $j (threads->list(threads::joinable)) {
			$j->join();
			$nt--;
		}
	} while($nt >= $numthreads);

	printf("%i/%i Step %i\n", $i+1, @queue-0, $queue[$i]{'Steps'});
	my $thr = threads->create(\&Iterate, $i);
	$nt++;
	#Iterate($queue[$i]);

	while($i == @queue-1 && $nt) {
		foreach my $j (threads->list(threads::joinable)) {
			$j->join();
			$nt--;
		}
	}
}

sub MakeShared {
	my $ref=shift(@_);
	my %new;
	share(%new);
	my @floors;
	share(@floors);
	$new{'Steps'} = $ref->{'Steps'};
	$new{'Elevator'} = $ref->{'Elevator'};
	$new{'Floors'} = \@floors;
	foreach my $floor (@{ $ref->{'Floors'} }) {
		my @nf;
		share(@nf);
		push @nf, @$floor;
		push @{ $new{'Floors'} }, \@nf;
	}
	return \%new;
}
	

sub MakeClone {
	my($ref, $tofloor, @items)=@_;
	my $new = MakeShared($ref);

	foreach my $item (@items) {
		foreach my $floor (@{ $new->{'Floors'} }) {
			@$floor = grep { $_ ne $item } @$floor;
		}
	}
	push @{ $new->{'Floors'}[$tofloor] }, @items;
	foreach my $floor (@{ $new->{'Floors'} }) {
		@$floor = sort @$floor;
	}

	$new->{'Elevator'}=$tofloor;
	$new->{'Steps'}++;

	if(IsValid($new) && !Exists($new)) {
		if(@{ $new->{'Floors'}[3] } == 10) {
			printf("All 10 on 4th floor at iteration %i\n", $new->{'Steps'});
			$found=$new->{'Steps'};
		}
		push @queue, $new;
	}
}

sub Iterate {
	my $refnum = shift(@_);
	my $ref=$queue[$refnum];

	if($found > 0 && $ref->{'Steps'} >= $found) {
		return;
	}
	my $from = $ref->{'Floors'}[ $ref->{'Elevator'} ];
	my @targets;
	if($ref->{'Elevator'}>0) {
		push @targets, $ref->{'Elevator'}-1;
	}
	if($ref->{'Elevator'} < 3) {
		push @targets, $ref->{'Elevator'}+1;
	}

	for(my $i=0;$i<@$from;$i++) {
		map { MakeClone($ref, $_, $from->[$i]) } @targets;
		for(my $j=$i+1; $j<@$from; $j++) {
			map { MakeClone($ref, $_, $from->[$i], $from->[$j]) } @targets;
		}
	}
}

sub Exists {
	my $ref=shift(@_);
	foreach my $q (@queue) {
		next if($q->{'Steps'} > $ref->{'Steps'});
		if($ref->{'Elevator'} == $q->{'Elevator'} && Compare($ref->{'Floors'}, $q->{'Floors'})) {
			return 1;
		}
	}
	return 0;
}

sub IsValid {
	my $ref=shift(@_);

	foreach my $f (@{ $ref->{'Floors'} }) {
		next unless(grep { /.G/ } (@$f));
		foreach my $chip (grep { /.M/ } (@$f)) {
			my $gen=substr($chip, 0, 1) . 'G';
			if(!grep { $_ eq $gen } (@$f)) {
				return 0;
			}
		}
	}
	return 1;
}
