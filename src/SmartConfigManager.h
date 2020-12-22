#ifndef SmartConfigManager_h
#define SmartConfigManager_h
class SmartConfigManager
{

public:

    typedef void (*WaitingFormSmartConfigHandler)();
    void initWiFi(WaitingFormSmartConfigHandler handler);

private:
    bool WaitingForConnectWiFiWithTimeout();
    void ShowWiFiSmartConfig();
    bool TryConnectWiFiWithSmartConfig(WaitingFormSmartConfigHandler handler);
};

#endif