#ifndef BODYWEIGHT_WORKOUT_H
#define BODYWEIGHT_WORKOUT_H


const unsigned int BODYWEIGHT_NUM_REPS = 1;
const unsigned int BODYWEIGHT_NUM_SETS = 1;
const unsigned int BODYWEIGHT_REP_HANGING_DURATION_MS = 5000;
const unsigned int BODYWEIGHT_REST_DURATION_MS = 0;
const float BODYWEIGHT_MAX_PRECENT_LIMIT = 20.0f;

/**
 * This workout is used for max force assessment
 * 
 * this is the best of 8 reps
 */
class BodyweightAssessmentWorkout : public Workout {
  public:
    BodyweightAssessmentWorkout() : Workout( TWO_HANDS,
                                ASSESSMENT_BODYWEIGHT,
                                BODYWEIGHT_NUM_REPS, 
                                BODYWEIGHT_NUM_SETS, 
                                BODYWEIGHT_REP_HANGING_DURATION_MS, 
                                BODYWEIGHT_REST_DURATION_MS,
                                BODYWEIGHT_MAX_PRECENT_LIMIT) {}

  protected:


    
    void onRepFinish(){
#ifdef DEBUG
      Serial.println(F("setting new weight : "));
      Serial.println(mCurrentRepMaxForce);
#endif
      climber.setWeight(mCurrentRepMaxForce);
    }

    void onRepStart(){

    }

    void onRepAbort(){

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

        case READY :
          display.println(F("BodyWeight"));
          display.print(valueToString(getClimber().getWeight(), 1));
          display.println(F("kg"));
          break;

        case IDLE : 
          display.println(F("BodyWeight"));
          display.print(valueToString(getClimber().getWeight(), 1));
          display.println(F("kg"));
          display.println(F("start ..."));
          break;

        case HANGING :
          display.println(F("BodyWeight"));
          display.print(valueToString(mRepStats.last(), 1));
          display.println(F("kg"));
          
          display.print(remainingTime());
          display.println(F("s"));
          break;
        
        case DONE :
          display.println(F("Done !"));
          display.print(valueToString(getClimber().getWeight(), 1));
          display.println(F("kg"));
          break;

      }


      display.display();
    }


};

#endif
