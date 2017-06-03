#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include "EmonLib.h"                   // Include Emon Library

EnergyMonitor emon1;                   // Create an instance

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x3f, 16,2);

float power=0;

void setup()
{
  Serial.begin(19200);
  lcd.begin();
  emon1.voltage(1, 220, 0);  // Voltage: input pin, calibration, phase_shift
  emon1.current(2, 6.5);       // Current: input pin, calibration.
  delay(2000);
}

void loop()
{

  emon1.calcVI(30,2000);         // Calculate all. No.of half wavelengths (crossings), time-out
  int voltage   = emon1.Vrms;             //extract Vrms into Variable
  float current   = emon1.Irms; 
  
  Serial.print("V");
  Serial.print(voltage);
  Serial.print("I");
  Serial.print(current);
  Serial.print("A");
  
  if (current>0.07)
  {
    power=voltage*current;
  }
  
  lcd.setCursor(0, 0);
  lcd.print("V:");
  lcd.print(voltage);
  lcd.print(" V I:");
  lcd.print(current);
  lcd.print(" A     ");
  lcd.setCursor(0, 1);
  lcd.print("S:");
  lcd.print(power);
  lcd.print(" VA     ");
 
  delay(1000); // wait 10 milliseconds before the next loop   // Do nothing here...
  power=0;
}
