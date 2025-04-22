#include <ctype.h>
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



void add_patient(Queue* queue)
{
	int hospital_num = 0;
	int h_num_flag = 1;
	int surname_flag = 1;
	char patient_surname[MAX_STR_SIZE];
	Queue* temp = queue;
	while (h_num_flag)
	{
		printf("Введите номер принимающей больницы:");
		input_int_var(&hospital_num, 1, 100);
		while (temp->front != temp->rear)
		{
			if (temp->front->clinic_number == hospital_num)
			{
				h_num_flag = 0;
				break;
			}
			temp->front = temp->front->next;
		}

		if (h_num_flag)
		{
			printf("В городе нет больницы с таким номером, повторите ввод.");
		}

		if (temp->front->free_beds == 0)
		{
			printf("В этой больнице нет свободных мест, повторите ввод.");
		}
	}

	int patients_num = temp->front->total_beds - temp->front->free_beds;
	while (surname_flag)
	{
		printf("Введите фамилию пациента");
		input_str(patient_surname, 0);

		for (int i = 0; i < patients_num; i++)
		{
			surname_flag = 0;
			if (*(temp->front->patients + i) == patient_surname)
			{
				printf("Пациент с такой фамилией уже находится в данной больнице, повторите ввод.");
				surname_flag = 1;
				break;
			}
		}
	}

	temp->front->free_beds--;
	temp->front->patients = realloc(temp->front->patients, ++patients_num);
	*(temp->front->patients + patients_num - 1) = patient_surname;
	printf("Пациент успешно добавлен.");
}

void delete_patients(Queue* queue)
{
	int hospital_num = 0;
	int h_num_flag = 1;
	int surname_flag = 1;
	char patient_surname[MAX_STR_SIZE];
	Queue* temp = queue;
	while (h_num_flag)
	{
		printf("Введите номер выписывающей больницы:");
		input_int_var(&hospital_num, 1, 100);
		while (temp->front != temp->rear)
		{
			if (temp->front->clinic_number == hospital_num)
			{
				h_num_flag = 0;
				break;
			}
			temp->front = temp->front->next;
		}

		if (h_num_flag)
		{
			printf("В городе нет больницы с таким номером, повторите ввод.");
		}

		if (temp->front->free_beds == temp->front->total_beds)
		{
			printf("В этой больнице нет больных, повторите ввод.");
		}
	}

	int patients_num = temp->front->total_beds - temp->front->free_beds;
	while (surname_flag)
	{
		printf("Введите фамилию пациента");
		input_str(patient_surname, 0);

		for (int i = 0; i < patients_num; i++)
		{
			surname_flag = 0;
			if (*(temp->front->patients + i) == patient_surname)
			{
				surname_flag = 0;
				break;
			}
		}
		if (surname_flag)
		{
			printf("Пациента с такой фамилией нет в данной больнице, повторите ввод.");
		}
	}

	temp->front->free_beds++;
	temp->front->patients = realloc(temp->front->patients, --patients_num);
	printf("Пациент %s успешно выписан.", patient_surname);
}



