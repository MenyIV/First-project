#include <OneWire.h>

#include <Wire.h>

#include <LiquidCrystal.h>

#include <DallasTemperature.h>
#include <dht.h>
#include <LiquidCrystal_I2C.h>

#define dht_dpin A1 //no ; here. Set equal to channel sensor is on
//#define LCD_LIGHT_PIN A1
#define ONE_WIRE_BUS 9

dht DHT;

OneWire ourWire(ONE_WIRE_BUS);

DallasTemperature sensors(&ourWire);

// inicializuje lcd s definicí připojení na piny.
// MojeLCD1(RS,Enable, D4, D5, D6, D7)

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

boolean buttonState = 0;
const int buttonPin = 8;
// usually the rotary encoders three pins have the ground pin in the middle
enum PinAssignments {
  encoderPinA = 3,   // right (labeled DT on our decoder, yellow wire)
  encoderPinB = 2,   // left (labeled CLK on our decoder, green wire)
  clearButton = 4,    // switch (labeled SW on our decoder, orange wire)
  // connect the +5v and gnd appropriately
};

//volatile unsigned int encoderPos = 0;  // a counter for the dial
float encoderPos = 0;  // a counter for the dial
boolean rele1 = A2;
boolean rele2 = A3;
boolean led1 = 10;
boolean led2 = 11;
boolean led3 = 12;                                                                                                                                                                                                                                                                                                                                             
;
//unsigned int lastReportedPos = 1;   // change management
float lastReportedPos;   // change managemen
float settemp;
int Setup;
static boolean rotating=false;      // debounce management
float teplota0;
float vlhko0;
float teplota1;
float teplota2;
float histereze;
float OFFcool;
float OFFheat;
float oritemp;
float orihum;
float kortemp = -19;    //korekce čidla DHT11
float korhum = 33 ;
float mintemp1;
float maxtemp1;
int mask = 0;

float prumertemp1;
float sbertemp;
int pocetprirustku;


//Casování programu
int n = 100;
unsigned long mycounter;

// interrupt service routine vars
boolean A_set = false;              
boolean B_set = false;


