/*********
  Руи Сантос
  Более подробно о проекте на: http://randomnerdtutorials.com  
*********/

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#define temperatureCelsius
// даем название BLE-серверу:
#define bleServerName "dhtESP32"

// для генерирования UUID можно воспользоваться этим сайтом:
// https://www.uuidgenerator.net/
#define SERVICE_UUID "91bad492-b950-4226-aa2b-4ede9fa42f59"

#ifdef temperatureCelsius
  BLECharacteristic dhtTemperatureCelsiusCharacteristics("cba1d466-344c-4be3-ab3f-189f80dd7518", BLECharacteristic::PROPERTY_NOTIFY);
  BLEDescriptor dhtTemperatureCelsiusDescriptor(BLEUUID((uint16_t)0x2902));
#else
  BLECharacteristic dhtTemperatureFahrenheitCharacteristics("f78ebbff-c8b7-4107-93de-889a6a06d408", BLECharacteristic::PROPERTY_NOTIFY);
  BLEDescriptor dhtTemperatureFahrenheitDescriptor(BLEUUID((uint16_t)0x2901));
#endif

BLECharacteristic dhtHumidityCharacteristics("ca73b3ba-39f6-4ab3-91ae-186dc9577d99", BLECharacteristic::PROPERTY_NOTIFY);
BLEDescriptor dhtHumidityDescriptor(BLEUUID((uint16_t)0x2903));

// контакт, к которому подключен датчик DHT:
const int DHTPin = 14;

bool deviceConnected = false;

// задаем функции обратного вызова onConnect() и onDisconnect():
class MyServerCallbacks: public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    deviceConnected = true;
  };
  void onDisconnect(BLEServer* pServer) {
    deviceConnected = false;
  }
};

void setup() {
  // запускаем последовательную коммуникацию:
  Serial.begin(115200);

  // создаем BLE-устройство:
  BLEDevice::init(bleServerName);

  // создаем BLE-сервер:
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // создаем BLE-сервис:
  BLEService *dhtService = pServer->createService(SERVICE_UUID);

  // создаем BLE-характеристики и BLE-дескриптор: bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.descriptor.gatt.client_characteristic_configuration.xml       


  dhtService->addCharacteristic(&dhtTemperatureCelsiusCharacteristics);
  dhtTemperatureCelsiusDescriptor.setValue("DHT temperature Celsius");
                                       //  "Температура в Цельсиях"
  dhtTemperatureCelsiusCharacteristics.addDescriptor(new BLE2902());

  dhtService->addCharacteristic(&dhtHumidityCharacteristics);
  dhtHumidityDescriptor.setValue("DHT humidity"); 
                             //  "Влажность"
  dhtHumidityCharacteristics.addDescriptor(new BLE2902());
  
  // запускаем сервис:
  dhtService->start();

  // запускаем рассылку оповещений:
  pServer->getAdvertising()->start();
  Serial.println("Waiting a client connection to notify...");
             //  "Ждем подключения клиента, чтобы отправить уведомление..."
}

void loop() {
  if (deviceConnected) {
    // считываем температуру в градусах Цельсия (по умолчанию):
    float t = 25.0;
    // считываем температуру в градусах Фаренгейта
    // (isFahrenheit = true):
    float f = 80.0;
    // считываем влажность:
    float h = 99.99;
 
    // проверяем, удалось ли прочесть данные,
    // и если нет, то выходим из loop(), чтобы попробовать снова:
    if (isnan(h) || isnan(t) || isnan(f)) {
      Serial.println("Failed to read from DHT sensor!");
                 //  "Не удалось прочесть данные с датчика DHT!"
      return;
    }
    // отправляем уведомление о том,
    // что с датчика DHT считаны данные о температуре:

    static char temperatureCTemp[7];
    dtostrf(t, 6, 2, temperatureCTemp);
    // задаем значение для температурной характеристики (Цельсий)
    // и отправляем уведомление подключенному клиенту:
    dhtTemperatureCelsiusCharacteristics.setValue(temperatureCTemp);
    dhtTemperatureCelsiusCharacteristics.notify();
    Serial.print("Temperature Celsius: ");
             //  "Температура в градусах Цельсия: "
    Serial.print(t);
    Serial.print(" *C");

    
    // отправляем уведомление о том,
    // что с датчика DHT считаны данные о влажности:
    static char humidityTemp[7];
    dtostrf(h, 6, 2, humidityTemp);
    // задаем значение для влажностной характеристики
    // и отправляем уведомление подключенному клиенту:
    dhtHumidityCharacteristics.setValue(humidityTemp);
    dhtHumidityCharacteristics.notify();   
    Serial.print(" - Humidity: ");
             //  " - Влажность: "
    Serial.print(h);
    Serial.println(" %");
    
    delay(2000);
  }
}
