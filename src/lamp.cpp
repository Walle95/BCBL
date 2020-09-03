///////////////////////////////////////////////////////////////////////////////////
//        Класс лампы
//        Реализация
//
//
////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
//        Класс лампы
//        Обьявление
//
//
////////////////////////////////////////////////////////////////////////////////////

#include <mbed.h>

//
class Lamp
{
    private:

    public:


         

    DigitalOut LampPin;                                //Выход лампы
    int LampResurs_St;                                 //Ресурсное время лампы    
    int LampResurs_Set;                                //Ручное выставление ресурсного вермени лампы    
    int LampResusr_Res;                                //Сброс отработанного времени на выставленное лампы  
    int LampResusr_OSt;                                //оставшееся время ресурса лампы 



   Lamp(PinName pin) : LampPin(pin)  {} //Конструктор


   bool LampInit(int OnValue,int Alarm)
    {
      if (LampResusr_OSt>0 && !Alarm)
            {
               LampPin=1;
               return true;
            }
            else
            {
               LampPin=0;
               return false;
            }
            
    }
      
    int getLampResurs_St(void)
    {
       return LampResurs_St;
    }

    int setLampResurs_St(int Val_St)
    {
       LampResurs_St=Val_St;
       return LampResurs_St;
    }

    int getLampResurs_Set(void)
    {
       return LampResurs_Set;
    }
    int setLampResurs_Set(int Val_Set)
    {
       LampResurs_Set=Val_Set;
       return LampResurs_Set;
    }

    int getLampResurs_Res(void)
    {
       return LampResusr_Res;
    }
    int setLampResurs_Res(int Val_Res)
    {
       LampResusr_Res=Val_Res;
       return LampResusr_Res;
    }

    int getLampResurs_OSt(void)
    {
       return LampResusr_OSt;
    }
    int setLampResurs_OSt(int Val_OSt)
    {
       LampResusr_OSt=Val_OSt;
       return LampResusr_OSt;
    }

};

