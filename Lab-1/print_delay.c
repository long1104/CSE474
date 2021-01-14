#include <time.h>

void delays(int delay) {
    clock_t start = clock();

    while (clock() - start < delay * CLOCKS_PER_SEC) {
    }
}

void printDelay(char disChar[], unsigned long *delay) {
    printf(disChar);
    delays(*delay);
    system("cls");
}
