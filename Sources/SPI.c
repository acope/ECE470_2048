#include "masterHeaderFile.h"
#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */

void displayGameBoard(char *matrix)
{
	unsigned int sCount = 0;
	unsigned int bCount = 0, pixelNum = 0;
	unsigned char pixel = 0, hArrayElement = 0, column = 0, vArrayElement = 0, row = 0, picture = 0;

	
//	SPI0_outChar(c); //dummy becuase
	
	while (sCount < (150*200))//keep going untill the pixels are acounted for
	{
		
		if(sCount > (150-80)*200){
		  //when we get to the gameboard;
	//	lCount++; //#fix, prob don't need this ether, just for logic sake

			if(sCount % 200 > 80 && sCount % 200 < 80 + 81)
			{
				
				row = bCount / 80; //#fix there might be a casting issue here, I don't remember how they work.
				vArrayElement = row / 20;
				column = bCount % 80; 
				hArrayElement = column / 20;
				// get the right picture from the colections of pictures
				picture = *(matrix + (vArrayElement*4) + hArrayElement);
				// get the pixel from the picture
				pixelNum = (column % 20) + (row % 20)*20;

				pixel = getPixel(picture, pixelNum);

				SPI0_outChar(pixel);
			  bCount ++;
			}else{
			  SPI0_outChar(0x00);
			  
			}
			
		}else{
			SPI0_outChar(0x00);//send out a black pixel
			
		}

		sCount++;
	}
	
}

char getPixel(unsigned char pictureNumber, int pixel)
{

	
	switch(pictureNumber){
	  			
		case 1:
			return *(sprite2+pixel);
		
		case 2:
		  return *(sprite4+pixel);
		  
		case 3:
		  return *(sprite8+pixel);
		  
		case 4:
		  return *(sprite16+pixel);
		  
		case 5:
		  return *(sprite32+pixel);
		  
		case 6:
		  return *(sprite64+pixel);
		  
		case 7:
		  return *(sprite128+pixel);
		  
		case 8:
		  return *(sprite256+pixel);
		  
		case 9:
		  return *(sprite512+pixel);
		  
		case 10:
		  return *(sprite1024+pixel);
		  
		case 11:
		  return *(sprite2048+pixel);
		  
		default:
			return 0x00;
	}
	
}

void SPI0_init(void)
{
	SPI0BR = 0x10;  //set baud rate to 6Mhz
	SPI0CR1 = 0x53; //enable SPI, date shift on SCK's riging edge
	SPI0CR2 = 0x02; //Disable bidriectional mode, SPI stops in wait mote
	DDRS != 0x80;
	PTS != 0x80;
}

void SPI0_outChar(char cx)
//From the HCS12 Book, PG.  446
{
	char temp;
	
	while(!(SPI0SR_SPTEF));
	SPI0DR = cx;
	while(!(SPI0SR_SPIF));
	temp = SPI0DR;
}