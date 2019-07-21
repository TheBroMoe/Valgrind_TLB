
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <time.h>
#include <math.h>

// Read length for buffer
#define MAX_READ_LEN 128

// Enum state for eviction policy
enum evictionPolicy {FIFO, LRU};

// Three Numbers to return
long int totalReferences;
long int hitCounter;
long int missCounter;

// LINKED LIST IMPLEMENTATION FOR TLB

// Node structure used to store data into linked list
struct node {
	unsigned long arrayAddress;  // Value of page number to be stored in TLB
	struct node* next;
};

// LinkedList data structure used for internal storage of TLB
typedef struct{
	struct node* first;
	struct node* last;
	int count;
} LinkedList;

// Removes the first node from given Linked List
void remove_first(LinkedList *linkList) {
	if ( linkList->first == NULL) {
		return;
	}
	struct node *temp = linkList->first->next;
	free(linkList->first);
	linkList->first = temp;
	linkList->count--;
}

// Prints the first node
void print_first_node(LinkedList *linkList) {
	printf("THE FIRST LINKED LIST IS %lu\n", linkList->first->arrayAddress);
}

void print_last_node(LinkedList *linkList) {
	printf("THE LAST LINKED LIST IS %lu\n", linkList->last->arrayAddress);
}

// Searches Linked list for given page number
// Returns index if page number found
int search_list(LinkedList *linkList, unsigned long number) {
	// Initialize counter and current node as first node
	struct node *current_node = linkList->first;
	int counter = 0;
	// Search through linked list
	while(current_node != NULL) {
		// Return counter if page number found
		if (current_node->arrayAddress == number) {
			return counter;
		}
		counter++;
		current_node = current_node->next;
	}
	return -1; 
}


// Insert function used to store page number in Linked List
void insert_last_address(LinkedList *linkList, unsigned long number){
	// Create new node and allocate memory for storage
	struct node *new_node;
	new_node = malloc(sizeof(struct node));
	new_node->arrayAddress = number;

	// If linked list empty, set node as first and last element
	if (linkList->first == NULL) {
		new_node->next = NULL;
		linkList->first = new_node;
		linkList->last = new_node;
	// Otherwise, set node as last element
	} else {
		new_node->next = NULL;
		linkList->last->next = new_node;
		linkList->last = new_node;
	}
	linkList->count++;
	return;
}	

// Moves node of given index from Linked list to the last node
void move_to_last(LinkedList *linkList, int index) {
	struct node *current_node = linkList->first;
	// If node to move is first element
	if (index == 0){
		// If node is the only element in linked list
		if (current_node->next == NULL){
			return;
		}
		
		// Set next node as first node and move previous first node to last node
		linkList->first = current_node->next;
		current_node->next = NULL;
		linkList->last->next = current_node;
		linkList->last = current_node;
		return;
	}

	// Return if node to move is already the last node
	if(index == linkList->count - 1){
		return;
	}

	// Otherwise traverse through linked list and replace node at index to last node
	int counter = 0;
	while(current_node != NULL) {
		if (counter == index - 1) {
			struct node *temp = current_node->next;
			current_node->next = current_node->next->next;
			temp->next = NULL;
			linkList->last->next = temp;
			linkList->last = temp;
			return;
		}
		current_node = current_node->next;
		counter++;
	}
	return;
}

// Empties all nodes within given Linked List
void destroy(LinkedList *linkList) {
	struct node *temp = linkList->first;
	struct node *curr = linkList->first;
        while (temp != NULL) {
            temp = temp->next;
			free(curr);
			curr = temp;
        }
	free(temp);
	linkList->first = NULL;
	linkList->last = NULL;
	linkList->count = 0;
}

// Parses the valgrind output to find and return the starting character
int findInitialLetter(char *buffer, int iFlag) {
	int letterIndex;
	for(letterIndex = 0; (buffer[letterIndex] == ' '); letterIndex++);

	if ((buffer[letterIndex] == 'I') && (iFlag)) {
		return -1;
	}		
	return letterIndex;
}

// Parses the valgrind output to find and return the address
void grabHex(char *buffer,char *arrayList, int letterIndex) {
	int numberIndex;
	int i;
	for(numberIndex = letterIndex+1; (buffer[numberIndex] == ' '); numberIndex++);
	for(i = 0; (buffer[numberIndex] != ','); numberIndex++, i++) {	
	}
	
}

// TLB handler function for adding and removing page numbers from the TLB
void tlb_handler(LinkedList *page_table, unsigned long page_num, enum evictionPolicy evicPolicy,int tlbsize,int flushAmount){
		// Search TLB to see if address is already stored
		int addr_hit = search_list(page_table, page_num);

		// TLB hit
		if (addr_hit >= 0){
			hitCounter++;  // Increment total hits
			// If LRU eviction policy update TLB
			if(evicPolicy == LRU){
				int index = search_list(page_table, page_num);
				move_to_last(page_table, index);
			}

		// TLB MISS
		}else{
			missCounter++;  // Increment total misses
			
			// Remove page number based on eviction policy
			if(page_table->count >= tlbsize){
				remove_first(page_table);
			}
			// Insert page number in TLB
			insert_last_address(page_table, page_num);
		}

		totalReferences++;  // Increment total memory references

		// If we have a flush period
		if(flushAmount > 0){
			// Check if we have completed flush period cycle
			if(totalReferences % flushAmount == 0){
				// Flush TLB
				destroy(page_table);
			}
		}
}

