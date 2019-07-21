#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void merge(int low, int mid, int high, int *a, int *b) {
   int l1, l2, i;
   l1 = low;
   l2 = mid + 1;
   for(i = low; l1 <= mid && l2 <= high; i++) {
      if(a[l1] <= a[l2]){
         b[i] = a[l1];
         l1++;
      }
      else{
         b[i] = a[l2];
         l2++;
      }
   }
   
   while(l1 <= mid)  {
      b[i] = a[l1];
      i++;
      l1++;
   }  
      

   while(l2 <= high){
      b[i] = a[l2];
      i++;
      l2++;
   }   
      

   for(i = low; i <= high; i++)
      a[i] = b[i];
}

void sort(int low, int high, int *a, int *b) {
   int mid;
   
   if(low < high) {
      mid = (low + high) / 2;
      sort(low, mid, a, b);
      sort(mid+1, high, a, b);
      merge(low, mid, high, a, b);
   } else { 
      return;
   }   
}

int main(int argc, char *argv[]) { 
   int number_of_elements;
   int *a, *b;
   if(argc <= 1 || !argc){
      exit(0);
   }
   sscanf (argv[1],"%d",&number_of_elements);
   if(number_of_elements<10 || number_of_elements>10000000){
      exit(0);
   }
   a = malloc(number_of_elements * sizeof(int));
   b = malloc((number_of_elements-1) * sizeof(int));

   srand(time(NULL)); 
   for (int i = 0; i < number_of_elements; i++)
      a[i] = rand();

   sort(0, number_of_elements-1, a, b);

  #ifdef DEBUG
   printf("The sorted array is : \n");
   for (int i = 0; i < number_of_elements; i++)
      printf("%d \n", a[i]);
   #endif
}