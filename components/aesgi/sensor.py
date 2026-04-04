import esphome.codegen as cg
from esphome.components import sensor
import esphome.config_validation as cv
from esphome.const import (
    CONF_STATUS,
    DEVICE_CLASS_CURRENT,
    DEVICE_CLASS_DURATION,
    DEVICE_CLASS_EMPTY,
    DEVICE_CLASS_ENERGY,
    DEVICE_CLASS_FREQUENCY,
    DEVICE_CLASS_POWER,
    DEVICE_CLASS_TEMPERATURE,
    DEVICE_CLASS_VOLTAGE,
    ENTITY_CATEGORY_DIAGNOSTIC,
    ICON_COUNTER,
    STATE_CLASS_MEASUREMENT,
    STATE_CLASS_TOTAL_INCREASING,
    UNIT_AMPERE,
    UNIT_CELSIUS,
    UNIT_EMPTY,
    UNIT_HERTZ,
    UNIT_MILLISECOND,
    UNIT_PERCENT,
    UNIT_SECOND,
    UNIT_VOLT,
    UNIT_WATT,
    UNIT_WATT_HOURS,
)

from . import AESGI_COMPONENT_SCHEMA, CONF_AESGI_ID

DEPENDENCIES = ["aesgi"]

CODEOWNERS = ["@syssi"]

# CONF_STATUS = "status"
CONF_DC_VOLTAGE = "dc_voltage"
CONF_DC_CURRENT = "dc_current"
CONF_DC_POWER = "dc_power"
CONF_AC_VOLTAGE = "ac_voltage"
CONF_AC_CURRENT = "ac_current"
CONF_AC_POWER = "ac_power"
CONF_DEVICE_TEMPERATURE = "device_temperature"
CONF_ENERGY_TODAY = "energy_today"
CONF_OUTPUT_POWER_THROTTLE = "output_power_throttle"
CONF_BATTERY_CURRENT_LIMIT = "battery_current_limit"
CONF_BATTERY_VOLTAGE_LIMIT = "battery_voltage_limit"
CONF_UPTIME = "uptime"
CONF_AC_VOLTAGE_NOMINAL = "ac_voltage_nominal"
CONF_AC_FREQUENCY_NOMINAL = "ac_frequency_nominal"
CONF_AC_VOLTAGE_UPPER_LIMIT = "ac_voltage_upper_limit"
CONF_AC_VOLTAGE_UPPER_LIMIT_DELAY = "ac_voltage_upper_limit_delay"
CONF_AC_VOLTAGE_LOWER_LIMIT = "ac_voltage_lower_limit"
CONF_AC_VOLTAGE_LOWER_LIMIT_DELAY = "ac_voltage_lower_limit_delay"
CONF_AC_FREQUENCY_UPPER_LIMIT = "ac_frequency_upper_limit"
CONF_AC_FREQUENCY_UPPER_LIMIT_DELAY = "ac_frequency_upper_limit_delay"
CONF_AC_FREQUENCY_LOWER_LIMIT = "ac_frequency_lower_limit"
CONF_AC_FREQUENCY_LOWER_LIMIT_DELAY = "ac_frequency_lower_limit_delay"

CONF_ERROR_HISTORY_SLOT1_ERROR_CODE = "error_history_slot1_error_code"
CONF_ERROR_HISTORY_SLOT2_ERROR_CODE = "error_history_slot2_error_code"
CONF_ERROR_HISTORY_SLOT3_ERROR_CODE = "error_history_slot3_error_code"
CONF_ERROR_HISTORY_SLOT4_ERROR_CODE = "error_history_slot4_error_code"
CONF_ERROR_HISTORY_SLOT5_ERROR_CODE = "error_history_slot5_error_code"
CONF_ERROR_HISTORY_SLOT6_ERROR_CODE = "error_history_slot6_error_code"

CONF_ERROR_HISTORY_SLOT1_ERROR_TIME = "error_history_slot1_error_time"
CONF_ERROR_HISTORY_SLOT2_ERROR_TIME = "error_history_slot2_error_time"
CONF_ERROR_HISTORY_SLOT3_ERROR_TIME = "error_history_slot3_error_time"
CONF_ERROR_HISTORY_SLOT4_ERROR_TIME = "error_history_slot4_error_time"
CONF_ERROR_HISTORY_SLOT5_ERROR_TIME = "error_history_slot5_error_time"
CONF_ERROR_HISTORY_SLOT6_ERROR_TIME = "error_history_slot6_error_time"

