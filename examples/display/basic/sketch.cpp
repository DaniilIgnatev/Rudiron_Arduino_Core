#include "sketch.h"
#include "Adafruit_ST7735.h"
#include "FreeMono9pt7b.h"

Adafruit_ST7735 tft(1, 4, 6);

void setup()
{
    tft.initR(INITR_MINI160x80); // initialize a ST7735S chip, mini display

    tft.fillScreen(ST77XX_RED);
    delay(100);
    tft.fillScreen(ST77XX_GREEN);
    delay(100);
    tft.fillScreen(ST77XX_BLUE);
    delay(100);
    tft.fillScreen(ST77XX_BLACK);

    tft.setFont(&FreeMono9pt7b);
    tft.println(" Rudiron Buterbrod!");
}

void loop()
{

}
