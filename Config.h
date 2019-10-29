#include <Adafruit_BluefruitLE_SPI.h>
#include <Adafruit_BLE.h>

#ifndef CONFIG_H
#define CONFIG_H

//#define DEBUG

#include <EEPROM.h>
#include "HX711.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Isometryx config
const int MAX_ABORTED_REPS = 3;
const int FRAME_RATE_MS = 100;
const int DISPLAY_TIME_MS = 2000;
const int MAX_IDLE_TIME_MS = 10000;

const int BODYWEIGHT_EEPROM_ADDRESS = 0; 


// HX711 circuit wiring
const short L_LOADCELL_DOUT_PIN = 3;
const short L_LOADCELL_SCK_PIN = 2;

const short R_LOADCELL_DOUT_PIN = 5;
const short R_LOADCELL_SCK_PIN = 4;

HX711 scaleUp;
HX711 scaleDown;


// SSD1306 wiring
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET    -1 // Reset pin # (or -1 if sharing Arduino reset pin)

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


// BLE wiring
#define BUFSIZE                        128   // Size of the read buffer for incoming data
#define VERBOSE_MODE                   true  // If set to 'true' enables debug output
#define BLUEFRUIT_SPI_CS               8
#define BLUEFRUIT_SPI_IRQ              7
#define BLUEFRUIT_SPI_RST              -1    // Optional but recommended, set to -1 if unused

Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);


String valueToString(const float value, int precision) {
  String valueString = "" ;
  char buff[10];
  dtostrf(value, 3, precision, buff);
  valueString += buff;

  return valueString;
}

float readFloat(unsigned int addr) {
  union {
    byte b[4];
    float f;
  } data;
  for (int i = 0; i < 4; i++) {
    data.b[i] = EEPROM.read(addr + i);
  }
  return data.f;
}

void writeFloat(unsigned int addr, float x) {
  union {
    byte b[4];
    float f;
  } data;
  data.f = x;
  for (int i = 0; i < 4; i++) {
    EEPROM.write(addr + i, data.b[i]);
  }
}

// A small helper
void error(const __FlashStringHelper*err) {
  Serial.println(err);
  while (1);
}

#endif
