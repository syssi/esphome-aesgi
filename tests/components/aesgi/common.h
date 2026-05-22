#pragma once
#include <string>
#include "esphome/components/aesgi/aesgi.h"

namespace esphome::aesgi::testing {

class TestableAesgi : public Aesgi {
 public:
  using Aesgi::on_status_data_;
  using Aesgi::on_device_type_data_;
  using Aesgi::on_output_power_throttle_data_;
  using Aesgi::on_grid_disconnect_parameters_data_;
  using Aesgi::on_error_history_data_;
  using Aesgi::on_battery_current_limit_data_;
  using Aesgi::on_auto_test_data_;
  using Aesgi::on_operation_mode_data_;
};

}  // namespace esphome::aesgi::testing
