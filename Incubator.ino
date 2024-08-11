#include "DHT.h"
#include <LiquidCrystal.h>
LiquidCrystal lcd(6,8,5,4,3,2);
#define DHTPIN 7   // what pin we're connected to
const int lm35_pin = A1;	

#define DHTTYPE DHT11 // DHT 11 

#define pwm 9 

byte degree[8] = 
              {
                0b00011,
                0b00011,
                0b00000,
                0b00000,
                0b00000,
                0b00000,
                0b00000,
                0b00000
              };
// Initialize DHT sensor for normal 16mhz Arduino
DHT dht(DHTPIN, DHTTYPE);

//fan_pw
int sensorPin = A0;
int PWM = 3;
int sensorVal;

// put your main code here, to run repeatedly:


void setup() {
 
 pinMode(sensorPin, INPUT) ; 
 pinMode(PWM, OUTPUT) ; 
 lcd.begin(16, 2);
 lcd.createChar(1, degree);
 lcd.clear();
 lcd.print("   Fan Speed  ");
 lcd.setCursor(0,1);
 lcd.print("  Controlling ");
 delay(2000);
 analogWrite(pwm, 255);
 lcd.clear();
 delay(2000);
 Serial.begin(9600); 
 dht.begin();
}

void loop() {
  // Wait a few seconds between measurements.
  delay(2000);

  
  sensorVal = analogRead(sensorPin) ;
  Serial.print(sensorVal) ;
  delay(1000);

  int temp_adc_val;
  float temp_val;
  temp_adc_val = analogRead(lm35_pin);	/* Read Temperature */
  temp_val = (temp_adc_val * 4.88);	/* Convert adc value to equivalent voltage */
  temp_val = (temp_val/10);	/* LM35 gives output of 10mv/°C */
  Serial.print("Temperature = ");
  Serial.print(temp_val);
  Serial.print(" Degree Celsius\n");
  delay(1000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius
  //float t = dht.readTemperature();
  // Read temperature as Fahrenheit
  float f = dht.readTemperature(true);
  
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(temp_val) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Compute heat index
  // Must send in t in Fahrenheit!
  float hi = dht.computeHeatIndex(f, h);

  Serial.print("Humidity: "); 
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("temperature: "); 
  Serial.print(temp_val);
  Serial.print(" *C ");
  Serial.print(f);
  Serial.print(" *F\t");
  Serial.print("Heat index: ");
  Serial.print(hi);
  Serial.println(" *F");

    lcd.setCursor(0,0);
  lcd.print("Temp: ");
  lcd.print(temp_val);   // Printing terature on LCD
  lcd.print(" C");
  lcd.setCursor(0,1);
   if(temp_val <20 )
    { 
      analogWrite(9,0);
      lcd.print("Fan OFF            ");
      delay(100);
    }
    
    else if(temp_val==26)
    {             
      analogWrite(pwm, 51);
      lcd.print("Fan Speed: 20%   ");
      delay(100);
    }
    
     else if(temp_val==20)
    {
      analogWrite(pwm, 102);
      lcd.print("Fan Speed: 40%   ");
      delay(100);
    }
    
     else if(temp_val==28)
    {
      analogWrite(pwm, 153);
      lcd.print("Fan Speed: 60%   ");
      delay(100);
    }
    
    else if(temp_val==29)
    {
      analogWrite(pwm, 204);
      
      lcd.print("Fan Speed: 80%    ");
      delay(100);
    }
     else if(temp_val>29)
    {
      analogWrite(pwm, 255);
      lcd.print("Fan Speed: 100%   ");
      delay(100);
    } 
  delay(3000);
}