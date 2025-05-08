#include <Servo.h>

Servo servo;

const int buttonPin = 2;    // Botón conectado al pin 2
const int servoPin = 7;     // Servo conectado al pin 7

bool lastButtonState = LOW;
bool currentButtonState;
bool servoAt90 = false;

void setup() {
  pinMode(buttonPin, INPUT);      // Usa resistencia externa (pull-down)
  servo.attach(servoPin);         
  servo.write(0);                 // Posición inicial
  Serial.begin(9600);
}

void loop() {
  currentButtonState = digitalRead(buttonPin);

  // Detecta flanco de subida (cuando se presiona el botón)
  if (currentButtonState == HIGH && lastButtonState == LOW) {
    if (!servoAt90) {
      servo.write(90);       // Va a 90°
      servoAt90 = true;
    } else {
      servo.write(0);        // Vuelve a 0°
      servoAt90 = false;
    }
    delay(200);  // Antirrebote
  }

  lastButtonState = currentButtonState;
}
