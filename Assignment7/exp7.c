/*
 * Experiment 7: Develop an ESP32-based LED blinking program and
 *               establish Wi-Fi connectivity for IoT applications.
 *
 * Hardware: ESP32 Development Board (ESP-WROOM-32)
 * Onboard LED: GPIO 2
 *
 * Features:
 * 1. Configures GPIO 2 as an output pin for the LED.
 * 2. Rapidly blinks the LED while attempting Wi-Fi connection.
 * 3. Connects to the local Wi-Fi access point / hotspot.
 * 4. Displays connection status, local IP address, and signal strength on Serial Monitor.
 * 5. Turns LED continuously ON once connection is successfully established.
 */

#include <WiFi.h>

/* Define Onboard LED Pin (GPIO 2 on most ESP32 Dev Boards) */
#define LED_PIN 2

/* Wi-Fi Credentials - Replace with your network details or mobile hotspot */
const char* ssid     = "College_WiFi";       /* Your Wi-Fi SSID */
const char* password = "Password123";        /* Your Wi-Fi Password */

void setup()
{
    /* Initialize Serial Communication at 115200 baud rate */
    Serial.begin(115200);
    delay(1000);

    /* Configure LED pin as digital output */
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);

    Serial.println("\n========================================");
    Serial.println("  ESP32 LED Blink & Wi-Fi Connectivity  ");
    Serial.println("========================================");

    /* Set ESP32 to Station (Client) mode */
    WiFi.mode(WIFI_STA);

    /* Begin connecting to the Wi-Fi network */
    Serial.print("Connecting to Wi-Fi SSID: ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);

    /*
     * Blink LED rapidly while waiting for Wi-Fi connection
     * WiFi.status() returns WL_CONNECTED when connection succeeds
     */
    while (WiFi.status() != WL_CONNECTED)
    {
        digitalWrite(LED_PIN, HIGH);  /* LED ON */
        delay(250);
        digitalWrite(LED_PIN, LOW);   /* LED OFF */
        delay(250);
        Serial.print(".");
    }

    /* Connection Successful */
    Serial.println("\n[SUCCESS] Connected to Wi-Fi!");
    Serial.print("Assigned IP Address: ");
    Serial.println(WiFi.localIP());
    Serial.print("Signal Strength (RSSI): ");
    Serial.print(WiFi.RSSI());
    Serial.println(" dBm");
    Serial.println("========================================\n");

    /* Keep LED ON solid to indicate successful connection */
    digitalWrite(LED_PIN, HIGH);
}

void loop()
{
    /*
     * In the main loop, check if connection is active:
     * If connected, blink LED in a slow 'heartbeat' rhythm (1 sec ON, 1 sec OFF)
     * If connection drops, attempt reconnection.
     */
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
