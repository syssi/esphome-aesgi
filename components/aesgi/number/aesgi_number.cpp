#include "aesgi_number.h"
#include "esphome/core/log.h"

namespace esphome {
namespace aesgi {

static const char *const TAG = "aesgi.number";

void AesgiNumber::dump_config() { LOG_NUMBER("", "Aesgi Number", this); }
void AesgiNumber::control(float value) {
  char buffer[5];

  if (this->holding_register_ == 'L') {
    snprintf(buffer, sizeof(buffer), "%03d", (int) value);
  } else {
    snprintf(buffer, sizeof(buffer), "%2.1f", value);
  }

  this->parent_->send(this->holding_register_, buffer);
  this->publish_state(value);
}

}  // namespace aesgi
}  // namespace esphome
