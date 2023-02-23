#include "DelayESP.h"

DelayESP::DelayESP(unsigned long interval_for_connection = 1000) {
  _timing = millis();
  _interval_for_connection = interval_for_connection;
}
bool DelayESP::timeIsOver() {
  if (millis() - _timing >= _interval_for_connection) {
    return true;
  }
  return false;
}
void DelayESP::resetTimer() {
  _timing = millis();
}

long DelayESP::getTiming() {
  return ( millis() - _timing );
}
