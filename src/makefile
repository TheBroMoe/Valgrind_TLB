.PHONY: clean

valtb379: 
	gcc -Wall -std=c99  valtb379.c -lm -o valtb379.out
	gcc -Wall -std=c99  quicksort.c -o quick.out
	gcc -Wall -std=c99  mergesort.c -o merge.out
	gcc -Wall -std=c99  heapsort.c -o heap.out
	gcc -Wall -std=c99  ex_segfault.c -o ex_seg.out
	gcc -Wall -std=c99  ex_memleak.c -o ex_mem.out
	gcc -Wall -std=c99  writeIP.c -o wip.out

clean:
	$(RM) *.out
