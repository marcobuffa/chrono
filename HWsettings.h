#ifdef __cplusplus
extern "C" {
#endif

#ifndef HWSETTINGS_H_INCLUDED
#define HWSETTINGS_H_INCLUDED

//Physical buttons #pin connection
//Please use interrupt capable Arduino pins
const int button2 = 2;
const int button3 = 3;

const int heat = 13;

//Hitachi like 16x2 pin connection (4 bit mode)
const int rs=7, en=8, d4=9, d5=10, d6=11, d7=12;

//DHT11 room sensor data pin and type
const int dhtData = 4;
const int dhtType = DHT_TYPE_11;

#endif //HWSETTINGS_H_INCLUDED

#ifdef __cplusplus
} // extern "C"
#endif
