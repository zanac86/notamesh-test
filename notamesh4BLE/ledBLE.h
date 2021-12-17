//
#if BLE_ON > 0

  void SendBLEStr(char * s);
  ExtendedFlags BLEExtFlag;                                   //Флаги расширенных настроек

  #define  PROTOCOL_ON 1                  //Протокол включен

  #include <BLEDevice.h>                                            // Подключаем BLE
  #include <BLEServer.h>
  #include <BLEUtils.h>
  #include <BLE2902.h>
                          //Установка UUID
  #define SERVICE_UUID            "C6FBDD3C-7123-4C9E-86AB-005F1A7EDA01"
  #define CHARACTERISTIC_UUID_RX  "B88E098B-E464-4B54-B827-79EB2B150A9F"
  #define CHARACTERISTIC_UUID_TX  "D769FACF-A4DA-47BA-9253-65359EE480FB"

  BLECharacteristic *pBLE_Characteristic;
  bool BLEdeviceConnected = false;

  void Parser(const char * s);
  
  // функция вызова сервера BLE
  class MyBLEServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer)      {BLEdeviceConnected = true;}
    void onDisconnect(BLEServer* pServer)   {BLEdeviceConnected = false;}
  };
  // функция вызова для получении данных от Android-приложения
  class MyBLECallbacks: public BLECharacteristicCallbacks 
  { void onWrite(BLECharacteristic *pCharacteristic);         //преопределим функцию приема
  };

      //Функция настройки BLE
  void setupBLE() 
  {   BLEDevice::init( BLE_DEVICE_NAME );                     // создаем BLE-устройство:
      BLEServer *pServer = BLEDevice::createServer();         // создаем BLE-сервер:
      pServer->setCallbacks(new MyBLEServerCallbacks());      // создаем функцию вызова BLE-сервера и подключаем ее  
      BLEService *pService = pServer->createService(          // Создаем BLE-сервис и присваиваем ему UUID   
                          SERVICE_UUID);  
      pBLE_Characteristic = pService->createCharacteristic(   // Создаем BLE-характеристику, присваиваем ей UUID
                          CHARACTERISTIC_UUID_TX,
                          BLECharacteristic::PROPERTY_NOTIFY);//        и установить ее как уведомление
      pBLE_Characteristic->addDescriptor(new BLE2902());      // Добавить дискриптор
    
      BLECharacteristic *pCharacteristic;
      pCharacteristic = pService->createCharacteristic(       // Создаем BLE-характеристику, присваиваем ей UUID
                          CHARACTERISTIC_UUID_RX,
                          BLECharacteristic::PROPERTY_WRITE); //        и установить ее как прием
      pCharacteristic->setCallbacks(new MyBLECallbacks());    // создаем функцию вызова BLE-характеристики и подключаем ее
    
      pService->start();                                      // Запускаем сервис:
      pServer->getAdvertising()->start();                     // Начинаем рассылку оповещений
      BLEExtFlag=ExtFlag;                                     //Сохраним
      #if LOG_ON == 1
        Serial.println(F("BLE: Waiting to connect..."));      //  "Ждем подключения..."
      #endif
  }

        //Передать данные о режиме
  void SendBLEStr(char * s)
  {   if(BLEdeviceConnected) 
      { pBLE_Characteristic->setValue(s);                     // прикрепим для передачи
        pBLE_Characteristic->notify();                        // отправляем значение Android-приложению:
        #if LOG_ON == 1
          Serial.print(F("BLE: Sent value: "));               // "Отправленное значение: "
          Serial.println(s); 
        #endif
      }
  }

        //Функция приема данных с Android-приложения
  void MyBLECallbacks::onWrite(BLECharacteristic *pCharacteristic) 
  { std::string rxS = pCharacteristic->getValue();          //Прочитаем что пришло   
    if(rxS.length() > 0) {                                  //Пришло 
      #if LOG_ON == 1
        Serial.print(F("BLE: Received value: "));           // "Отправленное значение: "
        Serial.println(rxS.c_str()); 
      #endif
    }
    Parser(rxS.c_str());   
  }
#endif
