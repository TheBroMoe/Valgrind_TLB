#!/bin/bash
make clean -C ./src/
make -C ./src/
clear

# LIST OF EXECUTABLE PROGRAMS
executableArray=("heap" "quick" "merge")

# LOOP THROUGH EXECUTABLE PROGRAMS
for exe in ${executableArray[@]}; do
    # RUN vatlb379 PROGRAM WITH VARIOUS PAGE SIZES
    for ((pgSize=16; pgSize <= 65536; pgSize*=2)) do
        valgrind --tool=lackey --trace-mem=yes ./src/$exe.out 2>&1 | ./src/valtb379.out -i -p FIFO $pgSize 128
    done

    # PLOT RESULTS TO GNUPLOT
    gnuplot -persist <<-EOFMarker
        set datafile separator ","
        set terminal png size 1400,800
        set output "./plots/pg_$exe.png"
        set title "References vs pgSize for $exe"
        set xlabel "pgSize"
        set ylabel "References"
        set xdata time
        set timefmt "%s"
        set format x "%s"
        set grid
        
        plot "./data/data.csv" using 5:2 with lines lw 2 lt 3 title 'total', \
            "./data/data.csv" using 5:3 with lines lw 2 lt 1 title 'hits', \
            "./data/data.csv" using 5:4 with lines lw 2 lt 2 title 'misses'

EOFMarker
    # OPEN RESULTING PLOT AND RENAME DATA FILE
    gnome-open ./plots/pg_$exe.png
    mv ./data/data.csv ./data/pg_$exe.csv
done

