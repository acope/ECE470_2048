/*Button control layout
 *Black --> PH0
 *White --> PH1
 *Red   --> PH2
 *Green --> PH3
 *Blue  --> PH4
 *Each button is connected to a hardware debounce circuit, ~20ms delay 
 *Each debounce circuit has a pull down resisitor
 *When a button is pressed the input voltage is 0V
 */


#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "controller.h" 

#define CONTR_BTN_EN 0xE0 //0b11100000

#define button direction //Button pressed is now the direction, global variable able to be read from outside of controller.c, sent to controller.h

//Define global variables
volatile char btnPressed;
volatile char direction;

// Button values when pressed:
int GREEN, UP      = 0x08; // UP
int RED,   DOWN    = 0x04; // DOWN
int BLUE,  LEFT    = 0x10; // LEFT
int WHITE, RIGHT   = 0x02; // RIGHT
int BLACK, RESTART = 0x01; // RESTART

#pragma CODE_SEG NON_BANKED

interrupt VectorNumber_Vporth void controllerISR(){
 
 direction ^= ~PTH;
 PORTB = direction; //When a button is pressed corresponding LED will light up
 /* 
  * Button values when pressed:
  * Green = 0x08 = UP
  * Red   = 0x04 = DOWN
  * Blue  = 0x10 = LEFT
  * White = 0x02 = RIGHT
  * Black = 0x01 = RESTART
  */
 
 
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





