#include <Arduino.h>
#include <U8g2lib.h>

constexpr uint8_t OLED_CS_PIN = 21;
constexpr uint8_t OLED_DC_PIN = 17;
constexpr uint8_t OLED_RESET_PIN = 16;

constexpr uint16_t WORD_DURATION_MS = 4000;
constexpr uint16_t DEFINITION_DURATION_MS = 8000;
constexpr uint16_t EXAMPLE_DURATION_MS = 10000;

constexpr uint16_t BODY_MAX_WIDTH_PX = 120;
constexpr uint8_t BODY_MAX_LINES = 4;
constexpr uint8_t BODY_LINE_HEIGHT_PX = 12;

U8G2_SSD1309_128X64_NONAME0_F_4W_HW_SPI display(U8G2_R0, OLED_CS_PIN,
                                                OLED_DC_PIN, OLED_RESET_PIN);

struct VocabularyEntry {
  const char* word;
  const char* definition;
  const char* example;
};

const VocabularyEntry entry = {"ELOQUENT",
                               "Fluent or persuasive in speaking or writing.",
                               "She gave an eloquent speech at the ceremony."};

enum class DisplayPhase { Word, Definition, Example };

DisplayPhase currentPhase = DisplayPhase::Word;
unsigned long phaseStartedAt = 0;

bool drawCenteredWord(const char* word) {
  display.setFont(u8g2_font_8x13B_tf);

  const uint16_t textWidth = display.getStrWidth(word);

  if (textWidth > BODY_MAX_WIDTH_PX) {
    return false;
  }

  const int16_t x = (display.getDisplayWidth() - textWidth) / 2;

  // Baseline places the word near the vertical center.
  display.drawStr(x, 38, word);

  return true;
}

bool drawWrappedBodyText(const char* text) {
  char workingText[180];
  strncpy(workingText, text, sizeof(workingText) - 1);
  workingText[sizeof(workingText) - 1] = '\0';

  char lines[BODY_MAX_LINES][64] = {};
  uint8_t lineCount = 0;

  display.setFont(u8g2_font_6x12_tf);

  char* word = strtok(workingText, " ");

  while (word != nullptr) {
    if (lineCount >= BODY_MAX_LINES) {
      return false;
    }

    char candidate[64];

    if (lines[lineCount][0] == '\0') {
      snprintf(candidate, sizeof(candidate), "%s", word);
    } else {
      snprintf(candidate, sizeof(candidate), "%s %s", lines[lineCount], word);
    }

    if (display.getStrWidth(candidate) <= BODY_MAX_WIDTH_PX) {
      strncpy(lines[lineCount], candidate, sizeof(lines[lineCount]) - 1);
    } else {
      lineCount++;

      if (lineCount >= BODY_MAX_LINES) {
        return false;
      }

      if (display.getStrWidth(word) > BODY_MAX_WIDTH_PX) {
        return false;
      }

      strncpy(lines[lineCount], word, sizeof(lines[lineCount]) - 1);
    }

    word = strtok(nullptr, " ");
  }

  if (lines[lineCount][0] != '\0') {
    lineCount++;
  }

  const uint8_t totalTextHeight = lineCount * BODY_LINE_HEIGHT_PX;

  const int16_t top = (display.getDisplayHeight() - totalTextHeight) / 2;

  for (uint8_t i = 0; i < lineCount; i++) {
    const uint16_t lineWidth = display.getStrWidth(lines[i]);

    const int16_t x = (display.getDisplayWidth() - lineWidth) / 2;

    const int16_t baseline = top + 10 + i * BODY_LINE_HEIGHT_PX;

    display.drawStr(x, baseline, lines[i]);
  }

  return true;
}

void drawFitError(const char* section) {
  display.setFont(u8g2_font_6x12_tf);
  display.drawStr(4, 26, section);
  display.drawStr(4, 42, "DOES NOT FIT");
}

void renderCurrentPhase() {
  display.clearBuffer();

  switch (currentPhase) {
    case DisplayPhase::Word:
      if (!drawCenteredWord(entry.word)) {
        drawFitError("WORD");
      }

      break;

    case DisplayPhase::Definition:
      if (!drawWrappedBodyText(entry.definition)) {
        drawFitError("DEFINITION");
      }

      break;

    case DisplayPhase::Example:
      if (!drawWrappedBodyText(entry.example)) {
        drawFitError("EXAMPLE");
      }

      break;
  }

  display.sendBuffer();
  phaseStartedAt = millis();
}

uint16_t getCurrentPhaseDuration() {
  switch (currentPhase) {
    case DisplayPhase::Word:
      return WORD_DURATION_MS;

    case DisplayPhase::Definition:
      return DEFINITION_DURATION_MS;

    case DisplayPhase::Example:
      return EXAMPLE_DURATION_MS;
  }

  return WORD_DURATION_MS;
}

void advancePhase() {
  switch (currentPhase) {
    case DisplayPhase::Word:
      currentPhase = DisplayPhase::Definition;
      break;

    case DisplayPhase::Definition:
      currentPhase = DisplayPhase::Example;
      break;

    case DisplayPhase::Example:
      currentPhase = DisplayPhase::Word;
      break;
  }

  renderCurrentPhase();
}

void setup() {
  display.begin();
  renderCurrentPhase();
}

void loop() {
  const unsigned long elapsed = millis() - phaseStartedAt;

  if (elapsed >= getCurrentPhaseDuration()) {
    advancePhase();
  }
}
