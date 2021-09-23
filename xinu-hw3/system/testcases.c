/**
 * COSC 3250 - Project 3 Test Cases
 * This program provides our synchronous serial driver with test cases to test its functionality.
 * @authors Mario Ochoa, Jacqueline Gutierrez
 * Instructor Sabirat Rubya 
 * TA-BOT:MAILTO mario.ochoa@marquette.edu jacqueline.gutierrez@marquette.edu 
 */

/**
 * @file testcases.c
 * @provides testcases
 *
 * $Id: testcases.c 175 2008-01-30 01:18:27Z brylow $
 *
 * Modified by:
 *
 * and
 *
 */
/* Embedded XINU, Copyright (C) 2007.  All rights reserved. */

#include <xinu.h>

devcall putc(int dev, char c)
{
    return 0;
}

/**
 * testcases - called after initialization completes to test things.
 */
void testcases(void)
{
    int c;

    kprintf("===TEST BEGIN===\r\n");

    c = kgetc();
    // kprintf("Character Value: %c\r\n",c);
    // kprintf("ASCI Value: %d\r\n",c);

    if(isdigit(c)){
	c = c - 48;
    }
 

    switch (c)
    {

case 0:
                      kprintf("detected 0 \r\n");
                      break;
           case 1:
                      kprintf("detected 1 \r\n");
                      kputc(c);
                      break;
           case 2:
                      kprintf("detected 2  \r\n");
                      int i = (kcheckc());
                      if (i==0){
                      kprintf("kcheckc returns false\r\n");
                      }
                      else{
                      kprintf("kcheck returns true\r\n");
                      }
                 break;
 
        case 3:
              	kprintf("detected 3 \r\n");
                kungetc(c);
                break;

 	case 4:
		kprintf("detected 4 \r\n");	
		break;

	case 5:
                kprintf("detected 5 \r\n");
                break;
        case 6:
                kprintf("detected 6 \r\n");
                break;

        case 7:
                kprintf("detected 7  \r\n");
                break;

        case 8:
                kprintf("detected 8 \r\n");
                break;

        case 9:
                kprintf("Detected 9 \r\n");     
                break;

////////////////////////


	case 'a':
                kprintf("detected a \r\n");
                break;
        case 'b':
                kprintf("detected b \r\n");
                break;

        case 'c':
                kprintf("detected c  \r\n");
                break;

        case 'd':
                kprintf("detected d \r\n");
                break;

        case 'e':
                kprintf("Detected e \r\n");     
                break;

        // TODO: Test your operating system!

    default:
       // kprintf("Hello Xinu World!\r\n");
	kprintf("Default case statement\r\n");
	kprintf("Nothing in the testcases matched your input \r\n");

    }

    

    kprintf("\r\n===TEST END===\r\n");
    return;
}
