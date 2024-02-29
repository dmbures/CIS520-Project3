#include <stdio.h>
#include <stdint.h>
#include "bitmap.h"
#include "block_store.h"
// include more if you need

// You might find this handy.  I put it around unused parameters, but you should
// remove it before you submit. Just allows things to compile initially.
#define UNUSED(x) (void)(x)


struct block_store 
{
    block_store_t *bit_map;
};

block_store_t *block_store_create()
{
    block_store_t *bs;                              // allocate memory for the block store
    memset(bs, 0, sizeof(block_store_t));           // initializes it to zeros using memset

    bs->bit_map = bitmap_initialize(BITMAP_SIZE_BYTES, 0 /*NONE in the BITMAP_FLAGS enum*/);
    
    // sets the bitmap field of the block store to an index starting at BITMAP_START_Block     (What is this?)   

    // mark the blocks used by the bitmap as allocated using the  block_store_request funciton    
    block_store_request(bs, 0 /*const size_t block_id*/);      // (this funciton will be implemented in checkpoint 2; also I have not idea what to pass for block_id)
    return bs;
}

// destroys a block store by freeing the memory allocated to it
void block_store_destroy(block_store_t *const bs)
{
    if(bs == NULL)      //checks if pointer is NULL
        return;

    free(bs->bit_map);
    free(bs);
}
size_t block_store_allocate(block_store_t *const bs)
{
    size_t index = bitmap_ffs(bs->bit_map);    // finds first free block in the block store
    bitmap_set(bs->bit_map, index);             //marks it as allocated in the bitmap

    return index;
}

bool block_store_request(block_store_t *const bs, const size_t block_id)
{
    UNUSED(bs);
    UNUSED(block_id);
    return false;
}

void block_store_release(block_store_t *const bs, const size_t block_id)
{
    UNUSED(bs);
    UNUSED(block_id);
}

size_t block_store_get_used_blocks(const block_store_t *const bs)
{
    UNUSED(bs);
    return 0;
}

size_t block_store_get_free_blocks(const block_store_t *const bs)
{
    UNUSED(bs);
    return 0;
}

size_t block_store_get_total_blocks()
{
    return 0;
}

size_t block_store_read(const block_store_t *const bs, const size_t block_id, void *buffer)
{
    UNUSED(bs);
    UNUSED(block_id);
    UNUSED(buffer);
    return 0;
}

size_t block_store_write(block_store_t *const bs, const size_t block_id, const void *buffer)
{
    UNUSED(bs);
    UNUSED(block_id);
    UNUSED(buffer);
    return 0;
}

block_store_t *block_store_deserialize(const char *const filename)
{
    UNUSED(filename);
    return NULL;
}

size_t block_store_serialize(const block_store_t *const bs, const char *const filename)
{
    UNUSED(bs);
    UNUSED(filename);
    return 0;
}