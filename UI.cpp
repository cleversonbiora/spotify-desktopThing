#include "ui.h"
#include "lgfx_ESP32_2432S028.h"

void UI::drawTimeline()
{
  tft.fillRect( 0,  160, 320, 5, TFT_BLACK);
  tft.fillRect( 0,  160, 80, 5, TFT_WHITE);
}