#include "aesgi_rs485.h"
#include "esphome/core/log.h"
#include "esphome/core/hal.h"
#include "esphome/core/helpers.h"

namespace esphome {
namespace aesgi_rs485 {

static const char *const TAG = "aesgi_rs485";

void AesgiRs485::loop() {
  const uint32_t now = millis();
  if (now - this->last_aesgi_rs485_byte_ > this->rx_timeout_) {
    ESP_LOGVV(TAG, "Buffer cleared due to timeout: %s",
              format_hex_pretty(&this->rx_buffer_.front(), this->rx_buffer_.size()).c_str());  // NOLINT
    this->rx_buffer_.clear();
    this->last_aesgi_rs485_byte_ = now;
  }

  while (this->available()) {
    uint8_t byte;
    this->read_byte(&byte);
    if (this->parse_aesgi_rs485_byte_(byte)) {
      this->last_aesgi_rs485_byte_ = now;
    } else {
      ESP_LOGVV(TAG, "Buffer cleared due to reset: %s",
                format_hex_pretty(&this->rx_buffer_.front(), this->rx_buffer_.size()).c_str());  // NOLINT
      this->rx_buffer_.clear();
    }
  }
}

uint8_t chksum(const uint8_t data[], const uint8_t len) {
  uint8_t checksum = 0;
  for (uint8_t i = 0; i < len; i++) {
    checksum = checksum + data[i];
  }
  return checksum;
}

bool AesgiRs485::parse_aesgi_rs485_byte_(uint8_t byte) {
  size_t at = this->rx_buffer_.size();
  this->rx_buffer_.push_back(byte);
  const uint8_t *raw = &this->rx_buffer_[0];

  if (at > 80) {
    ESP_LOGW(TAG, "Maximum frame length exceeded. Resetting buffer...");
    return false;
  }

  if (at == 0) {
    // return false to reset buffer
    return raw[0] == 0x2A;
  }

  if (at == 1)
    return true;

  if (at == 2)
    return true;

  uint8_t address = (raw[1] - '0') * 10 + (raw[2] - '0');

  if (byte != '\r')
    return true;

  size_t frame_len = at + 1;

  if (frame_len < 5) {
    ESP_LOGW(TAG, "Frame too short: %d", frame_len);
    return false;
  }

  uint8_t computed_crc = chksum(raw, frame_len - 2);
  uint8_t remote_crc = raw[frame_len - 2];
  if (!this->verify_checksum_(computed_crc, remote_crc)) {
    ESP_LOGW(TAG, "CRC check failed! 0x%02X != 0x%02X", computed_crc, remote_crc);
    return false;
  }

  std::string data(this->rx_buffer_.begin(), this->rx_buffer_.begin() + frame_len);

  bool found = false;
  for (auto *device : this->devices_) {
    if (device->address_ == address) {
      device->on_aesgi_rs485_data(data);
      found = true;
    }
  }
  if (!found) {
    ESP_LOGW(TAG, "Got AesgiRs485 frame from unknown address 0x%02X!", address);
  }

  // return false to reset buffer
  return false;
}

void AesgiRs485::dump_config() {
  ESP_LOGCONFIG(TAG, "AesgiRs485:");
  ESP_LOGCONFIG(TAG, "  RX timeout: %d ms", this->rx_timeout_);
}
float AesgiRs485::get_setup_priority() const {
  // After UART bus
  return setup_priority::BUS - 1.0f;
}

void AesgiRs485::send(uint8_t address, uint8_t command, const std::string &value) {
  if (address < 1 || address > 32) {
    ESP_LOGW(TAG, "Invalid device address: %d", address);
    return;
  }

  size_t frame_len = 1 + 2 + 1 + (value.empty() ? 0 : 1 + value.size()) + 1;
  std::vector<uint8_t> frame(frame_len);

  frame[0] = '#';
  frame[1] = '0' + (address / 10);
  frame[2] = '0' + (address % 10);
  frame[3] = command;

  if (!value.empty()) {
    frame[4] = ' ';
    for (size_t i = 0; i < value.size(); i++) {
      frame[5 + i] = value[i];
    }
  }

  frame[frame_len - 1] = '\r';

  this->write_array(frame.data(), frame_len);
  this->flush();
}

void AesgiRs485::send_broadcast(uint8_t command, const std::string &value) {
  size_t frame_len = 5 + (value.empty() ? 0 : 1 + value.size()) + 1;
  std::vector<uint8_t> frame(frame_len);

  frame[0] = '#';
  frame[1] = 'b';
  frame[2] = '0';
  frame[3] = '1';
  frame[4] = command;

  if (!value.empty()) {
    frame[5] = ' ';
    for (size_t i = 0; i < value.size(); i++) {
      frame[6 + i] = value[i];
    }
  }

  frame[frame_len - 1] = '\r';

  this->write_array(frame.data(), frame_len);
  this->flush();
}

}  // namespace aesgi_rs485
}  // namespace esphome
