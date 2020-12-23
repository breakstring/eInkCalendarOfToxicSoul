#include "QWeather.h"
#define enum_to_string(x) #x

const String QWEATHER_BASE_URL = "https://geoapi.qweather.com";


QWeather::QWeather(String key, lang l)
{
    _key = key;
    _lang = l;
}

String QWeather::GetLanguage(lang l)
{
    String result = "";
    switch(l)
    {
        case lang::zh_hans:
            result = "zh-hans";
            break;
        case lang::zh_hant:
            result = "zh-hant";
            break;
        default:
            result = enum_to_string(l);
            break;
    }
    return result;
}

void QWeather::SetUserKey(String key)
{
    _key = key;
}

void QWeather::SetLanguage(lang l)
{
    _lang = l;
}

vector<GeoInfo> QWeather::GetGeoInfoList(String location, String adm = "", String range = "cn",uint8_t maxNumber = 10)
{
    vector<GeoInfo> result;
    WiFiClientSecure client;
    client.setInsecure();
    HTTPClient httpClient;

    String requestURL = QWEATHER_BASE_URL + "/v2/city/lookup?key=" + _key + "&location=" + location + "&adm="+ adm +"&range=" + range + "&number=" + String(maxNumber) + "&lang=" + GetLanguage(_lang);
    if(httpClient.begin(requestURL))
    {
        u8_t httpCode = httpClient.GET();
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
        {
            String payload = httpClient.getString();
            Serial.println(payload);

            DynamicJsonDocument doc(8192);
            deserializeJson(doc, payload);

            JsonArray locationArray = doc["location"];
            for(JsonVariant v : locationArray) {
                JsonObject loc =  v.as<JsonObject>();
                Serial.println(v.as<int>());
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

        httpClient.end();
        client.stop();
    }
    return result;
}

GeoInfo QWeather::GetGeoInfo(String location,String adm = "", String range = "cn")
{
    return GetGeoInfoList(location,adm,range)[0];
}
