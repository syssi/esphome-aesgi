# esphome-aesgi

![GitHub actions](https://github.com/syssi/esphome-aesgi/actions/workflows/ci.yaml/badge.svg)
![GitHub stars](https://img.shields.io/github/stars/syssi/esphome-aesgi)
![GitHub forks](https://img.shields.io/github/forks/syssi/esphome-aesgi)
![GitHub watchers](https://img.shields.io/github/watchers/syssi/esphome-aesgi)
[!["Buy Me A Coffee"](https://img.shields.io/badge/buy%20me%20a%20coffee-donate-yellow.svg)](https://www.buymeacoffee.com/syssi)

ESPHome component to monitor a AEconversion micro inverter using RS485

## Supported devices

* INV250-45EU RS485
* INV350-60EU RS485  (`PV350W`)
* INV500-90EU RS485

## Requirements

* [ESPHome 2024.6.0 or higher](https://github.com/esphome/esphome/releases)
* Generic ESP32 or ESP8266 board

## Installation

You can install this component with [ESPHome external components feature](https://esphome.io/components/external_components.html) like this:
```yaml
external_components:
  - source: github://syssi/esphome-aesgi@main
```

or just use the `esp32-ble-example.yaml` as proof of concept:

```bash
# Install esphome
pip3 install esphome

# Clone this external component
git clone https://github.com/syssi/esphome-aesgi.git
cd esphome-aesgi

# Create a secrets.yaml containing some setup specific secrets
cat > secrets.yaml <<EOF
wifi_ssid: MY_WIFI_SSID
wifi_password: MY_WIFI_PASSWORD

mqtt_host: MY_MQTT_HOST
mqtt_username: MY_MQTT_USERNAME
mqtt_password: MY_MQTT_PASSWORD
EOF

# Validate the configuration, create a binary, upload it, and start logs
esphome run esp32-ble-example.yaml

```

## Example response all sensors enabled

```
[aesgi:073]: Status frame received (58 bytes)
[sensor:093]: 'aesgi status': Sending state 0.00000  with 0 decimals of accuracy
[sensor:093]: 'aesgi dc voltage': Sending state 20.00000 V with 1 decimals of accuracy
[sensor:093]: 'aesgi dc current': Sending state 0.00000 A with 2 decimals of accuracy
[sensor:093]: 'aesgi dc power': Sending state 0.00000 W with 0 decimals of accuracy
[sensor:093]: 'aesgi ac voltage': Sending state 235.10001 V with 1 decimals of accuracy
[sensor:093]: 'aesgi ac current': Sending state 0.01000 A with 2 decimals of accuracy
[sensor:093]: 'aesgi ac power': Sending state 1.00000 W with 0 decimals of accuracy
[sensor:093]: 'aesgi device temperature': Sending state 50.00000 °C with 0 decimals of accuracy
[sensor:093]: 'aesgi energy today': Sending state 44.00000 Wh with 0 decimals of accuracy
[aesgi:111]: Device type frame received (14 bytes)
[text_sensor:064]: 'aesgi device type': Sending state 'PV350W'
[aesgi:130]: Output power frame received (11 bytes)
[sensor:093]: 'aesgi output power': Sending state 100.00000 % with 0 decimals of accuracy
[aesgi:149]: Settings frame received (62 bytes)
[aesgi:160]: Errors frame received (73 bytes)
[aesgi:171]: Current limit frame received (12 bytes)
[sensor:093]: 'aesgi current limit': Sending state 11.00000 A with 1 decimals of accuracy
[aesgi:190]: Operation mode frame received (14 bytes)
[text_sensor:064]: 'aesgi operation mode': Sending state 'MPPT'
[sensor:093]: 'aesgi voltage limit': Sending state 20.00000 V with 1 decimals of accuracy
```

## Protocol

See [AESGI_V1.7_DE.PDF](https://www.photovoltaikforum.com/core/attachment/131681-aesgi-v1-7-de-pdf/).

## Known issues

None.

## Debugging

If this component doesn't work out of the box for your device please update your configuration to enable the debug output of the UART component and increase the log level to the see outgoing and incoming serial traffic:

```
logger:
  level: VERY_VERBOSE
  logs:
    scheduler: DEBUG
    component: DEBUG

uart:
  - id: uart_0
    baud_rate: 9600
    tx_pin: ${tx_pin}
    rx_pin: ${rx_pin}
    debug:
      direction: BOTH
      dummy_receiver: false
      after:
        delimiter: "\n"
      sequence:
        - lambda: UARTDebug::log_string(direction, bytes);
```

## References

* https://www.photovoltaikforum.com/core/attachment/131681-aesgi-v1-7-de-pdf/
* https://wiki.opensourceecology.de/Solarspeicher_(station%C3%A4r)#Protokoll_und_Kommunikation
* https://github.com/swissembedded/em/blob/master/basicapps/aeconversion_aesgi/aesgi.blib
* https://forum.fhem.de/index.php?topic=46264
