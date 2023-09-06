//
// Created by vlasi on 2/25/2023.
//

#include "WiFiConnection.h"

WiFiConnection::WiFiConnection() {

}

bool WiFiConnection::init() {

    _is_wifi_connected = false;
    _is_access_point = false;

    scan_WIFI();

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
        if (millis() - timing >= CHECK_WIFI_TIME) {
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

void WiFiConnection::loop() {

    if (!isWiFiConnected()) {
        return;
    }

    unsigned long timing = millis();
    // if WiFi is down, try reconnecting every CHECK_WIFI_TIME seconds
    if ((WiFi.status() != WL_CONNECTED) && (timing - _previousMillis >= CHECK_WIFI_TIME)) {
        Serial.print(millis());
        Serial.println("Reconnecting to WiFi...");
        WiFi.disconnect();
        WiFi.reconnect();
        _previousMillis = timing;
    }

}

void WiFiConnection::scan_WIFI() {
    Serial.println("WIFI scan ...");
    // WiFi.scanNetworks returns the number of networks found
    int n = WiFi.scanNetworks();
    if (n == 0) {
        Serial.println("[ERR] no networks found");
    } else {
        Serial.printf("[OK] %i networks found:\n",n);
        for (int i = 0; i < n; ++i) {
            // Print SSID for each network found
            Serial.printf("  %i: ",i+1);
            Serial.println(WiFi.SSID(i));
            Serial.println(WiFi.RSSI(i));
            delay(10);
        }
    }
    Serial.println("...");
}

bool WiFiConnection::isWiFiConnected() {

    if (WiFi.status() != WL_CONNECTED) {
        _is_wifi_connected = false;
    } else {
        _is_wifi_connected = true;
    }

    return _is_wifi_connected;

}

bool WiFiConnection::isAccessPoint() {

    return _is_access_point;

}

void WiFiConnection::setWiFiProperties(char *ssid, char *password) {

    _ssid = ssid;
    _password = password;

}
