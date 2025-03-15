#pragma once

#include "esphome/core/component.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include "esphome/components/aesgi_rs485/aesgi_rs485.h"

namespace esphome {
namespace aesgi {

class Aesgi : public PollingComponent, public aesgi_rs485::AesgiRs485Device {
 public:
  void set_online_status_binary_sensor(binary_sensor::BinarySensor *online_status_binary_sensor) {
    online_status_binary_sensor_ = online_status_binary_sensor;
  }

  void set_status_sensor(sensor::Sensor *status_sensor) { status_sensor_ = status_sensor; }
  void set_dc_voltage_sensor(sensor::Sensor *dc_voltage_sensor) { dc_voltage_sensor_ = dc_voltage_sensor; }
  void set_dc_current_sensor(sensor::Sensor *dc_current_sensor) { dc_current_sensor_ = dc_current_sensor; }
  void set_dc_power_sensor(sensor::Sensor *dc_power_sensor) { dc_power_sensor_ = dc_power_sensor; }
  void set_ac_voltage_sensor(sensor::Sensor *ac_voltage_sensor) { ac_voltage_sensor_ = ac_voltage_sensor; }
  void set_ac_current_sensor(sensor::Sensor *ac_current_sensor) { ac_current_sensor_ = ac_current_sensor; }
  void set_ac_power_sensor(sensor::Sensor *ac_power_sensor) { ac_power_sensor_ = ac_power_sensor; }
  void set_device_temperature_sensor(sensor::Sensor *device_temperature_sensor) {
    device_temperature_sensor_ = device_temperature_sensor;
  }
  void set_energy_today_sensor(sensor::Sensor *energy_today_sensor) { energy_today_sensor_ = energy_today_sensor; }
  void set_output_power_sensor(sensor::Sensor *output_power_sensor) { output_power_sensor_ = output_power_sensor; }
  void set_current_limit_sensor(sensor::Sensor *current_limit_sensor) { current_limit_sensor_ = current_limit_sensor; }
  void set_voltage_limit_sensor(sensor::Sensor *voltage_limit_sensor) { voltage_limit_sensor_ = voltage_limit_sensor; }
  void set_uptime_sensor(sensor::Sensor *uptime_sensor) { uptime_sensor_ = uptime_sensor; }
  void set_ac_voltage_nominal_sensor(sensor::Sensor *ac_voltage_nominal_sensor) {
    ac_voltage_nominal_sensor_ = ac_voltage_nominal_sensor;
  }
  void set_ac_frequency_nominal_sensor(sensor::Sensor *ac_frequency_nominal_sensor) {
    ac_frequency_nominal_sensor_ = ac_frequency_nominal_sensor;
  }
  void set_ac_voltage_upper_limit_sensor(sensor::Sensor *ac_voltage_upper_limit_sensor) {
    ac_voltage_upper_limit_sensor_ = ac_voltage_upper_limit_sensor;
  }
  void set_ac_voltage_upper_limit_delay_sensor(sensor::Sensor *ac_voltage_upper_limit_delay_sensor) {
    ac_voltage_upper_limit_delay_sensor_ = ac_voltage_upper_limit_delay_sensor;
  }
  void set_ac_voltage_lower_limit_sensor(sensor::Sensor *ac_voltage_lower_limit_sensor) {
    ac_voltage_lower_limit_sensor_ = ac_voltage_lower_limit_sensor;
  }
  void set_ac_voltage_lower_limit_delay_sensor(sensor::Sensor *ac_voltage_lower_limit_delay_sensor) {
    ac_voltage_lower_limit_delay_sensor_ = ac_voltage_lower_limit_delay_sensor;
  }
  void set_ac_frequency_upper_limit_sensor(sensor::Sensor *ac_frequency_upper_limit_sensor) {
    ac_frequency_upper_limit_sensor_ = ac_frequency_upper_limit_sensor;
  }
  void set_ac_frequency_upper_limit_delay_sensor(sensor::Sensor *ac_frequency_upper_limit_delay_sensor) {
    ac_frequency_upper_limit_delay_sensor_ = ac_frequency_upper_limit_delay_sensor;
  }
  void set_ac_frequency_lower_limit_sensor(sensor::Sensor *ac_frequency_lower_limit_sensor) {
    ac_frequency_lower_limit_sensor_ = ac_frequency_lower_limit_sensor;
  }
  void set_ac_frequency_lower_limit_delay_sensor(sensor::Sensor *ac_frequency_lower_limit_delay_sensor) {
    ac_frequency_lower_limit_delay_sensor_ = ac_frequency_lower_limit_delay_sensor;
  }
  void set_error_history_error_code_sensor(uint8_t slot, sensor::Sensor *error_code_sensor) {
    this->error_history_[slot].error_code_sensor_ = error_code_sensor;
  }
  void set_error_history_error_time_sensor(uint8_t slot, sensor::Sensor *error_time_sensor) {
    this->error_history_[slot].error_time_sensor_ = error_time_sensor;
  }

