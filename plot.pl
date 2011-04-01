#!/usr/bin/perl -w

system "rm data/outM*";

for(my $j = 1; $j <= 5; $j++) {
    open (OUT,">>data/outM$j");
    print OUT "15 0 $j 0\n";
    close OUT;
}

for(my $i = 1; $i <= 5; $i++) {
    for(my $j = 1; $j <= 5; $j++) {

        system "grep Finished data/out.D15P${i}M$j > tempfile";

        open (FILE, "<tempfile");
        my @lines = <FILE>;

        my $total = 0;

        foreach $line (@lines) {
            @tokens = split (/ /,$line);
            $total = $total + $tokens[2]/$tokens[5];
        }

        
        open (OUT,">>data/outM$j");
        print OUT "15 $i $j $total\n" if $total > 50000;
        close OUT;
        
        close FILE;
        system "rm tempfile";
    }
}


system 'echo "set terminal postscript eps enhanced dashed size 4,3

set output \'preproc.eps\'

set multiplot layout 1,1

set style line 1 lt 1 lc rgb \"#ff0000\" lw 2
set style line 2 lt 1 lc rgb \"#00ff00\" lw 2
set style line 3 lt 1 lc rgb \"#0000ff\" lw 2
set style line 4 lt 1 lc rgb \"#ff00ff\" lw 2
set style line 5 lt 1 lc rgb \"#00ffff\" lw 2
set style line 6 lt 1 lc rgb \"#ffff00\" lw 2

set tics nomirror scale 0
set border 3 front lw 2 lt 1

set ylabel \"total throughput (txns/sec)\"
set xlabel \"number of preprocessor nodes\"
plot [0:][0:] \'data/outM1\' using 2:4 with lines ls 1 title \'1 client\',\
              \'data/outM2\' using 2:4 with lines ls 2 title \'2 clients\',\
              \'data/outM3\' using 2:4 with lines ls 3 title \'3 clients\',\
              \'data/outM4\' using 2:4 with lines ls 4 title \'4 clients\',\
              \'data/outM5\' using 2:4 with lines ls 5 title \'5 clients\'


" | gnuplot'

