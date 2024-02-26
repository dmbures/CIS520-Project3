# Homework 3 - Block Store


Assignment Requirements:
You must update and complete the given CMakeLists.txt to build a shared object (a dynamic library) called libblock_store.so that uses src/block_store.c and src/bitmap.c for source files.
Complete all the functions as described in the header to build a library called libblock_store.so
You must check all function calls for error conditions. Occasionally, this will mean you will have to check errno, so don’t forget to #include <errno.h>. Please refer to manual pages for information. Hint: There's only one global errno per thread so you have to check it right after you make a system call, otherwise the next system call might set it to something else (like "Success"). Hint 2: You can print the "plain English" error message associated with the current errno code by calling perror("some useful text").
You must ensure that your solution is free of memory leaks. You can check your program for leaks with Valgrind (don't forget to load its module: module load Valgrind/3.16.1-gompi-2020a).
Rubric:
This assignment is out of 100 points. The included tests.cpp file provides points for a variety of tests.

All file operations should be using POSIX interface; you will lose points for using FILE objects. If your code does not compile, you will receive a zero for the assignment.

Insufficient Parameter Validation: Deduction of up to 20% of rubric score

Insufficient Error Checking: Deduction of up to 20% of rubric score

Insufficient Block and Inline Comments: Deduction of up to 20% of rubric score

Submission compiles with warnings (with -Wall -Wextra -Wshadow): Deduction of up to 30%

 

Implementation guidelines:
block_store_create(): This function creates a new block store and returns a pointer to it. It first allocates memory for the block store and initializes it to zeros using the memset Then it sets the bitmap field of the block store to an overlay of a bitmap with size BITMAP_SIZE_BITS on the blocks starting at index BITMAP_START_BLOCK. Finally, it marks the blocks used by the bitmap as allocated using the block_store_request function.
block_store_destroy(block_store_t *const bs): This function destroys a block store by freeing the memory allocated to it. It first checks if the pointer to the block store is not NULL, and if so, it frees the memory allocated to the bitmap and then to the block store.
block_store_allocate(block_store_t *const bs): This function finds the first free block in the block store and marks it as allocated in the bitmap. It returns the index of the allocated block or SIZE_MAX if no free block is available.
block_store_request(block_store_t *const bs, const size_t block_id): This function marks a specific block as allocated in the bitmap. It first checks if the pointer to the block store is not NULL and if the block_id is within the range of valid block indices. If the block is already marked as allocated, it returns false. Otherwise, it marks the block as allocated and checks that the block was indeed marked as allocated by testing the bitmap. It returns true if the block was successfully marked as allocated, false otherwise.
block_store_release(block_store_t *const bs, const size_t block_id): This function marks a specific block as free in the bitmap. It first checks if the pointer to the block store is not NULL and if the block_id is within the range of valid block indices. Then, it resets the bit corresponding to the block in the bitmap.
block_store_get_used_blocks(const block_store_t *const bs): This function returns the number of blocks that are currently allocated in the block store. It first checks if the pointer to the block store is not NULL and then uses the bitmap_total_set function to count the number of set bits in the bitmap.
block_store_get_free_blocks(const block_store_t *const bs): This function returns the number of blocks that are currently free in the block store. It first checks if the pointer to the block store is not NULL and then calculates the difference between the total number of blocks and the number of used blocks using the block_store_get_used_blocks and BLOCK_STORE_NUM_BLOCKS.
block_store_get_total_blocks(): This function returns the total number of blocks in the block store, which is defined by the BLOCK_STORE_NUM_BLOCKS.
block_store_read(const block_store_t *const bs, const size_t block_id, void *buffer): This function reads the contents of a block into a buffer. It returns the number of bytes successfully read.
block_store_write(block_store_t *const bs, const size_t block_id, const void *buffer): This function writes the contents of a buffer to a block. It returns the number of bytes successfully written.
block_store_deserialize(const char *const filename): This function deserializes a block store from a file. It returns a pointer to the resulting block_store_t struct. Implementing this function is OPTIONAL for this assignment.
block_store_serialize(const block_store_t *const bs, const char *const filename): This function serializes a block store to a file. It returns the size of the resulting file in bytes. Note: If a test case expects a specific number of bytes to be written but your file is smaller, pad the rest of the file with zeros until the file is of the expected size. Modify your block_store_deserialize function accordingly to accept padding if present (only if you implemented block_store_deserialize).

