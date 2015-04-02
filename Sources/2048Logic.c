/*
This is a file of functions that have NOT been tested nor compiled.

Currently the function take a 4x4 array of chars. 

I am thinking of chanigng this to use pointers, but I am not that far ahead yet
This would prove to be way easier to work with then, assuming I am using somesort of 
parallel/interrupt driven display mechanism.
*/
#include "masterHeaderFile.h"

char array1[16], array2[16];
char *pArray = array1; //This will allways be pointing to the good data
char *pArrayTemp = array2; //This will allways be pointing to the temp array

void arrayReduceWhiteSpaces(char array[])
{
	char column = 0;

	for(column=0; column <4; column++)
	{
		reduceColumnWhiteSpaces(array, column);
	}
}

void reduceColumnWhiteSpaces(char array[], char colNumber)
{
	char c;

	for(c = 0; c < 4; c++)
	{
		if((array[colNumber+c*4+4]==0) && (array[colNumber + c*4] != 0)) //If there is a "white Space" at row+1 and row has a number
		{
			array[colNumber+c*4+4] = array[colNumber+c*4]; //Bring down number
			array[colNumber + c*4] = 0; //set row = 0
		}
	}	
}

void arrayCondence(char array[])
{
	char c;

	for(c = 0; c<4;c++)
	{
		columnCondence(array, c);
	}
}

void columnCondence(char array[], char column)
{
	char c;

	for(c = 3; c>= 0; c--) //Looky there, a forloop that goes down instead of up
	{
		if(array[c*4 + column]==array[c*4-4+column]) //if row and row+1 numbers are equal
		{
			array[c*4 + column] *= 2; //double value
			array[c*4 - 4 + column] = 0; //clear upper value
		}
	}
}

char* rotateRight(char *matrix)
{
	char *temp; //used when swapping the two matrixes
	char cA[]= {23,8,4,0};
	char c, cAValue;

	//I hope this works
	for(c =0; c<16; c++)
	{
		cAValue = cA[c % 4];
		*(pArrayTemp + c) = *(matrix + cAValue + (c/4));
	}
	//swap matrix
	temp = matrix; 
	matrix = pArrayTemp;
	pArrayTemp = temp;
	//give back the matix
	return matrix;	
}

/* pointer example 
retreived from http://www.tutorialspoint.com/cprogramming/c_pointer_to_an_array.htm

#include <stdio.h>

int main ()
{
    an array with 5 elements 
   double balance[5] = {1000.0, 2.0, 3.4, 17.0, 50.0};
   double *p;
   int i;

   p = balance;
 
    output each array element's value 
   printf( "Array values using pointer\n");
   for ( i = 0; i < 5; i++ )
   {
       printf("*(p + %d) : %f\n",  i, *(p + i) );
   }

   printf( "Array values using balance as address\n");
   for ( i = 0; i < 5; i++ )
   {
       printf("*(balance + %d) : %f\n",  i, *(balance + i) );
   }
 
   return 0;
}
*/
