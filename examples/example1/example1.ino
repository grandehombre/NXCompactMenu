/*
  this example shows how to create a menu with multiple sub-menus. It has been tested on the RAM-constrained Arduino Nano
  
  The menu handler has been designed to work with small amounts of RAM, such as when using a Nano (2KB of RAM)
  All fixed strings are stored in PROGMEM and there is code in the menu library that handles them as needed.

  1may2020, Nick Damoulakis


Note that none of this code is directly needed by the menu library. However, you will need to supply pointers to
functions such as clearScreen(), setCursor() and printText() and those functions will need to access the display.
To that end, they will need to use the 'oled' variable, created below.
If your display driver is not shown here, that doesn't matter. 
Use whatever code is needed to create and initialise a display object such as the oled object below and use that 
in the menuSupport file to interact with the display. See menuSupport for more info

None of the code in this file is specifically needed by the menu lib; just the display (oled) object.

The keyhandler shown in loop() simply monitors 4 switches (up, down, back and ok) and calls the relevant menu code.
See keyHandler file for more info
*/ 

// Uncomment the line that represents your display driver
// ......................................................
//#define LCD_IS_SH1106
//#define LCD_IS_SSD1306
#define LCD_IS_ST7789


#if defined(LCD_IS_SH1106) || defined(LCD_IS_SSD1306)
  #include <SSD1306Ascii.h>
  #include <SSD1306AsciiWire.h>
  
  #define I2C_ADDRESS 0x3C      // 0x3C or 0x3D
  #define RST_PIN -1            // Define proper RST_PIN if required.
  SSD1306AsciiWire oled;
#endif

#if defined(LCD_IS_ST7789)
  #include <Adafruit_GFX.h>    // Core graphics library
  #include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
  #include <SPI.h>             // Arduino SPI library
  
  // ST7789 TFT module connections
  #define TFT_CS     -1
  #define TFT_RST    8  // define reset pin, or set to -1 and connect to Arduino RESET pin
  #define TFT_DC     9  // define data/command pin
  
  // Initialize Adafruit ST7789 TFT library
  Adafruit_ST7789 oled = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);
#endif


#include "NXCompactMenu.h"
NXCompactMenu nixMenu;

MENU mainMenu;
MENU configMenu;
MENU configMenuPwr;
MENU configMenuChannel;

// dummy struc, used for this example
struct DeviceParams {
  int channel;
  int power;
} deviceParams;

/**

*/
void setup() {
  Serial.begin(115200);
  delay(350);
  Serial.println("");

  Serial.println(F("Initialising ..."));

  // initialise display

#if defined(LCD_IS_SH1106) || defined(LCD_IS_SSD1306)
  //this is needed for my LCD. not relevant to nix_menu
  Wire.begin();
  Wire.setClock(400000L);

#if defined(LCD_IS_SH1106)
  #if RST_PIN >= 0
    oled.begin(&SH1106_128x64, I2C_ADDRESS, RST_PIN);
  #else     // RST_PIN >= 0
    oled.begin(&SH1106_128x64, I2C_ADDRESS);
  #endif    // RST_PIN >= 0
#endif

#if defined(LCD_IS_SSD1306)
  #if RST_PIN >= 0
    oled.begin(&Adafruit128x64, I2C_ADDRESS, RST_PIN);
  #else // RST_PIN >= 0
    oled.begin(&Adafruit128x64, I2C_ADDRESS);
  #endif // RST_PIN >= 0
#endif

  oled.setFont(Adafruit5x7);
  oled.clear();
#endif

#if defined(LCD_IS_ST7789)
  oled.init(240, 240, SPI_MODE2);    // Init ST7789 display 240x240 pixel
  oled.fillScreen(ST77XX_BLACK);
  oled.setRotation(2);
  oled.setTextSize(2);
  oled.setTextWrap(false);
#endif

  oled.println(F("Initialising ..."));

  // initialise keyboard-handler
  keyInit(&nixMenu);

  // initialise menu
  menuInit();

  Serial.print("Menu version: "); Serial.println(nixMenu.GetLibVersion());
  Serial.println(F("Ready."));
  oled.println(F("Ready."));
}

/**

*/
void loop() {
  keyHandler();
}

/*
  util showing available memory...this is handy when working with Nanos, as they only have 2KB of RAM
*/
int freeRAM () {
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}
