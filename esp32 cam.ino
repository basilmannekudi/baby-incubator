#include <Arduino.h>
#include <base64.h>

String FIREBASE_HOST = "YOUR-HOST-CRED";
String FIREBASE_AUTH = "YOUR-AUTH-CRED";
#include "FirebaseESP32.h"
FirebaseData firebaseData;
#include <WiFi.h>
#include <WiFiManager.h>
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include "esp_camera.h"
int counter=0;
String urlencode(String str);
// WARNING!!! Make sure that you have either selected ESP32 Wrover Module,

//            or another board which has PSRAM enabled

// CAMERA_MODEL_AI_THINKER
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
#define led 33
String Photo2Base64()
{

  camera_fb_t *fb = NULL;

  fb = esp_camera_fb_get();

  if (!fb)
  {

    Serial.println("Camera capture failed");

    return "";
  }

  String imageFile = "data:image/jpeg;base64,";
String encrypt = base64::encode(fb->buf, fb->len);
  


  esp_camera_fb_return(fb);

//  return imageFile;
//Serial.println(encrypt);
return encrypt;
}

// https://github.com/zenmanenergy/ESP8266-Arduino-Examples/

String urlencode(String str)

{

  String encodedString = "";

  char c;

  char code0;

  char code1;

  char code2;

  for (int i = 0; i < str.length(); i++)
  {

    c = str.charAt(i);

    if (c == ' ')
    {

      encodedString += '+';
    }
    else if (isalnum(c))
    {

      encodedString += c;
    }
    else
    {

      code1 = (c & 0xf) + '0';

      if ((c & 0xf) > 9)
      {

        code1 = (c & 0xf) - 10 + 'A';
      }

      c = (c >> 4) & 0xf;

      code0 = c + '0';

      if (c > 9)
      {

        code0 = c - 10 + 'A';
      }

      code2 = '\0';

      encodedString += '%';

      encodedString += code0;

      encodedString += code1;

       encodedString+=code2;
    }

    yield();
  }

  return encodedString;
}

void setupCamera()
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
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  // init with high specs to pre-allocate larger buffers
  if (psramFound())
  {

    config.frame_size = FRAMESIZE_VGA;

    config.jpeg_quality = 12; // 0-63 lower number means higher quality

    config.fb_count = 1;
  }
  else
  {

    config.frame_size = FRAMESIZE_VGA;

    config.jpeg_quality = 10; // 0-63 lower number means higher quality

    config.fb_count = 1;
  }
  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK)
  {
    Serial.printf("Camera init failed with error 0x%x", err);
    delay(1000);
    ESP.restart();
  }
  sensor_t *s = esp_camera_sensor_get();
  s->set_framesize(s, FRAMESIZE_QQVGA); // VGA|CIF|QVGA|HQVGA|QQVGA   ( UXGA? SXGA? XGA? SVGA? )
}
void  startStreaming()
{

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
  Firebase.setMaxRetry(firebaseData, 3);
  Firebase.setMaxErrorQueue(firebaseData, 30);
  Firebase.enableClassicRequest(firebaseData, true);
  String jsonData = "{\"photo\":\"" + Photo2Base64() + "\"}";
  String photoPath = "/live";
  FirebaseJson json2;
  json2.set("photo", Photo2Base64());
  if (Firebase.setJSON(firebaseData, photoPath, json2))
  {
   
  //  Serial.println("Image Uploaded");
  }
  else
  {

    Serial.println(firebaseData.errorReason());
  }
}
void initWiFi() {
WiFiManager wm;
bool res;
res = wm.autoConnect("Live Config");

// anonymous ap
if(!res) {
    digitalWrite(led,HIGH);
    } 
    else {
        //if you get here you have connected to the WiFi    
       digitalWrite(led,LOW);

    }
}
void setup()
{
  pinMode(led,OUTPUT);
  digitalWrite(led,HIGH);
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
  Serial.begin(115200);
  
  Serial.setDebugOutput(true);

   initWiFi();
  setupCamera();
  // drop down frame size for higher initial frame rate
}

void loop()
{
  

startStreaming();
  delay(100);
}
