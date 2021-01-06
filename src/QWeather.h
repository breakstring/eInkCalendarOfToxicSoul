#ifndef _QWEATHER_H
#define _QWEATHER_H

#include <Arduino.h>
#include <Vector>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <string>

using namespace std;

/**
 * @brief 按小时预测天气。。。和按天预测一样，请求越多需求内存越高
 * 
 */
enum HourlyPredictionType
{
    /**
     * @brief 1天的
     * 
     */
    H24 = 24,
    /**
     * @brief 3天的
     * 
     */
    H72 = 72,
    /**
     * @brief 7天的
     * 
     */
    H128 =  128
};

/**
 * @brief 按天获取预报的类型，同时请求的数据越多，对于处理数据的内存也越高。。。所以，能少请求就少请求
 * 
 */
enum DailyPredictionType
{
    /**
     * @brief 3天
     * 
     */
    D3 = 3, 
    /**
     * @brief 7天
     * 
     */
    D7 = 7, 
    /**
     * @brief 10天
     * 
     */
    D10 = 10, //6144
    /**
     * @brief 15天
     * 
     */
    D15 = 15
};

/**
 * @brief 度量单位
 * 
 */
enum UnitType
{
    /**
     * @brief 公制
     * 
     */
    METRIC,
    /**
     * @brief 英制
     * 
     */
    IMPERIAL
};

/**
 * @brief API类型
 * 
 */
enum APIVersion
{
    /**
     * @brief 商业版本
     * 
     */
    COMMERCIAL,
    /**
     * @brief 开发版本
     * 
     */
    DEV
};

/**
 * @brief 基本天气信息
 * 
 */
struct BasicWeatherData
{

    /**
     * @brief 天气状况图标
     * 
     */
    String icon;
    /**
     * @brief 天气状况文字描述
     * 
     */
    String text;
};

/**
 * @brief 风力数据
 * 
 */
struct WindData{
    /**
     * @brief 风向360角度
     * 
     */
    String wind360;
    /**
     * @brief 风向
     * 
     */
    String windDir;
    /**
     * @brief 风力等级
     * 
     */
    String windScale;
    /**
     * @brief 风速
     * 
     */
    String windSpeed;    
};


/**
 * @brief 扩展天气数据
 * 
 */
struct ExtendedWeatherData
{
    /**
     * @brief 相对湿度
     * 
     */
    String humidity;
    /**
     * @brief 降水量
     * 
     */
    String precip;
    /**
     * @brief 大气压强
     * 
     */
    String pressure;
    /**
     * @brief 能见度
     * 
     */
    String vis;
    /**
     * @brief 云量
     * 
     */
    String cloud;
    /**
     * @brief 实况露点温度
     * 
     */
    String dew;
    /**
     * @brief 紫外线强度
     * 
     */
    String uvIndex;
};
/**
 * @brief 按日请求时每天的综合数据组合
 * 
 */
struct DailyCommonData: BasicWeatherData, WindData
{

};
/**
 * @brief 按小时预报的天气情况
 * 
 */
struct HourlyWeather: BasicWeatherData,WindData,ExtendedWeatherData
{
    /**
     * @brief 预报时间
     * 
     */
    String fxTime;

    /**
     * @brief 温度
     * 
     */
    String temp;

    /**
     * @brief 降水概率
     * 
     */
    String pop;


};
/**
 * @brief 当前天气
 * 
 */
struct CurrentWeather : BasicWeatherData, ExtendedWeatherData, WindData 
{
    /**
     * @brief 实况观测时间
     * 
     */
    String obsTime;
    /**
     * @brief 温度
     * 
     */
    String temp;
    /**
     * @brief 体感温度
     * 
     */
    String feelsLike;



};

/**
 * @brief 按天预报的气象数据
 * 
 */
struct DailyWeather: ExtendedWeatherData
{
    /**
     * @brief 预报时间
     * 
     */
    String fxDate;
    /**
     * @brief 日出时间
     * 
     */
    String sunrise;
    /**
     * @brief 日落时间
     * 
     */
    String sunset;
    /**
     * @brief 月升时间
     * 
     */
    String moonrise;
    /**
     * @brief 月落时间
     * 
     */
    String moonset;
    /**
     * @brief 月相名称
     * 
     */
    String moonPhase;
    /**
     * @brief 当天最高温度
     * 
     */
    String tempMax;
    /**
     * @brief 当天最低温度
     * 
     */
    String tempMin;

    /**
     * @brief 白天的天气数据
     * 
     */
    DailyCommonData Day;
    /**
     * @brief 晚上的天气数据
     * 
     */
    DailyCommonData Night;

};

/**
 * @brief 和风天气API中位置信息
 * 
 */
struct GeoInfo
{
    /**
     * @brief 地区/城市名称
     * 
     */
    String name;
    /**
     * @brief 地区/城市ID
     * 
     */
    String id;
    /**
     * @brief 地区/城市维度
     * 
     */
    String lat;
    /**
     * @brief 地区/城市经度
     * 
     */
    String lon;
    /**
     * @brief 该地区/城市的上级行政区划名称
     * 
     */
    String adm2;
    /**
     * @brief 该地区/城市所属一级行政区域
     * 
     */
    String adm1;
    /**
     * @brief 该地区/城市所属国家名称
     * 
     */
    String country;
    /**
     * @brief 时区
     * 
     */
    String tz;
    /**
     * @brief UTC时区偏移
     * 
     */
    String utcOffset;
    /**
     * @brief 当时是否处于夏令时
     * 
     */
    String isDst;
    /**
     * @brief 属性
     * 
     */
    String type;
    /**
     * @brief 评分
     * 
     */
    String rank;
    /**
     * @brief 天气预报链接
     * 
     */
    String fxLink;
};



