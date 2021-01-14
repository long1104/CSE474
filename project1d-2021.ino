// IMPORTANT: ELEGOO_TFTLCD LIBRARY MUST BE SPECIFICALLY
// CONFIGURED FOR EITHER THE TFT SHIELD OR THE BREAKOUT BOARD.
// SEE RELEVANT COMMENTS IN Elegoo_TFTLCD.h FOR SETUP.
//Technical support:goodtft@163.com

#include <Elegoo_GFX.h>    // Core graphics library
#include <Elegoo_TFTLCD.h> // Hardware-specific library

// The control pins for the LCD can be assigned to any digital or
// analog pins...but we'll use the analog pins as this allows us to
// double up the pins with the touch screen (see the TFT paint example).
#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0

#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin

// When using the BREAKOUT BOARD only, use these 8 data lines to the LCD:
// For the Arduino Uno, Duemilanove, Diecimila, etc.:
//   D0 connects to digital pin 8  (Notice these are
//   D1 connects to digital pin 9   NOT in order!)
//   D2 connects to digital pin 2
//   D3 connects to digital pin 3
//   D4 connects to digital pin 4
//   D5 connects to digital pin 5
//   D6 connects to digital pin 6
//   D7 connects to digital pin 7
// For the Arduino Mega, use digital pins 22 through 29
// (on the 2-row header at the end of the board).

// Assign human-readable names to some common 16-bit color values:
#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

//the delay used for parsing integers from the serial monitor, measured in milliseconds
#define TIMEOUT_DELAY 80


Elegoo_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
// If using the shield, all control and data lines are fixed, and
// a simpler declaration can optionally be used:
// Elegoo_TFTLCD tft;



/****************************************** 
* function name: setup
* function inputs: void
* function outputs: void, serial monitor output
* function description: Sets up both the Arduino serial and the TFT display
* author: NA
******************************************/ 
void setup(void) {
  Serial.begin(9600);
  Serial.println(F("TFT LCD test"));


#ifdef USE_Elegoo_SHIELD_PINOUT       // This is defined in Elegoo_TFTLCD.h
  Serial.println(F("Using Elegoo 2.4\" TFT Arduino Shield Pinout"));
#else
  Serial.println(F("Using Elegoo 2.4\" TFT Breakout Board Pinout"));
#endif

  Serial.print("TFT size is "); Serial.print(tft.width()); Serial.print("x"); Serial.println(tft.height());

  tft.reset();

   uint16_t identifier = tft.readID();
   if(identifier == 0x9325) {
    Serial.println(F("Found ILI9325 LCD driver"));
  } else if(identifier == 0x9328) {
    Serial.println(F("Found ILI9328 LCD driver"));
  } else if(identifier == 0x4535) {
    Serial.println(F("Found LGDP4535 LCD driver"));
  }else if(identifier == 0x7575) {
    Serial.println(F("Found HX8347G LCD driver"));
  } else if(identifier == 0x9341) {
    Serial.println(F("Found ILI9341 LCD driver"));
  } else if(identifier == 0x8357) {
    Serial.println(F("Found HX8357D LCD driver"));
  } else if(identifier==0x0101)
  {     
      identifier=0x9341;
       Serial.println(F("Found 0x9341 LCD driver"));
  }
  else if(identifier==0x1111)
  {     
      identifier=0x9328;
       Serial.println(F("Found 0x9328 LCD driver"));
  }
  else {
    Serial.print(F("Unknown LCD driver chip: "));
    Serial.println(identifier, HEX);
    Serial.println(F("If using the Elegoo 2.8\" TFT Arduino shield, the line:"));
    Serial.println(F("  #define USE_Elegoo_SHIELD_PINOUT"));
    Serial.println(F("should appear in the library header (Elegoo_TFT.h)."));
    Serial.println(F("If using the breakout board, it should NOT be #defined!"));
    Serial.println(F("Also if using the breakout, double-check that all wiring"));
    Serial.println(F("matches the tutorial."));
    identifier=0x9328;
  
  }
  tft.begin(identifier);
  return;
}

/****************************************** 
* function name: loop 
* function inputs: void, serial monitor is used to input delay
* function outputs: void
* function description: main function to accomplish goal of flashing characters on a screen with a user specified delay
* author: Chase Arline, Long Nguyen
******************************************/ 
void loop(void) 
{                                   
    tft.setTextColor(GREEN); tft.setTextSize(2);  //sets the text color and text size for printing on the display
    Serial.flush();                               //flushes all serial data to make sure user-input is clear
    long msDelay=0;                                //user-defined delay in milliseconds, max is ~ 2147483 seconds
    while(msDelay<=0)                             //checks the serial monitor and waits until data (other than 0) is sent into it, in order to get the delay from the user
    {                                 
        msDelay=Serial.parseInt();                //sets the user defined delay                       
    }
    Serial.setTimeout(TIMEOUT_DELAY);             //sets the timeout delay at 20 milliseconds to reduce the waiting time of Serial.parseInt
    long goodDelay=0;      
    while(1)
    {
        goodDelay=msDelay-20;           //goodDelay is the user-defined delay minus the timeout delay from Serial.parseInt, this negates the delay effect of Serial.parseInt timeout "delay"
        long start = millis();
        printDelay("A B C D", &goodDelay);         //procedure call in order to print 'A B C D' and delay by 'ms' milliseconds
        Serial.print("time(ms) taken to print characters: ");
        Serial.println(millis()-start);
        start = millis();
        goodDelay=msDelay-TIMEOUT_DELAY;
        printDelay("", &(goodDelay));             //procedure call in order to print nothing and delay by goodDelay
        goodDelay=Serial.parseInt();              //checking to see if the user wants to set a new delay for the program
        if(goodDelay>0)                          // ^
        {                                        
            msDelay=goodDelay;           //set the normal delay to the newly-defined delay if it is greater than zero
        }
        Serial.print("time(ms) taken to clear characters: ");
        Serial.println(millis()-start);
    }
    return;
}

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
