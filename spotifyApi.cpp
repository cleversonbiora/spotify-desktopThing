#include "spotifyApi.h"
#include <HTTPClient.h>
#include <ArduinoJson.h>

String api_url  = "https://api.spotify.com/v1";
String auth_url  = "https://accounts.spotify.com/api/token";
String access_token  = "";
String auth_basic  = "MmYwYWY1MTUwMGFhNDJkZWExMTAwNjNkNmIxNGI2OWQ6NTZmZmQxZTk4ZGQ5NDVkMWEwYjE2NmYxZjY0YzEyYjk=";
String refresh_token  = "AQA2K3pCKqeh4BnFM6a_bsUhrsJg33WBjMl3hhYWJz557MOI9hWCr5nHqK20BYYDaICTb7iMYyAmhguM2ddiqFZljqiOItxG66MCT6jMbC1nFgmmk1Fg5H0IZgyW3V2eI7E";

void SpotifyApi::init(){
  getToken();
}
void SpotifyApi::getToken(){
    HTTPClient http;

    String serverPath = auth_url;
    
    http.begin(serverPath.c_str());
    http.addHeader("Authorization", "Basic " + auth_basic);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    String httpRequestData = "grant_type=refresh_token&refresh_token="+refresh_token;           

    int httpResponseCode = http.POST(httpRequestData);
    
    if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      String payload = http.getString();
      DynamicJsonDocument doc(1024);
      deserializeJson(doc, payload);
      String token = doc["access_token"]; 
      access_token = token;
      Serial.println(payload);
    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    // Free resources
    http.end();

}

DynamicJsonDocument SpotifyApi::getInfo(){
    //getToken();
    HTTPClient http;

    String serverPath = api_url + "/me/player";
    
    http.begin(serverPath.c_str());
    
    http.addHeader("Authorization", "Bearer " + access_token);
    http.addHeader("Content-Type", "application/json");       

    int httpResponseCode = http.GET();
    
    DynamicJsonDocument doc(64000);
    if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      String payload = http.getString();
      deserializeJson(doc, payload);
      Serial.println(payload);
    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    // Free resources
    http.end();
    return doc;

}

void SpotifyApi::next()
{
    //getToken();
    HTTPClient http;

    String serverPath = api_url + "/me/player/next";
    
    http.begin(serverPath.c_str());
    
    http.addHeader("Authorization", "Bearer " + access_token);
    http.addHeader("Content-Type", "application/json");
    int httpResponseCode = http.POST("{}");
    
    if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      String payload = http.getString();
      Serial.println(payload);
    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    // Free resources
    http.end();
}

void SpotifyApi::prev()
{
    //getToken();
    HTTPClient http;

    String serverPath = api_url + "/me/player/previous";
    
    http.begin(serverPath.c_str());
    
    http.addHeader("Authorization", "Bearer " + access_token);
    http.addHeader("Content-Type", "application/json");
    int httpResponseCode = http.POST("{}");
    
    if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      String payload = http.getString();
      Serial.println(payload);
    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    // Free resources
    http.end();
}

void SpotifyApi::pause()
{
    //getToken();
    HTTPClient http;

    String serverPath = api_url + "/me/player/pause";
    
    http.begin(serverPath.c_str());
    
    http.addHeader("Authorization", "Bearer " + access_token);
    http.addHeader("Content-Type", "application/json");
    int httpResponseCode = http.PUT("{}");
    
    if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      String payload = http.getString();
      Serial.println(payload);
    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    // Free resources
    http.end();
}

void SpotifyApi::play()
{
    //getToken();
    HTTPClient http;

    String serverPath = api_url + "/me/player/play";
    
    http.begin(serverPath.c_str());
    
    http.addHeader("Authorization", "Bearer " + access_token);
    http.addHeader("Content-Type", "application/json");
    int httpResponseCode = http.PUT("{}");
    
    if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      String payload = http.getString();
      Serial.println(payload);
    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    // Free resources
    http.end();
}