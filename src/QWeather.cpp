#include "QWeather.h"
#include "urlencode.h"

const String QWEATHER_BASE_URL = "https://geoapi.qweather.com";





QWeather::QWeather(String key, String l)
{
    _key = key;
    _lang = l;
}



void QWeather::SetUserKey(String key)
{
    _key = key;
}

void QWeather::SetLanguage(String l)
{
    _lang = l;
}

vector<GeoInfo> QWeather::GetGeoInfoList(String location, String adm, String range,uint8_t maxNumber)
{
    vector<GeoInfo> result;
    WiFiClientSecure client;
    client.setInsecure();
    HTTPClient httpClient;

    String encodedURL = QWEATHER_BASE_URL + "/v2/city/lookup?key=" + _key + "&gzip=n&location=" + urlencode(location) + "&adm="+ urlencode(adm) +"&range=" + range + "&number=" + String(maxNumber) + "&lang=" + _lang;

    if(httpClient.begin(encodedURL))
    {
        u8_t httpCode = httpClient.GET();
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
        {
            String payload = httpClient.getString();

            DynamicJsonDocument doc(8192);
            deserializeJson(doc, payload);

            JsonArray locationArray = doc["location"];
            for(JsonVariant v : locationArray) {
                JsonObject loc =  v.as<JsonObject>();
                GeoInfo gi;
                gi.name = loc["name"].as<String>(); 
                gi.id = loc["id"].as<String>();
                gi.lat = loc["lat"].as<String>();
                gi.lon = loc["lon"].as<String>(); 
                gi.adm1 = loc["adm1"].as<String>();
                gi.adm2 = loc["adm2"].as<String>();
                gi.country = loc["country"].as<String>();
                gi.tz = loc["tz"].as<String>();
                gi.utcOffset = loc["utcOffset"].as<String>();
                gi.isDst = loc["isDst"].as<String>();
                gi.type = loc["type"].as<String>();
                gi.rank = loc["rank"].as<String>();
                gi.fxLink = loc["fxLink"].as<String>();
                result.push_back(gi);
            }

        }
        else
        {
            Serial.printf("Connect to weather api server failed, the http status code is:%u\n",httpCode);
            String payload = httpClient.getString();
            Serial.println(payload);
        }
        
        httpClient.end();
        client.stop();
    }
    else
    {
        Serial.println("Failed to connect to weather api server");
    }
    
    return result;
}

GeoInfo QWeather::GetGeoInfo(String location,String adm, String range)
{
    return GetGeoInfoList(location,adm,range,1)[0];
}
