
// variable to hold the 4 possible power-level strings. This is only needed for this example
const __FlashStringHelper *powerText[4];

int fontHeight = -1;
int displayLines = -1;
int displayWidth;
int displayHeight;

/**
  Sample menu initialiser
*/
void menuInit() {
  mainMenu.title = F("Main menu");   // This text lives in PROGMEM...
  mainMenu.isShown = false;
  mainMenu.curPos = 0;

  // Define the number of entries and allocate memory based on that
  mainMenu.count = 13;    // this is used further down to ensure you don't try to use more menu entries than you have allowed for
  mainMenu.entries = (MENU_ENTRY*)malloc(mainMenu.count * sizeof(MENU_ENTRY));

  int ctr = 0;

  // This menu has more entries than can fit in the 128x64 LCD I am using it with. This is done so it can demostrate menu scrolling.
  // ------------------------------------------------------------------------------------------------------------------------------

  // This ctr check is a safe way to ensure that you do not try to add more menus than you have allocated memory for.
  if (ctr < mainMenu.count) {
    // Create a menu entry, for menu mainMenu, index ctr, title to use, menu-entry handler pointer.
    // Menu handler functions are defined as:
    // int menuItemHandler()
    // i.e. they return an int (0=ok) and receive no parameters)

    // I have intentionally created too many entries here so they will overflow small screens and you can then see the
    // auto-scroll behaviour in action.

    nixMenu.createMenuEntry(&mainMenu, ctr++, F("menu 1"), &menuItemHandler1);
  }

  if (ctr < mainMenu.count) {
    nixMenu.createMenuEntry(&mainMenu, ctr++, F("menu 2"), &menuItemHandler1);
  }

  if (ctr < mainMenu.count) {
    nixMenu.createMenuEntry(&mainMenu, ctr++, F("menu 3"), &menuItemHandler1);
  }

  if (ctr < mainMenu.count) {
    nixMenu.createMenuEntry(&mainMenu, ctr++, F("menu 4"), &menuItemHandler1);
  }

  if (ctr < mainMenu.count) {
    nixMenu.createMenuEntry(&mainMenu, ctr++, F("menu 5"), &menuItemHandler1);
  }

  if (ctr < mainMenu.count) {
    nixMenu.createMenuEntry(&mainMenu, ctr++, F("menu 6"), &menuItemHandler1);
  }

  if (ctr < mainMenu.count) {
    nixMenu.createMenuEntry(&mainMenu, ctr++, F("menu 7"), &menuItemHandler1);
  }

  if (ctr < mainMenu.count) {
    nixMenu.createMenuEntry(&mainMenu, ctr++, F("menu 8"), &menuItemHandler1);
  }

  if (ctr < mainMenu.count) {
    nixMenu.createMenuEntry(&mainMenu, ctr++, F("menu 9"), &menuItemHandler1);
  }

  if (ctr < mainMenu.count) {
    nixMenu.createMenuEntry(&mainMenu, ctr++, F("menu 10"), &menuItemHandler1);
  }

  if (ctr < mainMenu.count) {
    nixMenu.createMenuEntry(&mainMenu, ctr++, F("Config menu"), &menuItemHandlerConfigMenu);
  }

  if (ctr < mainMenu.count) {
    nixMenu.createMenuEntry(&mainMenu, ctr++, F("PrintParams"), &menuItemHandlerPrintParamsMenu);
  }

  if (ctr < mainMenu.count) {
    nixMenu.createMenuEntry(&mainMenu, ctr++, F("Exit menu"), NULL);
  }


  // .............. Config submenu
  // ..........................
  configMenu.title = F("Config");
  configMenu.isShown = false;
  configMenu.curPos = 0;

  // Define the number of entries and allocate memory based on that
  configMenu.count = 3;
  configMenu.entries = (MENU_ENTRY*)malloc(configMenu.count * sizeof(MENU_ENTRY));

  ctr = 0;

  if (ctr < configMenu.count) {
    nixMenu.createMenuEntry(&configMenu, ctr++, F("Set transmit power"), &menuItemHandlerConfigPwrMenu);
  }

  if (ctr < configMenu.count) {
    nixMenu.createMenuEntry(&configMenu, ctr++, F("Set channel"), &menuItemHandlerConfigChannelMenu);
  }

  if (ctr < configMenu.count) {
    nixMenu.createMenuEntry(&configMenu, ctr++, F("Save changes"), &menuItemHandlerConfigMenuSave);
  }

  // .............. Config submenu, set transmit power submenmu
  // ..........................
  configMenuPwr.title = F("Transmit power");
  configMenuPwr.isShown = false;
  configMenuPwr.curPos = 0;

  // Define the number of entries and allocate memory based on that
  configMenuPwr.count = 4;     // 4 inside the switch and 1 after it!
  configMenuPwr.entries = (MENU_ENTRY*)malloc(configMenuPwr.count * sizeof(MENU_ENTRY));

  ctr = 0;

  // This ctr check, is a safe way to ensure that you do not try to add more menus than you have allocated memory for.
  if (ctr < configMenuPwr.count) {
    powerText[ctr] = F("20dbm");
    nixMenu.createMenuEntry(&configMenuPwr, ctr, powerText[ctr], &menuItemHandlerConfigMenuPower20);
    ctr++;
  }

  if (ctr < configMenuPwr.count) {
    powerText[ctr] = F("17dbm");
    nixMenu.createMenuEntry(&configMenuPwr, ctr, powerText[ctr], &menuItemHandlerConfigMenuPower17);
    ctr++;
  }

  if (ctr < configMenuPwr.count) {
    powerText[ctr] = F("14dbm");
    nixMenu.createMenuEntry(&configMenuPwr, ctr, powerText[ctr], &menuItemHandlerConfigMenuPower14);
    ctr++;
  }

  if (ctr < configMenuPwr.count) {
    powerText[ctr] = F("11dbm");
    nixMenu.createMenuEntry(&configMenuPwr, ctr, powerText[ctr], &menuItemHandlerConfigMenuPower11);
    ctr++;
  }


  // .............. Config submenu, set channel submenu. Note that the title of first entry in this submenu is dynamically changed
  // ..........................

  configMenuChannel.title = F("Channel");
  configMenuChannel.isShown = false;
  configMenuChannel.curPos = 0;
  configMenuChannel.count = 3;
  configMenuChannel.entries = (MENU_ENTRY*)malloc(configMenuChannel.count * sizeof(MENU_ENTRY));

  ctr = 0;

  if (ctr < configMenuChannel.count) {
    // allocate enough room in the title for a string like "Current channel xxx". menuItemHandlerConfigMenuGetChannel() will be
    // replacing the title text, whenever the Incr and Decr menus are invoked
    nixMenu.createMenuEntry(&configMenuChannel, ctr, F("Current channel    "), &menuItemHandlerConfigMenuGetChannel);
    getCurChannelText(configMenuChannel, ctr);
    ctr++;
  }

  if (ctr < configMenuChannel.count) {
    nixMenu.createMenuEntry(&configMenuChannel, ctr++, F("Incr channel"), &menuItemHandlerConfigMenuIncrChannel);
  }

  if (ctr < configMenuChannel.count) {
    nixMenu.createMenuEntry(&configMenuChannel, ctr++, F("Decr channel"), &menuItemHandlerConfigMenuDecrChannel);
  }


  // We now need to provide some info in the menu initialiser, which depend on our display.

#if defined(LCD_IS_SH1106) || defined(LCD_IS_SSD1306)
  displayWidth = oled.displayWidth();
  displayHeight = oled.displayHeight();
  fontHeight = oled.fontHeight() + 1;
  displayLines = oled.displayHeight() / fontHeight;
#endif

#ifdef LCD_IS_ST7789
  displayWidth = oled.width(),
  displayHeight = oled.height(),

  fontHeight = 18; // Hard-coded value
  displayLines = (oled.height() / fontHeight) - 1;
#endif

  Serial.print("Display width, height: "); Serial.print(displayWidth); Serial.print(", "); Serial.println(displayHeight);
  Serial.print("Display lines, font height: "); Serial.print(displayLines); Serial.print(", "); Serial.println(fontHeight);

  // DisplayLines is used by the menu lib to decide when to do scrolling of the menu entries. Very important number!
  // FontHeight is used by the setCursor routine below, to calculate the y position.
  // when setCursor is called, its x and y params are col and row, not pixel values.
  // They need to be converter to pixel values; See setCursor() below

  nixMenu.menuInit(
    &mainMenu,              // menu to start with
    clearScreen,            // pointer to a function that will clear the screen
    setCursor,              // pointer to a function that will set the cursor. it accepts X and Y params.
    printText,              // pointer to a function that will print text. it accepts a char *string
    menuItemHandlerNoMenu,  // pointer to a function that will do whatever you want when the user exits the menu(s) completely
    displayWidth,
    displayHeight,
    displayLines
  );
}


