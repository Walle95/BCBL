/////////////////////////////////////////////////////////////////////////////////////////////
//
//
//  Глобальные переменные и функции
//
//
////////////////////////////////////////////////////////////////////////////////////////////
#include <mbed.h>
#include <Stream.h>
#include "QEI/QEI_N.h"
#include "TextLCD/TextLCD.h"
#include "RPG/RPG.h"
#include "Alarm.h"
#include "GenButton.h"
#include "myRTC.h"


#include  "Lamp.cpp"


GenButton GeneralButton;
bool ButtonSaveChange;
bool ButtonClick;

Alarm alarm1;
bool alarm11;

myRTC myrtc;

bool a1 = 0;       //доп. переменные
bool b1 = 0;
bool a2 = 0;
bool b2 = 0;












QEI wheel (PB_6,PB_7,PC_13, 24); //(pinA, pinB, button, countobor)
RPG wheelRPG (PB_6,PB_7,PC_13);
//I2C i2c_lcd(I2C_SDA,I2C_SCL); // I2C_SDA, I2C_SCL
I2C i2c_lcd(D14,D15); // I2C_SDA, I2C_SCL
TextLCD_I2C lcd(&i2c_lcd, 0x4E, TextLCD_I2C::LCD16x2,TextLCD::HD44780);

const int COUNTLAMP=23; // Колличество ламп 24 в установке (23 т.к. массив начинается с 0)

PinName y[23] = {
    PB_0,
    PB_1,
    PB_2,
    PB_3,
    PB_4,
    PB_5,
    PB_8,
    PB_9,
    PB_10,
    PB_11,
    PB_12,
    PB_13,
    PB_14,
    PB_15,
    PC_0,
    PC_1,
    PC_2,
    PC_3,
    PC_4,
    PC_5,
    PC_6,
    PC_7,
    PC_8
   };

Lamp LampNumber[23]= {  
    PA_0,
    PA_1,
    PA_2,
    PA_3,
    PA_4,
    PA_5,
    PA_8,
    PA_9,
    PA_10,
    PA_11,
    PA_12,
    PA_13,
    PA_14,
    PA_15,
    PD_0,
    PD_1,
    PD_2,
    PC_9,
    PC_10,
    PC_11,
    PC_12,
    PC_14,
    PC_15  
};


void LampInit()
{
        for (size_t i = 0; i < COUNTLAMP; i++)
        {
            LampNumber[i].LampInit(1,0);
        }       
        

               
}

//---переменые энкодера для использования в программе----
int         rotateGeneral;                             //количество проворотов энкодера    
bool        button;                             //нажатие на кнопку (импульс)   
int         countB;                             //количество нажатий на кнопку           
bool        ChangeRotate;                       //фиксирование поворота энкодера

//---переменные для экрана и уровней----
bool LevelOpt;
bool LightDisp;
bool Level1Back;                                //возврат уровня            
int Level;
int NMenuL1;                                    //на какую строку уровня 1 будет преход
int NMenuL2;                                    //на какую строку уровня 2 будет преход
int ActMenuL2;                                  //на какой строке уровня 2 будет изменяться параметр
int NMenuL2_1;                                  //на какую строку подуровня 2_1 будет преход
int NMenuL2_2;                                  //на какую строку подуровня 2_2 будет преход
int8_t backL2_1;                                //возврат на уровень 2_1
int DispLightTime;                              //Время работы подсветки после бездействия
bool TikTimeD;
int TimeOldD;

//---Переменные для таймера---
Timer t;                                        //вызов таймера                
int SecSystem;                                  //системные секунды        
int SecSystemOld;
int Sec;
int Min;
int Hour;
bool TikTimeR;                                     //Счетчик Ресурсного временного промежутка 
int TimeOldR;                                      //сравнение с прошедшим значением Ресурсного времени

//Включение/выключение установки
bool OnOff;                                      //Состояние системы Вкл/Выкл               
bool OnOffold;                                   //Значение состояния системы предыдущего цикла  
int CountOnOff;                                  //количество включений установки
bool ResCountOnOff;                              //Сброс количества включений системы   
  


