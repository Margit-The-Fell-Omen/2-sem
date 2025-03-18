#include "header.h"
#include <stdio.h>

int main()
{
    char str[MAX_SIZE];                                                 // строка выражения
    int flag = 1;											            // флаг главного цикла программы
    Stack* stack = NULL;                                                // стек
    int behaviour_choise;                                              // выбор выполняемых действий
    while (flag)                                                        // главный цикл программы
    {
        input_int_var(& behaviour_choise, 1, 1, 2);      // функция ввода 
        input_str(str);                                    // функция ыыода строки
        switch (behaviour_choise)
        {
            case 1:
            check_brackets(stack, str);                     // функция проверки синтаксиса скобок
            break;

            case 2:
            do_math(str);                                               // функция вычисления линейного уравнения
            break;
        }
        restart_program(&flag);					                // функция перезапуска программы
    }
    return 0;
}