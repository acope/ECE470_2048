#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */

#include "sound.h"


//DEFINE NOTE PITCH
#define C0    45827   //Unaudiable on Board speaker
#define Cs0   43256
#define D0    40829
#define Ds0   38538
#define E0    36376
#define F0    34335
#define Fs0   32409
#define G0    30590
#define Gs0   28874
#define A0    27254
#define As0   25725
#define B0    24282
#define C1    22919
#define Cs1   21633
#define D1    20419
#define Ds1   19274
#define E1    18192
#define F1    17172
#define Fs1   16208
#define G1    15299
#define Gs1   14441
#define A1    13630
#define As1   12866
#define B1    12144
#define C2    11462
#define Cs2   10819
#define D2    10212
#define Ds2   9639
#define E2    9098
#define F2    8588
#define Fs2   8106
#define G2    7651
#define Gs2   7222
#define A2    6817
#define As2   6434
#define B2    6073
#define C3    5733
#define Cs3   5411
#define D3    5107
#define Ds3   4821
#define E3    4550
#define F3    4295
#define Fs3   4054
#define G3    3827
#define Gs3   3612
#define A3    3409
#define As3   3218
#define B3    3037
#define C4    2867
#define Cs4   2706
#define D4    2554
#define Ds4   2411
#define E4    2276
#define F4    2148
#define Fs4   2028
#define G4    1914
#define Gs4   1806
#define A4    1705
#define As4   1609
#define B4    1519
#define C5    1434
#define Cs5   1353
#define D5    1277
#define Ds5   1206
#define E5    1138
#define F5    1074
#define Fs5   1014
#define G5    957
#define Gs5   903
#define A5    853
#define As5   805
#define B5    760
#define C6    717
#define Cs6   677
#define D6    639
#define Ds6   603
#define E6    569
#define F6    537
#define Fs6   507
#define G6    479
#define Gs6   452
#define A6    426
#define As6   402
#define B6    380
#define C7    359
#define Cs7   338
#define D7    319
#define Ds7   302
#define E7    285
#define F7    269
#define Fs7   254
#define G7    239
#define Gs7   226
#define A7    213
#define As7   201
#define B7    190
#define C8    179
#define Cs8   169
#define D8    160
#define Ds8   151
#define E8    142
#define F8    134
#define Fs8   127
#define G8    120
#define Gs8   113
#define A8    107
#define As8   101
#define B8    95
#define r     0 



//DEFINE NOTE REST
#define sixtenth          62             //0.06  seconds
#define eigth             125            //0.12  seconds
#define eigthDot          187            //0.19  seconds
#define quarter           250            //0.25  seconds
#define quarterDot        375            //0.37  seconds
#define half              500            //0.50  seconds
#define halfDot           750            //0.75  seconds
#define whole             1000           //1.00  second
#define tripeletQ         quarter/3      //0.08  seconds
#define tripeletS         sixtenth/3     //0.02  seconds
#define tripeletE         eigth/3        //0.04  seconds
#define tieEigthQuarter   eigth+quarter  //0.37  seconds
#define tieEigthHalf      eigth+half     //0.62  seconds
#define tieTripletEHalf   tripeletE+half //0.54  seconds
#define end_song          half           //end song, quiet for 0.5 seconds


//Define song to play
#define playTestPitch    0x01
#define playTetris       0x02
#define playPokemon      0x03
#define playIndianaJones 0x04



//variables used for RTI
int restValue = 0;
int j = 0;
volatile int noteValue = 0;
volatile int *noteP; //note pointer
volatile int *restP; //rest pointer

//other variables
int i;
volatile int pitch;
volatile int rest;


