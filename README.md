# Custom Memory Management (Malloc) written in C
**Main Data Structure:** Used a doubly linked list to store different sized chunks of memory with block nodes. Each node holds the size of the block, ptr to the start of the block, and a valid field to determine if the block is being used or is currently free.
 
<p align="center">
  <img src="https://user-images.githubusercontent.com/76246845/148721426-ea8c59b1-b38d-46a7-8f05-852b6aefd850.png" />
</p> 

**Internal Fragmentation:** Used MVT structuring to allocate a new smaller chunk from an existing bigger chunk of memory. By splitting bigger blocks based on size, it reduced any possibilities of internal fragmentation from occurring.
<p align="center">
  <img src="https://user-images.githubusercontent.com/76246845/148721457-1f1ec5da-a97e-4201-b0a7-f5a47bfac296.png" />
</p> 

**External Fragmentation:** To help reduce external fragmentation, the my_free() method frees the block and also does some coalescing. It tries merging adjacent blocks of memory together into one bigger chunk of memory. This makes it so the program will not end up with numerous small chunks of memory. 

**Main Algorithm:** The implementation of malloc is a modified version of the first-fit algorithm. In general, first fit allocations are efficient but aren’t as fast as O(1) since it always starts from the head and iterates until it finds the first free block. This leads to searching and going through both already allocated blocks and freed blocks. The tweak in the first fit algorithm made it so that whenever an allocation occurs, our head moves to the next biggest available memory chunk. Malloc would automatically start allocating from the next free chunk until we reach the end of the list(tail) or if the block is too small. 
