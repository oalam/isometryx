#ifndef SCREEN_H
#define SCREEN_H

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "Config.h"
#include "Climber.h"
#include "WorkoutFactory.h"

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

class Screen {
  public:
    Screen(WorkoutFactory &factory) :  mWorkoutFactory(factory) {}

    void setup() {
      // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
      if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
        Serial.println(F("SSD1306 allocation failed"));
        for (;;); // Don't proceed, loop forever
      }
      display.display();
      // Clear the buffer
      display.clearDisplay();
    }

    void displayBodyWeightAssessment(Workout *workout) {


#ifdef DEBUG
      if (workout->getStats().last() > 0.5) {
        Serial.print(F("current (kg) : "));
        Serial.println(workout->getStats().last());
      }
#endif

      display.clearDisplay();
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(0, 0);
      display.print(valueToString(workout->getClimber().getMaxForceL(), 1));
      display.print(F(" "));
      display.println(valueToString(workout->getClimber().getMaxForceR(), 1));

      display.print(valueToString(workout->getStats().last(), 1));
      display.print(F(" "));
      display.println(valueToString(workout->getStats().last(), 1));

      if (workout->percentMax() > 2) {
        display.print(valueToString(workout->percentMax(), 0));
        display.print(F("% "));
      } else {
        display.print(F("  0% "));
      }
      display.print(valueToString(workout->timeOverLimit(), 0));
      display.println(F("s"));

      display.print(valueToString(workout->getRepsCount(), 0));
      display.print(F("/"));
      display.println(valueToString(workout->getNumReps(), 0));
      display.display();
    }

    void displayWarmup(Workout *workout) {
      display.clearDisplay();
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(0, 0);
      display.println(F("Warmup"));
      display.display();
    }


    void displayMaxForceAssessment(Workout *workout) {
      display.clearDisplay();
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(0, 0);

      switch (workout->getState()) {
        case IDLE : 
          display.println(F("Max Force"));
          display.println(F("best of 8 7\" reps"));
          break;

        case HANGING :
          display.print(valueToString(workout->getClimber().getMaxForceL(), 1));
          display.print(F(" "));
          display.println(valueToString(workout->getClimber().getMaxForceR(), 1));

          display.print(valueToString(workout->getStats().last(), 1));
          display.print(F(" "));
          display.println(valueToString(workout->getStats().last(), 1));

          if (workout->percentMax() > 2) {
            display.print(valueToString(workout->percentMax(), 0));
            display.print(F("% "));
          } else {
            display.print(F("  0% "));
          }
          display.print(valueToString(workout->timeOverLimit(), 0));
          display.println(F("s"));

          display.print(valueToString(workout->getRepsCount(), 0));
          display.print(F("/"));
          display.println(valueToString(workout->getNumReps(), 0));
        
        case RESTING :
          display.println(F("Rest (s)"));

          display.print(valueToString(workout->restingTime(), 0));
          display.print(F("/"));
          display.println(valueToString(workout->restDurationMs(), 0));
      }


      display.display();
    }

    void displayMaxHangsWorkout(Workout *workout) {
      display.clearDisplay();
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(0, 0);
      display.println("Max hangs");
      display.display();
    }

    void loop() {
      Workout *workout = mWorkoutFactory.getCurrentWorkout();


     
        switch (workout->getType()) {
          case ASSESSMENT_MAXFORCE : displayMaxForceAssessment(workout); break;
        }
    }

  protected:
    WorkoutFactory &mWorkoutFactory;
};

#endif