/*********************************************************************/
/************************TETRIS THEME A*******************************/
/*********************************************************************/
int tetrisScore[]={
//|              1               |           2           |       3       |     4     |        5             |
   E6, B5, C6, D6, E6, D6, C6, B5, A5, A5, C6, E6, D6, C6, B5, C6, D6, E6, C6, A5, A5, r, D6, F6, A6, G6, F6, //27 NOTES
//|         6        |           7       |       8     :||
   E6, C6, E6, D6, C6, B5, B5, C6, D6, E6, C6, A5, A5, r,//REPEAT, 14 NOTES
//|             9                |          10           |       11      |     12    |          13          |  
   E6, B5, C6, D6, E6, D6, C6, B5, A5, A5, C6, E6, D6, C6, B5, C6, D6, E6, C6, A5, A5, r, D6, F6, A6, G6, F6, //27 NOTES
//|         14       |         15        |       16     |   17  |   18  |
   E6, C6, E6, D6, C6, B5, B5, C6, D6, E6, C6, A5, A5, r, E5, C5, D5, B4,//18 NOTES
//|  19  |    20  |    21 |  22   |     23     |     24     |
   C5, A4, Gs4, B4, E5, C5, D5, B4, C5, E5, A5, Gs5, r //13 NOTES
};
int tetrisDelay[]={
//|                             1                                |                       2                     |                 3                  |            4          |                   5                         |
   quarter, eigth, eigth, eigth, sixtenth, sixtenth, eigth, eigth, quarter, eigth, eigth, quarter, eigth, eigth, quarterDot, eigth, quarter, quarter, quarter, quarter, half, eigth, quarter, eigth, quarter, eigth, eigth,//27 NOTES
//|                  6                     |                    7                   |               8                 :||
   quarterDot, eigth, quarter, eigth, eigth, quarter, eigth, eigth, quarter, quarter, quarter, quarter, quarter, quarter, //REPEAT, 14 NOTES
//|                             9                                |                        10                   |                 11                 |            12         |                   13                        |  
   quarter, eigth, eigth, eigth, sixtenth, sixtenth, eigth, eigth, quarter, eigth, eigth, quarter, eigth, eigth, quarterDot, eigth, quarter, quarter, quarter, quarter, half, eigth, quarter, eigth, quarter, eigth, eigth,//27 NOTES
//|                  14                    |                    15                  |               16                  |     17    |     18    |
   quarterDot, eigth, quarter, eigth, eigth, quarter, eigth, eigth, quarter, quarter, quarter, quarter, quarter, quarter, half, half, half, half,  //18 NOTES
//|    19    |     20    |    21     |     22    |           23          |   24          |
   half, half, half, half, half, half, half, half, quarter, quarter, half, whole, end_song //13 NOTES
};

/*********************************************************************/
/**********************POKEMON TITLE THEME****************************/
/*********************************************************************/
int pokemontitleScore[]={
//|       1       |          2           |               3               ||:   4      |          5         |
   G4, B4, D5, Fs5, G5, G5, r, G5, G5, G5, G5, G5, F5, F5, F5, F5, F5, Fs5, G5, B5, D6, C5, F5, F6, E6, Ds6,  //26 NOTES
//|       6       |       7       |     8     |       9       |      10       |      11      |
   D6, F5, E5, Ds6, D5, C5, B4, C5, G5, B5, D6, C5, C6, B5, C6, D6, F5, E5, C5, D5, B4, C5, D5, //23 NOTES
//|     12   |          13            |        14      |       15      |      16   |
   G5, B5, D6, C5, F5, F6, E6, Ds6, D6, F5, E5, Ds5, D5, C5, B4, C5, G5, B5, D6, //19 NOTES
//|        17        |      18    |     19    |      20    |      21    |     22    |
   C5, C5, F6, E6, F6, G6, As6, G6, G5, A5, A6, As6, F6, F6, F5, As6, B6, C7, G6, G6,//20 NOTES
//|     23    |                 24                  |                  25            |                   26                |                27            :||
   G5, C7, Cs7, D7, D6, r, r, D6, r, r, D6, r, r, D6, r, r, D6, r, r, D6, C7, C7, Cs7, D7, D6, r, r, D6, r, r, D6, r, r, D6, r, r, D6, r, r, D6, C7, C7, B6, //43 NOTES, REPEAT BACK TO BEGINNING
//||:   4      |          5       | 
   G5, B5, D6, C5, F5, F6, E6, Ds6,//26 NOTES
//|       6       |       7       |     8     |       9       |      10       |      11      |
   D6, F5, E5, Ds6, D5, C5, B4, C5, G5, B5, D6, C5, C6, B5, C6, D6, F5, E5, C5, D5, B4, C5, D5, //23 NOTES
//|     12   |          13            |        14      |       15      |      16   |
   G5, B5, D6, C5, F5, F6, E6, Ds6, D6, F5, E5, Ds5, D5, C5, B4, C5, G5, B5, D6, //19 NOTES
//|        17        |      18    |     19    |      20    |      21    |     22    |
   C5, C5, F6, E6, F6, G6, As6, G6, G5, A5, A6, As6, F6, F6, F5, As6, B6, C7, G6, G6,//20 NOTES
//|     23    |                 24                  |                  25            |                   26                |                27                     :||
   G5, C7, Cs7, D7, D6, r, r, D6, r, r, D6, r, r, D6, r, r, D6, r, r, D6, C7, C7, Cs7, D7, D6, r, r, D6, r, r, D6, r, r, D6, r, r, D6, r, r, D6, C7, C7, B6 ,r//44 NOTES
};

