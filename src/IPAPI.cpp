#include "IPAPI.h"

const char *LOCALIZATION[] = {"en","de","es","pt-BR","fr","ja","zh-CN","ru"};

IPAPIResponse GetIPInfomation(String ip, Localization localization)
{
    WiFiClient client;
    HTTPClient httpClient;

    IPAPIResponse response;
    response.status = "fail";

    String requestUrl = "http://ip-api.com/json/" + ip +"?lang=" + LOCALIZATION[localization] + "&fields=66846719";
    Serial.println(requestUrl);

    if(httpClient.begin(client,requestUrl))
    {
        u8_t httpCode = httpClient.GET();
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
        {
            String payload = httpClient.getString();
            Serial.println(payload);

            StaticJsonDocument<1024> doc;
            deserializeJson(doc, payload);

            response.query = doc["query"].as<String>();
            response.status = doc["status"].as<String>();

            if(response.status == "success"){
                response.continent = doc["continent"].as<String>();
                response.continentCode = doc["continentCode"].as<String>();
                response.country = doc["country"].as<String>();
                response.countryCode = doc["countryCode"].as<String>();
                response.region = doc["region"].as<String>();
                response.regionName = doc["regionName"].as<String>();
                response.city = doc["city"].as<String>();
                response.district = doc["district"].as<String>();
                response.zip = doc["zip"].as<String>();
                response.lat = doc["lat"].as<float>();
                response.lon = doc["lon"].as<float>();
                response.timezone = doc["timezone"].as<String>();
                response.offset = doc["offset"].as<int32_t>();
                response.currency = doc["currency"].as<String>();
                response.isp = doc["isp"].as<String>();
                response.org = doc["org"].as<String>();
                response.as = doc["as"].as<String>();
                response.asname = doc["asname"].as<String>();
                response.reverse = doc["reverse"].as<String>();
                response.mobile = doc["mobile"].as<bool>();
                response.proxy = doc["proxy"].as<bool>();
                response.hosting = doc["hosting"].as<bool>();
            }
            else
            {
                response.message = doc["message"].as<String>();
            }
            
        }
        else
        {
            response.message = "http code:" + httpCode;
        }
        
    }
    else
    {
        response.message = "httpClient failed.";
    }
    
    httpClient.end();
    client.stop();
    return response;
}
