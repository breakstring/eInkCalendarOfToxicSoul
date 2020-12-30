#ifndef _SMARTCONFIGMANAGER_H
#define _SMARTCONFIGMANAGER_H
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