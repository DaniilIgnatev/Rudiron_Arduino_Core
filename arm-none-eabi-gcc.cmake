# This file is part of Arduino_Core_Rudiron_MDR32F9Qx.

# Arduino_Core_Rudiron_MDR32F9Qx is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# Arduino_Core_Rudiron_MDR32F9Qx is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with Arduino_Core_Rudiron_MDR32F9Qx. If not, see <https://www.gnu.org/licenses/>.

# Author: Daniil Ignatev


set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR ARM)


# Определение пути к Arduino15
if(WIN32)
    set(ARDUINO15_DIR "$ENV{USERPROFILE}/AppData/Local/Arduino15")
elseif(APPLE)
    set(ARDUINO15_DIR "$ENV{HOME}/Library/Arduino15")
else()
    set(ARDUINO15_DIR "$ENV{HOME}/.arduino15")
endif()
string(REPLACE "\\" "/" ARDUINO15_DIR ${ARDUINO15_DIR})


# Полный путь к gcc
set(ARM_TOOLCHAIN_DIR "${ARDUINO15_DIR}/packages/Rudiron/tools/gcc-arm-none-eabi/default/bin")
set(BINUTILS_PATH ${ARM_TOOLCHAIN_DIR})
set(TOOLCHAIN_PREFIX ${ARM_TOOLCHAIN_DIR}/arm-none-eabi-)


# Назначение инструментария
if (WIN32)
    set(CMAKE_C_COMPILER "${TOOLCHAIN_PREFIX}gcc.exe")
    set(CMAKE_CXX_COMPILER "${TOOLCHAIN_PREFIX}g++.exe")
else ()
    set(CMAKE_C_COMPILER "${TOOLCHAIN_PREFIX}gcc")
    set(CMAKE_CXX_COMPILER "${TOOLCHAIN_PREFIX}g++")
endif ()

set(CMAKE_ASM_COMPILER ${CMAKE_C_COMPILER})
set(CMAKE_OBJCOPY ${TOOLCHAIN_PREFIX}objcopy CACHE INTERNAL "objcopy tool")
set(CMAKE_SIZE_UTIL ${TOOLCHAIN_PREFIX}size CACHE INTERNAL "size tool")
set(CMAKE_EXE_LINKER_FLAGS "--specs=nosys.specs" CACHE INTERNAL "")


# Поиск корневых каталогов
set(CMAKE_FIND_ROOT_PATH ${BINUTILS_PATH})
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)


# Проверка компилятора
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

