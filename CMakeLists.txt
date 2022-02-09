cmake_minimum_required(VERSION 3.15.3)
# Optional: print out extra messages to see what is going on. Comment it to have less verbose messages
set(CMAKE_VERBOSE_MAKEFILE ON)

# Path to toolchain file. This one has to be before 'project()' below
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/arm-none-eabi-gcc.cmake")

# Setup project
project(Arduino_Core_Rudiron)
SET(CMAKE_BUILD_TYPE "Debug")

set(core M3)
set(cpu_name "MDR32F9Qx")

# Optional: issue a message to be sure it uses the correct toolchain file.
message(STATUS "CMAKE_TOOLCHAIN_FILE is: ${CMAKE_TOOLCHAIN_FILE}")

#Sources
set(core_path "${CMAKE_SOURCE_DIR}/core")
set(core_avr_path "${core_path}/avr")
set(core_rudiron_path "${core_path}/rudiron")
file(GLOB_RECURSE core_sources
        ${core_path}/*
        ${core_avr_path}/*
        ${core_rudiron_path}/*
        )

set(libraries_path "${CMAKE_SOURCE_DIR}/libraries")
file(GLOB_RECURSE libraries_sources
        ${libraries_path}/*
        )

set(sketch_path "${CMAKE_SOURCE_DIR}/sketch")
file(GLOB_RECURSE sketch_sources
        ${sketch_path}/*
        )

set(cmsis_path "${CMAKE_SOURCE_DIR}/system/Drivers/CMSIS/Device/MILANDR/${cpu_name}")

set(cmsis_include_path "${cmsis_path}/Include")
file(GLOB_RECURSE cmsis_include_sources
        ${cmsis_include_path}/*
        )

set(cmsis_source_path "${cmsis_path}/Source")
set(cmsis_source_templates_path "${cmsis_source_path}/Templates")
file(GLOB_RECURSE cmsis_source_sources
        ${cmsis_source_path}/*
        ${cmsis_source_templates_path}/*
        )

set(cmsis_startup_path "${cmsis_source_path}/Templates/gcc")
file(GLOB_RECURSE cmsis_startup_sources
        ${cmsis_startup_path}/*.S
        )


set(std_periph_driver_path "${CMAKE_SOURCE_DIR}/system/Drivers/${cpu_name}_StdPeriph_Driver")

set(std_periph_driver_inc_path "${std_periph_driver_path}/inc")
set(std_periph_driver_inc_usb_path "${std_periph_driver_inc_path}/USB_Library")
set(std_periph_driver_src_path "${std_periph_driver_path}/src")
set(std_periph_driver_src_usb_path "${std_periph_driver_src_path}/USB_Library")
file(GLOB_RECURSE std_periph_driver_sources
        ${std_periph_driver_inc_path}/*
        ${std_periph_driver_inc_usb_path}/*
        ${std_periph_driver_src_path}/*
        ${std_periph_driver_src_usb_path}/*
        )


set(system_path "${CMAKE_SOURCE_DIR}/system/${cpu_name}")
file(GLOB_RECURSE system_sources
        ${system_path}/*
        )

set(variants_path "${CMAKE_SOURCE_DIR}/variants/${cpu_name}")
file(GLOB_RECURSE variants_sources
        ${variants_path}/*
        )

#Executable
set(EXECUTABLE "${PROJECT_NAME}.elf")
#Linker file
set(LINKER_FILE "${cmsis_startup_path}/${cpu_name}.ld")

enable_language(C ASM)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)


set(SRC_LIST
        ${core_sources}
        ${libraries_sources}
        ${sketch_sources}
        ${cmsis_include_sources}
        ${cmsis_source_sources}
        ${cmsis_startup_sources}
        ${std_periph_driver_sources}
        ${system_sources}
        ${variants_sources}
        )

add_executable(${EXECUTABLE} ${SRC_LIST})

#Includes
target_include_directories(${EXECUTABLE} PRIVATE
        ${core_path}
        ${libraries_path}
        ${sketch_path}
        ${cmsis_include_path}
        ${cmsis_source_path}
        ${cmsis_source_templates_path}
        ${cmsis_startup_path}
        ${std_periph_driver_inc_path}
        ${std_periph_driver_inc_usb_path}
        ${std_periph_driver_src_path}
        ${std_periph_driver_src_usb_path}
        ${system_path}
        ${variants_path}
        )

# List of compiler defines, prefix with -D compiler option
target_compile_definitions(${EXECUTABLE} PRIVATE
        RUDIRON_BUTERBROD_rev5
        )


# Compiler options
target_compile_options(${EXECUTABLE} PRIVATE
    "-mcpu=cortex-${core}"
    "-mthumb"
    "-ffunction-sections"
    "-fdata-sections"
    "-pipe"
    "-DUSE_${cpu_name}"
    "-D__STARTUP_CLEAR_BSS"
    "-D_start=main"
    )

# Linker options
target_link_options(${EXECUTABLE} PRIVATE
    "-T${LINKER_FILE}"
    "-mcpu=cortex-${core}"
    "-mthumb"
    "-Wl,--gc-sections"
    "-specs=nano.specs"
    "-ffreestanding"
    "-nostartfiles"
    )


# Optional: Create hex, bin and S-Record files after the build
add_custom_command(TARGET ${EXECUTABLE}
        POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -O srec --srec-len=64 ${EXECUTABLE} ${PROJECT_NAME}.s19
        COMMAND ${CMAKE_OBJCOPY} -O ihex ${EXECUTABLE} ${PROJECT_NAME}.hex
        COMMAND ${CMAKE_OBJCOPY} -O binary ${EXECUTABLE} ${PROJECT_NAME}.bin)


# Optional: Print executable size as part of the post build process
add_custom_command(TARGET ${EXECUTABLE}
        POST_BUILD
        COMMAND ${CMAKE_SIZE_UTIL} ${EXECUTABLE})
