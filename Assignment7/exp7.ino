/*
 * Experiment 7: Develop an ESP32-based LED blinking program and
 *               establish Wi-Fi connectivity for IoT applications.
 *
 * Hardware: ESP32 Development Board (ESP-WROOM-32)
 * Onboard LED: GPIO 2
 */

#include <WiFi.h>

#define LED_PIN 2

/* Replace with your network details or mobile hotspot */
const char* ssid     = "College_WiFi";
const char* password = "Password123";

void setup()
{
    Serial.begin(115200);
    delay(1000);

    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);

    Serial.println("\n========================================");
    Serial.println("  ESP32 LED Blink & Wi-Fi Connectivity  ");
    Serial.println("========================================");

    WiFi.mode(WIFI_STA);
    Serial.print("Connecting to Wi-Fi SSID: ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);

    /* Blink LED rapidly while connecting */
    while (WiFi.status() != WL_CONNECTED)
    {
        digitalWrite(LED_PIN, HIGH);
        delay(250);
        digitalWrite(LED_PIN, LOW);
        delay(250);
        Serial.print(".");
    }

    Serial.println("\n[SUCCESS] Connected to Wi-Fi!");
    Serial.print("Assigned IP Address: ");
    Serial.println(WiFi.localIP());
    Serial.print("Signal Strength (RSSI): ");
    Serial.print(WiFi.RSSI());
    Serial.println(" dBm");
    Serial.println("========================================\n");

    digitalWrite(LED_PIN, HIGH);
}

void loop()
{
    if (WiFi.status() == WL_CONNECTED)
    {
        digitalWrite(LED_PIN, HIGH);
        delay(1000);
        digitalWrite(LED_PIN, LOW);
        delay(1000);
    }
    else
    {
        Serial.println("[WARNING] Wi-Fi disconnected! Reconnecting...");
        WiFi.reconnect();
        delay(5000);
    }
}
