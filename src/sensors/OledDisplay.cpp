//
// Created by vlasi on 2/24/2023.
//
#include "OledDisplay.h"

OledDisplay::OledDisplay() {

    _font_size = 1;
    _display_on = init();

}

OledDisplay::OledDisplay(int font_size) {

    _font_size = font_size;
    _display_on = init();

}

OledDisplay::~OledDisplay() {

    _display.clearDisplay();

}

bool OledDisplay::init() {

    Serial.println(F("Display initiation"));
    // Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
    Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

    _display = display;
    if (!_display.begin(SSD1306_SWITCHCAPVCC, I2C_OLED_DISPLAY_ADDRESS)) {
        Serial.println(F("Display allocation failed"));
        return false;
    }
    //_display = &display;

    delay(2000);
    _display.clearDisplay();
    _display.setTextSize(1);
    _display.setTextColor(WHITE);

    _display.setCursor(0, 0);
    _display.print("display inited");

    _display.display();

    Serial.println(F("Display inited"));
    return true;
}

void OledDisplay::printMessage(String message) {

    if (!_display_on) {
        return;
    }

    _display.clearDisplay();

    _display.setCursor(0, 0);
    _display.print(message);

    _display.display();

}

void OledDisplay::drawBitmap(const unsigned char *bitmap) {

    _display.clearDisplay();
    _display.drawBitmap(0, 0,  bitmap, SCREEN_WIDTH, SCREEN_HEIGHT, WHITE);
    _display.display();

}