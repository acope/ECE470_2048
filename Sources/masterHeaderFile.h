//Located in Main.c

extern char *pArray, *pArrayTemp;

//located in 2048Logic.c
void arrayReduceWhiteSpaces(char array[]);
void reduceColumnWhiteSpaces(char array[], char colNumber);
void arrayCondence(char array[]);
void columnCondence(char array[], char column);
void rotateRight(void);

//Located in SPI.c

void displayGameBoard(char *matrix);
void SPI0_init(void);
void SPI0_outChar(char cx);
char getPixel(unsigned char pictureNumber, int pixel);

//Located in sprites.c
//11 Sprites
extern const char sprite2[];
extern const char sprite4[];
extern const char sprite8[];
extern const char sprite16[];
extern const char sprite32[];
extern const char sprite64[];
extern const char sprite128[];
extern const char sprite256[];
extern const char sprite512[];
extern const char sprite1024[];
extern const char sprite2048[];

//Located in conteoller.h
void enablePortH(void);
void PortH_ISR_Enable(void);

//located in title.c
extern const char title[];

extern volatile char direction;
extern UP, DOWN, LEFT, RIGHT, RESTART;