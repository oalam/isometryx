#ifndef LOAD_STATS_H
#define LOAD_STATS_H


#include <float.h>
#include <math.h>

#define sqr(x) ((x)*(x))


const int MAX_POINTS = 10;

const int MIN_WEIGHT_THRESHOLD = 2;
const int MAX_TICK_WITHOUT_VALUE = 2;

class LoadStats {
  public:
    LoadStats() : mNumSamples(MAX_POINTS), mCurrNumSamples(0), mTotal(0), mRefVariance(0), mMin(0), mMax(0) {}

    void setNewSampleSize(int numSamples)
    {
      mNumSamples = numSamples;
      reset();
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
      if (mCurrNumSamples >= mNumSamples) // reduce samples total by one sample value
      {
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
