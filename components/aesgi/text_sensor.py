import esphome.codegen as cg
from esphome.components import text_sensor
import esphome.config_validation as cv
from esphome.const import CONF_ID

from . import AESGI_COMPONENT_SCHEMA, CONF_AESGI_ID

DEPENDENCIES = ["aesgi"]

CODEOWNERS = ["@syssi"]

CONF_ERRORS = "errors"
CONF_OPERATION_MODE = "operation_mode"
CONF_DEVICE_TYPE = "device_type"

TEXT_SENSORS = [
    CONF_OPERATION_MODE,
    CONF_ERRORS,
    CONF_DEVICE_TYPE,
]

CONFIG_SCHEMA = AESGI_COMPONENT_SCHEMA.extend(
    {
        cv.Optional(CONF_OPERATION_MODE): text_sensor.text_sensor_schema(
            icon="mdi:heart-pulse"
        ),
        cv.Optional(CONF_ERRORS): text_sensor.text_sensor_schema(
            icon="mdi:alert-circle-outline"
        ),
        cv.Optional(CONF_DEVICE_TYPE): text_sensor.text_sensor_schema(icon="mdi:chip"),
    }
)


async def to_code(config):
    hub = await cg.get_variable(config[CONF_AESGI_ID])
    for key in TEXT_SENSORS:
        if key in config:
            conf = config[key]
            sens = cg.new_Pvariable(conf[CONF_ID])
            await text_sensor.register_text_sensor(sens, conf)
            cg.add(getattr(hub, f"set_{key}_text_sensor")(sens))
