#include <hidef.h>      //common defines and macros 
#include "derivative.h"      // derivative-specific definitions
#include "sound.h"       //sound definitions
#include "controller.h"  //controller definitions
#include "masterHeaderFile.h"


void pieceMovement();

//Define song to play
//#define playTestPitch    0x01
#define playTetris       0x01
#define playPokemon      0x02
#define playIndianaJones 0x03
char x;
 

//Define the array's needed to keep track of the data
char  array1[] = {
    0,0,0,0,
    0,0,0,0,
    0,0,0,0,
    0,0,0,0 
    };
//char array1[16], array2[16];
//testing purposes for the following line.
char array2[16];

char *pArray = array1; //This will allways be pointing to the good data
char *pArrayTemp = array2; //This will allways be pointing to the temp array


void init_timer(){
     TSCR1 = 0x80;  // enable channel 7
     TSCR2 = 0x00;   //no interrupt and no scaler
     
}


  
    
void main(void) {

  
  int i = 0;

  SetClk24(); //Initialize PLL

  DDRA = 0xFF;   //for the SPI flag /#fix change to 1 bit
  PORTA = 0xFF;  //Set SPI Flag high
  SPI0_init();
  
  init_timer();
  
  
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
  placeRandomPeice(pArray, TCNT);
  displayGameBoard(pArray);
  
   // x = playTetris;
   
    IndianaJones(3);
  
  //PORTB = 0xff;
  for(;;){ 
   
   //If there was a button prerssed
   if(direction != 0x00){
    
     pieceMovement(); 
     displayGameBoard(pArray);
	
	 if(placeRandomPeice(pArray, TCNT)){ //if there was a new piece put on the board.
		displayGameBoard(pArray);
		
	 }else{ //game board full!
	 //display the game over!
	 }
	 
   } 
   
   /* x = playTetris;
   
    IndianaJones(x);
    TetrisThemeA(x);
    PokemonTitle(x);
    */
    
  } 
}

void rotateRight(void)
{
	char *temp; //used when swapping the two matrixes
	char cA[]= {23,8,4,0};
	char c, cAValue;

	/*//I hope this works
	for(c =0; c<16; c++)
	{
		cAValue = cA[c % 4];
		*(pArrayTemp + c) = *(pArray + cAValue + (c/4));
	} */
	
	*(pArrayTemp + 0) = *(pArray + 3);
	*(pArrayTemp + 1) = *(pArray + 7);
	*(pArrayTemp + 2) = *(pArray + 11);
	*(pArrayTemp + 3) = *(pArray + 15);
	*(pArrayTemp + 4) = *(pArray + 2);
	*(pArrayTemp + 5) = *(pArray + 6);
	*(pArrayTemp + 6) = *(pArray + 10);
	*(pArrayTemp + 7) = *(pArray + 14);
	*(pArrayTemp + 8) = *(pArray + 1);
	*(pArrayTemp + 9) = *(pArray + 5);
	*(pArrayTemp + 10) = *(pArray + 9);
	*(pArrayTemp + 11) = *(pArray + 13);
	*(pArrayTemp + 12) = *(pArray + 0);
	*(pArrayTemp + 13) = *(pArray + 4);
	*(pArrayTemp + 14) = *(pArray + 8);
	*(pArrayTemp + 15) = *(pArray + 12);
	
	
	
	//swap matrix
	temp = pArray; 
	pArray = pArrayTemp;
	pArrayTemp = temp;
	
}


void pieceMovement(void){
 
    
    if(direction == 0x08){
    
      //transform
      rotateRight();
      //transform
      rotateRight();
      
      
      //logic
      arrayReduceWhiteSpaces(pArray);
      arrayReduceWhiteSpaces(pArray);
      arrayCondence(pArray);
       arrayReduceWhiteSpaces(pArray);
      arrayReduceWhiteSpaces(pArray);
      //transform
      
      rotateRight();
      //transform
      rotateRight();
      
    
      
  }else if(direction == 0x04){     //down
      arrayReduceWhiteSpaces(pArray);
      arrayReduceWhiteSpaces(pArray);
       
      arrayCondence(pArray);
      arrayReduceWhiteSpaces(pArray);
      arrayReduceWhiteSpaces(pArray);
  
  }else if(direction == 0x10){  //left
     //transform
      rotateRight();
      //logic
      arrayReduceWhiteSpaces(pArray);
      arrayReduceWhiteSpaces(pArray);
      arrayCondence(pArray);
      arrayReduceWhiteSpaces(pArray);
      arrayReduceWhiteSpaces(pArray);

      //transform
      rotateRight();
      
      //transform
      rotateRight();
      //transform
      rotateRight();
  }else if(direction == 0x02){   //right
      //transform
      rotateRight();
      //transform
      rotateRight();
      //transform
      rotateRight();
      //logic
      arrayReduceWhiteSpaces(pArray);
      arrayReduceWhiteSpaces(pArray);
      arrayCondence(pArray);
      arrayReduceWhiteSpaces(pArray);
      arrayReduceWhiteSpaces(pArray);
      
      //transform
      rotateRight() ;
  }else{
    //do nothing
  }
  
  direction = 0x00;  //clear the button
  

}

