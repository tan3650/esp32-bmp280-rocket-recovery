#include <Wire.h>
#include <Adafruit_BMP280.h>
const int presspin = 21;
const int ledpin = 35;
Adafruit_BMP280 bmp; // I2C
//Adafruit_BMP280 bmp(BMP_CS); // hardware SPI
//Adafruit_BMP280 bmp(BMP_CS, BMP_MOSI, BMP_MISO,  BMP_SCK);

void setup() {
  pinMode(presspin, OUTPUT);
  digitalWrite(presspin, LOW);
  pinMode(ledpin, OUTPUT);
  digitalWrite(ledpin, LOW);
 // pinMode(buzzpin, OUTPUT);`
  //digitalWrite(buzzpin, LOW);
  Serial.begin(115200);
  // wait for native usb
  Serial.println(F("BMP280 test"));
  unsigned status;
  //status = bmp.begin(BMP280_ADDRESS_ALT, BMP280_CHIPID);
  status = bmp.begin(0x76);
  if (!status) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring or "
                     "try a different address!"));
    Serial.print("SensorID was: 0x");
    Serial.println(bmp.sensorID(), 16);
    Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
    Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
    Serial.print("        ID of 0x60 represents a BME 280.\n");
    Serial.print("        ID of 0x61 represents a BME 680.\n");
    while (1) delay(10);
  }

  /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X2,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X2,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
}
float curalt = 0; //cuz alr 600+ at my lvl
float curpress2,prevpress1,prevpress2,curpress1; 
float avg1 = 0;
float avg2 = 0;
int i, x;
int constred = 65;
float a=60;
int t=0;
int b=0;

/*
prevpress/10000
9. 3 450 round(9.345) 9 3,5 no noise
9. 5 450 round(9.5450) 10 10>9
*/
void loop() {
   prevpress1 = bmp.readPressure();     //pressure will dec as going up therefore curpress2<prevpress2 therefore press_diff -ve asel 
   prevpress2 = bmp.readPressure();    
   for (int i = 0; i < constred; i++) 
   {if(i%2==0)                                     
    {curpress1=bmp.readPressure();
    float pressure_diff1 = curpress1 - prevpress1;
    avg1 = avg1 + (pressure_diff1);
    prevpress1=curpress1;
    delay(10);
    }
    else
    printf("...");}                        // cuz alternate press readings so 200
   avg1 = (avg1/constred);              //avg of constred readings

   for (int x = 0; x < constred; x++) 
   {curpress2=bmp.readPressure();
   float pressure_diff2 = curpress2 - prevpress2;      //pressure will inc as going down therefore curpress1>prevpress1 therefore press_diff +ve asel 
    avg2 = avg2 + (pressure_diff2);
    prevpress2=curpress2;
    delay(10);}                        //consecutive
   avg2 = (avg2/constred);
  

   while (avg2>avg1 && (avg2-avg1)>a && b==0)  // && (avg2-avg1)>220 && curalt >= 1100 same thing just its avg over constred amt of readings is being compared. && curalt <= 900
   {t++;  
    if (t>=30)
    {
    digitalWrite(presspin, HIGH);
    digitalWrite(ledpin, HIGH);
    delay(4000);
    digitalWrite(presspin, LOW);
    digitalWrite(ledpin, LOW);
    b++;
    printf("apogee detected");}}

   avg1=avg2;            //update to latest
   Serial.print(avg1);
   Serial.print(avg2);
   Serial.println();
   Serial.print(bmp.readAltitude()); /* Adjusted to local forecast! */

   delay(10);
}
