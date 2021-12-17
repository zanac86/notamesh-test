//
#if PROTOCOL_ON > 0

#define LOG_PARSER 0
#define LOG_PARSER1 0



 void  IntToStr(char *s, uint16_t c, uint16_t x, uint8_t kol)
{  // вставить число с в строку s начиная с позиции x, kol - количество символов
    uint16_t b;
	if (kol == 1)		b=1;
	else if (kol == 2) 	b=10;
	else if (kol == 3) 	b=100;	
	else if (kol == 4) 	b=1000;
	else if (kol == 5) 	b=10000;
	else return;
    for(uint8_t a=0;a<kol;a++)
    {   if (( c< b)&&(b>1)) s[x++]= ' ';
        else                s[x++]= (char)((c / b)% 10 + 0x30);
        b/=10;
    }       
}


  bool FindChar(const char * s, uint16_t dataLen, char c, uint16_t *x)
  //Поиск в строке s символа с, x -содержит найденую позицию, если символ не найден  то функция возращает false
  { bool flag = false;
    while ( *x < dataLen)
    { if ( s[*x] == c)                                      //Нашли?
      { flag = true;
        break;
      }
      *x = *x +1;                                           // переход на следующий элемент 
    }
    return flag;
  }

uint16_t findStringStart(const char *Data, uint16_t dataLen, const char *Str, uint16_t startFind) {
  //поиск в строке Data строки Str, dataLen размер строки Data, startFind с какого положения  Data искать строку Str
  int i,j;
  int ln;
  for (ln=0; Str[ln]!=0; ++ln);              //Расчет длины Str
            #if LOG_PARSER == 1
              Serial.print(F("Parser: Data = "));
              Serial.print(Data);
              Serial.print(F(" Str = "));
              Serial.print (Str);
              Serial.print(F(" dataLen = "));
              Serial.print(dataLen);
              Serial.print(F(" startFind = "));
              Serial.print(startFind);
              Serial.print(F(" ln = "));
              Serial.println(ln);
            #endif
  int match = 0;
  if (dataLen<=startFind) return false;                   // если положение больше или равно размеру строки то не нашли
  for (i=startFind; i < dataLen; i++) {
    match = 0;
    for (j=0; Str[j]!=0; ++j) {
      if (i+j >= dataLen)    { //
        
            #if LOG_PARSER == 1
              Serial.print(F("Parser false: i = "));
              Serial.print(i);
              Serial.print(F(" j = "));
              Serial.println(j);
            #endif
        return false;                   //Закончился Data
      }
      if (Data[i+j] != Str[j]) { match = 0;break;}        // не совпало
      match++;
    }
    if (match == ln) {
            #if LOG_PARSER == 1
              Serial.print(F("Parser true: match = "));
              Serial.println(match);
            #endif
      return true;                          // нашли
    }
  }
  return false;
}

  uint16_t StrToInt(const char * s,uint16_t sLen, uint16_t n, uint16_t z)
  //преобразование строки s в число, начиная с n символа, в случае не удачи вернет число z, sLen Длинна принятой строки
  { bool flag = false;
    uint16_t result = 0;
    uint8_t cont =0;
            #if LOG_PARSER1 == 1
              Serial.print(F("StrToInt: s = "));
              Serial.print(s);
              Serial.print(F(" sLen = "));
              Serial.print(sLen);
              Serial.print(F(" n = "));
              Serial.print(n);
              Serial.print(F(" z = "));
              Serial.println(z);
            #endif
    while ( ( n < sLen) && (cont <5) )
    { if (( s[n] >= '0') && ( s[n] <= '9'))                 //Нашли?
      { flag = true;
        result = result*10+s[n]-'0';
      } else break;
      n++;                                           // переход на следующий элемент 
      cont++;
    }
            #if LOG_PARSER1 == 1
              Serial.print(F("result = "));
              Serial.print(result);
              Serial.print(F(" flag = "));
              Serial.print(flag);
              Serial.print(F(" n = "));
              Serial.print(n);
              Serial.print(F("cont = "));
              Serial.println(cont);
            #endif
    if (flag) return result;
    else      return z;
  }

  uint32_t StrToLong(const char * s,uint16_t sLen, uint16_t n, uint32_t z)
  //преобразование строки s в число, начиная с n символа, в случае не удачи вернет число z, sLen Длинна принятой строки
  { bool flag = false;
    uint32_t result = 0;
    uint8_t cont =0;
    while ( ( n < sLen) && (cont <10) )
    { if (( s[n] >= '0') && ( s[n] <= '9'))                 //Нашли?
      { flag = true;
        result = result*10+s[n]-'0';
      } else break;
      n++;                                           // переход на следующий элемент 
      cont++;
    }
    if (flag) return result;
    else      return z;
  }

