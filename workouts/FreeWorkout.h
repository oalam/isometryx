#ifndef FREE_WORKOUT_H
#define FREE_WORKOUT_H


const unsigned int FREE_NUM_REPS = 1;
const unsigned int FREE_NUM_SETS = 1;
const unsigned long FREE_REP_HANGING_DURATION_MS = 7000;
const unsigned long FREE_REST_DURATION_MS = 5000;
const float FREE_MAX_PRECENT_LIMIT = 30.0f;

/**
 * This workout is used to hang as wanted
 */
class FreeWorkout : public Workout {
  public:
    FreeWorkout() : Workout( TXO_HANDS,
                                FREE_WORKOUT,
                                FREE_NUM_REPS, 
                                FREE_NUM_SETS, 
                                FREE_REP_HANGING_DURATION_MS, 
                                FREE_REST_DURATION_MS,
                                FREE_MAX_PRECENT_LIMIT) {}

  protected:

    
    void onRepFinish(){
      
      // update maxForce for left and right hands
      if( mHangingMode == TWO_HANDS){
        if( mMaxForceLeft + mMaxForceRight < mCurrentRepMaxForce){
          mMaxForceLeft = mCurrentRepMaxForce / 2.0f;
          mMaxForceRight = mMaxForceLeft;
        }
      }else{
        if( mCurrentHand == LEFT && mMaxForceLeft < mCurrentRepMaxForce){
          mMaxForceLeft = mCurrentRepMaxForce;
        }
        if( mCurrentHand == RIGHT && mMaxForceRight < mCurrentRepMaxForce){
          mMaxForceRight = mCurrentRepMaxForce;
        }
      }

    }

    void render() {

      display.clearDisplay();
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(0, 0);

      switch (getState()) {

        case IDLE : 
          display.println(F("Max Force"));
          display.print(getNumReps());
          display.println(F(" reps"));
          display.print(valueToString(getClimber().getMaxForceL(), 1));
          display.print(F(" "));
          display.println(valueToString(getClimber().getMaxForceR(), 1));
          break;

        case HANGING :
          if(mHangingMode == ONE_HAND){
            if(mCurrentHand == LEFT){
              display.print(F("L:"));
              display.print(valueToString(mMaxForceLeft, 1));
              display.print(F(" "));
              display.print(valueToString(getClimber().getMaxForceL(), 1));
            }else{
              display.print(F("R:"));
              display.print(valueToString(mMaxForceRight, 1));
              display.print(F(" "));
              display.println(valueToString(getClimber().getMaxForceR(), 1));
            }
          }else{
            display.print(F("B:"));
            display.print(valueToString(mMaxForceLeft+mMaxForceRight, 1));
            display.print(F(" "));
            display.println(valueToString(getClimber().getMaxForceL()+ getClimber().getMaxForceR(), 1));
          }
          
          display.print(valueToString(getStats().last(), 1));
          display.print(F(" "));
          display.println(valueToString(getStats().last(), 1));

          if (percentMax() > 2) {
            display.print(valueToString(percentMax(), 0));
            display.print(F("% "));
          } else {
            display.print(F("  0% "));
          }
          display.print(valueToString(timeOverLimit(), 0));
          display.println(F("s"));

          display.print(valueToString(getRepsCount(), 0));
          display.print(F("/"));
          display.println(valueToString(getNumReps(), 0));
          break;
        
        case RESTING :
          display.println(F("Rest (s)"));

          display.print(valueToString(restingTime(), 0));
          display.print(F("/"));
          display.println(valueToString(restDurationMs(), 0));
                    display.print(valueToString(mMaxForceLeft, 1));
          display.print(F(" "));
          display.println(valueToString(mMaxForceRight, 1));
          break;
          
        case DONE :
          display.println(F("Done !"));
          display.print(valueToString(mMaxForceLeft, 1));
          display.print(F(" "));
          display.println(valueToString(mMaxForceRight, 1));
          break;

        case SWITCHING_HANDS :
          display.println(F("time to \nswitch"));
          break;

        default:
          display.println(F("..."));
          break;
      }


      display.display();
    }

    void onRepStart(){}
    void onRepAbort(){}
    void onRestStart(){}
    void onRestFinish(){}
    void onSetFinish(){}
    void onWorkoutFinish(){}

  private:
    float mMaxForceLeft = 0.0f;
    float mMaxForceRight = 0.0f;

};

#endif
