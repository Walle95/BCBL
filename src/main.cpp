#include "Set.h"






//DigitalOut led(LED3);
int main() 
{   
    t.start();                                          //включение библиотеки системного времени (библиотека Times)////tthffgt

    lcd.setCursor(lcd.CurOn_BlkOn); ///Курсор

    
    
    while(1)
    {    
     LampInit();
     printf("DispLightTime: %i\n", DispLightTime);      
     printf("PB_0: %i\n", DigitalIn(PA_10));
     //printf("Level: %i\n", LampNumber[0].LampResurs_Set);
     printf("Sec: %i\n", Sec);


    
    


   
     
    //led=!led;    
    // LampNumber[0].LampPin=(!LampNumber[0].LampPin);
    // 
    drebezg_encoder(100000);                               // вызов классов с определенной частотой (200 мс), для избежания дребезга и последовательной обработки кода программы.
    LevelChoice (100000);
    Level0 ();
    Level1 ();
    Level2 ();
    LevelOptions();
    

    TimerObsh (900000,t);
    ResursT ();    
    On_OffSystem();
    AlarmStatusSystem();
    Button();


   


    
    }
}