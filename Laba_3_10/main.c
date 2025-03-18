#include "header.h"
#include <stdio.h>

int main()
{							                                                            // выбор состава семьи
    char str[MAX_SIZE];                                                                                                      // выбор функци вывода
    int flag = 1;											                                                        // флаг главного цикла программы
    Stack* stack = NULL;
    int expression_choise;
    while (flag)                                                                                                    // главный цикл программы
    {
        input_int_var(& expression_choise, 1, 1, 2);
        input_str(str);
        switch (expression_choise)
        {
            case 1:
            check_brackets(stack, str);
            break;

            case 2:
            do_math(str);
            break;
        }
        restart_program(&flag);								                                                        // функция перезапуска программы
    }
    return 0;
}