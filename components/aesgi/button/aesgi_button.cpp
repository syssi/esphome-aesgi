#include "aesgi_button.h"
#include "esphome/core/log.h"
#include "esphome/core/application.h"

namespace esphome {
namespace aesgi {

static const char *const TAG = "aesgi.button";

void AesgiButton::dump_config() { LOG_BUTTON("", "Aesgi Button", this); }
void AesgiButton::press_action() {
  if (this->holding_register_ == 'B') {
    this->parent_->send(this->holding_register_, "0 00.0");
    break;
  }

  this->parent_->send(this->holding_register_);
}

}  // namespace aesgi
}  // namespace esphome
