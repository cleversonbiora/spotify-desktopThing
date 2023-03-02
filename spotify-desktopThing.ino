#pragma GCC optimize ("Ofast")

#define LGFX_USE_V1
#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h> 
#include <LovyanGFX.hpp>
#include "lgfx_ESP32_2432S028.h"
#include "spotify_image.c"
#include "spotifyApi.h"
#include <WiFi.h>
#include <ArduinoJson.h>

const char* ssid = "Todi's Home";
const char* password = "0052946C2";

SpotifyApi spotifyApi;
int refresh_speed = 25; // 25msec
int refresh_api = 0; // 25msec
int durationTotal = 0; 
int currentTime = 0;
bool isPlaying = false;
String musicId = "";

//========================================================================
void setup(void) {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  spotifyApi.init();
  initUI();
  splashScreen();
  drawPlayer();
  xTaskCreatePinnedToCore(TaskRunningOnAppCore, 
                    "TaskOnApp", 
                    2048, 
                    NULL, 
                    4, 
                    NULL,
                      APP_CPU_NUM);

  xTaskCreatePinnedToCore(TaskRunningOnProtocolCore, 
                    "TaskOnPro", 
                    2048, 
                    NULL, 
                    8, 
                    NULL, 
                    PRO_CPU_NUM);
}
//========================================================================
void loop() {
  Serial.print("Loop");
  //updatePlayer();
  delay(500);  // Speed Adjust
}

void initUI(){
  tft.init();
  tft.setRotation(1);
}

void splashScreen(){
  tft.fillScreen(TFT_BLACK);
  tft.pushImage(0, 0,320,240,(uint16_t*)spotify_image);
  delay(2000);
}

void drawTimeline()
{
  int pos = (320 * currentTime) / durationTotal;
  tft.fillRect( 0,  160, 320, 5, TFT_BLACK);
  tft.fillRect( 0,  160, pos, 5, TFT_WHITE);

}

void drawPlayer()
{
  tft.fillScreen(TFT_DARKGRAY);
  tft.fillRect( 0,  160, 320, 80, TFT_BLACK);

  tft.setTextSize(3);
  tft.setCursor(20,190); 
  tft.println("Prev");
  
  tft.setTextSize(3);
  tft.setCursor(120,190); 
  tft.println("Play");
  
  tft.setTextSize(3);
  tft.setCursor(220,190); 
  tft.println("Next");
}

void updatePlayer()
{
  if(WiFi.status()== WL_CONNECTED){
    DynamicJsonDocument doc = spotifyApi.getInfo();
    //drawPlayer();
    if(doc.containsKey("item")){
      JsonObject item = doc["item"];

      String id = item["id"];
      if(musicId != id){
        musicId = id;
        drawPlayer();
      }

      String name = item["name"];
      tft.setTextColor(TFT_WHITE, TFT_DARKGRAY);
      tft.setTextSize(3);
      tft.setCursor(20,20);
      tft.println(name);
      
      JsonArray artists = item["artists"];
      tft.setTextSize(2);
      tft.setCursor(20,90); 
      tft.println(formatArtist(artists));

      int duration = item["duration_ms"];
      durationTotal = duration;
      tft.setTextSize(1.5);
      tft.setCursor(100,140); 
      tft.println(formatTimer(duration));

    }
    if(doc.containsKey("progress_ms")){
      int currently = doc["progress_ms"];
      currentTime = currently;
      tft.setCursor(20,140); 
      tft.println(formatTimer(currently));
    }
    if(doc.containsKey("is_playing")){
      bool is_playing = doc["is_playing"];
      isPlaying = is_playing;
    }
    drawTimeline();
  }
}

bool keyTouch(){
  uint16_t tX, tY;
T:bool touched = tft.getTouch( &tX, &tY);
  if (touched) {
    Serial.printf("%d,%d\n", tX, tY);
    if(WiFi.status()== WL_CONNECTED){
      if(isPlaying){
        spotifyApi.pause();
      }else{
        spotifyApi.play();
      }
      //spotifyApi.next();
    }
  }

  return false;
}

String formatTimer(int millis){
  unsigned long Now = millis/1000;
  // we use modulo to split the parts
  int Seconds = Now%60;
  int Minutes = (Now/60)%60;
  int Hours = (Now/3600)%24;
  if(Hours > 0){
    String time = String(Hours) + ":" + String(Minutes) + ":" + String(Seconds);
    return time;
  }else{
    String time = String(Minutes) + ":" + String(Seconds);
    return time;
  }  
}

String formatArtist(JsonArray artists){
  int count = 1;
  String names = "";
  for (JsonVariant value : artists) {
    if(count == 1){
      String name = value["name"];
      names += name; 
    }else{
      String name = value["name"];
      names += ", " + name; 
    }
    count++;
  }
  return names;
}

//-------------------------------------------

void TaskRunningOnAppCore(void *arg) {
    while(1) {
      Serial.print("updatePlayer");
      updatePlayer();
      vTaskDelay(1000); 
    }
}
 
void TaskRunningOnProtocolCore(void *arg) {
    while(1) {
      Serial.print("keyTouch");
      keyTouch();
      vTaskDelay(25);
    }
}

