/**
 * @file sendnow.c
 * @provides sendnow.
 *
 */
/* Embedded Xinu, Copyright (C) 2020.   All rights reserved. */

#include <xinu.h>

/**
 * Send a message to anoher thread
 * @param pid proc id of recipient
 * @param msg contents of message
 * @return OK on sucess, SYSERR on failure
 */

syscall sendnow(int pid, message msg)
{
	register pcb *ppcb;

	/* TODO:
 	* - Acquire Lock and release when appropriate
 	* - PID & Process Error Checking
 	* - Deposit Message, raise flag
 	* -  If receving message is blocked, ready process
 	* - Return OK
 	*/
	
	ppcb = &proctab[pid];	

	lock_acquire(ppcb->msg_var.core_com_lock);

	if (isbadpid(pid)){
		
		lock_release(ppcb->msg_var.core_com_lock);
		return SYSERR;
	}

	if (ppcb->state == PRFREE){
		
		lock_release(ppcb->msg_var.core_com_lock);
		return SYSERR;
	}

	unparkcore(getcpuid(), ppcb, NULL);	//wake it up?

	ppcb->msg_var.msgin = msg;
	ppcb->msg_var.hasMessage = 1;

	if (ppcb->state == PRRECV){
		ready(pid, RESCHED_YES, ppcb->core_affinity);

	}
		
	//ppcb->msg_var.msgin = -1;
	lock_release(ppcb->msg_var.core_com_lock);

	return OK;
}
