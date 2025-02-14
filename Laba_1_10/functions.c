#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include "header.h"

void input_int_var(int* a, int t)						// функция ввода и проверки целых чисел
{
	int check = 0;
	while (check != 1)	                                // цикл ввода и проверки
	{
		switch (t)										// вывод сообщений с описанием вводимой величины
		{
		case 1:
			printf("Введите количество матриц(целое положительное десятичное число): \n");
			break;
		}
		check = scanf("%d", a);								                                                // ввод значения
		if (check != 1)														                                    // проверка правильности типа введённого значения
			printf("\033[1;31mОшибка ввода: не целое десятичное число, попробуйте ввести еще раз.\033[0m\n");   // вывод сообщения об ошибке
		else if (*a <= 0)
		{
			printf("\033[1;31mОшибка ввода: не положительное число, попробуйте ввести еще раз.\033[0m\n");		// вывод сообщения об ошибке
			check--;
		}
		else if (*a >= 100)
		{
			printf("\033[1;31mОшибка ввода: Слишком большое число, попробуйте ввести еще раз.\033[0m\n");		// вывод сообщения об ошибке
			check--;
		}
		rewind(stdin);																							// очистка буфера ввода
	}
}

firm_info** input_firm_name(int number_of_firms)
{
    
    firm_info** list_of_firms = memory_alloc(number_of_firms);
    wchar_t* name = (wchar_t*)calloc(30, sizeof(wchar_t));
	wchar_t* buffer = (wchar_t*)calloc(30, sizeof(wchar_t));
    for (int i = 0; i < list_of_firms; i++)
    {
        wprintf(L"Введите название %d фирмы:", i+1);
		if (fgetws(buffer, 31, stdin) == NULL)														// ввод строки и проверка
		{
			perror("Ошибка ввода строки");
			exit(EXIT_FAILURE);
		}
		if (wcslen(buffer) < 30)																			// проверка на соответствие заданному размеру строки
		{
			for (int x = 0; x < wcslen(buffer); x++)													
				*(name + x) = *(buffer + x);															// копирование введённой строки из буффера
			int len = wcslen(name);
			for (int x = 0; x < len; x++)
			{
				if (*(name + x) == '\n')
				{
					*(name + x) = '\0';
				}
			}
		}
		else
		{
			printf("Ошибка ввода строки: введите строку короче %d символов!\n", 30);						// вывод сообщения об ошибке
			rewind(stdin);																				// очистка буффера ввода
			i--;																						
		}
		
		rewind(stdin);																				// Очистка буфера ввода
		list_of_firms[i]->name = name;
    }
	free(buffer);
}

firm_info** memory_alloc(int number_of_firms)
{
    firm_info** list_of_firms = (firm_info**)calloc(number_of_firms, sizeof(firm_info*));
    for (int i = 0; i < list_of_firms; i++)
    {
        *(list_of_firms + i) = malloc(sizeof(firm_info));
    }
    return list_of_firms;
}

void restart_program(int* flag)																					// функция перезапуска программы
{
	printf("Для завершения работы программы введите \033[1;32m0\033[0m, иначе перезапуск программы.\n");	    // запрос на перезапуск программы 
	scanf_s("%d", flag);																						// ввод значения флага цикла программы
	if (flag != 0)																								// проверка значения флага
		printf("Перезапуск программы...\n");																	// вывод сообщения о перезапуске программы
	else
		printf("Завершение работы...\n");																		// вывод сообщения о завершении работы программы
}