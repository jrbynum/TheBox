/*************************************************************
project: <type project name here>
author: <type your name here>
description: <type what this file does>
*************************************************************/

#include "TheBox.h"



void updateDisplay()
{
	//convert the floats into strings and send to the display. The 4D Systems displays dont do floats or negative numbers using the high level 
	//ViSi Genie interface, so we use a string message box and display what ever we want to..... I just implemented the float conversion here as 
	//thats all i needed, but it could be mofified to display any ascii characters - up to 80. 80 characters is the limit of the object.
	if(sendStringFloat(0, volt0) == 0)
	{
		serialDebug.println("Serial Error write string!");
	}
	if(sendStringFloat(1, volt1) == 0)
	{
		serialDebug.println("Serial Error write string!");
	}
	if(sendStringFloat(2, volt2) == 0)
	{
		serialDebug.println("Serial Error write string!");
	}

    //I use the sendMessage function to communicate with the display and update the 4D Systems controls
    //the protocol is explained in the ViSi-Genie reference manual located at the 4D Systems website: www.4dsystems.com.au
	if((sendMessage(WRITE_OBJ, led0_obj, val0)) == 0)
	{
		Serial1.println("Serial Timeout Error!");
	}
	if((sendMessage(WRITE_OBJ, led1_obj, val1)) == 0)
	{
		Serial1.println("Serial Timeout Error!");
	}
	if((sendMessage(WRITE_OBJ, led2_obj, val2)) == 0)
	{
		Serial1.println("Serial Timeout Error!");
	}


	if((sendMessage(WRITE_OBJ, ledgauge0_obj, val0)) == 0)
	{
		Serial1.println("Serial Timeout Error!");
	}
	if((sendMessage(WRITE_OBJ, ledgauge1_obj, val1)) == 0)
	{
		Serial1.println("Serial Timeout Error!");
	}
	if((sendMessage(WRITE_OBJ, ledgauge2_obj, val2)) == 0)
	{
		Serial1.println("Serial Timeout Error!");
	}


}



uint8_t nacAck() 
{


	delay(10);
	serialTimeout = 0;
	while (!mySerial.available())
	{
		if(serialTimeout > 1000)
			return(0);
	
	}

	return mySerial.read(); // 0xc6 or 0xd5
}


// Reads Massges being sent from the screen
bool readMessage(uint8_t &command0, uint16_t &object0, uint16_t &value0) 
{

bool flag0 = false;
uint8_t buffer[6];
uint8_t checksum = 0x00;
uint8_t i=0;

	if (mySerial.available()>0) {
		buffer[i] = mySerial.read();
		command0 = buffer[0];
		i++;
		checksum = command0;
		if (command0==REPORT_EVENT) {
			if (mySerial.available()>3) {
				buffer[i] = mySerial.read();
				checksum ^= buffer[i];
				i++;
				object0 = buffer[1]<<8 | buffer[2];
				buffer[i] = mySerial.read();
				checksum ^= buffer[i];
				i++;
				object0 = buffer[1]<<8 | buffer[2];
				buffer[i] = mySerial.read();
				checksum ^= buffer[i];
				i++;
				value0 = buffer[3]<<8 | buffer[4];
			}
		}
		flag0 = (checksum==mySerial.read());
	}
return flag0;
}


// Sends messages to the Screen
uint8_t sendMessage(uint8_t command0, uint16_t object0, uint16_t value0) 
{
	
	uint8_t buffer[6];
	uint8_t checksum = 0x00;
	uint8_t i=0;
	uint8_t ret;
	
	buffer[i] = command0;
	checksum = command0;
	i++;
	buffer[i] = object0 >>8;
	checksum ^= buffer[i];
	i++;
	buffer[i] = object0 & 0xff;
	checksum ^= buffer[i];
	i++;
	buffer[i] = value0 >>8;
	checksum ^= buffer[i];
	i++;
	buffer[i] = value0 & 0xff;
	checksum ^= buffer[i];
	i++;
	buffer[i] = checksum;
	i++;
	mySerial.write(buffer, i);
	//delay (250);
	ret = nacAck();
	
	if(ret > 0)
		digitalWrite(REDLED, HIGH);
	else
		digitalWrite(REDLED, LOW);
	
	
	
return (ret);
}

