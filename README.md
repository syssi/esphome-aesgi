# esphome-aesgi

![GitHub actions](https://github.com/syssi/esphome-aesgi/actions/workflows/ci.yaml/badge.svg)
![GitHub stars](https://img.shields.io/github/stars/syssi/esphome-aesgi)
![GitHub forks](https://img.shields.io/github/forks/syssi/esphome-aesgi)
![GitHub watchers](https://img.shields.io/github/watchers/syssi/esphome-aesgi)
[!["Buy Me A Coffee"](https://img.shields.io/badge/buy%20me%20a%20coffee-donate-yellow.svg)](https://www.buymeacoffee.com/syssi)

ESPHome component to monitor a AEconversion micro inverter using RS485

## Supported devices

* PV350W

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
* https://github.com/akrypth/aeclogger/blob/master/main.c
* https://github.com/snaptec/openWB/issues/7
* https://github.com/swissembedded/em/blob/master/basicapps/aeconversion_aesgi/aesgi.blib
* https://forum.fhem.de/index.php?topic=46264
* https://solaranzeige.de/phpBB3/viewtopic.php?t=1167
* https://wiki.opensourceecology.de/Solarspeicher_(station%C3%A4r)#Protokoll_und_Kommunikation
