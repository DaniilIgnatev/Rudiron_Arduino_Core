#include "sketch.h"
#include "tank_receiver.h"
#include "tank_transmitter.h"

#include "Adafruit_ST7735.h"

Adafruit_ST7735 tft(1, 4, 6);

#include "SPI.h"

void setup()
{
    
    pinMode(LED_BUILTIN_1, OUTPUT);
    pinMode(LED_BUILTIN_2, OUTPUT);

    for (int i = 0; i < 3; i++)
    {
        digitalWrite(LED_BUILTIN_1, true);
        digitalWrite(LED_BUILTIN_2, true);
        delay(100);
        digitalWrite(LED_BUILTIN_1, false);
        digitalWrite(LED_BUILTIN_2, false);
        delay(100);
    }

    Serial.begin(115200);
    Serial.println("РУДИРОН Бутерброд!");

    // tank_setup();

    tft.initR(INITR_MINI160x80); // initialize a ST7735S chip, mini display

    tft.fillScreen(ST77XX_RED);
    delay(100);
    tft.fillScreen(ST77XX_GREEN);
    delay(100);
    tft.fillScreen(ST77XX_BLUE);
    delay(100);
    tft.fillScreen(ST77XX_BLACK);
}

bool pressed1 = false;

bool pressed2 = false;

bool pressed3 = false;

void loop()
{
    pressed1 = digitalRead(BUTTON_BUILTIN_1);
    pressed2 = digitalRead(BUTTON_BUILTIN_2);
    pressed3 = digitalRead(BUTTON_BUILTIN_3);

    // tank_loop();
}
