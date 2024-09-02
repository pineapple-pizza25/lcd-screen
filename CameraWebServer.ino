#include "esp_camera.h"
#include <TFT_eSPI.h>  // Include the TFT library (use the one that matches your LCD)

// Define LCD pin mappings if not already done
#define TFT_SDA 21
#define TFT_SCL 22
#define TFT_RST -1  // Set to -1 if using a shared reset pin
#define TFT_DC 15
#define TFT_CS 13

TFT_eSPI tft = TFT_eSPI();  // Create an instance of the TFT library

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();

  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
 config.pin_d0 = 5;    // Y2
  config.pin_d1 = 18;   // Y3
  config.pin_d2 = 19;   // Y4
  config.pin_d3 = 21;   // Y5
  config.pin_d4 = 36;   // Y6
  config.pin_d5 = 39;   // Y7
  config.pin_d6 = 34;   // Y8
  config.pin_d7 = 35;   // Y9
  config.pin_xclk = 0;
  config.pin_pclk = 22;
  config.pin_vsync = 25;
  config.pin_href = 23;
  config.pin_sscb_sda = 26;
  config.pin_sscb_scl = 27;
  config.pin_pwdn = -1;
  config.pin_reset = -1;
  config.xclk_freq_hz = 20000000;
  config.frame_size = FRAMESIZE_QVGA;  // Adjust as needed
  config.pixel_format = PIXFORMAT_JPEG;  // JPEG format for LCD
  config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
  config.fb_location = CAMERA_FB_IN_PSRAM;
  config.jpeg_quality = 12;
  config.fb_count = 1;

  // Initialize the camera
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  sensor_t *s = esp_camera_sensor_get();
  // Adjust camera settings if needed
  s->set_vflip(s, 1);

  // Initialize the LCD
  tft.begin();
  tft.setRotation(3);  // Adjust orientation if needed
  tft.fillScreen(TFT_BLACK);

  Serial.println("Camera and LCD initialized.");
}

void loop() {
  // Capture a photo
  camera_fb_t *fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Camera capture failed");
    return;
  }

  // Display the image on the LCD
  if (fb->format == PIXFORMAT_JPEG) {
    tft.setSwapBytes(true);  // Swap bytes for JPEG data
    // Decode JPEG data to the LCD
    int width = fb->width;
    int height = fb->height;
    if (tft.width() >= width && tft.height() >= height) {
      tft.pushColors((uint16_t *)fb->buf, width * height);  // Push image data to the LCD
    } else {
      Serial.println("Image size exceeds LCD dimensions.");
    }
  } else {
    Serial.println("Unsupported pixel format.");
  }

  // Return the frame buffer back to the driver
  esp_camera_fb_return(fb);

  delay(1000);  // Delay between frames (adjust as needed)
}
