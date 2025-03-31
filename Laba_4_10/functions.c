#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"

//* ======================== QUEUE ===========================

Queue* create_queue()
{
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    queue->front = queue->rear = NULL;
    return queue;
}

int isEmpty(Queue* queue)
{
    return (queue->front == NULL);
}

void enqueue(Queue* queue, int field, int data)
{
    q_node* new_node = (q_node*)malloc(sizeof(q_node));
    switch (field) 
    {
        case 1:
        new_node->clinic_number = data;
        break;
        case 2:
        new_node->total_beds = data;
        break;
        case 3:
        new_node->free_beds = data;
        break;
    }
    new_node->next = NULL;
    if (isEmpty(queue))
    {
        queue->front = queue->rear = new_node;
    }
    else
    {
        queue->rear->next = new_node;
        queue->rear = new_node;
    }
}

q_node* dequeue(Queue* queue)
{
    if (isEmpty(queue)) 
    {
        fprintf(stderr, "Queue underflow. Cannot dequeue from an empty queue.\n");
        return NULL;
    }

    q_node* temp = queue->front;
    queue->front = queue->front->next;

    return temp;
}

int peek(Queue* queue, int field)
{
    switch (field) 
    {
        case 1:
        return queue->front->clinic_number;
        case 2:
        return queue->front->total_beds;
        case 3:
        return queue->front->free_beds;
    }
    return -1;
}

void free_queue(Queue* queue)
{
    while (queue->front) 
    {
    free(dequeue(queue));
    }
    free(queue);
}
//* ======================== FUNCTIONAL ========================

//* ======================== INPUT ========================
void input_int_var(int* a, int t, int min, int max)							// функция ввода и проверки целых чисел
{
	int check = 0;
	int c;
	while (check != 1)	                                					// цикл ввода и проверки
	{
		printf("Выберите тип выполняемой операции:\n\t 1 - проверка синтаксиса скобок\n\t 2 - проверка синтаксиса скобок + вычисление выражения\n");
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
	char buffer[MAX_STR_SIZE];											// буфер для ввода строки
	char c;																// переменная для очистки буфера ввода
		int valid = 0;													// переменная для проверки ввода
		printf("Input string\n");
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

			if (char_count > MAX_STR_SIZE/2-1)							// проверка на слишком большую строку
			{   
				printf("\033[1;31m Название слишком длинное. Пожалуйста, введите название не более 30 символов.\033[0m\n");
				continue;
			}
			else if (buffer_len >= MAX_STR_SIZE)
				while ((c = getchar()) != '\n' && c != EOF);			// очистка буфера ввода			
			strncpy(destination, buffer, MAX_STR_SIZE);				// копирование буффера в поле структуры
			valid = 1; 													// Ввод корректен, выход из цикла
		}
}

void input_queue(Queue* queue)
{
    printf("");
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