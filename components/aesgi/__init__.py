import esphome.codegen as cg
from esphome.components import uart
import esphome.config_validation as cv
from esphome.const import CONF_ID

DEPENDENCIES = ["uart"]
CODEOWNERS = ["@syssi"]
AUTO_LOAD = ["binary_sensor", "text_sensor", "sensor"]
MULTI_CONF = True

CONF_AESGI_ID = "aesgi_id"

aesgi_ns = cg.esphome_ns.namespace("aesgi")
AesgiComponent = aesgi_ns.class_("Aesgi", cg.Component)

AESGI_COMPONENT_SCHEMA = cv.Schema(
    {
        cv.Required(CONF_AESGI_ID): cv.use_id(AesgiComponent),
    }
)

CONFIG_SCHEMA = cv.All(
    cv.Schema({cv.GenerateID(): cv.declare_id(AesgiComponent)})
    .extend(cv.polling_component_schema("1s"))
    .extend(uart.UART_DEVICE_SCHEMA)
)


def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)
    yield uart.register_uart_device(var, config)
