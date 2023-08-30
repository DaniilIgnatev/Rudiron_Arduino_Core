# This file is part of Arduino_Core_Rudiron.

# Arduino_Core_Rudiron is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# Arduino_Core_Rudiron is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with Arduino_Core_Rudiron.  If not, see <https://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.15.3)
set(CMAKE_VERBOSE_MAKEFILE OFF)

# Название проекта
project(Arduino_Core_Rudiron)

# Sources
set(core_path "${PROJECT_SOURCE_DIR}/core")
set(core_avr_path "${core_path}/avr")
set(core_rudiron_path "${core_path}/rudiron")
file(GLOB_RECURSE core_sources
        ${core_path}/*
        ${core_avr_path}/*
        ${core_rudiron_path}/*
)

set(cmsis_path "${PROJECT_SOURCE_DIR}/system/Drivers/CMSIS/Device/MILANDR/${cpu_name}")
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

set(std_periph_driver_path "${PROJECT_SOURCE_DIR}/system/Drivers/${cpu_name}_StdPeriph_Driver")
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

set(system_path "${PROJECT_SOURCE_DIR}/system/${cpu_name}")
set(system_board_path "${system_path}/${board_name}")
file(GLOB_RECURSE system_sources
        ${system_path}/*
        ${system_board_path}/*
)

set(variants_path "${PROJECT_SOURCE_DIR}/variants/${cpu_name}/${board_name}")
file(GLOB_RECURSE variants_sources
        ${variants_path}/*
)

# Путь до скрипта линковщика
set(ARDUINO_CORE_RUDIRON_LINKER_FILE "${cmsis_startup_path}/${cpu_name}.ld" PARENT_SCOPE)

# Путь до корневой папки библиотек
set(libraries_path "${PROJECT_SOURCE_DIR}/libraries")

# Файлы исходного кода библиотек
file(GLOB_RECURSE SPI_sources ${libraries_path}/SPI/*)
file(GLOB_RECURSE Wire_sources ${libraries_path}/Wire/*)

file(GLOB_RECURSE nrf24l01_sources_ ${libraries_path}/nrf24l01/*)
set(nrf24l01_sources ${nrf24l01_sources_} PARENT_SCOPE)
file(GLOB_RECURSE ESP_AT_Lib_sources_ ${libraries_path}/ESP_AT_Lib/*)
set(ESP_AT_Lib_sources ${ESP_AT_Lib_sources_} PARENT_SCOPE)
file(GLOB_RECURSE MPU6050_sources_ ${libraries_path}/MPU6050/*)
set(MPU6050_sources ${MPU6050_sources_} PARENT_SCOPE)
file(GLOB_RECURSE SD_sources_ ${libraries_path}/SD/*)
set(SD_sources ${SD_sources_} PARENT_SCOPE)
file(GLOB_RECURSE Adafruit_BusIO_sources_ ${libraries_path}/Adafruit_BusIO/*)
set(Adafruit_BusIO_sources ${Adafruit_BusIO_sources_} PARENT_SCOPE)
file(GLOB_RECURSE Adafruit_Unified_Sensor_sources_ ${libraries_path}/Adafruit_Unified_Sensor/*)
set(Adafruit_Unified_Sensor_sources ${Adafruit_Unified_Sensor_sources_} PARENT_SCOPE)
file(GLOB_RECURSE Adafruit_GFX_Library_sources_ ${libraries_path}/Adafruit_GFX_Library/*)
set(Adafruit_GFX_Library_sources ${Adafruit_GFX_Library_sources_} PARENT_SCOPE)
file(GLOB_RECURSE Adafruit_ST7735_and_ST7789_Library_sources_ ${libraries_path}/Adafruit_ST7735_and_ST7789_Library/*)
set(Adafruit_ST7735_and_ST7789_Library_sources ${Adafruit_ST7735_and_ST7789_Library_sources_} PARENT_SCOPE)
file(GLOB_RECURSE Adafruit_MPU6050_sources_ ${libraries_path}/Adafruit_MPU6050/*)
set(Adafruit_MPU6050_sources ${Adafruit_MPU6050_sources_} PARENT_SCOPE)
file(GLOB_RECURSE Servo_sources_ ${libraries_path}/Servo/*)
set(Servo_sources ${Servo_sources_} PARENT_SCOPE)
file(GLOB_RECURSE Dynamixel_sources_ ${libraries_path}/Dynamixel/*)
set(Dynamixel_sources ${Dynamixel_sources_} PARENT_SCOPE)
file(GLOB_RECURSE FastLED_sources_ ${libraries_path}/FastLED/*)
set(FastLED_sources ${FastLED_sources_} PARENT_SCOPE)

# Пути к заголовочным файлам библиотек
set(SPI_includes ${libraries_path}/SPI)
set(Wire_includes ${libraries_path}/Wire)

set(nrf24l01_includes ${libraries_path}/nrf24l01 PARENT_SCOPE)
set(ESP_AT_Lib_includes ${libraries_path}/ESP_AT_Lib PARENT_SCOPE)
set(MPU6050_includes ${libraries_path}/MPU6050 PARENT_SCOPE)
set(SD_includes ${libraries_path}/SD PARENT_SCOPE)
set(Adafruit_BusIO_includes ${libraries_path}/Adafruit_BusIO PARENT_SCOPE)
set(Adafruit_Unified_Sensor_includes ${libraries_path}/Adafruit_Unified_Sensor PARENT_SCOPE)
set(Adafruit_GFX_Library_includes ${libraries_path}/Adafruit_GFX_Library ${libraries_path}/Adafruit_GFX_Library/Fonts PARENT_SCOPE)
set(Adafruit_ST7735_and_ST7789_Library_includes ${libraries_path}/Adafruit_ST7735_and_ST7789_Library PARENT_SCOPE)
set(Adafruit_MPU6050_includes ${libraries_path}/Adafruit_MPU6050 PARENT_SCOPE)
set(Servo_includes ${libraries_path}/Servo PARENT_SCOPE)
set(Dynamixel_includes ${libraries_path}/Dynamixel PARENT_SCOPE)
set(FastLED_includes ${libraries_path}/FastLED ${libraries_path}/FastLED/lib8tion ${libraries_path}/FastLED/platforms ${libraries_path}/FastLED/platforms/arm/common ${libraries_path}/FastLED/platforms/arm/MDR32F9Qx PARENT_SCOPE)

# Перечень путей к файлам исходного кода
set(ARDUIRON_CORE_RUDIRON_SOURCES
        ${system_sources}
        ${core_sources}
        ${cmsis_source_sources}
        ${cmsis_startup_sources}
        ${cmsis_include_sources}
        ${std_periph_driver_sources}
        ${variants_sources}

        ${SPI_sources}
        ${Wire_sources}

        PARENT_SCOPE)

# Перечень путей к файлам заголовков
set(ARDUIRON_CORE_RUDIRON_INCLUDES
        ${core_path}
        ${cmsis_include_path}
        ${cmsis_source_path}
        ${cmsis_source_templates_path}
        ${cmsis_startup_path}
        ${std_periph_driver_inc_path}
        ${std_periph_driver_inc_usb_path}
        ${std_periph_driver_src_path}
        ${std_periph_driver_src_usb_path}
        ${system_path}
        ${system_board_path}
        ${variants_path}

        ${SPI_includes}
        ${Wire_includes}

        PARENT_SCOPE)
