#include <Wire.h>
#include <WiFi.h>
#include "time.h"
#include "esp_sntp.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include "ClockUI.h"

// ================= DISPLAY =================
Adafruit_SH1106G display(128, 64, &Wire);

// ================= WIFI =================
const char *ssid = "";
const char *password = "";

// ================= NTP =================
const char *ntpServer1 = "pool.ntp.org";
const char *ntpServer2 = "time.nist.gov";
const long gmtOffset_sec = 3600 * 8; // x 8 for GMT+8 Kuala Lumpur
const int daylightOffset_sec = 0;

// ================= TIME =================
void printLocalTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) return;
  renderClock(timeinfo);
}

// ================= SETUP =================
void setup() {
  Serial.begin(115200);
  Wire.begin();

  display.begin(0x3C, true);
  display.setRotation(2);
  display.setTextColor(SH110X_WHITE);

  initClockUI(&display);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(500);

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer1, ntpServer2);
}

// ================= LOOP =================
void loop() {
  static unsigned long last = 0;
  if (millis() - last >= 1000) {
    last = millis();
    printLocalTime();
  }
}
