#pragma once
#include <string>

namespace esphome::aesgi::testing {

// Responses sourced from tests/esp8266-inverter-emulator.yaml

// *290   0  20.0  0.00     0 235.1  0.01     1  50     44 \xD9\r
const std::string STATUS_RESPONSE = "*290   0  20.0  0.00     0 235.1  0.01     1  50     44 \xD9\r";

// *299 PV350W \xA3\r
const std::string DEVICE_TYPE_RESPONSE = "*299 PV350W \xA3\r";

// *29L 100 \xB2\r
const std::string OUTPUT_POWER_THROTTLE_RESPONSE = "*29L 100 \xB2\r";

// *29P 230.0 50.0 264.5 0140 184.0 0140 31631 0160 29186 0160 \x15\r
const std::string GRID_DISCONNECT_RESPONSE = "*29P 230.0 50.0 264.5 0140 184.0 0140 31631 0160 29186 0160 \x15\r";

// *29F 07625 007 00000 006 00000 007 00001 025 00001 025 00002 025 00003 \xCF\r
const std::string ERROR_HISTORY_RESPONSE =
    "*29F 07625 007 00000 006 00000 007 00001 025 00001 025 00002 025 00003 \xCF\r";

// *29S 11.5 \xED\r  (sscanf %d reads 11)
const std::string BATTERY_CURRENT_LIMIT_RESPONSE = "*29S 11.5 \xED\r";

// *29B 0 20.0 '\r
const std::string OPERATION_MODE_RESPONSE = "*29B 0 20.0 '\r";

}  // namespace esphome::aesgi::testing
