void setup()
{
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(10, OUTPUT);
}

void loop()
{
  digitalWrite(13, HIGH);
  digitalWrite(11, LOW);
  delay(150); //Espera de 150 ms
  
  digitalWrite(12, LOW);
  delay(150); //Espera de 150 ms
  
  digitalWrite(12, HIGH);
  digitalWrite(10, LOW);
  delay(150); //Espera de 150 ms
  digitalWrite(11, HIGH);
  digitalWrite(13, LOW);
  delay(150); //Espera de 150 ms
  digitalWrite(10, HIGH);
  digitalWrite(12, LOW);
  delay(150); //Espera de 150 ms
  
  digitalWrite(11, LOW);
  delay(150); //Espera de 150 ms
  
  digitalWrite(11, HIGH);
  digitalWrite(13, LOW);
  delay(150); //Espera de 150 ms
  digitalWrite(12, HIGH);
  digitalWrite(10, LOW);
  delay(150); //Espera de 150 ms
}
