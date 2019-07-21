# A description of source files:

    * run.sh: Runs valtb379 once taking any and all desired input. This is meant to be used as a single-use case for executing valtb379
    
    * clean.sh: Removes all plots and data files and runs make clean on the source code
    
    * tlbsize_test.sh: Runs valtb39 for all example code with varying tlbsize inputs and plots the data into GNUPLOT

    * pgsize_test.sh: Runs valtb39 for all example code with varying tlbsize inputs and plots the data using GNUPLOT

    * data: Contains all the csv files written toby valtb379. Used toplot data toGNU Plot. Data obtained from executing
    code and scripts is written tothis directory

    * plots: Contains all the generated graphs from data using GNUPLOT. Plots obtained from executing scripts is written tothis directory

    * src: Contains all the source code used tohandle valgrind output and compute data tofiles. This includes
        - The given example code from eclass
        - The valtb379 executable


# Design Explanation:

* Data Structure for TLB:
For our implementation of simulating the TLB, we used a linked list tostore the page number in a node link structure with a size equivalent to the given tlbsize. We parsed the valgrind data from stdin.With every address taken from the pipe, we would search the Linked List to see if the page number already exists, and if it does not, we increment the number of misses, otherwise if we have a hit, we increment the number of hits, and after every memory reference we increment the total number of memory references. For a TLB miss, We always append a new node to the end of the list. This made it straightforward to evict nodes when required regardless of eviction policy. The TLB hits and misses are with respect to page references, hence a memory access spanning over two pages results in two page references and hence two TLB operations.
For the FIFO policy, removing nodes was trivial as the first node in will always be the node removed which was done using the remove_first() method. For the LRU policy, every time we had a TLB hit, we moved the node to the end to indicate it was the most recently used. This meant the first node would always be our least recently used node, and therefore was just as straightforward to remove. This allowed for linear time and linear space storage execution that was significantly efficient in it's implementation.

* Plotting and Computing Data
After a single execution of valtb379, the resulting data of number of misses, hits, and total refences was appended to a csv file at the end of exectution along with it's current tlbsize and pgsize, and was used by GNU PLOT to graph the resulting data into a png.