//* ======================== INPUT ========================
void input_int_var(int* a, int min, int max)							// функция ввода и проверки целых чисел
{
	int check = 0;
	int c;
	while (check != 1)	                                					// цикл ввода и проверки
	{
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

void input_str(char* destination, int check_num)										// функция ввода названия фирмы
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

			if (validateLatLon(buffer))
			{
				printf("\033[1;31m Неправильный формат координат. Пожалуйста, попробуйте ещё раз.\033[0m\n");
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

// Функция для проверки корректности ввода координат в виде широты и долготы.
// Ожидаемый формат: "<широта>[пробелы]","[пробелы]<долгота>"
// Возвращает 1, если формат корректен и значения координат в допустимом диапазоне, иначе — 0.
int validateLatLon(const char *input) 
{
    const char *p = input;

    // Пропускаем ведущие пробелы
    while (*p && isspace((unsigned char)*p)) 
	{
        p++;
    }
    if (*p == '\0') 
	{
        // Строка пуста, после удаления пробелов не осталось символов.
        return 0;
    }
    
    char *endPtr;
    // Парсинг широты
    double lat = strtod(p, &endPtr);
    if (p == endPtr) 
	{
        // Широта не распознана
        return 0;
    }
    p = endPtr;
    
    // Пропускаем пробелы после первого числа
    while (*p && isspace((unsigned char)*p)) 
	{
        p++;
    }
    
    // Проверяем наличие запятой в качестве разделителя
    if (*p != ',') 
	{
		return 0;
    }
    p++; // Переход за запятую
    
    // Пропускаем пробелы после запятой
    while (*p && isspace((unsigned char)*p)) 
	{
        p++;
    }
    
    // Парсинг долготы
    double lon = strtod(p, &endPtr);
    if (p == endPtr) 
	{
        // Долгота не распознана
        return 0;
    }
    p = endPtr;
    
    // Пропускаем пробелы до конца строки
    while (*p && isspace((unsigned char)*p)) 
	{
        p++;
    }
    
    // Если после обработки двух чисел ещё есть символы, формат недопустим.
    if (*p != '\0') 
	{
        return 0;
    }
    
    // Проверка диапазона для широты (должно быть от -90 до 90)
    if (lat < -90.0 || lat > 90.0) 
	{
        return 0;
    }
    
    // Проверка диапазона для долготы (должно быть от -180 до 180)
    if (lon < -180.0 || lon > 180.0) 
	{
        return 0;
    }
    
    // Если все проверки пройдены, возвращаем 1 (формат корректен)
    return 1;
}

int input_queue(Queue* queue)
{
	int hospital_num;
	q_node* temp = queue->front;
    printf("Введите информацию о больницах:");
    printf("Введите количество больниц:");
	input_int_var(&hospital_num, 1, 100);
	
	for (int i = 0; i < hospital_num; i++)
	{
		printf("Введите номер больницы:");
		input_int_var(&(temp->clinic_number), 1, 100);

		printf("Введите координаты больницы:");
		input_str(temp->location, 1);

		printf("Введите количество мест в больнице:");
		input_int_var(&(temp->total_beds), 1, 100);

		temp->patients = (char**)malloc(temp->total_beds * sizeof(char*));
		for (int j = 0; j < hospital_num; j++)
		{
			*(temp->patients + j) = (char*)malloc(MAX_STR_SIZE * sizeof(char));
		}

		printf("Введите количество свободных мест в больнице:");
		input_int_var(&(temp->free_beds), 1, 100);

		int patient_num = temp->total_beds - temp->free_beds;
		if (patient_num)
		{
			printf("Введите фамилии пациентов, находящихся в %d больнице:", i+1);
			for (int j = 0; j < (patient_num); j++)
			{
				printf("Введите фамилию %d пациента:", j+1);
				input_str(*(temp->patients + i), 0);
			}
		}
	}
	return hospital_num;
}

//* ======================== OUTPUT ========================
void output_beds(Queue* queue, int queue_len)
{
	Queue* temp = queue;
	printf("Данные о больнице:\n");
	printf("+-------------------------------------------------+\n");
	printf("| № | Количество мест | Количество свободных мест |\n");
	printf("+-------------------------------------------------+\n");
	while (temp->front)
	{
		printf("| %d | %d | %d |\n", temp->front->clinic_number, temp->front->total_beds, temp->front->free_beds);
		printf("+----------------------------------------------------------------------------------------------------+\n");
		temp->front = temp->front->next;
	}

}

void output_patients(Queue* queue, int patient_num, int hospital_num)
{
	Queue* temp = queue;
	while(temp->front->clinic_number != hospital_num)
	{
		temp->front = temp->front->next;
	}

	printf("Данные о больнице:\n");
	printf("+-------------------------------------------------+\n");
	printf("| № больницы | № пациента |   Фамилия пациента    |\n");
	printf("+-------------------------------------------------+\n");
	for (int i = 0; i < patient_num; i++)
	{
		printf("| %d | %d | %s |\n", temp->front->clinic_number, i, *(temp->front->patients + i));
		printf("+----------------------------------------------------------------------------------------------------+\n");
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