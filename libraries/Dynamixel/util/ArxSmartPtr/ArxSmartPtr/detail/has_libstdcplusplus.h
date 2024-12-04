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

#pragma once

#ifndef ARX_TYPE_TRAITS_HAS_LIBSTDCPLUSPLUS_H
#define ARX_TYPE_TRAITS_HAS_LIBSTDCPLUSPLUS_H

#if !defined(ARX_HAVE_LIBSTDCPLUSPLUS)
    #if ARX_SYSTEM_HAS_INCLUDE(<cstdlib>)
        #include <cstdlib>
        #if defined(__GLIBCXX__) || defined(_LIBCPP_VERSION)
            // For gcc's libstdc++ and clang's libc++, assume that
            // __cplusplus tells us what the standard includes support
            #define ARX_HAVE_LIBSTDCPLUSPLUS __cplusplus
        #elif defined(__UCLIBCXX_MAJOR__)
            // For uclibc++, assume C++98 support only.
            #define ARX_HAVE_LIBSTDCPLUSPLUS 199711L
        #else
            #error "Unknown C++ library found, please report a bug against the ArxTypeTraits library about this."
        #endif
    #else
        // Assume no standard library is available at all (e.g. on AVR)
        #define ARX_HAVE_LIBSTDCPLUSPLUS 0
    #endif
#endif

#endif // ARX_TYPE_TRAITS_HAS_LIBSTDCPLUSPLUS_H
