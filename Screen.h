#ifndef SCREEN_H
#define SCREEN_H

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "Mode.h"
#include "Config.h"
#include "LoadStats.h"
#include "Climber.h"
#include "Workout.h"

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

class Screen {
  public:
    Screen(Workout &workout) : mWorkout(workout) {}

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

    void displayBodyWeightAssessment() {

      if (mWorkout.lastL() > 0.5 || mWorkout.lastR() > 0.5) {
        Serial.print(F("current (kg) : "));
        Serial.print(mWorkout.lastL());
        Serial.print(F(" - "));
        Serial.println(mWorkout.lastR());
      }

      display.clearDisplay();
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(0, 0);
      display.print(valueToString(mWorkout.getClimber().getMaxForceL(), 1));
      display.print(F(" "));
      display.println(valueToString(mWorkout.getClimber().getMaxForceR(), 1));

      display.print(valueToString(mWorkout.lastL(), 1));
      display.print(F(" "));
      display.println(valueToString(mWorkout.lastR(), 1));

      if (mWorkout.percentMax() > 2) {
        display.print(valueToString(mWorkout.percentMax(), 0));
        display.print(F("% "));
      } else {
        display.print(F("  0% "));
      }
      display.print(valueToString(mWorkout.timeOverLimit(), 0));
      display.println(F("s"));

      display.print(valueToString(mWorkout.getRepsCount(), 0));
      display.print(F("/"));
      display.println(valueToString(mWorkout.getNumReps(), 0));
      display.display();
    }

    void displayWarmup() {
      display.clearDisplay();
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(0, 0);
      display.println(F("Warmup"));
      display.display();
    }


    void displayMaxForceAssessment() {
      display.clearDisplay();
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(0, 0);
      display.println(F("Max Force"));
      display.println(F("best of 8 7\" reps"));
      display.display();
    }

    void displayMaxHangsWorkout() {
      display.clearDisplay();
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(0, 0);
      display.println("Max hangs");
      display.display();
    }

    void loop() {
      switch (mWorkout.getMode().getType()) {
        case ASSESSMENT_BODYWEIGHT : displayBodyWeightAssessment(); break;
        case WARMUP: displayWarmup(); break;
        case ASSESSMENT_MAXFORCE : displayMaxForceAssessment(); break;
        case WORKOUT_MAXHANGS : displayMaxHangsWorkout(); break;
      }
    }

  protected:
    Workout &mWorkout;
};

#endif