# key: sensor_schema kwargs
SENSOR_DEFS = {
    CONF_STATUS: {
        "unit_of_measurement": UNIT_EMPTY,
        "icon": "mdi:heart-pulse",
        "accuracy_decimals": 0,
        "device_class": DEVICE_CLASS_EMPTY,
        "state_class": STATE_CLASS_MEASUREMENT,
    },
    CONF_DC_VOLTAGE: {
        "unit_of_measurement": UNIT_VOLT,
        "accuracy_decimals": 1,
        "device_class": DEVICE_CLASS_VOLTAGE,
        "state_class": STATE_CLASS_MEASUREMENT,
    },
    CONF_DC_CURRENT: {
        "unit_of_measurement": UNIT_AMPERE,
        "icon": "mdi:current-dc",
        "accuracy_decimals": 2,
        "device_class": DEVICE_CLASS_CURRENT,
        "state_class": STATE_CLASS_MEASUREMENT,
    },
    CONF_DC_POWER: {
        "unit_of_measurement": UNIT_WATT,
        "icon": "mdi:solar-power",
        "accuracy_decimals": 0,
        "device_class": DEVICE_CLASS_POWER,
        "state_class": STATE_CLASS_MEASUREMENT,
    },
    CONF_AC_VOLTAGE: {
        "unit_of_measurement": UNIT_VOLT,
        "accuracy_decimals": 1,
        "device_class": DEVICE_CLASS_VOLTAGE,
        "state_class": STATE_CLASS_MEASUREMENT,
    },
    CONF_AC_CURRENT: {
        "unit_of_measurement": UNIT_AMPERE,
        "icon": "mdi:current-ac",
        "accuracy_decimals": 2,
        "device_class": DEVICE_CLASS_CURRENT,
        "state_class": STATE_CLASS_MEASUREMENT,
    },
    CONF_AC_POWER: {
        "unit_of_measurement": UNIT_WATT,
        "accuracy_decimals": 0,
        "device_class": DEVICE_CLASS_POWER,
        "state_class": STATE_CLASS_MEASUREMENT,
    },
    CONF_DEVICE_TEMPERATURE: {
        "unit_of_measurement": UNIT_CELSIUS,
        "accuracy_decimals": 0,
        "device_class": DEVICE_CLASS_TEMPERATURE,
        "state_class": STATE_CLASS_MEASUREMENT,
    },
    CONF_ENERGY_TODAY: {
        "unit_of_measurement": UNIT_WATT_HOURS,
        "icon": ICON_COUNTER,
        "accuracy_decimals": 0,
        "device_class": DEVICE_CLASS_ENERGY,
        "state_class": STATE_CLASS_TOTAL_INCREASING,
        "entity_category": ENTITY_CATEGORY_DIAGNOSTIC,
    },
    CONF_OUTPUT_POWER_THROTTLE: {
        "unit_of_measurement": UNIT_PERCENT,
        "icon": "mdi:solar-power",
        "accuracy_decimals": 0,
        "state_class": STATE_CLASS_MEASUREMENT,
    },
    CONF_BATTERY_CURRENT_LIMIT: {
        "unit_of_measurement": UNIT_AMPERE,
        "icon": "mdi:current-dc",
        "accuracy_decimals": 1,
        "device_class": DEVICE_CLASS_CURRENT,
        "state_class": STATE_CLASS_MEASUREMENT,
    },
    CONF_BATTERY_VOLTAGE_LIMIT: {
        "unit_of_measurement": UNIT_VOLT,
        "accuracy_decimals": 1,
        "device_class": DEVICE_CLASS_VOLTAGE,
        "state_class": STATE_CLASS_MEASUREMENT,
    },
    CONF_UPTIME: {
        "unit_of_measurement": UNIT_SECOND,
        "icon": "mdi:timer",
        "accuracy_decimals": 0,
        "device_class": DEVICE_CLASS_DURATION,
        "state_class": STATE_CLASS_TOTAL_INCREASING,
        "entity_category": ENTITY_CATEGORY_DIAGNOSTIC,
    },
    CONF_AC_VOLTAGE_NOMINAL: {
        "unit_of_measurement": UNIT_VOLT,
        "accuracy_decimals": 1,
        "device_class": DEVICE_CLASS_VOLTAGE,
        "state_class": STATE_CLASS_MEASUREMENT,
        "entity_category": ENTITY_CATEGORY_DIAGNOSTIC,
    },
    CONF_AC_FREQUENCY_NOMINAL: {
        "unit_of_measurement": UNIT_HERTZ,
        "accuracy_decimals": 1,
        "device_class": DEVICE_CLASS_FREQUENCY,
        "state_class": STATE_CLASS_MEASUREMENT,
        "entity_category": ENTITY_CATEGORY_DIAGNOSTIC,
    },
    CONF_AC_VOLTAGE_UPPER_LIMIT: {
        "unit_of_measurement": UNIT_VOLT,
        "accuracy_decimals": 1,
        "device_class": DEVICE_CLASS_VOLTAGE,
        "state_class": STATE_CLASS_MEASUREMENT,
        "entity_category": ENTITY_CATEGORY_DIAGNOSTIC,
    },
    CONF_AC_VOLTAGE_UPPER_LIMIT_DELAY: {
        "unit_of_measurement": UNIT_MILLISECOND,
        "icon": "mdi:timer-cancel-outline",
        "accuracy_decimals": 0,
        "device_class": DEVICE_CLASS_DURATION,
        "entity_category": ENTITY_CATEGORY_DIAGNOSTIC,
    },
    CONF_AC_VOLTAGE_LOWER_LIMIT: {
        "unit_of_measurement": UNIT_VOLT,
        "accuracy_decimals": 1,
        "device_class": DEVICE_CLASS_VOLTAGE,
        "state_class": STATE_CLASS_MEASUREMENT,
        "entity_category": ENTITY_CATEGORY_DIAGNOSTIC,
    },
    CONF_AC_VOLTAGE_LOWER_LIMIT_DELAY: {
        "unit_of_measurement": UNIT_MILLISECOND,
        "icon": "mdi:timer-cancel-outline",
        "accuracy_decimals": 0,
        "device_class": DEVICE_CLASS_DURATION,
        "entity_category": ENTITY_CATEGORY_DIAGNOSTIC,
    },
    CONF_AC_FREQUENCY_UPPER_LIMIT: {
        "unit_of_measurement": UNIT_HERTZ,
        "accuracy_decimals": 2,
        "device_class": DEVICE_CLASS_FREQUENCY,
        "state_class": STATE_CLASS_MEASUREMENT,
        "entity_category": ENTITY_CATEGORY_DIAGNOSTIC,
    },
    CONF_AC_FREQUENCY_UPPER_LIMIT_DELAY: {
        "unit_of_measurement": UNIT_MILLISECOND,
        "icon": "mdi:timer-cancel-outline",
        "accuracy_decimals": 0,
        "device_class": DEVICE_CLASS_DURATION,
        "entity_category": ENTITY_CATEGORY_DIAGNOSTIC,
    },
    CONF_AC_FREQUENCY_LOWER_LIMIT: {
        "unit_of_measurement": UNIT_HERTZ,
        "accuracy_decimals": 2,
        "device_class": DEVICE_CLASS_FREQUENCY,
        "state_class": STATE_CLASS_MEASUREMENT,
        "entity_category": ENTITY_CATEGORY_DIAGNOSTIC,
    },
    CONF_AC_FREQUENCY_LOWER_LIMIT_DELAY: {
        "unit_of_measurement": UNIT_MILLISECOND,
        "icon": "mdi:timer-cancel-outline",
        "accuracy_decimals": 0,
        "device_class": DEVICE_CLASS_DURATION,
        "entity_category": ENTITY_CATEGORY_DIAGNOSTIC,
    },
}

