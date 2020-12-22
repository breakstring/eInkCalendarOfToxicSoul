#ifndef MyIP_h
#define MyIP_h

#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>


enum Language
{
    CHINESE,
    ENGLISH
};


class MyIP
{

public:
    /**
     * @brief IP Address
     * 
     */
    String IP;
    String CountryCode;
    String CountryName;
    String Province;
    String City;
    String Latitude;
    String Longitude;



    /**
     * @brief Construct a new MyIP object
     * 
     * @param language 
     */
    MyIP(Language language);


};

#endif