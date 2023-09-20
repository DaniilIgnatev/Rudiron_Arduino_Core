#This file is part of Rudiron_Arduino_Core.

#Rudiron_Arduino_Core is free software: you can redistribute it and/or modify
#it under the terms of the GNU General Public License as published by
#the Free Software Foundation, either version 3 of the License, or
#(at your option) any later version.

#Rudiron_Arduino_Core is distributed in the hope that it will be useful,
#but WITHOUT ANY WARRANTY; without even the implied warranty of
#MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#GNU General Public License for more details.

#You should have received a copy of the GNU General Public License
#along with Rudiron_Arduino_Core.  If not, see <https://www.gnu.org/licenses/>.



set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR ARM)


#проверка нахождения gcc в path
if (WIN32)
    string(FIND "$ENV{PATH}" "gcc-arm-none-eabi" TOOLCHAIN_FOUND_IN_PATH)
else ()
    string(FIND "$ENV{PATH}" "gcc-arm-none-eabi" TOOLCHAIN_FOUND_IN_PATH)
endif ()

if (NOT ${TOOLCHAIN_FOUND_IN_PATH} MATCHES "-1")
    set(TOOLCHAIN_PREFIX arm-none-eabi-)
else ()
    message("GCC NOT FOUND IN PATH. SPECIFY FULL PATH TO GCC BIN FOLDER!")
    #при необходимости укажите полный путь к gcc
    set(ARM_TOOLCHAIN_DIR "/Users/../gcc-arm-none-eabi/bin")
    set(BINUTILS_PATH ${ARM_TOOLCHAIN_DIR})
    set(TOOLCHAIN_PREFIX ${ARM_TOOLCHAIN_DIR}/arm-none-eabi-)
endif ()


#назначение инструментария
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


#поиск корневых каталогов
set(CMAKE_FIND_ROOT_PATH ${BINUTILS_PATH})
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)


#проверка компилятора
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

