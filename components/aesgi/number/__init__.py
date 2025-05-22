import esphome.codegen as cg
from esphome.components import number
import esphome.config_validation as cv
from esphome.const import (
    CONF_ID,
    CONF_MAX_VALUE,
    CONF_MIN_VALUE,
    CONF_MODE,
    CONF_STEP,
    CONF_UNIT_OF_MEASUREMENT,
    ENTITY_CATEGORY_CONFIG,
    ICON_EMPTY,
    UNIT_AMPERE,
    UNIT_PERCENT,
    UNIT_VOLT,
)

from .. import AESGI_COMPONENT_SCHEMA, CONF_AESGI_ID, aesgi_ns

DEPENDENCIES = ["aesgi"]

CODEOWNERS = ["@syssi"]

CONF_OUTPUT_POWER_THROTTLE = "output_power_throttle"
CONF_OUTPUT_POWER_THROTTLE_BROADCAST = "output_power_throttle_broadcast"
CONF_BATTERY_VOLTAGE_LIMIT = "battery_voltage_limit"
CONF_BATTERY_CURRENT_LIMIT = "battery_current_limit"

NUMBERS = {
    CONF_OUTPUT_POWER_THROTTLE: ord("L"),
    CONF_OUTPUT_POWER_THROTTLE_BROADCAST: ord("8"),
    CONF_BATTERY_VOLTAGE_LIMIT: ord("B"),
    CONF_BATTERY_CURRENT_LIMIT: ord("S"),
}

AesgiNumber = aesgi_ns.class_("AesgiNumber", number.Number, cg.Component)

AESGI_NUMBER_SCHEMA = (
    number.number_schema(
        AesgiNumber,
        icon=ICON_EMPTY,
        entity_category=ENTITY_CATEGORY_CONFIG,
    )
    .extend(
        {
            cv.Optional(CONF_STEP, default=0.1): cv.float_,
            cv.Optional(CONF_MODE, default="BOX"): cv.enum(
                number.NUMBER_MODES, upper=True
            ),
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
)

CONFIG_SCHEMA = AESGI_COMPONENT_SCHEMA.extend(
    {
        cv.Optional(CONF_OUTPUT_POWER_THROTTLE): AESGI_NUMBER_SCHEMA.extend(
            {
                cv.Optional(
                    CONF_UNIT_OF_MEASUREMENT, default=UNIT_PERCENT
                ): cv.string_strict,
                cv.Optional(CONF_MIN_VALUE, default=1.0): cv.float_,
                cv.Optional(CONF_MAX_VALUE, default=100.0): cv.float_,
                cv.Optional(CONF_STEP, default=1.0): cv.float_,
            }
        ),
        cv.Optional(CONF_OUTPUT_POWER_THROTTLE_BROADCAST): AESGI_NUMBER_SCHEMA.extend(
            {
                cv.Optional(
                    CONF_UNIT_OF_MEASUREMENT, default=UNIT_PERCENT
                ): cv.string_strict,
                cv.Optional(CONF_MIN_VALUE, default=1.0): cv.float_,
                cv.Optional(CONF_MAX_VALUE, default=100.0): cv.float_,
                cv.Optional(CONF_STEP, default=1.0): cv.float_,
            }
        ),
        cv.Optional(CONF_BATTERY_VOLTAGE_LIMIT): AESGI_NUMBER_SCHEMA.extend(
            {
                cv.Optional(
                    CONF_UNIT_OF_MEASUREMENT, default=UNIT_VOLT
                ): cv.string_strict,
                # 20-60V vs. 40-80V
                cv.Optional(CONF_MIN_VALUE, default=0.0): cv.float_,
                cv.Optional(CONF_MAX_VALUE, default=80.0): cv.float_,
            }
        ),
        cv.Optional(CONF_BATTERY_CURRENT_LIMIT): AESGI_NUMBER_SCHEMA.extend(
            {
                cv.Optional(
                    CONF_UNIT_OF_MEASUREMENT, default=UNIT_AMPERE
                ): cv.string_strict,
                cv.Optional(CONF_MIN_VALUE, default=0.0): cv.float_,
                cv.Optional(CONF_MAX_VALUE, default=20.0): cv.float_,
            }
        ),
    }
)


async def to_code(config):
    hub = await cg.get_variable(config[CONF_AESGI_ID])
    for key, address in NUMBERS.items():
        if key in config:
            conf = config[key]
            var = cg.new_Pvariable(conf[CONF_ID])
            await cg.register_component(var, conf)
            await number.register_number(
                var,
                conf,
                min_value=conf[CONF_MIN_VALUE],
                max_value=conf[CONF_MAX_VALUE],
                step=conf[CONF_STEP],
            )
            cg.add(getattr(hub, f"set_{key}_number")(var))
            cg.add(var.set_parent(hub))
            cg.add(var.set_holding_register(address))
