#ifdef __cplusplus
extern "C" {
#endif


// The control pins for the LCD can be assigned to any digital or
// analog pins...but we'll use the analog pins as this allows us to
// double up the pins with the touch screen (see the TFT paint example).
#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0

#define LCD_RESET A4

//Coordinate references
#define ORIGIN_Y 0
#define ORIGIN_X 0
//Text sizes
#define MED_SCRIPT 2
#define SMALL_SCRIPT 1

//Default value for printed data
#define DEFAULT_FLOAT -1
#define DEFAULT_ALARM 0
#define DEFAULT_BOOL 0

#define ALARM_NUM_PRINTS 4
#define BATTERY_NUM_PRINTS 1
#define MEASURE_NUM_PRINTS 6

#define YP A3  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 9   // can be a digital pin
#define XP 8   // can be a digital pin
#define TS_MINX 120
#define TS_MAXX 900

#define TS_MINY 70
#define TS_MAXY 920

#define BLACK   0x0000
#define PURPLE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define PURPLE 0x780F
#define WHITE   0xFFFF
#define BACKGROUND_COLOR 0xFFFF

#define MIN_PRESSURE 0
#define MAX_PRESSURE 1000

#define NUM_OF_TASKS 5


void Scheduler();
void loop();
void setup();


#ifdef __cplusplus
}
#endif
