#define BLYNK_TEMPLATE_ID "TMPL31SA8UcuC"
#define BLYNK_TEMPLATE_NAME "ESP32 LED WIFI"
#define BLYNK_AUTH_TOKEN "46Nk2BCyeszQUGdqiA3tP0g75qo6UUlQ"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

char ssid[] = "vadapav";
char pass[] = "mns12345";

#define LED_PIN 2

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
