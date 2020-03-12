#include <Arduino.h>

#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <Wire.h>
#include <SPI.h>
#include <BH1750.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_SSD1306.h>
#include <BlynkSimpleEsp32.h>
Adafruit_BMP280 bmp;
BH1750 lightMeter;
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

char auth[] = "9cxhsp-5Sp37bsGGP_CnHHonM9f-748h";
char ssid[] = "Embed";
char pass[] = "weareincontrol";

float Temp;
float Press;
float lux;

void setup() {
  Serial.begin(115200);

  //oled init
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  //BMP280 init
  if (!bmp.begin()) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
    while (1) delay(10);
  }

  //bh1750 init
  lightMeter.begin();

  //WiFi & Blynk
  delay(11);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  int wifi_ctr = 0;

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("WiFi connected");
  Blynk.begin(auth, ssid, pass, "server.wyns.it", 8081);
}

void loop() {
  delay(1000);
  Temp = bmp.readTemperature();
  lux = lightMeter.readLightLevel();

  //printen op oled
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.setTextColor(WHITE);
  display.println("BMP280:");
  display.setCursor(0, 10);
  display.setTextColor(BLACK, WHITE);
  display.print((char)247); //'°' graden symbool
  display.print("C: ");
  display.print(Temp, 1);
  
  display.setCursor(0, 30);
  display.setTextColor(WHITE);
  display.println("BH1750:");
  display.setCursor(0, 40);
  display.setTextColor(BLACK, WHITE);
  display.print("Light = ");
  display.print(lux);
  display.print(" lux");
  display.display();
  
  // op blynk schrijven
  Blynk.run();
  Blynk.virtualWrite(V4, Temp);
  Blynk.virtualWrite(V5, lux);
}
