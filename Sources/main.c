#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "masterHeaderFile.h" //header file for our functions



void main(void) {
  /* put your own code here */
  
  SPI0_init(); 


	EnableInterrupts;


  for(;;) {
    _FEED_COP(); /* feeds the dog */
  } /* loop forever */
  /* please make sure that you never leave main */
}

interrupt VectorNumber_Vrti void RTI_handler(void){
 
 
//Logical RTI stuff
    
    
    
   
}