ERROR_HISTORY_ERROR_CODES = [
    CONF_ERROR_HISTORY_SLOT1_ERROR_CODE,
    CONF_ERROR_HISTORY_SLOT2_ERROR_CODE,
    CONF_ERROR_HISTORY_SLOT3_ERROR_CODE,
    CONF_ERROR_HISTORY_SLOT4_ERROR_CODE,
    CONF_ERROR_HISTORY_SLOT5_ERROR_CODE,
    CONF_ERROR_HISTORY_SLOT6_ERROR_CODE,
]

ERROR_HISTORY_ERROR_TIMES = [
    CONF_ERROR_HISTORY_SLOT1_ERROR_TIME,
    CONF_ERROR_HISTORY_SLOT2_ERROR_TIME,
    CONF_ERROR_HISTORY_SLOT3_ERROR_TIME,
    CONF_ERROR_HISTORY_SLOT4_ERROR_TIME,
    CONF_ERROR_HISTORY_SLOT5_ERROR_TIME,
    CONF_ERROR_HISTORY_SLOT6_ERROR_TIME,
]

_ERROR_CODE_SCHEMA = sensor.sensor_schema(
    unit_of_measurement=UNIT_EMPTY,
    icon="mdi:alert-circle-outline",
    accuracy_decimals=0,
    device_class=DEVICE_CLASS_EMPTY,
    state_class=STATE_CLASS_MEASUREMENT,
)

_ERROR_TIME_SCHEMA = sensor.sensor_schema(
    unit_of_measurement=UNIT_SECOND,
    icon="mdi:clock-outline",
    accuracy_decimals=0,
    device_class=DEVICE_CLASS_EMPTY,
)

CONFIG_SCHEMA = (
    AESGI_COMPONENT_SCHEMA.extend(
        {
            cv.Optional(key): sensor.sensor_schema(**kwargs)
            for key, kwargs in SENSOR_DEFS.items()
        }
    )
    .extend({cv.Optional(key): _ERROR_CODE_SCHEMA for key in ERROR_HISTORY_ERROR_CODES})
    .extend({cv.Optional(key): _ERROR_TIME_SCHEMA for key in ERROR_HISTORY_ERROR_TIMES})
)


async def to_code(config):
    hub = await cg.get_variable(config[CONF_AESGI_ID])
    for key in SENSOR_DEFS:
        if key in config:
            conf = config[key]
            sens = await sensor.new_sensor(conf)
            cg.add(getattr(hub, f"set_{key}_sensor")(sens))
    for i, key in enumerate(ERROR_HISTORY_ERROR_CODES):
        if key in config:
            conf = config[key]
            sens = await sensor.new_sensor(conf)
            cg.add(hub.set_error_history_error_code_sensor(i, sens))
    for i, key in enumerate(ERROR_HISTORY_ERROR_TIMES):
        if key in config:
            conf = config[key]
            sens = await sensor.new_sensor(conf)
            cg.add(hub.set_error_history_error_time_sensor(i, sens))
