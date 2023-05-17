#ifndef Setup_functions
#define Setup_functions
#include <Arduino.h>

    void conexion_wifi(const char* ssid, const char* password){
        WiFi.begin(ssid, password); 
        while (WiFi.status() != WL_CONNECTED){
            delay(500);
            Serial.println(ssid); 
            Serial.println("Connecting to WiFi..");
        }
        Serial.println("Connected to the WiFi network");
    }

#endif