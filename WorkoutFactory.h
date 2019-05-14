#ifndef WORKOUT_FACTORY_H
#define WORKOUT_FACTORY_H

#include "Workout.h"
#include "workouts/BodyweightAssesmentWorkout.h"
#include "workouts/MaxForceAssesmentWorkout.h"


class WorkoutFactory {
  public:
    WorkoutFactory()  {}

    void setType(WorkoutType type) {
      mModeType = type;
    }

    Workout *getCurrentWorkout() {
      if (mCurrentWorkout == NULL) {
        return next();
      }
      return mCurrentWorkout;
    }

    Workout *next() {

      if(mModeType == NULL){
        setType(WARMUP);
      }

      if (mCurrentWorkout != NULL) {
        delete mCurrentWorkout;
        mCurrentWorkout = NULL;
      }

      switch (mModeType) {
        case ASSESSMENT_BODYWEIGHT :
          setType(WARMUP);
          Serial.println(F("Workout: WARMUP"));
          mCurrentWorkout = new MaxForceAssessmentWorkout();
          break;
          
        case WARMUP:
          setType(ASSESSMENT_MAXFORCE);
          Serial.println(F("Workout: ASSESSMENT_MAXFORCE"));
          mCurrentWorkout = new MaxForceAssessmentWorkout();
          break;

        case ASSESSMENT_MAXFORCE :
          setType(WORKOUT_MAXHANGS);
          Serial.println(F("Workout: WORKOUT_MAXHANGS"));
          mCurrentWorkout = new MaxForceAssessmentWorkout();
          break;

        case WORKOUT_MAXHANGS :
          setType(ASSESSMENT_BODYWEIGHT);
          Serial.println(F("Workout: ASSESSMENT_BODYWEIGHT"));
          mCurrentWorkout = new BodyweightAssessmentWorkout();
          break;
      }

      return mCurrentWorkout;
    }

  void setup(){
    getCurrentWorkout()->setup();
  }

  void loop(){
    getCurrentWorkout()->loop();
  }

  protected:
    WorkoutType mModeType = WARMUP;
    Workout *mCurrentWorkout = NULL;
};

#endif
