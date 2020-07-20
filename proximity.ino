#include <Arduino_APDS9960.h>

BLEService proximityService("1819");  // Location and Navigation

BLEIntCharacteristic proximityCharacteristic("2A67", BLERead | BLENotify | BLEBroadcast);  // Location and Speed Characteristic
BLEByteCharacteristic proximityThresholdCharacteristic("2AD6", BLERead | BLEWrite);  // Alert Notification Control Point


int proximityThreshold = 80;
int lastProximity = 0;

void pollProximity() {

  if (!APDS.proximityAvailable())
    return;

  int proximity = APDS.readProximity();
  if (proximity < proximityThreshold || proximityCharacteristic.subscribed()) {
    if (lastProximity != proximity) {
      lastProximity = proximity;
      proximityCharacteristic.writeValue(proximity);
    }
  }
}


void setupProximityService() {
  if (!APDS.begin()) {  // Initialize APDS sensor
    Serial.println("Failed to initializing APDS9960 (proximity) sensor!!");
    while (1);
  }

  BLEDescriptor proxDesc("2901", "proximity");
  proximityCharacteristic.addDescriptor(proxDesc);
  proximityCharacteristic.setEventHandler(BLERead, _readProximity);
  proximityCharacteristic.broadcast();
  proximityService.addCharacteristic(proximityCharacteristic);

  BLEDescriptor proxThresDesc("2901", "proximity alert level");
  proximityThresholdCharacteristic.addDescriptor(proxThresDesc);
  proximityThresholdCharacteristic.setEventHandler(BLEWrite, _updateProximityThreshold);
  proximityThresholdCharacteristic.writeValue(proximityThreshold);
  proximityService.addCharacteristic(proximityThresholdCharacteristic);

  BLE.addService(proximityService);

}
void _updateProximityThreshold(BLEDevice central, BLECharacteristic characteristic) {
  proximityThreshold = proximityThresholdCharacteristic.value();
}

void _readProximity(BLEDevice central, BLECharacteristic characteristic) {
  if (!APDS.proximityAvailable()) {
    Serial.println("proximity unavailable");
    return;
  }

  int v = APDS.readProximity();
  if (v != lastProximity) {
    Serial.print("proximity reading (lower = closer): ");
    Serial.println(v);
    lastProximity = v;
    proximityCharacteristic.writeValue(v);
  }
}
