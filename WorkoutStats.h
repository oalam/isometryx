#ifndef WORKOUT_STATS_H
#define WORKOUT_STATS_H

#include "HX711.h"
#include <float.h>
#include <math.h>

#define sqr(x) ((x)*(x))


const unsigned int MIN_WEIGHT_THRESHOLD = 2;
const unsigned int MAX_TICK_WITHOUT_VALUE = 2;


HX711 scaleUp;
HX711 scaleDown;


class WorkoutStats {
  public:
    WorkoutStats() : mNumSamples(10), mCurrNumSamples(0), mTotal(0), mRefVariance(0), mMin(0), mMax(0) {}


    void setup() {
      scaleDown.begin(L_LOADCELL_DOUT_PIN, L_LOADCELL_SCK_PIN);
      scaleDown.set_scale(22.04622621848775f);
      scaleDown.tare();

      scaleUp.begin(R_LOADCELL_DOUT_PIN, R_LOADCELL_SCK_PIN);
      scaleUp.set_scale(22.04622621848775f);
      scaleUp.tare();
    }


    void loop() {
      if (scaleDown.wait_ready_timeout(1000) && scaleUp.wait_ready_timeout(1000)) {
        float readingL = scaleDown.get_units() / 1000.0f;
        float readingR = scaleUp.get_units() / 1000.0f;
        unsigned long currentTime = millis();
        addData(readingL + readingR);

      } else {
        Serial.println(F("HX711 not found."));
      }
    }

    void reset()    {
      mCurrNumSamples = 0;
      mTotal = 0;
      mRefVariance = 0;
      mMin = 0;
      mMax = 0;
    }

    void addData(float val) {

      mLast = abs(val);
      // reduce samples total by one sample value
      if (mCurrNumSamples >= mNumSamples) {
        mTotal = mTotal * (mNumSamples - 1) / mNumSamples;
        mRefVariance = mRefVariance * (mNumSamples - 1) / mNumSamples;
      }
      else
        mCurrNumSamples++; // increment the current number
     
      mTotal += val;
      mRefVariance += val * val; // add the square of val (referenced to zero)
      if (val > mMax)
        mMax = val;
      if (val < mMin)
        mMin = val;

      if (mean() <= MIN_WEIGHT_THRESHOLD) {
        reset();
      }
    }

    float mean() const {
      if (mCurrNumSamples != 0)
        return mTotal / mCurrNumSamples;
      else return 0.0f;
    }
    
    float variance() const {
      if (mCurrNumSamples != 0)
        return (mRefVariance - (mCurrNumSamples * sqr(mean()))) / mCurrNumSamples;
      else return 0.0f;
    }
    
    float stdDeviation() const {
      return sqrt(variance());
    }
    
    float maxVal() const {
      return mMax;
    }

    float minVal() const {
      return mMin;
    }

    float total() const {
      return mTotal;
    }

    unsigned int samples() const {
      return mCurrNumSamples;
    }

    unsigned int sampleSize() const {
      return mNumSamples;
    }

    float last() const {
      return mLast;
    }

  protected:
    unsigned int mNumSamples;
    unsigned int mCurrNumSamples;
    float mTotal;
    float mRefVariance; // not true variance - it must be calculated
    float mMin;
    float mMax;
    float mLast;

};

#endif
