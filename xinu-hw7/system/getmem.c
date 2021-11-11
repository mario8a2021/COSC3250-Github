/**
 * COSC 3250 - Project 7
 * This program will allocate memory for processes.
 * @authors Mario Ochoa, Jacqueline Gutierrez
 * Instructor Sabirat Rubya
 * TA-BOT:MAILTO mario.ochoa@marquette.edu, jacqueline.gutierrez@marquette.edu
 */

/**
 * @file getmem.c
 *
 */
/* Embedded Xinu, Copyright (C) 2009.  All rights reserved. */

#include <xinu.h>

/**
 * @ingroup memory_mgmt
 *
 * Allocate heap memory.
 *
 * @param nbytes
 *      Number of bytes requested.
 *
 * @return
 *      ::SYSERR if @p nbytes was 0 or there is no memory to satisfy the
 *      request; otherwise returns a pointer to the allocated memory region.
 *      The returned pointer is guaranteed to be 8-byte aligned.  Free the block
 *      with memfree() when done with it.
 */
void *getmem(ulong nbytes)
{
    register memblk *prev, *curr, *leftover;
    irqmask im;

    if (0 == nbytes)
    {
        return (void *)SYSERR;
    }

    /* round to multiple of memblock size   */
    nbytes = (ulong)roundmb(nbytes);

    im = disable();

	/* TODO:
     *      - Use cpuid to use correct freelist
     *           ex: freelist[cpuid]
     *      - Acquire memory lock (memlock)
     *      - Traverse through the freelist
     *        to find a block that's suitable 
     *        (Use First Fit with simple compaction)
     *      - Release memory lock
     *      - return memory address if successful
     */

    uint cpuid = getcpuid();
    curr = freelist[cpuid].head; //curr
    prev = &freelist;

    while(curr != NULL){

	if(curr->length < nbytes){
		prev = curr;
		curr = curr->next;

	}

	else if(curr->length == nbytes){
		prev->next = curr->next;
		freelist[cpuid].length = freelist[cpuid].length - nbytes;		
		curr->next = curr;
		lock_release(freelist[cpuid].memlock);	
		restore(im);

		return (void *) curr;
	}
	else if(curr->length > nbytes ){
		leftover = (struct memblock *)((uint)curr + nbytes); 
		leftover->next = curr->next;
		leftover->length = freelist[cpuid].length - nbytes;

		prev->next = leftover;
		freelist[cpuid].length = freelist[cpuid].length - nbytes;
		curr->next = curr;
		lock_release(freelist[cpuid].memlock);
		restore(im);
	
		return (void *) curr; 

	}



    } 



    restore(im);
    return (void *)SYSERR;
}
