/**
 * COSC 3250 - Project 5
 * This program would be able to create processes
 * @authors Mario Ochoa, Jacqueline Gutierrez
 * Instructor Sabirat Rubya 
 * TA-BOT:MAILTO mario.ochoa@marquette.edu, jacqueline.gutierrez@marquette.edu
 */

/**
 * @file create.c
 * @provides create, newpid, userret
 *
 * COSC 3250 / COEN 4820 Assignment 4
 */
/* Embedded XINU, Copyright (C) 2008.  All rights reserved. */

#include <arm.h>
#include <xinu.h>

/* Assembly routine for atomic operations */
extern int _atomic_increment_post(int *);
extern int _atomic_increment_limit(int *, int);

static pid_typ newpid(void);
void userret(void);
void *getstk(ulong);

/**
 * Create a new process to start running a function.
 * @param funcaddr address of function that will begin in new process
 * @param ssize    stack size in bytes
 * @param name     name of the process, used for debugging
 * @param nargs    number of arguments that follow
 * @return the new process id
 */

syscall create(void *funcaddr, ulong ssize, ulong priority ,char *name, ulong nargs, ...)
{
	ulong *saddr;               /* stack address                */
	ulong pid;     		    /* stores new process id        */	
	pcb *ppcb;                  /* pointer to proc control blk  */
	ulong i;
	va_list ap;                 /* points to list of var args   */
	ulong pads = 0;             /* padding entries in record.   */
	void INITRET(void);

	if (ssize < MINSTK)
		ssize = MINSTK;
	ssize = (ulong)(ssize + 3) & 0xFFFFFFFC;
	/* round up to even boundary    */
	saddr = (ulong *)getstk(ssize);     /* allocate new stack and pid   */
	pid = newpid();
	/* a little error checking      */
	if ((((ulong *)SYSERR) == saddr) || (SYSERR == pid))
	{
		return SYSERR;
	}

	_atomic_increment_post(&numproc);

	ppcb = &proctab[pid];
	/* setup PCB entry for new proc */
	ppcb->state = PRSUSP;

	// TODO: Setup PCB entry for new process.

	//-------------------------	
	ppcb->core_affinity = -1;
	ppcb->stklen = ssize;
	ppcb-> stkbase = saddr;
	ppcb-> priority = priority;
	strncpy(ppcb->name,name,PNMLEN);
	//-----------------------

	//strncpy (look up function)


	/* Initialize stack with accounting block. */
	*saddr = STACKMAGIC;
	*--saddr = pid;
	*--saddr = ppcb->stklen;
	*--saddr = (ulong)ppcb->stkbase;

	/* Handle variable number of arguments passed to starting function   */
	if (nargs)
	{
		pads = ((nargs - 1) / 4) * 4;
	}
	/* If more than 4 args, pad record size to multiple of native memory */
	/*  transfer size.  Reserve space for extra args                     */
	for (i = 0; i < pads; i++)
	{
		*--saddr = 0;
	}

	// TODO: Initialize process context.
	//---------------------------------
	ppcb->regs[PREG_R0] = 0;
	ppcb->regs[PREG_R1] = 0;   
        ppcb->regs[PREG_R2] = 0;
        ppcb->regs[PREG_R3] = 0;
        ppcb->regs[PREG_R4] = 0;
        ppcb->regs[PREG_R5] = 0;
        ppcb->regs[PREG_R6] = 0;
        ppcb->regs[PREG_R7] = 0;   
	ppcb->regs[PREG_R8] = 0;
        ppcb->regs[PREG_R9] = 0;
        ppcb->regs[PREG_R10] = 0;
        ppcb->regs[PREG_R11] = 0;
        ppcb->regs[PREG_IP] = 0;
	ppcb->regs[PREG_SP] = (int) saddr;	
	
	//---------------------------------
	
	// TODO:  Place arguments into activation record.
	//        See K&R 7.3 for example using va_start, va_arg and
	//        va_end macros for variable argument functions.

	//store first 4 args in r0-r3
	//store the rest of the args in stack

	//---------------------------------	
	
	va_start(ap,nargs);
	
	int a;
	int b;
	if(nargs <= 4){ 
		b = nargs;
	}
	else { 
		b = 4;
	}

	//int b;
	for(a = 0; a < b;a++){
		ppcb->regs[a] = va_arg(ap,int);
	}	

	if (nargs > 4){
		for(b = 4; b < nargs; b++ ){
                	*(saddr + b - 4) = va_arg(ap,int);
        	}
	}
	
	ppcb->regs[PREG_LR] = (int) userret;
	ppcb->regs[PREG_PC] = (int) funcaddr;
	va_end(ap);
	
	//--------------------------------
	return pid;
}

/**
 * Obtain a new (free) process id.
 * @return a free process id, SYSERR if all ids are used
 */
static pid_typ newpid(void)
{
	pid_typ pid;                /* process id to return     */
	static pid_typ nextpid = 0;

	for (pid = 0; pid < NPROC; pid++)
	{                           /* check all NPROC slots    */
		//        nextpid = (nextpid + 1) % NPROC;
		_atomic_increment_limit(&nextpid, NPROC);
		if (PRFREE == proctab[nextpid].state)
		{
			return nextpid;
		}
	}
	return SYSERR;
}

/**
 * Entered when a process exits by return.
 */
void userret(void)
{
	uint cpuid = getcpuid();
	kill(currpid[cpuid]);
}