//---Ресурсное время работы 
int Resurs_Time_St;                              //Ресурсное время, выставленное 
int Resurs_Time_Set;                             //Ручное выставление ресурсного времени 
bool OnOffRes;                                   //Сокращение ресурса при вкл/выключении 
int KoefOnOffRes;                              //Коэффициент, который отвечет за снижение ресурса лампы от количества включений/выключений   
 
int L1Resurs_St;                                 //Ресурсное время лампы 1   
int L1Resurs_Set;                                //Ручное выставление ресурсного вермени лампы 1   
int L1Resusr_Res;                                //Сброс отработанного времени на выставленное лампы 1 
int L1Resusr_OSt;                                //оставшееся время ресурса лампы 1

int L2Resurs_St;                                 //Ресурсное время лампы 2   
int L2Resurs_Set;                                //Ручное выставление ресурсного вермени лампы 2   
int L2Resusr_Res;                                //Сброс отработанного времени на выставленное лампы 2 
int L2Resusr_OSt;                                //оставшееся время ресурса лампы 2

int SetPointGeneralResurs;                       //Общая уставка для всех ламп
//int CoefficientResourse;                         //Общий коэффициент ресурса ламп



//----Alarm----///
bool L1_Alarm;                                   //предупреждение об окончании ресурса лампы 1
int L1_AlarmT;                                   //Остаток времени, меньше которого появится предупреждение  

int OnOffint;

       

        //////////////////////////////////////Функции////////////////////////////////////////




//-------------------------------------------------------------------Функция чтения циклического значения в определенных пределах
int8_t CiclZnach (int8_t UpPredel, int8_t DnPredel, int16_t rotateCZ)       
{
           // printf("rotateCZ %i\n", rotateCZ);
           // printf("UpPredel %i\n", UpPredel);
    if  ((rotateCZ > UpPredel) or (rotateCZ < DnPredel))
        {
           wheel.reset();
           return 0;
        }
           return rotateCZ;
}
//------------------------------------------------------------------
DigitalInOut r(PC_0);

//------------------------------------------------------------------Функция для формирования импульса для различных промежутком времени
bool TikTime (int TimePrOld, int TimePr)                                   
{
        //printf("rotateCZ %i\n", TimePrOld);
        //printf("UpPredel %i\n", TimePr);
        if (TimePrOld != TimePr)
            {
               //TimePrOld = TimePr;
               return true; 
            }
               return false; 
}
//--------------------------------------------------------------------


//--------------------------------------------------------------------Счетчик времени
void TimerObsh (int TimerSec,Timer &t)              
{
    if (1)                                                    //Условие включения счетчика (планируется активация при включении установки). 
        {
            
            SecSystem = (int)t.read();              //Чтение системного вермени. Библиотека "Timer"    
                if (SecSystemOld != SecSystem)      //
                {
                        //------Секунды------//
                    Sec ++; 
                        if (Sec==60)
                        {  
                            Sec = 0;
                            Min ++;
                        }
                                //----Минуты----//
                        if  (Min == 60)
                        {  
                            Min = 0;
                            Hour ++;
                        }
                                //----Часы----//
                        if  (Hour == 24)
                        {  
                            Hour = 0;
                        }
                }   
            SecSystemOld = SecSystem; 
        }   
}
//--------------------------------------------------------------------





//---------------------------------------------------------------------Подсветка

bool BackLight (bool LightDisp)
{     
     if (LightDisp)
     {  
         lcd.setBacklight(lcd.LightOn);
        alarm11 = alarm1.indicatorAlarmLamp(true);     //индикация аварии (пока подвязал сюда для теста)
     }
     else
     {
         lcd.setBacklight(lcd.LightOff);
     }    
     return(0);     
}
//---------------------------------------------------------------------Логика кнопки
int Button (){
  
    ButtonSaveChange = GeneralButton.buttonSaveMode(button, Sec);  
    ButtonClick = GeneralButton.buttonOneClickFTrigger(button);
        
    return(0);    
}





//---------------------------------------------------------------------Индикатор аварии/предупреждения




//--------------------------------------------------------------------Исключение дребезка энкодера. включение/выключение подсветки экрана
void drebezg_encoder (int dredezg)              

