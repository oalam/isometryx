#ifndef WORKOUT_H
#define WORKOUT_H

#include "WorkoutStats.h"
#include "Climber.h"




enum WorkoutType {
  ASSESSMENT_BODYWEIGHT,
  WARMUP,
  ASSESSMENT_MAXFORCE,
  WORKOUT_MAXHANGS,
  FREE_WORKOUT
};

enum WorkoutState {
  HANGING,
  RESTING,
  IDLE,
  SWITCHING_HANDS,
  DONE
};

enum HangingMode {
  ONE_HAND,
  TWO_HANDS
};

enum Hand {
  LEFT,
  RIGHT,
  BOTH
};

class Workout {
  public:
    Workout(HangingMode hangingMode, WorkoutType type, unsigned short totalReps, unsigned short totalSets, unsigned long repHangingDurationMs, unsigned long restDurationMs, float maxPercentLimit) {
      mHangingMode = hangingMode;
      mType = type;
      mTotalReps = totalReps;
      mTotalSets = totalSets;
      mRepHangingDurationMs = repHangingDurationMs;
      mRestDurationMs = restDurationMs;
      mMaxPercentLimit = maxPercentLimit; 
      if(mHangingMode == TWO_HANDS){
         mCurrentHand = BOTH;
      }else{
        mCurrentHand = LEFT;
      }
      Serial.print(F("Init workout with (totalReps, totalSets, repHangingDurationMs, restDurationMs, maxPercentLimit) = "));
      Serial.print(totalReps);
      Serial.print(F(", "));
      Serial.print(totalSets);
      Serial.print(F(", "));
      Serial.print(repHangingDurationMs);
      Serial.print(F(", "));
      Serial.print(restDurationMs); 
      Serial.print(F(", "));
      Serial.println(maxPercentLimit);
      reset(IDLE);
    }


    float strenghToWeight() {
      return climber.strenghToWeight(stats.maxVal());
    }

    float percentMax() {
      float mMax = climber.getMaxForceL() + climber.getMaxForceR();
      if(mHangingMode == ONE_HAND){
        if(mCurrentHand == LEFT){
          mMax = climber.getMaxForceL();
        }
        else {
          mMax = climber.getMaxForceR();
        }
        
      }
     
      if (mMax != 0.0f)
        return stats.last() * 100.0f / mMax;
      else return 0.0f;
    }

    float timeOverLimit() {
      return mTimeOverLimit / 1000.0f;
    }

    float restingTime() {
      return mRestingTime/ 1000.0f;
    }

     float restDurationMs() {
      return mRestDurationMs/ 1000.0f;
    }

    Climber getClimber() {
      return climber;
    }

    WorkoutState getState(){
      return mState;
    }

    WorkoutType getType(){
      return mType;
    }

    HangingMode getHangingMode(){
      return mHangingMode;
    }

    Hand getCurrentHand(){
      return mCurrentHand;
    }

    WorkoutStats getStats() {
      return stats;
    }

    int getRepsCount() {
      return mRepsCount;
    }

    int getNumReps() {
      return mTotalReps;
    }

    void reset(WorkoutState newState) {
      mTimeOverLimit = 0;
      mRestingTime = 0;
      mStartTime = millis();
      mCurrentRepMaxForce = 0.0f;
      mState = newState;
      stats.reset();
    }


    void printRep(){
      Serial.print(F("Rep "));
      Serial.print(mRepsCount);
      Serial.print(F("/"));
      Serial.print(mTotalReps);      
    }

    void printSet(){
      Serial.print(F("Set "));
      Serial.print(mSetsCount);
      Serial.print(F("/"));
      Serial.print(mTotalSets);      
    }

    void setup() {
      stats.setup();
      climber.load();
    }

    void loop() {
    
      stats.loop(); 
      render();
      
      // wait a little while DONE or SWITCHING_HANDS
      if (mState == DONE || mState == SWITCHING_HANDS) {
        delay(IDLE_TIME_MS);
        reset(IDLE);
        return;
      }

      if (mState == IDLE) {
        // Start HANGING rep if there's more thant 2kg applied while IDLE
        if(stats.last() > 2){
          mRepsCount++;
          onRepStart();
          printRep();
          if(mHangingMode == ONE_HAND){
            if(mCurrentHand == LEFT)
              Serial.println(F(" started for left hand"));
            else
              Serial.println(F(" started for right hand"));
          }else
            Serial.println(F(" started for both hand"));
          
          reset(HANGING);         
        }
      }
      
      if (mState == HANGING){
      
        // increase time over limit
        if (percentMax() >= mMaxPercentLimit) {
          mTimeOverLimit += FRAME_RATE_MS;
        }

        // check for new max force
        if(mCurrentRepMaxForce < stats.maxVal() ){
          mCurrentRepMaxForce = stats.maxVal();
        }

        // Go back to IDLE when there's less than 2kg applied when HANGING
        if(stats.last() <= 2) {
          onRepAbort();
          printRep();
          Serial.println(F(" aborted"));
          mRepsCount--;
          reset(IDLE);
          return;
        }

        // Finish a rep when we've been hanging for the requested time
        if (mTimeOverLimit > mRepHangingDurationMs) {
          
          onRepFinish();
          printRep();
          Serial.println(F(" finished"));

          // switch hands but don't rest and go IDLE to start right hand hanging
          if (mHangingMode == ONE_HAND){
            
            if (mCurrentHand == LEFT) {
              Serial.println(F("done a rep with left hand, switching to right"));
              mCurrentHand = RIGHT;
              mRepsCount--;
              reset(SWITCHING_HANDS);
              return;
            }
            else {
              Serial.println(F("done a full rep with the two hands, switching back to left"));
              mCurrentHand = LEFT;
            }
          }

          // have we done with the set ?
          if (mRepsCount >= mTotalReps) {
            mRepsCount = 0;
            mSetsCount++;
            onSetFinish();
            printSet();
            Serial.println(F(" finished"));
          }

          // have we done with the workout ?
          if (mSetsCount >= mTotalSets) {
            mSetsCount = 0;
            onWorkoutFinish();
            Serial.println(F("Workout finished !"));
            reset(DONE);
            return;
          }else{
            Serial.println(F("Rest started"));
            onRestStart();
            reset(RESTING);
            return;
          }
                    
        }
      }
      

      if (mState == RESTING){

        // Rest time is over returning to IDLE
        if(mRestingTime >= mRestDurationMs) {
          onRestFinish();
          Serial.println(F("rest finished"));
          reset(IDLE);
          return;
        } 

        // otherwise increase resting time
        else {
          mRestingTime += FRAME_RATE_MS;
        }

      } 
    }

  protected:

    virtual void onRepStart() = 0;
    virtual void onRepFinish() = 0;
    virtual void onRepAbort() = 0;
    virtual void onRestStart() = 0;
    virtual void onRestFinish() = 0;
    virtual void onSetFinish() = 0;
    virtual void onWorkoutFinish() = 0;
    virtual void render() = 0;

    unsigned short mRepsCount = 0;
    unsigned short mSetsCount = 0;
    unsigned short mTotalReps;
    unsigned short mTotalSets;
    unsigned long mRepHangingDurationMs;
    unsigned long mRestDurationMs;
    float mMaxPercentLimit;
    float mCurrentRepMaxForce;
    WorkoutState mState;


    unsigned long mStartTime;
    unsigned long mTimeOverLimit;
    unsigned long mRestingTime;

    Hand mCurrentHand;
    HangingMode mHangingMode;
    WorkoutType mType;
    Climber climber;
    WorkoutStats stats;
};

#endif
