#include <Arduino.h>
#include <SPI.h>
#ifdef ARDUINO_ARCH_ESP8266
  #include <ESP8266WiFi.h>
#else
  #include <WiFi.h>
#endif

// To heck with Adafruit's politics.
#include <ILI9341.h>

const int16_t tft_color_idle = ILI9341_BLUE, tft_color_wifi_timeout = ILI9341_RED, tft_color_wifi_con = ILI9341_YELLOW, tft_color_client_disconnect = ILI9341_GREEN, tft_color_client_timeout = ILI9341_PURPLE;

ILI9341 tft = ILI9341 (15, 2);
const int tft_w = 240, tft_h = 320;
const int32_t tft_len = tft_w * tft_h * 2;

WiFiServer wifi_server (9341);
IPAddress wifi_ip (192, 168, 2, 6), \
    wifi_gateway (192, 168, 2, 1), \
    wifi_subnet (255, 255, 255, 0);
const char *wifi_ssid = "go";
const char *wifi_pass = "go";

const int cli_block_len = 240; // must be even and integer fraction of tft_len

void connect () {
    int wifi_status;

    while ((wifi_status = WiFi.begin (wifi_ssid, wifi_pass)) != WL_CONNECTED) {
        tft.fill (tft_color_wifi_con);
        int delays = 0;
        while ((wifi_status = WiFi.status()) != WL_CONNECTED) {
            delay (500);
            if (delays++ > 20)
                break;
        }
        if (wifi_status == WL_CONNECTED)
            break;

        tft.fill (tft_color_wifi_timeout);
        delay (1000);
    }
    tft.fill (tft_color_idle);
}

void setup () {
    tft.init ();

    WiFi.config (wifi_ip, wifi_gateway, wifi_subnet);
    connect();
    wifi_server.begin();
}

void loop () {
    if (WiFi.status() != WL_CONNECTED)
        connect();

    WiFiClient client = wifi_server.available();
    if (! client) {
        delay (200);
        return;
    }
    
    tft.window (0, 0, tft_w - 1, tft_h - 1);
    for (int32_t b = 0; b < tft_len / cli_block_len; b ++) {
        int delays = 0;
        while (client.available() < cli_block_len) {
            if (! client.connected()) {
                tft.fill (tft_color_client_disconnect);
                delay (1000);
                tft.fill (tft_color_idle);
                return;
            }
            delay (1);
            if (delays++ > (10 * cli_block_len)) {
                client.flush();
                client.stop();
                tft.fill (tft_color_client_timeout);
                delay (1000);
                tft.fill (tft_color_idle);
                return; 
            }
        }
        for (int c = 0; c < cli_block_len; c += 2)
            tft.pixel (client.read() + (client.read() << 8));
    }

    client.flush();
    client.stop();
    return;
}

