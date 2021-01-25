/****************************************** 
* function name: printDelay 
* function inputs: printable is the array of characters to be displayed, allows string literals; ms is the paramaterized delay in milliseconds, *pointer
* function outputs: returns void
* function description: prints the input characters on the display and delays for a parameterized delay
* author: Chase Arline, Long Nguyen
******************************************/ 
void printDelay(const char printable[], long *ms)
{
    long start = millis();
    tft.setCursor(0,0);                                 //moves the cursor to the top left corner to start a new line
    tft.fillScreen(BLACK);
    tft.print(printable);                               //prints the characters on the screen
    long unfinishedDelay = *ms - (millis()-start);
    if(unfinishedDelay>0 && unfinishedDelay < 2147483)
    {
        delay(unfinishedDelay);                
    }
    return;
}
