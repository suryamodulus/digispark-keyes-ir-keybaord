#include "DigiKeyboard.h"
// not all keys are mapped in the DigiKeyboard.h file.
// you have to map it here
#define KEY_HOME   0x4A
#define KEY_PAGE_UP   0x4B
#define KEY_PAGE_DOWN   0x4E
#define KEY_ESCAPE   0x29
#define KEY_UP_ARROW   0x52
#define KEY_DOWN_ARROW   0x51
#define KEY_LEFT_ARROW   0x50
#define KEY_RIGHT_ARROW   0x4F
 
int irPin = 2; //Sensor pin connect to digital pin2 (ATINY85 pin7)
int start_bit = 2200; //Start bit threshold (Microseconds)
int bin_1 = 1000; //Binary 1 threshold (Microseconds)
int bin_0 = 400; //Binary 0 threshold (Microseconds)
const byte BIT_PER_BLOCK = 32;
 
void setup() {
  pinMode(irPin, INPUT);
}
 
void loop() {
  DigiKeyboard.update(); // keep updating the keyboard
  // this is generally not necessary but with some older systems it seems to
  // prevent missing the first character after a delay:
  DigiKeyboard.sendKeyStroke(0);
 
  int key = getIRKey();        //Fetch the key
 
  if(key != 0) //Ignore keys that are zero
  {
    //DigiKeyboard.print("=>");  //uncomment this if you want to 
    //DigiKeyboard.println(key); //print out the value of the button
 
    switch(key)
    {
      case 22: DigiKeyboard.println("1"); break;
      case 25: DigiKeyboard.println("2"); break;
      case 13: DigiKeyboard.println("3"); break;
      case 12: DigiKeyboard.println("4"); break;
      case 24: DigiKeyboard.println("5"); break;
      case 94: DigiKeyboard.println("6"); break;
      case 8: DigiKeyboard.println("7"); break;
      case 28: DigiKeyboard.println("8"); break;
      case 90: DigiKeyboard.println("9"); break;    
      case 82: DigiKeyboard.println("0"); break;  
      case 14: DigiKeyboard.sendKeyStroke(KEY_LEFT_ARROW); break; 
      case 10: DigiKeyboard.sendKeyStroke(KEY_RIGHT_ARROW); break;
      case 11: DigiKeyboard.sendKeyStroke(KEY_DOWN_ARROW); break;  
      case  7: DigiKeyboard.sendKeyStroke(KEY_UP_ARROW); break;   
      case 64: DigiKeyboard.sendKeyStroke(KEY_SPACE); break; 
      case 66: DigiKeyboard.sendKeyStroke(KEY_ESCAPE); break;
      case 74: DigiKeyboard.sendKeyStroke(KEY_ENTER); break; 
    }    
  }
}
 
/////////////////////////////////////////////////////////////
// decode infrared signal
/////////////////////////////////////////////////////////////
int getIRKey() {
  int data[BIT_PER_BLOCK];
  int i;
  while(pulseIn(irPin, HIGH) < start_bit); //Wait for a start bit
 
  for(i = 0 ; i < BIT_PER_BLOCK ; i++) 
    data[i] = pulseIn(irPin, HIGH); //Start measuring bits, I only want HIGH pulses
 
  delay(100);  
  for(i = 0 ; i < BIT_PER_BLOCK ; i++) //Parse them
  {   
    if(data[i] > bin_1) //is it a 1?
      data[i] = 1;
    else if(data[i] > bin_0) //is it a 0?
      data[i] = 0;
    else
      return -1; //Flag the data as invalid; Return -1 on invalid data
  }
 
  //based on NEC protocol, command data started from bit 16
  //and end with bit 24 (8 bits long)
  int result = 0;
  for(i = 16 ; i < 24; i++) { 
    //DigiKeyboard.print(data[i]); //print out the value of button in binary form
    if(data[i] == 1) result |= (1<<i-16); //Convert data bits to integer
  }  
  return result; //Return key number
}