void setup() {
  Serial.begin(9600);  // output
  Serial.println("Meny termostat");
  delay(500);
  Serial.println("spoustim LCD");
  delay(500);
  lcd.begin(16, 2); //inicializace displeje
  Serial.println("spoustim DS18 Teplotni cidlo");
  delay(500);
  sensors.begin();

  // Set the button pin as an input.
  pinMode(buttonPin, INPUT);

  //  // Set the LCD display backlight pin as an output.
  //  pinMode(LCD_LIGHT_PIN, OUTPUT);
  //
  //  // Turn off the LCD backlight.
  //  digitalWrite(LCD_LIGHT_PIN, LOW);


  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(rele1, OUTPUT);
  pinMode(rele2, OUTPUT);//
  //  // Turn off the LCD backlight.
  //  digitalWrite(LCD_LIGHT_PIN, LOW);
  digitalWrite(rele1, LOW);
  digitalWrite(rele2, LOW);
  Setup = 0;
  settemp = 26;
  histereze = 1;
  OFFcool = 15;
  OFFheat = 15;
  mintemp1 = 50 ;
  maxtemp1 = 1;
  pocetprirustku = 1;
  mycounter = millis();

  //  LastReportedPos = 20;

  pinMode(encoderPinA, INPUT_PULLUP); // new method of enabling pullups
  pinMode(encoderPinB, INPUT_PULLUP); 
  pinMode(clearButton, INPUT_PULLUP);
  // turn on pullup resistors (old method)
  //digitalWrite(encoderPinA, HIGH);
  // digitalWrite(encoderPinB, HIGH);
  // digitalWrite(clearButton, HIGH);

  // encoder pin on interrupt 0 (pin 2)        vpravo + SW
  attachInterrupt(0, doEncoderA, CHANGE);
  // encoder pin on interrupt 1 (pin 3)         vlevo - SWW
  attachInterrupt(1, doEncoderB, CHANGE);




}
void loop(){
  //////////////////////////////////////////////////////////////////////////////////////////////////
  //
  //
  //načítání teplot  DS18 a DHT11
  //
  ///////////////////////////////////////////////////////////////////////////////////////////////////

  if (Setup == 0){
    Serial.println();
    Serial.print("Requesting temperature...");
    sensors.requestTemperatures(); // Send the command to get temperatures
    Serial.println("DONE");


    Serial.print("Device 1 (index 0) = ");
    Serial.print(sensors.getTempCByIndex(0));
    Serial.println(" Degrees C");
    Serial.print("Device 1 (index 0) = ");
    Serial.print(sensors.getTempFByIndex(0));
    Serial.println(" Degrees F");
    teplota1 = sensors.getTempCByIndex(0);



    Serial.print("Device 2 (index 0) = ");
    Serial.print(sensors.getTempCByIndex(1));
    Serial.println(" Degrees C");
    Serial.print("Device 2 (index 0) = ");
    Serial.print(sensors.getTempFByIndex(1));
    Serial.println(" Degrees F");
    teplota2 = sensors.getTempCByIndex(1);


    DHT.read11(dht_dpin);

    teplota0 = DHT.temperature;
    vlhko0 = DHT.humidity;

    Serial.print("Current humidity = ");
    orihum = teplota0 + korhum;
    Serial.print(orihum);
    Serial.print("%  ");
    Serial.print("temperature = ");
    oritemp = vlhko0 + kortemp;
    Serial.print(oritemp); 
    Serial.println("°C  ");

  }


  /////////////////////////////////////////////////////////////////////////////////////////
  //
  //
  // MENU setup 0 == vyplé Default
  // MENU setup 1 == statistiky
  // MENU setup 2 == nastavování teploty SetupON   -  rozdělit na masku a funkci
  //
  //
  /////////////////////////////////////////////////////////////////////////////////////


  if (digitalRead(clearButton) == LOW)  
  {        //klik SW
    Serial.println("SW klik");
    Setup = Setup + 1;
    Serial.println("Setup=");
    Serial.print(Setup);
    delay(500);
  }

  if (Setup == 2 && mask == 0) 
  {  
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("nastav teplotu:");
    lcd.setCursor(0, 1);
    lcd.print("T");
    lcd.setCursor(1, 1);
    lcd.print(teplota1);
    mask = 1;
  }
  if (Setup == 2){        //klik SW
                                            // ukončení nastavení zapíše settemp
    settemp = lastReportedPos;
//    delay (750);
  }
  if (Setup == 2){       //když je zaplý setup ano1 
    rotating = true;  // reset the debouncer
    if (encoderPos > 50 ){
      //    encoderPos = 0;
      lastReportedPos = 49;             //nepustí pod TsetMAX      koncák
    }
    if (encoderPos < -20 ){
      //    encoderPos = 0;
      lastReportedPos = -19;             //nepustí pod TsetMIN      koncák
    }
    else
    {
      if (lastReportedPos != encoderPos && encoderPos<50 && encoderPos>-19 ) {   //když se změní pozice a zároveň je v intervalu Tmin Tma
        Serial.print("Index:");                                                   // vypíše do konzole a přepíše poslední
        Serial.println(encoderPos, DEC);                        //pozici do aktuální
        Serial.println(lastReportedPos);
        lastReportedPos = encoderPos;
      }
    lcd.setCursor(8, 1);
    lcd.println(lastReportedPos);
    }
  }

  if (Setup == 3 )  {        //klik SW
    Serial.println("KONEC MENU zapis HODNOT");
    Setup = 0;                                                  // ukončení nastavení zapíše settemp
    settemp = lastReportedPos;
    encoderPos=lastReportedPos;
    mask = 0;
    delay (500);

  }


  //////////////////////////////////////////////////////////////////////////////////////////////////
  //
  //
  //
  //podsvícení LCD
  //
  ///////////////////////////////////////////////////////////////////////////////////////////////////

  //  if (Setup == 0);
  //  {
  //    // buttonState = digitalRead(buttonPin);         //načte tlačítko podsvícení a rozsvítí
  //    if (digitalRead(buttonPin) == 0)               //LCD backlight ON OFF
  //    {
  //      digitalWrite(LCD_LIGHT_PIN, LOW);
  //      Serial.println("Backlight OFF");
  //    }
  //    if (digitalRead(buttonPin) == 1)
  //    {
  //      digitalWrite(LCD_LIGHT_PIN, HIGH);
  //      Serial.println("Backlight ON");
  //    }
  //  }
  //////////////////////////////////////////////////////////////////////////////////////////////////
  //
  //
  //
  //statistika teplot   vyhodnocení pouze jednou za čas - dodělat
  //
  ///////////////////////////////////////////////////////////////////////////////////////////////////
  if (millis() <= mycounter) {
    if (teplota1 > maxtemp1);
    {       //maximum
      maxtemp1 = teplota1;
      Serial.println ("maximum");
      Serial.println (maxtemp1);
    }
    if (teplota1 < mintemp1);
    {         //minimum
      mintemp1 = teplota1;
      Serial.println ("minimum");
      Serial.println (mintemp1);
    }

    sbertemp = sbertemp + teplota1;
    prumertemp1 = sbertemp / pocetprirustku;
    pocetprirustku = pocetprirustku + 1;

    mycounter = mycounter + 300000;
  }

  //////////////////////////////////////////////////////////////////////////////////////////////////
  //
  //
  //
  //výpis na LCD
  //
  ///////////////////////////////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////111111111111111/////////////////////////////////////////////

  if (Setup == 0){        //pokud je setup vyplý tak se vypisuje nějakej text na LCD
    lcd.clear();                        ///
    lcd.setCursor(0, 0);                //  /T1   T3    Tset             S
    lcd.print("A");  
    lcd.setCursor(1, 0);
    lcd.print(teplota1);
    ///
    //  /T2   Hu    Tset-T1          S
    lcd.setCursor(0, 1);                ///
    lcd.print("B");
    lcd.setCursor(1, 1);
    lcd.print(teplota2);

    lcd.setCursor(7, 0);
    lcd.print("V");
    lcd.setCursor(8, 0);                 // není to celý chyběj čidla
    lcd.print(oritemp);
    lcd.setCursor(10, 0);                 // není to celý chyběj čidla
    lcd.print("H");
    lcd.setCursor(11, 0);                 // není to celý chyběj čidla
    lcd.print(orihum);
   

    lcd.setCursor(7, 1);
    lcd.print("S");
    lcd.setCursor(9, 1);                 // není to celý chyběj čidla
    lcd.print(settemp);

    lcd.setCursor(13, 0);                  //sem vrazit šipky
    lcd.print("   ");
    lcd.setCursor(15, 1);                  //sem vrazit šipky
    lcd.print("");

    lcd.setCursor(15, 0);
    lcd.print("");
    delay(1000);
  }
  ///////////////////////////////////////222222222222/////////////////////////////////////////////
  if (Setup == 1){        
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("L:");  
    lcd.setCursor(2, 0);
    lcd.print(mintemp1);
    lcd.setCursor(0, 1);
    lcd.print("H:");
    lcd.setCursor(2, 1);
    lcd.print(maxtemp1);
    lcd.setCursor(8, 0);
    lcd.print("P");                                          //vlozit specialni znak prumeru
    lcd.setCursor(9, 0);
    lcd.print(prumertemp1);
    //  lcd.setCursor(10, 0);
    //  lcd.print("H");
    //  lcd.setCursor(11, 0);
    //   lcd.print(orihum);
    //   lcd.println("  ");
    lcd.setCursor(7, 1);
    lcd.print("S");
    lcd.setCursor(9, 1);
    lcd.print(settemp);
    //  lcd.setCursor(13, 0);         
    //  lcd.print("   ");
    lcd.setCursor(15, 1);                  //sem vrazit šipky
    lcd.print("");
    lcd.setCursor(15, 0);             //sem vrazit šipky
    lcd.print("");
    delay(1000);
  }


  //////////////////////////////////////////////////////////////////////////////////////////////////
  //
  //
  //
  //funkce termostatu
  //
  ///////////////////////////////////////////////////////////////////////////////////////////////////
  if (Setup == 0){//(Setup == 0 || Setup == 1 ){
    if(teplota1 <= settemp-histereze && teplota1 < OFFheat)  
    {// čeká na samovolné ohřátí
      digitalWrite(led1, LOW);
      digitalWrite(led2, HIGH);
      digitalWrite(led3, LOW);
      Serial.print ("cekam na teplo");
      digitalWrite(rele1, HIGH);
      digitalWrite(rele2, HIGH);
      Serial.println("LOW LOW ceka na minimalni teplotu ohřevu");
      //      delay (1000);
      //      digitalWrite(led2, 0);
      //      delay (1000);

    }


    if(teplota1 <= settemp-histereze && teplota1 > OFFheat) { 
      // topí
      digitalWrite(led1, HIGH);
      digitalWrite(led2, LOW);
      digitalWrite(led3, LOW);
      Serial.print ("topim");
      digitalWrite(rele1, HIGH);
      digitalWrite(rele2, LOW);
      Serial.print("Rele2,HIGH topí");
      //      delay (1000);

    }

    if(teplota1 >= settemp+histereze && teplota1 <OFFcool) {   
      //chladí
      digitalWrite(led1,LOW);
      digitalWrite(led2, LOW);
      digitalWrite(led3, HIGH);
      Serial.print ("chladim");
      digitalWrite(rele1, LOW);
      digitalWrite(rele2, HIGH);
      Serial.println("Rele1,HIGH chladi");
      //      delay (1000);

    }
    if(teplota1 >= settemp+histereze && teplota1 > OFFcool) { 
      //čeká na chladnutí 
      digitalWrite(led1,LOW);
      digitalWrite(led2, HIGH);
      digitalWrite(led3, LOW);
      Serial.println ("cekam na chlad minimalni teplotu chlazeni");
      digitalWrite(rele1, HIGH);
      digitalWrite(rele2, HIGH);
      //      delay (1000);

    }

    if (teplota1 <= settemp-histereze && teplota1 <= OFFheat) //nedělá nic
    {

      digitalWrite(led1,LOW);
      digitalWrite(led2, HIGH);
      digitalWrite(led3, LOW);
      Serial.print ("cekam na hodnotu");
      //      delay (1000);
    }
    //    if (teplota1 >= settemp+histereze){
    //      //    digitalWrite(led1, 1);
    //      //    digitalWrite(led2, 1);
    //      //    digitalWrite(led3, 0);
    //      Serial.print ("přetopeno");
    //    digitalWrite(rele1, LOW);
    //    digitalWrite(rele2, LOW);

    //
    //    }
    //    if (teplota1 <= settemp-histereze){
    //      //    digitalWrite(led1, O);
    //      //    digitalWrite(led2, 1);
    //      //    digitalWrite(led3, 1);
    //      Serial.print ("přechlazeno");
    //        digitalWrite(rele1, LOW);
    //        digitalWrite(rele2, LOW);
    //
    //
    //    }




  }


}







// Interrupt on A changing state    změna stavu kroutítka
void doEncoderA(){
  // debounce
  if ( rotating ) delay (1);  // wait a little until the bouncing is done

  // Test transition, did things really change? 
  if( digitalRead(encoderPinA) != A_set ) {  // debounce once more
    A_set = !A_set;

    // adjust counter + if A leads B
    if ( A_set && !B_set && encoderPos<50 ) 
      encoderPos += .2;

    rotating = false;  // no more debouncing until loop() hits again
  }
}

// Interrupt on B changing state, same as A above
void doEncoderB(){
  if ( rotating ) delay (1);
  if( digitalRead(encoderPinB) != B_set ) {
    B_set = !B_set;
    //  adjust counter - 1 if B leads A
    if( B_set && !A_set  && encoderPos>-20 ) 
      encoderPos -= .2;

    rotating = false;
  }
}


