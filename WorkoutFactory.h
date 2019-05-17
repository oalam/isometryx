#ifndef WORKOUT_FACTORY_H
#define WORKOUT_FACTORY_H



#include "Workout.h"
#include "workouts/BodyweightAssesmentWorkout.h"
#include "workouts/MaxForceAssesmentWorkout.h"
#include "workouts/FreeWorkout.h"
#include "workouts/SummaryWorkout.h"

class WorkoutFactory {
  public:
    WorkoutFactory()  { }

  


    void setType(WorkoutType type) {
      mModeType = type;
    }

    Workout *getCurrentWorkout() {
      if (mCurrentWorkout == NULL) {
        mModeType = SUMMARY;
        return next();
      }
      return mCurrentWorkout;
    }

    Workout *next() {

      if (mCurrentWorkout != NULL) {
        delete mCurrentWorkout;
        mCurrentWorkout = NULL;
      }

      switch (mModeType) {
        case SUMMARY :
          setType(FREE_WORKOUT);
          Serial.println(F("Workout: FREE_WORKOUT"));
          mCurrentWorkout = new FreeWorkout();
          break;
          
        case FREE_WORKOUT:
          setType(ASSESSMENT_MAXFORCE);
          Serial.println(F("Workout: ASSESSMENT_MAXFORCE"));
          mCurrentWorkout = new MaxForceAssessmentWorkout();
          break;

        case ASSESSMENT_MAXFORCE :
          setType(ASSESSMENT_BODYWEIGHT);
          Serial.println(F("Workout: ASSESSMENT_BODYWEIGHT"));
          mCurrentWorkout = new BodyweightAssessmentWorkout();
          break;

        case ASSESSMENT_BODYWEIGHT :
          setType(SUMMARY);
          Serial.println(F("Workout: SUMMARY"));
          mCurrentWorkout = new SummaryWorkout();
          break;
     
      }

      return mCurrentWorkout;
    }


  void loop(){
    getCurrentWorkout()->loop();
    if(getCurrentWorkout()->doChangeWorkout())
      next();

  }

  protected:
    WorkoutType mModeType;
    Workout *mCurrentWorkout = NULL;
};

#endif