{
    wait_us(dredezg); 
                                                    //использование задержки для исключения дребезга      
    int    rotateNew=wheel.getPulses();                          //Чтенеие поворотов энкодера. библиотека OEI  
    bool        buttonRPG=wheelRPG.pb();                              //Чтение "кнопочного" сигнала энкодера. библиотека RPG  
        ChangeRotate = (rotateGeneral!=rotateNew) ? true : false;            //Чтение поворота энкодера без проскальзывания
        if (ChangeRotate or button)                              //Условие включения подсветки дисплея  
        {   
               if (DispLightTime > 0) 
               {
                   LightDisp = 1;
               }
               else if (DispLightTime = 0)
               {
                   LightDisp = 0;
               }   
               BackLight(LightDisp);

               DispLightTime = 15;                                //Через 20 сек дисплей погаснет и вернется на главный экран 
        }
            else 
            {
                alarm11 = alarm1.indicatorAlarmLamp(false);  
                TikTimeD = TikTime (TimeOldD, Sec);                    //Функция для отчета времени с момента НЕактивности энкодера
                if (TikTimeD)
                    {
                        if (DispLightTime > 0)
                        {
                            DispLightTime--;
                        } 
                            else if (Level != 0)
                            {
                               LightDisp = false;
                               lcd.cls();
                               Level=0;
                               countB = 0;
                               wheel.reset();
                            }
                    }
                TimeOldD = Sec;
                
            }
            if (DispLightTime == 0)
            {
                LightDisp = 0;
            }          


        //Ограничение задаваемых значений
        if (rotateGeneral > 9999)
        {            
            wheel.reset();
            wheel.setPulses(0);                //??????????????????????????????????????    
        }
        if (rotateGeneral < 0)
        {            
            wheel.reset();
            wheel.setPulses(9999);                  //??????????????????????????????????????    
        }
            
        rotateGeneral=rotateNew;                                             //Поворот энкодера. Значение переменной, которое будет использоваться в программе 
        
             
        if (buttonRPG)                                                 //Условие чтения импульсного значения для переменной button, которая будет использоваться в программе
        {
            button = true;                                           
        }
        else
        {
            button = false;
        }
        if (ButtonClick)
        {
             countB++;
        }       

}

//------------------------------------------------------------------------

void ResursT ()                   // Ресурсное время работы установки в целом и каждой лампы в отдельности   
{
   printf("OnOff %i\n", OnOffint);
   if (OnOff)                                                                          //Когда установка включается, от ресурса отнимается время от включения, а затем уже идет                                                                                                
   {    
       //DI_Micro[0] = 1;                                                                               //стандартный ресурс (почасовой)
        if (!OnOffRes)                                                                 //импульс включения и вычета коэффициента от ресурса                 
            {
              for (size_t i = 0; i < COUNTLAMP; i++)
              {
                  (LampNumber[i].LampResusr_OSt > 0 && !DigitalIn(y[i])) ? LampNumber[i].LampResusr_OSt = LampNumber[i].LampResusr_OSt - KoefOnOffRes: LampNumber[i].LampResusr_OSt = LampNumber[i].LampResusr_OSt;
              }
              OnOffRes = true;  
            }
            else 
                {
                TikTimeR = TikTime (TimeOldR, Sec);
                
                if (TikTimeR)
                    {                        
                        for (size_t i = 0; i < COUNTLAMP; i++)
                        {
                            (LampNumber[i].LampResusr_OSt > 0 && !DigitalIn(y[i])) ? LampNumber[i].LampResusr_OSt--: LampNumber[i].LampResusr_OSt = LampNumber[i].LampResusr_OSt;
                        }
                    }
                TimeOldR = Sec;
                 }
    }
        else
        {
           OnOffRes = false;
        }
}

