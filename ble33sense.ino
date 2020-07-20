#include <ArduinoBLE.h>

#define SERVICE_NAME "Arduino BLE 33 Sense"
#define SERVICE_UUID "19B10000-E8F2-537E-4F6C-D104768A1214"


void setup() {
  Serial.begin(38400);
  while (!Serial);

  // begin initialization
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");
    while (1);
  }

  // set the local name peripheral advertises
  BLE.setLocalName(SERVICE_NAME);
  // set the UUID for the service this peripheral advertises
  BLE.setAdvertisedServiceUuid(SERVICE_UUID);
  BLE.setAppearance(0x0080);  // Set as Generic computer

  Serial.println("Building services...");

  setupEnvironmentService();
  setupIMUService();
  setupLEDService();
  setupColourService();
  setupProximityService();

  Serial.println("Advertising services...");
  BLE.advertise();
}

void loop() {
  BLE.poll();

  pollIMU();
  pollColours();
  pollProximity();
}


// Eent calls when connected/disconnected and when characteristics change
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
