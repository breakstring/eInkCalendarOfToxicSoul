#include "SmartConfigManager.h"
#include "esp_system.h"
#include <esp_wifi.h>
#include "WiFi.h"
#include <string.h>
#include <Arduino.h>

String WiFi_SSID;
String WiFi_Password;
const char *scSSID;
const char *scPassword;
const u8_t WIFI_RECONNECT_LIMITATION = 30;

/**
 * @brief 等待重连无线网络
 * 
 * @return true 成功
 * @return false 失败
 */
bool SmartConfigManager::WaitingForConnectWiFiWithTimeout()
{
  u8_t reconnectCount = 0;
  while (WiFi.status() != WL_CONNECTED && reconnectCount < WIFI_RECONNECT_LIMITATION)
  {
    delay(500);
    Serial.print(".");
    reconnectCount++;
  }

  return (WiFi.status() == WL_CONNECTED);
}

/**
 * @brief 尝试通过SmartConfig配置无线网络
 * 
 * @return true 连接成功
 * @return false 连接失败
 */
bool SmartConfigManager::TryConnectWiFiWithSmartConfig(WaitingFormSmartConfigHandler handler)
{

  WiFi.beginSmartConfig();
  //Wait for SmartConfig packet from mobile
  handler();
  while (!WiFi.smartConfigDone())
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("SmartConfig received.");

  return WaitingForConnectWiFiWithTimeout();
}

void SmartConfigManager::initWiFi(WaitingFormSmartConfigHandler handler)
{


  WiFi.mode(WIFI_AP_STA);

  try
  {
    wifi_config_t conf;
    esp_err_t espWiFiGetConfigResult = esp_wifi_get_config(WIFI_IF_STA, &conf); // load wifi settings to struct conf
    if (espWiFiGetConfigResult == ESP_OK)
    {
      scSSID = reinterpret_cast<const char *>(conf.sta.ssid);
      scPassword = reinterpret_cast<const char *>(conf.sta.password);
      WiFi_SSID = String(scSSID);
      WiFi_Password = String(scPassword);
      if (WiFi_SSID != "" && WiFi_Password != "")
      {
        Serial.println("Try connect WiFi with saved credentials.");
        WiFi.reconnect();
        if (WaitingForConnectWiFiWithTimeout())
        {
          Serial.printf("Connect WiFi successfully with IP: '%s'\n", WiFi.localIP().toString().c_str());
          return;
        }
        else
        {
          Serial.println("Connect WiFi timeout, will re-connect with SmartConfig QR Code.");
        }
        
      }
    }
  }
  catch (const std::exception &e)
  {
    Serial.printf("Error: %s\n", e.what());
    esp_restart();

  }

  while (!TryConnectWiFiWithSmartConfig(handler))
  {
    Serial.println("Retry SmartConfig.");
    WiFi.stopSmartConfig();
  }
  Serial.printf("Connect WiFi successfully with IP: '%s'\n", WiFi.localIP().toString().c_str());
  return;
}
