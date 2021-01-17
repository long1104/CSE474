#include <time.h>

// Function that simulate a delay
// Arguments:
//    - delay (int): delay time in second
void delays(int delay) {
    clock_t start = clock();

    while (clock() - start < delay * CLOCKS_PER_SEC) {
    }
}

// Print the input disChar with the given amount of delay
// Arguments:
//     - disChar (char[]): array of characters that we want to print
//     - delay (usigned long* ): the pointer to amount of time that we want to delay
void printDelay(char disChar[], unsigned long *delay) {
    printf(disChar);
    delays(*delay);
    system("cls");
}
