#ifndef CLIMBER_H
#define CLIMBER_H

#include <EEPROM.h>
#include "Config.h"

const int maxForceLAddress = 0;   // Location we want the data to be put.
const int maxForceRAddress =  4; // Move address to the next byte after maxForceAddress
const int bodyWeightAddress =  8; // Move address to the next byte after maxForceAddress

class Climber {
  public:
    Climber() : mMaxForceL(0), mMaxForceR(0), mWeight(71.5f) {
      load();
    }


    float readFloat(unsigned int addr) {
      union {
        byte b[4];
        float f;
      } data;
      for (int i = 0; i < 4; i++) {
        data.b[i] = EEPROM.read(addr + i);
      }
      return data.f;
    }

    void writeFloat(unsigned int addr, float x) {
      union {
        byte b[4];
        float f;
      } data;
      data.f = x;
      for (int i = 0; i < 4; i++) {
        EEPROM.write(addr + i, data.b[i]);
      }
    }

    void setMaxForceR(float maxForce) {
      mMaxForceR = maxForce;
      writeFloat(maxForceRAddress, maxForce);
    }

    void setMaxForceL(float maxForce) {
      mMaxForceL = maxForce;
      writeFloat(maxForceLAddress, maxForce);
    }

    float getMaxForceR() {
      return mMaxForceR;
    }

    float getMaxForceL() {
      return mMaxForceL;
    }

    float getWeight(){
      return mWeight;
    }

    void setWeight(float weight){
      mWeight = weight;
      writeFloat(bodyWeightAddress, mWeight);
    }

    float strenghToWeight(float loadWeight) {
      return loadWeight / mWeight;
    }

    void load() {
      mMaxForceL = readFloat(maxForceLAddress);
      mMaxForceR = readFloat(maxForceRAddress);
      mWeight = readFloat(bodyWeightAddress);
      Serial.print(F("Read climber data from EEPROM : "));
      Serial.print(mMaxForceL);
      Serial.print( F(" "));
      Serial.print(mMaxForceR);
      Serial.print( F(" "));
      Serial.println(mWeight);
      if(mMaxForceL <= 0.0f)
        mMaxForceL = 3.0f;

      if(mMaxForceR <= 0.0f)
        mMaxForceR = 3.0f;
    }

  protected:
    float mMaxForceL;
    float mMaxForceR;
    float mWeight;



};

#endif
