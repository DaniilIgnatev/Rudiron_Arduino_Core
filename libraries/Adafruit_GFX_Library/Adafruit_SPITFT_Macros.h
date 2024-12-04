// THIS FILE INTENTIONALLY LEFT BLANK.

// Modified in 2024 by Yuri Denisov for AQUARIUS_Arduino_Core_Rudiron_MDR32F9Qx

#define Aquarius

#if defined(Aquarius)
#define HelloString "Rudiron System Aquarius! Welcome!"
#define HelloStringLength 33
#else
#define HelloString "Welcome!"
#define HelloStringLength 8
#endif

// Macros previously #defined here have been made into (mostly) inline
// functions in the Adafruit_SPITFT class. Other libraries might still
// contain code trying to #include this header file, so until everything's
// updated this file still exists (but doing nothing) to avoid trouble.
