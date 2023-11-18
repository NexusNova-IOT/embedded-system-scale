#ifndef LCD_DISPLAY_H
#define LCD_DISPLAY_H
void setupDisplay();
void showInDisplay(const char* message, int row = 0, int column = 0, bool clean = true);
#endif