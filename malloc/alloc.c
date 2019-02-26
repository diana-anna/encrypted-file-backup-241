/**
 * Malloc Lab
 * CS 241 - Spring 2019
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// struct
typedef struct meta_data {
    size_t size; // size of data ONLY
    struct meta_data* next;
    struct meta_data* prev;
    void* data;
    int is_allocated;
} meta_data;

static meta_data* head = NULL;
static int heap_initialized = 0;

void heap_init() {
    void* brk = sbrk(sizeof(meta_data) + 8); // idk it seems ok
    head = (meta_data*) brk; // head points to first md
    head->next = NULL;
    head->prev = NULL;
    head->size = 8;
    head->is_allocated = 0;
    heap_initialized = 1;
}

// combines a block with its next and/or prev if they are free
void coalesce(meta_data* addr) { // ASK ABOUT SIZE CHANGES
    if (!addr->next->is_allocated) { // merge current and next
        meta_data* neighbor = addr->next;

        // size of 1 and 2 + size of 2's meta_data
        addr->size += neighbor->size + sizeof(neighbor);
        meta_data* old_neighbor_next = neighbor->next;
        addr->next = old_neighbor_next;
        old_neighbor_next->prev = addr;
    }
    if (!addr->prev->is_allocated) { // merge current and prev
        meta_data* neighbor = addr->prev;    

    // size of 1 and 0 + size of 1's meta_data
        neighbor->size += addr->size + sizeof(addr);
        meta_data* old_addr_next = addr->next;
        neighbor->next = old_addr_next;
        old_addr_next->prev = neighbor;
    }
}

// splits a block that has been partially allocd
void split(meta_data* addr, size_t size_allocd) {
    meta_data* temp = addr;
    temp += sizeof(addr) + size_allocd; // marks beginning of unallocated block
    //meta_data* end_of_block = temp;
    //while (!end_of_block->is_allocated) {
    //    end_of_block = end_of_block++;
    //} // marks the end of the unallocated block
    //end_of_block--;
    temp->size = addr->size - size_allocd;
    addr->size = size_allocd;
    temp->is_allocated = 0;
}

// returns pointer to end of LL
meta_data* get_end() {
    if (!head) return NULL;
    meta_data* temp = head;
    while (temp->next) {
        temp = temp->next;
    }

    return temp;
}

// returns pointer to ptr's metadata address
meta_data* meta_data_from_ptr(void* ptr) {
    if (!head) return NULL;
    meta_data* temp = head;
    while (temp) {
        if (temp->data == ptr) {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

// returns pointer to first meta_data with size >= requested
meta_data* meta_data_from_size(size_t size) {
    if (!head) return NULL;

    meta_data* temp = head;
    while(temp) {
        if (!temp->is_allocated && temp->size == size) {
            return temp;
        } else if (!temp->is_allocated && temp->size > size) {
            split(temp, size);
            return temp;
        }
        temp = temp->next;
    }
    
    // not enough space -> sbrk
// should be meta_data* ?????idk
    void* brk = sbrk(sizeof(meta_data) + (2 * size)); // idk just be safe?
    if (brk == (void*) -1) return NULL; // not enough memory :(((
    meta_data* new_md = (meta_data*) brk;
    new_md->size = 2 * size;
    new_md->is_allocated = 0;
    new_md->next = NULL;
    meta_data* new_md_prev = get_end();
    new_md_prev->next = new_md;
    new_md->prev = new_md_prev;
    
    return meta_data_from_size(size);
}

/**
 * Allocate space for array in memory
 *
 * Allocates a block of memory for an array of num elements, each of them size
 * bytes long, and initializes all its bits to zero. The effective result is
 * the allocation of an zero-initialized memory block of (num * size) bytes.
 *
 * @param num
 *    Number of elements to be allocated.
 * @param size
 *    Size of elements.
 *
 * @return
 *    A pointer to the memory block allocated by the function.
 *
 *    The type of this pointer is always void*, which can be cast to the
 *    desired type of data pointer in order to be dereferenceable.
 *
 *    If the function failed to allocate the requested block of memory, a
 *    NULL pointer is returned.
 *
 * @see http://www.cplusplus.com/reference/clibrary/cstdlib/calloc/
 */
