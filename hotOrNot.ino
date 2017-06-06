/*

 Parts required:
 1 TMP36 temperature sensor
 3 red LEDs
 3 220 ohm resistors
 1 potentiometer
 1 servo moter
 1 button
 2 100uf capacitors
 
#include <Servo.h>
Servo myServo;

const int sensorPin = A0;
const int potPin = A1;
const int ledPin = 4;
const int buttonPin = 7;
int buttonState = LOW;
float baselineTemp = 0;
// room temperature in Celcius
int potVal = 0;
float angle;
float sensorValue = 0;         // the sensor value
float sensorMin = 1023;        // minimum sensor value
float sensorMax = 0;           // maximum sensor value
float sensorAvg;
float baselineVoltage;
float upperBound = 0;


void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  digitalWrite(ledPin, HIGH);
  while (millis() < 5000) {
    sensorValue = analogRead(sensorPin);

    // record the maximum sensor value
    if (sensorValue > sensorMax) {
      sensorMax = sensorValue;
    }

    // record the minimum sensor value
    if (sensorValue < sensorMin) {
      sensorMin = sensorValue;
    }
  }
  // open a serial connection to display values
  myServo.attach(3);
  Serial.begin(9600);
  
  Serial.println(sensorMin);
  Serial.println(sensorMax);
  sensorAvg = (sensorMax + sensorMin)/2;
  Serial.println(sensorAvg);
  
  float baselineVoltage = (sensorMin / 1024.0) * 5.0;
  Serial.println(baselineVoltage);
  baselineTemp = (baselineVoltage - .5) * 100;
  Serial.println(baselineTemp);
  digitalWrite(ledPin, LOW);
  
}


void calibrate() {
  digitalWrite(ledPin, HIGH);
  while (millis() < 5000) {
    sensorValue = analogRead(sensorPin);
    
    // record the maximum sensor value
    if (sensorValue > sensorMax) {
      sensorMax = sensorValue;
      }
    
    // record the minimum sensor value
    if (sensorValue < sensorMin) {
      sensorMin = sensorValue;
      }
    }
    delay(5000);
    Serial.println(sensorMin);
    Serial.println(sensorMax);
    sensorAvg = (sensorMax + sensorMin)/2;
    Serial.println(sensorAvg);
      
  //  float baselineVoltage = (sensorAvg / 1024.0) * 5.0;
    float baselineVoltage = (sensorMin/1024)*5.0;
    Serial.println(baselineVoltage);
    baselineTemp = (baselineVoltage - .5) * 100 - 1.5;
    Serial.println(baselineTemp);
    digitalWrite(ledPin, LOW);
}



void loop() {
  buttonState = digitalRead(buttonPin);
  Serial.println(buttonState);
  
  if(buttonState == HIGH){
      calibrate();  
    }

  Serial.print("potVal,upperBound:  ");
  Serial.print(upperBound);
  
  Serial.print(" baselineTemp: ");
  Serial.print(baselineTemp);
 
  potVal = analogRead(potPin);
  int sensorVal = analogRead(sensorPin);

  Serial.print(", sensor Value: ");
  Serial.print(sensorVal);

  float voltage = (sensorVal / 1024.0) * 5.0;

  
  Serial.print(", Volts: ");
  Serial.print(voltage);

  // ((volatge - 500mV) times 100)
  Serial.print(", degrees C: ");
  float temperature = (voltage - .5) * 100;
  Serial.print(temperature);

  // if the current temperature is lower than the baseline
  // turn off all LEDs
  upperBound = map(potVal, 0, 1023, 0.0 ,8.0);
  if(upperBound == 0){
    digitalWrite(ledPin, HIGH);
    delay(1000);
    digitalWrite(ledPin, LOW);
  }
  angle = map(temperature,  baselineTemp, baselineTemp + upperBound, 0, 179);
  Serial.print(", angle: ");
  Serial.println(angle);
  myServo.write(angle);
  delay(1000);
}
