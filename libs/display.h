#ifndef DISPLAY_H
#define DISPLAY_H

extern void clearDisplay();
extern void entryModeSet();
extern void initDisplay();
extern void moveCursor(void);
extern void write_lcd(char c);

#endif