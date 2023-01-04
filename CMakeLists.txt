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
project(Arduino_Core_Rudiron VERSION 1.0.0)

# Sources
set(core_path "${PROJECT_SOURCE_DIR}/core")
set(core_avr_path "${core_path}/avr")
set(core_rudiron_path "${core_path}/rudiron")
file(GLOB_RECURSE core_sources
        ${core_path}/*
        ${core_avr_path}/*
        ${core_rudiron_path}/*
)

set(libraries_path "${PROJECT_SOURCE_DIR}/libraries")
file(GLOB_RECURSE libraries_sources
        ${libraries_path}/*
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
file(GLOB_RECURSE system_sources
        ${system_path}/*
)

set(variants_path "${PROJECT_SOURCE_DIR}/variants/${cpu_name}/${board_name}")
file(GLOB_RECURSE variants_sources
        ${variants_path}/*
)

# Путь до скрипта линковщика
set(ARDUINO_CORE_RUDIRON_LINKER_FILE "${cmsis_startup_path}/${cpu_name}.ld" PARENT_SCOPE)

# Перечень путей к файлам исходного кода
set(ARDUIRON_CORE_RUDIRON_SOURCES
        ${system_sources}
        ${core_sources}
        ${cmsis_source_sources}
        ${cmsis_startup_sources}
        ${cmsis_include_sources}
        ${std_periph_driver_sources}
        ${variants_sources}
        ${libraries_sources}
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
        ${variants_path}
        ${libraries_path}
        ${libraries_path}/SPI
        ${libraries_path}/nrf24l01
        ${libraries_path}/ESP_AT_Lib
        ${libraries_path}/Wire
        ${libraries_path}/MPU6050
        ${libraries_path}/SD
        ${libraries_path}/Adafruit_BusIO
        ${libraries_path}/Adafruit_GFX_Library
        ${libraries_path}/Adafruit_GFX_Library/Fonts
        ${libraries_path}/Adafruit_ST7735_and_ST7789_Library
        ${libraries_path}/Adafruit_MPU6050
        ${libraries_path}/Adafruit_Unified_Sensor
        ${libraries_path}/Servo
        PARENT_SCOPE
        )
