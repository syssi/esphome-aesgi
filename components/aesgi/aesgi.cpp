#include "aesgi.h"
#include "esphome/core/log.h"
#include "esphome/core/helpers.h"

namespace esphome::aesgi {

static const char *const TAG = "aesgi";

static const uint8_t MAX_NO_RESPONSE_COUNT = 5;

static const uint8_t AESGI_COMMAND_STATUS = '0';
static const uint8_t AESGI_COMMAND_DEVICE_TYPE = '9';
static const uint8_t AESGI_COMMAND_OUTPUT_POWER_THROTTLE = 'L';
static const uint8_t AESGI_COMMAND_AUTO_TEST = 'A';
static const uint8_t AESGI_COMMAND_GRID_DISCONNECT_PARAMETERS = 'P';
static const uint8_t AESGI_COMMAND_ERROR_HISTORY = 'F';
static const uint8_t AESGI_COMMAND_BATTERY_CURRENT_LIMIT = 'S';
static const uint8_t AESGI_COMMAND_OPERATION_MODE = 'B';

static const uint8_t AESGI_COMMAND_QUEUE_SIZE = 7;
static const uint8_t AESGI_COMMAND_QUEUE[AESGI_COMMAND_QUEUE_SIZE] = {
    AESGI_COMMAND_STATUS,
    AESGI_COMMAND_DEVICE_TYPE,
    AESGI_COMMAND_OUTPUT_POWER_THROTTLE,
    AESGI_COMMAND_GRID_DISCONNECT_PARAMETERS,
    AESGI_COMMAND_ERROR_HISTORY,
    AESGI_COMMAND_BATTERY_CURRENT_LIMIT,
    AESGI_COMMAND_OPERATION_MODE,
};

static std::string read_field(const std::string &data, size_t *pos) {
  while (*pos < data.size() && data[*pos] == ' ')
    (*pos)++;
  const size_t begin = *pos;
  while (*pos < data.size() && data[*pos] != ' ')
    (*pos)++;
  return data.substr(begin, *pos - begin);
}

static void skip_field(const std::string &data, size_t *pos) {
  while (*pos < data.size() && data[*pos] == ' ')
    (*pos)++;
  while (*pos < data.size() && data[*pos] != ' ')
    (*pos)++;
}

void Aesgi::on_aesgi_rs485_data(const std::string &data) {
  this->reset_online_status_tracker_();

  uint8_t command = data[3];
  switch (command) {
    case AESGI_COMMAND_STATUS:
      this->on_status_data_(data);
      break;
    case AESGI_COMMAND_DEVICE_TYPE:
      this->on_device_type_data_(data);
      break;
    case AESGI_COMMAND_OUTPUT_POWER_THROTTLE:
      this->on_output_power_throttle_data_(data);
      break;
    case AESGI_COMMAND_AUTO_TEST:
      this->on_auto_test_data_(data);
      break;
    case AESGI_COMMAND_GRID_DISCONNECT_PARAMETERS:
      this->on_grid_disconnect_parameters_data_(data);
      break;
    case AESGI_COMMAND_ERROR_HISTORY:
      this->on_error_history_data_(data);
      break;
    case AESGI_COMMAND_BATTERY_CURRENT_LIMIT:
      this->on_battery_current_limit_data_(data);
      break;
    case AESGI_COMMAND_OPERATION_MODE:
      this->on_operation_mode_data_(data);
      break;

    default:
      ESP_LOGW(TAG, "Unhandled response (%zu bytes) received: %s", data.size(), data.c_str());
  }

  // Send next command after each received frame
  if (this->next_command_ < AESGI_COMMAND_QUEUE_SIZE) {
    this->send(AESGI_COMMAND_QUEUE[this->next_command_++ % AESGI_COMMAND_QUEUE_SIZE]);
  }
}

void Aesgi::on_auto_test_data_(const std::string &data) {
  ESP_LOGI(TAG, "Auto test frame received (%zu bytes)", data.size());

  size_t pos = 1;
  skip_field(data, &pos);  // address+command
  for (int i = 0; i < 10; i++)
    skip_field(data, &pos);
  auto result = parse_number<int>(read_field(data, &pos));

  if (!result.has_value()) {
    ESP_LOGE(TAG, "Parsing auto test response failed: %s", data.c_str());
    return;
  }

  this->publish_state_(this->auto_test_result_sensor_, result.value());
}

void Aesgi::on_status_data_(const std::string &data) {
  ESP_LOGI(TAG, "Status frame received (%zu bytes)", data.size());

  size_t pos = 1;
  skip_field(data, &pos);  // address+command

  // *290   0  20.0  0.00     0 235.1  0.01     1  50     44 \xD9\r
  auto status = parse_number<int>(read_field(data, &pos));
  auto dc_voltage = parse_number<float>(read_field(data, &pos));
  auto dc_current = parse_number<float>(read_field(data, &pos));
  auto dc_power = parse_number<int>(read_field(data, &pos));
  auto ac_voltage = parse_number<float>(read_field(data, &pos));
  auto ac_current = parse_number<float>(read_field(data, &pos));
  auto ac_power = parse_number<int>(read_field(data, &pos));
  auto device_temperature = parse_number<int>(read_field(data, &pos));
  auto energy_today = parse_number<int>(read_field(data, &pos));

  if (!status.has_value() || !dc_voltage.has_value() || !dc_current.has_value() || !dc_power.has_value() ||
      !ac_voltage.has_value() || !ac_current.has_value() || !ac_power.has_value() || !device_temperature.has_value() ||
      !energy_today.has_value()) {
    ESP_LOGE(TAG, "Parsing status response failed: %s", data.c_str());
    return;
  }

  this->publish_state_(this->status_sensor_, status.value());
  this->publish_state_(this->errors_text_sensor_, "");
  this->publish_state_(this->dc_voltage_sensor_, dc_voltage.value());
  this->publish_state_(this->dc_current_sensor_, dc_current.value());
  this->publish_state_(this->dc_power_sensor_, dc_power.value());
  this->publish_state_(this->ac_voltage_sensor_, ac_voltage.value());
  this->publish_state_(this->ac_current_sensor_, ac_current.value());
  this->publish_state_(this->ac_power_sensor_, ac_power.value());
  this->publish_state_(this->device_temperature_sensor_, device_temperature.value());
  this->publish_state_(this->energy_today_sensor_, energy_today.value());
}

void Aesgi::on_device_type_data_(const std::string &data) {
  ESP_LOGI(TAG, "Device type frame received (%zu bytes)", data.size());

  size_t pos = 1;
  skip_field(data, &pos);  // address+command

  // *299 PV350W \xA3\r
  const std::string device_type = read_field(data, &pos);

  if (device_type.empty()) {
    ESP_LOGE(TAG, "Parsing device type frame response failed: %s", data.c_str());
    return;
  }

  this->publish_state_(this->device_type_text_sensor_, device_type);
}

void Aesgi::on_output_power_throttle_data_(const std::string &data) {
  ESP_LOGI(TAG, "Output power throttle frame received (%zu bytes)", data.size());

  size_t pos = 1;
  skip_field(data, &pos);  // address+command

  // *29L 100 \xB2\r
  auto output_power = parse_number<int>(read_field(data, &pos));

  if (!output_power.has_value()) {
    ESP_LOGE(TAG, "Parsing output power throttle response failed: %s", data.c_str());
    return;
  }

  this->publish_state_(this->output_power_throttle_sensor_, output_power.value());
  this->publish_state_(this->output_power_throttle_number_, output_power.value());
  this->publish_state_(this->output_power_throttle_broadcast_number_, output_power.value());
}

void Aesgi::on_grid_disconnect_parameters_data_(const std::string &data) {
  ESP_LOGI(TAG, "Grid disconnect parameters frame received (%zu bytes)", data.size());

  size_t pos = 1;
  skip_field(data, &pos);  // address+command

  // *29P 230.0 50.0 264.5 0140 184.0 0140 31631 0160 29186 0160 \x15\r
  auto ac_voltage_nominal = parse_number<float>(read_field(data, &pos));
  auto ac_frequency_nominal = parse_number<float>(read_field(data, &pos));
  auto ac_voltage_upper_limit = parse_number<float>(read_field(data, &pos));
  auto ac_voltage_upper_limit_delay = parse_number<float>(read_field(data, &pos));
  auto ac_voltage_lower_limit = parse_number<float>(read_field(data, &pos));
  auto ac_voltage_lower_limit_delay = parse_number<float>(read_field(data, &pos));
  auto ac_frequency_upper_limit = parse_number<int>(read_field(data, &pos));
  auto ac_frequency_upper_limit_delay = parse_number<int>(read_field(data, &pos));
  auto ac_frequency_lower_limit = parse_number<int>(read_field(data, &pos));
  auto ac_frequency_lower_limit_delay = parse_number<int>(read_field(data, &pos));

  if (!ac_voltage_nominal.has_value() || !ac_frequency_nominal.has_value() || !ac_voltage_upper_limit.has_value() ||
      !ac_voltage_upper_limit_delay.has_value() || !ac_voltage_lower_limit.has_value() ||
      !ac_voltage_lower_limit_delay.has_value() || !ac_frequency_upper_limit.has_value() ||
      !ac_frequency_upper_limit_delay.has_value() || !ac_frequency_lower_limit.has_value() ||
      !ac_frequency_lower_limit_delay.has_value()) {
    ESP_LOGE(TAG, "Parsing grid disconnect parameters response failed: %s", data.c_str());
    return;
  }

  this->publish_state_(this->ac_voltage_nominal_sensor_, ac_voltage_nominal.value());
  this->publish_state_(this->ac_frequency_nominal_sensor_, ac_frequency_nominal.value());
  this->publish_state_(this->ac_voltage_upper_limit_sensor_, ac_voltage_upper_limit.value());
  this->publish_state_(this->ac_voltage_upper_limit_delay_sensor_, ac_voltage_upper_limit_delay.value());
  this->publish_state_(this->ac_voltage_lower_limit_sensor_, ac_voltage_lower_limit.value());
  this->publish_state_(this->ac_voltage_lower_limit_delay_sensor_, ac_voltage_lower_limit_delay.value());
  this->publish_state_(this->ac_frequency_upper_limit_sensor_, count_to_hertz_(ac_frequency_upper_limit.value()));
  this->publish_state_(this->ac_frequency_upper_limit_delay_sensor_, ac_frequency_upper_limit_delay.value());
  this->publish_state_(this->ac_frequency_lower_limit_sensor_, count_to_hertz_(ac_frequency_lower_limit.value()));
  this->publish_state_(this->ac_frequency_lower_limit_delay_sensor_, ac_frequency_lower_limit_delay.value());
}

void Aesgi::on_error_history_data_(const std::string &data) {
  ESP_LOGI(TAG, "Error history frame received (%zu bytes)", data.size());

  size_t pos = 1;
  skip_field(data, &pos);  // address+command

  // *29F 07625 007 00000 006 00000 007 00001 025 00001 025 00002 025 00003 \xCF\r
  auto uptime = parse_number<int>(read_field(data, &pos));
  if (!uptime.has_value()) {
    ESP_LOGE(TAG, "Parsing error history response failed: %s", data.c_str());
    return;
  }

  int error_codes[6];
  int error_times[6];
  for (int i = 0; i < 6; i++) {
    auto code = parse_number<int>(read_field(data, &pos));
    auto time = parse_number<int>(read_field(data, &pos));
    if (!code.has_value() || !time.has_value()) {
      ESP_LOGE(TAG, "Parsing error history response failed: %s", data.c_str());
      return;
    }
    error_codes[i] = code.value();
    error_times[i] = time.value();
  }

  this->publish_state_(this->uptime_sensor_, uptime.value());
  for (int i = 0; i < 6; i++) {
    this->publish_state_(this->error_history_[i].error_code_sensor_, error_codes[i]);
    this->publish_state_(this->error_history_[i].error_time_sensor_, error_times[i]);
  }
}

void Aesgi::on_battery_current_limit_data_(const std::string &data) {
  ESP_LOGI(TAG, "Battery current limit frame received (%zu bytes)", data.size());

  size_t pos = 1;
  skip_field(data, &pos);  // address+command

  // *29S 11.5 \xED\r
  auto current_limit = parse_number<float>(read_field(data, &pos));

  if (!current_limit.has_value()) {
    ESP_LOGE(TAG, "Parsing battery current limit response failed: %s", data.c_str());
    return;
  }

  this->publish_state_(this->battery_current_limit_sensor_, current_limit.value());
  this->publish_state_(this->battery_current_limit_number_, current_limit.value());
}

void Aesgi::on_operation_mode_data_(const std::string &data) {
  ESP_LOGI(TAG, "Operation mode frame received (%zu bytes)", data.size());

  size_t pos = 1;
  skip_field(data, &pos);  // address+command

  // *29B 0 20.0 '\r
  auto operation_mode = parse_number<int>(read_field(data, &pos));
  auto voltage_limit = parse_number<float>(read_field(data, &pos));

  if (!operation_mode.has_value() || !voltage_limit.has_value()) {
    ESP_LOGE(TAG, "Parsing operation mode response failed: %s", data.c_str());
    return;
  }

  this->publish_state_(this->operation_mode_text_sensor_, operation_mode.value() == 0   ? "MPPT"
                                                          : operation_mode.value() == 2 ? "Battery"
                                                                                        : "Unknown");
  this->publish_state_(this->battery_voltage_limit_sensor_, voltage_limit.value());
  this->publish_state_(this->battery_voltage_limit_number_, voltage_limit.value());
}

void Aesgi::update() {
  this->track_online_status_();

  // Loop through all commands if connected
  if (this->next_command_ != AESGI_COMMAND_QUEUE_SIZE && this->no_response_count_ < MAX_NO_RESPONSE_COUNT) {
    ESP_LOGW(TAG,
             "Command queue (%d of %d) was not completely processed. "
             "Please increase the update_interval if you see this warning frequently",
             this->next_command_ + 1, AESGI_COMMAND_QUEUE_SIZE);
  }
  this->next_command_ = 0;

  this->send(AESGI_COMMAND_QUEUE[this->next_command_++ % AESGI_COMMAND_QUEUE_SIZE]);
}

void Aesgi::track_online_status_() {
  if (this->no_response_count_ < MAX_NO_RESPONSE_COUNT) {
    this->no_response_count_++;
  }
  if (this->no_response_count_ == MAX_NO_RESPONSE_COUNT) {
    this->publish_device_unavailable_();
    this->no_response_count_++;
  }
}

void Aesgi::reset_online_status_tracker_() {
  this->no_response_count_ = 0;
  this->publish_state_(this->online_status_binary_sensor_, true);
}

void Aesgi::publish_device_unavailable_() {
  this->publish_state_(this->online_status_binary_sensor_, false);
  this->publish_state_(this->errors_text_sensor_, "Offline");
  this->publish_state_(this->operation_mode_text_sensor_, "Unknown");
  this->publish_state_(this->device_type_text_sensor_, "Unknown");

  this->publish_state_(this->auto_test_result_sensor_, NAN);
  this->publish_state_(this->status_sensor_, NAN);
  this->publish_state_(this->dc_voltage_sensor_, NAN);
  this->publish_state_(this->dc_current_sensor_, NAN);
  this->publish_state_(this->dc_power_sensor_, NAN);
  this->publish_state_(this->ac_voltage_sensor_, NAN);
  this->publish_state_(this->ac_current_sensor_, NAN);
  this->publish_state_(this->ac_power_sensor_, NAN);
  this->publish_state_(this->device_temperature_sensor_, NAN);
  this->publish_state_(this->energy_today_sensor_, NAN);
  this->publish_state_(this->output_power_throttle_sensor_, NAN);
  this->publish_state_(this->battery_current_limit_sensor_, NAN);
  this->publish_state_(this->battery_voltage_limit_sensor_, NAN);
  this->publish_state_(this->uptime_sensor_, NAN);
  this->publish_state_(this->ac_voltage_nominal_sensor_, NAN);
  this->publish_state_(this->ac_frequency_nominal_sensor_, NAN);
  this->publish_state_(this->ac_voltage_upper_limit_sensor_, NAN);
  this->publish_state_(this->ac_voltage_upper_limit_delay_sensor_, NAN);
  this->publish_state_(this->ac_voltage_lower_limit_sensor_, NAN);
  this->publish_state_(this->ac_voltage_lower_limit_delay_sensor_, NAN);
  this->publish_state_(this->ac_frequency_upper_limit_sensor_, NAN);
  this->publish_state_(this->ac_frequency_upper_limit_delay_sensor_, NAN);
  this->publish_state_(this->ac_frequency_lower_limit_sensor_, NAN);
  this->publish_state_(this->ac_frequency_lower_limit_delay_sensor_, NAN);

  for (auto &slot : this->error_history_) {
    this->publish_state_(slot.error_code_sensor_, NAN);
    this->publish_state_(slot.error_time_sensor_, NAN);
  }
}

void Aesgi::publish_state_(binary_sensor::BinarySensor *binary_sensor, const bool &state) {
  if (binary_sensor == nullptr)
    return;

  binary_sensor->publish_state(state);
}

void Aesgi::publish_state_(number::Number *number, float value) {
  if (number == nullptr)
    return;

  number->publish_state(value);
}

void Aesgi::publish_state_(sensor::Sensor *sensor, float value) {
  if (sensor == nullptr)
    return;

  sensor->publish_state(value);
}

void Aesgi::publish_state_(text_sensor::TextSensor *text_sensor, const std::string &state) {
  if (text_sensor == nullptr)
    return;

  text_sensor->publish_state(state);
}

void Aesgi::dump_config() {  // NOLINT(google-readability-function-size,readability-function-size)
  ESP_LOGCONFIG(TAG, "Aesgi:");
  ESP_LOGCONFIG(TAG, "  Address: 0x%02X", this->address_);
  LOG_BINARY_SENSOR("", "Online Status", this->online_status_binary_sensor_);

  LOG_SENSOR("", "Auto test result", this->auto_test_result_sensor_);
  LOG_SENSOR("", "Status", this->status_sensor_);
  LOG_SENSOR("", "DC voltage", this->dc_voltage_sensor_);
  LOG_SENSOR("", "DC current", this->dc_current_sensor_);
  LOG_SENSOR("", "DC power", this->dc_power_sensor_);
  LOG_SENSOR("", "AC voltage", this->ac_voltage_sensor_);
  LOG_SENSOR("", "AC current", this->ac_current_sensor_);
  LOG_SENSOR("", "AC power", this->ac_power_sensor_);
  LOG_SENSOR("", "Device temperature", this->device_temperature_sensor_);
  LOG_SENSOR("", "Energy today", this->energy_today_sensor_);
  LOG_SENSOR("", "Output power", this->output_power_throttle_sensor_);
  LOG_SENSOR("", "Battery current limit", this->battery_current_limit_sensor_);
  LOG_SENSOR("", "Battery voltage limit", this->battery_voltage_limit_sensor_);
  LOG_SENSOR("", "Uptime", this->uptime_sensor_);
  LOG_SENSOR("", "AC voltage nominal", this->ac_voltage_nominal_sensor_);
  LOG_SENSOR("", "AC frequency nominal", this->ac_frequency_nominal_sensor_);
  LOG_SENSOR("", "AC voltage upper limit", this->ac_voltage_upper_limit_sensor_);
  LOG_SENSOR("", "AC voltage upper limit delay", this->ac_voltage_upper_limit_delay_sensor_);
  LOG_SENSOR("", "AC voltage lower limit", this->ac_voltage_lower_limit_sensor_);
  LOG_SENSOR("", "AC voltage lower limit delay", this->ac_voltage_lower_limit_delay_sensor_);
  LOG_SENSOR("", "AC frequency upper limit", this->ac_frequency_upper_limit_sensor_);
  LOG_SENSOR("", "AC frequency upper limit delay", this->ac_frequency_upper_limit_delay_sensor_);
  LOG_SENSOR("", "AC frequency lower limit", this->ac_frequency_lower_limit_sensor_);
  LOG_SENSOR("", "AC frequency lower limit delay", this->ac_frequency_lower_limit_delay_sensor_);

  LOG_TEXT_SENSOR("", "Operation mode", this->operation_mode_text_sensor_);
  LOG_TEXT_SENSOR("", "Errors", this->errors_text_sensor_);
  LOG_TEXT_SENSOR("", "Device Type", this->device_type_text_sensor_);
}

}  // namespace esphome::aesgi
