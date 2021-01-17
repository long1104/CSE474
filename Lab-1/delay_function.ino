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
    tft.setCursor(0,0);
                         //moves the cursor to the top left corner to start a new line
    tft.fillScreen(BLACK);      
    Serial.print("time (ms) taken for clearing screen: ");
    Serial.println((millis()-start));                   //fills the screen with black to remove any previous items/text
    tft.print(printable); 
                           //prints the characters on the screen
    if(*ms>0 && *ms < 2147483)
    {
        delay(*ms);                             //delays for 'ms' milliseconds if the delay is valid (ms greater than zero but less than maximum), otherwise it would wait a long time if ms<0, as delay takes unsigned
    }
  
    return;
}
