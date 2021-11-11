/**
 * COSC 3250 - Project 7
 * This program will free memory and make it avalible for other processes.
 * @authors Mario Ochoa, Jacqueline Gutierrez
 * Instructor Sabirat Rubya
 * TA-BOT:MAILTO mario.ochoa@marquette.edu, jacqueline.gutierrez@marquette.edu
 */


/**
 * @file free.c
 */
/* Embedded Xinu, Copyright (C) 2009, 2013.  All rights reserved. */

#include <xinu.h>

/**
 * @ingroup libxc
 *
 * Attempt to free a block of memory based on malloc() accounting information
 * stored in preceding two words.
 *
 * @param ptr
 *      A pointer to the memory block to free.
 */
syscall free(void *ptr)
{
    struct memblock *block;

	/* TODO:
     *      1) set block to point to memblock to be free'd (ptr)
     *      2) find accounting information of the memblock
     *      3) call freemem on the block with its length
     */

   
    block =(struct memblock*) ptr;

    block--;

    if (block->next != block){
        return SYSERR;
    } 
    freemem(block, block->length);




    return OK;
}
