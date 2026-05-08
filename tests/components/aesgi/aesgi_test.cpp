#include <gtest/gtest.h>
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include "common.h"
#include "responses.h"

namespace esphome::aesgi::testing {

// Responses sourced from tests/esp8266-inverter-emulator.yaml

class AesgiStatusTest : public ::testing::Test {
 protected:
  TestableAesgi aesgi_;
  sensor::Sensor status_;
  sensor::Sensor dc_voltage_;
  sensor::Sensor dc_current_;
  sensor::Sensor dc_power_;
  sensor::Sensor ac_voltage_;
  sensor::Sensor ac_current_;
  sensor::Sensor ac_power_;
  sensor::Sensor device_temperature_;
  sensor::Sensor energy_today_;
  text_sensor::TextSensor errors_;

  void SetUp() override {
    aesgi_.set_status_sensor(&status_);
    aesgi_.set_dc_voltage_sensor(&dc_voltage_);
    aesgi_.set_dc_current_sensor(&dc_current_);
    aesgi_.set_dc_power_sensor(&dc_power_);
    aesgi_.set_ac_voltage_sensor(&ac_voltage_);
    aesgi_.set_ac_current_sensor(&ac_current_);
    aesgi_.set_ac_power_sensor(&ac_power_);
    aesgi_.set_device_temperature_sensor(&device_temperature_);
    aesgi_.set_energy_today_sensor(&energy_today_);
    aesgi_.set_errors_text_sensor(&errors_);
  }
};

TEST_F(AesgiStatusTest, Status) {
  aesgi_.on_status_data_(STATUS_RESPONSE);
  EXPECT_FLOAT_EQ(status_.state, 0.0f);
}

TEST_F(AesgiStatusTest, DcVoltage) {
  aesgi_.on_status_data_(STATUS_RESPONSE);
  EXPECT_NEAR(dc_voltage_.state, 20.0f, 0.05f);
}

TEST_F(AesgiStatusTest, DcCurrent) {
  aesgi_.on_status_data_(STATUS_RESPONSE);
  EXPECT_NEAR(dc_current_.state, 0.0f, 0.005f);
}

TEST_F(AesgiStatusTest, DcPower) {
  aesgi_.on_status_data_(STATUS_RESPONSE);
  EXPECT_FLOAT_EQ(dc_power_.state, 0.0f);
}

TEST_F(AesgiStatusTest, AcVoltage) {
  aesgi_.on_status_data_(STATUS_RESPONSE);
  EXPECT_NEAR(ac_voltage_.state, 235.1f, 0.05f);
}

TEST_F(AesgiStatusTest, AcCurrent) {
  aesgi_.on_status_data_(STATUS_RESPONSE);
  EXPECT_NEAR(ac_current_.state, 0.01f, 0.005f);
}

TEST_F(AesgiStatusTest, AcPower) {
  aesgi_.on_status_data_(STATUS_RESPONSE);
  EXPECT_FLOAT_EQ(ac_power_.state, 1.0f);
}

TEST_F(AesgiStatusTest, DeviceTemperature) {
  aesgi_.on_status_data_(STATUS_RESPONSE);
  EXPECT_FLOAT_EQ(device_temperature_.state, 50.0f);
}

TEST_F(AesgiStatusTest, EnergyToday) {
  aesgi_.on_status_data_(STATUS_RESPONSE);
  EXPECT_FLOAT_EQ(energy_today_.state, 44.0f);
}

TEST_F(AesgiStatusTest, ErrorsEmpty) {
  aesgi_.on_status_data_(STATUS_RESPONSE);
  EXPECT_EQ(errors_.state, "");
}

TEST_F(AesgiStatusTest, NullSensorSafe) {
  TestableAesgi bare;
  EXPECT_NO_FATAL_FAILURE(bare.on_status_data_(STATUS_RESPONSE));
}

class AesgiDeviceTypeTest : public ::testing::Test {
 protected:
  TestableAesgi aesgi_;
  text_sensor::TextSensor device_type_;

  void SetUp() override { aesgi_.set_device_type_text_sensor(&device_type_); }
};

TEST_F(AesgiDeviceTypeTest, DeviceType) {
  aesgi_.on_device_type_data_(DEVICE_TYPE_RESPONSE);
  EXPECT_EQ(device_type_.state, "PV350W");
}

TEST_F(AesgiDeviceTypeTest, NullSensorSafe) {
  TestableAesgi bare;
  EXPECT_NO_FATAL_FAILURE(bare.on_device_type_data_(DEVICE_TYPE_RESPONSE));
}

class AesgiOutputPowerThrottleTest : public ::testing::Test {
 protected:
  TestableAesgi aesgi_;
  sensor::Sensor output_power_throttle_;

