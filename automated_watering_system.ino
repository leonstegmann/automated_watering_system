//include Libraries for OLED Display 
#include <Adafruit_GFX.h>
#include <Adafruit_GrayOLED.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_SPITFT_Macros.h>
#include <gfxfont.h>
#include <Adafruit_SSD1306.h>
#include <splash.h>

//include Wire Library for I2C-Bus
#include <Wire.h>

//include Lichtsensor Library for class-functions
#include <BH1750.h>
BH1750 lightMeter (0x23); //HEX-Adress for the I2C-Bus

//OLED
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

//Transistor for PUMP
#define Transistor 13
bool pump_power = 0;

//Waterlevelsensor (needs Analog Pin)
#define water_level_sensor  A0
float waterlevel;


void setup(){

//PIN SETUP
  pinMode(Transistor, OUTPUT );
  digitalWrite(Transistor,LOW);
//CAN_BUS SETUP USRP, Arduino as MASTER
  Wire.begin();//(SDA 4, SCL 5) //for Slave would be zb. Wire.begin(0x8)
//OLED_DISPLAY Setup
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // mit I2C-Adresse 0x3c initialisieren
//Lichtsensor SETUP
  lightMeter.begin();
//OPEN SERIAL COMMUNICATION
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Native USB only
  }
 
//Waterlevel Sensor Setup
  pinMode(water_level_sensor, INPUT);

}


void loop() 
{ 
  float lux = lightMeter.readLightLevel();
  waterlevel = analogRead(water_level_sensor)/10.24;
  OLED_actual(lux,waterlevel,false);
  PUMP(waterlevel,lux);
  delay(3000);
}

void PUMP(float waterlevel, float lux){ //bei acteller Schlauchlänge knapp 400ml bei 10s ON
  if (lux <=20 && waterlevel >= 30){
    digitalWrite(Transistor, HIGH);
     OLED_actual(lux,waterlevel,true);  
    delay(10000);
    digitalWrite(Transistor, LOW);
  }
}

void OLED_actual(float lux, float waterlevel,bool pump_power){
  
  //Display Setup
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);

  //Lux  
  display.setCursor(0,0); // (Spalte | Zeile)
  display.println("Lux");
  display.setCursor(33,0);
  display.println(lux);

  //Waterlevel
  display.setCursor(0,10);
  display.println("Wlvl");
  display.setCursor(33,10);
  display.println(waterlevel);
  
  //Power Pumpe
  display.setCursor(0,20);
  display.println("Power");
  display.setCursor(37,20);
  if (pump_power== true){  
    display.println("ON");
  }
  else {
    display.println("OFF");  
  }
  
  display.display(); //Display aktualisieren
}
