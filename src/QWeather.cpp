#include "QWeather.h"
#include "urlencode.h"

/**
 * @brief 和风天气城市信息搜索API基地址
 * 
 */
const String QWEATHER_GEOAPI_BASE_URL = "https://geoapi.qweather.com";

/**
 * @brief 和风天气预报和实况信息查询商业版基地址
 * 
 */
const String QWEATHER_WEATHERAPI_BASE_URL_COMMERCIAL = "https://api.qweather.com/v7/weather";
/**
 * @brief 和风天气预报和实况信息查询开发版基地址
 * 
 */
const String QWEATHER_WEATHERAPI_BASE_URL_DEV = "https://devapi.qweather.com/v7/weather";


/**
 * @brief 和风天气预报和实况信息查询商业版基地址
 * 
 */
const String QWEATHER_AIRAPI_BASE_URL_COMMERCIAL = "https://api.qweather.com/v7/air";
/**
 * @brief 和风天气预报和实况信息查询开发版基地址
 * 
 */
const String QWEATHER_AIRAPI_BASE_URL_DEV = "https://devapi.qweather.com/v7/air";


void QWeather::Config(String key, String l, UnitType unitType, APIVersion apiVersion)
{
    SetUserKey(key);
    SetLanguage(l);
    SetUnitType(unitType);
    SetAPIVersion(apiVersion);
}
void QWeather::SetAPIVersion(APIVersion apiVersion)
{
    _apiVersion = apiVersion;
}

void QWeather::SetUserKey(String key)
{
    _key = key;
}

void QWeather::SetLanguage(String l)
{
    _lang = l;
}

void QWeather::SetUnitType(UnitType unitType)
{
    _unitType = unitType;
}

