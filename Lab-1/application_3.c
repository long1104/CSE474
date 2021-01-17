#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
    int delay;

    scanf("%i", &delay);

    while (1) {
        f1Data(delay);
        f2Clear(delay);
    }
}

// Function that simulate a delay
// Arguments:
//    - delay (int): delay time in second
void printDelay(int delay) {
    clock_t start = clock();

    while (clock() - start < delay * CLOCKS_PER_SEC) {
    }
}

// Function that print out the character "A B C D"
// Arguments:
//    - delay (unsigned long): delay time in second
void f1Data(unsigned long delay) {
    printf("A B C D");
    printDelay(delay);
    system("cls");
}

// Clear the screen then delay for amount of time of delay
// argument
// Arguments:
//    - delay (unsigned long): delay time in second
void f2Clear(unsigned long delay) {
    system("cls");
    printDelay(delay);
}
