//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Programa para el control de la asistencia en las reuniones de IEEE 2018/2019 ////////////////////////////
///////////////////////////////////// by Arturo P. /////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
 Conexiones:
  Módulo RC522, de izquierda a derecha:
   * 3.3V
   * Pin 9
   * Gnd
   * Libre
   * Pin 12
   * Pin 11
   * Pin 13
   * Pin 10

  Leds
  * Rojo  Pin A0
  * Verde Pin A1
  
  LCD 
  * Vss   Gnd
  * Vdd   5V
  * Vo    Potenciometro pin solo
  * RS    Pin 2
  * RW    Gnd
  * E     Pin 3
  * D0-D3 Libre
  * D4-D7 Pin 4-7
  * A     5V
  * K     Gnd
  * Conectar potenciometro a 5V y Gnd
 */
 
#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal.h>

#define RST_PIN  9    //Pin 9 para el reset del RC522
#define SS_PIN  10   //Pin 10 para el SS (SDA) del RC522
MFRC522 mfrc522(SS_PIN, RST_PIN); //Creamos el objeto para el RC522

LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

#define red A0
#define green A1
int a=0;

void setup() {  
  lcd.begin(16, 2);  //Iniciamos la pantalla LCD
  
  Serial.begin(9600); //Iniciamos La comunicacion serial
  SPI.begin();        //Iniciamos el Bus SPI
  mfrc522.PCD_Init(); // Iniciamos el MFRC522
  Serial.println("Control de asistencia:");

  //Definimos los leds y los dejamos apagados
  pinMode(red,OUTPUT);
  pinMode(green,OUTPUT);
  digitalWrite(red,LOW);
  digitalWrite(green,LOW);
  
  lcd.print("Pase su tarjeta");
}

byte ActualUID[4]; //almacenará el código del Tag leído

//Codigos de los usuarios
byte Ejemplo1[4]= {0x9A, 0x64, 0x7C, 0x59} ;
byte Ejemplo2[4]= {0x9C, 0x64, 0x7C, 0x59} ;


void loop() {

  // Revisamos si hay nuevas tarjetas  presentes
  if ( mfrc522.PICC_IsNewCardPresent()) 
        {  
      //Seleccionamos una tarjeta
            if ( mfrc522.PICC_ReadCardSerial()) 
            {
                  // Enviamos serialemente su UID
                  for (byte i = 0; i < 4; i++) {
                          ActualUID[i]=mfrc522.uid.uidByte[i];          
                  }               
                  //comparamos los UID para determinar si es uno de nuestros usuarios  
                  if(compareArray(ActualUID,Ejemplo1))
                    texto("Ejemplo Número Uno");
                  else if(compareArray(ActualUID,Ejemplo2))
                    texto("Ejemplo Número Dos");
                 else if(a==0){
                    lcd.setCursor(0, 0);
                    lcd.print("Tarjeta no     ");
                    lcd.setCursor(0, 1);
                    lcd.print("valida          ");
                    digitalWrite(red,HIGH);
                    delay(500);
                    digitalWrite(red,LOW);
                    delay(100);
                    digitalWrite(red,HIGH);
                    delay(400);
                    digitalWrite(red,LOW);
                    lcd.setCursor(0, 0);
                    lcd.print("Pase su tarjeta");
                    lcd.setCursor(0, 1);
                    lcd.print("                         ");
                  }
                  // Terminamos la lectura de la tarjeta tarjeta  actual
                  mfrc522.PICC_HaltA();
                  a=0;
            }

  }

}
//Funcion para el texto una vez pasada la targeta
  void texto(char *nombre){
    Serial.println(nombre);
    lcd.setCursor(0, 0);
    lcd.print("Bienvenido     ");
    lcd.setCursor(0, 1);
    lcd.print(nombre);
    digitalWrite(green,HIGH);
    delay(1500);
    digitalWrite(green,LOW);
    lcd.setCursor(0, 0);
    lcd.print("Pase su tarjeta");
    lcd.setCursor(0, 1);
    lcd.print("                         ");
    a=1;
  }

//Función para comparar dos vectores
 boolean compareArray(byte array1[],byte array2[])
{
  if(array1[0] != array2[0])return(false);
  if(array1[1] != array2[1])return(false);
  if(array1[2] != array2[2])return(false);
  if(array1[3] != array2[3])return(false);
  return(true);
}
