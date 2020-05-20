#ifndef _SPIFFS_USER_H_
#define _SPIFFS_USER_H_

#define EXISTED 1
#define NON_EXISTED 0

void report_spiffs(void);
unsigned char check_wifi_config();

void write_wifi_config(char *conf);
unsigned char read_wifi_config(char *ssid,char *psswd);

void write_user_config(char *conf);
unsigned char read_user_config(char *conf);

#endif
