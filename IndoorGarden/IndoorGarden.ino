#include <CurieBLE.h>
#include "CurieIMU.h"

byte humidity_sensor_pin = A0;
byte humidity_sensor_vcc = 6;
const int motorPin1  = 10; // Pin  7 of L293
const int motorPin2  = 9;  // Pin  2 of L293

BLEService ledService("19B10000-E8F2-537E-4F6C-D104768A1214"); // BLE LED Service
//BLEService humidityService("29e70001-9227-e097-3473-09a5cc69a43a"); // BLE LED Service

// BLE LED Switch Characteristic - custom 128-bit UUID, read and writable by central
BLEUnsignedCharCharacteristic ledCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
BLEUnsignedCharCharacteristic humidCharacteristic("29e70002-9227-e097-3473-09a5cc69a43a", BLERead | BLEWrite);

const int ledPin = 13; // pin to use for the LED
const int waterPin = 12;

void setup() {
  Serial.begin(9600);
  Serial.print("The device is on!");

  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);

  pinMode(humidity_sensor_vcc, OUTPUT);
  pinMode(waterPin, OUTPUT);
  digitalWrite(humidity_sensor_vcc, LOW);
  digitalWrite(waterPin, LOW);

  // set LED pin to output mode
  pinMode(ledPin, OUTPUT);

  CurieIMU.begin();

  // Set the accelerometer range to 250 degrees/second
  CurieIMU.setAccelerometerRange(2);

  // begin initialization
  BLE.begin();

  // set advertised local name and service UUID:
  BLE.setLocalName("Farm");
  //BLE.setAdvertisedService(humidityService);
  BLE.setAdvertisedService(ledService);


  // add the characteristic to the service
  //humidityService.addCharacteristic(humidCharacteristic);
  ledService.addCharacteristic(humidCharacteristic);
  ledService.addCharacteristic(ledCharacteristic);

  // add service
  //BLE.addService(humidityService);
  BLE.addService(ledService);


  // set the initial value for the characeristic:
  ledCharacteristic.setValue(0);
  humidCharacteristic.setValue(0);

  // start advertising
  BLE.advertise();

  Serial.println("BLE LED-Humid Peripheral");
}

int read_humidity_sensor() {
  digitalWrite(humidity_sensor_vcc, HIGH);
  delay(500);
  int value = analogRead(humidity_sensor_pin);
  digitalWrite(humidity_sensor_vcc, LOW);
  int val = map(value, 0, 1023, 0, 255);
  val = 255 - val;
  return val;
}

void printHumidityValue() {
  Serial.print("Humidity Level (0-1023): ");
  Serial.println(read_humidity_sensor());
}

void loop() {
  float ax, ay, az;  
  // listen for BLE peripherals to connect:
  CurieIMU.readAccelerometerScaled(ax, ay, az);
  Serial.print("a:\t");
  Serial.print(ax);
  Serial.print("\t");
  Serial.print(ay);
  Serial.print("\t");
  Serial.print(az);
  Serial.println();
  BLEDevice central = BLE.central();

  // if a central is connected to peripheral:
  if (central) {
    Serial.print("Connected to central: ");
    // print the central's MAC address:
    Serial.println(central.address());

    // while the central is still connected to peripheral:
    while (central.connected()) {

      if (humidCharacteristic.written()) {
        if (humidCharacteristic.value() == 1) {   // any value with 0
          Serial.println("Water it");
          digitalWrite(motorPin1, HIGH);
          digitalWrite(motorPin2, LOW);
          delay(1000);
        } else if (humidCharacteristic.value() == 2) {   // any value with 0
          Serial.println("Water it");
          digitalWrite(motorPin1, HIGH);
          digitalWrite(motorPin2, LOW);
          delay(2000);
        }  else if (humidCharacteristic.value() == 3) {   // any value with 0
          Serial.println("Water it");
          digitalWrite(motorPin1, HIGH);
          digitalWrite(motorPin2, LOW);
          delay(3000);
        } 
        digitalWrite(motorPin1, LOW);
        digitalWrite(motorPin2, LOW);
      }

      if (ledCharacteristic.written()) {
        if (ledCharacteristic.value()) {   // any value other than 0
          Serial.println("LED on");
          digitalWrite(ledPin, HIGH);         // will turn the LED on
        } else {                              // a 0 value
          Serial.println(F("LED off"));
          digitalWrite(ledPin, LOW);          // will turn the LED off
        }
      }
    }

    // when the central disconnects, print it out:
    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());
  }
}
