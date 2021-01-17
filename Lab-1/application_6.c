#include <stdio.h>
#include <stdlib.h>

void delays(int delay);
void printDelay(char disChar[], unsigned long *delay);

int main()
{
    int delay;

    scanf("%i", &delay);                // Get the users input for the amount of delay in second

    while (1) {
        printDelay("A B C D", &delay);
        printDelay("", &delay);
    }
}