void LevelChoice (int LevelUD)                  //переход по уровням меню. 0 - отображение общего ресурса и остаточного ресурса, 1 - выбор лампы с остаточным ресурсом, 2 - настройки для каждого параметра.
{   
    wait_us(LevelUD);
    if (LightDisp and ButtonClick and (Level == 0))  //Условие, при котором, при нажатии на кнопку будет переход на уровень 1
    {
        lcd.cls();
        Level = 1;                              //меню выбора параметра для редактирования (Lamp1....)
        wheel.reset();                          //Сброс количества оборотов энкодера
        rotateGeneral = 0;                             //Обнуление оборотов переменной во всей программе
    }   
        else if (ButtonClick and (Level == 1) and /*!Level1Back*/ !LevelOpt)   //Условие пререхода на второй уровень (Начальнй ресурс ламп, сброс остаточного ресурса к начальному, возврат на предыдущее меню)
    {
        //lcd.cls();
        Level = 2;                              //вход в один из параметров
        NMenuL2 = NMenuL2 + rotateGeneral;             //переход на выбранную строку меню
        wheel.reset();
        rotateGeneral = 0;
        countB = 0;                             //сброс количества нажатий на кнопку, в программе
    }
      

       // else if (button and (Level == 2))      //Запасной уровень
    //{
        //  Level = 3;
        // wheel.reset();
    //}
    //printf("Уровень: %i\n", Level);
}


void On_OffSystem()                  //Включение/выключение установки
{
    if  (rotateGeneral == 10)
        {
            OnOffint = 1;                           ////////УДАЛИТЬ, для теста    
            OnOff = true;
            CountOnOff++;
        }
        else if (rotateGeneral == 11)
        {
            OnOff = false;
            //OnOffint = 0;                          ////////УДАЛИТЬ, для теста    
        }
        else if ((button) and (rotateGeneral == 12))
        {
            //OnOffint = 0;                          ////////УДАЛИТЬ, для теста 
            OnOff = false;
            CountOnOff=0; 
        }
}

//////////////////////////////////////////////////////////////////////////////
void LevelOptions(){                         //уровень с настройками
    
    if (Level == 9){

        //задание общей уставки для всех ламп
        lcd.cls();


        lcd.locate(1, 0); 
        lcd.printf("Set Point: %i\n", SetPointGeneralResurs);
        lcd.locate(1, 1); 
        lcd.printf("K_lamp: %i\n", KoefOnOffRes);
         


        if (a1)
        {
            lcd.locate(0, 0); 
            lcd.printf("-"); 
        } else if (a2)
        {
            lcd.locate(0, 1); 
            lcd.printf("-");
        } else if (a1 == 0 && a2 == 0)
        {
            lcd.locate(0, rotateGeneral); 
            lcd.printf(">");            
        }

        if (a1 == 0 && a2 == 0 && rotateGeneral > 2)
        {
            wheel.reset();                          //Сброс количества оборотов энкодера
            rotateGeneral = 0;                             //Обнуление оборотов переменной во всей программе
        }
        


        if (rotateGeneral == 0 && ButtonClick == 1)
        {
            a1 = 1;                        
        }
            if (a1 == 1)                                 //жОйский костыль :/
            {
                SetPointGeneralResurs = rotateGeneral;                         
            }
                if (a1 == 1 && ButtonClick == 0)
                {
                    b1 = 1;
                }
                    if (b1 == 1 && ButtonClick == 1)
                    {
                        a1 = 0;
                        b1 = 0;
                    }

        if (rotateGeneral == 1 && ButtonClick == 1)
        {
            a2 = 1;            
        }
            if (a2 == 1)                                 //жОйский костыль :/
            {
                KoefOnOffRes = rotateGeneral;                         
            }
                if (a2 == 1 && ButtonClick == 0)
                {
                    b2 = 1;
                }
                    if (b2 == 1 && ButtonClick == 1)
                    {
                        a2 = 0;
                        b2 = 0;
                    }
    
        if (ButtonSaveChange)
        {                
            for (size_t i = 0; i < 23; i++)
            {
                LampNumber[i].LampResurs_Set = SetPointGeneralResurs;
                LampNumber[i].LampResurs_St = SetPointGeneralResurs;
                LampNumber[i].LampResusr_OSt = SetPointGeneralResurs;
            }
            Level = 0;
            a1 = 0;
            b1 = 0;
            a2 = 0;
            b2 = 0;
            lcd.cls();
                   
        }
    } 
 }

//////////////////////////////////////////////////////////////////////////////

