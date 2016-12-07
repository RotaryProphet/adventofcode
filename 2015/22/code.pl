#!/usr/bin/perl
use strict;
use Storable qw/dclone/;

my @spells=(
	{'name' => 'Magic Missile', 'cost' => 53, 'dmg' => 4},
	{'name' => 'Drain', 'cost' => 73, 'dmg' => 2, 'heal' => 2},
	{'name' => 'Shield', 'cost' => 113, 'armor' => 7, 'duration' => 6},
	{'name' => 'Poison', 'cost' => 173, 'dmg' => 3, 'duration' => 6},
	{'name' => 'Recharge', 'cost' => 229, 'mana' => 101, 'duration' => 5},
);
my %player=(
	'hp'	=> 50,
	'mana'	=> 500,
	'amor'	=> 0,
);
my %boss=(
	'hp'	=> 71,
	'dmg'	=> 10,
);
my %firstround = (
	'player'	=> {%player},
	'boss'		=> {%boss},
	'round'		=> 0,
	'manaspent'	=> 0,
	'stack'		=> [],
	'effects'	=> [],
);
my %day2 = (
	'player'	=> {%player},
	'boss'		=> {%boss},
	'round'		=> 0,
	'manaspent'	=> 0,
	'stack'		=> [],
	'effects'	=> [],
	'day2'		=> 1,
);
	

my @rounds=(\%firstround, \%day2);
my $best=0;
my $bestday2=0;

while(my $round=shift(@rounds)) {
	$round->{'round'}++;
	push @{ $round->{'stack'}}, "R $round->{'round'}.1";
	if($round->{'day2'}) {
		$round->{'player'}{'hp'}--;
		if($round->{'player'}{'hp'}<=0) {
			next;
		}
	}
	ApplyEffects($round);
	if($round->{'boss'}{'hp'} <= 0) {
		Won($round);
		next;
	}

	foreach my $spell (@spells) {
		if($spell->{'cost'} > $round->{'player'}{'mana'}) {
			next;
		} elsif(EffectActive($round, $spell->{'name'})) {
			next;
		} elsif($round->{'day2'} && $bestday2 > 0 && $round->{'manaspent'} + $spell->{'cost'} >= $best) {
			next;
		} elsif(!$round->{'day2'} && $best > 0 && $round->{'manaspent'} + $spell->{'cost'} >= $best) {
			next;
		}

		my $newround = dclone($round);
		$newround->{'manaspent'}+=$spell->{'cost'};
		$newround->{'player'}{'mana'}-=$spell->{'cost'};
		push @{$newround->{'stack'}}, "Cast $spell->{'name'} $spell->{'cost'} ($round->{'player'}{'mana'})";
		AddEffect($newround, $spell);

		push @{ $newround->{'stack'}}, "R $round->{'round'}.2";
		ApplyEffects($newround);
		if($newround->{'boss'}{'hp'} <= 0) {
			Won($newround);
			next;
		}
		my $dmg=MAX(1, $newround->{'boss'}{'dmg'} - $newround->{'player'}{'armor'});
		$newround->{'player'}{'hp'} -= $dmg;
		push @{$newround->{'stack'}}, "DMG: $dmg ($newround->{'player'}{'hp'})";
		if($newround->{'player'}{'hp'} <= 0) {
			next;
		}
		unshift @rounds, $newround;
	}
}

sub Won {
	my($round)=@_;

	if($round->{'day2'}) {
		if($bestday2 <= 0 || $round->{'manaspent'} < $bestday2) {
			$bestday2 = $round->{'manaspent'};
			print "Day 2 New best win: $bestday2\n";
		}
	} else {
		if($best <= 0 || $round->{'manaspent'} < $best) {
			#print join("\n", @{$round->{'stack'}},'');
			$best = $round->{'manaspent'};
			print "New best win: $best\n";
		}
	}
}

sub ApplyEffects {
	my($round)=@_;

	my @effects=@{$round->{'effects'}};
	$round->{'effects'}=[];

	$round->{'player'}{'armor'} = 0;
	foreach my $ref (@effects) {
		if($ref->{'heal'}) {
			my $h=$ref->{'heal'};
			if($round->{'player'}{'hp'} + $h >= $player{'hp'}) {
				$h = $player{'hp'} - $round->{'player'}{'hp'};
			}
			$round->{'player'}{'hp'}+=$h;
			push @{$round->{'stack'}}, "Heal $h ($round->{'player'}{'hp'})";
		}
		#$round->{'player'}{'hp'} += $ref->{'heal'};
		$round->{'player'}{'mana'} += $ref->{'mana'};
		$round->{'player'}{'armor'} += $ref->{'armor'};
		$round->{'boss'}{'hp'} -= $ref->{'dmg'};
		if($ref->{'dmg'}) {
			push @{$round->{'stack'}}, "Hit $ref->{'dmg'} ($round->{'boss'}{'hp'})";
		}
		
		if(--$ref->{'duration'} > 0) {
			push @{ $round->{'effects'} }, $ref;
		} else {
			push @{$round->{'stack'}}, "End $ref->{'name'}";
		}
	}
}

sub EffectActive {
	my($round, $name)=@_;

	foreach my $ref (@{$round->{'effects'}}) {
		return 1 if($ref->{'name'} eq $name);
	}
	return 0;
}

sub AddEffect {
	my($round, $spell)=@_;
	push @{$round->{'effects'}}, {%$spell};
	push @{$round->{'stack'}}, "Begin $spell->{'name'}";
}

sub MIN {
	return $_[0] < $_[1] ? $_[0] : $_[1];
}

sub MAX {
	return $_[0] > $_[1] ? $_[0] : $_[1];
}

