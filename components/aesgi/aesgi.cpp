#include "aesgi.h"
#include "esphome/core/log.h"
#include "esphome/core/helpers.h"

namespace esphome {
namespace aesgi {

static const char *const TAG = "aesgi";

static const uint8_t MAX_NO_RESPONSE_COUNT = 5;

static const uint8_t AESGI_COMMAND_STATUS = '0';
static const uint8_t AESGI_COMMAND_DEVICE_TYPE = = '9';
static const uint8_t AESGI_COMMAND_OUTPUT_POWER = 'L';
static const uint8_t AESGI_COMMAND_AUTO_TEST = 'A';
static const uint8_t AESGI_COMMAND_SETTINGS = 'P';
static const uint8_t AESGI_COMMAND_ERRORS = 'F';
static const uint8_t AESGI_COMMAND_CURRENT_LIMIT = 'S';
static const uint8_t AESGI_COMMAND_OPERATION_MODE = 'B';

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
    case AESGI_COMMAND_OUTPUT_POWER:
      this->on_output_power_data_(data);
      break;
    case AESGI_COMMAND_AUTO_TEST:
      ESP_LOGI(TAG, "Auto test response (%zu bytes) received: %s", data.size(), data.c_str());
      break;
    case AESGI_COMMAND_SETTINGS:
      this->on_settings_data_(data);
      break;
    case AESGI_COMMAND_ERRORS:
      this->on_errors_data_(data);
      break;
    case AESGI_COMMAND_CURRENT_LIMIT:
      this->on_current_limit_data_(data);
      break;
    case AESGI_COMMAND_OPERATION_MODE:
      this->on_operation_mode_data_(data);
      break;

    default:
      ESP_LOGW(TAG, "Unhandled response (%zu bytes) received: %s", data.size(), data.c_str());
  }
}

void Aesgi::on_status_data_(const std::string &data) {
  if (data.size() < 58) {
    ESP_LOGW(TAG, "Status frame too short. Skipping");
    return;
  }

  ESP_LOGI(TAG, "Status frame received (%zu bytes)", data.size());

  int skipped;
  int status;
  float dc_voltage;
  float dc_current;
  int dc_power;
  float ac_voltage;
  float ac_current;
  int ac_power;
  int device_temperature;
  int energy_today;

  // *290   0  20.0  0.00     0 235.1  0.01     1  50     44 \xD9\r
  int ret =
      sscanf(data.c_str(), "*%d %d %f %f %d %f %f %d %d %d", &skipped, &status, &dc_voltage, &dc_current,  // NOLINT
             &dc_power, &ac_voltage, &ac_current, &ac_power, &device_temperature, &energy_today);

  if (ret != 10) {
    ESP_LOGE(TAG, "Parsing status response failed: %s", data.c_str());
    return;
  }

  this->publish_state_(this->status_sensor_, status);
  this->publish_state_(this->dc_voltage_sensor_, dc_voltage);
  this->publish_state_(this->dc_current_sensor_, dc_current);
  this->publish_state_(this->dc_power_sensor_, dc_power);
  this->publish_state_(this->ac_voltage_sensor_, ac_voltage);
  this->publish_state_(this->ac_current_sensor_, ac_current);
  this->publish_state_(this->ac_power_sensor_, ac_power);
  this->publish_state_(this->device_temperature_sensor_, device_temperature);
  this->publish_state_(this->energy_today_sensor_, energy_today);
}

void Aesgi::on_device_type_data_(const std::string &data) {}
void Aesgi::on_output_power_data_(const std::string &data) {}
void Aesgi::on_settings_data_(const std::string &data) {}
void Aesgi::on_errors_data_(const std::string &data) {}
void Aesgi::on_current_limit_data_(const std::string &data) {}
void Aesgi::on_operation_mode_data_(const std::string &data) {}

void Aesgi::update() {
  this->track_online_status_();
  this->send('0');
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

  this->publish_state_(status_sensor_, NAN);
  this->publish_state_(dc_voltage_sensor_, NAN);
  this->publish_state_(dc_current_sensor_, NAN);
  this->publish_state_(dc_power_sensor_, NAN);
  this->publish_state_(ac_voltage_sensor_, NAN);
  this->publish_state_(ac_current_sensor_, NAN);
  this->publish_state_(ac_power_sensor_, NAN);
  this->publish_state_(device_temperature_sensor_, NAN);
  this->publish_state_(energy_today_sensor_, NAN);
}

void Aesgi::publish_state_(binary_sensor::BinarySensor *binary_sensor, const bool &state) {
  if (binary_sensor == nullptr)
    return;

  binary_sensor->publish_state(state);
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

  LOG_SENSOR("", "Status", this->status_sensor_);
  LOG_SENSOR("", "DC voltage", this->dc_voltage_sensor_);
  LOG_SENSOR("", "DC current", this->dc_current_sensor_);
  LOG_SENSOR("", "DC power", this->dc_power_sensor_);
  LOG_SENSOR("", "AC voltage", this->ac_voltage_sensor_);
  LOG_SENSOR("", "AC current", this->ac_current_sensor_);
  LOG_SENSOR("", "AC power", this->ac_power_sensor_);
  LOG_SENSOR("", "Device temperature", this->device_temperature_sensor_);
  LOG_SENSOR("", "Energy today", this->energy_today_sensor_);

  LOG_TEXT_SENSOR("", "Operation mode", this->operation_mode_text_sensor_);
  LOG_TEXT_SENSOR("", "Errors", this->errors_text_sensor_);
  LOG_TEXT_SENSOR("", "Device Type", this->device_type_text_sensor_);
}

}  // namespace aesgi
}  // namespace esphome
