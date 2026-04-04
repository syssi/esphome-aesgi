"""Schema structure tests for aesgi ESPHome component modules."""

import os
import sys

sys.path.insert(0, os.path.join(os.path.dirname(__file__), ".."))

import components.aesgi as hub  # noqa: E402
from components.aesgi import (  # noqa: E402
    binary_sensor,
    button,  # noqa: E402
    number,  # noqa: E402
    sensor,
    text_sensor,
)


class TestHubConstants:
    def test_conf_aesgi_id_defined(self):
        assert hub.CONF_AESGI_ID == "aesgi_id"


class TestSensorDefs:
    def test_sensor_defs_completeness(self):
        assert "status" in sensor.SENSOR_DEFS
        assert sensor.CONF_DC_VOLTAGE in sensor.SENSOR_DEFS
        assert sensor.CONF_DC_CURRENT in sensor.SENSOR_DEFS
        assert sensor.CONF_AC_VOLTAGE in sensor.SENSOR_DEFS
        assert sensor.CONF_AC_CURRENT in sensor.SENSOR_DEFS
        assert sensor.CONF_DEVICE_TEMPERATURE in sensor.SENSOR_DEFS
        assert len(sensor.SENSOR_DEFS) == 23

    def test_error_history_error_codes_count(self):
        assert len(sensor.ERROR_HISTORY_ERROR_CODES) == 6

    def test_error_history_error_times_count(self):
        assert len(sensor.ERROR_HISTORY_ERROR_TIMES) == 6

    def test_error_history_naming(self):
        assert (
            sensor.CONF_ERROR_HISTORY_SLOT1_ERROR_CODE
            == "error_history_slot1_error_code"
        )
        assert (
            sensor.CONF_ERROR_HISTORY_SLOT6_ERROR_CODE
            == "error_history_slot6_error_code"
        )
        assert (
            sensor.CONF_ERROR_HISTORY_SLOT1_ERROR_TIME
            == "error_history_slot1_error_time"
        )
        assert (
            sensor.CONF_ERROR_HISTORY_SLOT6_ERROR_TIME
            == "error_history_slot6_error_time"
        )


class TestBinarySensorDefs:
    def test_binary_sensor_defs_completeness(self):
        assert binary_sensor.CONF_BALANCING in binary_sensor.BINARY_SENSOR_DEFS
        assert binary_sensor.CONF_ONLINE_STATUS in binary_sensor.BINARY_SENSOR_DEFS
        assert len(binary_sensor.BINARY_SENSOR_DEFS) == 8

    def test_charging_discharging_present(self):
        from components.aesgi.const import CONF_CHARGING, CONF_DISCHARGING

        assert CONF_CHARGING in binary_sensor.BINARY_SENSOR_DEFS
        assert CONF_DISCHARGING in binary_sensor.BINARY_SENSOR_DEFS


class TestTextSensors:
    def test_text_sensors_list(self):
        assert text_sensor.CONF_ERRORS in text_sensor.TEXT_SENSORS
        assert text_sensor.CONF_OPERATION_MODE in text_sensor.TEXT_SENSORS
        assert text_sensor.CONF_DEVICE_TYPE in text_sensor.TEXT_SENSORS
        assert len(text_sensor.TEXT_SENSORS) == 3


class TestButtonConstants:
    def test_buttons_dict(self):
        assert button.CONF_AUTO_TEST in button.BUTTONS
        assert button.CONF_OPERATION_MODE_PV in button.BUTTONS
        assert len(button.BUTTONS) == 2

    def test_button_addresses_are_unique(self):
        addresses = list(button.BUTTONS.values())
        assert len(addresses) == len(set(addresses))


class TestNumberConstants:
    def test_numbers_dict(self):
        assert number.CONF_OUTPUT_POWER_THROTTLE in number.NUMBERS
        assert number.CONF_BATTERY_VOLTAGE_LIMIT in number.NUMBERS
        assert number.CONF_BATTERY_CURRENT_LIMIT in number.NUMBERS
        assert len(number.NUMBERS) == 4

    def test_number_addresses_are_unique(self):
        addresses = list(number.NUMBERS.values())
        assert len(addresses) == len(set(addresses))
