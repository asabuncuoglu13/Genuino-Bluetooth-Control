# Genuino-Bluetooth-Control

This repo is created to experiment on Bluetooth Communication between Arduino/Genuino 101 and Android Devices. 

### First Experiment : LED CONTROL

I used Intel Curie examples for Arduino library. (Examples>CurieBLE>Peripheral>LED)

It is a simple example that toggles LED with setting characteristic value to 0 and any value other than 0.

I used Android BluetoothLEGatt example (you can import it from the Android examples) and added writeCharacteristic() method to communicate with Arduino.
