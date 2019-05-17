#include "Config.h"
#include "WorkoutFactory.h"
#include "EventHandler.h"

WorkoutFactory factory;
EventHandler eventHandler(factory);

void setup() {
  Serial.begin(250000);
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println(F("Isometryx"));
  display.display();
  delay(500);

  factory.setup();
  eventHandler.setup();  
}

void loop() {
  eventHandler.loop();  
  factory.loop();
  
  delay(FRAME_RATE_MS);
}
