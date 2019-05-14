#ifndef CONFIG_H
#define CONFIG_H

//#define DEBUG

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Isometryx config
const int FRAME_RATE_MS = 100;
const int IDLE_TIME_MS = 2000;


// HX711 circuit wiring
const short L_LOADCELL_DOUT_PIN = 2;
const short L_LOADCELL_SCK_PIN = 3;

const short R_LOADCELL_DOUT_PIN = 4;
const short R_LOADCELL_SCK_PIN = 5;

// SSD1306 wiring
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET    -1 // Reset pin # (or -1 if sharing Arduino reset pin)

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);




String valueToString(const float value, int precision) {
  String valueString = "" ;
  char buff[10];
  dtostrf(value, 3, precision, buff);
  valueString += buff;

  return valueString;
}

void debug(const String str){
  #ifdef DEBUG
    Serial.print(str);
  #endif
}
void debug(const unsigned int str){
  #ifdef DEBUG
    Serial.print(str);
  #endif
}
void debug(const float str){
  #ifdef DEBUG
    Serial.print(str);
  #endif
}
void debug(const unsigned long str){
  #ifdef DEBUG
    Serial.print(str);
  #endif
}
#endif
