/*************************************************************
project: <type project name here>
author: <type your name here>
description: <type what this file does>
*************************************************************/

#include "TheBox.h"


//*****************************************************************
// ISR are placed here
//*****************************************************************

//***************************************************************************************************************************
//Heartbeat 1 ms timer interrupt - uses timer 2 - this is the Tone interrupt for the Arduino Libaray so this function cannot be used
//***************************************************************************************************************************
/* 
 * Install the Interrupt Service Routine (ISR) for Timer2 overflow. 
 * This is normally done by writing the address of the ISR in the 
 * interrupt vector table but conveniently done by using ISR()  
* this is called every 1 ms*/  
ISR(TIMER2_OVF_vect) {  
		  /* Reload the timer */  
		  TCNT2 = tcnt2;  
		  
			//toggle pin 2 for testing
			//debug();

			//Heartbeat LED - 1 second toggle
			timer_toggle_led++;
			serialTimeout++;
			if(timer_toggle_led >= 1000 )
			{
				 timer_toggle_led=0;
				/* Write to a digital pin so that we can confirm our timer */  
				digitalWrite(BLUELED, toggleled == 0 ? HIGH : LOW);  
				toggleled = ~toggleled;  
			}


}  
