#include "AsyncTaskLib.h"
#include <LiquidCrystal.h>

//#define DEBUG(a) Serial.print(millis()); Serial.print(": "); Serial.println(a);


// pines para LCD
const int rs = 12, en = 11, d4 = 31, d5 = 32, d6 = 33, d7 = 34;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


#define pin_photo A0
int photo_value = 0;
void read_photoresistor(void);


#define pin_temp A1
#define beta 4090
#define resistance 10
float temp_value = 0.0;
float temperatura = 0.0;
void read_temperature(void);

AsyncTask asyncTaskTemp(200, true, read_temperature);
AsyncTask asyncTaskPhoto(100, true,read_photoresistor);

//pines LED
int ledRojo=23;    // pin para el LED rojo *** cambiar pines del led a pwm para rgb completo
int ledVerde=24;   // pin para el LED verde
int ledAzul=25;    // pin para el LED azul

/*
bool estado_A = false;
bool estado_B = false;
bool estado_C = false;
bool estado_D = false;


byte estado;
#define estado_A 1
#define estado_B 2
#define estado_C 3
#define estado_D 4
*/

enum estado
{
  estado_A,
  estado_B,
  estado_C,
  estado_D
};


// Variables globales
estado currentState;



void read_photoresistor()
{
  photo_value = analogRead(pin_photo);
  lcd.setCursor(0, 0);
  lcd.print("PHOTO:");
  lcd.print(photo_value);
}

void read_temperature()// Funcion encargada de leer y procesar la informacion del sensor de temperatura
{
  // se usa la sigiente formula para convertir la informacion dada por el sensor en grados centigrados
  long a =1023 - analogRead(pin_temp);
  temperatura = beta /(log((1025.0 * 10 / a - 10) / 10) + beta / 298.0) - 273.0;
  lcd.setCursor(0, 1);
  lcd.print("temp:");
  lcd.print(temperatura); // se muestra en pantalla la temperatura dada por la convercion del valor del sensor
}

void setup()
{
	Serial.begin(9600);
	Serial.println("Starting");
  lcd.begin(16, 2);
  lcd.clear();
  //se inicializan los pines del led como salidas
  pinMode(ledRojo,OUTPUT);
  pinMode(ledVerde,OUTPUT);
  pinMode(ledAzul,OUTPUT);


   currentState = estado_A;

//	asyncTaskTemp.OnFinish = action2;
//	asyncTask1.OnFinish = action1;
  
	asyncTaskTemp.Start();
  asyncTaskPhoto.Start();
  
 
}

void loop()
{
	asyncTaskTemp.Update();
	asyncTaskPhoto.Update();
  actualizar_estado();
  
  }
    
void actualizar_estado()
{
  switch (currentState)
  {
    case estado::estado_A: funcion_A();   break;
    case estado::estado_B: funcion_B();   break;
    case estado::estado_C: funcion_C();   break;
    case estado::estado_D: funcion_D();   break;
    default: break;
  }
}

void funcion_A(){
  digitalWrite(ledRojo,LOW); 
  digitalWrite(ledAzul,HIGH);
  digitalWrite(ledVerde,LOW);
  if(temperatura > 18){
     currentState = estado::estado_B;
    }
  }
  
void funcion_B(){
  digitalWrite(ledRojo,LOW); 
  digitalWrite(ledAzul,LOW);
  digitalWrite(ledVerde,HIGH);
  if(temperatura > 25){
     currentState = estado::estado_C;
     }
  if(temperatura <= 18){
     currentState = estado::estado_A;
     }
  }

  
void funcion_C(){
  digitalWrite(ledRojo,HIGH); 
  digitalWrite(ledAzul,LOW);
  digitalWrite(ledVerde,LOW);
  if(temperatura > 40){
     currentState = estado::estado_D;
  }
  if(temperatura <= 25){
     currentState = estado::estado_B;
  }
  }
  
  
void funcion_D(){
  digitalWrite(ledRojo,HIGH); 
  digitalWrite(ledAzul,HIGH);
  digitalWrite(ledVerde,LOW);
  
  delay(500);
  currentState = estado::estado_B;
  } 

  
