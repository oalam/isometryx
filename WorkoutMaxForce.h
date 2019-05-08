#ifndef MAXFORCE_WORKOUT_H
#define MAXFORCE_WORKOUT_H


#include "Workout.h"

const unsigned int MAX_FORCE_NUM_REPS = 8;
const unsigned int MAX_FORCE_NUM_SETS = 1;
const unsigned int MAX_FORCE_REP_HANGING_DURATION_MS = 7000;
const unsigned int MAX_FORCE_REST_DURATION_MS = 10000;
const float MAX_FORCE_MAX_PRECENT_LIMIT = 20.0f;

/**
 * This workout is used for max force assessment
 * 
 * this is the best of 8 reps
 */
class WorkoutMaxForce : public Workout {
  public:
    WorkoutMaxForce() : Workout(ASSESSMENT_MAXFORCE,
                                MAX_FORCE_NUM_REPS, 
                                MAX_FORCE_NUM_SETS, 
                                MAX_FORCE_REP_HANGING_DURATION_MS, 
                                MAX_FORCE_REST_DURATION_MS,
                                MAX_FORCE_MAX_PRECENT_LIMIT) {}

  protected:


    
    void onRepFinish(){

    }

    void onRepStart(){

    }

    void onRepAbort(){

    }
    
    void onRestStart(){

    }

    void onRestFinish(){

    }


    void repLoop(){

    }

  private:

   

};

#endif
