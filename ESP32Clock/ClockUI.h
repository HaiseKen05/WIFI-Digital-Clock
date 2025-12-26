#ifndef CLOCK_UI_H
#define CLOCK_UI_H

#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include "time.h"

// ================= CONFIG =================
#define SCREEN_W 128
#define SCREEN_H 64

// ================= INIT ===================
void initClockUI(Adafruit_SH1106G *disp);

// ================= RENDER =================
void renderClock(struct tm &timeinfo);
void drawCenteredText(const char *text, int y, uint8_t size);

#endif
