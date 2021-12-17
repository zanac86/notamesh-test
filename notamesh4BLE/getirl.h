
//----------------- IR Receiver and Button Command Processing ---------------------------------------------
#if ( KEY_GYVER_ON >0 )

void getirl() {                                                   // This is the IR function that gets the value and selects/performs command.

  if (Protocol) {

    if ((ledMode < 220) || (ledMode >= 230) || (demorun < 100)) {
      LED2_Off;                                                      //Выключить светодиод

      #if LOG_ON == 1
        Serial.print(F("Command: 0x"));Serial.println(Command,HEX);
      #endif

      switch(Command) {
        
        #if Command_Brightness_plus 
          case Command_Brightness_plus :   //////////////////////////////////////////////////////////////////////////  Увеличить максимальную яркость и остановится если достигли максимума
                                    max_bright = min((max_bright<5)? max_bright+1:max_bright/2*3,255); 
                                    LEDS.setBrightness(max_bright); 
                                    sendBright();
                                    #if LOG_ON == 1
                                        Serial.print(F("Brightness+ ")); Serial.println(max_bright);
                                    #endif
                                    break;         
        #endif

        #if Command_Brightness_plus_R
          case Command_Brightness_plus_R :   //////////////////////////////////////////////////////////////////////////  Увеличить максимальную яркость, при достижения максимума начать с минимума
                                    if (max_bright == 255) max_bright=1;
                                    else max_bright = min((max_bright<5)? max_bright+1:max_bright/2*3,255);
                                    LEDS.setBrightness(max_bright); 
                                    sendBright();
                                    #if LOG_ON == 1
                                        Serial.print(F("Brightness+ ")); Serial.println(max_bright);
                                    #endif
                                    break;         
        #endif

        #if Command_Brightness_minus
          case Command_Brightness_minus :    /////////////////////////////////////////////////////////////////////////  Уменьшить максимальную яркость и остановится если достигли максимума
                                    max_bright = max((max_bright<5)? max_bright-1:max_bright/3*2,1); 
                                    LEDS.setBrightness(max_bright);
                                    sendBright(); 
                                    #if LOG_ON == 1
                                        Serial.print(F("Brightness- ")); Serial.println(max_bright);
                                    #endif
                                    break;         
        #endif

        #if Command_Brightness_minus_R
          case Command_Brightness_minus_R :    /////////////////////////////////////////////////////////////////////////  Уменьшить максимальную яркость, при достижения минимума начать с максимума
                                    if (max_bright == 1) max_bright=255;
                                    else max_bright = max((max_bright<5)? max_bright-1:max_bright/3*2,1); 
                                    LEDS.setBrightness(max_bright); 
                                    sendBright();
                                    #if LOG_ON == 1
                                        Serial.print(F("Brightness- ")); Serial.println(max_bright);
                                    #endif
                                    break;         
        #endif

        #if Command_Reset
          case Command_Reset :              ///////////////////////////////////////////////////////////////////////////  Сброс всех настроек 
                                    ledMode = 0; strobe_mode(ledMode,1); FastLED.show(); bootme(); 
                                    #if LOG_ON == 1
                                        Serial.println(F("Reset"));
                                    #endif
                                    break;         
        #endif

        #if Command_Stop
          case Command_Stop :              ///////////////////////////////////////////////////////////////////////////  Стоп 
                                    SetOff(demorun); 
                                    ledMode = 255; palchg=0;
                                    solid = CRGB::Black;
                                    FastLED.setBrightness(0);
                                    #if LOG_ON == 1
                                      Serial.println(F("Stop"));
                                    #endif
                                    break;         
        #endif

        #if Command_Start
          case Command_Start :              ///////////////////////////////////////////////////////////////////////////  Старт 
                                    SetOn(demorun);  
                                    ledMode = newMode;    
                                    LEDS.setBrightness(max_bright);
                                    #if LOG_ON == 1
                                      Serial.println(F("Start"));
                                    #endif
                                    break;         
        #endif

        #if Command_Start_Stop
          case Command_Start_Stop :         ///////////////////////////////////////////////////////////////////////////  Старт/Стоп 
                                    if (TestOff(demorun) )
                                    {
                                        SetOn(demorun); 
                                        ledMode = newMode;    
                                        LEDS.setBrightness(max_bright);
                                        #if LOG_ON == 1
                                          Serial.println(F("Start"));
                                        #endif
                                    }
                                    else
                                    {
                                        SetOff(demorun);
                                        ledMode = 255; palchg=0;
                                        solid = CRGB::Black;
                                        FastLED.setBrightness(0);
                                        #if LOG_ON == 1
                                          Serial.println(F("Stop"));
                                        #endif
                                     }
                                    break;         
        #endif

        #if Command_Demo_On
          case Command_Demo_On :            /////////////////////////////////////////////////////////////////////////////  Востановим демо режим (какой был до этого) 
                                    SetOn(demorun); meshwait(); 
                                    #if LOG_ON == 1
                                        Serial.println(F("Demo On"));
                                    #endif
                                    sendDemoRun();
                                    break;         
        #endif
  

        #if Command_Demo_Sequence
          case Command_Demo_Sequence :            /////////////////////////////////////////////////////////////////////////////  Включит демо режим (перебор) 
                                    demorun = 1; meshwait(); 
                                    #if LOG_ON == 1
                                        Serial.println(F("Demo Sequence On"));
                                    #endif
                                    break;         
        #endif

        #if Command_Demo_Random
          case Command_Demo_Random :     /////////////////////////////////////////////////////////////////////////////  Включит демо режим (Случайно) 
                                    demorun = 2; meshwait(); 
                                    #if LOG_ON == 1
                                        Serial.println(F("Demo Random On"));
                                    #endif
                                    sendDemoRun();
                                    break;         
        #endif
        
        #if Command_Demo_Sequence_Select
          case Command_Demo_Sequence_Select :         /////////////////////////////////////////////////////////////////////////////  Включит демо режим из выбранных режимов (перебор) 
                                    demorun = 3; meshwait(); 
                                    #if LOG_ON == 1
                                        Serial.println(F("Demo Sequence Select On"));
                                    #endif
                                    sendDemoRun();
                                    break;         
        #endif

        #if Command_Demo_Random_Select
          case Command_Demo_Random_Select :     /////////////////////////////////////////////////////////////////////////////  Включит демо режим из выбранных режимов (Случайно) 
                                    demorun = 4; meshwait(); 
                                    #if LOG_ON == 1
                                        Serial.println(F("Demo Random Select On"));
                                    #endif
                                    sendDemoRun();
                                    break;         
        #endif

        #if Command_Setup_Mode_On
          case Command_Setup_Mode_On : ///////////////////////////////////////////////////////////////////////////  Перейти в режим настройки
                                    if (demorun< 100) demorun += 100; ledMode = 220;
                                    #if LOG_ON == 1
                                      Serial.println(F("Setup Mode On "));
                                    #endif
                                    break;         
        #endif
  
        #if Command_Length_Garland_plus
          case Command_Length_Garland_plus : ///////////////////////////////////////////////////////////////////////////  Увеличить количество светодиодов в гирлянде 
                                    SetOff(demorun); ledMode = 200;
                                    if (NUM_LEDS < (MAX_LEDS-1)) NUM_LEDS++;                          //Новое значение 
                                    #if SAVE_EEPROM >0
                                      #if MAX_LEDS < 255
                                        EEPROM.write(STRANDLEN, NUM_LEDS);                              //Сохранить в память
                                        EEPROM.write(STRANDLEN+1, 0);                                   //Сохранить в память
                                      #else
                                        EEPROM.write(STRANDLEN,   (uint16_t)(NUM_LEDS) & 0x00ff);       //Сохранить в память
                                        EEPROM.write(STRANDLEN+1, (uint16_t)(NUM_LEDS) >> 8);           //Сохранить в память
                                      #endif
                                      #if !defined(__AVR__)
                                         EEPROM.commit();
                                      #endif
                                    #endif
                                    #if LOG_ON == 1
                                      Serial.print(F("Length Garland ")); Serial.println(NUM_LEDS);
                                    #endif
                                    sendSetKolLed();
                                    break;         
        #endif
  
        #if Command_Length_Garland_minus
          case Command_Length_Garland_minus : ///////////////////////////////////////////////////////////////////////////  Уменьшить количество светодиодов в гирлянде
                                    SetOff(demorun); ledMode = 200;
                                    if (NUM_LEDS > 0) NUM_LEDS--;                                     //Новое значение 
                                    #if SAVE_EEPROM >0
                                      #if MAX_LEDS < 255
                                        EEPROM.write(STRANDLEN, NUM_LEDS);                              //Сохранить в память
                                        EEPROM.write(STRANDLEN+1, 0);                                   //Сохранить в память
                                      #else
                                        EEPROM.write(STRANDLEN,   (uint16_t)(NUM_LEDS) & 0x00ff);       //Сохранить в память
                                        EEPROM.write(STRANDLEN+1, (uint16_t)(NUM_LEDS) >> 8);           //Сохранить в память
                                      #endif
                                      #if !defined(__AVR__)
                                         EEPROM.commit();
                                      #endif
                                    #endif
                                    #if LOG_ON == 1
                                      Serial.print(F("Length Garland ")); Serial.println(NUM_LEDS);
                                    #endif
                                    sendSetKolLed();
                                    break;         
        #endif
  
        #if Command_Rotate
          case Command_Rotate :             ///////////////////////////////////////////////////////////////////////////  Сменить направление движения эффектов
                                    if (Protocol == 1) {          //отключить повтор
                                      thisdir = thisdir*-1;
                                      sendRotate();
                                      #if LOG_ON == 1
                                        Serial.println(F("Rotate")); 
                                      #endif
                                    }
                                    break;         
        #endif
  
        #if Command_Speed_minus
          case Command_Speed_minus :                ///////////////////////////////////////////////////////////////////////////  Замедлить движение
                                    if (thisdelay< 1000) thisdelay++;
                                    sendDSpeed();
                                    #if LOG_ON == 1
                                      Serial.print(F("Speed- ")); Serial.println(thisdelay); 
                                    #endif
                                    break;         
        #endif
  
        #if Command_Speed_plus
          case Command_Speed_plus :                ///////////////////////////////////////////////////////////////////////////  Ускорить движение
                                    if (thisdelay>0) thisdelay--;
                                    sendDSpeed();
                                    #if LOG_ON == 1
                                      Serial.print(F("Speed+ ")); Serial.println(thisdelay); 
                                    #endif
                                    break;         
        #endif
  
        #if Command_Glitter
          case Command_Glitter :                ///////////////////////////////////////////////////////////////////////////  Включить/выключить сверкание
                                    if (Protocol == 1) {          //отключить повтор
                                      glitter = !glitter;
                                      sendGlitter();
                                      #if LOG_ON == 1
                                        Serial.print(F("Glitter "));Serial.println(glitter); 
                                      #endif
                                    }
                                    break;         
        #endif

          #if Command_BackGround
          case Command_BackGround :                ///////////////////////////////////////////////////////////////////////////  Включить/выключить заполнение фона
                                    if (Protocol == 1) {          //отключить повтор
                                      background = !background;
                                      sendGround();
                                      #if LOG_ON == 1
                                        Serial.print(F("BackGround "));Serial.println(background); 
                                      #endif
                                    }
                                    break;         
        #endif

          #if Command_Candle
          case Command_Candle :                ///////////////////////////////////////////////////////////////////////////  Включить/выключить Свечки
                                    #if CANDLE_KOL >0
                                      if (Protocol == 1) {          //отключить повтор
                                        candle = !candle;
                                        sendCandle();
                                        PolCandle = random8(CANDLE_KOL);
                                        #if LOG_ON == 1
                                          Serial.print(F("Candle "));Serial.println(candle); 
                                        #endif
                                      }
                                    #endif
                                    break;         
        #endif

        #if Command_Previous_mode
          case Command_Previous_mode :          ///////////////////////////////////////////////////////////////////////////  Предыдущий эффект. Отключает демонстрационный режим
                                    if (Protocol == 1) {          //отключить повтор
                                      if (newMode>0)  SetMode(newMode-1);
                                      else            SetMode(maxMode-1);
                                      #if LOG_ON == 1
                                        Serial.println(F("Previous mode"));
                                      #endif
                                    }
                                    break;         
        #endif

        #if Command_Previous_mode_Demo
          case Command_Previous_mode_Demo :          ///////////////////////////////////////////////////////////////////////////  Предыдущий эффект. Оставляет демонстрационный режим
                                    if (Protocol == 1) {          //отключить повтор
                                      if (newMode>0)  SetMode(newMode-1);
                                      else            SetMode(maxMode-1);
                                      SetOn(demorun);
                                      #if LOG_ON == 1
                                        Serial.println(F("Previous mode + Demo"));
                                      #endif
                                    }
                                    break;         
        #endif

        #if Command_Next_mode
          case Command_Next_mode :              ///////////////////////////////////////////////////////////////////////////  Следующий эффект. Отключает демонстрационный режим
                                    if (Protocol == 1) {          //отключить повтор
                                      if (newMode >=(maxMode-1))  SetMode(0);
                                      else                      SetMode(newMode+1);
                                      #if LOG_ON == 1
                                        Serial.println(F("Next mode"));
                                      #endif
                                    }
                                    break;         
        #endif

        #if Command_Next_mode_Demo
          case Command_Next_mode_Demo :              ///////////////////////////////////////////////////////////////////////////  Следующий эффект. Оставлякт демонстрационный режим
                                    if (Protocol == 1) {          //отключить повтор
                                      if (newMode >=(maxMode-1))  SetMode(0);
                                      else                      SetMode(newMode+1);
                                      SetOn(demorun);
                                      #if LOG_ON == 1
                                        Serial.println(F("Next mode + Demo"));
                                      #endif
                                    }
                                    break;         
        #endif

        #if Command_Mode_0
          case Command_Mode_0 :                ///////////////////////////////////////////////////////////////////////////  Эффект 0
                                    SetMode (0);   break; 
        #endif
        
        #if Command_Mode_1
          case Command_Mode_1 :                ///////////////////////////////////////////////////////////////////////////  Эффект 1
                                    SetMode (1);   break; 
        #endif
        
        #if Command_Mode_2
          case Command_Mode_2 :                ///////////////////////////////////////////////////////////////////////////  Эффект 2
                                    SetMode (2);   break; 
        #endif
        
        #if Command_Mode_3
          case Command_Mode_3 :                ///////////////////////////////////////////////////////////////////////////  Эффект 3
                                    SetMode (3);   break; 
        #endif
        
        #if Command_Mode_4
          case Command_Mode_4 :                ///////////////////////////////////////////////////////////////////////////  Эффект 4
                                    SetMode (4);   break; 
        #endif
        
        #if Command_Mode_5
          case Command_Mode_5 :                ///////////////////////////////////////////////////////////////////////////  Эффект 5
                                    SetMode (5);   break; 
        #endif
        
        #if Command_Mode_6
          case Command_Mode_6 :                ///////////////////////////////////////////////////////////////////////////  Эффект 6
                                    SetMode (6);   break; 
        #endif
        
        #if Command_Mode_7
          case Command_Mode_7 :                ///////////////////////////////////////////////////////////////////////////  Эффект 7
                                    SetMode (7);   break; 
        #endif

        #if Command_Mode_8
          case Command_Mode_8 :                ///////////////////////////////////////////////////////////////////////////  Эффект 8
                                    SetMode (8);   break; 
        #endif
        
        #if Command_Mode_9
          case Command_Mode_9 :                ///////////////////////////////////////////////////////////////////////////  Эффект 9
                                    SetMode (9);   break; 
        #endif
        
        #if Command_Mode_10
          case Command_Mode_10 :                ///////////////////////////////////////////////////////////////////////////  Эффект 10
                                    SetMode (10);   break; 
        #endif
        
        #if Command_Mode_11
          case Command_Mode_11 :                ///////////////////////////////////////////////////////////////////////////  Эффект 11
                                    SetMode (11);   break; 
        #endif
        
        #if Command_Mode_12
          case Command_Mode_12 :                ///////////////////////////////////////////////////////////////////////////  Эффект 12
                                    SetMode (12);   break; 
        #endif
        
        #if Command_Mode_13
          case Command_Mode_13 :                ///////////////////////////////////////////////////////////////////////////  Эффект 13
                                    SetMode (13);   break; 
        #endif
        
        #if Command_Mode_14
          case Command_Mode_14 :                ///////////////////////////////////////////////////////////////////////////  Эффект 14
                                    SetMode (14);   break; 
        #endif
        
        #if Command_Mode_15
          case Command_Mode_15 :                ///////////////////////////////////////////////////////////////////////////  Эффект 15
                                    SetMode (15);   break; 
        #endif
        
        #if Command_Mode_16
          case Command_Mode_16 :                ///////////////////////////////////////////////////////////////////////////  Эффект 16
                                    SetMode (16);   break; 
        #endif
        
        #if Command_Mode_17
          case Command_Mode_17 :                ///////////////////////////////////////////////////////////////////////////  Эффект 17
                                    SetMode (17);   break; 
        #endif

        #if Command_Mode_18
          case Command_Mode_18 :                ///////////////////////////////////////////////////////////////////////////  Эффект 18
                                    SetMode (18);   break; 
        #endif
        
        #if Command_Mode_19
          case Command_Mode_19 :                ///////////////////////////////////////////////////////////////////////////  Эффект 19
                                    SetMode (19);   break; 
        #endif
        
        #if Command_Mode_20
          case Command_Mode_20 :                ///////////////////////////////////////////////////////////////////////////  Эффект 20
                                    SetMode (20);   break; 
        #endif
        
        #if Command_Mode_21
          case Command_Mode_21 :                ///////////////////////////////////////////////////////////////////////////  Эффект 21
                                    SetMode (21);   break; 
        #endif
        
        #if Command_Mode_22
          case Command_Mode_22 :                ///////////////////////////////////////////////////////////////////////////  Эффект 22
                                    SetMode (22);   break; 
        #endif
        
        #if Command_Mode_23
          case Command_Mode_23 :                ///////////////////////////////////////////////////////////////////////////  Эффект 23
                                    SetMode (23);   break; 
        #endif
        
        #if Command_Mode_24
          case Command_Mode_24 :                ///////////////////////////////////////////////////////////////////////////  Эффект 24
                                    SetMode (24);   break; 
        #endif
        
        #if Command_Mode_25
          case Command_Mode_25 :                ///////////////////////////////////////////////////////////////////////////  Эффект 25
                                    SetMode (25);   break; 
        #endif
        
        #if Command_Mode_26
          case Command_Mode_26 :                ///////////////////////////////////////////////////////////////////////////  Эффект 26
                                    SetMode (26);   break; 
        #endif
        
        #if Command_Mode_27
          case Command_Mode_27 :                ///////////////////////////////////////////////////////////////////////////  Эффект 27
                                    SetMode (27);   break; 
        #endif

        #if Command_Mode_28
          case Command_Mode_28 :                ///////////////////////////////////////////////////////////////////////////  Эффект 28
                                    SetMode (28);   break; 
        #endif
        
        #if Command_Mode_29
          case Command_Mode_29 :                ///////////////////////////////////////////////////////////////////////////  Эффект 29
                                    SetMode (29);   break; 
        #endif
        
        #if Command_Mode_30
          case Command_Mode_0 :                ///////////////////////////////////////////////////////////////////////////  Эффект 30
                                    SetMode (30);   break; 
        #endif
        
        #if Command_Mode_31
          case Command_Mode_31 :                ///////////////////////////////////////////////////////////////////////////  Эффект 31
                                    SetMode (31);   break; 
        #endif
        
        #if Command_Mode_32
          case Command_Mode_32 :                ///////////////////////////////////////////////////////////////////////////  Эффект 32
                                    SetMode (32);   break; 
        #endif
        
        #if Command_Mode_33
          case Command_Mode_33 :                ///////////////////////////////////////////////////////////////////////////  Эффект 33
                                    SetMode (33);   break; 
        #endif
        
        #if Command_Mode_34
          case Command_Mode_34 :                ///////////////////////////////////////////////////////////////////////////  Эффект 34
                                    SetMode (34);   break; 
        #endif
        
        #if Command_Mode_35
          case Command_Mode_35 :                ///////////////////////////////////////////////////////////////////////////  Эффект 35
                                    SetMode (35);   break; 
        #endif
        
        #if Command_Mode_36
          case Command_Mode_36 :                ///////////////////////////////////////////////////////////////////////////  Эффект 36
                                    SetMode (36);   break; 
        #endif
        
        #if Command_Mode_37
          case Command_Mode_37 :                ///////////////////////////////////////////////////////////////////////////  Эффект 37
                                    SetMode (37);   break; 
        #endif

        #if Command_Mode_38
          case Command_Mode_38 :                ///////////////////////////////////////////////////////////////////////////  Эффект 38
                                    SetMode (38);   break; 
        #endif
        
        #if Command_Mode_39
          case Command_Mode_39 :                ///////////////////////////////////////////////////////////////////////////  Эффект 39
                                    SetMode (39);   break; 
        #endif
        
        #if Command_Mode_40
          case Command_Mode_40 :                ///////////////////////////////////////////////////////////////////////////  Эффект 40
                                    SetMode (40);   break; 
        #endif
 
        #if Command_Mode_41
          case Command_Mode_41 :                ///////////////////////////////////////////////////////////////////////////  Эффект 41
                                    SetMode (41);   break; 
        #endif
 
       
        #if Command_Save_Mode
          case Command_Save_Mode :              ///////////////////////////////////////////////////////////////////////////  Сохранить эффект как запускающийся первым
                                    if (Protocol == 1) {          //отключить повтор
                                     #if SAVE_EEPROM >0
                                      EEPROM.write(STARTMODE,ledMode); 
                                      #if !defined(__AVR__)
                                         EEPROM.commit();
                                      #endif
                                     #endif
                                     sendSetMode();
                                      #if LOG_ON == 1
                                        Serial.println(F("Save Mode"));
                                      #endif
                                    }
                                    break;         
        #endif
        
       #if Command_Delay_minus
          case Command_Delay_minus :            ///////////////////////////////////////////////////////////////////////////  Уменьшить задержку на 100ms
                                    SetOff(demorun); ledMode = 201;
                                    if (meshdelay > 0) meshdelay--;                                   //Новое значение 
                                     #if SAVE_EEPROM >0
                                      EEPROM.write(STRANDEL,meshdelay);
                                      #if !defined(__AVR__)
                                         EEPROM.commit();
                                      #endif
                                    #endif
                                    sendDelay();
                                   #if LOG_ON == 1
                                      Serial.print(F("Delay ")); Serial.print(meshdelay*100); Serial.println(F(" ms"));
                                    #endif
                                    break;         
        #endif

       #if Command_Delay_plus
          case Command_Delay_plus :            ///////////////////////////////////////////////////////////////////////////  Увеличить задержку на 100ms
                                    SetOff(demorun); ledMode = 201;
                                    if (meshdelay <100) meshdelay++;                                  //Новое значение 
                                    #if SAVE_EEPROM >0
                                      EEPROM.write(STRANDEL,meshdelay);
                                      #if !defined(__AVR__)
                                         EEPROM.commit();
                                      #endif
                                    #endif
                                    sendDelay();
                                    #if LOG_ON == 1
                                      Serial.print(F("Delay ")); Serial.print(meshdelay*100); Serial.println(F(" ms"));
                                    #endif
                                    break;         
        #endif

       #if Command_Palette_Stop
          case Command_Palette_Stop :            ///////////////////////////////////////////////////////////////////////////  Остановить перебор Палитр
                                    palchg = 0;
                                    #if LOG_ON == 1
                                      Serial.print(F("Palette_Stop "));Serial.println(gCurrentPaletteNumber);
                                    #endif
                                    break;         
        #endif

       #if Command_Palette_Previous
          case Command_Palette_Previous :        ///////////////////////////////////////////////////////////////////////////  Установить предыдущую палитру
                                    if (Protocol == 1) {          //отключить повтор
                                      palchg = 1; 
                                      if (gCurrentPaletteNumber > 0)  gCurrentPaletteNumber--; 
                                      else                            gCurrentPaletteNumber = gGradientPaletteCount-1; 
                                      gTargetPalette = (gGradientPalettes[gCurrentPaletteNumber]); 
                                      sendPal();                                            //отправить палитру                                      
                                      #if LOG_ON == 1
                                        Serial.print(F("Palette- "));Serial.println(gCurrentPaletteNumber);
                                      #endif
                                    }
                                    break;         
        #endif

       #if Command_Palette_Next
          case Command_Palette_Next :            ///////////////////////////////////////////////////////////////////////////  Установить следующую палитру
                                    if (Protocol == 1) {          //отключить повтор
                                      palchg = 2; 
                                      if (gCurrentPaletteNumber < (gGradientPaletteCount-1))  gCurrentPaletteNumber++; 
                                      else                                                gCurrentPaletteNumber = 0; 
                                      gTargetPalette = (gGradientPalettes[gCurrentPaletteNumber]); 
                                      sendPal();                                            //отправить палитру                                      
                                      #if LOG_ON == 1
                                        Serial.print(F("Palette+ "));Serial.println(gCurrentPaletteNumber);
                                      #endif
                                    }
                                    break;         
        #endif

       #if Command_Palette_Auto
          case Command_Palette_Auto :            ///////////////////////////////////////////////////////////////////////////  Включить перебор палитр
                                    palchg = 3; 
                                    #if LOG_ON == 1
                                      Serial.println(F("Continuous palette change"));
                                    #endif
                                    break;         
        #endif

       #if Command_Solid_Black
          case Command_Solid_Black :            ///////////////////////////////////////////////////////////////////////////  Установить цвет Черный
                                    SetOff(demorun); ledMode = 255; palchg=0;
                                    solid = CRGB::Black;
                                    #if LOG_ON == 1
                                      Serial.println(F("Solid_Black"));
                                    #endif
                                    break;         
        #endif

       #if Command_Solid_Red
          case Command_Solid_Red :            ///////////////////////////////////////////////////////////////////////////  Установить цвет Красный
                                    SetOff(demorun); ledMode = 255; palchg=0;
                                    solid = CRGB::Red;
                                     #if LOG_ON == 1
                                      Serial.println(F("Solid_Red"));
                                    #endif
                                    break;         
        #endif

       #if Command_Solid_Orange
          case Command_Solid_Orange :            ///////////////////////////////////////////////////////////////////////////  Установить цвет Оранжевый
                                    SetOff(demorun); ledMode = 255; palchg=0;
                                    solid = CRGB::Orange;
                                     #if LOG_ON == 1
                                      Serial.println(F("Solid_Orange"));
                                    #endif
                                    break;         
        #endif

       #if Command_Solid_Yellow
          case Command_Solid_Yellow :            ///////////////////////////////////////////////////////////////////////////  Установить цвет Желтый
                                    SetOff(demorun); ledMode = 255; palchg=0;
                                    solid = CRGB::Yellow;
                                    #if LOG_ON == 1
                                      Serial.println(F("Solid_Yellow"));
                                    #endif
                                    break;         
        #endif

       #if Command_Solid_Green
          case Command_Solid_Green :            ///////////////////////////////////////////////////////////////////////////  Установить цвет Зеленый
                                    SetOff(demorun); ledMode = 255; palchg=0;
                                    solid = CRGB::Green;
                                    #if LOG_ON == 1
                                      Serial.println(F("Solid_Green"));
                                    #endif
                                    break;         
        #endif

       #if Command_Solid_SkyBlue
          case Command_Solid_SkyBlue :            ///////////////////////////////////////////////////////////////////////////  Установить цвет Голубой
                                    SetOff(demorun); ledMode = 255; palchg=0;
                                    solid = CRGB::SkyBlue;
                                    #if LOG_ON == 1
                                      Serial.println(F("Solid_SkyBlue"));
                                    #endif
                                    break;         
        #endif

       #if Command_Solid_Blue
          case Command_Solid_Blue :            ///////////////////////////////////////////////////////////////////////////  Установить цвет Синий
                                    SetOff(demorun); ledMode = 255; palchg=0;
                                    solid = CRGB::Blue;
                                    #if LOG_ON == 1
                                      Serial.println(F("Solid_Blue"));
                                    #endif
                                    break;         
        #endif

       #if Command_Solid_Violet
          case Command_Solid_Violet :            ///////////////////////////////////////////////////////////////////////////  Установить цвет Фиолетовый
                                    SetOff(demorun); ledMode = 255; palchg=0;
                                    solid = CRGB::Violet;
                                    #if LOG_ON == 1
                                      Serial.println(F("Solid_Violet"));
                                    #endif
                                    break;         
        #endif

       #if Command_Solid_White
          case Command_Solid_White :            ///////////////////////////////////////////////////////////////////////////  Установить цвет Белый
                                    SetOff(demorun); ledMode = 255; palchg=0;
                                    solid = CRGB::White;
                                    #if LOG_ON == 1
                                      Serial.println(F("Solid_White"));
                                    #endif
                                    break;         
        #endif

        default:     break;                // We could do something by default

      } // switch Command
    }
    else
    {   //        Режим настройки
      LED2_On;                                                      //Включить светодиод
      #if LOG_ON == 1
        Serial.print(F("Setup Command: 0x"));Serial.println(Command,HEX);
      #endif

      switch(Command) {
        
        #if Setup_Command_Setup_Mode_Off
          case Setup_Command_Setup_Mode_Off :              //////////////////////////////////////////////////////////////////////////  Выйти из режима настройки
              #if Setup_Command_Setup_Mode_Off1
                case Setup_Command_Setup_Mode_Off1 :       //////////////////////////////////////////////////////////////////////////  Выйти из режима настройки
              #endif
              #if Setup_Command_Setup_Mode_Off2
                case Setup_Command_Setup_Mode_Off2 :       //////////////////////////////////////////////////////////////////////////  Выйти из режима настройки
              #endif
              #if Setup_Command_Setup_Mode_Off3
                case Setup_Command_Setup_Mode_Off3 :       //////////////////////////////////////////////////////////////////////////  Выйти из режима настройки
              #endif
//                                    ledMode = newMode;
//                                    SetOn(demorun);     
                                    #if SAVE_EEPROM >0
                                      #if MAX_LEDS < 255
                                        EEPROM.write(STRANDLEN, NUM_LEDS);                              //Сохранить в память
                                        EEPROM.write(STRANDLEN+1, 0);                                   //Сохранить в память
                                      #else
                                        EEPROM.write(STRANDLEN,   (uint16_t)(NUM_LEDS) & 0x00ff);       //Сохранить в память
                                        EEPROM.write(STRANDLEN+1, (uint16_t)(NUM_LEDS) >> 8);           //Сохранить в память
                                      #endif
                                      if ( ledMode == 221) ExtFlag.RedGreen = ! ExtFlag.RedGreen;
                                      
                                      EEPROM.write(EXTFLAG, ExtFlag.Byte);                    // сохраним в EPROM расширенные настройки 
                                      #if !defined(__AVR__)
                                         EEPROM.commit();
                                      #endif
                                    #endif
                                    #if LOG_ON == 1
                                        Serial.println(F("Setup Mode Off "));
                                        Serial.print(F("Led:"));Serial.println(NUM_LEDS);
                                        Serial.print(F("RGB:"));Serial.println(ExtFlag.RedGreen);
                                        Serial.println(F("Reset "));
                                    #endif
                                    sendSetKolLed();    //Послать установленное количество светодиодов
                                    sendSetRGB();       //Послать установленый RGB
                                    sendSetGliter();    //Послать установлены ли вспышки
                                    sendSetCandle();    //Послать установлены ли свечи
                                    sendSetGround();    //Послать установлены ли фон
                                    sendSetMode();      //Послать С какого эффекта начинать
                                    sendSetDemoRun();   //Послать с какого демо режима начинать

                                    LED2_Off;                                                      //Выключить светодиод
                                   bootme();      
                                    break;   
        #endif
        #if Setup_Command_Length_Garland_plus
          case Setup_Command_Length_Garland_plus : ///////////////////////////////////////////////////////////////////////////  Увеличить количество светодиодов в гирлянде 
                                    if (NUM_LEDS < MAX_LEDS) NUM_LEDS++;                                                      //Новое значение 
                                    sendSetKolLed();    //Послать установленное количество светодиодов
                                    #if LOG_ON == 1
                                      Serial.print(F("Length Garland Plus: ")); Serial.println(NUM_LEDS);
                                    #endif
                                    break;         
        #endif
  
        #if Setup_Command_Length_Garland_plus_speed
          case Setup_Command_Length_Garland_plus_speed : ///////////////////////////////////////////////////////////////////////////  Увеличить количество светодиодов в гирлянде 
                                    if (NUM_LEDS < (MAX_LEDS-10)) NUM_LEDS+= 10; else NUM_LEDS = MAX_LEDS;                          //Новое значение 
                                    sendSetKolLed();    //Послать установленное количество светодиодов
                                    #if LOG_ON == 1
                                      Serial.print(F("Length Garland Plus: ")); Serial.println(NUM_LEDS);
                                    #endif
                                    break;         
        #endif
  
        #if Setup_Command_Length_Garland_minus
          case Setup_Command_Length_Garland_minus : ///////////////////////////////////////////////////////////////////////////  Уменьшить количество светодиодов в гирлянде
                                    if (NUM_LEDS > 1) NUM_LEDS--;                                     //Новое значение 
                                    sendSetKolLed();    //Послать установленное количество светодиодов
                                    #if LOG_ON == 1
                                      Serial.print(F("Length Garland Munus: ")); Serial.println(NUM_LEDS);
                                    #endif
                                    break;         
        #endif

        #if Setup_Command_Length_Garland_minus_speed
          case Setup_Command_Length_Garland_minus_speed : ///////////////////////////////////////////////////////////////////////////  Уменьшить количество светодиодов в гирлянде
                                    if (NUM_LEDS > 10) NUM_LEDS-= 10; else NUM_LEDS = 1;              //Новое значение 
                                    sendSetKolLed();    //Послать установленное количество светодиодов
                                    #if LOG_ON == 1
                                      Serial.print(F("Length Garland Munus: ")); Serial.println(NUM_LEDS);
                                    #endif
                                    break;         
        #endif

        #if Setup_Command_Solid_Green
          case Setup_Command_Solid_Green :            ///////////////////////////////////////////////////////////////////////////  Установить цвет Зеленый
                                    ledMode = 221;              //Новое значение 
                                    #if LOG_ON == 1
                                      Serial.println(F("Green Red Blue"));
                                    #endif
                                    break;         
        #endif

       #if Setup_Command_Solid_Red
          case Setup_Command_Solid_Red :            ///////////////////////////////////////////////////////////////////////////  Установить цвет Красный
                                    ledMode = 220;              //Новое значение 
                                    #if LOG_ON == 1
                                      Serial.println(F("Red Green Blue"));
                                    #endif
                                    break;         
        #endif

        default:     break;                // We could do something by default

      } // switch Command

      
    }
    Protocol = 0;                                             // Reset Protocol variable to not read the same value twice.

  } // if Protocol

} // getirl()


#endif
