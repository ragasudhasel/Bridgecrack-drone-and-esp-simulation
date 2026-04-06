/*
  ESP32-CAM BridgeGuard Uploader (Self-Contained)
  ------------------------------
  Captures image and sends to Flask Server via HTTP POST.
*/

#include "esp_camera.h"
#include <WiFi.h>

// ===================
// PIN DEFINITIONS (AI Thinker)
// ===================
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM     0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27

#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM       5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

// ===================
// WIFI CREDENTIALS - UPDATE THESE!
// ===================
const char* ssid = "OPPO A76";
const char* password = "n8mrahuu";

// ===================
// SERVER CONFIG - UPDATE IP!
// ===================
// Check 'ipconfig' to be sure (currently detected as 10.160.65.60)
String serverName = "10.160.65.60";   
int serverPort = 5000;
String serverPath = "/upload_image";

const int timerInterval = 1000;    // Reduced to 1 second for faster streaming
unsigned long previousMillis = 0;   

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();

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
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  if(psramFound()){
    config.frame_size = FRAMESIZE_QVGA; // Reduced from VGA to QVGA (320x240) for speed
    config.jpeg_quality = 14;           // Lower quality = Smaller size = Faster upload (10-63 lower is better quality)
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_QVGA;
    config.jpeg_quality = 16;
    config.fb_count = 1;
  }

  // Camera Init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  // WiFi Connect
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= timerInterval) {
    if(WiFi.status() == WL_CONNECTED){
        sendImage();
        previousMillis = currentMillis;
    }
    else {
        Serial.println("WiFi Disconnected");
        // Try to reconnect?
        WiFi.begin(ssid, password);
        delay(1000);
    }
  }
}

String sendImage() {
  camera_fb_t * fb = NULL;
  fb = esp_camera_fb_get();
  if(!fb) {
    Serial.println("Camera capture failed");
    return "Camera Error";
  }

  Serial.println("Connecting to server: " + serverName);
  WiFiClient client;
  
  if (client.connect(serverName.c_str(), serverPort)) {
    Serial.println("Connected to server!");
    
    String head = "--RandomBoundary\r\nContent-Disposition: form-data; name=\"file\"; filename=\"esp32-cam.jpg\"\r\nContent-Type: image/jpeg\r\n\r\n";
    String tail = "\r\n--RandomBoundary--\r\n";
  
    uint32_t imageLen = fb->len;
    uint32_t extraLen = head.length() + tail.length();
    uint32_t totalLen = imageLen + extraLen;
  
    client.println("POST " + serverPath + " HTTP/1.1");
    client.println("Host: " + serverName);
    client.println("Content-Length: " + String(totalLen));
    client.println("Content-Type: multipart/form-data; boundary=RandomBoundary");
    client.println();
    client.print(head);
  
    uint8_t *fbBuf = fb->buf;
    size_t fbLen = fb->len;
    for (size_t n=0; n<fbLen; n=n+1024) {
      if (n+1024 < fbLen) {
        client.write(fbBuf, 1024);
        fbBuf += 1024;
      } else if (fbLen%1024>0) {
        size_t remainder = fbLen%1024;
        client.write(fbBuf, remainder);
      }
    }   
    client.print(tail);
    
    // Read Response
    long startTimer = millis();
    while ((startTimer + 5000) > millis()) {
      while (client.available()) {
        char c = client.read();
        Serial.print(c);
      }
    }
    Serial.println();
    client.stop();
  }
  else {
    Serial.println("Connection to server failed.");
  }
  
  esp_camera_fb_return(fb); 
  return "";
}
