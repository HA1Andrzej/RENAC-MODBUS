# RENAC-MODBUS

Remember to copy the Custom Component renac.h file to the ESPHome folder in Home Assistant !!

The package request command can instead easily be send from ESPHome using **uart.write** and e.g. triggered via the ESPHome Time component.

Example: 

`time:
  - platform: homeassistant
    id: esptime
  - platform: sntp
    on_time:
       // Request package every 10 seconds from inverter at slave address: 0x01
      - seconds: /10
        then:
          - uart.write: [0x02, 0x05, 0x01, 0x02, 0x60, 0x01, 0x85, 0xFC, 0x03]`



💡 A list of all Renac inverters registers and the communication protocol is now finally publicly available and can be found here.

I've tested on ESP32 with hardware UART. I experience minor ESPHome <--> Home Assistant connection issues using the ESP32 software UART. So I've revised my production config to use the hardware UART pins instead. This is rock solid. But remember to turn off debug communication on the UART pins.
