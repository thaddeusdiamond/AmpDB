#!/usr/bin/perl -w

use strict;
use warnings;

my $dbs = 16;
my @preprocs = (1,2,3,4,5);
my @mediators = (1,2,3,4,5);
my $wait_time = 10000;
my $port = 10000;

open (HOSTS,"hosts");
my @hosts = map{ chomp; $_ }<HOSTS>;
close HOSTS;

foreach my $p (@preprocs) {
    foreach my $m (@mediators) {
        if($dbs + $p + $m > @hosts){
            print "Warning: ", $dbs + $p + $m, " machines needed, but only ",
                  scalar @hosts, " provided\n";
            next;
        }
        
        my $configfile = "testD${dbs}P${p}M${m}.conf";
        open (CONFIGFILE, ">$configfile");
        
        # db nodes
        for(my $i = 0; $i < $dbs; $i++) {
            my $ip = $hosts[$i];
            print CONFIGFILE "node$i=db:3:0:$i:$ip:".($port++).":".($port++)."\n";
        }
        
        # mediator nodes
        for(my $i = 0; $i < $m; $i++) {
            my $ip = $hosts[$i+$dbs];
            print CONFIGFILE "node".(100+$i)."=mediator:1:0:$i:$ip:".($port++).":".($port++)."\n";
        }
        
        # preprocessor nodes
        for(my $i = 0; $i < $p; $i++) {
            my $ip = $hosts[$i+$dbs+$m];
            print CONFIGFILE "node".(200+$i)."=preprocessor:1:0:$i:$ip:".($port++).":".($port++)."\n";
        }

        print CONFIGFILE "wait_time=$wait_time\nbinlog=/tmp/binlog\n";
        close CONFIGFILE;

        my $pid = fork;
        if($pid < 0){
                print "Fork failed, exiting...\n";
                unlink $configfile;
                exit(0);
        }elsif($pid == 0){
                open STDOUT, '>', "out.D${dbs}P${p}M${m}"
                        or die "Cannot redirect STDOUT";
                open STDERR, '>&STDOUT'
                        or die "Cannot redirect STDERR (dup STDOUT)";
                open STDIN, '<', '/dev/null';
                exec{ "utils/cluster" }
                        "utils/cluster", $configfile,
                        "utils/dummy-db", "preprocessor/preproc", "mediator/mediator";
                die "Exec failed";
        }else{
                sleep 10;
                kill INT => $pid;
                sleep 2;
                kill KILL => $pid;

                waitpid $pid, 0;
        }

        unlink $configfile;

        $port = 10000 if $port > 20000;
    }
}

