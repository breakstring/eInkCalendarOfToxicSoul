#include "MyIP.h"

const char *GET_IP_LOCATION_URL_EN = "https://api.myip.la/en?json";
const char *GET_IP_LOCATION_URL_CN = "https://api.myip.la/cn?json";


MyIP::MyIP(Language language = Language::CHINESE)
{
    WiFiClientSecure client;
    client.setInsecure();
    HTTPClient httpClient;

    String MyIPAPI = (language == Language::CHINESE) ? GET_IP_LOCATION_URL_CN : GET_IP_LOCATION_URL_EN;
    Serial.printf("Get IP from: %s\n", MyIPAPI.c_str());
    if (httpClient.begin(client, MyIPAPI))
    {
        u8_t httpCode = httpClient.GET();
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
        {
            String payload = httpClient.getString();
            Serial.println(payload);
            DynamicJsonDocument doc(384);
            deserializeJson(doc, payload);

            
            IP = doc["ip"].as<String>();            
            JsonObject location = doc["location"];
            City = location["city"].as<String>();                 // "北京"
            CountryCode = location["country_code"].as<String>(); // "CN"
            CountryName = location["country_name"].as<String>(); // "中国"
            Latitude = location["latitude"].as<String>();         // "39.938884"
            Longitude = location["longitude"].as<String>();       // "116.397459"
            Province = location["province"].as<String>();         // "北京

        }
        else
        {
            Serial.println("Failed to get ip information.");
        }

        httpClient.end();
        client.stop();
    }
    else
    {
        Serial.println("Failed to get ip information.");
    }
}

