#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct Block {
    int block_size;
    struct Block* next_block;
};

const int OVERHEAD_SIZE = sizeof(struct Block) + sizeof(void*); // size & pointer of a block
const int POINTER_SIZE = sizeof(void*);
struct Block* free_head;

void my_initialize_heap(int size) {
    free_head = (struct Block*) malloc(size);
    free_head->block_size = size - OVERHEAD_SIZE;
    free_head->next_block = NULL;
}

void* my_alloc(int size) {
    if (size <= 0) {
        printf("Size must be greater than 0\n");
        return NULL;
    }

    // Ensure size is a multiple of POINTER_SIZE
    if (size % POINTER_SIZE != 0) {
        size = size + (POINTER_SIZE - (size % POINTER_SIZE));
    }

    struct Block* curr = free_head;
    struct Block* prev = NULL;

    while (curr) {
        if (curr->block_size >= size) {
            if (curr->block_size - size >= OVERHEAD_SIZE + POINTER_SIZE) { // Splittable
                struct Block* newBlock = (struct Block*)((char*)curr + OVERHEAD_SIZE + size);
                newBlock->block_size = curr->block_size - size - OVERHEAD_SIZE;
                newBlock->next_block = curr->next_block;

                curr->block_size = size;
                curr->next_block = newBlock;

                if (prev) {
                    prev->next_block = curr;
                } else {
                    free_head = newBlock;  // Update free_head to the new block
                }
                return (void*)((char*)curr + OVERHEAD_SIZE);
            } else { // Not splittable
                if (prev) {
                    prev->next_block = curr->next_block;
                } else {
                    free_head = curr->next_block;  // Update free_head to the next block
                }
                return (void*)((char*)curr + OVERHEAD_SIZE);
            }
        } else {
            prev = curr;
            curr = curr->next_block;
        }
    }
    return NULL; // No suitable block found
}

void my_free(void* data) {
    if (!data) return;
    struct Block* freed_block = (struct Block*)((char*)data - OVERHEAD_SIZE);
    freed_block->next_block = free_head;
    free_head = freed_block;
}

void menuOptionOne() {
   int *numOne = my_alloc(sizeof(int));
   printf("Address of int A: %p\n", numOne); 
   my_free(numOne); 

   int *numTwo = my_alloc(sizeof(int));
   printf("Address of int B: %p\n", numTwo); 
};

//Allocate two ints and print their addresses; they should be exactly the size of your overhead plus the
//larger of (the size of an integer; the minimum block size) apart.
void menuOptionTwo() {
   int *numOne = my_alloc(sizeof(int));
   printf("Address of int A: %p\n", numOne); 
   int *numTwo = my_alloc(sizeof(int));
   printf("Address of int B: %p\n", numTwo); 
   printf("Verifying Results...\n");
   int overheadPlusLarger = OVERHEAD_SIZE + sizeof(void*); 
   printf("Size of overhead + larger of (the size of an integer; the minimum block size): %d bytes\n", overheadPlusLarger); 
   printf("Address B - Address A: %d bytes \n", (int)numTwo - (int)numOne); 
};

//Allocate three ints and print their addresses, then free the second of the three. Allocate an array of 2
//double values and print its address (to allocate an array in C, allocate (2 * sizeof(double)); verify
//that the address is correct. Allocate another int and print its address; verify that the address is the
//same as the int that you freed.
void menuOptionThree() {
   int *numOne = my_alloc(sizeof(int));
   printf("Address of int A: %p\n", numOne); 
   int *numTwo = my_alloc(sizeof(int));
   printf("Address of int B: %p\n", numTwo); 
   int *numThree = my_alloc(sizeof(int));
   printf("Address of int C: %p\n", numThree); 
   my_free(numTwo); 

   printf("After freeing int B...\n"); 
   double *arr = my_alloc(2 * sizeof(double));
   printf("Address of array of 2 double values: %p\n", arr); 

   int *numFour = my_alloc(sizeof(int));
   printf("Address of int D (should be the int B): %p\n", numFour); 
};

//Allocate one char, then allocate one int, and print their addresses. They should be exactly the same
//distance apart as in test #2.
void menuOptionFour() {
   int check = 0; 

   char *charOne = my_alloc(sizeof(char));
   printf("Address of char A: %p\n", charOne); 
   int *numTwo = my_alloc(sizeof(int));
   printf("Address of int B: %p\n", numTwo); 
   
   int overheadPlusLarger = OVERHEAD_SIZE + sizeof(void*); 
   printf("Size of overhead + larger of (the size of an integer; the minimum block size): %d\n", overheadPlusLarger); 

}; 

//Allocate space for a 80-element int array, then for one more int value. Verify that the address of
//the int value is 80 * sizeof(int) + the size of your header after the array's address. Free the array.
//Verify that the int's address and value has not changed.
void menuOptionFive() {
  int *arr = my_alloc(80 * sizeof(int));
  int *numOne = my_alloc(sizeof(int));
  printf("Address of array: %p\n", arr); 
  printf("Address of int A: %p\n", numOne);
  printf("Address of int value: %p\n", arr + 80 + sizeof(int));
  printf("Value of int A: %d\n", *numOne); 

  printf("Difference betwween array and int A: %d\n", (int)numOne - (int)arr); 

  my_free(arr); 

  printf("After freeing array...\n"); 
  printf("Address of int value: %p\n", numOne);
  printf("Value of int A: %d\n", *numOne); 

}

//Testing case
int main() {
     int menuChoice = 0;
   int runAgain = 1;
   while (runAgain == 1) {
      printf("\n1. Allocate an int \n2. Allocate two ints \n3. Allocate three ints \n4. Allocate one char \n5. Allocate space for an 80-element int array \n6. Quit \nChoose a menu option: ");
      scanf("%d", &menuChoice); 
      printf("\n---Test Case %d---\n", menuChoice); 
      my_initialize_heap(1000); 

      if (menuChoice == 1) {
         menuOptionOne(); 
      }

      else if (menuChoice == 2) {
         menuOptionTwo(); 
      }

      else if (menuChoice == 3) {
         menuOptionThree(); 
      }

      else if (menuChoice == 4) {
         menuOptionFour();
      }

      else if (menuChoice == 5) {
         menuOptionFive(); 
      } 

      else if (menuChoice == 6) {
         printf("Done!");
         runAgain = 0; 
      }
   } 
   return 0;
}
