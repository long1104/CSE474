#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
    int delay;

    scanf("%i", &delay);      // take the input delay from the users

    while (1) {
        printf("A B C D");
        printDelay(delay);
        system("cls");
        printDelay(delay);
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
