//located in 2048Logic.c
void arrayReduceWhiteSpaces(char array[]);
void reduceColumnWhiteSpaces(char array[], char colNumber);
void arrayCondence(char array[]);
void columnCondence(char array[], char column);

//Located in SPI.c



void displayGameBoard(char *matrix);
void SPI0_init(void);
void SPI0_outChar(char cx);
char getPixel(unsigned char pictureNumber, int pixel);