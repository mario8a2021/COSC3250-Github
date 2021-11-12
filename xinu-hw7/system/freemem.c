/**
 * COSC 3250 - Project 7
 * This program will free allocated memory that is stored with the memblock structure.
 * @authors Mario Ochoa, Jacqueline Gutierrez
 * Instructor Sabirat Rubya
 * TA-BOT:MAILTO mario.ochoa@marquette.edu, jacqueline.gutierrez@marquette.edu
 */


/**
 * @file freemem.c
 *
 */
/* Embedded Xinu, Copyright (C) 2009.  All rights reserved. */

#include <xinu.h>

/**
 * @ingroup memory_mgmt
 *
 * Frees a block of heap-allocated memory.
 *
 * @param memptr
 *      Pointer to memory block allocated with memget().
 *
 * @param nbytes
 *      Length of memory block, in bytes.  (Same value passed to memget().)
 *
 * @return
 *      ::OK on success; ::SYSERR on failure.  This function can only fail
 *      because of memory corruption or specifying an invalid memory block.
 */
syscall freemem(void *memptr, ulong nbytes)
{
    register struct memblock *block, *next, *prev;
    irqmask im;
    ulong top;

    /* make sure block is in heap */
    if ((0 == nbytes)
        || ((ulong)memptr < (ulong)memheap)
        || ((ulong)memptr > (ulong)platform.maxaddr))
    {
        return SYSERR;
    }

    block = (struct memblock *)memptr;
    nbytes = (ulong)roundmb(nbytes);

    im = disable();

	/* TODO:
     *      - Determine correct freelist to return to
     *        based on block address
     *      - Acquire memory lock (memlock)
     *      - Find where the memory block should
     *        go back onto the freelist (based on address)
     *      - Find top of previous memblock
     *      - Make sure block is not overlapping on prev or next blocks
     *      - Coalesce with previous block if adjacent
     *      - Coalesce with next block if adjacent
     */
    
    int core = -1;

    for(int i = 0; i < NCORES; i++){
	if(  (ulong)memptr <= (freelist[i].base + freelist[i].bound) && (ulong)memptr >= freelist[i].base ){
		core = i;
		//kprintf("%d",core);
	}
    }

    lock_acquire(freelist[core].memlock);
    prev = NULL; 
    next = freelist[core].head;

    while(next != NULL && next < block){
	prev = next;
        next = next->next;    	
    }

    if ( (ulong) prev == (ulong)&freelist[core]  ){
	top = NULL;	
    }

    else {
	top = (ulong)prev + prev->length;
    }

    freelist[core].length = freelist[core].length + nbytes;    

    if (top == (ulong) block){
	prev->length = prev->length + nbytes;
        block = prev;
    }

    else{

    block->length = nbytes;
    block->next = next;
    prev->next = block;

    }

    if ( (ulong)block + block->length == (ulong)next ) {

	block->length = block->length + next->length;
	block->next = next->next;
    
    }

    lock_release(freelist[core].memlock);
  
    restore(im);
    return OK;
}




