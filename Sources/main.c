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
#include "masterHeaderFile.h"


void pieceMovement(char *array);

//Define song to play
//#define playTestPitch    0x01
#define playTetris       0x01
#define playPokemon      0x02
#define playIndianaJones 0x03
char x;
 

//Define the array's needed to keep track of the data
char  array1[] = {
    1,2,3,4,
    5,6,7,8,
    9,10,11,1,
    0,0,11,11
  };
//char array1[16], array2[16];
//testing purposes for the following line.
char array2[];

char *pArray = array1; //This will allways be pointing to the good data
char *pArrayTemp = array2; //This will allways be pointing to the temp array

  
    
void main(void) {

  
  int i = 0;

  SetClk24(); //Initialize PLL

  DDRA = 0xFF;   //for the SPI flag /#fix change to 1 bit
  PORTA = 0xFF;  //Set SPI Flag high
  SPI0_init();
  
  
  
  
  //used for testing, DDRJ,DDRB,PTJ,PORTB #fix
  DDRJ = 0xff;
  PTJ = 0x00;
  DDRB =0xff;
  PORTB=0x00;
  
  //Timer Interrupt Initialization
  sound_init();
  
  //RTI initializations
  RTICTL = 0x59;  //kick in every 20ms(lowest value rest is .02s = 20ms)
  CRGINT = 0x80;  //enable Real Time Interrupts (RTI)!
  CRGFLG = 0x80;  //make sure it is cleared at the beginning
  
  //Enable PortH(Controller Buttons) and PortH Interrupts
  enablePortH();
  PortH_ISR_Enable();
  
  //Now entereth the main loop
  
  for(;;){ 
   
   //If there was a button prerssed
   if(button != 0x00){
    
     pieceMovement(pArray); 
     displayGameBoard(pArray);
    
   }
   
   /* x = playTetris;
   
    IndianaJones(x);
    TetrisThemeA(x);
    PokemonTitle(x);*/
    
    
  } 
}

void pieceMovement(char *matrix){
    if(button == UP){
    //transform
    //transform
    //logic
    //transform
    //transform
  }else if(button == DOWN){
    arrayReduceWhiteSpaces(matrix);
    arrayCondence(matix);
  }else if(button == LEFT){ 
    //transform
    //transform
    //transform
    //logic
    //transform
  }else if(button == RIGHT){
    //transform
    //logic
    //transform
    //transform
    //transform
  }else{
    //do nothing
  }
  
  button = 0x00;

}