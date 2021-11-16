/**
 * COSC 3250 - Project 3
 * This program is a synchronous serial driver that allow the operating system to communicate with our I/O device
 * @authors Mario Ochoa, Jacqueline Gutierrez
 * Instructor Sabirat Rubya 
 * TA-BOT:MAILTO mario.ochoa@marquette.edu jacqueline.gutierrez@marquette.edu 
 */


/**
 * @file kprintf.c
 */

/* Embedded Xinu, Copyright (C) 2009, 2013.  All rights reserved. */

#include <xinu.h>
#include <spinlock.h>

#define UNGETMAX 10             /* Can un-get at most 10 characters. */

static unsigned char ungetArray[UNGETMAX];

/**
 * Synchronously read a character from a UART.  This blocks until a character is
 * available.  The interrupt handler is not used.
 *
 * @return
 *      The character read from the UART as an <code>unsigned char</code> cast
 *      to an <code>int</code>.
 */

static int i = 0;

syscall kgetc(void)
{
    
    volatile struct pl011_uart_csreg *regptr;

    /* Pointer to the UART control and status registers.  */
    regptr = (struct pl011_uart_csreg *)0x3F201000;

    char c = 0;
    int flagptr = 0;

    if ( i > 0) {
	c = ungetArray[i-1];
	i--;	     
    }
    
   do{ 

    flagptr = regptr->fr; //flag register

    }while(flagptr & PL011_FR_RXFE);
   

    c =  regptr->dr;   //data register  
    
    return c;


    // TODO: First, check the unget buffer for a character.
    //       Otherwise, check UART flags register, and
    //       once the receiver is not empty, get character c.

    //return SYSERR;
}






/**
 * kcheckc - check to see if a character is available.
 * @return true if a character is available, false otherwise.
 */
syscall kcheckc(void)
{
    volatile struct pl011_uart_csreg *regptr;
    regptr = (struct pl011_uart_csreg *)0x3F201000;

    if ((i == 0) && (regptr->fr & PL011_FR_RXFE)){
	return 0;
    }
    else{
	return 1;
    }
          

    // TODO: Check the unget buffer and the UART for characters.

    return SYSERR;
}






/**
 * kungetc - put a serial character "back" into a local buffer.
 * @param c character to unget.
 * @return c on success, SYSERR on failure.
 */
syscall kungetc(unsigned char c)
{
    if (i < UNGETMAX){

	ungetArray[i]  = c;
	i++;
	return c;

    }
    else{
	return SYSERR;	
    }

    // TODO: Check for room in unget buffer, put the character in or discard.
    //return SYSERR;
}





/**
 * Synchronously write a character to a UART.  This blocks until the character
 * has been written to the hardware.  The interrupt handler is not used.
 *
 * @param c
 *      The character to write.
 *
 * @return
 *      The character written to the UART as an <code>unsigned char</code> cast
 *      to an <code>int</code>.
 */
syscall kputc(uchar c)
{
    volatile struct pl011_uart_csreg *regptr;

    /* Pointer to the UART control and status registers.  */
    regptr = (struct pl011_uart_csreg *)0x3F201000;
    
    int flagptr = 0;

    do{
    
    flagptr = regptr->fr; //flag register
    
    }while(flagptr & PL011_FR_TXFF);



    regptr->dr = c; //data register

    // TODO: Check UART flags register.
    //       Once the Transmitter FIFO is not full, send character c.

    return c;
}






/**
 * kernel printf: formatted, synchronous output to SERIAL0.
 *
 * @param format
 *      The format string.  Not all standard format specifiers are supported by
 *      this implementation.  See _doprnt() for a description of supported
 *      conversion specifications.
 * @param ...
 *      Arguments matching those in the format string.
 *
 * @return
 *      The number of characters written.
 */


syscall kprintf(const char *format, ...)
{
    lock_acquire(serial_lock);

    int retval;
    va_list ap;

    va_start(ap, format);
    retval = _doprnt(format, ap, (int (*)(int, int))kputc, 0);
    va_end(ap);

    lock_release(serial_lock);

    return retval;
}
