#include "aesgi_number.h"
#include "esphome/core/log.h"

// Fallback for ESPHome < 2026.10.0
#ifndef ESPHOME_LOG_TAG
#define ESPHOME_LOG_TAG(name, tag) static const char *const name = tag
#endif

namespace esphome::aesgi {

ESPHOME_LOG_TAG(TAG, "aesgi.number");

void AesgiNumber::dump_config() { LOG_NUMBER("", "Aesgi Number", this); }
void AesgiNumber::control(float value) {
  char buffer[7] = {0};

  if (this->holding_register_ == '8') {
    snprintf(buffer, sizeof(buffer), "%03.0f", value);
    this->parent_->send_broadcast(this->holding_register_, buffer);
    this->publish_state(value);
    return;
  }

  if (this->holding_register_ == 'L') {
    snprintf(buffer, sizeof(buffer), "%03.0f", value);
  }

  if (this->holding_register_ == 'B') {
    snprintf(buffer, sizeof(buffer), "%d %04.1f", (value > 0.0f) ? 2 : 0, value);
  }

  if (this->holding_register_ == 'S') {
    snprintf(buffer, sizeof(buffer), "%04.1f", value);
  }

  this->parent_->send(this->holding_register_, buffer);
  this->publish_state(value);
}

}  // namespace esphome::aesgi
