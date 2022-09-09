#include "Arduino.h"
#include "Adafruit_ST7735.h"
#include "FreeSans9pt7b.h"

Adafruit_ST7735 tft(1, 4, 6);

void setup()
{
    tft.initR(INITR_MINI160x80); // initialize a ST7735S chip, mini display
    tft.invertDisplay(1);

    tft.fillScreen(ST7735_BLACK);
    delay(100);
    tft.fillScreen(ST7735_WHITE);
    delay(100);

    tft.fillScreen(ST7735_RED);
    delay(100);
    tft.fillScreen(ST7735_GREEN);
    delay(100);
    tft.fillScreen(ST7735_BLUE);
    delay(100);

    tft.fillScreen(ST7735_CYAN);
    delay(100);
    tft.fillScreen(ST7735_MAGENTA);
    delay(100);
    tft.fillScreen(ST7735_YELLOW);
    delay(100);

    tft.fillScreen(ST7735_ORANGE);
    delay(100);
    tft.setCursor(0, 10);
    tft.setFont(&FreeSans9pt7b);
    tft.setTextColor(ST7735_BLACK);
    tft.println("Rudiron  Buterbrod");
    delay(1000);

    tft.fillScreen(ST7735_BLACK);
    delay(100);
    tft.setCursor(0, 16);
    tft.setFont(&FreeSans9pt7b);
    tft.setTextColor(ST7735_ORANGE);
    tft.println("Rudiron  Buterbrod");
    delay(1000);
}

void loop()
{
}
