#include "header.h"
#include <stdio.h>

int main()
{
    char str[MAX_STR_SIZE];                                                 // строка выражения
    int flag = 1;											            // флаг главного цикла программы
    int behaviour_choice;                                               // выбор выполняемых действий
    while (flag)                                                        // главный цикл программы
    {
        input_int_var(& behaviour_choice, 1, 1, 2);       // функция ввода 

        restart_program(&flag);					                // функция перезапуска программы
    }
    return 0;
}