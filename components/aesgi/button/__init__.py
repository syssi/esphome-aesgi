import esphome.codegen as cg
from esphome.components import button
import esphome.config_validation as cv
from esphome.const import CONF_ICON, CONF_ID

from .. import AESGI_COMPONENT_SCHEMA, CONF_AESGI_ID, aesgi_ns

DEPENDENCIES = ["aesgi"]

CODEOWNERS = ["@syssi"]

CONF_AUTO_TEST = "auto_test"
CONF_OPERATION_MODE_PV = "operation_mode_pv"

BUTTONS = {
    CONF_AUTO_TEST: ord("A"),
    CONF_OPERATION_MODE_PV: ord("B"),
}

AesgiButton = aesgi_ns.class_("AesgiButton", button.Button, cg.Component)

CONFIG_SCHEMA = AESGI_COMPONENT_SCHEMA.extend(
    {
        cv.Optional(CONF_AUTO_TEST): button.BUTTON_SCHEMA.extend(
            {
                cv.GenerateID(): cv.declare_id(AesgiButton),
                cv.Optional(CONF_ICON, default="mdi:check-circle-outline"): cv.icon,
            }
        ).extend(cv.COMPONENT_SCHEMA),
        cv.Optional(CONF_OPERATION_MODE_PV): button.BUTTON_SCHEMA.extend(
            {
                cv.GenerateID(): cv.declare_id(AesgiButton),
                cv.Optional(CONF_ICON, default="mdi:solar-power"): cv.icon,
            }
        ).extend(cv.COMPONENT_SCHEMA),
    }
)


async def to_code(config):
    hub = await cg.get_variable(config[CONF_AESGI_ID])
    for key, address in BUTTONS.items():
        if key in config:
            conf = config[key]
            var = cg.new_Pvariable(conf[CONF_ID])
            await cg.register_component(var, conf)
            await button.register_button(var, conf)
            cg.add(var.set_parent(hub))
            cg.add(var.set_holding_register(address))
