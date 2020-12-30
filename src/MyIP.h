#ifndef _MYIP_H
#define _MYIP_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>

/**
 * @brief Language of the geolocation information. 
 * 
 */
enum Language
{
    CHINESE,
    ENGLISH
};


/**
 * @brief Get IP Address and Geolocation from https://api.myip.la/
 * 
 */
class MyIP
{

public:
    /**
     * @brief IP Address
     * 
     */
    String IP;
    /**
     * @brief Country Code
     * 
     */
    String CountryCode;
    /**
     * @brief Country Name
     * 
     */
    String CountryName;
    /**
     * @brief Provice Name
     * 
     */
    String Province;
    /**
     * @brief City
     * 
     */
    String City;
    /**
     * @brief Latitude
     * 
     */
    String Latitude;
    /**
     * @brief Longitude
     * 
     */
    String Longitude;


    /**
     * @brief Construct a new MyIP object
     * 
     * @param language 
     */
    MyIP(Language language);


};

#endif