void sendMode(uint8_t mode) {
  //Послать текущий режим
 char s[] = "$MODE=000;";
 IntToStr(s,mode,6,3);
 #if BLE_ON > 0
  SendBLEStr(s);
 #endif
}

void sendPal() {
  //Послать текущую палитру
 char s[] = "$PAL=000;";
 IntToStr(s,gCurrentPaletteNumber,5,3);
 #if BLE_ON > 0
  SendBLEStr(s);
 #endif
}

void sendBright() {
  //Послать текущую яркость
 char s[] = "$BRIGHT=000;";
 IntToStr(s,max_bright,8,3);
 #if BLE_ON > 0
  SendBLEStr(s);
 #endif
}

void sendDemoRun() {
  //Послать текущий деморежим
 char s[] = "$DEMORUN=00;";
 IntToStr(s,demorun,9,2);
 #if BLE_ON > 0
  SendBLEStr(s);
 #endif
}

void sendRotate() {
  //Послать направление
 char s[] = "$ROTATE=0;";
 if (thisdir>0) s[8]='1';
 #if BLE_ON > 0
  SendBLEStr(s);
 #endif
}

void sendGlitter() {
  //Послать включены ли вспышки
 char s[] = "$GLITTER=0;";
 if (glitter >0) s[9]='1';
 #if BLE_ON > 0
  SendBLEStr(s);
 #endif
}

void sendGround() {
  //Послать включен ли фон
 char s[] = "$GROUND=0;";
 if (background>0) s[8]='1';
 #if BLE_ON > 0
  SendBLEStr(s);
 #endif
}

void sendCandle() {
  //Послать включены ли свечи
 char s[] = "$CANDLE=0;";
 if (candle>0) s[8]='1';
 #if BLE_ON > 0
  SendBLEStr(s);
 #endif
}

void sendDelay() {
  //Послать значение задержки
 char s[] = "$DELAY=000;";
 IntToStr(s,meshdelay,7,3);
 #if BLE_ON > 0
  SendBLEStr(s);
 #endif
}

void sendDSpeed() {
  //Послать значение задержки
 char s[] = "$SPEED=000;";
 IntToStr(s,thisdelay,7,3);
 #if BLE_ON > 0
  SendBLEStr(s);
 #endif
}

void sendSetMAX_LEDS() {
  //Послать установленное количество светодиодов
 char s[] = "$SETMAX_LEDS=0000;";
 IntToStr(s,MAX_LEDS,13,4);
 #if BLE_ON > 0
  SendBLEStr(s);
 #endif
}

void sendSetKolLed() {
  //Послать установленное количество светодиодов
 char s[] = "$SETKOLLED=0000;";
 IntToStr(s,NUM_LEDS,11,4);
 #if BLE_ON > 0
  SendBLEStr(s);
 #endif
}


void sendSetRGB() {
  //Послать установленый RGB
 char s[] = "$SETRGB=0;";
 IntToStr(s,ExtFlag.RedGreen,8,1);
 #if BLE_ON > 0
  SendBLEStr(s);
 #endif
}

void sendSetGliter() {
  //Послать установлены ли вспышки
 char s[] = "$SETGLITTER=0;";
 IntToStr(s,ExtFlag.Glitter,12,1);
 #if BLE_ON > 0
  SendBLEStr(s);
 #endif
}

