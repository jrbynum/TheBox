/*************************************************************
project: <type project name here>
author: <type your name here>
description: <type what this file does>
*************************************************************/

#include "TheBox.h"

//initialize all variables to their proper conditions. We do this as they are not initialized we declared globaly across modules
void init_global_vars(){

	timer_toggle_led = 0;
	toggleled=0;  
	/* Timer2 reload value, globally available */  
	tcnt2 = 0;  
	serialTimeout = 0;	

}

//Setup the pins for inputs or outputs and attach interrupts if needed
void setup_inputs_outputs()
{
			  
	pinMode(YELLOWLED, OUTPUT);     
	pinMode(BLUELED, OUTPUT);     
	pinMode(REDLED, OUTPUT);     
	pinMode(RESETDISPLAYPIN, OUTPUT); // define D2 as an output pin (Reset on the 4D Arduino Adaptor Shield)
 
    //set the outputs to high to turn the LEDS off initally
	digitalWrite(YELLOWLED, HIGH);
	digitalWrite(BLUELED, HIGH);
	digitalWrite(REDLED, HIGH);
}

void setup_heartbeat_timer(){

		  /* First disable the timer overflow interrupt while we're configuring */  
		  TIMSK2 &= ~(1<<TOIE2);  
		  
		  /* Configure timer2 in normal mode (pure counting, no PWM etc.) */  
		  TCCR2A &= ~((1<<WGM21) | (1<<WGM20));  
		  TCCR2B &= ~(1<<WGM22);  
		  
		  /* Select clock source: internal I/O clock */  
		  ASSR &= ~(1<<AS2);  
		  
		  /* Disable Compare Match A interrupt enable (only want overflow) */  
		  TIMSK2 &= ~(1<<OCIE2A);  
		  
		  /* Now configure the prescaler to CPU clock divided by 128 */  
		  TCCR2B |= (1<<CS22)  | (1<<CS20); // Set bits  
		  TCCR2B &= ~(1<<CS21);             // Clear bit  
		  
		  /* We need to calculate a proper value to load the timer counter. 
		   * The following loads the value 131 into the Timer 2 counter register 
		   * The math behind this is: 
		   * (CPU frequency) / (prescaler value) = 125000 Hz = 8us. 
		   * (desired period) / 8us = 125. 
		   * MAX(uint8) + 1 - 125 = 131; 
		   */  
		  /* Save value globally for later reload in ISR */  
		  tcnt2 = 131;   
		  
		  /* Finally load end enable the timer */  
		  TCNT2 = tcnt2;  
		  TIMSK2 |= (1<<TOIE2);  

}

void initializeDisplay()
{
	mySerial.begin(57600); // set the data rate for the Display communications Serial port
	resetDisplay();
	form_num = 0; //start at form0 in the display

}

void initializeHardware()
{

	setup_inputs_outputs();
	init_global_vars();
	setup_heartbeat_timer();
}

