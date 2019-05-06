#ifndef MODE_H
#define MODE_H

enum ModeType {
  ASSESSMENT_BODYWEIGHT,
  WARMUP,
  ASSESSMENT_MAXFORCE,
  WORKOUT_MAXHANGS
};


class Mode {
  public:
    Mode()  {    }

    void setup() {
      setType(ASSESSMENT_BODYWEIGHT);
    }

    ModeType getType() {
      return mModeType;
    }

    void setType(ModeType newType) {
      mModeType = newType;
    }

    void next() {
      switch (mModeType) {
        case ASSESSMENT_BODYWEIGHT :
          setType(WARMUP);
          break;
        case WARMUP:
          setType(ASSESSMENT_MAXFORCE);
          break;
        case ASSESSMENT_MAXFORCE :
          setType(WORKOUT_MAXHANGS);
          break;
        case WORKOUT_MAXHANGS :
          setType(ASSESSMENT_BODYWEIGHT);
          break;
      }


    }

    void loop() {

      switch (mModeType) {
        case ASSESSMENT_BODYWEIGHT :
          // Serial.println("Assessement:  body weight");
          break;
        case WARMUP:
          //  Serial.println("Warm up");
          break;
        case ASSESSMENT_MAXFORCE :
          //  Serial.println("Assessement: max force");
          break;
        case WORKOUT_MAXHANGS :
          // Serial.println("Workou: max hangs");
          break;
      }
    }

  protected:
    ModeType mModeType;
};

#endif
