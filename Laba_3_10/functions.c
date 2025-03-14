#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "header.h"

//* ======================== Memory ===========================
Stack* createNode(char new_data) 
{
    Stack* new_node = (Stack*)malloc(sizeof(Stack));
    new_node->data = new_data;
    new_node->next = NULL;
    return new_node;
}

//* ======================== STACK ========================

int isEmpty(Stack* stack)
{
    return stack == NULL;
}

void push(Stack* stack, char new_data)
{
	Stack* new_node = createNode(new_data);
	if (!new_node)
	{
		printf("\n Ошибка выделения памяти\n");
        return;
	}
	
	new_node->next = stack;
	stack = new_node;
}

void pop(Stack* stack)
{
	if (isEmpty(stack)) 
	{
        printf("\n Стек уже пуст\n");
        return;
    }
    else 
	{
        Stack* temp = stack;
        stack = stack->next;
        free(temp);
    }
}

int peek(Stack* stack)
{
	return (!isEmpty(stack)) ? stack->data : NULL;
}

//* ======================== Checks ============================
int is_valid(const char* str)                     	// функция проверки на бытиё числом
{
    while (*str)                            		// цикл по элементам строки
    {
        if (!isdigit(*str) && *str != '.' && *str != ',' && *str != '+' && *str != '-' && *str != '*' && *str != '/') return 0;
        str++;
    }
    return 1;
}

//* ======================== MATH ========================
void check_brackets(Stack* stack, char* destination)
{
	int error_flag = 0;
	for (int i = 0; i < strlen(destination) && !error_flag, i++)
	{
		switch (destination[i])
		{
		case '(' || '[' || '{':
			push(stack, destination[i]);
			break;
		case ')':
			if (peek(stack) == '(')
			{
				pop(stack);
			}
			else
			{
				printf("Syntax mistake on the %d character!", i);
				error_flag = 1;
			}
			break;
		case ']':
			if (peek(stack) == '[')
			{
				pop(stack);
			}
			else
			{
				printf("Syntax mistake on the %d character!", i);
				error_flag = 1;
			}
			break;
		case '}':
			if (peek(stack) == '{')
			{
				pop(stack);
			}
			else
			{
				printf("Syntax mistake on the %d character!", i);
				error_flag = 1;
			}
			break;
		}
	}
	if (isEmpty(stack))
	{
		printf("All good!");
	}
	
}
//TODO function to check ()[]{}
//TODO IMAGINATION-3000
//* ======================== INPUT ========================
void input_int_var(int* a, int t, int min, int max)							// функция ввода и проверки целых чисел
{
	int check = 0;
	int c;
	while (check != 1)	                                					// цикл ввода и проверки
	{
		switch (t)															// вывод сообщений с описанием вводимой величины
		{
		case 1:
			printf("Введите количество студентов(целое положительное десятичное число): \n");
			break;
		case 2:
			printf("Выберите тип вводимой структуры: ФИО студента или ФИО студента и иформация о семье (1-2): \n");
			break;
		case 3:
			printf("Выберите состав семьи:\n\t1. отец, мать, брат\n\t2. отец, мать, сестра, брат\n\t3. отец, мать, сестра\n (1-3): \n");
			break;
		}
		check = scanf("%d", a);								   				// ввод значения
		if (check != 1)														// проверка правильности типа введённого значения
			printf("\033[1;31m Ошибка ввода: не целое десятичное число, попробуйте ввести еще раз.\033[0m\n");  // вывод сообщения об ошибке
		else if (*a <= 0)
		{
			printf("\033[1;31m Ошибка ввода: не положительное число, попробуйте ввести еще раз.\033[0m\n");		// вывод сообщения об ошибке
			check--;
		}
		else if (*a >= 1000)
		{
			printf("\033[1;31m Ошибка ввода: Слишком большое число, попробуйте ввести еще раз.\033[0m\n");		// вывод сообщения об ошибке
			check--;
		}
		if (min != 0 && max != 0) 
		{
			if (*a < min || *a > max) 
			{
				printf("\033[1;31m Ошибка ввода: Число не в пределе 1-12, попробуйте ввести еще раз.\033[0m\n");// вывод сообщения об ошибке
				check--;
			}
		}
		while ((c = getchar()) != '\n' && c != EOF);						// очистка буфера ввода
	}
}

void input_str(char* destination)										// функция ввода названия фирмы
{
	char buffer[MAX_NAME_SIZE];											// буфер для ввода строки
	char c;																// переменная для очистки буфера ввода
		int valid = 0;													// переменная для проверки ввода
		while (!valid) 													// цикл ввода и проверки
		{
			if (fgets(buffer, sizeof(buffer), stdin) == NULL) 			// ввод строки
			{
				printf("\033[1;31m Ошибка ввода. Пожалуйста, попробуйте ещё раз.\033[0m\n");
				continue;
			}

			buffer[strcspn(buffer, "\n")] = '\0';						// Удаление символа новой строки, если он есть

			int char_count = 0;											// счётчик длины UTF-8 строки
			for (char* p = buffer; *p != '\0';) 						// Вычисление количества UTF-8 символов
			{
				if ((*p & 0x80) == 0)
					p += 1; // 1-байтовый символ (ASCII)
				else if ((*p & 0xE0) == 0xC0)
					p += 2; // 2-байтовый символ
				else if ((*p & 0xF0) == 0xE0)
					p += 3; // 3-байтовый символ
				else if ((*p & 0xF8) == 0xF0)
					p += 4; // 4-байтовый символ
				else 
				{
					printf("\033[1;31m Неверная кодировка символов. Пожалуйста, используйте UTF-8.\033[0m\n");
					char_count = -1;
					break;
				}
				char_count++;
			}

		if (char_count == -1)											// проверка наневерную кодировку
				continue;
			
			int buffer_len = strlen(buffer); 							// получение длины строки	

			if (char_count == 0) 										// проверка на пустую строку
			{
				printf("\033[1;31m Название не может быть пустым. Пожалуйста, попробуйте ещё раз.\033[0m\n");
				continue;
			}

			if (!is_valid(buffer))                                                 // проверка на бытиё чистом
			{
                printf("\033[1;31m Введено не числовое значение. Пожалуйста, повторите ввод.\033[0m\n");
                continue;
			}

			if (char_count > MAX_NAME_SIZE/2-1)							// проверка на слишком большую строку
			{   
				printf("\033[1;31m Название слишком длинное. Пожалуйста, введите название не более 30 символов.\033[0m\n");
				continue;
			}
			else if (buffer_len >= MAX_NAME_SIZE)
				while ((c = getchar()) != '\n' && c != EOF);			// очистка буфера ввода			
			strncpy(destination, buffer, MAX_NAME_SIZE);				// копирование буффера в поле структуры
			valid = 1; 													// Ввод корректен, выход из цикла
		}
}

//* ======================== OUTPUT ========================
void display_stack(Stack* stack)
{
	Stack* temp = stack;
	printf("Stack:\n\t");
	while(temp->next)
	{
		printf("%4d", temp->data);
	}
}
//* ======================== Restart ========================
void restart_program(int* flag)																				    // функция перезапуска программы
{
	printf("\nДля завершения работы программы введите \033[1;32m0\033[0m, иначе перезапуск программы.\n");	    // запрос на перезапуск программы 
	scanf("%d", flag);																							// ввод значения флага цикла программы
	if (*flag != 0)																								// проверка значения флага
		printf("Перезапуск программы...\n");																	// вывод сообщения о перезапуске программы
	else
		printf("Завершение работы...\n");																		// вывод сообщения о завершении работы программы
}