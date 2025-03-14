#include "header.h"

int main()
{
    int number_of_students;									                                                        // количество студентов
    int struct_choise;									                                                            // выбор типа вводимых данных
    int family_choise;									                                                            // выбор состава семьи
    char str[MAX_NAME_SIZE];                                                                                                      // выбор функци вывода
    int flag = 1;											                                                        // флаг главного цикла программы
    Stack* stack = NULL;
    while (flag)                                                                                                    // главный цикл программы
    {
        input_str(str);
        check_brackets(str);
        restart_program(&flag);								                                                        // функция перезапуска программы
    }
    return 0;
}