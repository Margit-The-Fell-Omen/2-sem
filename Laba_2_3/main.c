#include "header.h"

int main()
{
    int number_of_students;									// количество студентов
    int struct_choise;									    // количество студентов
    int family_choise;									    // количество студентов
    int function_choice;
    int flag = 1;											// флаг главного цикла программы

    while (flag)                                            // главный цикл программы
    {
        input_int_var(&number_of_students, 1, 0, 0);            // функция ввода целого числа
        input_int_var(&struct_choise, 2, 1, 2);                 // функция ввода целого числа
        if (struct_choise == 1) 
        {
            Full_name* students = students_alloc(number_of_students);
            input_students_names((Full_name*)students, number_of_students);
            output_student_full_name(students, number_of_students);
            students_free(students);						                    // функция освобождения памяти структуры
        }
        else if (struct_choise == 2) 
        {
            input_int_var(&family_choise, 3, 1, 3);                 // функция ввода целого числа
            Student_with_family* students = students_with_families_alloc(number_of_students);
            function_choice = input_students_with_families_names(students, number_of_students, family_choise);
            output_student_with_family(students, number_of_students, family_choise, function_choice);
            students_with_families_free(students);
        }

        restart_program(&flag);								               // функция перезапуска программы
    }
    return 0;
}
