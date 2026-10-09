#include "aesgi_button.h"
#include "esphome/core/log.h"
#include "esphome/core/application.h"

// Fallback for ESPHome < 2026.10.0
#ifndef ESPHOME_LOG_TAG
#define ESPHOME_LOG_TAG(name, tag) static const char *const name = tag
#endif

namespace esphome::aesgi {

ESPHOME_LOG_TAG(TAG, "aesgi.button");

void AesgiButton::dump_config() { LOG_BUTTON("", "Aesgi Button", this); }
void AesgiButton::press_action() {
  if (this->holding_register_ == 'B') {
    this->parent_->send(this->holding_register_, "0 00.0");
    return;
  }

  this->parent_->send(this->holding_register_);
}

}  // namespace esphome::aesgi
