#ifndef WORKOUT_H
#define WORKOUT_H

#include "HX711.h"
#include "LoadStats.h"
#include "Climber.h"
#include "Mode.h"

Climber climber;
LoadStats loadStatsL;
LoadStats loadStatsR;
HX711 scaleR;
HX711 scaleL;
Mode mode;

enum WorkoutState {
  HANGING,
  RESTING
};




class Workout {
  public:
    Workout()  {
    }

    float last() {
      return loadStatsL.last() + loadStatsR.last();
    }

    float lastL() {
      return loadStatsL.last();
    }

    float lastR() {
      return loadStatsR.last();
    }

    float mean() {
      return loadStatsL.mean() + loadStatsR.mean();
    }

    float maxValL() {
      return loadStatsL.maxVal();
    }

    float maxValR() {
      return loadStatsR.maxVal();
    }

    float maxVal() {
      return loadStatsL.maxVal() + loadStatsR.maxVal();
    }

    float strenghToWeight() {
      return climber.strenghToWeight(maxVal());
    }

    float percentMax() {
      float mMax = climber.getMaxForceL() + climber.getMaxForceR();
      if (mMax != 0.0f)
        return last() * 100.0f / mMax;
      else return 0.0f;
    }

    float timeOverLimit() {
      return mTimeOverLimit / 1000.0f;
    }

    Climber getClimber() {
      return climber;
    }

    Mode getMode() {
      return mode;
    }

    int getRepsCount() {
      return mRepsCount;
    }

    int getNumReps() {
      return mNumReps;
    }

    /**
       to switch between modes
    */
    Mode setNextMode() {
      mRepsCount = 0;
      mSetsCount = 0;
      reset();
      mode.next();
    }

    void reset() {
      mTimeOverLimit = 0;
      mStartTime = millis();
      loadStatsL.reset();
      loadStatsR.reset();
    }

    void checkState() {
      if (state == RESTING && last() > 2) {
        state = HANGING;
        reset();
      }
      else if (state == HANGING && last() <= 2) {
        state = RESTING;
        reset();
      } else if (state == HANGING && mTimeOverLimit > 7000) {
        mRepsCount ++;

        if (mRepsCount >= mNumReps) {
          mRepsCount = 0;
          mNumSets++;
        }
        mTimeOverLimit = 0;
        state = RESTING;
        reset();
      }

      if (percentMax() >= MAX_PERCENT_LIMIT) {
        mTimeOverLimit += 200;
      }



    }

    void setup() {
      scaleL.begin(L_LOADCELL_DOUT_PIN, L_LOADCELL_SCK_PIN);
      scaleL.set_scale(22.04622621848775f);
      scaleL.tare();

      scaleR.begin(R_LOADCELL_DOUT_PIN, R_LOADCELL_SCK_PIN);
      scaleR.set_scale(22.04622621848775f);
      scaleR.tare();

      climber.load();
      mode.setup();
    }


    void loop() {
      if (scaleL.wait_ready_timeout(1000) && scaleR.wait_ready_timeout(1000)) {
        float readingL = scaleL.get_units() / 1000.0f;
        float readingR = scaleR.get_units() / 1000.0f;
        unsigned long currentTime = millis();
        loadStatsL.addData(readingL);
        loadStatsR.addData(readingR);

        if (readingL > mMaxL)
          mMaxL = readingL;

        if (readingR > mMaxR)
          mMaxR = readingR;

        if (climber.getMaxForceL() < mMaxL ) {
          climber.setMaxForceL(mMaxL);
        }

        if (climber.getMaxForceR() < mMaxR) {
          climber.setMaxForceR(mMaxR);
        }


      } else {
        Serial.println(F("HX711 not found."));
      }

      checkState();

      mode.loop();

    }

  protected:

    int mRepsCount = 0;
    int mSetsCount = 0;
    int mNumReps = 6;
    int mNumSets = 1;
    float mMaxL = 0.0f;
    float mMaxR = 0.0f;
    WorkoutState state = RESTING;


    long mStartTime;
    long mTimeOverLimit;
};

#endif
