

class Alarm
{
private:


public:
    
////////////////////////////////////////////////////////////
//Индикация аларма лампы. Возвращает True или False

  int indicatorAlarmLamp(bool ActiveAlarmLamp){        

        if (ActiveAlarmLamp)
        {
            return(1);
        }
        else
        {
            return(0);
        }
    }
};

