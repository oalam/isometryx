#ifndef WORKOUT_H
#define WORKOUT_H

#include "HX711.h"
#include "WorkoutStats.h"
#include "Climber.h"


enum WorkoutType {
  ASSESSMENT_BODYWEIGHT,
  WARMUP,
  ASSESSMENT_MAXFORCE,
  WORKOUT_MAXHANGS
};

enum WorkoutState {
  HANGING,
  RESTING,
  IDLE
};


class Workout {
  public:
    Workout(WorkoutType type, unsigned short numReps, unsigned short numSets, unsigned short repHangingDurationMs, unsigned short restDurationMs, float maxPercentLimit) {
      mType = type;
      mNumReps = numReps;
      mNumSets = numSets;
      mRepHangingDurationMs = repHangingDurationMs;
      mRestDurationMs = restDurationMs;
      mMaxPercentLimit = maxPercentLimit; 
    }


    float strenghToWeight() {
      return climber.strenghToWeight(stats.maxVal());
    }

    float percentMax() {
      float mMax = climber.getMaxForceL() + climber.getMaxForceR();
      if (mMax != 0.0f)
        return stats.last() * 100.0f / mMax;
      else return 0.0f;
    }

    float timeOverLimit() {
      return mTimeOverLimit / 1000.0f;
    }

    float restingTime() {
      return mRestingTime/ 1000.0f;
    }

     float restDurationMs() {
      return mRestDurationMs/ 1000.0f;
    }

    Climber getClimber() {
      return climber;
    }

    WorkoutState getState(){
      return state;
    }

    WorkoutType getType(){
      return mType;
    }

    WorkoutStats getStats() {
      return stats;
    }
    int getRepsCount() {
      return mRepsCount;
    }

    int getNumReps() {
      return mNumReps;
    }

    void reset() {
      mTimeOverLimit = 0;
      mRestingTime = 0;
      mStartTime = millis();
      stats.reset();
    }


    void printRep(){
      Serial.print(F("Rep "));
      Serial.print(mRepsCount);
      Serial.print(F("/"));
      Serial.print(mNumReps);      
    }

    void setup() {
      stats.setup();
      climber.load();
    }

    void loop() {
      stats.loop();


      if (percentMax() >= mMaxPercentLimit) {
        mTimeOverLimit += 200;
      }

      if (climber.getMaxForceL() < mMaxL ) {
        climber.setMaxForceL(mMaxL);
      }

      if (climber.getMaxForceR() < mMaxR) {
        climber.setMaxForceR(mMaxR);
      }

      if (state == IDLE && stats.last() > 2) {
        state = HANGING;
        onRepStart();
        printRep();
        Serial.println(F(" started"));
      }
      else if (state == HANGING && stats.last() <= 2) {
        state = IDLE;
        onRepAbort();
        printRep();
        Serial.println(F(" aborted"));
        reset();
      } else if (state == HANGING && mTimeOverLimit > mRepHangingDurationMs) {
        
        // handle reps
        mRepsCount ++;
        if (mRepsCount >= mNumReps) {
          mRepsCount = 0;
          mNumSets++;
        }

        // handle sets
        if (mSetsCount >= mNumSets) {
          mRepsCount = 0;
          mNumSets++;
        }

        // reset stuff
        state = RESTING;
        onRepFinish();
        printRep();
        Serial.println(F(" finished"));
        reset();
        onRestStart();
      }
      else if (state == RESTING && mRestingTime >= mRestDurationMs) {
        state = IDLE;
        onRestFinish();
        Serial.println(F("rest finished"));
        reset();
      }else if (state == RESTING && mRestingTime < mRestDurationMs) {
        mRestingTime += 200;
        Serial.println(F(" started"));
      }
      repLoop();
    }

  protected:

    virtual void onRepStart() = 0;
    virtual void onRepFinish() = 0;
    virtual void onRepAbort() = 0;
    virtual void onRestStart() = 0;
    virtual void onRestFinish() = 0;
    virtual void repLoop() = 0;

    unsigned short mRepsCount = 0;
    unsigned short mSetsCount = 0;
    unsigned short mNumReps;
    unsigned short mNumSets;
    unsigned short mRepHangingDurationMs;
    unsigned short mRestDurationMs;
    float mMaxPercentLimit;
    float mMaxL = 0.0f;
    float mMaxR = 0.0f;
    WorkoutState state = IDLE;


    unsigned long mStartTime;
    unsigned long mTimeOverLimit;
    unsigned long mRestingTime;

    WorkoutType mType;
    Climber climber;
    WorkoutStats stats;
};

#endif