  void SetUp() override { aesgi_.set_output_power_throttle_sensor(&output_power_throttle_); }
};

TEST_F(AesgiOutputPowerThrottleTest, OutputPowerThrottle) {
  aesgi_.on_output_power_throttle_data_(OUTPUT_POWER_THROTTLE_RESPONSE);
  EXPECT_FLOAT_EQ(output_power_throttle_.state, 100.0f);
}

TEST_F(AesgiOutputPowerThrottleTest, NullSensorSafe) {
  TestableAesgi bare;
  EXPECT_NO_FATAL_FAILURE(bare.on_output_power_throttle_data_(OUTPUT_POWER_THROTTLE_RESPONSE));
}

class AesgiGridDisconnectTest : public ::testing::Test {
 protected:
  TestableAesgi aesgi_;
  sensor::Sensor ac_voltage_nominal_;
  sensor::Sensor ac_frequency_nominal_;
  sensor::Sensor ac_voltage_upper_limit_;
  sensor::Sensor ac_voltage_upper_limit_delay_;
  sensor::Sensor ac_voltage_lower_limit_;
  sensor::Sensor ac_voltage_lower_limit_delay_;
  sensor::Sensor ac_frequency_upper_limit_;
  sensor::Sensor ac_frequency_upper_limit_delay_;
  sensor::Sensor ac_frequency_lower_limit_;
  sensor::Sensor ac_frequency_lower_limit_delay_;

  void SetUp() override {
    aesgi_.set_ac_voltage_nominal_sensor(&ac_voltage_nominal_);
    aesgi_.set_ac_frequency_nominal_sensor(&ac_frequency_nominal_);
    aesgi_.set_ac_voltage_upper_limit_sensor(&ac_voltage_upper_limit_);
    aesgi_.set_ac_voltage_upper_limit_delay_sensor(&ac_voltage_upper_limit_delay_);
    aesgi_.set_ac_voltage_lower_limit_sensor(&ac_voltage_lower_limit_);
    aesgi_.set_ac_voltage_lower_limit_delay_sensor(&ac_voltage_lower_limit_delay_);
    aesgi_.set_ac_frequency_upper_limit_sensor(&ac_frequency_upper_limit_);
    aesgi_.set_ac_frequency_upper_limit_delay_sensor(&ac_frequency_upper_limit_delay_);
    aesgi_.set_ac_frequency_lower_limit_sensor(&ac_frequency_lower_limit_);
    aesgi_.set_ac_frequency_lower_limit_delay_sensor(&ac_frequency_lower_limit_delay_);
  }
};

TEST_F(AesgiGridDisconnectTest, AcVoltageNominal) {
  aesgi_.on_grid_disconnect_parameters_data_(GRID_DISCONNECT_RESPONSE);
  EXPECT_NEAR(ac_voltage_nominal_.state, 230.0f, 0.05f);
}

TEST_F(AesgiGridDisconnectTest, AcFrequencyNominal) {
  aesgi_.on_grid_disconnect_parameters_data_(GRID_DISCONNECT_RESPONSE);
  EXPECT_NEAR(ac_frequency_nominal_.state, 50.0f, 0.05f);
}

TEST_F(AesgiGridDisconnectTest, AcVoltageUpperLimit) {
  aesgi_.on_grid_disconnect_parameters_data_(GRID_DISCONNECT_RESPONSE);
  EXPECT_NEAR(ac_voltage_upper_limit_.state, 264.5f, 0.05f);
}

TEST_F(AesgiGridDisconnectTest, AcVoltageUpperLimitDelay) {
  aesgi_.on_grid_disconnect_parameters_data_(GRID_DISCONNECT_RESPONSE);
  EXPECT_FLOAT_EQ(ac_voltage_upper_limit_delay_.state, 140.0f);
}

TEST_F(AesgiGridDisconnectTest, AcVoltageLowerLimit) {
  aesgi_.on_grid_disconnect_parameters_data_(GRID_DISCONNECT_RESPONSE);
  EXPECT_NEAR(ac_voltage_lower_limit_.state, 184.0f, 0.05f);
}

TEST_F(AesgiGridDisconnectTest, AcVoltageLowerLimitDelay) {
  aesgi_.on_grid_disconnect_parameters_data_(GRID_DISCONNECT_RESPONSE);
  EXPECT_FLOAT_EQ(ac_voltage_lower_limit_delay_.state, 140.0f);
}

TEST_F(AesgiGridDisconnectTest, AcFrequencyUpperLimit) {
  // count_to_hertz_(31631) = 1502500.0 / 31631 ≈ 47.5 Hz
  aesgi_.on_grid_disconnect_parameters_data_(GRID_DISCONNECT_RESPONSE);
  EXPECT_NEAR(ac_frequency_upper_limit_.state, 47.5f, 0.1f);
}

TEST_F(AesgiGridDisconnectTest, AcFrequencyUpperLimitDelay) {
  aesgi_.on_grid_disconnect_parameters_data_(GRID_DISCONNECT_RESPONSE);
  EXPECT_FLOAT_EQ(ac_frequency_upper_limit_delay_.state, 160.0f);
}

TEST_F(AesgiGridDisconnectTest, AcFrequencyLowerLimit) {
  // count_to_hertz_(29186) = 1502500.0 / 29186 ≈ 51.5 Hz
  aesgi_.on_grid_disconnect_parameters_data_(GRID_DISCONNECT_RESPONSE);
  EXPECT_NEAR(ac_frequency_lower_limit_.state, 51.5f, 0.1f);
}

TEST_F(AesgiGridDisconnectTest, AcFrequencyLowerLimitDelay) {
  aesgi_.on_grid_disconnect_parameters_data_(GRID_DISCONNECT_RESPONSE);
  EXPECT_FLOAT_EQ(ac_frequency_lower_limit_delay_.state, 160.0f);
}

TEST_F(AesgiGridDisconnectTest, NullSensorSafe) {
  TestableAesgi bare;
  EXPECT_NO_FATAL_FAILURE(bare.on_grid_disconnect_parameters_data_(GRID_DISCONNECT_RESPONSE));
}

class AesgiErrorHistoryTest : public ::testing::Test {
 protected:
  TestableAesgi aesgi_;
  sensor::Sensor uptime_;
  sensor::Sensor error_code_0_;
  sensor::Sensor error_time_0_;
  sensor::Sensor error_code_2_;
  sensor::Sensor error_time_2_;

