#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include "my_malloc.h"
#include <time.h>

/*
Doubly Linked List for storing different block nodes( free and used blocks)
*/
struct blockNode
{
  struct blockNode *last;
  struct blockNode *next;
  int size;            // size in bytes
  char valid;          // 1 if used, 0 if free
  void *ptr;           // location in memory
};

// Helper functions
struct blockNode* find_block(size_t size);
struct blockNode* big_block(size_t size);

bool bigBlock = true; // Determines which helper function to use for malloc
size_t bufferSize; // Total size of buffer
void *startOfBuffer = NULL; // Pointer to start of Buffer

// Used for find_Block
static struct blockNode *head;
// Used for big_block()
static struct blockNode *next;


void mem_init() {
    bufferSize = 20000; // hard coded size
    // Create a new blockNode for the head of the double-linked list
    startOfBuffer = malloc(bufferSize);
    head = malloc(sizeof(struct blockNode));
    head->size = bufferSize;
    head->valid = 0;
    head->ptr = startOfBuffer;
    next = head;   
    // Make it loop
    head->last = head;
    head->next = head;
}

/*  Returns a ptr to a block of memory
    Returns NULL if no free block is found
 */
void *my_malloc(size_t size) {
    struct blockNode* block;   
    if(bigBlock){
        block = big_block(size);
        if(block == NULL){
            bigBlock = false;
            block = find_block(size);
        }
    } else {
        block = find_block(size);
    }
    if(block == NULL) {
     printf("No available blocks of size %lu!\n", size);
     return NULL;
  }

    if(block->size > size) {
        // Split the block if it has extra memory to reduce internal fragmentation
        struct blockNode* splitBlock = malloc(sizeof(struct blockNode));
        
        // Place the split block after the current block
        splitBlock->next = block->next;
        splitBlock->next->last = splitBlock;
        splitBlock->last = block;
        block->next = splitBlock;
        
        // Fix splitBlocks size and ptr
        splitBlock->size = block->size - size;
        splitBlock->valid = 0;
        splitBlock->ptr = block->ptr + size;
        block->size = size;
        next = splitBlock;
    } else {
        next = block->next;
    }
    block->valid = 1; // Set block to used
    return block->ptr; // Return ptr from the node to user
}

// Finds a block of memory from the head and returns it
// Returns null if no free block found
struct blockNode* find_block(size_t size) {
    struct blockNode* block = head;
    while(true){
        if(block->valid == 0 && block->size >= size) {
        return block;
        } else if(block->next != head){
            block = block->next;
        } else {
            return NULL;
        }
    }
}

// Returns inital big block if size is smaller or equal to it
struct blockNode* big_block(size_t size) {
        struct blockNode* newBlock = next;
        if(next->valid == 0 && next->size >= size) {
        return next;
        } else {
            return NULL;
        }
}

// Frees the block
void my_free(void* block) {
  // Find matching node
  struct blockNode* node = head;
    while(true){
        if(node->ptr == block) {
        break;
        } else if (node->next != head){
            node = node->next;
        } else {
            break;
        }
    }
  node->valid = 0;

  // coalescing blocks before it
  if(node != head && node->last->valid == 0) {
    struct blockNode* previousBlock = node->last;
    previousBlock->next = node->next;
    previousBlock->next->last = previousBlock;
    previousBlock->size += node->size;
    
    if(next == node) {
      next = previousBlock;
    }
    free(node);
    node = previousBlock;
  }

 // coalescing blocks after it
  if(node->next != head && !(node->next->valid)) {
    struct blockNode* nextBlock = node->next;
    node->next = nextBlock->next;
    node->next->last = node;
    node->size += nextBlock->size;
    
    if(next == nextBlock) {
      next = node;
    }
 
    free(nextBlock);
  }
}





// HELPERS @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@




/* Number of free blocks smaller than "size" bytes. */
int mem_small_free(int size) {
  
  int count = 0;

  /* Iterate over memory list */
  struct blockNode* block = head;
  do {
    /* Add to total only if this block's size is less than the target */
    if(block->size <= size) {
      count += !(block->valid);
    }
  } while((block = block->next) != head);

  return count;
}  

/****** Memory status/property functions ******
 * Implement these functions.
 * Note that when we refer to "memory" here, we mean the 
 * memory pool this module manages via mem_init/my_malloc/my_free. 
 */

