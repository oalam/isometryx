#include "Config.h"
#include "Workout.h"

Workout workout;

void setup() {
  Serial.begin(1000000);


/*
  float latestBodyWeight = readFloat(BODYWEIGHT_EEPROM_ADDRESS);
  Serial.print(F("Read climber data from EEPROM : "));
  Serial.println(latestBodyWeight);
  workout.setBodyWeight(latestBodyWeight);
*/

  scaleDown.begin(L_LOADCELL_DOUT_PIN, L_LOADCELL_SCK_PIN);
  scaleDown.set_scale(22.04622621848775f);
 //scaleDown.set_scale(10.83f);
  scaleDown.tare(10);

  scaleUp.begin(R_LOADCELL_DOUT_PIN, R_LOADCELL_SCK_PIN);
  scaleUp.set_scale(22.04622621848775f);
 //scaleUp.set_scale(2.35f);
  scaleUp.tare(10);


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


  if ( !ble.begin(VERBOSE_MODE) )  {
    error(F("Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring?"));
  }
   //ble.sendCommandCheckOK("AT+GAPDEVNAME=Isometryx");
 /* if ( ! ble.factoryReset() ){
     error(F("Couldn't factory reset BLE device"));
  }
  ble.echo(false);
  ble.info();*/
  ble.setMode(BLUEFRUIT_MODE_DATA);
  ble.verbose(false);  // debug info is a little annoying after this point!
  delay(1000);

}

void loop() {
  workout.loop();

    
  
  delay(FRAME_RATE_MS);
}
