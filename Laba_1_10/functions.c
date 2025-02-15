#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include "header.h"

void input_int_var(int* a, int t)						// функция ввода и проверки целых чисел
{
	int check = 0;
	int c;
	while (check != 1)	                                // цикл ввода и проверки
	{
		switch (t)										// вывод сообщений с описанием вводимой величины
		{
		case 1:
			printf("Введите количество фирм(целое положительное десятичное число): \n");
			break;
		}
		check = scanf("%d", a);								                                                	// ввод значения
		if (check != 1)														                                    		// проверка правильности типа введённого значения
			printf("\033[1;31m Ошибка ввода: не целое десятичное число, попробуйте ввести еще раз.\033[0m\n");   // вывод сообщения об ошибке
		else if (*a <= 0)
		{
			printf("\033[1;31m Ошибка ввода: не положительное число, попробуйте ввести еще раз.\033[0m\n");		// вывод сообщения об ошибке
			check--;
		}
		else if (*a >= 100)
		{
			printf("\033[1;31m Ошибка ввода: Слишком большое число, попробуйте ввести еще раз.\033[0m\n");		// вывод сообщения об ошибке
			check--;
		}
		while ((c = getchar()) != '\n' && c != EOF);
	}
}

firm_info* firm_info_alloc(int number_of_firms)
{
	firm_info* list_of_firms = (firm_info*)calloc(number_of_firms, sizeof(firm_info));
	for (int i = 0; i < number_of_firms; i++) 
	{
		list_of_firms[i].name = (char*)calloc(32, sizeof(char));
		list_of_firms[i].taxes = (char*)calloc(9, sizeof(char));
		list_of_firms[i].taxes_deadline = (char*)calloc(12, sizeof(char));
		list_of_firms[i].taxes_payed = (char*)calloc(12, sizeof(char));
	}
	return list_of_firms;
}

int is_numeric(const char* str) {
    while (*str) {
        if (!isdigit(*str) && *str != '.' && *str != ',') return 0;
        str++;
    }
    return 1;
}

void firm_info_free(firm_info* list_of_firms, int number_of_firms)
{
	for (int i = 0; i < number_of_firms; i++) 
	{
		free(list_of_firms[i].name);
		list_of_firms[i].name = NULL;
		free(list_of_firms[i].taxes);
		list_of_firms[i].taxes = NULL;
		free(list_of_firms[i].taxes_deadline);
		list_of_firms[i].taxes_deadline = NULL;
		free(list_of_firms[i].taxes_payed);
		list_of_firms[i].taxes_payed = NULL;
	}
	free(list_of_firms);
	list_of_firms = NULL;
}

void input_firm_name(firm_info* list_of_firms, int number_of_firms) 
{
    char name[32];

    for (int i = 0; i < number_of_firms; i++) 
	{
        int valid = 0;
        while (!valid) 
		{
            printf("Введите название %d-й фирмы (не более 30 символов): ", i + 1);
            if (fgets(name, 32, stdin) == NULL) 
			{
                printf("\033[1;31m Ошибка ввода. Пожалуйста, попробуйте ещё раз.\033[0m\n");
                continue;
            }

			int length = strlen(name);
			
            // Проверяем, была ли введена слишком длинная строка
            if (name[length - 1] != '\n') 
			{
                // Очищаем оставшиеся символы в буфере ввода
                int c;
                while ((c = getchar()) != '\n' && c != EOF);
                printf("\033[1;31m Название слишком длинное. Пожалуйста, введите название не более 30 символов.\033[0m\n");
            } else 
			{

				if (length == 0) 
				{
					printf("\033[1;31m Название не может быть пустым. Пожалуйста, попробуйте ещё раз.\033[0m\n");
					continue;
				}

                // Удаляем символ новой строки '\n'
                name[length - 1] = '\0';

				// Заполнение оставшегося места пробелами для выравнивания
                if (length < 30) 
				{
                    for (int j = length - 1; j < 30; j++) 
					{
                        name[j] = ' ';
                    }
                    name[30] = '\0'; // Добавляем завершающий нулевой символ
                }

                strcpy(list_of_firms[i].name, name);
                valid = 1; // Ввод корректен, выходим из цикла
            }
        }
    }
}

void input_firm_taxes(firm_info* list_of_firms, int number_of_firms) {
    char taxes[9]; // Буфер для строки ввода (максимум 7 символов + '\0')
	int c;

    for (int i = 0; i < number_of_firms; i++) {
        int valid = 0;
        while (!valid) {
            printf("Введите величину налогообложения фирмы %s (не более 7 символов): ", list_of_firms[i].name);
            if (fgets(taxes, sizeof(taxes), stdin) == NULL) {
                printf("\033[1;31m Ошибка ввода. Пожалуйста, попробуйте ещё раз.\033[0m\n");
                continue;
            }

            size_t len = strlen(taxes);

            // Проверяем, была ли введена слишком длинная строка
            if (taxes[len - 1] != '\n') {
                // Очищаем остаток буфера ввода
                while ((c = getchar()) != '\n' && c != EOF);
                printf("\033[1;31m Введено слишком много символов. Пожалуйста, введите не более 7 символов.\033[0m\n");
                continue;
            }

            taxes[len - 1] = '\0'; // Удаляем символ новой строки
            len--; // Уменьшаем длину после удаления '\n'

			if (!is_numeric(taxes))
			{
                printf("\033[1;31m Введено не числовое значение. Пожалуйста, введите числовое значение.\033[0m\n");
                continue;
			}

            // Проверка на пустую строку
            if (len == 0) {
                printf("\033[1;31m Величина налогообложения не может быть пустой (введите 0, если её нет). Попробуйте ещё раз.\033[0m\n");
                continue;
            }

            // Заполнение оставшегося места пробелами для выравнивания
            if (len < 7) {
                for (size_t j = len; j < 7; j++) {
                    taxes[j] = ' ';
                }
                taxes[7] = '\0'; // Добавляем завершающий нулевой символ
            }

            strcpy(list_of_firms[i].taxes, taxes);
            valid = 1; // Ввод корректен
        }
    }
}

