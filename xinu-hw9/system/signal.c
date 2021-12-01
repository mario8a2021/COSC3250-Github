
/**
 * COSC 3250 - Project 9
 * signal.c releases one thread that has been put into the wait state
 * @authors Mario Ochoa, Jacqueline Gutierrez
 * Instructor Sabirat Rubya
 * TA-BOT:MAILTO mario.ochoa@marquette.edu, jacqueline.gutierrez@marquette.edu
 */

/**
 * @file signal.c
 *
 */
/* Embedded Xinu, Copyright (C) 2009.  All rights reserved. */

#include <xinu.h>

/**
 * @ingroup semaphores
 *
 * Signal a semaphore, releasing up to one waiting thread.
 *
 * signal() may reschedule the currently running thread.  As a result, signal()
 * should not be called from non-reentrant interrupt handlers unless ::resdefer
 * is set to a positive value at the start of the interrupt handler.
 *
 * @param sem
 *      Semaphore to signal.
 *
 * @return
 *      ::OK on success, ::SYSERR on failure.  This function can only fail if @p
 *      sem did not specify a valid semaphore.
 */
syscall signal(semaphore sem)
{
    register struct sement *semptr;
    int count;
    irqmask im;

    im = disable();
    if (isbadsem(sem))
    {
        restore(im);
        return SYSERR;
    }

    semptr = &semtab[sem];

    /* TODO:
     * Make this incrementation atomically-safe
     * so that multiple cores may safely signal a semaphore.
     */
    	count = _atomic_increment(&semptr->count); //atomically incrementing

	//count = ++semptr->count;

    if (count <= 0)
    {
        ready(dequeue(semptr->queue), RESCHED_NO, getcpuid());
    }

    udelay(3);
    restore(im);

    return OK;
}
