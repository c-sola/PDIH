void setup() {
  pinMode(13, OUTPUT);
}

void loop() {
  //Letra H (••••)
  for (int i = 0; i < 4; i++) {
    digitalWrite(13, HIGH);
    delay(200); 
    digitalWrite(13, LOW);
    delay(200); 
  }
  delay(600);
  
  //Letra O (---)
  for (int i = 0; i < 3; i++) {
    digitalWrite(13, HIGH);
    delay(600); 
    digitalWrite(13, LOW);
    delay(200); 
  }
  delay(600);

  //Letra L (•-••)
  digitalWrite(13, HIGH);
  delay(200);
  digitalWrite(13, LOW);
  delay(200);
  digitalWrite(13, HIGH);
  delay(600);
  digitalWrite(13, LOW);
  delay(200);
  for (int i = 0; i < 2; i++) {
    digitalWrite(13, HIGH);
    delay(200); 
    digitalWrite(13, LOW);
    delay(200);
  }
  delay(600);

  //Letra A (•-)
  digitalWrite(13, HIGH);
  delay(200); 
  digitalWrite(13, LOW);
  delay(200);
  digitalWrite(13, HIGH);
  delay(600); 
  digitalWrite(13, LOW);
  delay(600);

  //Dejamos un espacio entre palabras
  delay(1400);

  //Letra M (--)
  for (int i = 0; i < 2; i++) {
    digitalWrite(13, HIGH);
    delay(600); 
    digitalWrite(13, LOW);
    delay(200);
  }
  delay(600);

  //Letra U (••-)
  for (int i = 0; i < 2; i++) {
    digitalWrite(13, HIGH);
    delay(200); 
    digitalWrite(13, LOW);
    delay(200);
  }
  digitalWrite(13, HIGH);
  delay(600); 
  digitalWrite(13, LOW);
  delay(600);

  //Letra N (-•)
  digitalWrite(13, HIGH);
  delay(600); 
  digitalWrite(13, LOW);
  delay(200);
  digitalWrite(13, HIGH);
  delay(200);
  digitalWrite(13, LOW);
  delay(600);

  //Letra D (-••)
  digitalWrite(13, HIGH);
  delay(600); 
  digitalWrite(13, LOW);
  delay(200); 
  for (int i = 0; i < 2; i++) {
    digitalWrite(13, HIGH);
    delay(200); 
    digitalWrite(13, LOW);
    delay(200);
  }
  delay(600);


  //Letra O (---)
  for (int i = 0; i < 3; i++) {
    digitalWrite(13, HIGH);
    delay(600);
    digitalWrite(13, LOW);
    delay(200);
  }
  delay(5000);
}
