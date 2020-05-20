#ifndef _MAIN_USER_H_
#define _MAIN_USER_H_
///////////// Std Lib //////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//////////// FreeRTOS /////////////
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

/////////// ESP Lib ///////////////
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "esp_netif.h"
#include "esp_event.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "esp_task_wdt.h"

//////////// User Lib /////////////
#include "airkiss.h"
#include "driver/uart.h"
#include "usr_spiffs.h"
#include "station.h"
#include "mqtt.h"

#endif
