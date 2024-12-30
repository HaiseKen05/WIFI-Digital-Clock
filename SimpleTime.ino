#include <WiFi.h>
#include "time.h"
#include "esp_sntp.h"
#include "LiquidCrystal_I2C.h"

// LCD Setup
LiquidCrystal_I2C lcd(0x27, 20, 4);

// Wi-Fi Credentials
const char *ssid = "Wifi_name";
const char *password = "Wifi_password";

// NTP Server Configuration
const char *ntpServer1 = "pool.ntp.org";
const char *ntpServer2 = "time.nist.gov";
const long gmtOffset_sec = 3600 * 8;       // GMT offset in seconds
const int daylightOffset_sec = 3600;      // Daylight savings offset in seconds

// Time Zone Configuration (optional)
const char *time_zone = "CET-1CEST,M3.5.0,M10.5.0/3";

// Location Information
const char *location = "Philippines, Cebu";

// Buffers to store last displayed date and time
char lastDisplayedDate[20] = " ";
char lastDisplayedTime[20] = " ";

// Function to Print Local Time to LCD
void printLocalTime() {
  struct tm timeinfo;

  // Check if time is available
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time!");
    return;
  }

  // Prepare date and time strings
  char currentDate[20];
  char currentTime[20];
  strftime(currentDate, sizeof(currentDate), "%B %d %Y", &timeinfo);
  strftime(currentTime, sizeof(currentTime), "%A %H:%M:%S", &timeinfo);

  // Update Date on LCD if it has changed
  if (strcmp(lastDisplayedDate, currentDate) != 0) {
    lcd.setCursor(0, 0);
    lcd.print(" ");  // Clear row
    lcd.setCursor(0, 0);
    lcd.print(currentDate);
    strncpy(lastDisplayedDate, currentDate, sizeof(lastDisplayedDate));
  }

  // Update Time on LCD if it has changed
  if (strcmp(lastDisplayedTime, currentTime) != 0) {
    lcd.setCursor(0, 1);
    lcd.print(" ");  // Clear row
    lcd.setCursor(0, 1);
    lcd.print(currentTime);
    strncpy(lastDisplayedTime, currentTime, sizeof(lastDisplayedTime));
  }

  // Display Location (static, does not change)
  lcd.setCursor(0, 2);
  lcd.print(" ");  // Clear row
  lcd.setCursor(0, 2);
  lcd.print(location);
}

// Callback Function for NTP Time Synchronization
void timeSyncCallback(struct timeval *t) {
  Serial.println("NTP Time Synchronized!");
  printLocalTime();
}

// Wi-Fi Connection Setup
void connectToWiFi() {
  Serial.printf("Connecting to Wi-Fi: %s ", ssid);
  WiFi.begin(ssid, password);

  // Wait until connected
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWi-Fi Connected!");
}

// Setup Function
void setup() {
  // Serial and LCD Initialization
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();

  // Connect to Wi-Fi
  connectToWiFi();

  // Configure NTP and Time Synchronization
  esp_sntp_servermode_dhcp(1);  // Optional: Enable DHCP for NTP server
  sntp_set_time_sync_notification_cb(timeSyncCallback);  // Set callback
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer1, ntpServer2);  // Configure NTP
}

// Loop Function
void loop() {
  delay(1000);        // Update time every second
  printLocalTime();   // Print updated time to LCD
}
