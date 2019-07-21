#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

void swap(int num1, int num2, int *input_array) {
   int temp = input_array[num1];
   input_array[num1] = input_array[num2];
   input_array[num2] = temp;
}

int partition(int left, int right, int pivot, int *input_array) {
   int i = left - 1;
   for(int j = left; j < right; j++){
      if(input_array[j] < pivot){
         i++;
         swap(i, j, input_array);
      }
   }
   swap(i+1, right, input_array);
   return i + 1;
  
}

void quickSort(int left, int right, int *input_array) {
   if(right-left <= 0) {
      return;   
   } else {
      int pivot = input_array[right];
      int partitionPoint = partition(left, right, pivot, input_array);
      quickSort(left,partitionPoint-1, input_array);
      quickSort(partitionPoint+1,right, input_array);
   }        
}

int main(int argc, char *argv[]) {
   int number_of_elements;
   int *input_array;
   if(argc <= 1 || !argc){
      exit(0);
   }
   sscanf (argv[1],"%d",&number_of_elements);
   if(number_of_elements<10 || number_of_elements>10000000){
      exit(0);
   }
   input_array = malloc(number_of_elements * sizeof(int));
   
   srand(time(NULL)); 
   for (int i = 0; i < number_of_elements; i++)
      input_array[i] = rand();

   quickSort(0, number_of_elements-1, input_array);

   #ifdef DEBUG
   printf("The sorted array is : \n");
   for (int i = 0; i < number_of_elements; i++)
      printf("%d \n", input_array[i]);
   #endif

}