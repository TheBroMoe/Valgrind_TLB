#!/bin/bash
make clean -C ./src/
make -C ./src/
clear

# LIST OF EXECUTABLE PROGRAMS
executableArray=("heap" "quick" "merge")

# LOOP THROUGH EXECUTABLE PROGRAMS
for exe in ${executableArray[@]}; do
    # RUN vatlb379 PROGRAM WITH VARIOUS PAGE SIZES
    for ((tlbSize=2; tlbSize <= 2048; tlbSize*=2)) do
        valgrind --tool=lackey --trace-mem=yes ./src/$exe.out 50000 2>&1 | ./src/valtb379.out -p FIFO 256 $tlbSize
    done

    # PLOT RESULTS TO GNUPLOT
    gnuplot -persist <<-EOFMarker
        set datafile separator ","
        set terminal png size 1400,800
        set output "./plots/tlb_$exe.png"
        set title "References vs tlbsize for $exe"
        set xlabel "tlbsize"
        set ylabel "References"
        set xdata time
        set timefmt "%s"
        set format x "%s"
        set format y '%g' 
        set logscale y 10
        set grid
        
        plot  "./data/data.csv" using 1:6 with lines lw 2 lt 1 title 'hit rate', \
            "./data/data.csv" using 1:7 with lines lw 2 lt 2 title 'miss rate'
            
EOFMarker
    # OPEN RESULTING PLOT AND RENAME DATA FILE
    gnome-open ./plots/tlb_$exe.png
    mv ./data/data.csv ./data/tlb_$exe.csv
done

