/**
   Keybaord handler
   In this example, a module with 4 switches (up, down, back and ok) is used.

*/

// what GPIOs are the switches connected to. Note that the switches have a common pin, which is connected to ground.
#define SWITCH_UP 5
#define SWITCH_DOWN 4
#define SWITCH_BACK 3
#define SWITCH_OK 6

#define KEY_CHECK_DELAY 150   // 150 ms between key checks

unsigned long int lastKeyCheck = 0;

enum KeyCodes_e {
  KEY_NONE, KEY_UP, KEY_DOWN, KEY_BACK, KEY_OK
};

struct KEYS_PRESSED {
  bool keyUp;
  bool keyDown;
  bool keyBack;
  bool keyOk;
  KeyCodes_e pressedKey;
} keysPressed;


// pointer to menu library object
NXCompactMenu *mnu;

void keyInit(NXCompactMenu *nixMenu) {
  pinMode(SWITCH_UP, INPUT_PULLUP);
  pinMode(SWITCH_DOWN, INPUT_PULLUP);
  pinMode(SWITCH_OK, INPUT_PULLUP);
  pinMode(SWITCH_BACK, INPUT_PULLUP);

  mnu = nixMenu;
}

/**

*/
void keyHandler() {
  if (millis() - lastKeyCheck > KEY_CHECK_DELAY) {
    lastKeyCheck = millis();

    keysPressed.keyUp = digitalRead(SWITCH_UP) == LOW;
    keysPressed.keyDown = digitalRead(SWITCH_DOWN) == LOW;
    keysPressed.keyOk = digitalRead(SWITCH_OK) == LOW;
    keysPressed.keyBack = digitalRead(SWITCH_BACK) == LOW;

    keysPressed.pressedKey = KEY_NONE;

    // We keep track of each individual switch/key that was pressed. However, since it is possible that more than one key was down at the time,
    // we also need to keep a single variable that holds the key we choose out of all of those that may have been pressed.
    // In such cases, we only recognise keys in the following order: Ok, Back, Up, Down, e.g. if Ok and some other keys were pressed, 
    // we'll only recognise Ok
    do {
      if (keysPressed.keyOk) {
        keysPressed.pressedKey = KEY_OK;
        break;
      }

      if (keysPressed.keyBack) {
        keysPressed.pressedKey = KEY_BACK;
        break;
      }

      if (keysPressed.keyUp) {
        keysPressed.pressedKey = KEY_UP;
        break;
      }

      if (keysPressed.keyDown) {
        keysPressed.pressedKey = KEY_DOWN;
        break;
      }
    } while (false);    // dummy loop that allows us multiple exits to one program location.

    // if something was pressed, go process it
    if (keysPressed.pressedKey != KEY_NONE) {
      keyHandler2();

      keysPressed.pressedKey = KEY_NONE;
      keysPressed.keyUp = false;
      keysPressed.keyDown = false;
      keysPressed.keyOk = false;
      keysPressed.keyBack = false;
    }
  }
}

/**

*/
void keyHandler2() {
  // pressing the Ok key when not in a menu, causes the main menu to be shown.
  if (keysPressed.pressedKey == KEY_OK && mnu->activeMenu == NULL) {
    mnu->activeMenu = &mainMenu;
    mnu->activeMenu->curPos = 0;    // set the first entry as the 'current' menu entry
  }

  // execute relevant key handlers
  if (mnu->activeMenu != NULL) {
    switch (keysPressed.pressedKey) {
      case KEY_UP:
        mnu->menuUpDown(-1);
        break;

      case KEY_DOWN:
        mnu->menuUpDown(1);
        break;

      case KEY_OK:
        mnu->menuSelect();
        break;

      case KEY_BACK:
        mnu->menuBack();
        break;
    }
  }
}
