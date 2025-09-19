#include "camera.h"
#include "esp_camera.h"
#include "hal/can_data.h"
#include <math.h>
#include <algorithm>

#define PWDN_GPIO_NUM 32
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM 0
#define SIOD_GPIO_NUM 26
#define SIOC_GPIO_NUM 27
#define Y9_GPIO_NUM 35
#define Y8_GPIO_NUM 34
#define Y7_GPIO_NUM 39
#define Y6_GPIO_NUM 36
#define Y5_GPIO_NUM 21
#define Y4_GPIO_NUM 19
#define Y3_GPIO_NUM 18
#define Y2_GPIO_NUM 5
#define VSYNC_GPIO_NUM 25
#define HREF_GPIO_NUM 23
#define PCLK_GPIO_NUM 22

typedef struct
{
    int x_min, y_min, x_max, y_max;
    long area;
    long sum_x, sum_y; // For centroid calculation
} BlobInfo;

void Camera::setup()
{
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
    if (err != ESP_OK)
    {
        return; // todo: handle errors
    }
}

void rgb_to_hsv(uint8_t r, uint8_t g, uint8_t b, uint16_t *h, uint16_t *s, uint16_t *v)
{
    uint8_t min_val, max_val, delta;

    min_val = MIN(r, MIN(g, b));
    max_val = MAX(r, MAX(g, b));
    delta = max_val - min_val;

    *v = max_val;

    if (max_val == 0)
    {
        *s = 0;
        *h = 0;
        return;
    }

    *s = (uint16_t)(255 * (long)delta / max_val);

    if (delta == 0)
    {
        return;
    }

    if (r == max_val)
    {
        *h = (uint16_t)(43 * (g - b) / delta);
    }
    else if (g == max_val)
    {
        *h = (uint16_t)(43 * (b - r) / delta + 85);
    }
    else
    {
        *h = (uint16_t)(43 * (r - g) / delta + 171);
    }
}

bool is_scissor(uint16_t &h,uint16_t  &s, uint16_t &v)
{
    const uint16_t h_min = 220;
    const uint16_t h_max = 251;
    const uint16_t s_min = 0;
    const uint16_t s_max = 500;
    const uint16_t v_min = 0;
    const uint16_t v_max = 500;

    if (h < h_min || h > h_max)
    {
        return false;
    }
    if (s < s_min || s > s_max)
    {
        return false;
    }
    if (v < v_min || v > v_max)
    {
        return false;
    }
    return true;
}

bool is_paper(uint16_t &h, uint16_t &s, uint16_t &v)
{
    const uint16_t h_min = 39;
    const uint16_t h_max = 59;
    const uint16_t s_min = 0;
    const uint16_t s_max = 500; 
    const uint16_t v_min = 0;
    const uint16_t v_max = 500;

    if (h < h_min || h > h_max)
    {
        return false;
    }
    if (s < s_min || s > s_max)
    {
        return false;
    }
    if (v < v_min || v > v_max)
    {
        return false;
    }
    return true;
}

bool is_rock(uint16_t &h, uint16_t &s, uint16_t &v)
{
    const uint16_t h_min = 339;
    const uint16_t h_max = 359;
    const uint16_t s_min = 0 ;
    const uint16_t s_max = 1000 ;
    const uint16_t v_min = 0;
    const uint16_t v_max = 1000;

    if (h < h_min || h > h_max)
    {
        return false;
    }
    if (s < s_min || s > s_max)
    {
        return false;
    }
    if (v < v_min || v > v_max)
    {
        return false;
    }
    return true;
}

typedef struct
{
    int px, py;
} StackNode;