  void set_errors_text_sensor(text_sensor::TextSensor *errors_text_sensor) { errors_text_sensor_ = errors_text_sensor; }
  void set_operation_mode_text_sensor(text_sensor::TextSensor *operation_mode_text_sensor) {
    operation_mode_text_sensor_ = operation_mode_text_sensor;
  }
  void set_device_type_text_sensor(text_sensor::TextSensor *device_type_text_sensor) {
    device_type_text_sensor_ = device_type_text_sensor;
  }

  void dump_config() override;

  void on_aesgi_rs485_data(const std::string &data) override;

  void update() override;

 protected:
  binary_sensor::BinarySensor *online_status_binary_sensor_;

  sensor::Sensor *status_sensor_;
  sensor::Sensor *dc_voltage_sensor_;
  sensor::Sensor *dc_current_sensor_;
  sensor::Sensor *dc_power_sensor_;
  sensor::Sensor *ac_voltage_sensor_;
  sensor::Sensor *ac_current_sensor_;
  sensor::Sensor *ac_power_sensor_;
  sensor::Sensor *device_temperature_sensor_;
  sensor::Sensor *energy_today_sensor_;
  sensor::Sensor *output_power_sensor_;
  sensor::Sensor *current_limit_sensor_;
  sensor::Sensor *voltage_limit_sensor_;
  sensor::Sensor *uptime_sensor_;
  sensor::Sensor *ac_voltage_nominal_sensor_;
  sensor::Sensor *ac_frequency_nominal_sensor_;
  sensor::Sensor *ac_voltage_upper_limit_sensor_;
  sensor::Sensor *ac_voltage_upper_limit_delay_sensor_;
  sensor::Sensor *ac_voltage_lower_limit_sensor_;
  sensor::Sensor *ac_voltage_lower_limit_delay_sensor_;
  sensor::Sensor *ac_frequency_upper_limit_sensor_;
  sensor::Sensor *ac_frequency_upper_limit_delay_sensor_;
  sensor::Sensor *ac_frequency_lower_limit_sensor_;
  sensor::Sensor *ac_frequency_lower_limit_delay_sensor_;

  struct ErrorHistorySlot {
    sensor::Sensor *error_code_sensor_{nullptr};
    sensor::Sensor *error_time_sensor_{nullptr};
  } error_history_[6];

  text_sensor::TextSensor *operation_mode_text_sensor_;
  text_sensor::TextSensor *errors_text_sensor_;
  text_sensor::TextSensor *device_type_text_sensor_;

  uint8_t no_response_count_{0};
  uint8_t next_command_{7};

  void on_status_data_(const std::string &data);
  void on_device_type_data_(const std::string &data);
  void on_output_power_data_(const std::string &data);
  void on_grid_disconnect_parameters_data_(const std::string &data);
  void on_error_history_data_(const std::string &data);
  void on_current_limit_data_(const std::string &data);
  void on_operation_mode_data_(const std::string &data);

  void publish_state_(binary_sensor::BinarySensor *binary_sensor, const bool &state);
  void publish_state_(sensor::Sensor *sensor, float value);
  void publish_state_(text_sensor::TextSensor *text_sensor, const std::string &state);
  void publish_device_unavailable_();
  void reset_online_status_tracker_();
  void track_online_status_();

  bool check_bit_(uint16_t mask, uint16_t flag) { return (mask & flag) == flag; }

  float count_to_hertz_(int count) {
    if (count > 0) {
      return 1502500 / count;
    }

    return NAN;
  }
};

}  // namespace aesgi
}  // namespace esphome
