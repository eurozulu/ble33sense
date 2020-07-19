#include <Arduino_APDS9960.h>

BLEService colourService("181A");  // Environmental Sensing

// Unsigned 16-bit characteristic 00RRGGBB
BLEIntCharacteristic colourRedCharacteristic("2A59", BLERead | BLENotify);
BLEIntCharacteristic colourGreenCharacteristic("2A59", BLERead | BLENotify);
BLEIntCharacteristic colourBlueCharacteristic("2A59", BLERead | BLENotify);
BLEIntCharacteristic colourClearCharacteristic("2A59", BLERead | BLENotify);


void pollColours() {
  if (colourRedCharacteristic.subscribed()) {
    _readColours(BLE.central(), colourRedCharacteristic);
  }
  if (colourGreenCharacteristic.subscribed()) {
    _readColours(BLE.central(), colourGreenCharacteristic);
  }
  if (colourBlueCharacteristic.subscribed()) {
    _readColours(BLE.central(), colourBlueCharacteristic);
  }
  if (colourClearCharacteristic.subscribed()) {
    _readColours(BLE.central(), colourClearCharacteristic);
  }
}


void setupColourService() {
  if (!APDS.begin()) {  // Initialize APDS sensor
    Serial.println("Failed to initializing APDS9960 (light) sensor!!");
    while (1);
  }

  BLEDescriptor colDescRed("2901", "Red light level");
  colourRedCharacteristic.addDescriptor(colDescRed);
  colourRedCharacteristic.setEventHandler(BLERead, _readColours);
  colourService.addCharacteristic(colourRedCharacteristic);

  BLEDescriptor colDescGreen("2901", "Green light level");
  colourGreenCharacteristic.addDescriptor(colDescGreen);
  colourGreenCharacteristic.setEventHandler(BLERead, _readColours);
  colourService.addCharacteristic(colourGreenCharacteristic);

  BLEDescriptor colDescBlue("2901", "Blue light level");
  colourBlueCharacteristic.addDescriptor(colDescBlue);
  colourBlueCharacteristic.setEventHandler(BLERead, _readColours);
  colourService.addCharacteristic(colourBlueCharacteristic);

  BLEDescriptor colDescClear("2901", "Clear light level");
  colourClearCharacteristic.addDescriptor(colDescClear);
  colourClearCharacteristic.setEventHandler(BLERead, _readColours);
  colourService.addCharacteristic(colourClearCharacteristic);

  BLE.addService(colourService);

}

void _readColours(BLEDevice central, BLECharacteristic characteristic) {
  if (!APDS.colorAvailable())
    return;

  int r, g, b, c;
  APDS.readColor(r, g, b, c);
  Serial.print("Red: ");
  Serial.print(r);
  Serial.print("  Green: ");
  Serial.print(g);
  Serial.print("  Blue: ");
  Serial.print(b);
  Serial.print("  Clear: ");
  Serial.print(c);
  Serial.println();

  colourRedCharacteristic.writeValue(r);
  colourGreenCharacteristic.writeValue(g);
  colourBlueCharacteristic.writeValue(b);
  colourClearCharacteristic.writeValue(c);

}
