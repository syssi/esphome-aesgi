#pragma once

#include "../aesgi.h"
#include "esphome/components/button/button.h"

namespace esphome::aesgi {

class Aesgi;
class AesgiButton : public button::Button {
 public:
  void set_parent(Aesgi *parent) { this->parent_ = parent; };
  void set_holding_register(uint8_t holding_register) { this->holding_register_ = holding_register; };

 protected:
  void press_action() override;
  Aesgi *parent_;
  uint8_t holding_register_;
};

}  // namespace esphome::aesgi
