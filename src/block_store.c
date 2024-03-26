#include <stdio.h>
#include <stdint.h>
#include "bitmap.h"
#include "block_store.h"
#include <string.h>
#include <errno.h>
// include more if you need

// You might find this handy.  I put it around unused parameters, but you should
// remove it before you submit. Just allows things to compile initially.
#define UNUSED(x) (void)(x)


typedef struct block_store
{
    bitmap_t *bit_map;
    uint8_t *data;
} block_store_t;

block_store_t *block_store_create()
{
    block_store_t *bs = malloc(sizeof(block_store_t)); // allocate memory for the block store

    bs->data = calloc(BLOCK_STORE_NUM_BLOCKS,BLOCK_SIZE_BYTES); // allocate memory for block data
    bs->bit_map= bitmap_overlay(BITMAP_SIZE_BITS, bs->data+BITMAP_START_BLOCK*BLOCK_SIZE_BYTES );
    for (int i = 0; i < BITMAP_NUM_BLOCKS; i++)
        block_store_request(bs, BITMAP_START_BLOCK+i);

    return bs; //returns new Block Storage device
}

// destroys a block store by freeing the memory allocated to it
void block_store_destroy(block_store_t *const bs)
{
    if(bs == NULL) {
        fprintf(stderr,"error = null pointer\n");  
        return;
    }     //checks if pointer is NULL

    bitmap_destroy(bs->bit_map);
    free(bs->data);
    free(bs); //frees block
}

size_t block_store_allocate(block_store_t *const bs)
{
    if (bs == NULL) {   
        fprintf(stderr,"error = null pointer\n");    
        return SIZE_MAX;  //returns SIZE_MAX if block is null
    }
    
    size_t index = bitmap_ffz(bs->bit_map);    // finds first free block in the block store

    if(index == SIZE_MAX){
        fprintf(stderr,"error = invailed block ID\n");    
        return SIZE_MAX; //returns SIZE_MAX if block is too small
    }

    bitmap_set(bs->bit_map, index);             //sets bitmap

    return index; //returns allocated block's id
}

bool block_store_request(block_store_t *const bs, const size_t block_id)
{
    if(bs == NULL){    
        fprintf(stderr,"error = null pointer\n");    
        return false; //returns false if block is null
    }
    if(block_id >= BITMAP_START_BLOCK+BITMAP_SIZE_BYTES){
        fprintf(stderr,"error = invailed block ID\n");    
        return false; //returns false if the block id greater than the available blocks
    }
    if(bitmap_test(bs->bit_map, block_id))
        return false; //returns false if block id is already set

    bitmap_set(bs->bit_map, block_id); //sets bitmap
    return true;
}

void block_store_release(block_store_t *const bs, const size_t block_id)
{
    if(bs == NULL){
        fprintf(stderr,"error = null pointer\n");    
        return;
    }
    if(block_id >= BITMAP_SIZE_BYTES ){
        fprintf(stderr,"error = invailed block ID\n");    
        return; //ends function if the block id is greater than the avaiable blocks
    }
    bitmap_reset(bs->bit_map, block_id); //Frees the specified block
}

size_t block_store_get_used_blocks(const block_store_t *const bs)
{
    if(bs == NULL){
        fprintf(stderr,"error = null pointer\n");    
        return SIZE_MAX; //returns SIZE_MAX if the block is null
    }
    return bitmap_total_set(bs->bit_map); //returns the number of blocks marked as in use
}

size_t block_store_get_free_blocks(const block_store_t *const bs)
{
    if(bs == NULL){
        fprintf(stderr,"error = null pointer\n");    
        return SIZE_MAX; //returns SIZE_MAX if the block is null
    }
    return BLOCK_STORE_NUM_BLOCKS-block_store_get_used_blocks(bs); //returns the number of blocks marked free for use
}

size_t block_store_get_total_blocks()
{
    return BLOCK_STORE_NUM_BLOCKS; //Returns the total number of user-addressable blocks
}

size_t block_store_read(const block_store_t *const bs, const size_t block_id, void *buffer)
{
    if(bs == NULL || buffer == NULL){ //returns 0 if the block or buffer is null
        fprintf(stderr,"error = null pointer\n");
        return 0;
    }
    UNUSED(block_id);
    size_t size = 0;
    if(bitmap_test(bs->bit_map, block_id)){ //if the bipmap is initialized
        size = BLOCK_SIZE_BYTES;
        memcpy(buffer,&bs->data[block_id],size);
    }
    return size; //reads data from the specified block and writes it to the designated buffer, returns 0 upon error
}

size_t block_store_write(block_store_t *const bs, const size_t block_id, const void *buffer)
{
    if(bs == NULL || buffer == NULL){ //returns 0 if the block or buffer is null
        fprintf(stderr,"error = null pointer\n");
        return 0;
    }

    memcpy(&bs->data[block_id],buffer,BLOCK_SIZE_BYTES);
    return BLOCK_SIZE_BYTES; //reads data from the specified buffer and writes it to the designated block
}

block_store_t *block_store_deserialize(const char *const filename)
{
    if(filename == NULL ){ //returns null if the filename is null
        fprintf(stderr,"error = invalid file name\n");
        return 0;
    }
    
    FILE * f = fopen(filename, "rb"); 
    if (f == NULL){ //returns null if the file cannot be edited
        perror(strerror(errno));
        return 0;
    }

    block_store_t *bs = block_store_create();
    if(bs == NULL){ //returns null pointer is null
        fprintf(stderr,"error = null pointer to block_store\n");
        fclose(f);
        return 0;
    }
    
    fread(bs->data, sizeof(bs->data[0]), BLOCK_STORE_NUM_BYTES, f);
    fclose(f);

    return bs; //returns pointer to the new Block Storage device
}

size_t block_store_serialize(const block_store_t *const bs, const char *const filename)
{
    if(filename == NULL ){ //returns null if the filename is null
        fprintf(stderr,"error = invalid file name\n");
        return 0;
    }
    
    if(bs == NULL ){ //returns null if the block pointer is null
        fprintf(stderr,"error = null pointer to block_store\n");
        return 0;
    }

    FILE * f = fopen(filename, "wb");
    if (f == NULL){ //returns null if the file cannot be edited
        perror(strerror(errno));
        return 0;
    }
    
    fwrite(bs->data, sizeof(bs->data[0]), BLOCK_STORE_NUM_BYTES, f);
    fclose(f);

    return BLOCK_STORE_NUM_BYTES; //writes the entirety of the BS device to file or overwriting existing BS device
}