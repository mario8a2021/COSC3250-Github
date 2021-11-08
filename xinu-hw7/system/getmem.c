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
    currentCpuId = freelist[cpuid].head;
    previousCpuId = &freelist;

    while(currentCpuId != NULL){

	if(currentCpuId->length < nbytes){
		previousCpuId = currentCpuId;
		currentCpuId = currentCpuId->next;

	}

	else if(currentCpuId->length == nbytes){
		previousCpuId->next = currentCpuId->next;
		freelist[cpuid].length = freelist[cpuid].length - nbytes;		
		currentCpuId->next = currentCpuId;
		lock_release(freelist[cpuid].memlock);	
		restore(im);

		return (void *) currentCpuId;
	}
	else if(currentCpuId->length > nbytes ){
		accessBytes = (struct memblock *)((uint)curr + nbytes); 
		accessBytes->next = currentCpuId->next;
		accessBytes->length = freelist[cpuid].length - nbytes;

		previousCpuId->next = accessBytes;
		freelist[cpuid].length = freelist[cpuid].length - nbytes;
		currentCpuId->next = currentCpuId;
		lock_release(freelist[cpuid].memlock);
		restore(im);
	
		return (void *) currentCpuId; 

	}



    } 



    restore(im);
    return (void *)SYSERR;
}