vector<GeoInfo> QWeather::GetGeoInfoList(String location, String adm, String range, uint8_t maxNumber)
{
    Serial.println("GetGeoInfoList from QWeather");
    vector<GeoInfo> result;
    WiFiClientSecure client;
    client.setInsecure();
    HTTPClient httpClient;

    String encodedURL = QWEATHER_GEOAPI_BASE_URL + "/v2/city/lookup?key=" + _key + "&gzip=n&location=" + urlencode(location) + "&adm=" + urlencode(adm) + "&range=" + range + "&number=" + String(maxNumber) + "&lang=" + _lang;
    Serial.println(encodedURL);

    if (httpClient.begin(encodedURL))
    {
        u8_t httpCode = httpClient.GET();
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
        {
            String payload = httpClient.getString();
            Serial.println(payload);
            DynamicJsonDocument doc(8192);
            deserializeJson(doc, payload);

            JsonArray locationArray = doc["location"];
            for (JsonVariant v : locationArray)
            {
                JsonObject loc = v.as<JsonObject>();
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
            Serial.printf("Connect to weather api server failed, the http status code is:%u\n", httpCode);
        }
    }
    else
    {
        Serial.println("Failed to connect to weather api server");
    }

    httpClient.end();
    client.stop();
    return result;
}

GeoInfo QWeather::GetGeoInfo(String location, String adm, String range)
{
    return GetGeoInfoList(location, adm, range, 1)[0];
}

CurrentWeather QWeather::GetCurrentWeather(String location)
{
    WiFiClientSecure client;
    client.setInsecure();
    HTTPClient httpClient;
    CurrentWeather result;

    String requestUrl = ((_apiVersion == APIVersion::DEV) ? QWEATHER_WEATHERAPI_BASE_URL_DEV : QWEATHER_WEATHERAPI_BASE_URL_COMMERCIAL) + "/now?gzip=n" + "&location=" + location + "&key=" + _key + "&lang=" + _lang + "&unit=" + ((_unitType == UnitType::METRIC) ? "m" : "i");
    Serial.println(requestUrl);
    if (httpClient.begin(requestUrl))
    {
        u8_t httpCode = httpClient.GET();
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
        {
            String payload = httpClient.getString();
            Serial.println(payload);
            DynamicJsonDocument doc(1024);
            deserializeJson(doc, payload);
            JsonObject now = doc["now"];
            result.obsTime = now["obsTime"].as<String>();     // "2020-12-24T14:59+08:00"
            result.temp = now["temp"].as<String>();           // "4"
            result.feelsLike = now["feelsLike"].as<String>(); // "-1"
            result.icon = now["icon"].as<String>();           // "100"
            result.text = now["text"].as<String>();           // "晴"
            result.wind360 = now["wind360"].as<String>();     // "225"
            result.windDir = now["windDir"].as<String>();     // "西南风"
            result.windScale = now["windScale"].as<String>(); // "3"
            result.windSpeed = now["windSpeed"].as<String>(); // "13"
            result.humidity = now["humidity"].as<String>();   // "16"
            result.precip = now["precip"].as<String>();       // "0.0"
            result.pressure = now["pressure"].as<String>();   // "1019"
            result.vis = now["vis"].as<String>();             // "30"
            result.cloud = now["cloud"].as<String>();         // "0"
            result.dew = now["dew"].as<String>();             // "-22"
        }
        else
        {
            Serial.printf("Connect to weather api server failed, the http status code is:%u\n", httpCode);
        }
    }
    else
    {
        Serial.println("Get current weather failed");
    }
    httpClient.end();
    client.stop();
    return result;
}

vector<DailyWeather> QWeather::GetDailyWeather(String location, DailyPredictionType dailyPredictionType)
{
    WiFiClientSecure client;
    client.setInsecure();
    HTTPClient httpClient;
    vector<DailyWeather> result;
    String requestUrl = ((_apiVersion == APIVersion::DEV) ? QWEATHER_WEATHERAPI_BASE_URL_DEV : QWEATHER_WEATHERAPI_BASE_URL_COMMERCIAL) + "/"+ String(dailyPredictionType) +"d?gzip=n" + "&location=" + location + "&key=" + _key + "&lang=" + _lang + "&unit=" + ((_unitType == UnitType::METRIC) ? "m" : "i");
    Serial.println(requestUrl);
    if (httpClient.begin(requestUrl))
    {
        u8_t httpCode = httpClient.GET();
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
        {
            String payload = httpClient.getString();
            DynamicJsonDocument doc(dailyPredictionType * 1024 + 512);
            deserializeJson(doc, payload);

            JsonArray dailyArray = doc["daily"];
            for (JsonVariant v : dailyArray)
            {
                JsonObject daily = v.as<JsonObject>();
                DailyWeather dw;
                dw.fxDate = daily["fxDate"].as<String>();
                dw.sunrise = daily["sunrise"].as<String>();
                dw.sunset = daily["sunset"].as<String>();
                dw.moonrise = daily["moonrise"].as<String>();
                dw.moonset = daily["moonset"].as<String>();
                dw.moonPhase = daily["moonPhase"].as<String>();
                dw.tempMax = daily["tempMax"].as<String>();
                dw.tempMin = daily["tempMin"].as<String>();
                dw.Day.icon = daily["iconDay"].as<String>();
                dw.Day.text = daily["textDay"].as<String>();
                dw.Day.wind360 = daily["wind360Day"].as<String>();
                dw.Day.windDir = daily["windDirDay"].as<String>();
                dw.Day.windScale = daily["windScaleDay"].as<String>();
                dw.Day.windSpeed = daily["windSpeedDay"].as<String>();
                dw.Night.icon = daily["iconNight"].as<String>();
                dw.Night.text = daily["textNight"].as<String>();
                dw.Night.wind360 = daily["wind360Night"].as<String>();
                dw.Night.windDir = daily["windDirNight"].as<String>();
                dw.Night.windScale = daily["windScaleNight"].as<String>();
                dw.Night.windSpeed = daily["windSpeedNight"].as<String>();
                dw.humidity = daily["humidity"].as<String>();
                dw.precip = daily["precip"].as<String>();
                dw.pressure = daily["pressure"].as<String>();
                dw.vis = daily["vis"].as<String>();
                dw.cloud = daily["cloud"].as<String>();
                dw.uvIndex = daily["uvIndex"].as<String>();
                result.push_back(dw);
            }
        }
        else
        {
            Serial.printf("Connect to daily weather api failed, the http status code is:%u\n", httpCode);
        }
    }
    else
    {
        Serial.println("Get daily weather failed");
    }

    httpClient.end();
    client.stop();
    return result;
}

vector<HourlyWeather> QWeather::GetHourlyWeather(String location, HourlyPredictionType hourlyPredictionType)
{
    WiFiClientSecure client;
    client.setInsecure();
    HTTPClient httpClient;
    vector<HourlyWeather> result;
    String requestUrl = ((_apiVersion == APIVersion::DEV) ? QWEATHER_WEATHERAPI_BASE_URL_DEV : QWEATHER_WEATHERAPI_BASE_URL_COMMERCIAL) + "/"+ String(hourlyPredictionType) +"h?gzip=n" + "&location=" + location + "&key=" + _key + "&lang=" + _lang + "&unit=" + ((_unitType == UnitType::METRIC) ? "m" : "i");
    if (httpClient.begin(requestUrl))
    {
        u8_t httpCode = httpClient.GET();
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
        {
            String payload = httpClient.getString();
            DynamicJsonDocument doc(hourlyPredictionType * 512);
            deserializeJson(doc, payload);

            JsonArray hourlyArray = doc["hourly"];
            for (JsonVariant v : hourlyArray)
            {
                JsonObject hourly = v.as<JsonObject>();
                HourlyWeather hw;
                hw.fxTime = hourly["fxTime"].as<String>();
                hw.temp = hourly["temp"].as<String>();
                hw.icon = hourly["icon"].as<String>();
                hw.text = hourly["text"].as<String>();
                hw.wind360 = hourly["wind360"].as<String>();
                hw.windDir = hourly["windDir"].as<String>();
                hw.windScale = hourly["windScale"].as<String>();
                hw.windSpeed = hourly["windSpeed"].as<String>();
                hw.humidity = hourly["humidity"].as<String>();
                hw.pop = hourly["pop"].as<String>();
                hw.precip = hourly["precip"].as<String>();
                hw.pressure = hourly["pressure"].as<String>();
                hw.cloud = hourly["cloud"].as<String>();
                hw.dew = hourly["dew"].as<String>();
                result.push_back(hw);
            }
        }
        else
        {
            Serial.printf("Connect to hourly weather api failed, the http status code is:%u\n", httpCode);
        }
    }
    else
    {
        Serial.println("Get hourly weather failed");
    }

    httpClient.end();
    client.stop();
    return result;    
}

CurrentAirQuality QWeather::GetCurrentAirQuality(String location)
{
    WiFiClientSecure client;
    client.setInsecure();
    HTTPClient httpClient;
    CurrentAirQuality result;
    String requestUrl = ((_apiVersion == APIVersion::DEV) ? QWEATHER_AIRAPI_BASE_URL_DEV : QWEATHER_AIRAPI_BASE_URL_COMMERCIAL) + "/now?gzip=n" +  "&location=" + location + "&key=" + _key + "&lang=" + _lang;
    Serial.println(requestUrl);
    if (httpClient.begin(requestUrl))
    {
        u8_t httpCode = httpClient.GET();
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
        {
            String payload = httpClient.getString();
            Serial.println(payload);
            DynamicJsonDocument doc(8192);
            deserializeJson(doc, payload);

            JsonObject now = doc["now"];
            result.pubTime = now["pubTime"].as<String>(); 
            result.aqi = now["aqi"].as<String>();
            result.level = now["level"].as<String>();
            result.category = now["category"].as<String>();
            result.primary = now["primary"].as<String>();
            result.pm10 = now["pm10"].as<String>();
            result.pm2p5 = now["pm2p5"].as<String>();
            result.no2 = now["no2"].as<String>();
            result.so2 = now["so2"].as<String>();
            result.co = now["co"].as<String>();
            result.o3 = now["o3"].as<String>();

            JsonArray stationsArray = doc["station"].as<JsonArray>();
            for (JsonVariant v : stationsArray)
            {
                JsonObject s = v.as<JsonObject>();
                AirStationData asd;
                asd.pubTime = s["pubTime"].as<String>();
                asd.name = s["name"].as<String>();
                asd.id = s["id"].as<String>();
                asd.aqi = s["aqi"].as<String>();
                asd.level = s["level"].as<String>();
                asd.category = s["category"].as<String>();
                asd.primary = s["primary"].as<String>();
                asd.pm10 = s["pm10"].as<String>();
                asd.pm2p5 = s["pm2p5"].as<String>();
                asd.no2 = s["no2"].as<String>();
                asd.so2 = s["so2"].as<String>();
                asd.co = s["co"].as<String>();
                asd.o3 = s["o3"].as<String>();
                result.Stations.push_back(asd);
            }
        }
        else
        {
            Serial.printf("Connect to air quality api failed, the http status code is:%u\n", httpCode);
        }
    }
    else
    {
        Serial.println("Get current air quality failed");
    }

    httpClient.end();
    client.stop();
    return result;  
}