uint8_t sendStringFloat(uint8_t object_index, float fnum) 
{
	
	//uint8_t charbuffer[84];
	char charbuff[5];
	uint8_t checksum = 0x00;
	uint8_t i=0;
	uint8_t ret;
	
	
	
	dtostrf(fnum, 5, 2, charbuff);
	
	uint8_t length = strlen(charbuff);
	
//	    serialDebug.print("Data: ");
//	    serialDebug.println(*str);
	
	//set the command byte
	txbuffer[i] = 0x02;
	//add the command byte to the checksum
	checksum = 0x02;
	//increment the byte counter
	i++;
	
	//set the send buffer with the MSB of the object
	txbuffer[i] = object_index;
	//XOR the checksum
	checksum ^= txbuffer[i];
	//increment the byte counter
	i++;
	

	//set the length
	txbuffer[i] = 5;
	//XOR the checksum
	checksum ^= txbuffer[i];
	//increment the byte counter
	i++;
	
	
	//set the LSB of the object
	txbuffer[i] = charbuff[0];
	//XOR the checksum
	checksum ^= txbuffer[i];
	//increment the byte counter
	i++;
	
	txbuffer[i] = charbuff[1];
	//XOR the checksum
	checksum ^= txbuffer[i];
	//increment the byte counter
	i++;
	txbuffer[i] = charbuff[2];
	//XOR the checksum
	checksum ^= txbuffer[i];
	//increment the byte counter
	i++;
	txbuffer[i] = charbuff[3];
	//XOR the checksum
	checksum ^= txbuffer[i];
	//increment the byte counter
	i++;
	txbuffer[i] = charbuff[4];
	//XOR the checksum
	checksum ^= txbuffer[i];
	//increment the byte counter
	i++;
	txbuffer[i] = checksum;
	i++;
	mySerial.write(txbuffer, 9);
	//delay (250);
	ret = nacAck();

	if(ret > 0)
		digitalWrite(REDLED, HIGH);
	else
		digitalWrite(REDLED, LOW);

	
return (ret);
}


void resetDisplay()
{

	//Reset the display 
	digitalWrite(RESETDISPLAYPIN, HIGH); // Reset
	delay(100);
	digitalWrite(RESETDISPLAYPIN, LOW); // Clear Reset
  
	delay(3000);  // Allow time for the display to initialize before communicating

}

//*****************************************************************************
// Function to check if a switch is pressed - returns the number of the switch
// returns 0 if no switch is pressed.
// This is based on the votage divider switch circuit.
//*****************************************************************************
int checkButtons(int analogPin) {                

int val = 0;
float voltage;

	val = analogRead(analogPin);
	if (val < 10)
		return(0); // no switch pressed
	
	//convert adc reading to voltage
	voltage = val * (5.0 /1024.0);
	
	
	if( voltage  > S1MIN && voltage < S1MAX)
	{
		sw1led = true;
		return(1); //Switch 1 pressed
	}
	else
	{
		sw1led = false;
	}

	if( voltage  > S2MIN && voltage < S2MAX)
	{
		sw2led = true;
		return(2); //Switch 2 pressed
	}
	else
	{
		sw2led = false;
	}
	if( voltage  > S3MIN && voltage < S3MAX)
	{
		sw3led = true;
		return(3); //Switch 3 pressed
	}
	else
	{
		sw3led = false;
	}
	if( voltage  > S4MIN && voltage < S4MAX)
	{
		sw4led = true;
		return(4); //Switch 4 pressed
	}
	else
	{
		sw4led = false;
	}
	if( voltage  > S5MIN && voltage < S5MAX)
	{
		sw5led = true;
		return(5); //Switch 5 pressed
	}
	else
	{
		sw5led = false;
	}
	if( voltage  > S6MIN && voltage < S6MAX)
	{
		sw6led = true;
		return(6); //Switch 6 pressed
	}
	else
	{
		sw6led = false;
	}
	if( voltage  > S7MIN && voltage < S7MAX)
	{
		sw7led = true;
		return(7); //Switch 7 pressed
	}
	else
	{
		sw7led = false;
	}
	if( voltage  > S8MIN && voltage < S8MAX)
	{
		sw8led = true;
		return(8); //Switch 8 pressed
	}
	else
	{
		sw8led = false;
	}
	if( voltage  > S9MIN && voltage < S9MAX)
	{
		sw9led = true;
		return(9); //Switch 9 pressed
	}
	else
	{
		sw9led = false;
	}
	if( voltage  > S10MIN && voltage < S10MAX)
	{
		sw10led = true;
		return(10); //Switch 10 pressed
	}
	else
	{
		sw10led = false;
	}
	
	return(0); // no switch pressed or mutliple switches pressed

}
