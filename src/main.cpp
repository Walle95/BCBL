#include "Set.h"





DigitalOut led(LED3);
int main() 
{   
    t.start();                                          //включение библиотеки системного времени (библиотека Times)////tthffgt
    //lcd.printf("Obsh T: %i\n", Sec);
    //lcd.printf("Work T: %i\n", Min);
    //lcd.setCursor(lcd.CurOn_BlkOn); ///Курсор
    


    
    while(1)
    {
     LampInit();
     //printf("res: %i\n", res);
     //printf("a1 %i\n", a1);
     //printf("DITest: %i\n", );
     printf("rotateGeneral: %i\n", rotateGeneral);
     //printf("Level: %i\n", LampNumber[0].LampResurs_Set);
     printf("Sec: %i\n", Sec);


    


   
     
    //led=!led;    
    // LampNumber[0].LampPin=(!LampNumber[0].LampPin);
    // 
    drebezg_encoder(200000);                               // вызов классов с определенной частотой (200 мс), для избежания дребезга и последовательной обработки кода программы.
    LevelChoice (200000);
    Level0 (200000);
    Level1 (200000);
    Level2 (200000);
    LevelOptions();
    

    TimerObsh (900000,t);
    ResursT (200000);    
    On_OffSystem(200000);
   // Alarm (200000);
    Button();


   


    
    }
}