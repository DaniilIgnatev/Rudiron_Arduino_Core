/****************************************************************************************************************************
  HTTPGET.ino
  For ESP8266/ESP32-AT-command running shields

  ESP_AT_Lib is a wrapper library for the ESP8266/ESP32 AT-command shields

  Based on and modified from ESP8266 https://github.com/esp8266/Arduino/releases
  Built by Khoi Hoang https://github.com/khoih-prog/ESP_AT_Lib
  Licensed under MIT license

  @example HTTPGET.ino
  @brief The HTTPGET demo of library WeeESP8266.
  @author Wu Pengfei<pengfei.wu@itead.cc>
  @date 2015.03

  @par Copyright:
  Copyright (c) 2015 ITEAD Intelligent Systems Co., Ltd. \n\n
  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License as
  published by the Free Software Foundation; either version 2 of
  the License, or (at your option) any later version. \n\n
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
 *****************************************************************************************************************************/

#include "ESP_AT_Lib.h"
#include "ESP_AT_Debug.h"

#define SSID ""
#define PASSWORD ""

#define HOST_PORT (80)

ESP8266 wifi(&EspSerial);

void setup(void)
{
  while (true)
  {
    ESP_AT_LIB_DEBUG_OUTPUT.begin(115200);
    while (!ESP_AT_LIB_DEBUG_OUTPUT)
      ;

#if defined(BOARD_NAME)
    ESP_AT_LIB_DEBUG_OUTPUT.println("\nStart TCPServer on " + String(BOARD_NAME));
#else
    ESP_AT_LIB_DEBUG_OUTPUT.println("\nStart TCPServer");
#endif

    // initialize serial for ESP module
    EspSerial.begin(ESP_AT_BAUD);
    wifi.restart();

    ESP_AT_LIB_DEBUG_OUTPUT.print("FW Version:");
    ESP_AT_LIB_DEBUG_OUTPUT.println(wifi.getVersion().c_str());

    if (wifi.setOprToStationSoftAP())
    {
      ESP_AT_LIB_DEBUG_OUTPUT.println("Set AP/STA Mode OK");
    }
    else
    {
      ESP_AT_LIB_DEBUG_OUTPUT.println("Set AP/STA Mode failed");
      continue;
    }

    if (wifi.joinAP(SSID, PASSWORD))
    {
      ESP_AT_LIB_DEBUG_OUTPUT.println("Connect to WiFi OK");
      ESP_AT_LIB_DEBUG_OUTPUT.print("IP: ");
      ESP_AT_LIB_DEBUG_OUTPUT.println(wifi.getLocalIP().c_str());
    }
    else
    {
      ESP_AT_LIB_DEBUG_OUTPUT.println("Connect to WiFi failed");
      continue;
    }

    if (wifi.enableMUX())
    {
      ESP_AT_LIB_DEBUG_OUTPUT.println("enableMUX OK");
    }
    else
    {
      ESP_AT_LIB_DEBUG_OUTPUT.println("enableMUX failed");
      continue;
    }

    if (wifi.startTCPServer(HOST_PORT))
    {
      ESP_AT_LIB_DEBUG_OUTPUT.println("Start TCP server OK");
    }
    else
    {
      ESP_AT_LIB_DEBUG_OUTPUT.println("start TCP server failed");
      continue;
    }

    if (wifi.setTCPServerTimeout(10))
    {
      ESP_AT_LIB_DEBUG_OUTPUT.println("Set TCP server timeout 10 seconds");
      break;
    }
    else
    {
      ESP_AT_LIB_DEBUG_OUTPUT.println("Set TCP server timeout failed");
      continue;
    }
  }

  ESP_AT_LIB_DEBUG_OUTPUT.println("Done");
}
void loop(void)
{
  //Размер буфера приемника esp8266 = 1460 байт
  uint8_t buffer[1460] = {0};

  // id сокета
  uint8_t mux_id;

  uint32_t len = wifi.recv(&mux_id, buffer, sizeof(buffer), 100);

  if (len > 0)
  {
    ESP_AT_LIB_DEBUG_OUTPUT.print("Status:[");
    ESP_AT_LIB_DEBUG_OUTPUT.print(wifi.getIPStatus().c_str());
    ESP_AT_LIB_DEBUG_OUTPUT.println("]");

    ESP_AT_LIB_DEBUG_OUTPUT.print("Received from :");
    ESP_AT_LIB_DEBUG_OUTPUT.println(mux_id);
    ESP_AT_LIB_DEBUG_OUTPUT.print("[");

    for (uint32_t i = 0; i < len; i++)
    {
      ESP_AT_LIB_DEBUG_OUTPUT.print((char)buffer[i]);
    }

    ESP_AT_LIB_DEBUG_OUTPUT.println("]");

    if (wifi.send(mux_id, buffer, len))
    {
      ESP_AT_LIB_DEBUG_OUTPUT.println("Send back OK");
    }
    else
    {
      ESP_AT_LIB_DEBUG_OUTPUT.println("Send back failed");
    }

    if (wifi.releaseTCP(mux_id))
    {
      ESP_AT_LIB_DEBUG_OUTPUT.print("Release TCP ");
      ESP_AT_LIB_DEBUG_OUTPUT.print(mux_id);
      ESP_AT_LIB_DEBUG_OUTPUT.println(" OK");
    }
    else
    {
      ESP_AT_LIB_DEBUG_OUTPUT.print("Release TCP ");
      ESP_AT_LIB_DEBUG_OUTPUT.print(mux_id);
      ESP_AT_LIB_DEBUG_OUTPUT.println(" failed");
    }

    ESP_AT_LIB_DEBUG_OUTPUT.print("Status:[");
    ESP_AT_LIB_DEBUG_OUTPUT.print(wifi.getIPStatus().c_str());
    ESP_AT_LIB_DEBUG_OUTPUT.println("]");
  }
}
