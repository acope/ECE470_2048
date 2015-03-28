/************************************************
Much to do.

Added like everything

RTI: Need initilization code

Need to add PORT H interrupts
Need to add 

************************************************/




#include <hidef.h>      //common defines and macros 
#include "derivative.h"      // derivative-specific definitions
#include "sound.h"       //sound definitions
#include "controller.h"  //controller definitions


//Define song to play
#define playTestPitch    0x01
#define playTetris       0x02
#define playPokemon      0x03
#define playIndianaJones 0x04


void main(void) {
  SetClk24(); //Initialize PLL

  //Timer Interrupt Initialization
  sound_init();
  
  //RTI initializations
  RTICTL = 0x59;  //kick in every 20ms(lowest value rest is .02s = 20ms)
  CRGINT = 0x80;  //enable Real Time Interrupts (RTI)!
  CRGFLG = 0x80;  //make sure it is cleared at the beginning
  
  //Enable PortH(Controller Buttons) and PortH Interrupts
  enablePortH();
  PortH_ISR_Enable();
  
  for(;;){
  
    char x = playPokemon;
    
    IndianaJones(x);
    TetrisThemeA(x);
    PokemonTitle(x);
    TestPitch(x);
  } 
}

