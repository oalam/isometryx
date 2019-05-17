#ifndef SUMMARY_WORKOUT_H
#define SUMMARY_WORKOUT_H


const unsigned int SUMMARY_NUM_REPS = 100;
const unsigned int SUMMARY_NUM_SETS = 1;
const unsigned long SUMMARY_REP_HANGING_DURATION_MS = 7000;
const unsigned long SUMMARY_REST_DURATION_MS = 5000;
const float SUMMARY_MAX_PRECENT_LIMIT = 10.0f;

/**
 * This workout is used to hang as wanted
 */
class SummaryWorkout : public Workout {
  public:
    SummaryWorkout() : Workout( TWO_HANDS,
                                SUMMARY,
                                SUMMARY_NUM_REPS, 
                                SUMMARY_NUM_SETS, 
                                SUMMARY_REP_HANGING_DURATION_MS, 
                                SUMMARY_REST_DURATION_MS,
                                SUMMARY_MAX_PRECENT_LIMIT) {}

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

    void onSetFinish(){

    }

    void onWorkoutFinish(){
   
    }


    void render() {

      display.clearDisplay();
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(0, 0);

     
      display.println(F("Summary"));

      display.print(sessionStats.getTUTs());
      display.print(F("s "));
      display.print(sessionStats.getTotalNumRep());
      display.println(F("r"));

      display.print((int)(sessionStats.getAvgPercentMax()));
      display.println(F("% avg"));
      display.print(sessionStats.getTimeOver85s());
      display.println(F("s >85%"));

      display.display();
    }
};

#endif
