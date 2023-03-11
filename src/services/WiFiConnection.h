//
// Created by vlasi on 2/25/2023.
//

#ifndef ESPMANAGER_WIFICONNECTION_H
#define ESPMANAGER_WIFICONNECTION_H

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

    bool isWiFiConnected();

    bool isAccessPoint();

private:
    char *_ssid;
    char *_password;

    bool _is_wifi_connected;
    bool _is_access_point;

};


#endif //ESPMANAGER_WIFICONNECTION_H
