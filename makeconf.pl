#!/usr/bin/perl
# make conf file on louise depending on what nodes are allocated by this qsub

if ($#ARGV+1 < 3) {
    die("Usage: $0 db-nodes mediator-nodes preprocessor-nodes"."\n");
}

my $db = $ARGV[0];
my $med = $ARGV[1];
my $preproc = $ARGV[2];

my $nodefile = $ENV{'PBS_NODEFILE'};
open(NODES, $nodefile) or die("Failed to open nodefile.\n");

my @allnodes = <NODES>;
my @nodes = unique(@allnodes);

if($db + $med + $preproc > $#nodes+1) {
    die("Only ".($#nodes+1)." nodes allocated for $db dbs $med mediators and $preproc preprocessors.\n");
}


for(my $i = 0; $i < $db; $i++) {
    my $addr = shift @nodes;
    chomp $addr;
    print("node$i=db:0:0:$i:$addr:10000:10000\n");
}

for(my $i = 0; $i < $med; $i++) {
    my $addr = shift @nodes;
    chomp $addr;
    print("node".(100+$i)."=mediator:0:0:$i:$addr:10000:10000\n");
}

for(my $i = 0; $i < $preproc; $i++) {
    my $addr = shift @nodes;
    chomp $addr;
    print("node".(200+$i)."=preprocessor:0:0:$i:$addr:10000:10000\n");
}

print "binlog=/tmp/binlog\n";
print "wait_time=10000\n";

# unique: removes duplicate items in a list
sub unique {
    my %seen = ();
    my @r = ();
    foreach my $a (@_) {
        unless ($seen{$a}) {
            push @r, $a;
            $seen{$a} = 1;
        }
    }
    return @r;
}
