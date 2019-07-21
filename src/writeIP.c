#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>

int main( int argc, char *argv[])
{
	int number_of_elements;
	sscanf (argv[1],"%d",&number_of_elements);
    FILE *fs = fopen("./data/input.csv", "wa");
	fprintf(fs, "%d, ", number_of_elements);
	fclose(fs);

	return 0;
}