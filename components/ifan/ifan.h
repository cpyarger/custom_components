#pragma once

#include "esphome/core/component.h"
#include "esphome/components/output/binary_output.h"
#include "esphome/components/output/float_output.h"
#include "esphome/components/fan/fan.h"

namespace esphome {
namespace ifan {

class IFan : public Component, public fan::Fan {
 public:
  IFan(output::FloatOutput *output, int speed_count=3) : output_(output), speed_count_(speed_count) {}
  void setup() override;
  void dump_config() override;
  void set_direction(output::BinaryOutput *direction) { this->direction_ = direction; }
  fan::FanTraits get_traits() override;

 protected:
  void control(const fan::FanCall &call) override;
  void write_state_();

  output::FloatOutput *output_;
  output::BinaryOutput *direction_{nullptr};
  int speed_count_{};
};

}  // namespace speed
}  // namespace esphome