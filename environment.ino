#include <Arduino_HTS221.h>
#include <Arduino_LPS22HB.h>

// Standard Environmental Sensing service
BLEService environmentService("181A");

// Standard 16-bit Temperature characteristic
BLEIntCharacteristic tempCharacteristic("2A6E", BLERead | BLENotify); // Remote clients can read and get updates

// Unsigned 16-bit Humidity characteristic
BLEUnsignedIntCharacteristic humidCharacteristic("2A6F", BLERead | BLENotify);

// barometer characteristic
BLEFloatCharacteristic barometerCharacteristic("2A6D", BLERead | BLENotify);


void buildEnvironmentService() {
  if (!HTS.begin()) {  // Initialize HTS22 sensor
    Serial.println("Failed to initialize humidity temperature sensor!");
    while (1);
  }
  
  if (!BARO.begin()) {  // Initialize LPS22HB sensor
    Serial.println("Failed to initialize barometer sensor!");
    while (1);
  }

  environmentService.addCharacteristic(tempCharacteristic);
  environmentService.addCharacteristic(humidCharacteristic);
  environmentService.addCharacteristic(barometerCharacteristic);

  tempCharacteristic.setEventHandler(BLERead, readTemprature);
  humidCharacteristic.setEventHandler(BLERead, _readHumidity);
  barometerCharacteristic.setEventHandler(BLERead, _readBarometer);
  BLE.addService(environmentService);
}

void readTemprature(BLEDevice central, BLECharacteristic characteristic) {
  // Get calibrated temperature as signed 16-bit int for BLE characteristic
  uint16_t value = (uint16_t) (HTS.readTemperature() * 100);
  characteristic.writeValue(value);
}

void _readHumidity(BLEDevice central, BLECharacteristic characteristic) {
  // Get humidity as unsigned 16-bit int for BLE characteristic
  uint16_t value = (uint16_t) (HTS.readHumidity() * 100);
  characteristic.writeValue(value);
}

void _readBarometer(BLEDevice central, BLECharacteristic characteristic) {
  // Get Barometer as float for BLE characteristic
  barometerCharacteristic.writeValue(BARO.readPressure(MILLIBAR));
}