int pokemontitleDelay[]={
//|                   1                  |                              2                      |                                       3                                         ||:            4           |                             5                           |
   sixtenth, sixtenth, sixtenth, sixtenth, quarter, quarter, eigth, sixtenth, sixtenth, quarter, quarter, quarter, tripeletE, tripeletE, tripeletE, tripeletE, tripeletE, tripeletE, quarterDot, eigth, half, quarterDot, tieEigthQuarter, quarter, sixtenth, sixtenth, //26 NOTES
//|                 6                  |                   7                  |            8           |                   9                  |                     10               |               11                 |
   half, quarterDot, sixtenth, sixtenth, half, tripeletQ, tripeletQ, tripeletQ, quarterDot, eigth, half, half, tripeletQ, tripeletQ, tripeletQ, half, tripeletQ, tripeletQ, tripeletQ, tieEigthHalf, eigth, eigth, eigth, //23 NOTES
//|          12           |                             13                        |                    14               |                  15                  |            16          |
   quarterDot, eigth, half, quarterDot, eigth, tieEigthQuarter, sixtenth, sixtenth, half, quarterDot, sixtenth, sixtenth, half, tripeletQ, tripeletQ, tripeletQ, quarterDot, eigth, half, //19 NOTES
//|                      17                          |           18           |          19            |             20         |          21           |            22          |
   quarterDot, eigth, tripeletQ, tripeletQ, tripeletQ, quarterDot, eigth, half, quarterDot, eigth, half, quarterDot, eigth, half, half, quarter, quarter, quarterDot, eigth, half, //20 NOTES
//|         23           |                                                    24                                              |                                            25                                        |                                                26                                                  |                                         27                                          :||
   half, quarter, quarter, eigthDot, sixtenth, eigth, sixtenth, sixtenth, eigth, sixtenth, sixtenth, eigth, sixtenth, sixtenth, eigth, sixtenth, sixtenth, eigth, sixtenth, sixtenth, tripeletQ, tripeletQ, tripeletQ, eigthDot, sixtenth, eigth, sixtenth, sixtenth, eigth, sixtenth, sixtenth, eigth, sixtenth, sixtenth, eigth, sixtenth, sixtenth, eigth, sixtenth, sixtenth, tripeletQ, tripeletQ, tripeletQ, //43 NOTES, REPEAT BACK TO BEGINNING
//||:            4           |                             5                           | 
   quarterDot, eigth, half, quarterDot, eigth, tieEigthQuarter ,sixtenth, sixtenth, 
//|                 6                  |                   7                  |            8           |                   9                  |                     10               |               11                 |
   half, quarterDot, sixtenth, sixtenth, half, tripeletQ, tripeletQ, tripeletQ, quarterDot, eigth, half, half, tripeletQ, tripeletQ, tripeletQ, half, tripeletQ, tripeletQ, tripeletQ, tieEigthHalf, eigth, eigth, eigth, //23 NOTES
//|          12           |                             13                        |                    14               |                  15                  |            16          |
   quarterDot, eigth, half, quarterDot, eigth, tieEigthQuarter, sixtenth, sixtenth, half, quarterDot, sixtenth, sixtenth, half, tripeletQ, tripeletQ, tripeletQ, quarterDot, eigth, half, //19 NOTES
//|                      17                          |           18           |          19            |             20         |          21           |            22          |
   quarterDot, eigth, tripeletQ, tripeletQ, tripeletQ, quarterDot, eigth, half, quarterDot, eigth, half, quarterDot, eigth, half, half, quarter, quarter, quarterDot, eigth, half, //20 NOTES
//|         23           |                                                    24                                              |                                            25                                        |                                                26                                                  |                                         27                                                   :||
   half, quarter, quarter, eigthDot, sixtenth, eigth, sixtenth, sixtenth, eigth, sixtenth, sixtenth, eigth, sixtenth, sixtenth, eigth, sixtenth, sixtenth, eigth, sixtenth, sixtenth, tripeletQ, tripeletQ, tripeletQ, eigthDot, sixtenth, eigth, sixtenth, sixtenth, eigth, sixtenth, sixtenth, eigth, sixtenth, sixtenth, eigth, sixtenth, sixtenth, eigth, sixtenth, sixtenth, tripeletQ, tripeletQ, tripeletQ, end_song //43 NOTES  
};


