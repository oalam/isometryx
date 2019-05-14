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
      if( mCurrentHand == LEFT && mMaxForceLeft < mCurrentRepMaxForce){
        mMaxForceLeft = mCurrentRepMaxForce;
      }else if( mCurrentHand == RIGHT && mMaxForceRight < mCurrentRepMaxForce){
        mMaxForceRight = mCurrentRepMaxForce;
      }
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
      if (climber.getMaxForceL() < mMaxForceLeft ) {
        climber.setMaxForceL(mMaxForceLeft);
      }

      if (climber.getMaxForceR() < mMaxForceRight) {
        climber.setMaxForceR(mMaxForceRight);
      }
    }


    void render(){

    }

  private:
    float mMaxForceLeft = 0.0f;
    float mMaxForceRight = 0.0f;

   

};

#endif
