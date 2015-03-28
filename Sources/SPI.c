void displayGameBoard(char *matrix)
{
	unsigned int sCount = 0;
	unsigned int lCount = 0;
	unsigned int bCount = 0;
	unsigned char pixel, hArrayElement, column, vArrayElement, row;
	while (sCount < 150*100)//keep going untill the pixels are acounted for
	{
		if(sCount > (150-80)*200) //when we get to the gameboard;
		{
			lCount++; //#fix, prob don't need this ether, just for logic sake

			if(lCount % 200> 80 || lCount % 200 < 80 + 80)
			{
				bCount ++;
				row = bCount / 80; //#fix there might be a casting issue here, I don't remember how they work.
				hArrayElement = row / 20;
				column = bcount % 80; 
				vArrayElement = column / 20;
				// get the right picture from the colections of pictures
				//arrayElementNumber = *(matrix + hArrayElement + vArrayElement);
				// get the pixel from the picture
				//pixel = arrayElement[column % 20 + row % 20];

				pixel = getPixel(*(matrix + hArrayElement + vArrayElement), (column % 20 + row % 20));

				SPIO_Send(pixel);
			}
		}else{
			SPOI_Send(0x00);//send out a black pixel
		}

		sCount++;
	}
}

char getPixel(unsigned char pictureNumber, int pixel)
{
	char frank[200];

	return frank[pixel];
}
void SPI0_init(void)
{
	SPI0BR = 0x10;  //set baud rate to 6Mhz
	SPI0CR1 = 0x50; //enable SPI, date shift on SCK's riging edge
	SPI0CR2 = 0x02; //Disable bidriectional mode, SPI stops in wait mote
}

void SPI0_outChar(void)
//From the HCS12 Book, PG.  446
{
	char temp;
	while(!(SPI0SR & SPTEF));
	SPI0DR = cx;
	while(!(SPI)SR & SPIF);
	temp = SPI0DR;
}