/*********************************************************************/
/**********************INDIANA JONES THEME****************************/
/*********************************************************************/
 int indianajonesScore[]={
//|  1   ||:     2        |     3     |      4        |
   E5, F5, G5, C6, D5, E5, F5, G5, A5, B5, F6, A5, B5,//13 NOTES
//|         5        |      6        |     7     |        8              |
   C6, D6, E6, E5, F5, G5, C6, D6, E6, F6, G5, G5, E6, D6, G5, E6, D6, G5,//18 NOTES 
//|          9              :|
   E6, D6, G5, E6, D6, E5, F5,//7 NOTES, REPEAT 1
//|     10       |     11    |        12     |
   G5, C6, D5, E5, F5, G5, A5, B5, F6, A5, B5,//11 NOTES
//|          13      |       14      |     15    |           16          |
   C6, D6, E6, E5, F5, G5, C6, D6, E6, F6, G5, G5, E6, D6, G5, E6, D6, G5, //18 NOTES, END OF FIRST REPEAT
//|           17             |     18    |         19        |
   E6, D6, G5, E6, D6, E5, G5, F5, D5, F5, E5, G5, E6, E5, G5,//15 NOTES 
//|    20    |         21        |   22      |         23           |
   F5, D5, F5, E5, G5, E6, D6, E6, F6, D6, F6, Ds6, D6, C6, r//15 NOTES  
 };
 
 int indianajonesDelay[]={
//|         1        ||:                  2                    |             3              |                    4                   |
   eigthDot, sixtenth, eigth, tieEigthHalf, eigthDot, sixtenth, halfDot, eigthDot, sixtenth, eigth, tieEigthHalf, eigthDot, sixtenth,//13 NOTES
//|                  5                          |                    6                   |             7              |                          8                              |
   quarter, quarter, quarter, eigthDot, sixtenth, eigth, tieEigthHalf, eigthDot, sixtenth, halfDot, eigthDot, sixtenth, quarter, eigthDot, sixtenth, quarter, eigthDot, sixtenth,//18 NOTES
//|                         9                                  :|
   quarter, eigthDot, sixtenth, eigth, eigth, eigthDot, sixtenth, //7 NOTES, REPEAT 1
//|                        10             |                11          |                  12                    |
   eigth, tieEigthHalf, eigthDot, sixtenth, halfDot, eigthDot, sixtenth, eigth, tieEigthHalf, eigthDot, sixtenth, //11 NOTES
//|                     13                      |                    14                  |                  15        |                        16                               |
   quarter, quarter, quarter, eigthDot, sixtenth, eigth, tieEigthHalf, eigthDot, sixtenth, halfDot, eigthDot, sixtenth, quarter, eigthDot, sixtenth, quarter, eigthDot, sixtenth,//18 NOTES, END OF FIRST REPEAT
//|                     17                                      |             18             |                         19                               |
   quarter, eigthDot, sixtenth, eigth, eigth, eigthDot, sixtenth, halfDot, eigthDot, sixtenth, tripeletE, tripeletE, tieTripletEHalf, eigthDot, sixtenth, //15 NOTES
//|             20            |                         21                               |               22           |                   23                          |
   halfDot, eigthDot, sixtenth, tripeletE, tripeletE, tieTripletEHalf, eigthDot, sixtenth, halfDot, eigthDot, sixtenth, tripeletE, tripeletE, tieTripletEHalf, end_song //15 NOTES
 }; 
