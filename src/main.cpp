#include "Set.h"






//DigitalOut led(LED3);
int main() 
{   
    t.start();                                          //включение библиотеки системного времени (библиотека Times)////tthffgt

    //lcd.printf("Obsh T: %i\n", Sec);
    //lcd.printf("Work T: %i\n", Min);
    //lcd.setCursor(lcd.CurOn_BlkOn); ///Курсор
    
    

    
    while(1)
    {    
     //LampInit();  ??? из-за этого висло
     printf("DispLightTime: %i\n", DispLightTime);      
     printf("Button: %s\n", button);
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
   // Alarm (200);
    Button();


   


    
    }
}