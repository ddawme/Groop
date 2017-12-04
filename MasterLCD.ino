
// Libraries  
#include <LiquidCrystal.h>
#include <Wire.h>

// Variables for LCD.print
float temp_reading = 0;

// Variables for LCD Control
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  
  lcd.begin(16, 2);   // Set up the LCD's number of columns and rows:
  
}

void loop() {

  lcd.print("Dane Rules");
  
  lcd.setCursor(0, 1);        // Set the cursor to column 0, line 1

  lcd.print("!!!!!!!!!!!!!!  ");
}

