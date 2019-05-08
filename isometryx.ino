#include "Config.h"
#include "WorkoutFactory.h"
#include "Screen.h"
#include "EventHandler.h"

WorkoutFactory factory;
Screen screen(factory);
EventHandler eventHandler(factory);

void setup() {
  Serial.begin(57600);

  factory.getCurrentWorkout()->setup();
  eventHandler.setup();
  screen.setup();
}

void loop() {
  eventHandler.loop();  
  
  // collect data accordingly to current workout
  factory.getCurrentWorkout()->loop();

  // display data to screen
  screen.loop();

  // wait a little
  delay(200);
}
