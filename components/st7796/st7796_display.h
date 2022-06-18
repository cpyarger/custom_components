#pragma once

#include "esphome/core/component.h"
#include "esphome/components/spi/spi.h"
#include "esphome/components/display/display_buffer.h"
#include "st7796_defines.h"
#include "st7796_init.h"

namespace esphome {
namespace st7796 {

enum ST7796Model {
  M5STACK = 0,
  TFT_24,
  WT_32,
};

enum ST7796ColorMode {
  BITS_8,
  BITS_8_INDEXED,
};
struct ST7796colors{
 static const Color RED;
 static const Color GREEN;
 static const Color BLUE;

};
class ST7796Display : public PollingComponent,
                       public display::DisplayBuffer,
                       public spi::SPIDevice<spi::BIT_ORDER_MSB_FIRST, spi::CLOCK_POLARITY_LOW,
                                             spi::CLOCK_PHASE_LEADING, spi::DATA_RATE_40MHZ> {
 public:

  void Lcd_Init(void);
  void Lcd_Clear(unsigned int color);


  #define LCD_CS_SET (  this->dc_pin_->digital_write(true))
  #define LCD_RST_SET (this->reset_pin_->digital_write(true))
  #define LCD_RS_SET (this->busy_pin_->digital_write(true))
  #define LED_SET (this->led_pin_->digital_write(true))
  #define LCD_CS_CLR ( this->dc_pin_->digital_write(false))
  #define LCD_RST_CLR (this->reset_pin_->digital_write(false))
  #define LCD_RS_CLR (this->busy_pin_->digital_write(false))



  void Lcd_Reset(void);
  void LCD_WriteData_16Bit(unsigned int data);
  void set_dc_pin(GPIOPin *dc_pin) { dc_pin_ = dc_pin; }
  void Lcd_SetRegion(unsigned int x_start,unsigned int y_start,unsigned int x_end,unsigned int y_end);
  void spi_init(void);

  void Lcd_writeregs(unsigned char data,bool type);

  float get_setup_priority() const override;
  void set_reset_pin(GPIOPin *reset) { this->reset_pin_ = reset; }
  void set_led_pin(GPIOPin *led) { this->led_pin_ = led; }
  void set_model(ST7796Model model) { this->model_ = model; }
  void set_palette(const uint8_t *palette) { this->palette_ = palette; }
  void set_buffer_color_mode(ST7796ColorMode color_mode) { this->buffer_color_mode_ = color_mode; }

  void command(uint8_t value);
  void data(uint8_t value);
  void send_command(uint8_t command_byte, const uint8_t *data_bytes, uint8_t num_data_bytes);
  uint8_t read_command(uint8_t command_byte, uint8_t index);
  virtual void initialize() = 0;

  void update() override;

  void fill(Color color) override;

  void dump_config() override;
  void setup() override {
    this->setup_pins_();
    this->initialize();
  }
  display::DisplayType get_display_type() override { return display::DisplayType::DISPLAY_TYPE_COLOR; }
 protected:
  void draw_absolute_pixel_internal(int x, int y, Color color) override;
  void setup_pins_();

  void init_lcd_(const uint8_t *init_cmd);
  void set_addr_window_(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
  void invert_display_(bool invert);
  void reset_();
  void fill_internal_(Color color);
  void display_();
  uint16_t convert_to_16bit_color_(uint8_t color_8bit);
  uint8_t convert_to_8bit_color_(uint16_t color_16bit);

  ST7796Model model_;
  int16_t width_{480};   ///< Display width as modified by current rotation
  int16_t height_{320};  ///< Display height as modified by current rotation
    uint16_t x_high_{0};
  uint16_t y_high_{0};
  uint16_t x_low_{0};
  uint16_t y_low_{0};
  uint16_t x_true_{0};
  uint16_t y_true_{0};
  const uint8_t *palette_;
  ST7796ColorMode buffer_color_mode_{BITS_8};

  uint32_t get_buffer_length_();
  int get_width_internal() override;
  int get_height_internal() override;

  void start_command_();
  void end_command_();
  void start_data_();
  void end_data_();

  uint8_t transfer_buffer_[64];

  uint32_t buffer_to_transfer_(uint32_t pos, uint32_t sz);

  GPIOPin *reset_pin_{nullptr};
  GPIOPin *led_pin_{nullptr};
  GPIOPin *dc_pin_;
  GPIOPin *busy_pin_{nullptr};
};

//-----------   M5Stack display --------------
class ST7796M5Stack : public ST7796Display {
 public:
  void initialize() override;
};

//-----------   ST7796_24_TFT display --------------
class ST7796TFT24 : public ST7796Display {
 public:
  void initialize() override;
};

class ST7796WT32 : public ST7796Display {
 public:
  void initialize() override;
};

}  // namespace st7796
}  // namespace esphome
