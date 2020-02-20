/*
 * This Library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This Library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the Arduino SSD1306Ascii Library.  If not, see
 * <http://www.gnu.org/licenses/>.


  Arduino menu handler, 
  --------------------
  Written by: Nick Damoulakis
  

  This menu handler has been designed to work with small amounts of RAM, such as when using a Nano.
  All fixed strings are stored in PROGMEM and there is code in the menu library that handles them as needed.

  v1.0.0    14feb2020
 */
 
#include <Arduino.h>
#include <NXCompactMenu.h>


NXCompactMenu::NXCompactMenu() {
  this->versionNumber = (char *)"1.00";
}

/**
   startUpMenu    menu to start with
   clearScreen    pointer to function that clears the screen
   setCursor      pointer to function that sets the cursor
   print          pointer to function that prints a message
*/
void NXCompactMenu::menuInit(MENU *startupMenu,
                       void (* clearScreen)(),
                       void (* setCursor)(int x, int y),
                       void (* print)(char *msg),
                       void (* noMenu)(),        // what to do when no menu is active
                       int displayWidth,
                       int displayHeight,
					   int displayLines
                      ) {
  this->_clearScreen = clearScreen;
  this->_setCursor = setCursor;
  this->_print = print;
  this->_noMenu = noMenu;
  this->_displayWidth = displayWidth;
  this->_displayHeight = displayHeight;
  this->_displayLines = displayLines;

  this->activeMenu = startupMenu;
}

/**

*/
char *NXCompactMenu::GetLibVersion() {
  return this->versionNumber;
}

/**
  Display the currently active menu
*/
int NXCompactMenu::displayMenu() {
  int retCode = 0;

  if (this->activeMenu != NULL) {
    // Print menu title
		(_clearScreen)();
    // setCursor 2nd param is row number, not pixel. 1st param is character column not pixel
    (_setCursor)(0, 0);
    if (this->activeMenu->titleNear != NULL) {
      (_print)(this->activeMenu->titleNear);
    } else {
      int l = strlen_PF((uint_farptr_t)this->activeMenu->title);
      if (l > 0 && l <= MAX_MENU_TITLE_SIZE) {
        strlcpy_PF(tmpTitle, (uint_farptr_t)this->activeMenu->title, l + 1);

        l = strlen(tmpTitle);

        (_print)(tmpTitle);
      }
    }

    int menuDispLines = this->activeMenu->count;
    if (menuDispLines > this->_displayLines - 1) {  // one line is taken up by the menu title
      menuDispLines = this->_displayLines - 1;
    }

Serial.print("menu lines, curPos: ");Serial.print(menuDispLines);Serial.print(", ");Serial.println(this->activeMenu->curPos);

    int startMenuEntry = 0;
    // if the selected menu is below the bottom of the screen, make sure it is visible
    if (this->activeMenu->curPos + 1 > menuDispLines) {
      startMenuEntry = this->activeMenu->curPos - menuDispLines + 1;
      if (startMenuEntry < 0) {
        startMenuEntry = 0;
      }
    }
	
	Serial.print("start menu entry: ");Serial.println(startMenuEntry);

    // Print menu entries
    for (int i = 0; i < menuDispLines; i++) {
      // setCursor 2nd param is row number, not pixel. 1st param is character column not pixel
      (_setCursor)(2, i + 1);

      if (this->activeMenu->entries[startMenuEntry + i].titleNear != NULL) {
        (_print)(this->activeMenu->entries[startMenuEntry + i].titleNear);
      } else {
        int l = strlen_PF((uint_farptr_t)this->activeMenu->entries[startMenuEntry + i].title);
        if (l > 0 && l <= MAX_MENU_TITLE_SIZE) {
			
          char tmpTitle[l + 1];
		  
          strlcpy_PF(tmpTitle, (uint_farptr_t)this->activeMenu->entries[startMenuEntry + i].title, l + 1);
          (_print)(tmpTitle);
        }
      }
    }

    activeMenu->isShown = true;

    // setCursor 2nd param is row number, not pixel. 1st param is character column not pixel
    (_setCursor)(0, this->activeMenu->curPos - startMenuEntry + 1);
    (_print)((char *)">");
  }

  return retCode;
}

/**
  Select current menu entry
*/
void NXCompactMenu::menuSelect() {
  if (this->activeMenu->entries[this->activeMenu->curPos].handler == NULL) {
    menuBack();
  } else {
    (this->activeMenu->entries[this->activeMenu->curPos].handler)();
    displayMenu();
  }
}

/**
  go to a new menu and remember the previous one
*/
void NXCompactMenu::menuSwitch(MENU *toGoTo) {
  if (this->activeMenu != NULL) {
    this->activeMenu->isShown = false;

    toGoTo->previousMenu = this->activeMenu;
    this->activeMenu = toGoTo;
    this->activeMenu->curPos = 0;
  }
}

/**
  go back to previous menu
*/
void NXCompactMenu::menuBack() {
  if (this->activeMenu->previousMenu != NULL) {
    this->activeMenu->isShown = false;
    this->activeMenu = this->activeMenu->previousMenu;

    displayMenu();
  } else {
    this->activeMenu = NULL;
    (_noMenu)();
  }
}

/**

*/
void NXCompactMenu::menuUpDown(int direction) {
  switch (direction) {
    case 1:
      if (++this->activeMenu->curPos >= this->activeMenu->count) {
        this->activeMenu->curPos = 0;
      }
      break;

    case -1:
      if (--this->activeMenu->curPos < 0 ) {
        this->activeMenu->curPos = this->activeMenu->count - 1;
      }
      break;
  }

  displayMenu();
}

void NXCompactMenu::createMenuEntry(MENU *mnu, int index, __FlashStringHelper *title, Fn handler) {
  MENU_ENTRY *me = new MENU_ENTRY();
  memcpy(&mnu->entries[index], me, sizeof(MENU_ENTRY));
  mnu->entries[index].titleNear = NULL;
  mnu->entries[index].title = title;
  mnu->entries[index].handler = handler;
}

