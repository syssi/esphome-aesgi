#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include "esphome/components/aesgi_rs485/aesgi_rs485.h"

namespace esphome::aesgi_rs485::testing {

class MockAesgiRs485Device : public AesgiRs485Device {
 public:
  std::string received_data;
  int call_count{0};

  void on_aesgi_rs485_data(const std::string &data) override {
    received_data = data;
    call_count++;
  }
};

class TestableAesgiRs485 : public AesgiRs485 {
 public:
  void loop() override {}
  using AesgiRs485::parse_aesgi_rs485_byte_;
  using AesgiRs485::verify_checksum_;

  bool feed(const std::string &frame) {
    bool result = false;
    for (char c : frame)
      result = parse_aesgi_rs485_byte_(static_cast<uint8_t>(c));
    return result;
  }
};

// chksum: sum of all bytes mod 256
inline uint8_t compute_chksum(const std::string &data) {
  uint8_t sum = 0;
  for (char c : data)
    sum += static_cast<uint8_t>(c);
  return sum;
}

// Build a valid aesgi_rs485 frame with correct checksum.
// Format: * <addr_hi> <addr_lo> <content> <crc> \r
// addr is 2-digit decimal (address 1..32).
// content should include any trailing delimiter if present in the real protocol.
inline std::string make_frame(uint8_t address, const std::string &content) {
  std::string frame = "*";
  frame += static_cast<char>('0' + (address / 10));
  frame += static_cast<char>('0' + (address % 10));
  frame += content;
  uint8_t crc = compute_chksum(frame);
  frame += static_cast<char>(crc);
  frame += '\r';
  return frame;
}

}  // namespace esphome::aesgi_rs485::testing
