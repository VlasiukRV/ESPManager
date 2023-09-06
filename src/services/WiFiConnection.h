//
// Created by vlasi on 2/25/2023.
//

#ifndef ESPMANAGER_WIFICONNECTION_H
#define ESPMANAGER_WIFICONNECTION_H

#define CHECK_WIFI_TIME 30000

#include <WiFi.h>
//#include <WiFiClient.h>
//#include <WiFiServer.h>
//#include <WiFiUdp.h>

class WiFiConnection {

public:

    WiFiConnection();

    bool init();

    void setWiFiProperties(char *ssid, char *password);

    bool initAccessPoint();

    bool initWiFiConnection();

    void scan_WIFI();

    bool isWiFiConnected();

    bool isAccessPoint();

    void loop();

private:
    char *_ssid;
    char *_password;

    bool _is_wifi_connected;
    bool _is_access_point;

    unsigned long _previousMillis = 0;

};


#endif //ESPMANAGER_WIFICONNECTION_H
