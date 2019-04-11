#include <stdlib.h>
#include <stdio.h>
#include <time.h>

typedef struct Block{
  struct Block* next;
  int used;
  // other data members....
} Block;


// Complete this
Block* findFreeBlock(Block* head) {
  // Your code here
}


// Optional but useful
Block* createRandomBlocks() {
  srand(time(NULL));
  int numberOfBlocks = rand() % 20 + 1;

  Block* head = NULL; // Your code here

  for(int i = 0; i < numberOfBlocks; i++) {
    /*
      Your code here
    */
    ;
  }

  return head;
}

Block* createBlocks() {
  Block* first = malloc(sizeof(Block));
  Block* second = malloc(sizeof(Block));
  Block* third = malloc(sizeof(Block));
  Block* fourth = malloc(sizeof(Block));
  
  first->used = 1;
  first->next = second;
  second->used = 1;
  second->next = third;
  third->used = 0;
  third->next = fourth;
  fourth->used = 0;
  fourth->next = NULL;

  return first;
}

int main() {
  Block* head = createBlocks();

  Block* freeBlock = findFreeBlock(head);
  printf("%p", freeBlock);
}