BlockPos Camera::getBlockPosition(int blockType)
{
    camera_fb_t *fb = esp_camera_fb_get();
    esp_camera_fb_return(fb);

    int index = 0;

    uint8_t output_mask_buffer[120][160];

    for (int row = 0; row < fb->height; row++)
    {
        for (int col = 0; col < fb->width; col++)
        {
            uint8_t byte1 = fb->buf[index];
            uint8_t byte2 = fb->buf[index + 1];
            uint16_t pixel = (byte2 << 8) | byte1;
            index += 2;
            uint8_t r = ((pixel >> 0) & 0x1F) << 3;
            uint8_t g = ((pixel >> 5) & 0x3F) << 2;
            uint8_t b = ((pixel >> 11) & 0x1F) << 3;
            uint16_t h, s, v;
            rgb_to_hsv(r, g, b, &h, &s, &v);
            if (blockType == 0)
            {
                if (is_rock(h, s, v))
                {
                    output_mask_buffer[row][col] = 255;
                }
                else
                {
                    output_mask_buffer[row][col] = 0;
                }
            }
            else if (blockType == 1)
            {
                if (is_paper(h, s, v))
                {
                    output_mask_buffer[row][column] = 255;
                }
                else
                {
                    output_mask_buffer[row][column] = 0;
                }
            }
            else if (blockType == 2)
            {
                if (is_scissor(h, s, v))
                {
                    output_mask_buffer[row][column] = 255;
                }
                else
                {
                    output_mask_buffer[row][column] = 0;
                }
            }
        }
    }
    int width = fd->width;
    int height = fd->height;

    // Input: src_mask (uint8_t), Output: dst_mask (uint8_t)
    // Create a temporary buffer for results to avoid modifying while reading
    uint8_t *temp_buffer = (uint8_t *)malloc(width * height * sizeof(uint8_t));
    if (!temp_buffer)
        return; // Handle allocation error

    for (int y = 1; y < height - 1; y++)
    { // Avoid borders for 3x3 kernel
        for (int x = 1; x < width - 1; x++)
        {
            // Assume pixel is white unless any neighbor is black
            uint8_t is_eroded = 255;
            // Check 3x3 neighborhood
            for (int ky = -1; ky <= 1; ky++)
            {
                for (int kx = -1; kx <= 1; kx++)
                {
                    if (output_mask_buffer[(y + ky)][(x + kx)] == 0)
                    {
                        is_eroded = 0;
                        break;
                    }
                }
                if (is_eroded == 0)
                    break;
            }
            temp_buffer[y * width + x] = is_eroded;
        }
    }
    // Input: src_mask (uint8_t), Output: dst_mask (uint8_t)
    // Similar temp_buffer logic
    uint8_t *temp_buffer2 = (uint8_t *)malloc(width * height * sizeof(uint8_t));
    if (!temp_buffer2)
        return;

    for (int y = 1; y < height - 1; y++)
    {
        for (int x = 1; x < width - 1; x++)
        {
            // Assume pixel is black unless any neighbor is white
            uint8_t is_dilated = 0;
            for (int ky = -1; ky <= 1; ky++)
            {
                for (int kx = -1; kx <= 1; kx++)
                {
                    if (temp_buffer[(y + ky) * width + (x + kx)] == 255)
                    {
                        is_dilated = 255;
                        break;
                    }
                }
                if (is_dilated == 255)
                    break;
            }
            temp_buffer2[y * width + x] = is_dilated;
        }
    }

    free(temp_buffer);

    // Input: mask_buffer (uint8_t), Output: Store detected blob properties
    // This is a simplified approach, often called "Find Blobs" or "Connected Components"

    // Create a visited_map to avoid re-processing pixels (same size as mask)
    uint8_t *visited_map = (uint8_t *)calloc(width * height, sizeof(uint8_t));
    if (!visited_map)
        return;

    BlobInfo current_blob;
    // List to store multiple blobs (you might need a dynamic array or fixed-size array)
    // For simplicity, let's just find the largest blob for now.
    BlobInfo largest_blob = {0, 0, 0, 0, 0, 0, 0};

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            if (mask_buffer[y * width + x] == 255 && visited_map[y * width + x] == 0)
            {
                // Found a new white pixel, start a new blob search (e.g., using a stack for DFS or queue for BFS)
                // Initialize current_blob
                current_blob.x_min = x;
                current_blob.y_min = y;
                current_blob.x_max = x;
                current_blob.y_max = y;
                current_blob.area = 0;
                current_blob.sum_x = 0;
                current_blob.sum_y = 0;

// Use a simple stack (array for embedded) for DFS
#define MAX_STACK_SIZE (width * height / 4) // Adjust as needed
                StackNode pixel_stack[MAX_STACK_SIZE];
                int stack_ptr = 0;

                pixel_stack[stack_ptr++] = (StackNode){x, y};
                visited_map[y * width + x] = 1;

                while (stack_ptr > 0)
                {
                    StackNode current = pixel_stack[--stack_ptr];

                    // Update blob properties
                    current_blob.area++;
                    current_blob.sum_x += current.px;
                    current_blob.sum_y += current.py;
                    current_blob.x_min = MIN(current_blob.x_min, current.px);
                    current_blob.y_min = MIN(current_blob.y_min, current.py);
                    current_blob.x_max = MAX(current_blob.x_max, current.px);
                    current_blob.y_max = MAX(current_blob.y_max, current.py);

                    // Explore 8-connected neighbors
                    for (int dy = -1; dy <= 1; dy++)
                    {
                        for (int dx = -1; dx <= 1; dx++)
                        {
                            int nx = current.px + dx;
                            int ny = current.py + dy;

                            if (nx >= 0 && nx < width && ny >= 0 && ny < height &&
                                mask_buffer[ny * width + nx] == 255 && visited_map[ny * width + nx] == 0)
                            {
                                visited_map[ny * width + nx] = 1;
                                if (stack_ptr < MAX_STACK_SIZE)
                                { // Check for stack overflow
                                    pixel_stack[stack_ptr++] = (StackNode){nx, ny};
                                }
                                else
                                {
                                    // Handle stack overflow, e.g., print error and break
                                    break;
                                }
                            }
                        }
                    }
                }

                // After finding a blob, filter it
                if (current_blob.area > MIN_CUBE_PIXEL_AREA_THRESHOLD &&
                    current_blob.area < MAX_CUBE_PIXEL_AREA_THRESHOLD)
                { // Filter by area
                    // Calculate aspect ratio (bounding box)
                    int bbox_width = current_blob.x_max - current_blob.x_min + 1;
                    int bbox_height = current_blob.y_max - current_blob.y_min + 1;
                    // You can add a check for aspect ratio being close to 1.0 here

                    // Calculate centroid
                    int centroid_x = current_blob.sum_x / current_blob.area;
                    int centroid_y = current_blob.sum_y / current_blob.area;

                    // Store or process this valid cube
                    // For example, if it's the target color, make it the "active" target
                }
            }
        }
    }
    free(visited_map);

    free(temp_buffer2);
}

LinePos Camera::getLinePosition()
{
}
