#pragma once

#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"

namespace esphome {
namespace aesgi_rs485 {

class AesgiRs485Device;

class AesgiRs485 : public uart::UARTDevice, public Component {
 public:
  AesgiRs485() = default;

  void loop() override;

  void dump_config() override;

  void register_device(AesgiRs485Device *device) { this->devices_.push_back(device); }

  float get_setup_priority() const override;

  void send(uint8_t address, uint8_t command);
  void set_rx_timeout(uint16_t rx_timeout) { rx_timeout_ = rx_timeout; }

 protected:
  bool parse_aesgi_rs485_byte_(uint8_t byte);

  std::vector<uint8_t> rx_buffer_;
  uint16_t rx_timeout_{50};
  uint32_t last_aesgi_rs485_byte_{0};
  std::vector<AesgiRs485Device *> devices_;
};

class AesgiRs485Device {
 public:
  void set_parent(AesgiRs485 *parent) { parent_ = parent; }
  void set_address(uint8_t address) { address_ = address; }
  virtual void on_aesgi_rs485_data(const std::string &data) = 0;

  void send(uint8_t command) { this->parent_->send(address_, command); }

 protected:
  friend AesgiRs485;

  AesgiRs485 *parent_;
  uint8_t address_;
};

}  // namespace aesgi_rs485
}  // namespace esphome
