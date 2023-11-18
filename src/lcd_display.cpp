#include "lcd_display.h"
#include <LiquidCrystal_I2C.h>

#define I2C_ADDR 0x27
#define LCD_COLUMNS 20
#define LCD_LINES 4

LiquidCrystal_I2C LCD(I2C_ADDR, LCD_COLUMNS, LCD_LINES);

void setupDisplay() {
    LCD.init();
    LCD.backlight();
    LCD.setCursor(0, 0);
}

void showInDisplay(const char *message, int column, int row, bool clean) {
  if (clean) LCD.clear();
  LCD.setCursor(column, row);
  LCD.print(message);
}