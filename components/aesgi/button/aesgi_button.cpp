#include "aesgi_button.h"

namespace esphome::aesgi {

void AesgiButton::press_action() {
  if (this->holding_register_ == 'B') {
    this->parent_->send(this->holding_register_, "0 00.0");
    return;
  }

  this->parent_->send(this->holding_register_);
}

}  // namespace esphome::aesgi
