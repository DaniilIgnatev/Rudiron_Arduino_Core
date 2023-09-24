#include "variant.h"
#include "wiring_private.h"
#include "config.h"

// Declared weak in Arduino.h to allow user redefinitions.
int atexit(void (* /*func*/)()) { return 0; }

void initVariant()
{
    Rudiron::CAN_RX_Package emptyPackage;
    emptyPackage.ID = 0;
    emptyPackage.Mask = 0;

    for (uint8_t i = 0; i < CAN_RX_BUFFER_LENGTH; i++)
    {
        _can_rx_buffer[i] = emptyPackage;
    }
}

void setupUSB()
{
}

HardwareSerial Serial(Rudiron::UART::getUART2());

HardwareSerial Serial1(Rudiron::UART::getUART1());

// Function that can be weakly referenced by serialEventRun to prevent
// pulling in this file if it's not otherwise used.
bool Serial0_available()
{
    return Serial.available();
}

bool Serial1_available()
{
    return Serial1.available();
}