void Level0()                     //Начальный экран (отображение общего начального ресурса установки, общего оставшегося ресурса)
    {
        if (Level==0)
            {           
                /*if (alarm11 == 1)
                {
                    lcd.locate(15,0);
                    lcd.printf("!");           
                }
                else
                {
                    lcd.locate(15,0);
                    lcd.printf(" ");
                } */            
                    
                lcd.printf("SP_Res:\n");
                lcd.locate(10, 0);                          
                lcd.printf("%ih\n", SetPointGeneralResurs);           

                lcd.printf("================\n");
                //lcd.printf("W_Res:\n");                
                //lcd.locate(10, 1);                          
                //lcd.printf("%ih\n", LampNumber[0].LampResusr_OSt);

               
               

            }
    }

void Level1()                     //перемещение в 1 уровне. выбор изменяемого параметра и значние оставшегося ресурса.
    {
        if (Level==1)
            {
                if (ChangeRotate)
                {
                    lcd.cls();
                }
                NMenuL1 = NMenuL2 + rotateGeneral;                         //Переменная для выбора параметра 
                
                if (NMenuL1<=COUNTLAMP)
                {
                    lcd.locate(1, 0);                           //Расположение надписи следующей строки на дисплее (столбец, строка)
                    lcd.printf("Lamp %i\n",NMenuL1+1);                     //Надпись Lamp 1 и значение остатка ресурса для 1 лампы на 1 строке со второго символа
                    lcd.locate(12, 0);                          //значение ресурса лампы (отводится 5 символов) на первойстроке с 12 символа
                    lcd.printf("%i\n", LampNumber[NMenuL1].LampResusr_OSt);
                     if (NMenuL1<=(COUNTLAMP-1))
                     {
                        lcd.locate(1, 1);                           //Расположение надписи следующей строки на дисплее (столбец, строка)
                        lcd.printf("Lamp %i\n",NMenuL1+2);                     //Надпись Lamp 2 и значение остатка ресурса для 2 лампы на 2 строке со второго символа
                        lcd.locate(12, 1);                          //значение ресурса лампы (отводится 5 символов) на первойстроке с 12 символа
                        lcd.printf("%i\n", LampNumber[NMenuL1+1].LampResusr_OSt);
                    }
                    Level1Back = false;
                    LevelOpt = false;
                }
                else if (NMenuL1>=COUNTLAMP)
                {       
                        lcd.locate(1, 0);                              
                        lcd.printf("Option\n");
                       // lcd.locate(1, 2);                              //Возврат на уровень выше (на уровень 0)
                       // lcd.printf("Back\n");
                       // Level1Back = true;
                       LevelOpt = true;                             //Условие возврата, а не перехода на уровень ниже
                        wheel.setPulses(COUNTLAMP+1);
                          if (ButtonClick)                                    //по нажатию на кнопку происходит переход на уровень выше (0), сброс оборотов энкодера и очистка экрана
                            {   
                                NMenuL2 = 0;
                                Level = 9;
                                wheel.reset();
                                lcd.cls();
                            }
                }   
            }   
    }

