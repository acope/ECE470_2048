#include "masterHeaderFile.h"
#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */

void displayGameBoard(char *matrix)
{
	unsigned int sCount = 0;
	unsigned int lCount = 0;
	unsigned int bCount = 0;
	unsigned char pixel, hArrayElement, column, vArrayElement, row;
	char c = 0; 
	
//	SPI0_outChar(c); //dummy becuase
	
	while (sCount < (150*200))//keep going untill the pixels are acounted for
	{
		
		if(sCount > (150-80)*200) //when we get to the gameboard;
	//	lCount++; //#fix, prob don't need this ether, just for logic sake

			if(lCount % 200> 80 || lCount % 200 < 80 + 80)
			{
				bCount ++;
				row = bCount / 80; //#fix there might be a casting issue here, I don't remember how they work.
				hArrayElement = row / 20;
				column = bCount % 80; 
				vArrayElement = column / 20;
				// get the right picture from the colections of pictures
				//arrayElementNumber = *(matrix + hArrayElement + vArrayElement);
				// get the pixel from the picture
				//pixel = arrayElement[column % 20 + row % 20];

				pixel = getPixel(*(matrix + hArrayElement + vArrayElement), (column % 20 + row % 20));

				SPI0_outChar(pixel);
			}
		}else{
			SPI0_outChar(0x00);//send out a black pixel
		}

		sCount++;
	}
}

char getPixel(unsigned char pictureNumber, int pixel)
{
	char frank[200];

	return 0xFF;
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