/* Get the number of contiguous areas of free space in memory. */
int mem_holes() {
  return mem_small_free(bufferSize + 1);
}

/* Number of non-allocated bytes */
int mem_free() {    
  int count = 0;

  /* Iterate over memory list */
  struct blockNode* block = head;
  do {
    /* Add to total only if this block isn't allocated */
    if(!(block->valid)) {
      count += block->size;
    }
  } while((block = block->next) != head);

  return count;
}

/* Get the number of bytes allocated */
int mem_allocated() {
  return bufferSize - mem_free();
}



/* Number of bytes in the largest contiguous area of unallocated memory */
int mem_largest_free() {
  
  int max_size = 0;

  /* Iterate over memory list */
  struct blockNode* block = head;
  do {
    if(!(block->valid) && block->size > max_size) {
      max_size = block->size;
    }
  } while((block = block->next) != head);

  return max_size;
}



/* Is a particular byte allocated or not?
 * A necessary precondition of this function is that ptr points to somewhere in
 * the block allocated for our memory list. In fact, C99 defines pointer
 * comparison only for pointers that point to parts of the same memory object.
 */
char mem_is_alloc(void *ptr) {

  /* Iterate over the memory list */
  struct blockNode* block = head;
  while(block->next != head) {
    /* If the next block's ptr is after the target,
       the target must be in this block */
    if(ptr < block->next->ptr) {
      return block->valid;
    }
    block = block->next;
  }

  /* Iterator is now at the last block, so we assume the target is here */
  return block->valid;
}

/* 
 * Feel free to use these functions, but do not modify them.  
 * The test code uses them, but you may find them useful.
 */


// Returns a pointer to the memory pool.
void *mem_pool() {
  return startOfBuffer;
}

// Returns the total number of bytes in the memory pool.
int mem_total() {
  return bufferSize;
}


// Get string name for a strategy. 
char *strategy_name(strategies strategy) {
  switch (strategy) {
    case Best:
      return "best";
    case Worst:
      return "worst";
    case First:
      return "first";
    case Next:
      return "next";
    default:
      return "unknown";
  }
}

// Get strategy from name.
strategies strategyFromString(char * strategy) {
  if (!strcmp(strategy,"best")) {
    return Best;
  } else if (!strcmp(strategy,"worst")) {
    return Worst;
  } else if (!strcmp(strategy,"first")) {
    return First;
  } else if (!strcmp(strategy,"next")) {
    return Next;
  } else {
    return 0;
  }
}

/* 
 * These functions are for you to modify however you see fit.  These will not
 * be used in tests, but you may find them useful for debugging.
 */

/* Use this function to print out the current contents of memory. */
void print_memory() {
  printf("Memory List {\n");
  /* Iterate over memory list */
  struct blockNode* block = head;
  do {
    printf("\tBlock %p,\tsize %d,\t%s\n",
           block->ptr,
           block->size,
           (block->valid ? "[ALLOCATED]" : "[FREE]"));
  } while((block = block->next) != head);
  printf("}\n");
}

/* Use this function to track memory allocation performance.  
 * This function does not depend on your implementation, 
 * but on the functions you wrote above.
 */ 
void print_memory_status() {
  printf("%d out of %d bytes allocated.\n",mem_allocated(),mem_total());
  printf("%d bytes are free in %d holes; maximum allocatable block is %d bytes.\n",mem_free(),mem_holes(),mem_largest_free());
  printf("Average hole size is %f.\n\n",((float)mem_free())/mem_holes());
}

/* Use this function to see what happens when your malloc and free
 * implementations are called.  Run "mem -try <args>" to call this function.
 * We have given you a simple example to start.
 */
void try_mymem(int argc, char **argv) {
  strategies strat;
  void *a, *b, *c, *d, *e;
  if(argc > 1) {
    strat = strategyFromString(argv[1]);
  } else {
    strat = First;
  }
	
  /* A simple example.  
     Each algorithm should produce a different layout. */
	
  mem_init();//500
	
  a = my_malloc(100);
  b = my_malloc(100);
  c = my_malloc(100);
  my_free(b);
  d = my_malloc(50);
  my_free(a);
  e = my_malloc(25);
	
  print_memory();
  print_memory_status();
	
}
