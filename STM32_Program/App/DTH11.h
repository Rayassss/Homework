#ifndef _DTH11_H_
#define _DTH11_H_

#include "System_Config.h"

#define DHT11_OUT_H PBout(10) = 1
#define DHT11_OUT_L PBout(10) = 0
#define DHT11_IN    PBin(10)

u16 DTH11_ReadData(u8 *buffer);

#endif
