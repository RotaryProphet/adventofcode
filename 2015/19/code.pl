#!/usr/bin/perl
use strict;

my %rule;
open(IN, "input.txt") || die;
my %rule = map { reverse =~ m/(\w*).*\b(\w+)/ } <IN>;
close(IN);

my $string = delete $rule{""};
my $count = 0;
while ($string =~ s/(@{[ join "|", keys %rule ]})/$rule{$1}/) {
	$count++;
	print "$count @{[scalar reverse $string]}\n"
}
