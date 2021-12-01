/**
 * COSC 3250 - Project 9
 * This file contains testcases to test our semaphore files
 * @authors Mario Ochoa, Jacqueline Gutierrez
 * Instructor Sabirat Rubya
 * TA-BOT:MAILTO mario.ochoa@marquette.edu, jacqueline.gutierrez@marquette.edu
 */




/**
 * @file testcases.c
 * @provides testcases
 *
 *
 * Modified by:	
 *
 * TA-BOT:MAILTO 
 *
 */
/* Embedded XINU, Copyright (C) 2007.  All rights reserved. */

#include <xinu.h>

/* Function prototypes */
void print_sem(semaphore sem);
uchar getc(void);
void putc(uchar);


/* Test process to use for testing semaphores. */
process testSemWait(semaphore sem)
{
    wait(sem);
    printf("process %d after wait()\r\n", currpid[getcpuid()]);
    return 0;
}

/**
 * testcases - called after initialization completes to test things.
 */
void testcases(void)
{
    uchar c;
    semaphore testsem;

    enable();

    printf("===TEST BEGIN===\r\n");
    
	// TODO: Test your operating system!
    c = getc();
    switch (c)
    {
    case 'a':;
	//printf("starting \r\n");
	//pid = create((void*) testsemwait, INITSTK, PRIORITY_HIGH, "SEM 1", testsm);
	uint pid = create((void*) testSemWait, INITSTK, PRIORITY_HIGH, "SEM 1", testsem);
	//printf("pid created \r\n");
	//uint pid2 = create((void*) testSemWait, INITSTK, PRIORITY_HIGH, "SEM 2", testsem);
	
	//print_sem(testsem);

	//wait(testsem);

	//print_sem(testsem);

	//signal(testsem);

	//new test sem wait proc
	//ready pid1
	//print sme pid1
	
	//ready(testsemwait(testsem), RESCHED_YES,1)
	//ready(testSemWait(testsem), RESCHED_YES,1);
	//printf("starting to ready pid \r\n");
	ready(testSemWait(testsem), RESCHED_YES,1);
	//printf("ready done \r\n");
	print_sem(testsem);

	wait(testsem);
	print_sem(testsem);
	signal(testsem);
	print_sem(testsem);
	printf("testing done \r\n");
//	break;
	
    //case 'b':
	//uint pid2 = create((void*) testSemWait, INITSTK, PRIORITY_HIGH, "SEM 1", testsem);
	
	//wait(testsem);
	
	//semfree(testsem);
	
	//ready(testSemWait(testsem), RESCHED_YES,1);

	//print_sem(testsem);
       
    default:
        break;
    }

    while (numproc > 5)  // Four null procs plus this test proc.
    {
        resched();
    }

    printf("\r\n===TEST END===\r\n");
    printf("\r\n\r\nAll user processes have completed.\r\n\r\n");

    while (1);
    return;
}

void print_sem(semaphore sem)
{
    struct sement *semptr;
    semptr = &semtab[sem];

    printf("\r\n%d->state: %s\r\n", sem,
           (semptr->state == 0x01) ? "SFREE" : "SUSED");
    printf("%d->count: %d\r\n", sem, semptr->count);
    printf("%d->queue: %s\r\n\r\n", sem,
           (isempty(semptr->queue)) ? "EMPTY" : "NONEMPTY");
}
