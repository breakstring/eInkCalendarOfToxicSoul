#ifndef _IPAPI_H
#define _IPAPI_H
#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClient.h>
#include <string>
/**
 * Get ip information from ip-api.com.
 * Detail api documentation refer to https://ip-api.com/docs
 * 
 */

/**
 * @brief Response from ip-api.com query
 * 
 */
struct IPAPIResponse
{
    /**
     * @brief IP used for the query
     * 
     */
    String query;
    /**
     * @brief success or fail
     * 
     */
    String status;
    /**
     * @brief included only when status is fail. Can be one of the following: private range, reserved range, invalid query
     * 
     */
    String message;
    /**
     * @brief Continent name
     * 
     */
    String continent;
    /**
     * @brief Two-letter continent code
     * 
     */
    String continentCode;
    /**
     * @brief Country name
     * 
     */
    String country;
    /**
     * @brief Two-letter country code ISO 3166-1 alpha-2
     * 
     */
    String countryCode;
    /**
     * @brief Region/state short code (FIPS or ISO)
     * 
     */
    String region;
    /**
     * @brief Region/state
     * 
     */
    String regionName;
    /**
     * @brief City
     * 
     */
    String city;
    /**
     * @brief District (subdivision of city)
     * 
     */
    String district;
    /**
     * @brief Zip code
     * 
     */
    String zip;
    /**
     * @brief Latitude
     * 
     */
    float_t lat;
    /**
     * @brief Longitude
     * 
     */
    float_t lon;
    /**
     * @brief Timezone (tz)
     * 
     */
    String timezone;
    /**
     * @brief Timezone UTC DST offset in seconds
     * 
     */
    int32_t offset;
    /**
     * @brief National currency
     * 
     */
    String currency;
    /**
     * @brief ISP name
     * 
     */
    String isp;
    /**
     * @brief Organization name
     * 
     */
    String org;
    /**
     * @brief AS number and organization, separated by space (RIR). Empty for IP blocks not being announced in BGP tables.
     * 
     */
    String as;
    /**
     * @brief AS name (RIR). Empty for IP blocks not being announced in BGP tables.
     * 
     */
    String asname;
    /**
     * @brief Reverse DNS of the IP (can delay response)
     * 
     */
    String reverse;
    /**
     * @brief Mobile (cellular) connection
     * 
     */
    bool mobile;
    /**
     * @brief Proxy, VPN or Tor exit address
     * 
     */
    bool proxy;
    /**
     * @brief Hosting, colocated or data center
     * 
     */
    bool hosting;
};

enum Localization{
    English,
    German,
    Spanish,
    Portuguese,
    French,
    Japanese,
    Chinese,
    Russian
};

IPAPIResponse GetIPInfomation(String ip = "", Localization localization = Localization::Chinese);

#endif