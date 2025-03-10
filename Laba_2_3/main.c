#include "header.h"

int main()
{
    int number_of_students;									                                                        // количество студентов
    int struct_choise;									                                                            // выбор типа вводимых данных
    int family_choise;									                                                            // выбор состава семьи
    int function_choice;                                                                                            // выбор функци вывода
    int flag = 1;											                                                        // флаг главного цикла программы

    while (flag)                                                                                                    // главный цикл программы
    {
        input_int_var(&number_of_students, 1, 0, 0);                                                                // функция ввода целого числа
        input_int_var(&struct_choise, 2, 1, 2);                                                                     // функция ввода целого числа
        if (struct_choise == 1)                                         
        {
            Full_name* students = students_alloc(number_of_students);                                               // функция выделения памяти для структуры
            input_students_names((Full_name*)students, number_of_students);                                         // функция ввода ФИО студента без семьи
            output_student_full_name(students, number_of_students);                                                 // функция вывода ФИО студентов без семей
            students_free(students);						                                                        // функция освобождения памяти структуры
        }
        else if (struct_choise == 2) 
        {
            input_int_var(&family_choise, 3, 1, 3);                                                                 // выбор состава семьи
            Student_with_family* students = students_with_families_alloc(number_of_students);                       // функция выделения памяти для структуры
            function_choice = input_students_with_families_names(students, number_of_students, family_choise);      // функция ввода ФИО студентов с семьёй
            output_student_with_family(students, number_of_students, family_choise, function_choice);               // функция вызова выбранной функции вывода
            students_with_families_free(students);                                                                  // функция освобождения памяти структуры
        }

        restart_program(&flag);								                                                        // функция перезапуска программы
    }
    return 0;
}