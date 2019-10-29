#ifndef WORKOUT_STATS_H
#define WORKOUT_STATS_H

#include <float.h>
#include <math.h>

const unsigned int MIN_WEIGHT_THRESHOLD = 1;
const unsigned int MAX_TICK_WITHOUT_VALUE = 2;

class WorkoutStats {
  public:
    WorkoutStats() : mNumSamples(20), mCurrNumSamples(0), mTotal(0), mMax(0) {}

    void reset()    {
      mCurrNumSamples = 0;
      mTotal = 0;
      mMax = 0;
    }

    void addData(float val) {

      mLast = abs(val);

      if(val < MIN_WEIGHT_THRESHOLD)
        return;
      
      // reduce samples total by one sample value
      if (mCurrNumSamples >= mNumSamples) {
        mTotal = mTotal * (mNumSamples - 1) / mNumSamples;
      }
      else
        mCurrNumSamples++; // increment the current number
     
      mTotal += val;
      if (val > mMax)
        mMax = val;

    }

    float mean() const {
      if (mCurrNumSamples != 0)
        return mTotal / mCurrNumSamples;
      else return 0.0f;
    }
    

    float maxVal() const {
      return mMax;
    }


    float last() const {
      return mLast;
    }

  protected:
    unsigned int mNumSamples;
    unsigned int mCurrNumSamples;
    float mTotal;
    float mMax;
    float mLast;

};

#endif