// MAIN METHOD
int main(int argc, char* argv[]) {

	// Initialize Linked List for tlb storage
	LinkedList *page_table = malloc(sizeof(LinkedList));
	page_table->first = NULL;
	page_table->last = NULL;
	page_table->count = 0;

	// Initialize buffer to take valgrind output
	char buff[MAX_READ_LEN + 1];
	char* numbers = NULL;


	// Initialize numbers to return and write to data faster
	totalReferences = 0;
	hitCounter = 0;
	missCounter = 0;
	
	// Initialize input arguments
	int iFlag = 0;
	enum evictionPolicy evicPolicy;
	int flushAmount = 0;

	// Set input arguments
	double pgsize = atoi(argv[argc-2]);
	int bitsToRead = floor(log(pgsize)/log(2));
	numbers = malloc(pgsize/16);  // Allocate numbers based on page size
	int tlbsize = atoi(argv[argc-1]);

	// Set eviction policy
	if (strcmp(argv[argc-3], "FIFO") == 0) {
		evicPolicy = FIFO;
	} else  {
		evicPolicy = LRU;
	}		
	
	// Set optional arguments if entered
	for (int i = 1; i <= (argc-4); i++) {
		if ((strcmp(argv[i], "-i") == 0)) {
			iFlag = 1;		
		} else if (strcmp(argv[i], "-f") == 0) {
			flushAmount = atoi(argv[i+1]);
		} 
	}	

	// MAIN LOOP
	// Read in standard input for valgrind output line by line
	while( fgets(buff, MAX_READ_LEN, stdin) ){

		// Ignore valgrind header text
		if (buff[0] == '=') {
			continue;
		}		


		// Get valgrind letter argument
		int letterIndex = findInitialLetter(buff, iFlag);		
		if (letterIndex == -1) {
			continue;
		}

		// Get valrgrind address
		int numberIndex;
		for(numberIndex = letterIndex+1; (buff[numberIndex] == ' '); numberIndex++);
		int i = 0;
		for(i = 0; (buff[numberIndex] != ','); numberIndex++, i++) {	
			numbers = realloc(numbers, (i+1));
			numbers[i] = buff[numberIndex];
		}
		numbers[i] = '\0';

		// Get size of reference
		char ref[3] = {0};
		ref[0] = buff[numberIndex + 1];
		ref[1] = buff[numberIndex + 2];		
		ref[2] = '\0';
		int refSize = atoi(ref);

		// Get page address
		unsigned long page_address = strtoul(numbers, NULL, 16);

		// Get page offset and max page number to check if page boundary is surpassed
		unsigned long page_offset = page_address + refSize;
		unsigned long max_page_num = page_offset >> bitsToRead;

		// Convert address to page number
		unsigned long numbers_page_num = page_address >> bitsToRead;

		// If page boundary is hit
		if(numbers_page_num != max_page_num){
			// Requires an additional memory reference and adding both addresses to TLB
			tlb_handler(page_table, numbers_page_num, evicPolicy, tlbsize, flushAmount);
			tlb_handler(page_table, max_page_num, evicPolicy, tlbsize, flushAmount);
		}else{
			// Otherwise just insert the page number
			tlb_handler(page_table, numbers_page_num, evicPolicy, tlbsize, flushAmount);
		}
		

	}
	
	// REPORT THREE NUMBERS: Hits, Misses, Total
	printf("Program Results:\nHits: %ld\nMisses:%ld\nTotal:%ld\n", 
		hitCounter, missCounter, totalReferences);

	// CALCULATE HIT RATE AND MISS RATE
	double hitRate = (long double) hitCounter / (long double) totalReferences;
	double missRate = (long double) missCounter / (long double) totalReferences;

	FILE *fr = fopen("./data/input.csv", "r");
	
	int number_of_elements;

	if(fr != NULL){
		fscanf(fr, "%d", &number_of_elements);
	}else{
		number_of_elements = 0;
	}

	// WRITE TO CSV FILE WITH THREE RESULTS
	FILE *fs = fopen("./data/data.csv", "a");
	if(number_of_elements > 0){
		fprintf(fs, "%d, %d, %lf, %ld, %ld, %ld, %lf, %lf\n",
			number_of_elements, tlbsize, pgsize, totalReferences, hitCounter, missCounter, hitRate, missRate);
	}else{
		fprintf(fs, "%d, %lf, %ld, %ld, %ld, %lf, %lf\n",
			tlbsize, pgsize, totalReferences, hitCounter, missCounter, hitRate, missRate);
	}
	fclose(fs);

	// Clear memory allocation from TLB
	destroy(page_table);

	return 0;
}
