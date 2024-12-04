/******************************************************************************
 * The MIT License
 *
 * Copyright (c) 2010, LeafLabs, LLC.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *****************************************************************************/

// Modified 2022 by Daniil Ignatev for Arduino_Core_Rudiron_MDR32F9Qx

// Modified in 2024 by Yuri Denisov for AQUARIUS_Arduino_Core_Rudiron_MDR32F9Qx

#define Aquarius

#if defined(Aquarius)
#define HelloString "Rudiron System Aquarius! Welcome!"
#define HelloStringLength 33
#else
#define HelloString "Welcome!"
#define HelloStringLength 8
#endif

#include "math.h"
#include "Servo.h"
#include "rudiron/timer.h"
#include "rudiron/gpio.h"

// 20 millisecond period config
#define MAX_OVERFLOW ((1 << 16) - 1)
#define TAU_MSEC 20
#define TAU_USEC (TAU_MSEC * 1000)
#define SERVO_OVERFLOW 1000

// Unit conversions
#define US_TO_COMPARE(us) ((uint16_t)map((us), 0, TAU_USEC, 0, SERVO_OVERFLOW))
#define COMPARE_TO_US(c) ((uint32_t)map((c), 0, SERVO_OVERFLOW, 0, TAU_USEC))
#define ANGLE_TO_US(a) ((uint16_t)(map((a), this->minAngle, this->maxAngle, \
                                       this->minPW, this->maxPW)))
#define US_TO_ANGLE(us) ((int16_t)(map((us), this->minPW, this->maxPW, \
                                       this->minAngle, this->maxAngle)))

Servo::Servo()
{
    this->resetFields();
}

bool Servo::attach(uint8_t pin, uint16_t minPW, uint16_t maxPW, int16_t minAngle, int16_t maxAngle)
{
    if (this->attached())
    {
        this->detach();
    }

    this->pin = pin;
    this->portPin = Rudiron::GPIO::get_rudiron_gpio(pin);
    this->minPW = minPW;
    this->maxPW = maxPW;
    this->minAngle = minAngle;
    this->maxAngle = maxAngle;

    if (Rudiron::Timer::hasTimer_for_pinName(portPin))
    {
        Rudiron::Timer &timer = Rudiron::Timer::getTimer_by_pinName(portPin);
        const uint16_t frequency = 1 * 1000000 / TAU_USEC;
        timer.setup(frequency);
        return true;
    }
    else
    {
        return false;
    }
}

bool Servo::detach()
{
    if (!this->attached())
    {
        return false;
    }

    this->resetFields();

    if (Rudiron::Timer::hasTimer_for_pinName(portPin))
    {
        Rudiron::Timer::getTimer_by_pinName(portPin).PWM_stop(portPin);
    }

    return true;
}

void Servo::write(int degrees)
{
    degrees = constrain(degrees, this->minAngle, this->maxAngle);
    this->writeMicroseconds(ANGLE_TO_US(degrees));
}

int Servo::read() const
{
    int us = this->readMicroseconds();
    int a = US_TO_ANGLE(us);
    // map() round-trips in a weird way we mostly correct for here;
    // the round-trip is still sometimes off-by-one for write(1) and
    // write(179).
    return a == this->minAngle || a == this->maxAngle ? a : a + 1;
}

void Servo::writeMicroseconds(uint16_t pulseWidth)
{
    if (!this->attached())
    {
        // ASSERT(0);
        return;
    }

    this->currentUS = constrain(pulseWidth, this->minPW, this->maxPW);
    int pwmValue = US_TO_COMPARE(currentUS);
    Rudiron::Timer::getTimer_by_pinName(portPin).PWM_start(portPin, pwmValue, SERVO_OVERFLOW);
}

uint16_t Servo::readMicroseconds() const
{
    if (!this->attached())
    {
        // ASSERT(0);
        return 0;
    }

    return this->currentUS;
}

void Servo::resetFields(void)
{
    this->pin = NOT_ATTACHED;
    this->minAngle = MIN_ANGLE;
    this->maxAngle = MAX_ANGLE;
    this->minPW = MIN_PULSE_WIDTH;
    this->maxPW = MAX_PULSE_WIDTH;
}