// Support methods needed by the menu class's init()
// ..........................................
// ..........................................
void clearScreen() {
#if defined(LCD_IS_SH1106) || defined(LCD_IS_SSD1306)
  oled.clear();
#endif

#ifdef LCD_IS_ST7789
  oled.fillScreen(ST77XX_BLACK);
#endif
}

/**
x and y params are col and row, not pixel values!
They need to be converted to pixel values, depending on the display you are using.
*/
void setCursor(int x, int y) {
  //  Serial.print("col, row: ");Serial.print(x);Serial.print(", ");Serial.println(y);
  
#if defined(LCD_IS_SH1106) || defined(LCD_IS_SSD1306)
  // this driver uses columns in the y param, so there is nothing for us to do, just pass it along
  // Howefer, we need to convert the x param to pixels by assuming a character width of 7.
  oled.setCursor(x * 7, y);
#endif

#ifdef LCD_IS_ST7789
  // this driver needs pixels in the y param, so we need to convcert columns --> pixels by multiplying by the font height.
  // We also need to convert the x param to pixels by assuming a character width of 8.
  oled.setCursor(x * 8, y * fontHeight);
#endif
}

void printText(char *msg) {
  oled.print(msg);
}

/**
  Method to be called when there is no active menu.
  It would generally clear the screen and show some message such as 'waiting for this to happen..."
*/
void menuItemHandlerNoMenu() {
  clearScreen();
  printText_PF(F("Waiting for something ..."));
}



