#ifndef SPOTIFYAPI_H
#define SPOTIFYAPI_H

#include <ArduinoJson.h>

class SpotifyApi
{
private:


public:
  void init();
  void getToken();
  DynamicJsonDocument getInfo();
  void play();
  void pause();
  void next();
  void prev();
};


#endif