#include "camera.h"
#include "esp_camera.h"
#include "hal/can_data.h"
#include <math>
#include <algorithm>

#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

void Camera::setup() {
    camera_config_t config;
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.pin_d0 = Y2_GPIO_NUM;
    config.pin_d1 = Y3_GPIO_NUM;
    config.pin_d2 = Y4_GPIO_NUM;
    config.pin_d3 = Y5_GPIO_NUM;
    config.pin_d4 = Y6_GPIO_NUM;
    config.pin_d5 = Y7_GPIO_NUM;
    config.pin_d6 = Y8_GPIO_NUM;
    config.pin_d7 = Y9_GPIO_NUM;
    config.pin_xclk = XCLK_GPIO_NUM;
    config.pin_pclk = PCLK_GPIO_NUM;
    config.pin_vsync = VSYNC_GPIO_NUM;
    config.pin_href = HREF_GPIO_NUM;
    config.pin_sccb_sda = SIOD_GPIO_NUM;
    config.pin_sccb_scl = SIOC_GPIO_NUM;
    config.pin_pwdn = PWDN_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;
    config.xclk_freq_hz = 20000000;
    config.pixel_format = PIXFORMAT_RGB565;
    config.frame_size = FRAMESIZE_QQVGA; 
    config.jpeg_quality = 12; 
    config.fb_count = 1;     
    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
        return; //todo: handle errors 
    }
}

void rgb_to_hsv(uint8_t r, uint8_t g, uint8_t b, uint8_t *h, uint8_t *s, uint8_t *v) {
    uint8_t min_val, max_val, delta;

    min_val = MIN(r, MIN(g, b));
    max_val = MAX(r, MAX(g, b));
    delta = max_val - min_val;

    *v = max_val; 

    if (max_val == 0) {
        *s = 0;
        *h = 0;
        return;
    }

    *s = (uint8_t)(255 * (long)delta / max_val); 

    if (delta == 0) {
        return;
    }

    if (r == max_val) {
        *h = (uint8_t)(43 * (g - b) / delta); 
    } else if (g == max_val) {
        *h = (uint8_t)(43 * (b - r) / delta + 85); 
    } else { 
        *h = (uint8_t)(43 * (r - g) / delta + 171); 
    }
}

bool is_rock(&h,&s,&v) {
    const uint8_t h_min, h_max;
    const uint8_t s_min, s_max;
    const uint8_t v_min, v_max;

    if (h < h_min || h > h_max) {
        return false;
    }
    if (s < s_min || s > s_max) {
        return false;
    }
    if (v < v_min || v > v_max) {
        return false;
    }
    return true;
}





BlockPos Camera::getBlockPosition() {
    camera_fb_t *fb = esp_camera_fb_get();
    esp_camera_fb_return(fb);

    int index = 0;

    uint8_t output_mask_buffer[120][160];

    for (int row = 0; row < fb->height; row++) {
        for (int col = 0; col < fb->width; col++) {
            uint8_t byte1 = fb->buf[index];
            uint8_t byte2 = fb->buf[index + 1];
            uint16_t pixel = (byte2 << 8) | byte1; 
            index += 2;
            uint8_t r = ((pixel >> 0) & 0x1F) << 3; 
            uint8_t g = ((pixel >> 5) & 0x3F) << 2; 
            uint8_t b = ((pixel >> 11) & 0x1F) << 3; 
            uint8_t h, s, v;
            rgb_to_hsv(r, g, b, &h, &s, &v);

            if (is_rock(h,s,v)) {
                output_mask_buffer[row][column] = 255;
            } else {
                output_mask_buffer[row][column] = 0;
            }

        }
    }
    // smooth

}

LinePos Camera::getLinePosition() {

}
