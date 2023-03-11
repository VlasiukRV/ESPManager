//
// Created by vlasi on 2/24/2023.
//

#ifndef ESPMANAGER_DISPLAY_H
#define ESPMANAGER_DISPLAY_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
//#include <Adafruit_Sensor.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define I2C_OLED_DISPLAY_ADDRESS 0x3C

class OledDisplay {

public:

    OledDisplay();

    OledDisplay(int font_size);

    ~OledDisplay();

    bool init();

    void printMessage(String message);

    void drawBitmap( const unsigned char *bitmap);

private:
    Adafruit_SSD1306 _display;

    bool _display_on;

    int _font_size;

};

#endif //ESPMANAGER_DISPLAY_H
