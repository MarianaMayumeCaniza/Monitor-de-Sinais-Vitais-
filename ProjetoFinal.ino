#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "MAX30105.h"
#include "spo2_algorithm.h"

MAX30105 particleSensor;
LiquidCrystal_I2C lcd(0x27, 20, 4); // Endereço I2C 0x3F, 20 colunas e 4 linhas

#define MAX_BRIGHTNESS 255

#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
uint16_t irBuffer[100]; // infrared LED sensor data
uint16_t redBuffer[100]; // red LED sensor data
#else
uint32_t irBuffer[100]; // infrared LED sensor data
uint32_t redBuffer[100]; // red LED sensor data
#endif

int32_t bufferLength;    // data length
int32_t spo2;            // SPO2 value
int8_t validSPO2;        // indicator to show if the SPO2 calculation is valid
int32_t heartRate;       // heart rate value
int8_t validHeartRate;   // indicator to show if the heart rate calculation is valid

long instance1 = 0, timer;
double hrv = 0, hr = 72, interval = 0;
int value = 0, count = 0;
bool flag = 0;

#define threshold 100      // to identify R peak
#define timer_value 10000   // 10 seconds timer to calculate hr

void sendToESP32(int batimentos, int spo2) {
  Wire.beginTransmission(9); // Endereço I2C do ESP32
  Wire.write("BPM:");
  Wire.write(batimentos);
  Wire.write(",SPO2:");
  Wire.write(spo2);
  Wire.endTransmission();
}

void setup() {
  lcd.init();
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Batimentos: ");
  lcd.print(value);

  lcd.setCursor(0, 1);
  lcd.print("SPO2: ");
  lcd.print(spo2, DEC);

  Serial.begin(9600);

  pinMode(10, INPUT); // Setup for leads off detection LO +
  pinMode(11, INPUT); // Setup for leads off detection LO -

  // Initialize sensor
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) {
    Serial.println(F("MAX30105 was not found. Please check wiring/power."));
    while (1);
  }

  byte ledBrightness = 60;
  byte sampleAverage = 4;
  byte ledMode = 2;
  byte sampleRate = 100;
  int pulseWidth = 411;
  int adcRange = 4096;

  particleSensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange);
}

void loop() {
  if ((digitalRead(10) == 1) || (digitalRead(11) == 1))
  {
    Serial.println("leads off!");
    instance1 = micros();
    timer = millis();
  }
  else
  {
    value = analogRead(A0);
    value = map(value, 250, 400, 0, 100);

    if ((value > threshold) && (!flag))
    {
      count++;
      Serial.println("in");
      flag = 1;
      interval = micros() - instance1;
      instance1 = micros();
    }
    else if ((value < threshold))
    {
      flag = 0;
    }

    if ((millis() - timer) > timer_value)
    {
      hr = count * 6;
      timer = millis();
      count = 0;
    }

    hrv = hr / 60 - interval / 1000000;

    Serial.print("Batimentos:");
    Serial.println(value);

    bufferLength = 100;

    for (byte i = 0; i < bufferLength; i++)
    {
      while (particleSensor.available() == false)
        particleSensor.check();

      redBuffer[i] = particleSensor.getRed();
      irBuffer[i] = particleSensor.getIR();
      particleSensor.nextSample();

      //Serial.print(F("red="));
      //Serial.print(redBuffer[i], DEC);
      //Serial.print(F(", ir="));
      //Serial.print(irBuffer[i], DEC);
    }

    maxim_heart_rate_and_oxygen_saturation(irBuffer, bufferLength, redBuffer, &spo2, &validSPO2, &heartRate, &validHeartRate);

    

    Serial.print("Batimentos:");
    Serial.println(value);
    Serial.print(F(", SPO2="));
    Serial.println(spo2, DEC);

    // Impressão no LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Batimentos: ");
    lcd.print(value);

    lcd.setCursor(0, 1);
    lcd.print("SPO2: ");
    lcd.print(spo2, DEC);

    sendToESP32(value, spo2);
    delay(30);
  }
}
