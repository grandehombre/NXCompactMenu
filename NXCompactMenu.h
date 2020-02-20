#ifndef NIX_MENU_H_INCLUDED
#define NIX_MENU_H_INCLUDED

#include <Arduino.h>

#define MAX_MENU_TITLE_SIZE 64

typedef int (* Fn)();
typedef void (* FnVoid)();
typedef void (* FnSetCurs)(int x, int y);
typedef void (* FnPrint)(char *msg);

enum MenuHandlerType {
  NORMAL,
  GO_BACK
};

struct MENU_ENTRY {
  __FlashStringHelper *title; // Thi sis what is displayed unless titleNear is not NULL
  char *titleNear = NULL;   // only needed if the title contents are dynamic.
  Fn handler;
  //  int (* handler)();
  //  MenuHandlerType handlerType = NORMAL;
};

struct MENU {
  __FlashStringHelper *title; // Thi sis what is displayed unless titleNear is not NULL
  char *titleNear = NULL;   // only needed if the title contents are dynamic.
  int count;
  bool isShown;
  int curPos;
  MENU *previousMenu = NULL;
  MENU_ENTRY *entries;    // pointer to array of menu entries
};

class NXCompactMenu {
  public:
    MENU *activeMenu;

    NXCompactMenu();
    void menuInit(MENU *startupMenu,
                  void (* clearScreen)(),
                  void (* setCursor)(int x, int y),
                  void (* print)(char *msg),
                  void (* noMenu)(),
                  int displayWidth,
                  int displayHeight,
                  int displayLines
                 );
    char *GetLibVersion();
    int displayMenu();
    void menuSelect();
    void menuSwitch(MENU *toGoTo);
    void menuBack();
    void menuUpDown(int direction);
    void createMenuEntry(MENU *mnu, int index, __FlashStringHelper *title, Fn handler);

  private:
    char *versionNumber;
	
    // temporary buffer used to hold strings copied from PROGMEM
    char tmpTitle[MAX_MENU_TITLE_SIZE + 1];

    FnVoid _clearScreen;
    FnPrint _print;
    FnSetCurs _setCursor;
    void (* _noMenu)();
	int _displayWidth;
	int _displayHeight;
	int _displayLines;
};

#endif
