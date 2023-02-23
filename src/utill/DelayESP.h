//
//
//
#include <Arduino.h>
#ifndef DelayESP_h
#define DelayESP_h

class DelayESP {
  public:
    DelayESP();
    DelayESP(unsigned long timing);
    bool timeIsOver();
    void resetTimer();
    long getTiming();
  private:
    unsigned long _timing ;
    long _interval_for_connection;
  protected:
};
#endif
