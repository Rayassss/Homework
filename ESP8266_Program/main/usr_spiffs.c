#include "esp_spiffs.h"
#include <stdio.h>
#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include "esp_err.h"
#include "esp_log.h"
#include "include/usr_spiffs.h"
#include "include/debug.h"

static const char *TAG = "spiffs";

void write_user_config(char *conf)
{
	struct stat st;
	if (stat("/spiffs/user_conf.txt", &st) == 0)
		unlink("/spiffs/user_conf.txt");
	FILE* f = fopen("/spiffs/user_conf.txt", "w");
	if (f == NULL) 
		ESP_LOGE(TAG, "Failed to open file for writing");
	fprintf(f, conf);
	fclose(f);
}
unsigned char read_user_config(char *conf)
{
	char tmp_buf[48];
	FILE *f = fopen("/spiffs/user_conf.txt", "r");
	if (f == NULL) {
		ESP_LOGE(TAG, "Failed to open file for reading");
		return 0;
	}
	fgets(tmp_buf, sizeof(tmp_buf), f);
	fclose(f);

	strcpy(conf,tmp_buf);
	return 1;
}
unsigned char check_wifi_config()
{
	esp_vfs_spiffs_conf_t conf = {
	      .base_path = "/spiffs",
	      .partition_label = NULL,
	      .max_files = 5,
	      .format_if_mount_failed = true
	};

	esp_err_t ret = esp_vfs_spiffs_register(&conf);
	if (ret != ESP_OK) {
	    if (ret == ESP_FAIL)
	        ESP_LOGE(TAG, "Failed to mount or format filesystem");
	    else if (ret == ESP_ERR_NOT_FOUND)
	        ESP_LOGE(TAG, "Failed to find SPIFFS partition");
	    else 
	        ESP_LOGE(TAG, "Failed to initialize SPIFFS (%s)", esp_err_to_name(ret));
	    return NON_EXISTED;
	}
	struct stat st;
	if (stat("/spiffs/config.txt", &st) == 0)
	{
		FILE *f = fopen("/spiffs/config.txt", "r");
		if (f == NULL) {
		    ESP_LOGE(TAG, "Failed to open file for reading");
		    return NON_EXISTED;
		}
		fclose(f);
		return EXISTED;
	}
	else
		return NON_EXISTED;
}
void write_wifi_config(char *conf)
{
	struct stat st;
	if (stat("/spiffs/config.txt", &st) == 0)
		unlink("/spiffs/config.txt");
    FILE* f = fopen("/spiffs/config.txt", "w");
    if (f == NULL) {
        ESP_LOGE(TAG, "Failed to open file for writing");
    }
    fprintf(f, conf);
    fclose(f);
}
unsigned char read_wifi_config(char *ssid,char *psswd)
{
	char tmp_buf[64];
	char tmp_psswd[32],tmp_ssid[32];
	FILE *f = fopen("/spiffs/config.txt", "r");
	if (f == NULL) {
		ESP_LOGE(TAG, "Failed to open file for reading");
		return 0;
	}
	fgets(tmp_buf, sizeof(tmp_buf), f);
	fclose(f);

	char* pos = strchr(tmp_buf, '`');
	if (pos)
		*pos = '\0';
	sprintf(tmp_ssid,"%s",tmp_buf);
	
#if DEBUG_FLAG == 1
	ESP_LOGI(TAG, "SSID: '%s'", tmp_buf);
#endif
	
	pos++;
	char* pos2 = strchr(pos, '\n');
	if (pos2)
		*pos2 = '\0';
	sprintf(tmp_psswd,"%s",pos);
	
#if DEBUG_FLAG == 1
	ESP_LOGI(TAG, "PASSWORD: '%s'", pos);
#endif
	
	strcpy(ssid,tmp_ssid);
	strcpy(psswd,tmp_psswd);
	return 1;
}
void report_spiffs(void)
{
    size_t total = 0, used = 0;
    esp_err_t ret = esp_spiffs_info(NULL, &total, &used);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to get SPIFFS partition information (%s)", esp_err_to_name(ret));
    } else {
        ESP_LOGI(TAG, "Partition size: total: %.2fKB, used: %.2fKB", total / 1024.0f, used / 1024.0f);
    }
    // All done, unmount partition and disable SPIFFS
//    esp_vfs_spiffs_unregister(NULL);
//    ESP_LOGI(TAG, "SPIFFS unmounted");
}
