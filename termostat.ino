/*
  Autor: MenyIV
  Web: 
  G+:
*/

//definice knihoven a komplexních proměnných
#include <LiquidCrystal.h>                        //knihovna LCD
#define LCD_LIGHT_PIN A1                          //PIN LCD Svetla
#include <dht11.h>                                //knihovna čidla DHT11
#define DHT11PIN 2                                //PIN DHT CIDLA
dht11 DHT11;
#define resistance 1                            //hystereze
/* DOPLNIT   JAK  JE P5IPOJEN měl bych to mít v programu s počítadlem času tak jsem tam přidal tlačítko na zhaínání
 * LCD R/W pin to ground
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)
         LCD(RS,Enable, D4, D5, D6, D7)
 */
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);            // zapojení LCD JHD – 162A
boolean rel1;                                     //stav RELE   rel1= chlazení; rel2=topení
boolean rel2;
unsigned char settemp;                            //proměnná pro nastavenou teplotu
unsigned char gettemp1;                           //teplota 1/VENKU+vlhkost 2/UVNITR 3/PONORAK  
char gethum1;
unsigned char gettemp2;                              
unsigned char gettemp3;           
const int buttonPin = 8;        //tlčítko pro zapnutí podsvícení
int buttonState = 0;            //stav tlačítka podsvícení



float set=20.0;                                   //proměnné
int n=0;

void setup() {
  lcd.begin(16, 2);
  
    // Set the button pin as an input.
  pinMode(buttonPin, INPUT);

   // Set the LCD display backlight pin as an output.
  pinMode(LCD_LIGHT_PIN, OUTPUT);

  // Turn off the LCD backlight.
  digitalWrite(LCD_LIGHT_PIN, LOW);
  
  
  pinMode(11,OUTPUT); //rel1   - PIN7
  pinMode(12,OUTPUT); //rel2   - PIN8
  
//CIDLA 1-DHT11  2-DS18B20  3-DS18B20 waterproof
  pinMode(A0,INPUT); // hum senzor
  pinMode(A1,INPUT); // gettemp1
  pinMode(A2,INPUT); // gettemp2
  pinMode(A3,INPUT); // gettemp3
  
//VSTUP TLACITKO KY-040
  pinMode(12,INPUT); // směr +
  pinMode(11,INPUT); // směr -
  pinMode(10,INPUT); //SW tlačítko  

//PODSVICENI LCD
  pinMode(9,INPUT); //TLACITKO
  
}



void loop() {                         //programová smyčka
//načtení čidel a vypsání jejich hodnot do serialu

//DHT11
  Serial.println("\n");
  int chk = DHT11.read(DHT11PIN);
  Serial.print("Read sensor: ");
  switch (chk)
  {
    case 0: Serial.println("OK"); break;
    case -1: Serial.println("Chyba souctu"); break;
    case -2: Serial.println("Cas vyprsel"); break;
    default: Serial.println("Neznámá chyba"); break;
  }
  gettemp1 = dht.readTemperature();     //zde možno upravit přepočet pokud nesedí výstup z čidla
  gethum1 = dht.readHumidity();
Serial.println("vnejsi cidlo DHT11:")
Serial.println("vnejsi teplota:")
Serial.println(gettemp1)
Serial.println("vnejsi vlhkost:")
Serial.println(gethum1)

//DS18B20

//podsvícení displeje zapnou/vypnout
buttonState = digitalRead(buttonPin);
Serial.println("Podsvícení LCD ON=1 OFF=0");
Serial.println(buttonState);
  if (buttonState == 1)
{
    digitalWrite(LCD_LIGHT_PIN, HIGH);
}
if (buttonState == 0)
{
 digitalWrite(LCD_LIGHT_PIN, LOW);
}


if digitaRead(     //pokud je tlačítko na podsvícení on tak on jinak off

if digitalRead(10,HIGH) 
 {
 }                 //no při kliku na SW nakopni smyčku ve které poběží nastavení. Ukončení a uložení hodnot opět klikem. 
                    // pokud přeteče tak spadne zpět do smyčky bez uložení hodnot A settime na 30-60s 


if (millis() > (1000 + lastJob1))
   {
    // zde bude kód programu, který se v mém případě
    // vykoná každou 1 vteřinu (1000 ms)
    // no takže zde bude celý program který načte čidla vypíše na display a vrazíto do relátek  hej
     
   
   
    // když je digitální pin 0 tak na něj zapiš 1
    if (digitalRead(LED1) == 0) digitalWrite(LED1, HIGH);
    // jinak zapiš 0
    else digitalWrite(LED1, LOW);
   
   
   



}

// výpis na display   -- nutné odzkoušet a porovnat aby to kněčemu vypadalo

 {}
