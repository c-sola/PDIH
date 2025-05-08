#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const int pinPiezo  = 11;
const int ledVerde  = 7;
const int ledRojo   = 6;
const int pinTemp   = A0;
const float VREF    = 5.0;  
LiquidCrystal_I2C lcd_1(0x27, 16, 2);
const int numReadings = 20;
int readings[numReadings];
int readIndex = 0;
long total = 0;
float average = 0;

void setup() {
  Wire.begin();
  lcd_1.init();
  lcd_1.backlight();

  pinMode(pinPiezo, OUTPUT);
  pinMode(ledVerde, OUTPUT);
  pinMode(ledRojo, OUTPUT);
  
  noTone(pinPiezo);
  digitalWrite(ledVerde, LOW);
  digitalWrite(ledRojo, LOW);

  Serial.begin(9600);

  for (int i = 0; i < numReadings; i++) {
    readings[i] = 0;
  }
}

void loop() {
  total = total - readings[readIndex];
  readings[readIndex] = analogRead(pinTemp);
  total = total + readings[readIndex];
  readIndex++;
  if (readIndex >= numReadings) {
    readIndex = 0;
  }
  average = total / (float) numReadings;
  
  float voltage = average * (VREF / 1023.0);
  float temperatura = (voltage - 0.5) * 100.0;
  
  Serial.print("Raw avg: ");
  Serial.print(average);
  Serial.print("  Voltaje: ");
  Serial.print(voltage, 3);
  Serial.print(" V  Temperatura: ");
  Serial.print(temperatura, 1);
  Serial.println(" °C");
  
  lcd_1.clear();
  lcd_1.setCursor(0, 0);
  lcd_1.print("Temp: ");
  lcd_1.print(temperatura, 1);
  lcd_1.print(" C");
  
  if (temperatura < 10.0) {
    lcd_1.setCursor(0, 1);
    lcd_1.print("HACE FRIO     ");
    tone(pinPiezo, 1000);
    digitalWrite(ledVerde, LOW);
    digitalWrite(ledRojo, HIGH);
  }
  else if (temperatura < 30.0) {
    lcd_1.setCursor(0, 1);
    lcd_1.print("TEMP NORMAL   ");
    noTone(pinPiezo);
    digitalWrite(ledVerde, HIGH);
    digitalWrite(ledRojo, LOW);
  }
  else {
    lcd_1.setCursor(0, 1);
    lcd_1.print("HACE CALOR    ");
    tone(pinPiezo, 1000);
    digitalWrite(ledVerde, LOW);
    digitalWrite(ledRojo, HIGH);
  }
  
  delay(1000);  
}
