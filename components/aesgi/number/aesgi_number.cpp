#include "aesgi_number.h"
#include "esphome/core/log.h"

namespace esphome {
namespace aesgi {

static const char *const TAG = "aesgi.number";

void AesgiNumber::dump_config() { LOG_NUMBER("", "Aesgi Number", this); }
void AesgiNumber::control(float value) {
  char buffer[7] = {0};

  if (this->holding_register_ == '8') {
    snprintf(buffer, sizeof(buffer), "%03d", (int) value);
    this->parent_->send_broadcast(this->holding_register_, buffer);
    this->publish_state(value);
    return;
  }

  if (this->holding_register_ == 'L') {
    snprintf(buffer, sizeof(buffer), "%03d", (int) value);
  }

  if (this->holding_register_ == 'B') {
    snprintf(buffer, sizeof(buffer), "%d %2.1f", (value > 0.0f) ? 2 : 0, value);
  }

  if (this->holding_register_ == 'S') {
    snprintf(buffer, sizeof(buffer), "%2.1f", value);
  }

  this->parent_->send(this->holding_register_, buffer);
  this->publish_state(value);
}

}  // namespace aesgi
}  // namespace esphome
