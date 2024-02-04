# RENAC-MODBUS

> [!IMPORTANT]
> The code is just running, there may be problems!

##**Remember to copy the Custom Component renac.h file to the ESPHome folder in Home Assistant !!**

I used the example of communications [htvekov/solivia_esphom](https://github.com/htvekov/solivia_esphome)

**Supported devices** 
---
Renac inverters

R3-5K-DT (confirmed)

**Description**
---
The package request command can instead easily be send from ESPHome using ```uart.write``` and e.g. triggered via the ESPHome Time component.

Example: 
```
time:
  - platform: homeassistant
    id: esptime
  - platform: sntp
    on_time:
       // Request package every 10 seconds from inverter at slave address: 0x01
      - seconds: /10
        then:
          - uart.write: [0x02, 0x05, 0x01, 0x02, 0x60, 0x01, 0x85, 0xFC, 0x03]

```

💡 A list of all Renac inverters registers and the communication protocol is now finally publicly available and can be found [here](https://github.com/HA1Andrzej/RENAC-MODBUS/blob/main/RenacPower_On-Grid%20Inverter%20Modbus%20Protocol%20V0.02.pdf).

I've tested on ESP32 with hardware UART. I experience minor ESPHome <--> Home Assistant connection issues using the ESP32 software UART. So I've revised my production config to use the hardware UART pins instead. This is rock solid. But remember to turn off debug communication on the UART pins.

A few of the 'public known' commands have been tested. Most unfortunately did fail. Haven't really spend much time on testing further commands, as all the data i need is in gateway package.
```
          - uart.write: [0x01, 0x65, 0x04, 0x29, 0x04, 0x00, 0x1B, 0x29, 0xCC, 0x00, 0x10, 0x2A, 0x30, 0x00, 0x11, 0x2E, 0xE0, 0x00, 0x06, 0xFF, 0xD7]
```

Complete 'live' package example - captured 
```
Request:01:65:04:29:04:00:1B:29:CC:00:10:2A:30:00:11:2E:E0:00:06:FF:D7
Response:01:65:04:29:04:46:00:02:00:0C:00:01:00:00:00:00:00:00:00:00:00:19:00:00:00:02:13:68:00:00:00:00:00:00:00:7B:00:00:00:00:00:00:08:E8:00:09:13:88:09:60:00:0B:13:88:08:D3:00:06:13:88:00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:29:CC:20:00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:2A:30:22:00:02:09:CB:00:02:00:00:00:3A:0C:8D:00:02:00:00:00:57:00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:2E:E0
```
Analyzing the package, we were able to read the following data: 

```
01 	- slave adres
65 	- kod funkcji
04 	- 
29 04 	- start 1 rejestru "10500" 
46 	- ilość rejestrów 1 "106"
00 02 	- Operating mode tab. 2.4.2 "normal" 
00 0C 	- tab. 2.4.8 "Poland"
00 01 	- MPTT "Independent"
00 00 	- NA
00 00	- NA
00 00   - NA
00 00   - NA
00 01	- kWh / 10 dzienne 
00 00 	- 
00 02 12 D1 - total power  /10
00 00 00 00
00 00 00 A3 
00 00 00 00 
00 00 
08 76 	- R phase grid voltage /10 	"216.6"
00 09 	- R phase grid current /10 	"0.9"

13 88	- R phase grid frequency /100 	"50"
08 FF   - T phase grid voltage /10 	"203.3"
00 0B 	- T phase grid current /10 	"1,1"
13 88 	- T phase grid frequency /100 	"50"
09 10 	- S phase grid voltage /10 	"232.0"
00 06 	- S phase grid current /10 	"0,6"
13 88 	- S phase grid frequency /100 	"50"
00 00 00 00
00 00 00 00 
00 00 00 00 
00 00 00 00 

29 CC 	- jump to 10700 

20 	- kolejne 32 rejestry

00 00 00 00
00 00 00 00
00 00 00 00
00 00 00 00
00 00 00 00 
00 00 00 00
00 00 00 00
00 00 00 00 

2A 30 	- jump to 10800 

22 	- kolejne 34 rejestry

00 02	- ilość stringów ?

0B 0C 		- PV1 voltage /10 "282.8"
00 02 		- PV1 current /10 "0.2"
00 00 00 54 	- PV1 power "84"

0B D8		- PV2 voltage /10 "303.2" 
00 03		- PV2 voltage /10 "0.3"
00 00 00 64 	- PV1 power "100"

00 00 
00 00 
00 00 00 00 

00 00 
00 00
00 00 00 00 

2E E0 		- jump to 12000

0C		- kolejne 12 rejestró 

01 6F 		- heat sink temp /10 "36.7"
01 6F 		- ambient temp /10 "36.7 "

00 8A 		- INV1 temp /10 "13,8"
FC 18 		- INV2 temp /10	""
FC 18 		- INV3 temp /10
FC 18 		- BoostMax temp /10

58 0D		- CRC
```
