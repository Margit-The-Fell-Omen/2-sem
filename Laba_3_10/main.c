#include "header.h"
#include <stdio.h>

int main()
{							                                                            // выбор состава семьи
    char str[MAX_SIZE];                                                                                                      // выбор функци вывода
    int flag = 1;											                                                        // флаг главного цикла программы
    Stack* stack = NULL;
    Context ctx;
    ctx.varList = NULL;
    while (flag)                                                                                                    // главный цикл программы
    {
        input_str(str);
        check_brackets(stack, str);
        do_math(stack, str, &ctx);
        restart_program(&flag);								                                                        // функция перезапуска программы
    }
    return 0;
}