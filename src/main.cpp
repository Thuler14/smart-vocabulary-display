#include <Arduino.h>
#include <U8g2lib.h>

constexpr uint8_t OLED_CS_PIN = 21;
constexpr uint8_t OLED_DC_PIN = 17;
constexpr uint8_t OLED_RESET_PIN = 16;

constexpr uint16_t WORD_MAX_WIDTH_PX = 120;

U8G2_SSD1309_128X64_NONAME0_F_4W_HW_SPI display(U8G2_R0, OLED_CS_PIN,
                                                OLED_DC_PIN, OLED_RESET_PIN);

bool drawCenteredWord(const char* word) {
  display.setFont(u8g2_font_8x13B_tf);

  const uint16_t textWidth = display.getStrWidth(word);

  if (textWidth > WORD_MAX_WIDTH_PX) {
    return false;
  }

  const int16_t x = (display.getDisplayWidth() - textWidth) / 2;

  // Baseline places the word near the vertical center.
  display.drawStr(x, 38, word);

  return true;
}

void setup() {
  display.begin();

  display.clearBuffer();

  if (!drawCenteredWord("CLEAR")) {
    display.setFont(u8g2_font_6x12_tf);
    display.drawStr(4, 35, "DOES NOT FIT");
  }

  display.sendBuffer();
}

void loop() {
  //
}
