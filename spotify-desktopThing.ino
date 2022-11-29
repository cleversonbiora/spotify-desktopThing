#pragma GCC optimize ("Ofast")
#define LGFX_USE_V1
#include <LovyanGFX.hpp>
#include "lgfx_ESP32_2432S028.h"
#include "spotify_image.c"
#include "spotifyApi.h"
#include "ui.h"
#include <WiFi.h>

const char* ssid = "Todi's Home";
const char* password = "0052946C2";

SpotifyApi spotifyApi;
UI ui;
struct Point {int X, Y;};
Point pos;
int rot, fall_cnt = 0;
bool started  = false;
boolean ST_ROT=false, b_LEFT=false, b_RIGHT=false, b_DOWN=false;
boolean b_PAUSE=false;
int game_speed = 25; // 25msec

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
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  //----------------------------------------------------------------------
  tft.pushImage(0, 0,320,240,(uint16_t*)spotify_image); // background
  delay(2000);  // Speed Adjust
  tft.fillScreen(TFT_LIGHTGREY);
  tft.fillRect( 0,  160, 320, 80, TFT_DARKGREY);
  ui.drawTimeline();  
}
//========================================================================
void loop() {
  Point next_pos;
  int next_rot = rot;
  KeyPadLoop();
  tft.setCursor(80,64); tft.println("Hello World");
  delay(game_speed);  // Speed Adjust
}

//========================================================================
void ClearKeys(){ST_ROT=false;b_LEFT=false;b_RIGHT=false;b_DOWN=false;}
//========================================================================
bool KeyPadLoop(){
  uint16_t tX, tY;
T:bool touched = tft.getTouch( &tX, &tY);
  if (touched) {
    Serial.printf("%d,%d\n", tX, tY);

    if(WiFi.status()== WL_CONNECTED){
      spotifyApi.next();
    }
    // DOWN --------------------------------------------------------------
    if(tX<120 && tY<120){ClearKeys();b_DOWN =true;delay( 25);return true;}
    // LEFT --------------------------------------------------------------
    if(tX<120 && tY>120){ClearKeys();b_LEFT =true;delay(200);return true;}
    // RIGHT -------------------------------------------------------------
    if(tX>210 && tY>120){ClearKeys();b_RIGHT=true;delay(200);return true;}
    // START / ROTATE ----------------------------------------------------
    if(tX>210 && tY<120){ClearKeys();ST_ROT =true;delay(250);return true;}
    // PAUSE -------------------------------------------------------------
    if(tX>110 && tX<210){
      if (b_PAUSE){b_PAUSE=false;delay(250);return true;}
      ClearKeys(); b_PAUSE=true ;delay(250);return true;
    }
    //--------------------------------------------------------------------
  }
  if ( b_PAUSE ){ 
    //tft.setCursor(226-80,64); tft.println("PAUSE");
    delay(250); goto T;
  }
  return false;
}
