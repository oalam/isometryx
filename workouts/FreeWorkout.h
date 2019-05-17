#ifndef FREE_WORKOUT_H
#define FREE_WORKOUT_H


const unsigned int FREE_NUM_REPS = 100;
const unsigned int FREE_NUM_SETS = 1;
const unsigned long FREE_REP_HANGING_DURATION_MS = 7000;
const unsigned long FREE_REST_DURATION_MS = 5000;
const float FREE_MAX_PRECENT_LIMIT = 10.0f;

/**
 * This workout is used to hang as wanted
 */
class FreeWorkout : public Workout {
  public:
    FreeWorkout() : Workout( TWO_HANDS,
                                FREE_WORKOUT,
                                FREE_NUM_REPS, 
                                FREE_NUM_SETS, 
                                FREE_REP_HANGING_DURATION_MS, 
                                FREE_REST_DURATION_MS,
                                FREE_MAX_PRECENT_LIMIT) {}

  protected:
    
    void onRepFinish(){

    }

    void onRepStart(){

    }

    void onRepAbort(){

      // do not consider as Aborted if long enough in FreeWork mode
      if(mTimeOverLimit >= 2000){
        mRepsCount++;
        mAbortedRepsCount--;    
        mPreviousMax = mCurrentRepMaxForce; 
        mPreviousMaxStrengh2Weight = mCurrentRepMaxStrengh2Weight;   
      }

    }
    
    void onRestStart(){

    }

    void onRestFinish(){

    }

    void onSetFinish(){

    }

    void onWorkoutFinish(){
   
    }


    void render() {

      display.clearDisplay();
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(0, 0);

      switch (getState()) {

        case IDLE : 
        case DONE :
        case READY :
          display.println(F("FreeHang"));

          display.print(mRepsCount);
          display.print(F("r "));
          display.print(getTUT());
          display.println(F("s"));

          display.print(valueToString(mAvgLoad,1));
          display.print(F(" "));
          display.println(valueToString(mPreviousMax,1));

          display.print(valueToString(mPreviousMaxStrengh2Weight,1));
          display.println(F(" s2w"));
          break;

        case HANGING :
          display.println(F("FreeHang"));

          display.print(valueToString(stats.last(), 1));
          display.print(F("kg "));
          display.print(percentMax());
          display.println(F("%"));

          display.print(timeOverLimit());
          display.println(F("s"));

          break;
        
      }


      display.display();
    }

  private:
    float mPreviousMax;
    float mPreviousMaxStrengh2Weight;
};

#endif
