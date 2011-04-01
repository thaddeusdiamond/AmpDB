#!/usr/bin/perl -w
use strict;
use IO::Socket;
BEGIN { $| = 1 }

my $sock;
my $port = 50001;
my $n = 1;
if($#ARGV >= 0) { $n = $ARGV[0]; }

for(my $i = 1; $i <= $n; $i++) {
    $port++ until($sock = IO::Socket::INET->new(LocalHost => '127.0.0.1',
                                                LocalPort => $port,
                                                Proto => 'tcp'));
    close $sock;
    print "$port\n";
    $port++;
}



