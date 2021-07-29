#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>


// onboarding_config_start is null-terminated string
extern const uint8_t onboarding_config_start[]    asm("_binary_onboarding_config_json_start");
extern const uint8_t onboarding_config_end[]    asm("_binary_onboarding_config_json_end");

// device_info_start is null-terminated string
extern const uint8_t device_info_start[]    asm("_binary_device_info_json_start");
extern const uint8_t device_info_end[]        asm("_binary_device_info_json_end");


void app_main(void) 
{
    unsigned char *onboarding_config = (unsigned char *) onboarding_config_start;
    unsigned int onboarding_config_len = onboarding_config_end - onboarding_config_start;
    unsigned char *device_info = (unsigned char *) device_info_start;
    unsigned int device_info_len = device_info_end - device_info_start;

    int iot_err;

    for (;;)
    {
        printf("LOOP\n");
        /* code */
    }
    // st_dev.h
    // ctx = st_conn_init(onboarding_config, onboarding_config_len, device_info, device_info_len);
    // if (ctx != NULL) {
    //     iot_err = st_conn_set_noti_cb(ctx, iot_noti_cb, NULL);
    //     if (iot_err)
    //         printf("fail to set notification callback function\n");
    // } else {
    //     printf("fail to create the iot_context\n");
    // }

    // caps_*.h 
    // capability_init();

    // device.h
    // iot_gpio_init();

    // connection_start();

    // device input handling
    // iot_os_thread_create(app_main_task, "app_main_task", 2048, NULL, 10, NULL);
}