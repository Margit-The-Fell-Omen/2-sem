#include <stdio.h>  // Для printf, fgets, sscanf и др. (хотя часть может быть в header.h)
#include <string.h> // Для strchr (если используется для очистки буфера)
#include "header.h" // Ваш заголовочный файл

// Имя файла для сохранения и загрузки данных
#define FILENAME "firms_data.txt"

int main() 
{
    int number_of_firms = 0;        // количество фирм
    int flag = 1;                   // флаг главного цикла программы
    firm_info* list_of_firms = NULL; // массив структур фирм

    printf("Добро пожаловать в программу учета налогов фирм!\n");

    while (flag) {
        // Вызываем функцию menu, передавая адреса list_of_firms и number_of_firms
        menu(&list_of_firms, &number_of_firms);

        // Вызываем функцию перезапуска/завершения программы
        restart_program(&flag);
    }

    // Финальное освобождение памяти, если программа завершается 
    // и память была выделена в последнем вызове menu
    if (list_of_firms != NULL) {
        firm_info_free(list_of_firms);
        list_of_firms = NULL;
    }

    printf("Программа завершена.\n");
    return 0;
}