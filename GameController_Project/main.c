#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "main_asm.h" /* interface to the assembly module */

#define CONTR_BTN 0x1F
#define BTN_BLACK 0x01
#define BTN_GREEN 0x02
#define BTN_BLUE  0x04
#define BTN_RED   0x08
#define BTN_WHITE 0x10

#define CONTR_BTNenable PTH &= ~CONTR_BTN
#define CONTR_BTNdisable PTH |= CONTR_BTN


void main(void) {
  PLL_init();        // set system clock frequency to 24 MHz 
  DDRH = 0x00; //Enable Port H 0-4 as an output
  DDRB  = 0xff;       // Port B is output
  DDRJ  = 0xff;       // Port J is output
  DDRP  = 0xff;       // Port P is output

  PTJ = 0x00;         // enable LED
  PTP = 0xFF;         // enable all 7-segment displays
  
  
  while(1){
    
    PORTB = ~PTH;
  
  }
}