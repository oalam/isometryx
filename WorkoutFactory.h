#ifndef WORKOUT_FACTORY_H
#define WORKOUT_FACTORY_H


#include "WorkoutMaxForce.h"
#include "Workout.h"




class WorkoutFactory {
  public:
    WorkoutFactory()  {
      setType(ASSESSMENT_MAXFORCE);
      mCurrentWorkout = new WorkoutMaxForce();
    }


    void setType(WorkoutType type) {
      mModeType = type;
    }

    Workout *getCurrentWorkout() {
      return mCurrentWorkout;
    }

    Workout *next() {

      if (mCurrentWorkout != NULL) {
        delete mCurrentWorkout;
        mCurrentWorkout = NULL;
      }

      switch (mModeType) {
        case ASSESSMENT_BODYWEIGHT :
          setType(WARMUP);
          Serial.println(F("Workout: WARMUP"));
          mCurrentWorkout = new WorkoutMaxForce();
          
        case WARMUP:
          setType(ASSESSMENT_MAXFORCE);
          Serial.println(F("Workout: ASSESSMENT_MAXFORCE"));
          mCurrentWorkout = new WorkoutMaxForce();
          
        case ASSESSMENT_MAXFORCE :
          setType(WORKOUT_MAXHANGS);
          Serial.println(F("Workout: WORKOUT_MAXHANGS"));
          mCurrentWorkout = new WorkoutMaxForce();
          
        case WORKOUT_MAXHANGS :
          setType(ASSESSMENT_BODYWEIGHT);
          Serial.println(F("Workout: ASSESSMENT_BODYWEIGHT"));
          mCurrentWorkout = new WorkoutMaxForce();
      }

      return mCurrentWorkout;
    }


  protected:
    WorkoutType mModeType;
    Workout *mCurrentWorkout = NULL;
};

#endif
