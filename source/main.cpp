/*************************************************************
project: The Box - Test code and hardware ustilizing and ATMEGA and a 4D systems uLCD-28PTU
		 Hardware: 	2 - 10K POTS connected to Analog 1 and 2, for 0-5 volt measurement.
		 
					1  - 10 button panel using a voltage divider circuit connected to
					analog 0 to detect which button is pressed. Currently utilizing two buttons
					to move through the screens on the 4D display
					
					3 - LEDS. Connected to Digital 8,9,10. Yellow, Blue, and Red
					1 - 1A 5 Volt regulator
					1 - RS232 port utilizing Serial1 (RX1, TX1) on the ATMEGA
					
					TODO - 
							add wireless (XBEE)
							add PWM motor control utilizing the POT for speed control
							add IR Receiver
							add CO2 detector
							add DS1821 temperature sensor
							add wii numchuck iC2 interface
							add ModBus
		Software:
					The 4D systems display utilizes the 4D Workshop and ViSi-Genie (high level interface)
					A 1 ms interrupt driven timer, utilizing TIMER2 is used as a general purpose timer, so the tone function will not work
					
					
author: Jonathan R. Bynum

description: 	This is the main progrsm file. 
				There are several other files needed:
				isr.cpp
				startup.cpp
				4dsystem.cpp
				thebox.h
*************************************************************/
#define MAIN

#include "TheBox.h"


#define ONE_WIRE_BUS 11

OneWire oneWire(ONE_WIRE_BUS);

DallasTemperature sensors(&oneWire);



void setup() { 
               
	initializeHardware();
    initializeDisplay();
	
	serialDebug.begin(19200); //for debugging to port 2
	Serial1.println("I am alive!");
	sensors.begin(); // IC Default 9 bit. If you have troubles consider upping it 12. Ups the delay giving the IC more time to process the temperature measuremen
}

void loop() 
{
 
  
  //read the analog values every loop
	val0 = analogRead(analogpin0); //button matrix
	val1 = analogRead(analogpin1); //Pot 1
	val2 = analogRead(analogpin2); //Pot 2
	
  //compute the analog volatges
	volt0= val0 * (5.0 / 1024.0);
	volt1= val1 * (5.0 / 1024.0);
	volt2= val2 * (5.0 / 1024.0);
	
	//print the values to serial port 
	serialDebug.print("A0: ");
	serialDebug.print(volt0);
    serialDebug.print(" A1: ");
	serialDebug.print(volt1);
	serialDebug.print(" A2: ");
	serialDebug.print(volt2);
	serialDebug.println("");
	
	
	serialDebug.print("Requesting temperatures...");
	sensors.requestTemperatures(); // Send the command to get temperatures
	serialDebug.println("DONE");
	serialDebug.print("Temperature for Device 1 is: ");
	serialDebug.print(sensors.getTempCByIndex(0)); // Why "byIndex"? You can have more than one IC on the same bus. 0 refers to the first IC on the wire
	serialDebug.println("");
	
		
	
	//check the button matrix to see if a button was pressed. I use a voltage divider matrix attached to Analog 0. The buttons are mapped according 
	//to the voltage produced when pressed.
    buttonPressed = checkButtons(0);

    //I use button 9 and 10 to scroll through the different screens loaded in the display
    //lets check to see what screen we need to be at...
    if(buttonPressed == 9)
    { // increment form
		form_num++;
		if(form_num == NUMFORMS)
			form_num = 0;
    
		if((sendMessage(WRITE_OBJ, form0_obj + form_num, 0)) == 0)
		{
			serialDebug.println("Serial Error!");
		}
    }
    else if(buttonPressed == 10)
    { //decrement form
		form_num--;
		if(form_num < 0)
		form_num = NUMFORMS - 1; //zero based
		if((sendMessage(WRITE_OBJ, form0_obj + form_num, 0)) == 0)
		{
			serialDebug.println("Serial Error!");
		}
    }

	updateDisplay(); //updates all the display with information

	
	
	// for debugging to make sure we arent resetting
	digitalWrite(YELLOWLED, HIGH);
	delay(250); // a half second delay makes the reading more stable other wise it will be too jumpy
	digitalWrite(YELLOWLED, LOW); 
	delay(250); // a half second delay makes the reading more stable other wise it will be too jumpy
  
}
