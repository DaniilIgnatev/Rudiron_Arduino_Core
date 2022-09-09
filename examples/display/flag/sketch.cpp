#include "Arduino.h"
#include "Adafruit_ST7735.h"
#include "FreeSans9pt7b.h"

Adafruit_ST7735 tft(1, 4, 6);

volatile uint16_t white_color = tft.color565(255, 255, 255);
uint16_t white_end = tft.height() / 3;

volatile uint16_t blue_color = tft.color565(0, 0, 255);
uint16_t blue_end = tft.height() / 3 * 2;

volatile uint16_t red_color = tft.color565(255, 0, 0);
uint16_t red_end = tft.height();

void setup()
{
    tft.initR(INITR_MINI160x80); // initialize a ST7735S chip, mini display
    tft.invertDisplay(1);

    tft.fillRect(0, 0, tft.width(), white_end, white_color);
    tft.fillRect(0, tft.height() / 3, tft.width(), blue_end, blue_color);
    tft.fillRect(0, tft.height() / 3 * 2, tft.width(), red_end, red_color);
}

void loop()
{
    white_end %= tft.height();
    tft.drawLine(0, white_end, tft.width(), white_end, white_color);
    white_end++;

    blue_end %= tft.height();
    tft.drawLine(0, blue_end, tft.width(), blue_end, blue_color);
    blue_end++;

    red_end %= tft.height();
    tft.drawLine(0, red_end, tft.width(), red_end, red_color);
    red_end++;

    delay(10);
}