void sendSetCandle() {
  //Послать установлены ли свечи
 char s[] = "$SETCANDLE=0;";
 IntToStr(s,ExtFlag.Candle,11,1);
 #if BLE_ON > 0
  SendBLEStr(s);
 #endif
}

void sendSetGround() {
  //Послать установлены ли фон
 char s[] = "$SETGROUND=0;";
 IntToStr(s,ExtFlag.Background,11,1);
 #if BLE_ON > 0
  SendBLEStr(s);
 #endif
}

void sendSetMode() {
  //Послать С какого эффекта начинать
 char s[] = "$SETMODE=000;";
 IntToStr(s,EEPROM.read(STARTMODE),9,3);
 #if BLE_ON > 0
  SendBLEStr(s);
 #endif
}

void sendSetDemoRun() {
  //Послать с какого демо режима начинать
 char s[] = "$SETDEMORUN=0";
 IntToStr(s,DEMO_MODE,12,1);
 #if BLE_ON > 0
  SendBLEStr(s);
 #endif
}

	void Parser(const char * s)									
	{ //Обработаем что пришло
		uint16_t idBegin=0;                                     //Указатель на начало команды
		uint16_t sLen= strlen(s);                               //Длинна принятой строки

		while (idBegin < sLen)                          		//Делаем пока не закончится принятая строка
		{ if (FindChar(s,sLen,'$',&idBegin))                       //Найдем начало команды
		  { uint16_t idEnd=idBegin;                             //Указатель на конец команды
			uint16_t idParam=idBegin;                           //Указатель на параметр
			if (FindChar(s,sLen,';',&idEnd))                       //Найдем конец команды
			{ if (!FindChar(s,sLen,'=',&idParam)) idParam=idEnd;   //Найдем начало параметра
			  uint8_t L;                                        //Длинна команды
			  if (idParam<idEnd)  L= idParam -idBegin-1; 
			  else                L= idEnd -idBegin-1; 
/*
          #if LOG_ON == 1
          Serial.print(F("L="));
          Serial.println(L);
          Serial.print(F("B="));
          Serial.println(idBegin);
          Serial.print(F("P="));
          Serial.println(idParam);
          Serial.print(F("E="));
          Serial.println(idEnd);
          #endif
*/
			  if ((L==7) &&                                      ///////////////////////////////////////////// Запрос состояния
			    findStringStart(s,idParam,"REQUEST",idBegin+1)) {
          if (demorun>=100) {
            #if LOG_ON == 1
              Serial.println(F("Parser: Setup Mode Off "));
            #endif
            SetOn(demorun); 
            ledMode = newMode; 
          } else {
  				  #if LOG_ON == 1
  					Serial.println(F("Parser: Request"));
  				  #endif
          }
          sendMode(ledMode);  //Послать текущий режим
          sendPal();          //Послать текущую палитру
          sendBright();       //Послать текущую яркость
          sendDemoRun();      //Послать текущий деморежим
          sendRotate();       //Послать направление
          sendGlitter();      //Послать включены ли вспышки
          sendGround();       //Послать включен ли фон
          sendCandle();       //Послать включены ли свечи
          sendDelay();        //Послать значение задержки
          sendDSpeed();       //Послать значение задержки
			  }	else
        if ((L==8) &&                                      ///////////////////////////////////////////// Запрос состояния Вход в режим настройки
          findStringStart(s,idParam,"REQUEST1",idBegin+1)) {
          if (demorun< 100) demorun += 100; ledMode = 220;
          #if LOG_ON == 1
            Serial.println(F("Parser: Setup Mode On "));
          #endif
          sendSetMAX_LEDS();  //Послать максимальное количество светодиодов
          sendSetKolLed();    //Послать установленное количество светодиодов
          sendSetRGB();       //Послать установленый RGB
          sendSetGliter();    //Послать установлены ли вспышки
          sendSetCandle();    //Послать установлены ли свечи
          sendSetGround();    //Послать установлены ли фон
          sendSetMode();      //Послать С какого эффекта начинать
          sendSetDemoRun();   //Послать с какого демо режима начинать
        } else
			  if ((L==6) && (idParam != idEnd) &&                   ///////////////////////////////////////////// Установить яркость
			    findStringStart(s,idParam,"BRIGHT",idBegin+1) )  {
				  max_bright = StrToInt(s,idEnd, idParam+1, 255);
				  LEDS.setBrightness(max_bright); 
				  #if LOG_ON == 1
					Serial.print(F("Parser: Brightness= ")); Serial.println(max_bright);
				  #endif
		    } else

			  if ((L==5) &&                                         ///////////////////////////////////////////// Сброс всех настроек
          findStringStart(s,idParam,"RESET",idBegin+1))  {
				  #if LOG_ON == 1
					Serial.println(F("Parser: Reset"));
				  #endif
          ledMode = 0; strobe_mode(ledMode,1); FastLED.show(); bootme(); 
			  } else 
				
			  if ((L==4) &&                                         ///////////////////////////////////////////// Останов
			    findStringStart(s,idParam,"STOP",idBegin+1)) {
				  SetOff(demorun); 
				  ledMode = 255; palchg=0;
				  solid = CRGB::Black;
				  FastLED.setBrightness(0);
				  #if LOG_ON == 1
					Serial.println(F("Parser: Stop"));
				  #endif
			  } else
				
			  if ((L==5)  &&                                        ///////////////////////////////////////////// Старт
			    findStringStart(s,idParam,"START",idBegin+1)) {
				  SetOn(demorun);  
				  ledMode = newMode;    
				  LEDS.setBrightness(max_bright);
				  #if LOG_ON == 1
					Serial.println(F("Parser: Start"));
				  #endif
			  } else
        				
			  if ((L==7) && (idParam != idEnd) &&                   ///////////////////////////////////////////// Включить демо режим (0-4, 255)
			    findStringStart(s,idParam,"DEMORUN",idBegin+1) )  {                     //    255 - Востановим демо режим (какой был до этого)
				  uint8_t x = StrToInt(s,idEnd, idParam+1, 255);
				  if ( x <=4)
				  { demorun = x; meshwait(); 
					#if LOG_ON == 1
					   Serial.print(F("Parser: Set Demo mode= ")); Serial.println(demorun);
					#endif
				  }
				  else if (x ==255){  
					SetOn(demorun); meshwait(); 
					#if LOG_ON == 1
						Serial.println(F("Parser: Demo On"));
					#endif;
				  }
			  } else

			  if ((L==6) &&                                        ///////////////////////////////////////////// Сменить направление движения эффектов
			    findStringStart(s,idParam,"ROTATE",idBegin+1))  {
          thisdir = thisdir*-1;
          sendRotate();
				  #if LOG_ON == 1
					Serial.println(F("Parser: Rotate"));
				  #endif
			  } else

			  if ((L==5) && (idParam != idEnd) &&                 ///////////////////////////////////////////// Скорость движения
          findStringStart(s,idParam,"SPEED",idBegin+1) )  {
				  uint16_t x = StrToInt(s,idEnd, idParam+1, 10);
				  if ( x <= 1000) thisdelay = x;
				  #if LOG_ON == 1
					 Serial.print(F("Parser: Set Speed= ")); Serial.println(thisdelay);
				  #endif
			  } else

			  if ((L==7) && (idParam != idEnd) &&                ///////////////////////////////////////////// Cверкание (0-выключить 1 включить)
          findStringStart(s,idParam,"GLITTER",idBegin+1) )  {
				  uint16_t x = StrToInt(s,idEnd, idParam+1, 0);
				  if ( x <= 1) glitter = x;
				  #if LOG_ON == 1
					 Serial.print(F("Parser: Set Glitter= ")); Serial.println(glitter);
				  #endif
			  } else

        if ((L==6) && (idParam != idEnd) &&               ///////////////////////////////////////////// Заполнение фона (0-выключить 1 включить)
          findStringStart(s,idParam,"GROUND",idBegin+1) )  {
				  uint16_t x = StrToInt(s,idEnd, idParam+1, 0);
				  if ( x <= 1) background = x;
				  #if LOG_ON == 1
					 Serial.print(F("Parser: Set BackGround= ")); Serial.println(background);
				  #endif
			  } else

        if ((L==6) && (idParam != idEnd) &&               ///////////////////////////////////////////// Свечки (0-выключить 1 включить)
          findStringStart(s,idParam,"CANDLE",idBegin+1) )  {
			    uint16_t x = StrToInt(s,idEnd, idParam+1, 0);
				  if ( x <= 1) {candle = x; PolCandle = random8(CANDLE_KOL);}
				  #if LOG_ON == 1
					 Serial.print(F("Parser: Set Candle= ")); Serial.println(candle);
				  #endif
			  } else

        if ((L==8) &&                                        ///////////////////////////////////////////// Предыдущий эффект. Отключает демонстрационный режим
          findStringStart(s,idParam,"PREVIOUS",idBegin+1))  {
				  if (newMode>0)  SetMode(newMode-1);
				  else            SetMode(maxMode-1);
				  #if LOG_ON == 1
					Serial.println(F("Parser: Previous mode"));
				  #endif
			  } else

			  if ((L==4) &&                                         ///////////////////////////////////////////// Следующий эффект. Отключает демонстрационный режим
          findStringStart(s,idParam,"NEXT",idBegin+1))  {
				  if (newMode >=(maxMode-1))  SetMode(0);
				  else                        SetMode(newMode+1);
				  #if LOG_ON == 1
					Serial.println(F("Parser: Next mode"));
				  #endif
			  } else

			  if ((L==7) && (idParam != idEnd) &&                 ///////////////////////////////////////////// Установить эффект
          findStringStart(s,idParam,"SETMODE",idBegin+1) )  {
				  uint16_t x = StrToInt(s,idEnd, idParam+1, 0);
				  if ( x < maxMode) {
					SetMode (x);
					#if LOG_ON == 1
					   Serial.print(F("Parser: Set mode= ")); Serial.println(x);
					#endif
				  }
			  } else

        if ((L==5) && (idParam != idEnd) &&                 ///////////////////////////////////////////// Скорость движения
          findStringStart(s,idParam,"DELAY",idBegin+1) )  {
				  uint16_t x = StrToInt(s,idEnd, idParam+1, 0);
				  if ( x <= 100) meshdelay = x;
				  #if LOG_ON == 1
					 Serial.print(F("Parser: Set delay= ")); Serial.println(meshdelay);
				  #endif
			 } else
			  
        if ((L==8) && (idParam != idEnd) &&                ///////////////////////////////////////////// Установить перебор Палитр
          findStringStart(s,idParam,"PAL_AUTO",idBegin+1) )  {
				  uint16_t x = StrToInt(s,idEnd, idParam+1, 0);
				  if ( x == 0) 
				  { palchg = 0;
					#if LOG_ON == 1
					  Serial.print(F("Parser: Palette change Off, Palette= ")); Serial.println(gCurrentPaletteNumber);
					#endif
				  }
				  if ( x == 1) 
				  { palchg = 3;
					#if LOG_ON == 1
					  Serial.print(F("Parser: Auto palette change, Palette=  ")); Serial.println(gCurrentPaletteNumber);
					#endif
				  }
			  } else

        if ((L==8) &&                                       ///////////////////////////////////////////// Установить предыдущую палитру
          findStringStart(s,idParam,"PAL_PREV",idBegin+1))  {
				  palchg = 1; 
				  if (gCurrentPaletteNumber > 0)  gCurrentPaletteNumber--; 
				  else                            gCurrentPaletteNumber = gGradientPaletteCount-1; 
				  gTargetPalette = (gGradientPalettes[gCurrentPaletteNumber]);
          sendPal();                                            //отправить палитру                                      
				  #if LOG_ON == 1
					Serial.print(F("Parser: Palette Previous Palette= ")); Serial.println(gCurrentPaletteNumber);
				  #endif
			  } else

			  if ((L==8) &&                                         ///////////////////////////////////////////// Установить следующую палитру
          findStringStart(s,idParam,"PAL_NEXT",idBegin+1))  {
				  palchg = 2; 
				  if (gCurrentPaletteNumber < (gGradientPaletteCount-1))  gCurrentPaletteNumber++; 
				  else                                                    gCurrentPaletteNumber = 0; 
				  gTargetPalette = (gGradientPalettes[gCurrentPaletteNumber]);
          sendPal();                                            //отправить палитру                                      
				  #if LOG_ON == 1
					Serial.print(F("Parser: Palette Next Palette= ")); Serial.println(gCurrentPaletteNumber);
				  #endif
			  } else

        if ((L==6) && (idParam != idEnd) &&                      ///////////////////////////////////////////// Установить палитру
          findStringStart(s,idParam,"SETPAL",idBegin+1) ) {
				  uint16_t x = StrToInt(s,idEnd, idParam+1, 0);
				  if ( x < gGradientPaletteCount) 
				  { gCurrentPaletteNumber = x; 
					gTargetPalette = (gGradientPalettes[gCurrentPaletteNumber]);
				  }
          sendPal();                                            //отправить палитру                                      
				  #if LOG_ON == 1
					 Serial.print(F("Parser: Set Palette= ")); Serial.println(gCurrentPaletteNumber);
				  #endif
			  } else

		    if ((L==5) &&  (idParam != idEnd) &&                  ///////////////////////////////////////////// Установить цвет
          findStringStart(s,idParam,"SOLID",idBegin+1) ) {
				  uint8_t r = StrToInt(s,idEnd, ++idParam, 0);                //Цвет R
				  if (FindChar(s,sLen,'=',&idParam)&&(idParam < idEnd))      //Найдем начало следующего числа
				  { uint8_t g = StrToInt(s,idEnd, ++idParam, 0);
					  if (FindChar(s,sLen,'=',&idParam)&&(idParam < idEnd))    //Найдем начало следующего числа
					  { uint8_t b = StrToInt(s,idEnd, ++idParam, 0);
					    SetOff(demorun); ledMode = 255; palchg=0;
					    solid = CRGB(r,g,b);
              #if LOG_ON == 1
                Serial.print(F("Parser: R= ")); 
                Serial.print(r);
                Serial.print(F(" G= ")); 
                Serial.print(g);
                Serial.print(F(" B= ")); 
                Serial.println(b);
              #endif
					  }
				  }
			  } else
        
        if ((L==9) && (idParam != idEnd) &&                      ///////////////////////////////////////////// Установить палитру
          findStringStart(s,idParam,"SETKOLLED",idBegin+1) ) {
          uint16_t x = StrToInt(s,idEnd, idParam+1, 0);
          if (x> MAX_LEDS) x = MAX_LEDS;
                    if (x != NUM_LEDS) {
            NUM_LEDS = x;                                                             //Новое значение 
            #if LOG_ON == 1
             Serial.print(F("Parser: Set Length Garland = ")); Serial.println(NUM_LEDS);
            #endif
          }
        } else
        
        if ((L==6) && (idParam != idEnd) &&                      ///////////////////////////////////////////// Установить палитру
          findStringStart(s,idParam,"SETRGB",idBegin+1) ) {
          uint16_t x = StrToInt(s,idEnd, idParam+1, 0);
          if (x>0) {
            ledMode = 220;              //Новое значение 
            #if LOG_ON == 1
              Serial.println(F("Parser: Set Red Green Blue"));
            #endif
          } else {
            ledMode = 221;              //Новое значение 
            #if LOG_ON == 1
              Serial.println(F("Parser: Set Green Red Blue"));
            #endif
          }
        } else
        
        if ((L==6) &&   (idParam != idEnd) &&              ///////////////////////////////////////////// Установить Flags настроек
          findStringStart(s,idParam,"SETUP1",idBegin+1) ) {
			    uint8_t rgb = StrToInt(s,idEnd, ++idParam, 0);              //Цвет RGB
				  if (FindChar(s,sLen,'=',&idParam)&&(idParam < idEnd))       //Найдем начало следующего числа
				  { uint8_t b = StrToInt(s,idEnd, ++idParam, 0);              //Блеск
					  if (FindChar(s,sLen,'=',&idParam)&&(idParam < idEnd))     //Найдем начало следующего числа
					  { uint8_t g = StrToInt(s,idEnd, ++idParam, 0);            //Фон
					    if (FindChar(s,sLen,'=',&idParam)&&(idParam < idEnd))   //Найдем начало следующего числа
					    { uint8_t c = StrToInt(s,idEnd, ++idParam, 0);          //Свечи
						    if (FindChar(s,sLen,'=',&idParam)&&(idParam < idEnd)) //Найдем начало следующего числа
						    { uint8_t d = StrToInt(s,idEnd, ++idParam, 0);        //Демо
						      BLEExtFlag.RedGreen=rgb;                        //Очередность  Цветов  RGB
						      BLEExtFlag.Glitter=b;                           //Флаг включения блеска
						      BLEExtFlag.Background=g;                        //Флаг включения заполнения фона
						      BLEExtFlag.Candle=c;                            //Флаг включения свечей

						      #if LOG_ON == 1
							      Serial.println(F("Parser: Setup Flags"));
						      #endif
						    }
					    }
					  }
				  }
			  } else
        if ((L==6) &&   (idParam != idEnd) &&               ///////////////////////////////////////////// Установить Flags настроек
          findStringStart(s,idParam,"SETUP2",idBegin+1) ) {
			    uint16_t L = StrToInt(s,idEnd, ++idParam, 100);              //Длинна гирлянлы
				  if (FindChar(s,sLen,'=',&idParam)&&(idParam < idEnd))      //Найдем начало следующего числа
				  { uint8_t N = StrToInt(s,idEnd, ++idParam, 0);               //С какого эффекта стартовать
					  if ( L > MAX_LEDS) L = MAX_LEDS;
					  #if SAVE_EEPROM >0
					  #if MAX_LEDS < 255
						EEPROM.write(STRANDLEN, L);                       //Сохранить в память
						EEPROM.write(STRANDLEN+1, 0);                     //Сохранить в память
					  #else
						EEPROM.write(STRANDLEN,   (uint16_t)(L) & 0x00ff);//Сохранить в память
						EEPROM.write(STRANDLEN+1, (uint16_t)(L) >> 8);    //Сохранить в память
					  #endif
					  EEPROM.write(STARTMODE, N);                         // сохраним в EEPROM номера режима с которого будет старт (байт)
					  EEPROM.write(STRANDEL, meshdelay);                  // сохраним в EPROM задержку (байт) 
					  EEPROM.write(EXTFLAG, BLEExtFlag.Byte);             // сохраним в EPROM расширенные настройки 
					  #if !defined(__AVR__)
						 EEPROM.commit();
					  #endif
					  #endif
					  #if LOG_ON == 1
					    Serial.println(F("Parser: Save setup to EEPROM"));
					    Serial.print(F("Led:"));Serial.println(L);
					    Serial.print(F("RGB:"));Serial.println(BLEExtFlag.RedGreen);
					    Serial.println(F("Reset "));
					  #endif
					  bootme();      
				  }
			  } else {
            #if LOG_ON == 1
              Serial.print(F("Not Parser: "));
              for (; idBegin<idParam; idBegin++)
                Serial.print(s[idBegin]);
              Serial.println(F(" "));  
            #endif
        }
			  idBegin = idEnd;
			} else idBegin = sLen;                      		//Заканчиваем цикл
		  } else idBegin = sLen;                        //Заканчиваем цикл
		}
	}
#else
  void sendMode(uint8_t mode) {}
  void sendPal() {}
  void sendBright() {}
  void sendDemoRun() {}
  void sendRotate() {}
  void sendGlitter() {}
  void sendGround() {}
  void sendCandle() {}
  void sendDelay() {}
  void sendDSpeed() {}
  void sendSetKolLed() {}
  void sendSetRGB() {}
  void sendSetGliter() {}
  void sendSetCandle() {}
  void sendSetGround() {}
  void sendSetMode() {}
  void sendSetDemoRun() {}
#endif