void Level2()                      //перемещение во 2 уровне
    {
        if (Level == 2)
        {
            lcd.cls(); 
            
            if (ChangeRotate) 
            { 
                lcd.cls();                         //при повороте энкодера происходит очистка дисплея
                
            }
            
            printf("NMenuL2 %i\n", NMenuL2);        //перемещение по меню уровня два (переменная для возврата на туже строку второго меню)

                               (countB==0) ? NMenuL2_1 = CiclZnach(2, 0, rotateGeneral): NMenuL2_1 = NMenuL2_1;    //используем счетчик нажатий для активации параметров на уровне 2
                                
                                switch (NMenuL2_1)                                                          //перемещение в подменю второго уровня (параметры настройки)    
                                    {
                                    case 0: 
                                                    if (countB == 0)
                                                        {
                                                        lcd.locate(1, 0);
                                                        lcd.printf("Lamp:resurs %i\n",LampNumber[NMenuL2].LampResurs_St);                   //Начальное значение ресурса лампы 1
                                                        lcd.locate(1, 1);
                                                        lcd.printf("Lamp:reset\n");                                    //Сброс вычитаемого ресурса (значение остаточного ресурса вновь начнется с начального значения)
                                                        }
                                                    if (countB == 1)
                                                        {

                                                        ActMenuL2 = CiclZnach(1, 0, rotateGeneral);
                                                        printf("ActMenuL2 %i\n", ActMenuL2);
                                                            if (ActMenuL2==0)
                                                                {
                                                                    lcd.locate(1, 0);
                                                                    lcd.printf("Lamp:r_st  %i\n", LampNumber[NMenuL2].LampResurs_St);   //
                                                                    lcd.locate(1, 1);
                                                                    lcd.printf("Lamp:back\n");                                           
                                                                }
                                                                    else 
                                                                    {
                                                                        lcd.locate(1, 0);
                                                                        lcd.printf("Lamp:back\n");
                                                                        lcd.locate(1, 1);
                                                                        lcd.printf("Lamp:r_st  %i\n", LampNumber[NMenuL2].LampResurs_St);
                                                                    }
                                                        }
                                                        else if (countB == 2)                                              //Подуровень изменяемого значения ресурса
                                                            {
                                                                ActMenuL2 = ActMenuL2;
                                                                printf("ActMenuL2_2 %i\n", ActMenuL2);
                                                                if (ActMenuL2==0)
                                                                {
                                                                    LampNumber[NMenuL2].LampResurs_Set = rotateGeneral;                                 //вводимое значение устанавливается поворотом энкодера
                                                                    lcd.locate(1, 0);
                                                                    lcd.printf("Lamp:r_set_  %i\n", LampNumber[NMenuL2].LampResurs_Set);            //отображение заданного значения ресурса лампы

                                                                }
                                                                    else 
                                                                    {
                                                                        countB = 0;                                         //после ввода значения счетчик нажатий сбрасывается в 0 и обороты энкодера также сбрасываются, 
                                                                        wheel.reset();                                      //происходит возврад в подменю настройки лампы
                                                                    }
                                                                
                                                            }
                                                        else if (countB == 3)                                               //Подтверждение введеного значения ресурса лампы
                                                            {
                                                                if (LampNumber[NMenuL2].LampResurs_St!=LampNumber[NMenuL2].LampResurs_Set)                               //Если введеное значение отличается от того, что было
                                                                    {                                                        //то переменной присваивается новое значение, иначе ничего
                                                                    LampNumber[NMenuL2].LampResurs_St=LampNumber[NMenuL2].LampResurs_Set;                                //не изменится, просто пройдет возврат к предыдущему подменю
                                                                    LampNumber[NMenuL2].LampResusr_OSt=LampNumber[NMenuL2].LampResurs_St;
                                                                    countB = 0;
                                                                    wheel.reset();
                                                                    }
                                                                    else {
                                                                        countB = 0;
                                                                        wheel.reset();
                                                                    }
                                                                if (backL2_1 == 1)                                            //возврат к предыдущему подменю
                                                                    {
                                                                     countB = 0; 
                                                                     backL2_1 = 0;   
                                                                    }
                                                            }    

                            
                                        break;
                                    case 1: lcd.locate(1, 0);
                                            lcd.printf("Lamp: reset\n");
                                            lcd.locate(1, 1);
                                            lcd.printf("Lamp: back\n");
                                                if (ButtonClick) 
                                                    {
                                                        Level = 1;
                                                        LampNumber[NMenuL2].LampResusr_OSt=LampNumber[NMenuL2].LampResurs_St;
                                                        wheel.reset();
                                                        lcd.cls();
                                                    }
                                        break;
                                    case 2: lcd.locate(1, 0);
                                            lcd.printf("Lamp: back\n");
                                            lcd.locate(1, 1);
                                            lcd.printf("Lamp: resurs %i\n", LampNumber[NMenuL2].LampResurs_St);
                                                if (ButtonClick) 
                                                    {
                                                        Level = 1;
                                                        //NMenuL1 = NMenuL2;
                                                        wheel.reset();
                                                        lcd.cls();
                                                    }
                                        break;

                                    default:
                                            
                                        break;
                                }
    
        }   
    }






