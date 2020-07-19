#include <Arduino_LSM9DS1.h>

// Standard Location and Navigation services
BLEService magneticService("1819");
BLEService gyroService("1819");
BLEService accelService("1819");

// Magnetic Declination characteristic
BLEFloatCharacteristic magneticCharacteristicX("2A2C", BLERead | BLENotify);
BLEFloatCharacteristic magneticCharacteristicY("2A2C", BLERead | BLENotify);
BLEFloatCharacteristic magneticCharacteristicZ("2A2C", BLERead | BLENotify);

// Position 3D characteristic gyro
BLEFloatCharacteristic gyroCharacteristicX("2A30", BLERead | BLENotify);
BLEFloatCharacteristic gyroCharacteristicY("2A30", BLERead | BLENotify);
BLEFloatCharacteristic gyroCharacteristicZ("2A30", BLERead | BLENotify);

// Location and Speed Characteristic characteristic
BLEFloatCharacteristic accelerometerCharacteristicX("2A67", BLERead | BLENotify);
BLEFloatCharacteristic accelerometerCharacteristicY("2A67", BLERead | BLENotify);
BLEFloatCharacteristic accelerometerCharacteristicZ("2A67", BLERead | BLENotify);


void pollIMU() {
  if (magneticCharacteristicX.subscribed()) {
    _readMagnetic(BLE.central(), magneticCharacteristicX);
  }
  if (magneticCharacteristicY.subscribed()) {
    _readMagnetic(BLE.central(), magneticCharacteristicY);
  }
  if (magneticCharacteristicZ.subscribed()) {
    _readMagnetic(BLE.central(), magneticCharacteristicZ);
  }

  if (gyroCharacteristicX.subscribed()) {
    _readGyro(BLE.central(), gyroCharacteristicX);
  }
  if (gyroCharacteristicY.subscribed()) {
    _readGyro(BLE.central(), gyroCharacteristicY);
  }
  if (gyroCharacteristicZ.subscribed()) {
    _readGyro(BLE.central(), gyroCharacteristicZ);
  }


  if (accelerometerCharacteristicX.subscribed()) {
    _readAccelerometer(BLE.central(), accelerometerCharacteristicX);
  }
  if (accelerometerCharacteristicY.subscribed()) {
    _readAccelerometer(BLE.central(), accelerometerCharacteristicY);
  }
  if (accelerometerCharacteristicZ.subscribed()) {
    _readAccelerometer(BLE.central(), accelerometerCharacteristicZ);
  }
}

void buildIMUService() {
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

  // Adds three services, magnetic, gyro and accelerometer, each having three Characteristic, x, y and z.

  BLEDescriptor magDescX("2901", "magnetic X");
  magneticCharacteristicX.addDescriptor(magDescX);
  magneticCharacteristicX.setEventHandler(BLERead, _readMagnetic);

  BLEDescriptor magDescY("2901", "magnetic Y");
  magneticCharacteristicY.addDescriptor(magDescY);
  magneticCharacteristicY.setEventHandler(BLERead, _readMagnetic);

  BLEDescriptor magDescZ("2901", "magnetic Z");
  magneticCharacteristicZ.addDescriptor(magDescZ);
  magneticCharacteristicZ.setEventHandler(BLERead, _readMagnetic);

  magneticService.addCharacteristic(magneticCharacteristicX);
  magneticService.addCharacteristic(magneticCharacteristicY);
  magneticService.addCharacteristic(magneticCharacteristicZ);

  BLE.addService(magneticService);


  BLEDescriptor gyroDescX("2901", "gyroscope X");
  gyroCharacteristicX.addDescriptor(gyroDescX);
  gyroCharacteristicX.setEventHandler(BLERead, _readGyro);

  BLEDescriptor gyroDescY("2901", "gyroscope Y");
  gyroCharacteristicY.addDescriptor(gyroDescY);
  gyroCharacteristicY.setEventHandler(BLERead, _readGyro);

  BLEDescriptor gyroDescZ("2901", "gyroscope Z");
  gyroCharacteristicZ.addDescriptor(gyroDescZ);
  gyroCharacteristicZ.setEventHandler(BLERead, _readGyro);

  gyroService.addCharacteristic(gyroCharacteristicX);
  gyroService.addCharacteristic(gyroCharacteristicY);
  gyroService.addCharacteristic(gyroCharacteristicZ);

  BLE.addService(gyroService);


  BLEDescriptor accelerometerDescX("2901", "accelerometer X");
  accelerometerCharacteristicX.addDescriptor(accelerometerDescX);
  accelerometerCharacteristicX.setEventHandler(BLERead, _readAccelerometer);

  BLEDescriptor accelerometerDescY("2901", "accelerometer Y");
  accelerometerCharacteristicY.addDescriptor(accelerometerDescY);
  accelerometerCharacteristicY.setEventHandler(BLERead, _readAccelerometer);

  BLEDescriptor accelerometerDescZ("2901", "accelerometer Z");
  accelerometerCharacteristicZ.addDescriptor(accelerometerDescZ);
  accelerometerCharacteristicZ.setEventHandler(BLERead, _readAccelerometer);

  accelService.addCharacteristic(accelerometerCharacteristicX);
  accelService.addCharacteristic(accelerometerCharacteristicY);
  accelService.addCharacteristic(accelerometerCharacteristicZ);

  BLE.addService(accelService);

}

void _readMagnetic(BLEDevice central, BLECharacteristic characteristic) {
  float x, y, z;
  if (IMU.magneticFieldAvailable()) {
    IMU.readMagneticField(x, y, z);
    if (characteristic == magneticCharacteristicX)
      magneticCharacteristicX.writeValue(x);

    else if (characteristic == magneticCharacteristicY)
      magneticCharacteristicY.writeValue(y);

    else if (characteristic == magneticCharacteristicZ)
      magneticCharacteristicZ.writeValue(z);
  }
}

void _readGyro(BLEDevice central, BLECharacteristic characteristic) {
  float x, y, z;
  if (IMU.gyroscopeAvailable()) {
    IMU.readGyroscope(x, y, z);
    if (characteristic == gyroCharacteristicX)
      gyroCharacteristicX.writeValue(x);

    else if (characteristic == gyroCharacteristicY)
      gyroCharacteristicY.writeValue(y);

    else if (characteristic == gyroCharacteristicZ)
      gyroCharacteristicZ.writeValue(z);
  }
}

void _readAccelerometer(BLEDevice central, BLECharacteristic characteristic) {
  float x, y, z;
  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(x, y, z);
    if (characteristic == accelerometerCharacteristicX) {
      accelerometerCharacteristicX.writeValue(x);
      
    } else if (characteristic == accelerometerCharacteristicY) {
      accelerometerCharacteristicY.writeValue(y);

    } else if (characteristic == accelerometerCharacteristicZ) {
      accelerometerCharacteristicY.writeValue(z);
    }
  }

}