// Helper functions for each menu entry
// ..........................................
// ..........................................


// Main menu entry handler for all the 'sample' entries in the main menu...
// ------------------------------------

/**

*/
int menuItemHandler1() {
  return 0;
}


// Config menu
// ------------------------------------

/**

*/
int menuItemHandlerConfigMenu() {
  // switch to the Config menu. Remember where we came from, so when the user pressed Back, we come back to this menu (Main, in my case)
  nixMenu.menuSwitch(&configMenu);
  return 0;
}


// 'Display config params' menu
// ------------------------------------

/**

*/
int menuItemHandlerPrintParamsMenu() {
  //Transceiver.PrintParameters();
  delay(1000);
  return 0;
}


int menuItemHandlerConfigMenuSave() {
  clearScreen();
  printText_PF(F("Saving settings ..."));

  //Transceiver.SaveParameters(PERMANENT);
  delay(1000);
  nixMenu.menuBack();

  return 0;
}


// Config menu, power
// ------------------------------------

/**

*/
int menuItemHandlerConfigPwrMenu() {
  nixMenu.menuSwitch(&configMenuPwr);

  // get current Tx power setting
  uint8_t power = deviceParams.power;

  Serial.print("Current Tx power: "); Serial.println(power);

  // find the corresponding menu entry and make it the 'current' one

  // copy the string from PROGMEM down to RAM so we can compare it
  // find its length
  Serial.print("pwr string "); Serial.println(powerText[power]);

  int l = strlen_PF(powerText[power]);
  
  // allocate buffer for it
  char tmp[l + 1];
  
  // copy it to RAM (from PROGMEM)
  strcpy_PF(tmp, powerText[power]);

  // now go through all the Tx power strings in PROGMEM and find one that matches ours
  for (int i = 0; i < 4; i++) {
    Serial.print("my string, other string: "); Serial.print(tmp); Serial.print(" "); Serial.println(powerText[i]);
    if (strcmp_PF(tmp, powerText[i]) == 0) {
      // found it, make it current
      nixMenu.activeMenu->curPos = i;
      nixMenu.displayMenu();
      break;
    }
  }

  return 0;
}

