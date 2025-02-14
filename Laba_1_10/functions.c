#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
		while ((c = getchar()) != '\n' && c != EOF);																	// очистка буфера ввода																			
	}
}

firm_info* firm_info_alloc(int number_of_firms)
{
	firm_info* list_of_firms = (firm_info*)calloc(number_of_firms, sizeof(firm_info));
	for (int i = 0; i < number_of_firms; i++) 
	{
		list_of_firms[i].name = (char*)calloc(31, sizeof(char));
		list_of_firms[i].taxes = (char*)calloc(7, sizeof(char));
		list_of_firms[i].taxes_deadline = (char*)calloc(10, sizeof(char));
		list_of_firms[i].taxes_payed = (char*)calloc(10, sizeof(char));
	}
	return list_of_firms;
}

void input_firm_name(firm_info* list_of_firms, int number_of_firms)
{
	
	char* name = (char*)calloc(31, sizeof(char));
	
	for (int i = 0; i < number_of_firms; i++)
	{
		printf("Введите название %d фирмы: ", i + 1);
		if (fgets(name, 31, stdin) == NULL)
		{
			printf("Ошибка ввода строки: введите строку короче %d символов!\n", 30);
			i--;
		}
		else
		{
			name[strlen(name) - 1] = '\0'; // Удаление символа новой строки
			strcpy(list_of_firms[i].name, name);
		}
	}

}

void input_firm_taxes(firm_info* list_of_firms, int number_of_firms)
{
	int c;
	char taxes[8];
	
	for (int i = 0; i < number_of_firms; i++)
	{
		printf("Введите величину налогоображения фирмы %s: ", list_of_firms[i].name);
		if (fgets(taxes, 7, stdin) == NULL)
		{
			printf("Ошибка ввода строки: введите строку короче %d символов!\n", 30);
			i--;
		}
		else
		{
			taxes[strlen(taxes) - 1] = '\0'; // Удаление символа новой строки
			strcpy(list_of_firms[i].taxes, taxes);
		}
	}
}

void input_firm_dates(firm_info* list_of_firms, int number_of_firms)
{
	int c;
	char* date = (char*)calloc(11, sizeof(char));
	
	for (int i = 0; i < number_of_firms; i++)
	{
		printf("Введите дату последнего срока внесения налога фирмы %s: ", list_of_firms[i].name);
		if (fgets(date, 10, stdin) == NULL)
		{
			printf("Ошибка ввода строки: введите строку короче %d символов!\n", 10);
			i--;
		}
		else
		{
			date[strlen(date) - 1] = '\0'; // Удаление символа новой строки
			strcpy(list_of_firms[i].taxes_deadline, date);
		}

		printf("Введите дату фактического внесения налога фирмы %s: ", list_of_firms[i].name);
		if (fgets(date, 10, stdin) == NULL)
		{
			printf("Ошибка ввода строки: введите строку короче %d символов!\n", 10);
			i--;
		}
		else
		{
			date[strlen(date) - 1] = '\0'; // Удаление символа новой строки
			strcpy(list_of_firms[i].taxes_payed, date);
		}
	}

}

void output_firm_info(firm_info* list_of_firms, int number_of_firms, int t)
{
	printf("Данные о фирмах:");
	printf("---------------------------------------------------------------------------\n");
	printf("| № |          Название фирмы        | налоги  |   дедлайн   | дата оплат |\n");
	printf("---------------------------------------------------------------------------\n");
	for (int i = 0; i < number_of_firms; i++) 
	{
		if (list_of_firms[i].taxes == NULL) 
		{
			printf("| %d | %s |  NULL   |    NULL     |    NULL    |\n", i, list_of_firms[i].name);
			printf("---------------------------------------------------------------------------\n");
		}
		else if (list_of_firms[i].taxes_deadline == NULL) 
		{
			printf("| %d | %s | %s |    NULL     |    NULL    |\n", i, list_of_firms[i].name, list_of_firms[i].taxes);
			printf("---------------------------------------------------------------------------\n");
		}
		else if (list_of_firms[i].taxes_payed == NULL) 
		{
			printf("| %d | %s | %s | %s |    NULL    |\n", i, list_of_firms[i].name, list_of_firms[i].taxes, list_of_firms[i].taxes_deadline);
			printf("---------------------------------------------------------------------------\n");
		}
		else
		{
			printf("| %d | %s | %s | %s | %s |\n", i, list_of_firms[i].name, list_of_firms[i].taxes, list_of_firms[i].taxes_deadline, list_of_firms[i].taxes_payed);
			printf("---------------------------------------------------------------------------\n");
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