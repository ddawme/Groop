#include "DHT.h"

//temperature stuff
#define dhtpin 12
#define dhttype DHT22
DHT dht(dhtpin, dhttype);



//variables for future serial communication
int rawValue = 0;
int processedValue = 0;
int previousValue = 0;
int dataPackage[] = {rawValue, processedValue};
unsigned long changeCounts = 1;


//Variables for measurements
const int sensorPower = 3;      // Turns sensor on and off
const int sensorPin = A0;       // Records sensor value
int outputValue;                // Holds rawValue before we convert it to a 0-100 value

//For easy calibration
int wetSoil = 250;              // Variable to hold wet soil value
int drySoil = 1023;             // Variable to hold dry soil value

const int button1 = 6;          // Left button; Dry Soil
const int button2 = 5;          // Right button; Wet Soil
const int calibrate = 13;       // Built in LED to show you are in calibration mode
bool cal_loop = false;          // Allows me to turn on and off the calibration loops


//time management variables
unsigned long previousMillis = 0;
const int checkInterval = 5000;


void setup() {
  
  //Setup power pin for the sensor
  pinMode(sensorPower, OUTPUT);
  digitalWrite(sensorPower, LOW);

  //Calibration Buttons
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);
  pinMode(calibrate, OUTPUT);
  digitalWrite(calibrate, LOW);
  

  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(2000);

  dht.begin();

  measureSoil();
  convertData();
  
}

void loop() {

calibrateMode();

unsigned long currentMillis = millis();

    if (currentMillis - previousMillis > checkInterval) {

      previousMillis = currentMillis;
      measureSoil();
      convertData();

    }
    
   if (previousValue != rawValue) {
      
      response();      
      temperatureFunction();
      previousValue = rawValue;
      changeCounts++;
      
   }
}

void measureSoil() {
  
  /*  
   *  This function turns the sensor on
   *  delays for a set value of time,
   *  takes a measurement
   *  and turns the sensor off
   */ 

  digitalWrite(sensorPower, HIGH);         //Turns on sensor
  delay(500);                              //Delay for 0.5 seconds
  outputValue = analogRead(sensorPin);     //Read sensor and assigns raw data to output_value
  delay(500);
  digitalWrite(sensorPower, LOW);          //Turns off sensor power
  
}

void convertData() {

/*
 *  The purpose of this function 
 *  is to convert the raw
 *  data to a 0-100 scale  
 */
 
  rawValue = outputValue;                              //Saves the raw data before converting it
  rawValue = map(rawValue, drySoil, wetSoil, 0, 100);  //Converts our raw data to 0-100 values
  rawValue = constrain(rawValue, 0, 100);              //Keeps reading between 0-100
  processedValue = rawValue;                           //Saves processed data 
  
}

void response() {

/*
 * This function will change 
 * throughout life of the 
 * project..
 */
 
  Serial.print("I am reading a moisture value of: ");
  Serial.print(processedValue);
  Serial.println("%");
  Serial.print("Change counts: ");
  Serial.println(changeCounts);
  
}

void calibrateMode() {

  if (digitalRead(button1) == HIGH && digitalRead(button2) == HIGH) {
  
    Serial.println("Calibrating...");         
    
    cal_loop = true;                          // Initializes while loops
    digitalWrite(calibrate, HIGH);            // Turns on LED to show user they are calibrating the unit
    Serial.println("Insert in dry soil");
    digitalWrite(sensorPower, HIGH);          // Turns on the sensor
    
    while (cal_loop) {

      drySoil = analogRead(sensorPin);
      delay(2000);                              // To allow user time to take fingers off buttons
      
      if (digitalRead(button1) == HIGH) {

        delay(1000);
        break;
        
      }            
    }

    Serial.println("Add water to desired moisture saturation level");

    while (cal_loop) {

      wetSoil = analogRead(sensorPin);

      if (digitalRead(button2) == HIGH) {

        delay(1000);      
        break;
        
      }      
    }

  cal_loop = false;
  digitalWrite(calibrate, LOW);
  digitalWrite(sensorPower, LOW);

  Serial.print("The calibrated Dry Soil value is:");
  Serial.println(drySoil);
  Serial.print("The calibrated Wet Soil value is:");
  Serial.println(wetSoil);
  
  }


}

void temperatureFunction() {
  
  // Wait a few seconds between measurements.
  delay(2000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(f);
  Serial.print(" *F\t");
  Serial.print("Heat index: ");
  Serial.print(hif);
  Serial.println(" *F");
  
}
