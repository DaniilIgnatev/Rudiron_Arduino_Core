cmake_minimum_required(VERSION 3.15.3)
# Optional: print out extra messages to see what is going on. Comment it to have less verbose messages
set(CMAKE_VERBOSE_MAKEFILE ON)

# Path to toolchain file. This one has to be before 'project()' below
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/arm-none-eabi-gcc.cmake)
# Setup project
project(Arduino_Core_Rudiron)
SET(CMAKE_BUILD_TYPE Debug)


set(cpu_name "MDR32F9Qx")
set(core "m3")
set(stdperiph "MDR32F9Qx_StdPeriph_Driver")


set(stdperlib_path "${CMAKE_SOURCE_DIR}/std-per-lib")
set(cmsis_path "${stdperlib_path}/CMSIS/${core}")

set(cmsis_coresupport_path "${cmsis_path}/CoreSupport")
file(GLOB_RECURSE SRC_cmsis_coresupport_path
    ${cmsis_coresupport_path}/*.cpp
    ${cmsis_coresupport_path}/*.hpp
    ${cmsis_coresupport_path}/*.c
    ${cmsis_coresupport_path}/*.h
    ${cmsis_coresupport_path}/old/*.cpp
    ${cmsis_coresupport_path}/old/*.hpp
    ${cmsis_coresupport_path}/old/*.c
    ${cmsis_coresupport_path}/old/*.h
    )

set(cmsis_devicesupport_path "${cmsis_path}/DeviceSupport/${cpu_name}/inc")
file(GLOB_RECURSE SRC_cmsis_devicesupport_path
    ${cmsis_devicesupport_path}/*.cpp
    ${cmsis_devicesupport_path}/*.hpp
    ${cmsis_devicesupport_path}/*.c
    ${cmsis_devicesupport_path}/*.h
    )

set(cmsis_devicesupport_startup_path "${cmsis_path}/DeviceSupport/${cpu_name}/startup")
file(GLOB_RECURSE SRC_cmsis_devicesupport_startup_path
    ${cmsis_devicesupport_startup_path}/*.cpp
    ${cmsis_devicesupport_startup_path}/*.hpp
    ${cmsis_devicesupport_startup_path}/*.c
    ${cmsis_devicesupport_startup_path}/*.h
    )

set(cmsis_devicesupport_startup_gcc_path "${cmsis_devicesupport_startup_path}/gcc")
file(GLOB_RECURSE SRC_cmsis_devicesupport_startup_gcc_path
    ${cmsis_devicesupport_startup_gcc_path}/*.ld
    ${cmsis_devicesupport_startup_gcc_path}/*.S
    )

set(periphery_path "${stdperlib_path}/${stdperiph}")
set(inc_periphery_path "${periphery_path}/inc")
file(GLOB_RECURSE SRC_inc_periphery_path
    ${inc_periphery_path}/*.cpp
    ${inc_periphery_path}/*.hpp
    ${inc_periphery_path}/*.c
    ${inc_periphery_path}/*.h
    )
file(GLOB_RECURSE SRC_src_periphery_path
    "${periphery_path}/src/*.cpp"
    "${periphery_path}/src/*.hpp"
    "${periphery_path}/src/*.c"
    "${periphery_path}/src/*.h"
    )

set(inc_periphery_usb_path "${periphery_path}/inc/USB_Library")
file(GLOB_RECURSE SRC_inc_periphery_usb_path
    ${inc_periphery_usb_path}/*.cpp
    ${inc_periphery_usb_path}/*.hpp
    ${inc_periphery_usb_path}/*.c
    ${inc_periphery_usb_path}/*.h
    )
file(GLOB_RECURSE SRC_src_periphery_usb_path
    "${periphery_path}/inc/USB_Library/src/*.cpp"
    "${periphery_path}/inc/USB_Library/src/*.hpp"
    "${periphery_path}/inc/USB_Library/src/*.c"
    "${periphery_path}/inc/USB_Library/src/*.h"
    )

set(inc_config_path "${stdperlib_path}/Config")
file(GLOB_RECURSE SRC_inc_config_path
    ${inc_config_path}/*.cpp
    ${inc_config_path}/*.hpp
    ${inc_config_path}/*.c
    ${inc_config_path}/*.h
    )

set(inc_config_cpu_path "${inc_config_path}/${cpu_name}")
file(GLOB_RECURSE SRC_inc_config_cpu_path
    ${inc_config_cpu_path}/*.cpp
    ${inc_config_cpu_path}/*.hpp
    ${inc_config_cpu_path}/*.c
    ${inc_config_cpu_path}/*.h
    )

set(sketch_path "sketch")
file(GLOB SRC_sketch_path
    ${sketch_path}/*.cpp)

set(libraries_path "libraries")
file(GLOB_RECURSE SRC_libraries_path

    )


#Output and linker file
set(EXECUTABLE ${PROJECT_NAME}.elf)
set(LINKER_FILE ${cmsis_devicesupport_startup_gcc_path}/${cpu_name}.ld)

enable_language(C ASM)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)


# Optional: issue a message to be sure it uses the correct toolchain file.
message(STATUS "CMAKE_TOOLCHAIN_FILE is: ${CMAKE_TOOLCHAIN_FILE}")


set(SRC_LIST
    ${SRC_cmsis_coresupport_path}
    ${SRC_cmsis_devicesupport_path}
    ${SRC_cmsis_devicesupport_startup_path}
    ${SRC_cmsis_devicesupport_startup_gcc_path}
    ${SRC_inc_periphery_path}
    ${SRC_src_periphery_path}
    ${SRC_inc_periphery_usb_path}
    ${SRC_src_periphery_usb_path}
    ${SRC_inc_config_path}
    ${SRC_inc_config_cpu_path}
    ${SRC_app_path}
    ${SRC_libraries_path}
    )


add_executable (${EXECUTABLE}
    ${SRC_LIST}
    )

# List of compiler defines, prefix with -D compiler option
target_compile_definitions(${EXECUTABLE} PRIVATE

    )

# List of includ directories
target_include_directories(${EXECUTABLE} PRIVATE
    ${cmsis_path}
    ${cmsis_coresupport_path}
    ${cmsis_devicesupport_startup_path}
    ${cmsis_devicesupport_startup_gcc_path}
    ${cmsis_devicesupport_path}

    ${periphery_path}
    ${inc_periphery_path}
    ${inc_periphery_usb_path}
    ${inc_config_path}
    ${inc_config_cpu_path}

    ${sketch_path}
    ${rudiron_sdk_path}
    ${rudiron_sdk_core_path}

    ${libraries_path}
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
    "-T${cmsis_devicesupport_startup_gcc_path}/${cpu_name}.ld"
    "-mcpu=cortex-${core}"
    "-mthumb"
    "-Wl,--gc-sections"
    "-specs=nosys.specs"
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
