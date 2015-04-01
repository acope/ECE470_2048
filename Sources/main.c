#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "masterHeaderFile.h" //header file for our functions

//char array1[16], array2[16];

void SetClk24(void) //Set the clock speed to 24Mhz
{
   CLKSEL &= 0x7F;
   PLLCTL |= 0x40;
   SYNR = 0x02;
   REFDV = 0x01;
   while(!(0x08 & CRGFLG)); //Wait for tracking to lock in
   CLKSEL |= 0x80;
}

void main(void) {
  /* put your own code here */
  char  array1[] = {
    1,2,3,4,
    5,6,7,8,
    9,10,11,1,
    0,0,11,11
  };
  int i = 0;
  
  SetClk24();
  
  SPI0_init(); 

  DDRA = 0xFF  ;
  PORTA = 0xFF;

	EnableInterrupts;

  //putsomthing in the array for testing
   PTS_PTS7 = 0;

  for(;;) {
   PORTA = 0x00;
   
   //SPI0_outChar(0xAA);
   displayGameBoard(array1);
   PORTA = 0xFF;
   for(i = 0; i < 500; i++);
   
  }/* please make sure that you never leave main */
}

interrupt VectorNumber_Vrti void RTI_handler(void){
 
 
//Logical RTI stuff
    
    
    
   
}