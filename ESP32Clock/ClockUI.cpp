#include "ClockUI.h"

// ================= DISPLAY PTR =================
static Adafruit_SH1106G *display;

// ================= PROGRESS BAR =================
#define BAR_COUNT 10
#define BAR_W     10
#define BAR_H     4
#define BAR_GAP   2
#define BAR_Y     60

// ================= INIT =================
void initClockUI(Adafruit_SH1106G *disp) {
  display = disp;
}

// ================= HELPERS =================
void drawCenteredText(const char *text, int y, uint8_t size) {
  int16_t x1, y1;
  uint16_t w, h;

  display->setTextSize(size);
  display->getTextBounds(text, 0, 0, &x1, &y1, &w, &h);
  display->setCursor((SCREEN_W - w) / 2, y);
  display->print(text);
}

// ================= UI ELEMENTS =================
static void drawDateLabel() {
  drawCenteredText("M / D / Y", 2, 1);
}

static void drawDate(const char *dateStr) {
  drawCenteredText(dateStr, 14, 1);
}

static void drawTime(const char *timeStr) {
  drawCenteredText(timeStr, 35, 2);
}

static void drawProgressBar(int seconds) {
  int bars = seconds / 6;
  int totalWidth = (BAR_W * BAR_COUNT) + (BAR_GAP * (BAR_COUNT - 1));
  int startX = (SCREEN_W - totalWidth) / 2;

  for (int i = 0; i < BAR_COUNT; i++) {
    int x = startX + i * (BAR_W + BAR_GAP);
    if (i < bars)
      display->fillRect(x, BAR_Y, BAR_W, BAR_H, SH110X_WHITE);
    else
      display->drawRect(x, BAR_Y, BAR_W, BAR_H, SH110X_WHITE);
  }
}

// ================= CLOCK RENDER =================
void renderClock(struct tm &timeinfo) {
  char dateStr[20];
  char timeStr[20];

  strftime(dateStr, sizeof(dateStr), "%m / %d / %Y", &timeinfo);
  strftime(timeStr, sizeof(timeStr), "%H:%M:%S", &timeinfo);

  display->clearDisplay();
  display->setTextColor(SH110X_WHITE);

  drawDateLabel();
  drawDate(dateStr);
  drawTime(timeStr);
  drawProgressBar(timeinfo.tm_sec);

  display->display();
}
