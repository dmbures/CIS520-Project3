#include <stdio.h>
#include <stdint.h>
#include "bitmap.h"
#include "block_store.h"
// include more if you need

// You might find this handy.  I put it around unused parameters, but you should
// remove it before you submit. Just allows things to compile initially.
#define UNUSED(x) (void)(x)


typedef struct block_store
{
   bitmap_t *bit_map;
    uint8_t *data;
    size_t used;
} block_store_t;

block_store_t *block_store_create()
{
    block_store_t *bs = malloc(sizeof(block_store_t)); // allocate memory for the block store
    bs->bit_map = bitmap_create(BLOCK_STORE_NUM_BLOCKS); //creates bitmap

    bitmap_set(bs->bit_map, 127); //sets bitmap
    bs->data = calloc(1, BLOCK_SIZE_BYTES * BLOCK_STORE_NUM_BLOCKS);

    return bs;
}

// destroys a block store by freeing the memory allocated to it
void block_store_destroy(block_store_t *const bs)
{
    if(bs == NULL) {
        return;
    }     //checks if pointer is NULL

    bitmap_destroy(bs->bit_map);
    free(bs->data);
    free(bs); //frees block
}
size_t block_store_allocate(block_store_t *const bs)
{
    if (bs == NULL)
    {
        return SIZE_MAX;  //returns SIZE_MAX if block is null
    }

    size_t index = bitmap_ffs(bs->bit_map);    // finds first free block in the block store
    
    if(index > BLOCK_STORE_NUM_BLOCKS - 1){
        return SIZE_MAX; //returns SIZE_MAX if block is too small
    }

    bitmap_set(bs->bit_map, index);             //sets bitmap
    bs->used++; //makrs block as used

    if(index >= 128){
        index--; //decrements
    }

    return index; //returns allocated block's id
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