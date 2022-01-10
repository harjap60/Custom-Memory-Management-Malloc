//Custom malloc implementation
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "my_malloc.h"

/*
Doubly Linked List for storing different block nodes( free and used blocks)
*/
struct blockNode
{
    struct blockNode *last;
    struct blockNode *next;
    int size;   // size in bytes
    char valid; // 1 if used, 0 if free
    void *ptr;  // location in memory
};

// Helper functions
struct blockNode *find_block(size_t size);
struct blockNode *big_block(size_t size);

bool bigBlock = true;       // Determines which helper function to use for malloc
size_t bufferSize;          // Total size of buffer
void *startOfBuffer = NULL; // Pointer to start of Buffer

// Used for find_Block
static struct blockNode *head;
// Used for big_block()
static struct blockNode *next;

void mem_init()
{
    bufferSize = 25000; // hard coded size
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
void *my_malloc(size_t size)
{
    struct blockNode *block;
    if (bigBlock)
    {
        block = big_block(size);
        if (block == NULL)
        {
            bigBlock = false;
            block = find_block(size);
        }
    }
    else
    {
        block = find_block(size);
    }
    if (block == NULL)
    {
        printf("No available blocks of size %lu!\n", size);
        return NULL;
    }

    if (block->size > size)
    {
        // Split the block if it has extra memory to reduce internal fragmentation
        struct blockNode *splitBlock = malloc(sizeof(struct blockNode));

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
    }
    else
    {
        next = block->next;
    }
    block->valid = 1;  // Set block to used
    return block->ptr; // Return ptr from the node to user
}

// Finds a block of memory from the head and returns it
// Returns null if no free block found
struct blockNode *find_block(size_t size)
{
    struct blockNode *block = head;
    while (true)
    {
        if (block->valid == 0 && block->size >= size)
        {
            return block;
        }
        else if (block->next != head)
        {
            block = block->next;
        }
        else
        {
            return NULL;
        }
    }
}

// Returns inital big block if size is smaller or equal to it
struct blockNode *big_block(size_t size)
{
    struct blockNode *newBlock = next;
    if (next->valid == 0 && next->size >= size)
    {
        return newBlock;
    }
    else
    {
        return NULL;
    }
}

// Frees the block
void my_free(void *block)
{
    // Find matching node
    struct blockNode *node = head;
    while (true)
    {
        if (node->ptr == block)
        {
            break;
        }
        else if (node->next != head)
        {
            node = node->next;
        }
        else
        {
            break;
        }
    }
    node->valid = 0;

    // coalescing blocks before it
    if (node != head && node->last->valid == 0)
    {
        struct blockNode *previousBlock = node->last;
        previousBlock->next = node->next;
        previousBlock->next->last = previousBlock;
        previousBlock->size += node->size;

        if (next == node)
        {
            next = previousBlock;
        }
        free(node); // Free node if we combined 2 adjacent nodes together
        node = previousBlock;
    }

    // coalescing blocks after it
    if (node->next != head && node->next->valid == 0)
    {
        struct blockNode *nextBlock = node->next;
        node->next = nextBlock->next;
        node->next->last = node;
        node->size += nextBlock->size;

        if (next == nextBlock)
        {
            next = node;
        }
        free(nextBlock); // Free node if we combined 2 adjacent nodes together
    }
}