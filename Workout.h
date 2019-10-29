#ifndef WORKOUT_H
#define WORKOUT_H

#include "WorkoutStats.h"


enum WorkoutType {
  DEADHANG,
  BODYWEIGHT,
  RESET
};

enum WorkoutState {
  HANGING,
  IDLE
};

class Workout {
  public:
    Workout() {
      fullReset(IDLE);
    }

    void setBodyWeight(float newWeight){
      mBodyWeight = newWeight;
    }

    float current(){
      return mRepStats.last();
    }

    float sessionMax(){
      return mSessionMax;
    }

    int percentMax() {     
      if (mSessionMax != 0.0f)
        return (int) (mRepStats.last() * 100.0f / mSessionMax);
      else return 0;
    }


    float strengthToWeight(float strength){
      return strength / mBodyWeight;
    }


    void reset(WorkoutState newState) {
      mIdleTime = 0;
      mHangingTime = 0;
      mStartTime = millis();
      mState = newState;
  
    }

    void fullReset(WorkoutState newState) {
      reset(newState);
      mAbortedRepsCount = 0;
      mRepStats.reset();
      mRepPercentMaxStats.reset();
    }


    /**
     * Grab current load from sensors and update hanging stats
     */
    void updateStats(){
      // grab stats from load cells
      if (scaleDown.wait_ready_timeout(100) && scaleUp.wait_ready_timeout(100)) {
        float readingL = abs(scaleDown.get_units() / 1000.0f);
        float readingR = abs(scaleUp.get_units() / 1000.0f);
        mRepStats.addData(readingL + readingR);

        
        if(mRepStats.last() > 1){
#ifdef DEBUG
          Serial.print(readingL);
          Serial.print(F(","));
          Serial.println(readingR);
#endif
          ble.print(readingL);
          ble.print(F(","));
          ble.println(readingR);
        }
       

      } else {
        Serial.println(F("HX711 not found."));
      }   

    }

    /**
     * Called after 3 short hangs to switch to the next workout mode
     */
    void updateWorkoutType(){
      if( mAbortedRepsCount >= MAX_ABORTED_REPS) {

        switch (mType) {
          case DEADHANG :
            mType = BODYWEIGHT;
            Serial.println(F("Workout: BODYWEIGHT"));
            break;
            
          case BODYWEIGHT:
           /* mType = RESET;
            Serial.println(F("Workout: RESET"));
            break;

          case RESET :*/
            mType = DEADHANG;
            Serial.println(F("Workout: DEADHANG"));
            break;
        }
        fullReset(IDLE);
      }
    }


    void render() {

      display.clearDisplay();
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(0, 0);

      switch (mType) {

        case DEADHANG:
          switch (mState) {

            case IDLE :
              display.println(F("DeadHang"));

              display.print(mTUT/1000);
              display.print(F("s "));
              display.print(mTotalNumRep);
              display.println(F("r"));

              display.print((int)mRepStats.mean());
              display.print(F("kg "));
              display.print((int)(mRepPercentMaxStats.mean()));
              display.println(F("% "));

              display.print(strengthToWeight(mRepStats.mean()));
              display.println(F("sw"));
              break;

            case HANGING :
              display.println(F("DeadHang"));

              display.print(mHangingTime/1000);
              display.println(F("s"));

              display.print((int)mRepStats.last());
              display.print(F("kg "));
              display.print(percentMax());
              display.println(F("%"));
            
              display.print(strengthToWeight(mRepStats.last()));
              display.println(F("sw "));
              display.println(F(""));
              break;
            
          }
          break;
        case BODYWEIGHT:
          switch (mState) {
    
            case IDLE : 
              display.println(F("BodyWeight"));
              
              display.print(mBodyWeight);
              display.println(F("kg"));
              
              display.println(F("start ..."));
              break;
    
            case HANGING :
              display.println(F("BodyWeight"));
              
              display.print(mRepStats.last());
              display.println(F("kg"));
              
              display.print(mHangingTime/1000);
              display.println(F("s"));
              break;
          
    
          }
          break;
        case RESET:

          break;
      }




      display.display();
    }

    void loop() {

      updateStats();
      updateWorkoutType();
      render();
      
      if (mState == IDLE) {
        mIdleTime += FRAME_RATE_MS;

        // If nothing has been done for too long do a full reset
        if(mIdleTime >= MAX_IDLE_TIME_MS){
          fullReset(IDLE);
        }

        // Start HANGING rep if there's more thant 2kg applied while IDLE
        if(mRepStats.last() > 2){
          mRepsCount++;   
          mTotalNumRep++;
          mRepStats.reset();
          reset(HANGING);         
        }
      }
      
      if (mState == HANGING){
        mHangingTime += FRAME_RATE_MS;

        // Store time under tension & percentMaxStats
        int pMax = percentMax();
        mTUT += FRAME_RATE_MS;
        mRepPercentMaxStats.addData(pMax);

        // check for new max force
        if(mSessionMax < mRepStats.last() ){
          mSessionMax = mRepStats.last();
          mSessionMaxStrength2Weight = strengthToWeight(mSessionMax);
        }
        
        // Go back to IDLE when there's less than 2kg applied when HANGING
        if(mRepStats.last() <= 2) {

          // under 1" hang we consider that's a click
          if(mHangingTime < 1000){
            mAbortedRepsCount++;
          }

          if(mType == BODYWEIGHT){
            mBodyWeight = mRepStats.maxVal();
            writeFloat(BODYWEIGHT_EEPROM_ADDRESS, mBodyWeight);
            Serial.print(F("Write climber data to EEPROM : "));
            Serial.println(mBodyWeight);
          }
          
          reset(IDLE);
          return;
        }
      }
    }

  protected:


    unsigned short mAbortedRepsCount = 0;
    unsigned short mRepsCount = 0;
    unsigned short mTotalNumRep = 0;


    float mBodyWeight = 70.0f;
    float mSessionMax = 20.0f;
    float mSessionMaxStrength2Weight = mSessionMax/mBodyWeight;
    

    unsigned int mHangingTime;
    unsigned int mIdleTime;
    unsigned long mStartTime;
    unsigned long mTUT = 0L;

    

    WorkoutType mType = DEADHANG;
    WorkoutState mState = IDLE;

    WorkoutStats mRepStats;
    WorkoutStats mRepPercentMaxStats;
    
};

#endif
