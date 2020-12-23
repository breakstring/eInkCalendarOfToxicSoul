#ifndef QWeather_h
#define QWeather_h

#include <Arduino.h>
#include <Vector>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <string>

using namespace std;


struct GeoInfo {
    String name;
    String id;
    String lat;
    String lon;
    String adm2;
    String adm1;
    String country;
    String tz;
    String utcOffset;
    String isDst;
    String type;
    String rank;
    String fxLink;
};

/**
 * @brief 和风天气API
 * 
 */
class QWeather
{
private:
    String _key;
    String _lang;
    String GetLanguage();

public:
    /**
     * @brief Construct a new QWeather object
     * 
     * @param key User key
     * @param l Language, Refer to: https://dev.qweather.com/docs/start/language 
     */
    QWeather(String key, String l =  "zh");
    /**
     * @brief Set the User Key
     * 
     * @param key User key
     */
    void SetUserKey(String key);
    /**
     * @brief Set the content Language 
     * 
     * @param l 语言，参见 https://dev.qweather.com/docs/start/language
     */
    void SetLanguage(String l);

    /**
     * @brief 按照查询条件返回可能的城市信息列表
     * 
     * @param location 输入需要查询的城市名称，可使用Location ID、多语言文字、以逗号分隔的经度/纬度坐标、ADCode（仅限中国城市）。例如location=beijing， location=116.4,39.1
     * @param adm 城市所属行政区划，只支持多语言文字，默认不限定行政区划。 可设定只在某个行政区划范围内进行搜索，用于出现重名城市或需要对结果进行过滤的时候使用。例如adm=beijing
     * @param range 搜索范围，默认搜索全球城市。 可设定只在某个国家范围内进行搜索，国家名称需使用ISO 3166 所定义的国家代码。例如 range=us 或者 range=world 查询全球
     * @param maxNumber 返回城市的数量，取值范围1-20，默认返回10个结果。
     * @return vector<GeoInfo> 
     */
    vector<GeoInfo> GetGeoInfoList(String location, String adm = "", String range = "cn",uint8_t maxNumber = 10);

    /**
     * @brief 按照查询条件返回最有可能的一个城市
     * 
     * @param location 输入需要查询的城市名称，可使用Location ID、多语言文字、以逗号分隔的经度/纬度坐标、ADCode（仅限中国城市）。例如location=beijing， location=116.4,39.1
     * @param adm 城市所属行政区划，只支持多语言文字，默认不限定行政区划。 可设定只在某个行政区划范围内进行搜索，用于出现重名城市或需要对结果进行过滤的时候使用。例如adm=beijing
     * @param range 搜索范围，默认搜索全球城市。 可设定只在某个国家范围内进行搜索，国家名称需使用ISO 3166 所定义的国家代码。例如 range=us 或者 range=world 查询全球
     * @return GeoInfo 
     */
    GeoInfo GetGeoInfo(String location, String adm = "", String range = "cn");
};


#endif