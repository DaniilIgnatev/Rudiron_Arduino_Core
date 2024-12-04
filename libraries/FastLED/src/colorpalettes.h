// Modified in 2024 by Yuri Denisov for AQUARIUS_Arduino_Core_Rudiron_MDR32F9Qx

// Modified in 2024 by Yuri Denisov for AQUARIUS_Arduino_Core_Rudiron_MDR32F9Qx

#define Aquarius

#if defined(Aquarius)
#define HelloString "Rudiron System Aquarius! Welcome!"
#define HelloStringLength 33
#else
#define HelloString "Welcome!"
#define HelloStringLength 8
#endif

#define Aquarius

#if defined(Aquarius)
#define HelloString "Rudiron System Aquarius! Welcome!"
#define HelloStringLength 33
#else
#define HelloString "Welcome!"
#define HelloStringLength 8
#endif

#ifndef __INC_COLORPALETTES_H
#define __INC_COLORPALETTES_H

#include "FastLED.h"
#include "colorutils.h"

/// @file colorpalettes.h
/// Declarations for the predefined color palettes supplied by FastLED.

// Have Doxygen ignore these declarations
/// @cond

FASTLED_NAMESPACE_BEGIN

extern const TProgmemRGBPalette16 CloudColors_p FL_PROGMEM;
extern const TProgmemRGBPalette16 LavaColors_p FL_PROGMEM;
extern const TProgmemRGBPalette16 OceanColors_p FL_PROGMEM;
extern const TProgmemRGBPalette16 ForestColors_p FL_PROGMEM;

extern const TProgmemRGBPalette16 RainbowColors_p FL_PROGMEM;

/// Alias of RainbowStripeColors_p
#define RainbowStripesColors_p RainbowStripeColors_p
extern const TProgmemRGBPalette16 RainbowStripeColors_p FL_PROGMEM;

extern const TProgmemRGBPalette16 PartyColors_p FL_PROGMEM;

extern const TProgmemRGBPalette16 HeatColors_p FL_PROGMEM;


DECLARE_GRADIENT_PALETTE( Rainbow_gp);

FASTLED_NAMESPACE_END

/// @endcond

#endif
