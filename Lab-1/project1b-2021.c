#include <stdio.h>

//  this is a simple routine that demonstrates how to fill and diaplay an array of characters

/* Old version:
        int i = 0;                                                                      //  declare a working variable

        char myArray[5];                                                        //  declare a character array

        for (i = 0; i <= 5; i++)                                        //  fill array with characters
        {
                //  fill with the ascii characters A..F
                //  65 is the ascii value for A

                myArray[i]= 65+i;
        }

        for (i = 0; i <= 5; i++)                                        //  display the array
        {
                printf("%c \n", myArray[i]);
        }

        printf("\n");

        return;
*/

// The error from the old version is that the array of characters is declared as length of 5.
// However, we want to print 6 characters for A to F. So when we start to fill the arrays with
// 6 characters (A to F), it would be not yelling any errors since C doesn't have out of bound
// checking, which it will allow you write to the index that out of bound. However, the out of
// bound index that you write will not guarantee to hold the value you put in since it is not
// registerd at the declaration of the array. Thus, later it might be used to stored different value
// by computer. That is the reason why when you try to print the value at index 5 of arrays 
// (which is out of bound), it will gives you some weird character that is different from the one you stored
// and you don't expect it. That is the bug in the snippet of codes above.
// 
// In order to fix it, we can just simply declare the array as length of 6, which is now the index 5
// will be registered for holding a character and will not be used for other purpose.


void main(void)
{
	int i = 0;									//  declare a working variable

	char myArray[6];							//  declare a character array

	for (i = 0; i <= 5; i++)					//  fill array with characters
	{
		//  fill with the ascii characters A..F
		//  65 is the ascii value for A

		myArray[i]= 65+i;
	}

	for (i = 0; i <= 5; i++)					//  display the array
	{
		printf("%c \n", myArray[i]);
	}

	printf("\n");

	return;
}
