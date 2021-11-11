/**
 * COSC 3250 - Project 7
 * This program allocates memory for new processes.
 * @authors Mario Ochoa, Jacqueline Gutierrez
 * Instructor Sabirat Rubya
 * TA-BOT:MAILTO mario.ochoa@marquette.edu, jacqueline.gutierrez@marquette.edu
 */


/**
 * @file malloc.c
 * This file is deprecated in favor of mem/malloc.c and the user heap
 * allocator.  However, it remains here for backup purposes.
 */
/* Embedded Xinu, Copyright (C) 2009, 2013.  All rights reserved. */

#include <xinu.h>

/**
 * @ingroup libxc
 *
 * Request heap storage, record accounting information, returning pointer
 * to assigned memory region.
 *
 * @param size
 *      number of bytes requested
 *
 * @return
 *      pointer to region on success, or @c NULL on failure
 */
void *malloc(ulong size)
{
    struct memblock *pmem;

    /* we don't allocate 0 bytes. */
    if (0 == size)
    {
        return NULL;
    }

	/** TODO:
      *      1) Make room for accounting info
      *      2) Acquire memory with getmem
      *         - handle possible error (SYSERR) from getmem...
      *      3) Set accounting info in pmem
      */  

    size = size+sizeof(struct memblock);//
    pmem= getmem(size);
    if (pmem == SYSERR){
         return NULL;		
    }
    else{
	pmem->next = pmem;
	pmem->length = size;
    }

    return (void *)(pmem + 1);  /* +1 to skip accounting info */
}
