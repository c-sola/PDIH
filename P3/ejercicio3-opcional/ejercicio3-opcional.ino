const int ldrPin = A0;
const int ledPin = 6;

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int ldrValue = analogRead(ldrPin);
  int ledBrightness = map(ldrValue, 0, 1023, 255, 0); // inverso: más luz → menos brillo

  analogWrite(ledPin, ledBrightness);

  Serial.print("Luz: ");
  Serial.print(ldrValue);
  Serial.print(" | Brillo LED: ");
  Serial.println(ledBrightness);

  delay(200);
}
