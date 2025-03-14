import esphome.codegen as cg
from esphome.components import uart
import esphome.config_validation as cv
from esphome.const import CONF_ADDRESS, CONF_ID

DEPENDENCIES = ["uart"]
MULTI_CONF = True

aesgi_rs485_ns = cg.esphome_ns.namespace("aesgi_rs485")
AesgiRs485 = aesgi_rs485_ns.class_("AesgiRs485", cg.Component, uart.UARTDevice)
AesgiRs485Device = aesgi_rs485_ns.class_("AesgiRs485Device")

CONF_AESGI_RS485_ID = "aesgi_rs485_id"
CONF_RX_TIMEOUT = "rx_timeout"

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(AesgiRs485),
            cv.Optional(
                CONF_RX_TIMEOUT, default="50ms"
            ): cv.positive_time_period_milliseconds,
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
    .extend(uart.UART_DEVICE_SCHEMA)
)


async def to_code(config):
    cg.add_global(aesgi_rs485_ns.using)
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    await uart.register_uart_device(var, config)

    cg.add(var.set_rx_timeout(config[CONF_RX_TIMEOUT]))


def aesgi_rs485_device_schema(default_address):
    schema = {
        cv.GenerateID(CONF_AESGI_RS485_ID): cv.use_id(AesgiRs485),
    }
    if default_address is None:
        schema[cv.Required(CONF_ADDRESS)] = cv.int_range(min=1, max=32)
    else:
        schema[cv.Optional(CONF_ADDRESS, default=default_address)] = cv.int_range(
            min=1, max=32
        )
    return cv.Schema(schema)


async def register_aesgi_rs485_device(var, config):
    parent = await cg.get_variable(config[CONF_AESGI_RS485_ID])
    cg.add(var.set_parent(parent))
    cg.add(var.set_address(config[CONF_ADDRESS]))
    cg.add(parent.register_device(var))
