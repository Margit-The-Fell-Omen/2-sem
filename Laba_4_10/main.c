#include "header.h"

int main()
{
    
    int flag = 1;											            // флаг главного цикла программы
    int behaviour_choice;                                               // выбор выполняемых действий
    while (flag)                                                        // главный цикл программы
    {

        restart_program(&flag);					                // функция перезапуска программы
    }
    return 0;
}