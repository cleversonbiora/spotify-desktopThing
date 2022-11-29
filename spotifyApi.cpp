#include "spotifyApi.h"
#include <HTTPClient.h>

void SpotifyApi::next()
{
      HTTPClient http;

      String serverPath = "https://api.spotify.com/v1/me/player/next";
      
      http.begin(serverPath.c_str());
      
      http.addHeader("Authorization", "Bearer BQC7Zs4UBRjRP4Vvyz5zyHjG0mBl-qFYXOV9DtGquOUTPwqzV_To27BFgt-FY1PEqkWJWP6wiZu3kBVJE_Q2IfIgCg2rnqXgq0U6Uq1apHJ96wXOSC12n-M6avUKoU-L9RNuFtR_Ep7Ra5DUGevtKjuWttWoxtv42oVHnAG03KF_yMpwiwBpD9PLiVWTVIvtvERY");
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