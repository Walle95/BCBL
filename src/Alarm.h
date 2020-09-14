//Обработка аварий

class Alarm
{
private:


public:
    
////////////////////////////////////////////////////////////
//Метод indicatorAlarmLamp обрабатывает условия формирования аварии по ресурсу ламп. Возвращает True или False.
//ActiveAlarmLamp - условие работы лампы (не знаю, понадобиться или нет, поэтому пока просто кидаем true)
//TimeWorkLamp - оставшееся время работы лампы
//WarningBorderLamp - аварийная граница работы лампы.

  int indicatorAlarmLamp(bool ActiveAlarmLamp, int TimeWorkLamp, int WarningBorderLamp){        

        if (ActiveAlarmLamp && (TimeWorkLamp <= WarningBorderLamp))
        {
            return(1);
        }
        else
        {
            return(0);
        }
    }
};

