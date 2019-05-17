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
        mPreviousMean = mRepStats.mean(); 
        mPreviousMeanStrengh2Weight = climber.strenghToWeight(mRepStats.mean()); 
        sessionStats.incrementRep();  
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

          display.print(sessionStats.getTUTs());
          display.print(F("s "));
          display.print(sessionStats.getTotalNumRep());
          display.println(F("r"));

          display.print(valueToString(mPreviousMean,1));
          display.print(F("kg "));
          display.print((int)(mRepPercentMaxStats.mean()));
          display.println(F("% "));

          display.print(valueToString(mPreviousMeanStrengh2Weight,1));
          display.println(F(" s2w"));
          break;

        case HANGING :
          display.println(F("FreeHang"));

          display.print(timeOverLimit());
          display.println(F("s"));

          display.print(valueToString(mRepStats.last(), 1));
          display.print(F("kg "));
          display.print(percentMax());
          display.println(F("%"));
        
          display.print(valueToString(climber.strenghToWeight(mRepStats.last()),1));
          display.println(F(" s2w"));

          break;
        
      }


      display.display();
    }

  private:
    float mPreviousMean;
    float mPreviousMeanStrengh2Weight;
};

#endif
