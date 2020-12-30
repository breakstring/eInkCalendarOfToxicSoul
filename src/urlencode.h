#ifndef _URLENCODE_H
#define _URLENCODE_H
// https://github.com/zenmanenergy/ESP8266-Arduino-Examples/blob/master/helloWorld_urlencoded/urlencode.ino



#include <Arduino.h>
#include <WString.h>


String urldecode(String str);
String urlencode(String str);
unsigned char h2int(char c);


#endif