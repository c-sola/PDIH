#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd_1(0x27, 16, 2);

const int pinLed = 13;  
const int pinBoton = 7;   

const unsigned long retardoRebote = 50;

unsigned long tiempoInicio = 0;
unsigned long tiempoReaccion = 0;

void setup() {
  Wire.begin();
  pinMode(pinLed, OUTPUT);
  pinMode(pinBoton, INPUT_PULLUP);

  lcd_1.init();       // Inicializa el LCD
  lcd_1.backlight();  // Enciende la retroiluminación

  lcd_1.clear();
  lcd_1.setCursor(0, 0);
  lcd_1.print("Juego Reaccion");
  lcd_1.setCursor(0, 1);
  lcd_1.print("Presiona boton");
  delay(2000);
  lcd_1.clear();
  lcd_1.setCursor(0, 0);
  lcd_1.print("Presiona boton");
}

void loop() {
  if (digitalRead(pinBoton) == LOW) {
    while (digitalRead(pinBoton) == LOW) { }
    delay(retardoRebote);
    
    lcd_1.clear();
    lcd_1.setCursor(0, 0);
    lcd_1.print("Preparado...");
    lcd_1.setCursor(0, 1);
    lcd_1.print("Esperando LED");
    
    delay(1000);

    int retardoAleatorio = random(2000, 5000);
    delay(retardoAleatorio);
    
    digitalWrite(pinLed, HIGH);
    lcd_1.clear();
    lcd_1.setCursor(0, 0);
    lcd_1.print("¡YA!");
    
    tiempoInicio = millis();
    
    while (digitalRead(pinBoton) == LOW) { }
    delay(retardoRebote);

    while (digitalRead(pinBoton) != LOW) { }
    delay(retardoRebote);

    tiempoReaccion = millis() - tiempoInicio;
    
    digitalWrite(pinLed, LOW);
    
    lcd_1.clear();
    lcd_1.setCursor(0, 0);
    lcd_1.print("Tiempo:");
    lcd_1.setCursor(0, 1);
    lcd_1.print(tiempoReaccion);
    lcd_1.print(" ms");
    
    delay(2000);

    lcd_1.clear();
    lcd_1.setCursor(0, 0);
    lcd_1.print("Presiona boton");
    
    while (digitalRead(pinBoton) == LOW) { }
    delay(retardoRebote);
  }
}
