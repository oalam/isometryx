#ifndef CONFIG_H
#define CONFIG_H



// HX711 circuit wiring
const int L_LOADCELL_DOUT_PIN = 2;
const int L_LOADCELL_SCK_PIN = 3;

const int R_LOADCELL_DOUT_PIN = 4;
const int R_LOADCELL_SCK_PIN = 5;


// SSD1306 wiring
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET    -1 // Reset pin # (or -1 if sharing Arduino reset pin)


//#define DEBUG

String valueToString(const float value, int precision) {
  String valueString = "" ;
  char buff[10];
  dtostrf(value, 3, precision, buff);
  valueString += buff;

  return valueString;
}

#endif