//#pragma CODE_SEG NON_BANKED

//Timer Channel 5 interrupt service routine
void interrupt 13 handler(){
 tone(noteValue);
}//interrupt 13


//RTI Interrupt service routine
//RTI acts as a ms_delay
void interrupt 7 RTI_ISR(){        
  CRGFLG= 0x80;   //clear real-time interrupt flag!!!    
     
  if(restValue == 0){
   noteValue = *(noteP+j);
   restValue = *(restP+j);
   //xsound_on();
   j++;
  } 
  restValue--; //decrease rest time 
}//interrupt 7

#pragma CODE_SEG DEFAULT 

void TetrisThemeA(char playSong){
    i = 0;
    
    while(playSong == playTetris){      
     if(i < 99){
      pitch = tetrisScore[i] / 2;
      rest = tetrisDelay[i] / 2;      
      *noteP = pitch; //make the note pointer = to pitch
      *restP = rest;  //rest pointer is now = to rest
      sound_on();
      i++;   
     }else{
       i=0;
     }//else      
    }//while
}//TetrisThemeA 

void PokemonTitle(char playSong){
    i = 0;
    
    while(playSong == playPokemon){     
     if(i < 245){
      pitch = pokemontitleScore[i] / 2;
      rest = pokemontitleDelay[i];
      sound_on();
      ms_delay(rest);
      i++;   
     }else{
       i=0;
     }//else      
    }//while
}//PokemonTitle 

void IndianaJones(char playSong){
    i = 0;
        
    while(playSong == playIndianaJones){      
     if(i < 97){
      pitch = indianajonesScore[i] / 2;
      rest = indianajonesDelay[i];
      sound_on(); 
      ms_delay(rest);
      i++;
     }else{
       i=0;
     }//else     

    }//while 
              
}//IndianaJones


/*******************************************************/
/****USED FOR TESTING PITCHES FOR SPEAKER AUDIBILITY****/
/*******************************************************/
void TestPitch(char playSong){
  
 while(playSong == playTestPitch){ 
 pitch = 0xFFFF;
 sound_on();
 ms_delay(quarter);
 }
}//TestPitch

    
/*****************************************************/
/*             ms_delay function                     */
/*         creates a millisecond delay               */
/*         Input: n (in milliseconds)                */
/*****************************************************/
void ms_delay(int n){
  int i,j;
  for(i=0; i<n; i++){
    for(j=0;j<6248;j++){
    }//for
  }//for
}//ms_delay



void sound_init(){
  TIOS =  0xA0; //select output compares 5 & 7                               
  TSCR2 = 0x04; //div by 16: 24MHz/16=1.5MHz, timer prescale                                 
  TSCR1 = 0x80; //enable timer                                               
                                                                             
  TC5 = TCNT;   //initialize count on TC5
  TC7 = TCNT;   //initialize count on TC7
  
  OC7M |= 0x20;  //pulse train out PT5
  OC7D |= 0x20;  //PT5 goes high on TC7 match
  TCTL1 |= 0x08; //PT5 low on TC5 match
  TCTL1 &= 0x04;  
}//sound_init



void sound_on(){
  TSCR1 = 0x80;      //enable timer
  TIE |= 0x20;       //enable TC5 interupts
  asm{
    cli
  } //enable interrupts
}//sound_on



void sound_off(){
 DisableInterrupts;
  TSCR1 = 0x00;       //disable timer
  TIE &= 0x20;        //disable TC5 interrupts
}//sound_off



void tone(int pitch){
  
  
  TC5 += pitch;
  TC7 = TC5;      //TC7 = TC6 + pitch
  pitch += TC7;   //add pitch
  TC5 = pitch;      //TC5 = TC7 + pitch
  TFLG1 = 0xA0;   //clear both C7F and C5F
}//tone


void SetClk24(void) //Set the clock speed to 24Mhz
{
   CLKSEL &= 0x7F;
   PLLCTL |= 0x40;
   SYNR = 0x02;
   REFDV = 0x01;
   while(!(0x08 & CRGFLG)); //Wait for tracking to lock in
   CLKSEL |= 0x80;
}
