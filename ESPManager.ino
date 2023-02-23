//#include "src/settingsManager/SettingsManager.hpp"
#include "src/settingsManager/Test_SettingsManager.h"

void setup() {
  
  Serial.begin(9600);
  delay(200);

  bool test_res = Test_SettingsManager::runTest();

}

void loop() {


}
