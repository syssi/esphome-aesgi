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

  void send(uint8_t address, uint8_t command, const std::string &value = "");
  void send_broadcast(uint8_t command, const std::string &value = "");
  void set_rx_timeout(uint16_t rx_timeout) { rx_timeout_ = rx_timeout; }

 protected:
  bool parse_aesgi_rs485_byte_(uint8_t byte);
  bool verify_checksum_(const uint8_t computed_crc, const uint8_t remote_crc) {
    // *03P 230.0 50.0 264.5 0140 184.0 0140 31631 0160 29186 0160 \r\r
    if (remote_crc == 0x20 && ((computed_crc + 0x20) & 0xFF) == '\r') {
      return true;
    }

    return computed_crc == remote_crc;
  }

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

  void send(uint8_t command, const std::string &value = "") { this->parent_->send(address_, command, value); }
  void send_broadcast(uint8_t command, const std::string &value = "") { this->parent_->send_broadcast(command, value); }

 protected:
  friend AesgiRs485;

  AesgiRs485 *parent_;
  uint8_t address_;
};

}  // namespace aesgi_rs485
}  // namespace esphome
