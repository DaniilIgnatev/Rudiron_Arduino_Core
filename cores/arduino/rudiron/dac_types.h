/*
This file is part of Arduino_Core_Rudiron_MDR32F9Qx.

Arduino_Core_Rudiron_MDR32F9Qx is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Arduino_Core_Rudiron_MDR32F9Qx is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Arduino_Core_Rudiron_MDR32F9Qx. If not, see <https://www.gnu.org/licenses/>.

Author: Daniil Ignatev
*/

#ifndef DAC_TYPES_H
#define DAC_TYPES_H

namespace Rudiron
{
    // Блоки ЦАП
    typedef enum
    {
        DAC_None = -1,
        DAC_1 = 0,
        DAC_2,
    } DAC_Name;

    // Каналы цап
    typedef enum
    {
        DAC_Channel_None = -1,
        DAC_Channel_1 = 0,
        DAC_Channel_2,
    } DAC_ChannelName;
}

#endif // DAC_TYPES_H
