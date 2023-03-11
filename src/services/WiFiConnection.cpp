//
// Created by vlasi on 2/25/2023.
//

#include "WiFiConnection.h"

WiFiConnection::WiFiConnection() {

}

bool WiFiConnection::init() {

    _is_wifi_connected = false;
    _is_access_point = false;

    _is_wifi_connected = initWiFiConnection();
    if (!_is_wifi_connected) {

        _is_access_point = initAccessPoint();

    }

    return ( _is_wifi_connected || _is_access_point );
}

bool WiFiConnection::initAccessPoint() {

    IPAddress ACCESS_POINT_LOCAL_IP(192, 168, 10, 22);
    IPAddress ACCESS_POINT_GATEWAY(192, 168, 10, 9);
    IPAddress ACCESS_POINT_SUBNET(255, 255, 255, 0);

    Serial.print("Setting soft-AP configuration ... ");
    Serial.println(
            WiFi.softAPConfig(ACCESS_POINT_LOCAL_IP, ACCESS_POINT_GATEWAY, ACCESS_POINT_SUBNET) ? "Ready" : "Failed!");

    Serial.print("Setting soft-AP ... ");

    WiFi.mode(WIFI_AP);
    bool accessPointReady = WiFi.softAP("ESP-WIFI-MANAGER", NULL);
    //myStatus.ip_adress = "";

    if (accessPointReady) {
        //uint32_t ip_adress = WiFi.softAPIP();
        //snprintf(ip_adress, 4, "%lu", (unsigned long)WiFi.softAPIP());
        //sprintf(ip_adress, "%d.%d.%d.%d", myStatus.ip_adress[0], myStatus.ip_adress[1], myStatus.ip_adress[2], myStatus.ip_adress[3]);

        Serial.println("Access Point Ready");
        Serial.print("Soft-AP IP address = ");
        Serial.println(WiFi.softAPIP());
    } else {
        Serial.println("Access Point Failed!");
    }

    return accessPointReady;

}

bool WiFiConnection::initWiFiConnection() {

    Serial.println("Connecting to Wi Fi");
    Serial.println(_ssid);
    Serial.println(_password);

    //connect to your local wi-fi network
    WiFi.mode(WIFI_STA);
    WiFi.begin(_ssid, _password);

    //DelayESP DelayESP(100000);
    unsigned long timing = millis();

    //check wi-fi is connected to wi-fi network
    while (WiFi.status() != WL_CONNECTED) {

        //if ( DelayESP.timeIsOver( ) ) {
        //  Serial.println("Failed to connect.");
        //  return false;
        //}
        if (millis() - timing >= 10000) {
            Serial.println("Failed to connect.");
            return false;
        }

    }
    Serial.println("");
    Serial.println("WiFi connected..!");
    Serial.print("Got IP: ");
    Serial.println(WiFi.localIP());

    return true;
}

bool WiFiConnection::isWiFiConnected() {

    return _is_wifi_connected;

}

bool WiFiConnection::isAccessPoint() {

    return _is_access_point;

}

void WiFiConnection::setWiFiProperties(char *ssid, char *password) {

    _ssid = ssid;
    _password = password;

}
