#include <stdio.h>

//  function prototypes

//  get data from the user
void getData(int* aValuePtr);

void main (void)
{
	//  declare a shared variable and a pointer to it
	int myValue;
	int* myPtr = &myValue;		//  let myPtr point to myValue

	//  get data from the user
       
	getData(myPtr);
       
	//  display the data as a character
	printf("The data is: %c \n", *myPtr);

}

//  prompt the user for some data and return it through a shared
//  variable pointed to by valuePtr
//
//  inputs:  	pointer to a container in which to place the data
//  outputs:	none
//  function:	the routine accepts a pointer to a container in which to store data from a user,
//				it prompts for the data, accepts the data, displays it, and returns

/* Old version of the snippet codes that cause error:
void getData(int* valuePtr) {
        //declare a temp place to store the data
        int tempValue;

        //  let valuePtr point to it
        valuePtr = &tempValue;

        //  prompt for data
        printf("Please enter a single digit between 0-9 \n");

        //  get the data
        *valuePtr = getchar();

        //  display its value as a character
        printf("The data is: %c \n", *valuePtr);

        return;
}
*/
// The error in the snippet code above is that we assigned the valuePtr
// to a local variable stored on the stack, i.e., tempValue. Since we don't backup the address
// stored in valuePtr before reassign it to new address, when we exit
// the getData function, the local variable (tempValue) will be disappeared,
// and we will never be able to retrieve the value from that variable anymore.
// Thus, when we back in main function, and we want to print it one more,
// we cannot print the value stored at the address stored in valuePtr anymore.
//
// We can just simply delete the local variable and the line that we reassign the value
// in the valuePtr.
// The new version is below.

void getData(int* valuePtr)
{
	//  prompt for data
	printf("Please enter a single digit between 0-9 \n");

	//  get the data
	*valuePtr = getchar();

	//  display its value as a character
	printf("The data is: %c \n", *valuePtr);

	return;

}
