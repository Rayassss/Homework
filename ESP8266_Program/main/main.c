#include "include/main.h"
static QueueHandle_t uart0_queue;
static char Rcv_Tmp[64];
void user_task(void *pvParameters)
{
	uart_event_t event;
	uint8_t buf_cnt = 0;
	uint8_t *dtmp = (uint8_t *) malloc(1024);
	memset(Rcv_Tmp,0x00,sizeof(Rcv_Tmp));
	mqtt_start();
	vTaskDelay(50);
	while(1)
	{
		if (xQueueReceive(uart0_queue, (void *)&event, (portTickType)portMAX_DELAY)) 
		{
			bzero(dtmp, 1024);
		    switch (event.type) 
		    {
				case UART_DATA:
					uart_read_bytes(UART_NUM_0, dtmp, 1, portMAX_DELAY);
					if(*dtmp=='`')
					{
						Publish_MQTT("humi",Rcv_Tmp);
						memset(Rcv_Tmp,0x00,sizeof(Rcv_Tmp));
						buf_cnt = 0;
						printf("\r\nPublished!\r\n");
					}
					else
					{
						uart_write_bytes(UART_NUM_0, (const char *) dtmp, 1);
						Rcv_Tmp[buf_cnt] = *dtmp;
						buf_cnt++;
					}
					break;
				case UART_FIFO_OVF:
					uart_flush_input(UART_NUM_0);
					xQueueReset(uart0_queue);
					break;
				case UART_BUFFER_FULL:
					uart_flush_input(UART_NUM_0);
					xQueueReset(uart0_queue);
					break;
                default:
                    ESP_LOGI("UART", "uart event type: %d", event.type);
                    break;
		    }
		}
	}
}
void app_main()
{
	char usr_ssid[32] = "D-Link_822";
	char usr_psswd[32] = "abc0987654321abc";
	uart_config_t uart_config = {
	        .baud_rate = 115200,
	        .data_bits = UART_DATA_8_BITS,
	        .parity = UART_PARITY_DISABLE,
	        .stop_bits = UART_STOP_BITS_1,
	        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
	};
	uart_param_config(UART_NUM_0, &uart_config);
	uart_driver_install(UART_NUM_0, 2048 , 2048 , 100, &uart0_queue, 0);
    if(check_config() == NO_CONFIGURED)
    	airkiss_config();
    else{
    	read_wifi_config(usr_ssid,usr_psswd);
    	wifi_connect(usr_ssid,usr_psswd);
    	vTaskDelay(20);
    	xTaskCreate(&user_task, "user_task", 10240, NULL, 3, NULL);
    }
    report_spiffs();
}
