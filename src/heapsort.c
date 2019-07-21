#include <stdio.h>
#include <stdlib.h>
#include <time.h>


void heapify(int *heap, int number_of_elements){
	int c, root, temp;
	srand(time(NULL)); 
	for (int i = 0; i < number_of_elements; i++)
		heap[i] = rand();
	for (int i = 1; i < number_of_elements; i++)
	{
		c = i;
		do
		{
			root = (c - 1) / 2;
			if (heap[root] < heap[c])
			{
				temp = heap[root];
				heap[root] = heap[c];
				heap[c] = temp;
			}
			c = root;
		} while (c != 0);
	}
}

void sort(int *heap, int number_of_elements){
	int c, root, temp;
	for (int j = number_of_elements - 1; j >= 0; j--)
	{
		temp = heap[0]; //swap the root node with the rightmost child
		heap[0] = heap[j];   
		heap[j] = temp;
		root = 0;
		do //maintain max heap
		{
			c = 2 * root + 1;   
			if (c < j - 1 && (heap[c] < heap[c + 1]))
				c++;
			if (c < j && heap[root]<heap[c]) 
			{
				temp = heap[root];
				heap[root] = heap[c];
				heap[c] = temp;
			}
			root = c;
		} while (c < j);
	}
}

int main( int argc, char *argv[])
{
	
	int *heap;
	int number_of_elements;
	if(argc <= 1 || !argc){
      exit(0);
   }
	sscanf (argv[1],"%d",&number_of_elements);
	if(number_of_elements<10 || number_of_elements>10000000){
		exit(0);
	}

	heap = malloc(number_of_elements * sizeof(int));
	heapify(heap, number_of_elements);
	sort(heap, number_of_elements);
	#ifdef DEBUG
	printf("The sorted array is : \n");
	for (int i = 0; i < number_of_elements; i++)
		printf("%d \n", heap[i]);
	#endif
	return 0;
}
