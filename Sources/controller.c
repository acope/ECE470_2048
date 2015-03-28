/*Button control layout
 *Black --> PH0
 *White --> PH1
 *Red   --> PH2
 *Green --> PH3
 *Blue  --> PH4
 *Each button is connected to a hardware debounce circuit, ~20ms delay 
 */


#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "controller.h" 

#define CONTR_BTN_EN 0xE0 //0b11100000

//Define global variables
volatile char btnPressed;



#pragma CODE_SEG NON_BANKED

interrupt VectorNumber_Vporth void controllerISR(){
 PORTB ^= 0xFF;  //When PORT H PH0 is pressed LED will turn off
 PIFH = 0xFF;   //Clear PortH interrupt flag, a MUST!
 
}

#pragma CODE_SEG DEFAULT 



void enablePortH(){
  DDRH &= CONTR_BTN_EN; //Mask Enable Port H Pins 0-4 as an input(0=input, 1=output)

}


void PortH_ISR_Enable(){
  PIEH = 0x1F; //Enable PIEH0-PIEH4 interrupts
  PPSH = 0x00; //Enable interrupt on falling edge, if you want rising edge make the bit 1
}





