#include <Arduino.h>
#ifdef ESP8266
  #include <ESP8266WiFi.h>
#else
  #include <WiFi.h>
#endif

#ifndef LED_BUILTIN
  #define LED_BUILTIN 2
#endif

const IPAddress wifi_ip (192, 168, 2, 5),
    wifi_gateway (192, 168, 2, 1),
    wifi_subnet (255, 255, 255, 0);
const char *wifi_ssid = "go";
const char *wifi_pass = "go";

const char *srv_name = "192.168.2.11";
const uint16_t srv_port = 17045;

const uint8_t PIN_BUT_ALERT = 3,
    PIN_STATUS = LED_BUILTIN;

void strobe_status (uint16_t seconds) {
    // NOTE: LED is active LOW
    for (uint16_t c = 0; c < seconds * 10; c ++) {
        digitalWrite (PIN_STATUS, LOW);
        delay (50);
        digitalWrite (PIN_STATUS, HIGH);
        delay (50);
    }
    digitalWrite (PIN_STATUS, LOW);
}

bool send_request (const char *alert_s, const char *alert_ack_s) {
    WiFiClient client;
    if (! client.connect (srv_name, srv_port)) {
        strobe_status (5);
        return false;
    }

    const uint16_t alert_s_n = strlen (alert_s) + 1;
    if (client.write (alert_s, alert_s_n) != alert_s_n) {
        client.stop();
        strobe_status (4);
        return false;
    }

    char in_b[80] = {0};
    uint16_t in_n = client.readBytesUntil (0, in_b, sizeof(in_b));
    in_b[in_n] = 0;
    if (strcmp (in_b, alert_ack_s)) {
        client.stop();
        strobe_status (3);
        return false;
    }

    client.stop();
    strobe_status (1);
    return true;
}

void wifi_connect () {
    WiFi.begin (wifi_ssid, wifi_pass);
    while (WiFi.status() != WL_CONNECTED)
        strobe_status (1);
}

void setup () {
    pinMode (PIN_BUT_ALERT, INPUT);
    pinMode (PIN_STATUS, OUTPUT);
    digitalWrite (PIN_STATUS, HIGH);
    WiFi.config (wifi_ip, wifi_gateway, wifi_subnet);
    wifi_connect ();
}

void loop () {
    if (WiFi.status() != WL_CONNECTED)
        wifi_connect();

    if (digitalRead (PIN_BUT_ALERT) == HIGH) {
        // delay to ignore some contact bounce
        delay (100);
        // wait for depress
        while (digitalRead (PIN_BUT_ALERT) == HIGH)
            delay (100);

        (void) send_request ("LUCAS ALERT", "ALERT ACK");
    }

    delay (100);
    return;
}