  void SetUp() override {
    aesgi_.set_uptime_sensor(&uptime_);
    aesgi_.set_error_history_error_code_sensor(0, &error_code_0_);
    aesgi_.set_error_history_error_time_sensor(0, &error_time_0_);
    aesgi_.set_error_history_error_code_sensor(2, &error_code_2_);
    aesgi_.set_error_history_error_time_sensor(2, &error_time_2_);
  }
};

TEST_F(AesgiErrorHistoryTest, Uptime) {
  aesgi_.on_error_history_data_(ERROR_HISTORY_RESPONSE);
  EXPECT_FLOAT_EQ(uptime_.state, 7625.0f);
}

TEST_F(AesgiErrorHistoryTest, Slot0ErrorCode) {
  aesgi_.on_error_history_data_(ERROR_HISTORY_RESPONSE);
  EXPECT_FLOAT_EQ(error_code_0_.state, 7.0f);
}

TEST_F(AesgiErrorHistoryTest, Slot0ErrorTime) {
  aesgi_.on_error_history_data_(ERROR_HISTORY_RESPONSE);
  EXPECT_FLOAT_EQ(error_time_0_.state, 0.0f);
}

TEST_F(AesgiErrorHistoryTest, Slot2ErrorCode) {
  aesgi_.on_error_history_data_(ERROR_HISTORY_RESPONSE);
  EXPECT_FLOAT_EQ(error_code_2_.state, 7.0f);
}

TEST_F(AesgiErrorHistoryTest, Slot2ErrorTime) {
  aesgi_.on_error_history_data_(ERROR_HISTORY_RESPONSE);
  EXPECT_FLOAT_EQ(error_time_2_.state, 1.0f);
}

TEST_F(AesgiErrorHistoryTest, NullSensorSafe) {
  TestableAesgi bare;
  EXPECT_NO_FATAL_FAILURE(bare.on_error_history_data_(ERROR_HISTORY_RESPONSE));
}

class AesgiBatteryCurrentLimitTest : public ::testing::Test {
 protected:
  TestableAesgi aesgi_;
  sensor::Sensor battery_current_limit_;

  void SetUp() override { aesgi_.set_battery_current_limit_sensor(&battery_current_limit_); }
};

TEST_F(AesgiBatteryCurrentLimitTest, BatteryCurrentLimit) {
  // sscanf %d truncates 11.5 to 11
  aesgi_.on_battery_current_limit_data_(BATTERY_CURRENT_LIMIT_RESPONSE);
  EXPECT_FLOAT_EQ(battery_current_limit_.state, 11.0f);
}

TEST_F(AesgiBatteryCurrentLimitTest, NullSensorSafe) {
  TestableAesgi bare;
  EXPECT_NO_FATAL_FAILURE(bare.on_battery_current_limit_data_(BATTERY_CURRENT_LIMIT_RESPONSE));
}

class AesgiOperationModeTest : public ::testing::Test {
 protected:
  TestableAesgi aesgi_;
  text_sensor::TextSensor operation_mode_;
  sensor::Sensor battery_voltage_limit_;

  void SetUp() override {
    aesgi_.set_operation_mode_text_sensor(&operation_mode_);
    aesgi_.set_battery_voltage_limit_sensor(&battery_voltage_limit_);
  }
};

TEST_F(AesgiOperationModeTest, OperationModeMppt) {
  aesgi_.on_operation_mode_data_(OPERATION_MODE_RESPONSE);
  EXPECT_EQ(operation_mode_.state, "MPPT");
}

TEST_F(AesgiOperationModeTest, BatteryVoltageLimit) {
  aesgi_.on_operation_mode_data_(OPERATION_MODE_RESPONSE);
  EXPECT_NEAR(battery_voltage_limit_.state, 20.0f, 0.05f);
}

TEST_F(AesgiOperationModeTest, NullSensorSafe) {
  TestableAesgi bare;
  EXPECT_NO_FATAL_FAILURE(bare.on_operation_mode_data_(OPERATION_MODE_RESPONSE));
}

}  // namespace esphome::aesgi::testing
