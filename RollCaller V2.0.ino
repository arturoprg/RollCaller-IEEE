//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Programa para el control de la asistencia en las reuniones de IEEE 2018/2019 ////
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

 //Introducimos las librerias que se usaran
#include <SPI.h>            //Libreria Lector
#include <MFRC522.h>       //Libreria Lector
#include <LiquidCrystal.h> //Libreria LCD

//Defino los pines
#define RST_PIN  9    //Pin 9 para el reset del RC522
#define SS_PIN  10   //Pin 10 para el SS (SDA) del RC522
MFRC522 mfrc522(SS_PIN, RST_PIN); //Creamos el objeto para el RC522

LiquidCrystal lcd(2, 3, 4, 5, 6, 7);//Pines de la LCD

#define red A0
#define green A1

//Defino las variables
char *Nombre; //donde se almacenará el nombre de la persona que ha pasado el codigo

byte ActualUID[4]; //almacenará el código del Tag leído

//Matriz con todos los códigos
byte Codigos[][4]=  {{0x9C, 0x64, 0x7C, 0x59},//Ejemplo Número Uno
                    {0x9A, 0x64, 0x7C, 0x59}};//Ejemplo Número Dos

//Nombres de los miembros
char *Nombres[]={"Ejemplo Número Uno",
                 "Ejemplo Número Dos"};

byte tam=sizeof(Nombres)/2; //Numero de nombres que hay

//Creo las funciones

//Funcion que compara 2 vectores
boolean compareArray(byte array1[],byte array2[])
{
  if(array1[0] != array2[0])return(false);
  if(array1[1] != array2[1])return(false);
  if(array1[2] != array2[2])return(false);
  if(array1[3] != array2[3])return(false);
  return(true);
}

//Funcion que compara 1vector y una fila de una matriz
boolean compareMatrix(byte array1[],byte array2[][4],byte j)
{
  if(array1[0] != array2[j][0])return(false);
  if(array1[1] != array2[j][1])return(false);
  if(array1[2] != array2[j][2])return(false);
  if(array1[3] != array2[j][3])return(false);
  return(true);
}

//Funcion para escribir en la LCD
void escribirLCD(char *linea0,char *linea1){
  lcd.setCursor(0, 0);
  lcd.print(linea0);
  lcd.setCursor(0, 1);
  lcd.print(linea1);
}

//Funcion para el texto una vez pasada la targeta
  void texto(){
    escribirLCD("Bienvenido     ",Nombre);
    digitalWrite(green,HIGH);
    delay(1500);
    digitalWrite(green,LOW);
    escribirLCD("Pase su tarjeta","                  ");
  }

//Quita las tildes de las letras
  void change(char *msg){
    byte  i=0;
    bool  a=false;
    byte  negativo[10]={127,119,115,109,102,95,87,83,77,70}; //son los numeros en valor absoluto que caracterizan las letras con tilde
    char  letra[10]={'A','E','I','O','U','a','e','i','o','u'};  //Vector de vocales sin tilde
 
    while (msg[i]!=0){  //Mientras haya texto...
      if (msg[i]<0){    //Si el caracter tiene tilde (es negativo)
        strcpy(&msg[i],&msg[i+1]);  //cambia el caracter por el caracter siguiente, puesto las letras con tilde genera 2 caracteres
        for (byte j=0;j<10;j++)   //Comprueba que vocal con tilde es y la sustituye por la vocal sin tilde
          if (msg[i]==-negativo[j]) msg[i]=letra[j];
      }
      //Vamos ha hacer que corte el texto cuando llegue al segundo espacio del texto
      if (msg[i]==' ' && a){  //Si llega a un espacio y ya ha pasado por un espacio...
        msg[i]=0;
        break;
        }
        
      if (msg[i]==' ')    //Si llega a un espacio...
        a=true;
        
      i++;
    }
    
  }
  
//Esta función enciende el led rojo y dice Targeta no valida 
void ledRojo(){
  escribirLCD("Tarjeta no         ","valida              ");
  digitalWrite(red,HIGH);
  delay(500);
  digitalWrite(red,LOW);
  delay(100);
  digitalWrite(red,HIGH);
  delay(400);
  digitalWrite(red,LOW);
  escribirLCD("Pase su tarjeta","             ");
}



void setup() { //definimos las entradas e iniciamos Serial, LCD y Lector 
  lcd.begin(16, 2); //Iniciamos LCD
  
  Serial.begin(9600); //Iniciamos La comunicacion serial
  SPI.begin();        //Iniciamos el Bus SPI
  mfrc522.PCD_Init(); // Iniciamos el MFRC522
  
  Serial.println("Control de asistencia:");
  
  pinMode(red,OUTPUT);
  pinMode(green,OUTPUT);
  digitalWrite(red,LOW);
  digitalWrite(green,LOW);

  escribirLCD("Pase su tarjeta"," ");
}


void loop() {
  // Revisamos si hay nuevas tarjetas  presentes
  if ( mfrc522.PICC_IsNewCardPresent()) 
        {  
      //Seleccionamos una tarjeta
            if ( mfrc522.PICC_ReadCardSerial()&&!compareArray(ActualUID,mfrc522.uid.uidByte))
            {
                  // Enviamos serialemente su UID
                  for (byte i = 0; i < 4; i++) {
                          ActualUID[i]=mfrc522.uid.uidByte[i];          
                  }               
                  //comparamos los UID para determinar si es uno de nuestros usuarios  
                  for (int j=0;j<tam;j++){ 
                    if(compareMatrix(ActualUID,Codigos,j)){
                        Nombre=Nombres[j]; 
                        Serial.println(Nombre);
                        change(Nombre);
                        texto();
                        break;
                    }
                    
                    else if(j==tam-1) ledRojo();
                  
                  // Terminamos la lectura de la tarjeta tarjeta  actual
                  mfrc522.PICC_HaltA();    
                  }
            }
      }
}




  
