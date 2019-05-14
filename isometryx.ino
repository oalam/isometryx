#include "Config.h"
#include "WorkoutFactory.h"
#include "EventHandler.h"

WorkoutFactory factory;
EventHandler eventHandler(factory);

void setup() {
  Serial.begin(115200);
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }

  factory.setup();
  eventHandler.setup();  
}

void loop() {
  eventHandler.loop();  
  factory.loop();
  
  delay(FRAME_RATE_MS);
}
