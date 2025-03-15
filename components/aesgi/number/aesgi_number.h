#pragma once

#include "../aesgi.h"
#include "esphome/core/component.h"
#include "esphome/components/number/number.h"

namespace esphome {
namespace aesgi {

class Aesgi;

class AesgiNumber : public number::Number, public Component {
 public:
  void set_parent(Aesgi *parent) { this->parent_ = parent; };
  void set_holding_register(uint8_t holding_register) { this->holding_register_ = holding_register; };
  void dump_config() override;

 protected:
  void control(float value) override;

  Aesgi *parent_;
  uint8_t holding_register_;
};

}  // namespace aesgi
}  // namespace esphome