void *calloc(size_t num, size_t size) {
    // implement calloc!
    meta_data* temp = malloc(size * num);
    if (!temp) return NULL;
    memset(temp, 0, num * size);
    return temp;
}

/**
 * Allocate memory block
 *
 * Allocates a block of size bytes of memory, returning a pointer to the
 * beginning of the block.  The content of the newly allocated block of
 * memory is not initialized, remaining with indeterminate values.
 *
 * @param size
 *    Size of the memory block, in bytes.
 *
 * @return
 *    On success, a pointer to the memory block allocated by the function.
 *
 *    The type of this pointer is always void*, which can be cast to the
 *    desired type of data pointer in order to be dereferenceable.
 *
 *    If the function failed to allocate the requested block of memory,
 *    a null pointer is returned.
 *
 * @see http://www.cplusplus.com/reference/clibrary/cstdlib/malloc/
 */
void *malloc(size_t size) {
    // implement malloc!
    if (!heap_initialized) heap_init();

    meta_data* md = meta_data_from_size(size);
    if (md == NULL) return NULL;

    return md;
}

/**
 * Deallocate space in memory
 *
 * A block of memory previously allocated using a call to malloc(),
 * calloc() or realloc() is deallocated, making it available again for
 * further allocations.
 *
 * Notice that this function leaves the value of ptr unchanged, hence
 * it still points to the same (now invalid) location, and not to the
 * null pointer.
 *
 * @param ptr
 *    Pointer to a memory block previously allocated with malloc(),
 *    calloc() or realloc() to be deallocated.  If a null pointer is
 *    passed as argument, no action occurs.
 */
void free(void *ptr) {
    // implement free!
    meta_data* md = meta_data_from_ptr(ptr); // check return type
    if (!md) return;
    
    md->is_allocated = 0;
    coalesce(md);
}

/**
 * Reallocate memory block
 *
 * The size of the memory block pointed to by the ptr parameter is changed
 * to the size bytes, expanding or reducing the amount of memory available
 * in the block.
 *
 * The function may move the memory block to a new location, in which case
 * the new location is returned. The content of the memory block is preserved
 * up to the lesser of the new and old sizes, even if the block is moved. If
 * the new size is larger, the value of the newly allocated portion is
 * indeterminate.
 *
 * In case that ptr is NULL, the function behaves exactly as malloc, assigning
 * a new block of size bytes and returning a pointer to the beginning of it.
 *
 * In case that the size is 0, the memory previously allocated in ptr is
 * deallocated as if a call to free was made, and a NULL pointer is returned.
 *
 * @param ptr
 *    Pointer to a memory block previously allocated with malloc(), calloc()
 *    or realloc() to be reallocated.
 *
 *    If this is NULL, a new block is allocated and a pointer to it is
 *    returned by the function.
 *
 * @param size
 *    New size for the memory block, in bytes.
 *
 *    If it is 0 and ptr points to an existing block of memory, the memory
 *    block pointed by ptr is deallocated and a NULL pointer is returned.
 *
 * @return
 *    A pointer to the reallocated memory block, which may be either the
 *    same as the ptr argument or a new location.
 *
 *    The type of this pointer is void*, which can be cast to the desired
 *    type of data pointer in order to be dereferenceable.
 *
 *    If the function failed to allocate the requested block of memory,
 *    a NULL pointer is returned, and the memory block pointed to by
 *    argument ptr is left unchanged.
 *
 * @see http://www.cplusplus.com/reference/clibrary/cstdlib/realloc/
 */
void *realloc(void *ptr, size_t size) {
    // implement realloc!
    if (!ptr) return malloc(size);

    if (!size) {
        free(ptr);
        return NULL;
    }

    meta_data* md = (meta_data*) ptr;
    meta_data* temp = malloc(size);

    if (size > md->size) {
        memcpy(temp, md, md->size);
        return temp;
    }

    memcpy(temp, md, size);
    return temp;
}