void input_firm_dates(firm_info* list_of_firms, int number_of_firms) {
    char date[12]; // Буфер для строки ввода (максимум 10 символов + '\0')

    for (int i = 0; i < number_of_firms; i++) {
        // Ввод даты последнего срока внесения налога
        int valid_deadline = 0;
        while (!valid_deadline) {
            printf("Введите дату последнего срока внесения налога для фирмы %s (формат ДД.ММ.ГГГГ): ", list_of_firms[i].name);
            if (fgets(date, sizeof(date), stdin) == NULL) {
                printf("\033[1;31m Ошибка ввода. Пожалуйста, попробуйте ещё раз.\033[0m\n");
                continue;
            }

            size_t len = strlen(date);

            // Проверяем, была ли введена слишком длинная строка
            if (date[len - 1] != '\n') {
                int c;
                while ((c = getchar()) != '\n' && c != EOF);
                printf("\033[1;31m Введено слишком много символов. Пожалуйста, введите дату в формате ДД.ММ.ГГГГ.\033[0m\n");
                continue;
            }

            date[len - 1] = '\0'; // Удаляем символ новой строки
            len--; // Уменьшаем длину после удаления '\n'

            // Проверка на пустую строку
            if (len == 0) {
                printf("\033[1;31m Дата не может быть пустой. Попробуйте ещё раз.\033[0m\n");
                continue;
            }

            // Заполнение оставшегося места пробелами
            if (len < 10) {
                for (size_t j = len; j < 10; j++) {
                    date[j] = ' ';
                }
                date[10] = '\0'; // Добавляем завершающий нулевой символ
            }

            strcpy(list_of_firms[i].taxes_deadline, date);
            valid_deadline = 1;
        }

        // Ввод даты фактического внесения налога
        int valid_payed = 0;
        while (!valid_payed) {
            printf("Введите дату фактического внесения налога для фирмы %s (формат ДД.ММ.ГГГГ): ", list_of_firms[i].name);
            if (fgets(date, sizeof(date), stdin) == NULL) {
                printf("\033[1;31m Ошибка ввода. Пожалуйста, попробуйте ещё раз.\033[0m\n");
                continue;
            }

            size_t len = strlen(date);

            // Проверяем, была ли введена слишком длинная строка
            if (date[len - 1] != '\n') {
                int c;
                while ((c = getchar()) != '\n' && c != EOF);
                printf("\033[1;31m Введено слишком много символов. Пожалуйста, введите дату в формате ДД.ММ.ГГГГ.\033[0m\n");
                continue;
            }

            date[len - 1] = '\0'; // Удаляем символ новой строки
            len--; // Уменьшаем длину после удаления '\n'

            // Проверка на пустую строку
            if (len == 0) {
                printf("\033[1;31m Дата не может быть пустой. Попробуйте ещё раз.\033[0m\n");
                continue;
            }

            // Заполнение оставшегося места пробелами
            if (len < 10) {
                for (size_t j = len; j < 10; j++) {
                    date[j] = ' ';
                }
                date[10] = '\0'; // Добавляем завершающий нулевой символ
            }

            strcpy(list_of_firms[i].taxes_payed, date);
            valid_payed = 1;
        }
    }
}

void output_firm_info(firm_info* list_of_firms, int number_of_firms, int t)
{
	printf("Данные о фирмах:\n");
	printf("+-------------------------------------------------------------------------+\n");
	printf("| № |          Название фирмы        | налоги  |   дедлайн   | дата оплат |\n");
	printf("+-------------------------------------------------------------------------+\n");
	for (int i = 0; i < number_of_firms; i++) 
	{
		switch (t) 
		{
			case 1:
			printf("| %d | %s |  NULL   |    NULL     |    NULL    |\n", i, list_of_firms[i].name);
			printf("+-------------------------------------------------------------------------+\n");
			break;
			case 2:
			printf("| %d | %s | %s |    NULL     |    NULL    |\n", i, list_of_firms[i].name, list_of_firms[i].taxes);
			printf("+-------------------------------------------------------------------------+\n");
			break;
			case 3:
			printf("| %d | %s | %s | %s  | %s |\n", i, list_of_firms[i].name, list_of_firms[i].taxes, list_of_firms[i].taxes_deadline, list_of_firms[i].taxes_payed);
			printf("+-------------------------------------------------------------------------+\n");
			break;
		}
	}
}

void restart_program(int* flag)																						    // функция перезапуска программы
{
	printf("\nДля завершения работы программы введите \033[1;32m0\033[0m, иначе перезапуск программы.\n");	    // запрос на перезапуск программы 
	scanf("%d", flag);																							// ввод значения флага цикла программы
	if (*flag != 0)																										// проверка значения флага
		printf("Перезапуск программы...\n");																	// вывод сообщения о перезапуске программы
	else
		printf("Завершение работы...\n");																		// вывод сообщения о завершении работы программы
}