/**
 * @brief 空气质量基础数据
 * 
 */
struct AirQualityData
{
    /**
     * @brief 空气质量发布时间
     * 
     */
    String pubTime;
    /**
     * @brief 空气质量指数
     * 
     */
    String aqi;
    /**
     * @brief 空气质量等级
     * 
     */
    String level;
    /**
     * @brief 空气质量级别
     * 
     */
    String category;
    /**
     * @brief 主要污染物
     * 
     */
    String primary;
    /**
     * @brief PM10
     * 
     */
    String pm10;
    /**
     * @brief PM2.5
     * 
     */
    String pm2p5;
    /**
     * @brief 二氧化碳
     * 
     */
    String no2;
    /**
     * @brief 二氧化硫
     * 
     */
    String so2;
    /**
     * @brief 一氧化碳
     * 
     */
    String co;
    /**
     * @brief 臭氧
     * 
     */
    String o3;
};
/**
 * @brief 空气质量监测站数据
 * 
 */
struct AirStationData : AirQualityData
{

    /**
     * @brief 站点名称
     * 
     */
    String name;
    /**
     * @brief 站点ID
     * 
     */
    String id;
};

/**
 * @brief 当前空气质量
 * 
 */
struct CurrentAirQuality: AirQualityData
{
    /**
     * @brief 当前城市各个空气监测站数据
     * 
     */
    vector<AirStationData> Stations;
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
    UnitType _unitType;
    String GetLanguage();
    APIVersion _apiVersion;

public:
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
     * @brief 设置API版本
     * 
     * @param apiVersion 
     */
    void SetAPIVersion(APIVersion apiVersion);

    /**
     * @brief 设置度量单位
     * 
     * @param unitType 度量单位
     */
    void SetUnitType(UnitType unitType);

    /**
     * @brief 按照查询条件返回可能的城市信息列表
     * 
     * @param location 输入需要查询的城市名称，可使用Location ID、多语言文字、以逗号分隔的经度/纬度坐标、ADCode（仅限中国城市）。例如location=beijing， location=116.4,39.1
     * @param adm 城市所属行政区划，只支持多语言文字，默认不限定行政区划。 可设定只在某个行政区划范围内进行搜索，用于出现重名城市或需要对结果进行过滤的时候使用。例如adm=beijing
     * @param range 搜索范围，默认搜索全球城市。 可设定只在某个国家范围内进行搜索，国家名称需使用ISO 3166 所定义的国家代码。例如 range=us 或者 range=world 查询全球
     * @param maxNumber 返回城市的数量，取值范围1-20，默认返回10个结果。
     * @return vector<GeoInfo> 
     */
    vector<GeoInfo> GetGeoInfoList(String location, String adm = "", String range = "cn", uint8_t maxNumber = 10);

    /**
     * @brief 按照查询条件返回最有可能的一个城市
     * 
     * @param location 输入需要查询的城市名称，可使用Location ID、多语言文字、以逗号分隔的经度/纬度坐标、ADCode（仅限中国城市）。例如location=beijing， location=116.4,39.1
     * @param adm 城市所属行政区划，只支持多语言文字，默认不限定行政区划。 可设定只在某个行政区划范围内进行搜索，用于出现重名城市或需要对结果进行过滤的时候使用。例如adm=beijing
     * @param range 搜索范围，默认搜索全球城市。 可设定只在某个国家范围内进行搜索，国家名称需使用ISO 3166 所定义的国家代码。例如 range=us 或者 range=world 查询全球
     * @return GeoInfo 
     */
    GeoInfo GetGeoInfo(String location, String adm = "", String range = "cn");

    /**
     * @brief 获取指定地区当前天气情况
     * 
     * @param location 需要查询地区的LocationID或以逗号分隔的经度/纬度坐标（十进制），LocationID可通过GetGeoInfo获取。例如： location=101010100 或 location=116.41,39.92
     * @return CurrentWeather 
     */
    CurrentWeather GetCurrentWeather(String location);

    /**
     * @brief 获取几天的天气预测
     * 
     * @param location 需要查询地区的LocationID或以逗号分隔的经度/纬度坐标（十进制），LocationID可通过GetGeoInfo获取。例如： location=101010100 或 location=116.41,39.92
     * @param dailyPredictionType 按天查询的类型
     * @return vector<DailyWeather> 
     */
    vector<DailyWeather> GetDailyWeather(String location, DailyPredictionType dailyPredictionType = DailyPredictionType::D7);

    /**
     * @brief 按小时获取天气预测
     * 
     * @param location 需要查询地区的LocationID或以逗号分隔的经度/纬度坐标（十进制），LocationID可通过GetGeoInfo获取。例如： location=101010100 或 location=116.41,39.92
     * @param hourlyPredictionType 按小时查询的类型
     * @return vector<HourlyWeather> 
     */
    vector<HourlyWeather> GetHourlyWeather(String location, HourlyPredictionType hourlyPredictionType = HourlyPredictionType::H24);

    /**
     * @brief 获取当前空气质量
     * 
     * @param location 需要查询地区的LocationID或以逗号分隔的经度/纬度坐标（十进制），LocationID可通过GetGeoInfo获取。例如： location=101010100 或 location=116.41,39.92
     * @return CurrentAirQuality 
     */
    CurrentAirQuality GetCurrentAirQuality(String location);
    /**
     * @brief 配置和风API参数
     * 
     * @param key User key
     * @param l Language, Refer to: https://dev.qweather.com/docs/start/language 
     * @param unitType 度量单位
     * @param apiVersion API请求类型
     */
    void Config(String key, String l = "zh", UnitType unitType = UnitType::METRIC, APIVersion apiVersion = APIVersion::DEV);
};

#endif