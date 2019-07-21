#!/bin/bash
./clean.sh
make -C ./src/

# LIST OF EXECUTABLE PROGRAMS
executableArray=("heap" "quick" "merge")

# LOOP THROUGH EXECUTABLE PROGRAMS
for exe in ${executableArray[@]}; do
    for ((inputSize=100; inputSize <= 1000000; inputSize*=10)) do
    
        ./src/wip.out $inputSize
        valgrind --tool=lackey --trace-mem=yes ./src/$exe.out $inputSize 2>&1 | ./src/valtb379.out -p FIFO 256 256
    done

    # PLOT RESULTS TO GNUPLOT
    gnuplot -persist <<-EOFMarker
        set datafile separator ","
        set terminal png size 1400,800
        set output "./plots/ip_$exe.png"
        set title "References vs inputSize for $exe"
        set xlabel "inputSize"
        set ylabel "References"
        set xdata time
        set timefmt "%s"
        set format x "%s"
        set format y '%g' 
        set logscale y 10
        set grid
        
        plot  "./data/data.csv" using 1:7 with lines lw 2 lt 1 title 'hit rate', \
            "./data/data.csv" using 1:8 with lines lw 2 lt 2 title 'miss rate'
            
EOFMarker
    # OPEN RESULTING PLOT AND RENAME DATA FILE
    gnome-open ./plots/ip_$exe.png
    mv ./data/data.csv ./data/ip_$exe.csv
done

