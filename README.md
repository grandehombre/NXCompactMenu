# NXCompactMenu
Compact menu library for Arduino Nano and above

It is designed to run in MCUs with little RAM by making extensive use of PROGMEM. The strings stored in PROGMEM are temporarily copied into RAM, as neeed, send to the display and then released.

The menu structure consists of a top-level title, followed by an array of menu entries.
Each menu entry has a title and a pointer to a handler function.
Submenus are also supported and there is support for 'back to previous menu' functionality.

The meny library is completely display-driver agnostic.
The app developer provides function pointers for the following functions that handle all the interaction with the display:
  - clear screen
  - set cursor position
  - print text

Additionally, the developer provides, at menu init time, the following display params:
  - width in pixels
  - height in pixels
  - number of lines
  
The library uses the number of lines to determine of a menu needs to be 'scrolled' so the current menu option is visible.

The menu library has been tested with the following display drivers, so far:
  - SH1106 and SSD1306 (using the SSD1306Ascii library)
  - ST7789 (using the Adafruit_ST7789 library)

Please note that the menu library itself is not aware of anything to do with display drivers.
I mention the above drivers and libraries because that is what has been used in the example code provided.
