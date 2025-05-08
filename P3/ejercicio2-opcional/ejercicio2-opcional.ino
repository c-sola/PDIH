const int trigPin = 13;
const int echoPin = 12;
const int buzzerPin = 8; 

void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
}

void loop() {
  // Enviar pulso al sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Medir duración del pulso
  long duration = pulseIn(echoPin, HIGH);
  float distance = duration * 0.034 / 2;

  Serial.print("Distancia: ");
  Serial.print(distance);
  Serial.println(" cm");

  if (distance < 300) { // Solo pita si está a menos de 100 cm
    // Mapea la distancia entre 0 y 100 cm a un delay entre 50 ms y 500 ms
    int delayTime = map(distance, 0, 100, 50, 500);

    tone(buzzerPin, 1000); // frecuencia fija, puedes cambiarla también si quieres
    delay(100);
    noTone(buzzerPin);
    delay(delayTime);
  } else {
    noTone(buzzerPin); // si está lejos, apagamos el buzzer
  }
}
