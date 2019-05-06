
#include "Config.h"
#include "Workout.h"
#include "Screen.h"
#include "EventHandler.h"

Workout workout;
Screen screen(workout);
EventHandler eventHandler(workout);

void setup() {
  Serial.begin(57600);

  eventHandler.setup();
  workout.setup();
  screen.setup();
}

void loop() {
  eventHandler.loop();
  
  // collect data accordingly to current workout
  workout.loop();

  // display data to screen
  screen.loop();

  // wait a little
  delay(200);
}
