#include "Config.h"
#include "WorkoutFactory.h"

WorkoutFactory factory;

void setup() {
  Serial.begin(250000);


  scaleDown.begin(L_LOADCELL_DOUT_PIN, L_LOADCELL_SCK_PIN);
  scaleDown.set_scale(22.04622621848775f);
  scaleDown.tare(5);

  scaleUp.begin(R_LOADCELL_DOUT_PIN, R_LOADCELL_SCK_PIN);
  scaleUp.set_scale(22.04622621848775f);
  scaleUp.tare(5);


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
  display.println(F("Hang"));
  display.println(F("Harder"));
  display.display();
  delay(500);

}

void loop() {
  factory.loop();
  
  delay(FRAME_RATE_MS);
}
