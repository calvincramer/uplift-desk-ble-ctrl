#ifndef TIME_H
#define TIME_H

#include "config.h"
#include "wifi.h"

const unsigned long MILLISECONDS_PER_DAY = 1000 * 60 * 60 * 24;

// unix time (seconds) from NTP
unsigned long unix_time = 0;

// when we asked NTP for time. Don't ask server often.
unsigned long ms_got_unix_time = 0;


void get_time() {
  // Expire saved time every once in a while
  if ((millis() - ms_got_unix_time) > MILLISECONDS_PER_DAY) {
    unix_time = 0;
    ms_got_unix_time = 0;
  }

  // Don't have time yet
  if (unix_time == 0) {
    unix_time = get_time_from_ntp();
    ms_got_unix_time = millis();
    wifi_off();
  }
}

// Returns hour in range 0 to 23 according to current time zone. -1 indicates error.
int get_hour_current_time_zone() {
  if (unix_time == 0 || ms_got_unix_time == 0) {
    return -1;
  }
  unsigned long time = unix_time;
  time += (millis() - ms_got_unix_time) / 1000;
  time += TIME_ZONE_SHIFT_SECONDS;

  time = time % (60 * 60 * 24); // take off days, only care about hour, minute, second
  int hour = time / (60*60);
  return hour;
}

// Prints current time
void print_time() {
  if (unix_time == 0 || ms_got_unix_time == 0) {
    Serial.print("<unknown time> (unix_time=");
    Serial.print(unix_time);
    Serial.print(", ms_got_unix_time=");
    Serial.print(ms_got_unix_time);
    Serial.print(")");
    return;
  }
  unsigned long time = unix_time;
  time += (millis() - ms_got_unix_time) / 1000;
  time += TIME_ZONE_SHIFT_SECONDS;

  time = time % (60 * 60 * 24); // take off days, only care about hour, minute, second
  int hour = time / (60*60);
  time = time % (60 * 60);      // take off hours
  int minute = time / 60;
  time = time % 60;             // take off minutes
  int second = time;

  if (hour < 10) {
    Serial.print("0");
  }
  Serial.print(hour);
  Serial.print(":");
  if (minute < 10) {
    Serial.print("0");
  }
  Serial.print(minute);
  Serial.print(":");
  if (second < 10) {
    Serial.print("0");
  }
  Serial.print(second);
}

#endif