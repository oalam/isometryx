#ifndef SESSION_STATS_H
#define SESSION_STATS_H



class SessionStats   {
  public:
    SessionStats(): mNumSamples(20), mCurrNumSamples(0), mTotal(0) {}

    void updateTUT(unsigned long time, unsigned int percentMax){
      addData(percentMax);
      mTUT += time;
      if(percentMax>=85){
        mTimeOver85Ms += time;
      }
    }

    int getTUTs() {
      return (mTUT) / 1000;
    }

    int getAvgPercentMax()  const {
      if (mCurrNumSamples != 0)
        return mTotal / mCurrNumSamples;
      else return 0;
    }

    float getAvgStrengh2Weight(){
      return 0;
    }

    void incrementRep(){
      mTotalRepsCount++;
    }

    int getTotalNumRep(){
      return mTotalRepsCount;
    }

    int getTimeOver85s(){
      return mTimeOver85Ms/1000;
    }

    float getAvgWeight(){
      return 0;
    }

protected:

    void addData(float val) {
      // reduce samples total by one sample value
      if (mCurrNumSamples >= mNumSamples) {
        mTotal = mTotal * (mNumSamples - 1) / mNumSamples;
      }
      else
        mCurrNumSamples++; // increment the current number
      mTotal += val;
    }

  protected:
    unsigned int mNumSamples;
    unsigned int mCurrNumSamples;
    float mTotal;

    unsigned int mTotalRepsCount = 0;
    unsigned long mTUT = 0;
    unsigned long mTimeOver85Ms = 0; 
};

#endif
