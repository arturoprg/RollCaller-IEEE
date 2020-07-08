//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Programa para el control de la asistencia en las reuniones de IEEE ///////////////
//////////////////////////////////// by Arturo P. ///////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
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

#include <SPI.h>            //Libreria Lector
#include <MFRC522.h>       //Libreria Lector
#include <LiquidCrystal.h> //Libreria LCD

#define RST_PIN  9    //Pin 9 para el reset del RC522
#define SS_PIN  10   //Pin 10 para el SS (SDA) del RC522
MFRC522 mfrc522(SS_PIN, RST_PIN); //Creamos el objeto para el RC522

LiquidCrystal lcd(2, 3, 4, 5, 6, 7);//Pines de la LCD

#define red A0
#define green A1
char lee;
int cont = 0;
byte ActualUID[4];

//Funcion para escribir en la LCD
void escribirLCD(char *linea0,char *linea1){
  lcd.setCursor(0, 0);
  lcd.print(linea0);
  lcd.setCursor(0, 1);
  lcd.print(linea1);
}

boolean compareArray(byte array1[],byte array2[])
{
  if(array1[0] != array2[0])return(false);
  if(array1[1] != array2[1])return(false);
  if(array1[2] != array2[2])return(false);
  if(array1[3] != array2[3])return(false);
  return(true);
}

//Esta función enciende el led rojo y dice Targeta no valida 
void ledRojo(){
  escribirLCD("Tarjeta no      ","registrada      ");
  digitalWrite(red,HIGH);
  delay(500);
  digitalWrite(red,LOW);
  delay(100);
  digitalWrite(red,HIGH);
  delay(400);
  digitalWrite(red,LOW);
}

void setup() { //definimos las entradas e iniciamos Serial, LCD y Lector 
  
  lcd.begin(16, 2); //Iniciamos LCD
  Serial.begin(9600); //Iniciamos La comunicacion serial
  SPI.begin();        //Iniciamos el Bus SPI
  mfrc522.PCD_Init(); // Iniciamos el MFRC522
  
  pinMode(red,OUTPUT);
  pinMode(green,OUTPUT);
  digitalWrite(red,LOW);
  digitalWrite(green,LOW);

  escribirLCD("Pase su tarjeta"," ");
}

void loop() {
  if (Serial.available() > 0){    // Mientras recibe de Python
    lee = Serial.read();          // Introduce lo que lee
    if (lee == '1'){              // Si recibe un 1 significa que no esta registrado el codigo
      ledRojo();
    }
    else if (lee == '2'){        // Si recibe un 2 significa que ha surgido algún error mientras funcionaba
      escribirLCD("Ha surgido un   ","error. Espere...");
      while (true){             // Entra en un bucle infinito (se reinicia al reiniciar Python
        digitalWrite(red,HIGH);
        delay(2000);
        digitalWrite(red,LOW);
        delay(2000);
      }
    }
    else if (cont == 0){      // Si es la primera letra, limpia la LCD
      escribirLCD("Buenas          ","                ");
      digitalWrite(green,HIGH);
      lcd.setCursor(cont, 1);
      lcd.print(lee);
      cont = cont+1;
    }
    else{                 // Escribe el resto de letras
      lcd.setCursor(cont, 1);
      lcd.print(lee);
      cont = cont+1;
    }
  }
  else{                 // Si deja de recibir, vuelve a la posicion de leer una tarjeta
    cont = 0;
    if (lee != '1'){
      digitalWrite(green,LOW);
      delay(1000);
      escribirLCD("Pase su tarjeta","             ");
    }
  }
  // Enviamos la tarjeta leida
  if ( mfrc522.PICC_IsNewCardPresent()) 
        {  
      //Seleccionamos una tarjeta
            if ( mfrc522.PICC_ReadCardSerial()&&!compareArray(ActualUID,mfrc522.uid.uidByte))
            {
                  // Enviamos serialemente su UID
                  for (byte i = 0; i < 4; i++) {
                          ActualUID[i]=mfrc522.uid.uidByte[i];
                          Serial.println(int(ActualUID[i]));
                  }
                  // Terminamos la lectura de la tarjeta tarjeta  actual
                  mfrc522.PICC_HaltA();    
                  }
            }

      }
