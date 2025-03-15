import esphome.codegen as cg
from esphome.components import aesgi_rs485
import esphome.config_validation as cv
from esphome.const import CONF_ID

AUTO_LOAD = ["aesgi_rs485", "binary_sensor", "number", "sensor", "text_sensor"]
CODEOWNERS = ["@syssi"]
MULTI_CONF = True

CONF_AESGI_ID = "aesgi_id"

aesgi_ns = cg.esphome_ns.namespace("aesgi")
Aesgi = aesgi_ns.class_("Aesgi", cg.PollingComponent, aesgi_rs485.AesgiRs485Device)

AESGI_COMPONENT_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_AESGI_ID): cv.use_id(Aesgi),
    }
)

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(Aesgi),
        }
    )
    .extend(cv.polling_component_schema("5s"))
    .extend(aesgi_rs485.aesgi_rs485_device_schema(1))
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await aesgi_rs485.register_aesgi_rs485_device(var, config)
