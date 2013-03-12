//-------------------------------------------------------------------
#ifndef __analogtest_main_H__
#define __analogtest_main_H__
//-------------------------------------------------------------------

#include <arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

//-------------------------------------------------------------------

//swich matrix voltage readings based on the voltage divider resistors
#define S1V 4.545
#define S2V 4.098
#define S3V 3.759
#define S4V 3.401
#define S5V 3.205
#define S6V 2.976
#define S7V 2.747
#define S8V 2.500
#define S9V 2.273
#define S10V 2.000


#define	S1MIN 4.445
#define	S2MIN 3.998
#define	S3MIN 3.659
#define	S4MIN 3.301
#define	S5MIN 3.105
#define	S6MIN 2.876
#define	S7MIN 2.647
#define	S8MIN 2.400
#define	S9MIN 2.173
#define	S10MIN 1.900

#define	S1MAX	4.645
#define	S2MAX	4.198
#define	S3MAX	3.859
#define	S4MAX	3.501
#define	S5MAX	3.305
#define	S6MAX	3.076
#define	S7MAX	2.847
#define	S8MAX	2.600
#define	S9MAX	2.373
#define	S10MAX	2.100


#define READ_OBJ 0x00
#define WRITE_OBJ 0x01
#define WRITE_STR 0x02
#define REPORT_OBJ 0x05
#define REPORT_EVENT 0x07

#define NUMFORMS 5

#define YELLOWLED 8
#define BLUELED 9
#define REDLED 10
#define RESETDISPLAYPIN 2

#define mySerial Serial   //use the hardware UART on the MEGA 
#define serialDebug Serial1


//seven segment display objects
#define  led0_obj 0x0F00 //analog 0
#define  led1_obj 0x0F01 //amalog 1
#define  led2_obj 0x0F02 //analog 2
#define  led3_obj 0x0F03 //temperature
//string objects - not used
#define string0_obj 0x0B00
#define string1_obj 0x0B01
#define string2_obj 0x0B02
//meter object
#define angularmeter_obj 0x0700
//temperature object
#define thermometer_obj 0x1200
//forms associated with the display
#define  form0_obj 0x0A00
#define  form1_obj 0x0A01
#define  form2_obj 0x0A02
#define  form3_obj 0x0A03
#define  form4_obj 0x0A04

#define ledgauge0_obj 0x0b00
#define ledgauge1_obj 0x0b01
#define ledgauge2_obj 0x0b02


#define switch1led_obj 0x0E00
#define switch21led_obj 0x0E01
#define switch3led_obj 0x0E02
#define switch4led_obj 0x0E03
#define switch5led_obj 0x0E04
#define switch6led_obj 0x0E05
#define switch7led_obj 0x0E06
#define switch8led_obj 0x0E07
#define switch9led_obj 0x0E08
#define switch10led_obj 0x0E09


#define analogpin0 0
#define analogpin1 1
#define analogpin2 2


//-------------------------------------------------------------------

// Put yout declarations here

//-------------------------------------------------------------------

 //use EXTERN for global vaiable - must initialize them in init_vars function
#ifdef MAIN
#define EXTERN /* nothing */
#else
#define EXTERN extern
#endif /* DEFINE_VARIABLES */

EXTERN int form_num;
EXTERN int buttonPressed;
EXTERN unsigned int val0;
EXTERN unsigned int val1;
EXTERN unsigned int val2;


EXTERN float volt0;
EXTERN float volt1;
EXTERN float volt2;

//display stuff
EXTERN uint8_t command;
EXTERN uint16_t object;
EXTERN uint16_t  value;
EXTERN boolean flag;
EXTERN uint8_t txbuffer[10]; //for the conversion of floats to ascii

EXTERN bool sw1led;
EXTERN bool sw2led;
EXTERN bool sw3led;
EXTERN bool sw4led;
EXTERN bool sw5led;
EXTERN bool sw6led;
EXTERN bool sw7led;
EXTERN bool sw8led;
EXTERN bool sw9led;
EXTERN bool sw10led;


/* Timer2 reload value, globally available */
EXTERN unsigned int tcnt2;
 
/* Toggle HIGH or LOW digital write */
EXTERN int toggleled;
//software timers
EXTERN unsigned long timer_toggle_led;
EXTERN unsigned long serialTimeout;

//function prtotypes

//defined in 4dsystems.cpp
extern void updateDisplay();
extern uint8_t nacAck();
extern bool readMessage(uint8_t &command0, uint16_t &object0, uint16_t &value0);
extern uint8_t sendMessage(uint8_t command0, uint16_t object0, uint16_t value0);  
extern uint8_t sendStringFloat(uint8_t object_index, float fnum); 
extern void resetDisplay();
extern int checkButtons(int analogPin);

//defined in startup.cpp
extern void initializeHardware();
extern void initializeDisplay();
extern void init_global_vars();
extern void setup_inputs_outputs();
extern void setup_heartbeat_timer();


//===================================================================
// -> DO NOT WRITE ANYTHING BETWEEN HERE...
// 		This section is reserved for automated code generation
// 		This process tries to detect all user-created
// 		functions in main_sketch.cpp, and inject their  
// 		declarations into this file.
// 		If you do not want to use this automated process,  
//		simply delete the lines below, with "&MM_DECLA" text 
//===================================================================
//---- DO NOT DELETE THIS LINE -- @MM_DECLA_BEG@---------------------
void loop();
void setup();
//---- DO NOT DELETE THIS LINE -- @MM_DECLA_END@---------------------
// -> ...AND HERE. This space is reserved for automated code generation!
//===================================================================


//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
