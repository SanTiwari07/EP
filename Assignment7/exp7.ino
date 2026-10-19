#define BLYNK_TEMPLATE_ID "TMPL3WUlKR7np"
#define BLYNK_TEMPLATE_NAME "ESP32 LED WIFI"
#define BLYNK_AUTH_TOKEN "aT-t7kVzjb-kPqrNsfeqyO4Z5fXqTFWG"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

char ssid[] = "Sample";
char pass[] = "Password";

#define LED_PIN 23

BLYNK_WRITE(V0) {
    int value = param.asInt();
    digitalWrite(LED_PIN, value);
    Serial.print("LED: ");
    Serial.println(value ? "ON" : "OFF");
}

void setup() {
    Serial.begin(115200);
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);

    Serial.println("Connecting to Wi-Fi...");
    Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("Wi-Fi: CONNECTED");
    } else {
        Serial.println("Wi-Fi: NOT CONNECTED");
    }

    if (Blynk.connected()) {
        Serial.println("Blynk: CONNECTED");
    } else {
        Serial.println("Blynk: NOT CONNECTED");
    }
}

void loop() {
    Blynk.run();
}
