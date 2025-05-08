const int tempPin = A0;     // Sensor de temperatura conectado a A0
const int ledRojo = 8;      // LED rojo
const int ledVerde = 9;     // LED verde
const int ledAzul = 10;     // LED azul

void setup() {
  pinMode(ledRojo, OUTPUT);
  pinMode(ledVerde, OUTPUT);
  pinMode(ledAzul, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int lectura = analogRead(tempPin);
  float voltaje = lectura * 5.0 / 1023.0;
  float temperaturaC = (voltaje - 0.5) * 100; 

  Serial.print("Temperatura: ");
  Serial.print(temperaturaC);
  Serial.println(" °C");

  // Apagamos todos los LEDs al inicio
  digitalWrite(ledRojo, LOW);
  digitalWrite(ledVerde, LOW);
  digitalWrite(ledAzul, LOW);

  // Encendemos según el rango de temperatura
  if (temperaturaC < 18) {
    digitalWrite(ledAzul, HIGH);  // Hace frío
  } else if (temperaturaC >= 18 && temperaturaC < 25) {
    digitalWrite(ledVerde, HIGH);  // Temperatura normal
  } else {
    digitalWrite(ledRojo, HIGH);   // Hace calor
  }

  delay(500);
}
