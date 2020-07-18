
#include <ArduinoBLE.h>

BLEService ledService("19B10000-E8F2-537E-4F6C-D104768A1214"); // create service
BLEService led2Service("19B10100-E8F2-537E-4F6C-D104768A1214"); // create service

// create switch characteristic and allow remote device to read and write
BLEByteCharacteristic switch1Characteristic("2B2B", BLERead | BLEWrite); // "19B10001-E8F2-537E-4F6C-D104768A1214"
BLEByteCharacteristic switch2Characteristic("2B2B", BLERead | BLEWrite); //"19B10101-E8F2-537E-4F6C-D104768A1214"
BLEByteCharacteristic switch3Characteristic("2B2B", BLERead | BLEWrite); // "19B10201-E8F2-537E-4F6C-D104768A1214"

BLEDescriptor switch1Descriptor("2901", "main lights");
BLEDescriptor switch2Descriptor("2901", "read light 1");
BLEDescriptor switch3Descriptor("2901", "read light 2");

const int ledPin = LED_BUILTIN; // pin to use for the LED

void setup() {
  Serial.begin(38400);
  while (!Serial);
  
  pinMode(ledPin, OUTPUT); // use the LED pin as an output

  // begin initialization
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");

    while (1);
  }

  // set the local name peripheral advertises
  BLE.setLocalName("LEDCallback");
  // set the UUID for the service this peripheral advertises
  BLE.setAdvertisedService(ledService);

  switch1Characteristic.addDescriptor(switch1Descriptor);
  switch2Characteristic.addDescriptor(switch2Descriptor);
  switch3Characteristic.addDescriptor(switch3Descriptor);

  // add the characteristic to the service
  ledService.addCharacteristic(switch1Characteristic);
  led2Service.addCharacteristic(switch2Characteristic);
  led2Service.addCharacteristic(switch3Characteristic);

  // add service
  BLE.addService(ledService);
  BLE.addService(led2Service);

  
  // assign event handlers for connected, disconnected to peripheral
  BLE.setEventHandler(BLEConnected, blePeripheralConnectHandler);
  BLE.setEventHandler(BLEDisconnected, blePeripheralDisconnectHandler);

  // assign event handlers for characteristic
  switch1Characteristic.setEventHandler(BLEWritten, switch1CharacteristicWritten);
  
  switch2Characteristic.setEventHandler(BLEWritten, switch1CharacteristicWritten);
  // set an initial value for the characteristic
  switch1Characteristic.setValue(0);
  switch2Characteristic.setValue(0);

  // start advertising
  BLE.advertise();

  Serial.println(("Bluetooth device active, waiting for connections..."));
}

void loop() {
  // poll for BLE events
  BLE.poll();
}

void blePeripheralConnectHandler(BLEDevice central) {
  // central connected event handler
  Serial.print("Connected event, central: ");
  Serial.println(central.address());
}

void blePeripheralDisconnectHandler(BLEDevice central) {
  // central disconnected event handler
  Serial.print("Disconnected event, central: ");
  Serial.println(central.address());
}

void switch1CharacteristicWritten(BLEDevice central, BLECharacteristic characteristic) {
  // central wrote new value to characteristic, update LED
  Serial.print("Characteristic event, written: (");
  Serial.print((char*)characteristic.value());
  Serial.print(") ");

  if (characteristic.value() && *characteristic.value() > 0) {
    Serial.println("LED on");
    digitalWrite(ledPin, HIGH);
  } else {
    Serial.println("LED off");
    digitalWrite(ledPin, LOW);
  }
}