int menuItemHandlerConfigMenuPower20() {
  setTxPower(0);
  return 0;
}

int menuItemHandlerConfigMenuPower17() {
  setTxPower(1);
  return 0;
}

int menuItemHandlerConfigMenuPower14() {
  setTxPower(2);
  return 0;
}

int menuItemHandlerConfigMenuPower11() {
  setTxPower(3);
  return 0;
}


// Note the the 'power' needs to be a number betweem 0 and 3, as this is what we have strings for. See powerText at top of this file.
void setTxPower(uint8_t power) {
  //Transceiver.SetTransmitPower(power);
  deviceParams.power = power;
  Serial.print("Tx power set to: "); Serial.println(power);
}


/**

  // Config submenu, 'Channel'
  // ------------------------------------

  This also demonstrates how you can change a menu entry's title. Since you cannot change the text that is in PROGMEM, i.e. the 'title' member of the MENU_ENTRY
  an alternative title is provided, called 'titleNear' which is in RAM. See getCurChannelText()
*/
int menuItemHandlerConfigChannelMenu() {
  nixMenu.menuSwitch(&configMenuChannel);

  //  deviceParams = Transceiver.GetParameters();
  getCurChannelText(configMenuChannel, 0);
  return 0;
}

/**

*/
int menuItemHandlerConfigMenuGetChannel() {
  // get the current channel and change the title to reflect it
  getCurChannelText(configMenuChannel, 0);

  return 0;
}

/**

*/
int menuItemHandlerConfigMenuIncrChannel() {
  if (++deviceParams.channel > 31) {
    deviceParams.channel = 0;
  }

  //Transceiver.SetChannel(deviceParams.Channel);

  menuItemHandlerConfigMenuGetChannel();
  //  nixMenu.displayMenu();

  return 0;
}

/**

*/
int menuItemHandlerConfigMenuDecrChannel() {
  if (--deviceParams.channel == 255) {    // wrap-around...byte number
    deviceParams.channel = 31;
  }

  //  Transceiver.SetChannel(deviceParams.Channel);
  menuItemHandlerConfigMenuGetChannel();
  //  nixMenu.displayMenu();

  return 0;
}

/**
  Get the current channel number and build a string containing it.
  That string will be stored in the specified menu entry's 'near' title (i.e. the one that is in RAM and not in PROGMEM)
*/
char *getCurChannelText(MENU mnu, int index) {
  // temp buffer
  char buf[MAX_MENU_TITLE_SIZE + 1];

  sprintf(buf, "Cur channel: %d", deviceParams.channel);

  // if we have already allocated memory for a 'near' title (i.e. one that lives in RAM), free it.
  if (mnu.entries[index].titleNear != NULL) {
    free(mnu.entries[index].titleNear);
  }

  // Allocate memory to hold the new title and copy the one we created above into it.
  mnu.entries[index].titleNear = (char *)malloc(strlen(buf) + 1);
  strcpy(mnu.entries[index].titleNear, buf);

  // there is no actual need to return the pointer but it is handy ...
  return mnu.entries[index].titleNear;
}


// Helper function needed in this example code. The menu lib does this on its own
void printText_PF(__FlashStringHelper *msg) {
  oled.print(msg);
}

// Helper function needed in this example code. The menu lib does this on its own
void printText_PF(const __FlashStringHelper *msg) {
  oled.print(msg);
}
