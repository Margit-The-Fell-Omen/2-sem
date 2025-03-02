#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"

//* ======================== Memory ===========================
Full_name* students_alloc(int number_of_students)   // функция выделения памяти для структуры
{
	Full_name* student = (Full_name*)malloc(number_of_students * sizeof(Full_name));
	return student;
}

Student_with_family* students_with_families_alloc(int number_of_students)   // функция выделения памяти для структуры
{
	Student_with_family* student = (Student_with_family*)malloc(number_of_students * sizeof(Student_with_family));
	return student;
}

void students_free(Full_name* students)                                        // функция освобождения памяти структуры
{
	free(students);
	students = NULL;
}

void students_with_families_free(Student_with_family* students)                                        // функция освобождения памяти структуры
{
	free(students);
	students = NULL;
}
//* ======================== Checks ============================
int is_numeric(const char* str)                     // функция проверки на бытиё числом
{
	while (*str)                                    // цикл по элементам строки
	{
		if (!isdigit(*str) && *str != '.' && *str != ',') return 0;
		str++;
	}
	return 1;
}

int is_valid_num(const char* str)                   // функция проверки числа
{
	int num = atoi(str);						    // перевод в int
	if (num > 1000000 || num < 0) return 0;
	return 1;
}

int str_compare(const char* str1, const char* str2)	// функция сравнения строк
{
	int len1 = strlen(str1);
	int len2 = strlen(str2);
	
	if (len1 != len2)                               // сравнение длин строк
	{
		return 0;
	} 
	for (int i = 0; i < len1; i++)                  // цикл по символам строк
	{
		if (*(str1 + i) != *(str2 + i))             // сравнение символов строк
		{
			return 0;
		}
	}
	return 1;
}

int is_valid_date(const char* str)					// функция проверки даты
{
	const char* en_months[13] = 					// английские месяцы
	{
		"0", "January", "February", "March", "April", "May", "June",
		"July", "August", "September", "October", "November", "December"
	};

	const char* ru_months[12] = 					// русские месяцы
	{ 
		"Январь", "Февраль", "Март", "Апрель", "Май", "Июнь",
		"Июль", "Август", "Сентябрь", "Октябрь", "Ноябрь", "Декобрь" 
	};

	int count = 0;									// счётчик совпадений

	for (int i = 0; i < 13; i++)					// цикл по месяцам
	{
		if (str_compare((*(en_months + i)), str) || str_compare(str, (*(ru_months + i)))) count++;
	}

	if (count) return 1;
	return 0;
}
//* ======================== TOP-5 ========================

//* ======================== INPUT ========================
void input_int_var(int* a, int t, int min, int max)											// функция ввода и проверки целых чисел
{
	int check = 0;
	int c;
	while (check != 1)	                                									// цикл ввода и проверки
	{
		switch (t)																			// вывод сообщений с описанием вводимой величины
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
		check = scanf("%d", a);								                                                	// ввод значения
		if (check != 1)														                                    // проверка правильности типа введённого значения
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
		while ((c = getchar()) != '\n' && c != EOF);															// очистка буфера ввода
	}
}

void input_students_names(Full_name* students, int number_of_students)
{
	for (int i = 0; i < number_of_students; i++)
	{
		printf("Введите имя %d студента: \n", i+1);
		input_str((*(students + i)).name);

		printf("Введите фамилию %d студента: \n", i+1);
		input_str((*(students + i)).surname);

		printf("Введите отчество %d студента: \n", i+1);
		input_str((*(students + i)).patronymic);
	}
}

int input_students_with_families_names( Student_with_family* students, int number_of_students, int family_choice)
{
	int function_choice;
	for (int i = 0; i < number_of_students; i++)
	{
		printf("Введите имя %d студента: \n", i+1);
		input_str((*(students + i)).full_name.name);
		printf("Введите фамилию %d студента: \n", i+1);
		input_str((*(students + i)).full_name.surname);
		printf("Введите отчество %d студента: \n", i+1);
		input_str((*(students + i)).full_name.patronymic);

		switch (family_choice)																			// вывод сообщений с описанием вводимой величины
		{
		case 1:
			printf("Введите имя отца %d студента: \n", i+1);
			input_str((*(students + i)).family.optionA.father.name);
			printf("Введите фамилию отца %d студента: \n", i+1);
			input_str((*(students + i)).family.optionA.father.surname);
			printf("Введите отчество отца %d студента: \n", i+1);
			input_str((*(students + i)).family.optionA.father.patronymic);

			printf("Введите имя матери %d студента: \n", i+1);
			input_str((*(students + i)).family.optionA.mother.name);
			printf("Введите фамилию матери %d студента: \n", i+1);
			input_str((*(students + i)).family.optionA.mother.surname);
			printf("Введите отчество матери %d студента: \n", i+1);
			input_str((*(students + i)).family.optionA.mother.patronymic);

			printf("Введите имя брата %d студента: \n", i+1);
			input_str((*(students + i)).family.optionA.brother.name);
			printf("Введите фамилию брата %d студента: \n", i+1);
			input_str((*(students + i)).family.optionA.brother.surname);
			printf("Введите отчество брата %d студента: \n", i+1);
			input_str((*(students + i)).family.optionA.brother.patronymic);
		break;
		case 2:
			printf("Введите имя отца %d студента: \n", i+1);
			input_str((*(students + i)).family.optionA.father.name);
			printf("Введите фамилию отца %d студента: \n", i+1);
			input_str((*(students + i)).family.optionA.father.surname);
			printf("Введите отчество отца %d студента: \n", i+1);
			input_str((*(students + i)).family.optionA.father.patronymic);

			printf("Введите имя матери %d студента: \n", i+1);
			input_str((*(students + i)).family.optionA.mother.name);
			printf("Введите фамилию матери %d студента: \n", i+1);
			input_str((*(students + i)).family.optionA.mother.surname);
			printf("Введите отчество матери %d студента: \n", i+1);
			input_str((*(students + i)).family.optionA.mother.patronymic);

			printf("Введите имя брата %d студента: \n", i+1);
			input_str((*(students + i)).family.optionA.brother.name);
			printf("Введите фамилию брата %d студента: \n", i+1);
			input_str((*(students + i)).family.optionA.brother.surname);
			printf("Введите отчество брата %d студента: \n", i+1);
			input_str((*(students + i)).family.optionA.brother.patronymic);

			printf("Введите имя сестры %d студента: \n", i+1);
			input_str((*(students + i)).family.optionB.sister.name);
			printf("Введите фамилию сестры %d студента: \n", i+1);
			input_str((*(students + i)).family.optionB.sister.surname);
			printf("Введите отчество сестры %d студента: \n", i+1);
			input_str((*(students + i)).family.optionB.sister.patronymic);
		break;
		case 3:
			printf("Введите имя матери %d студента: \n", i+1);
			input_str((*(students + i)).family.optionC.mother.name);
			printf("Введите фамилию матери %d студента: \n", i+1);
			input_str((*(students + i)).family.optionC.mother.surname);
			printf("Введите отчество матери %d студента: \n", i+1);
			input_str((*(students + i)).family.optionC.mother.patronymic);

			printf("Введите имя брата %d студента: \n", i+1);
			input_str((*(students + i)).family.optionC.brother.name);
			printf("Введите фамилию брата %d студента: \n", i+1);
			input_str((*(students + i)).family.optionC.brother.surname);
			printf("Введите отчество брата %d студента: \n", i+1);
			input_str((*(students + i)).family.optionC.brother.patronymic);

			printf("Введите имя сестры %d студента: \n", i+1);
			input_str((*(students + i)).family.optionC.sister.name);
			printf("Введите фамилию сестры %d студента: \n", i+1);
			input_str((*(students + i)).family.optionC.sister.surname);
			printf("Введите отчество сестры %d студента: \n", i+1);
			input_str((*(students + i)).family.optionC.sister.patronymic);
		break;
		}
	}
	printf("Введите что вывести на экран: \n\t1 - вывод ФИО студента\n\t2 - вывод ФИО семьи студента\n\t3 - вывод ФИО студента и его семьи\n");
	input_int_var(&function_choice, 4, 1, 3);
	return function_choice;
}

void input_str(char* destination)		// функция ввода названия фирмы
{
	char buffer[MAX_NAME_SIZE];											// буфер для ввода строки
	char c;																// переменная для очистки буфера ввода
		int valid = 0;													// переменная для проверки ввода
		while (!valid) 													// цикл ввода и проверки
		{
			if (fgets(buffer, sizeof(buffer), stdin) == NULL) 				// ввод строки
			{
				printf("\033[1;31m Ошибка ввода. Пожалуйста, попробуйте ещё раз.\033[0m\n");
				continue;
			}

			buffer[strcspn(buffer, "\n")] = '\0';							// Удаление символа новой строки, если он есть

			int char_count = 0;											// счётчик длины UTF-8 строки
			for (char* p = buffer; *p != '\0';) 							// Вычисление количества UTF-8 символов
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

		if (char_count == -1)													            // проверка наневерную кодировку
				continue;
			
			int buffer_len = strlen(buffer); 											            // получение длины строки	

			if (char_count == 0) 													            // проверка на пустую строку
			{
				printf("\033[1;31m Название не может быть пустым. Пожалуйста, попробуйте ещё раз.\033[0m\n");
				continue;
			}

			if (char_count > MAX_NAME_SIZE/2-1)													            // проверка на слишком большую строку
			{   
				printf("\033[1;31mНазвание слишком длинное. Пожалуйста, введите название не более 30 символов.\033[0m\n");
				continue;
			}
			else if (buffer_len >= MAX_NAME_SIZE)
				while ((c = getchar()) != '\n' && c != EOF);						

				if (char_count < MAX_NAME_SIZE/2-1)													            // Заполнение оставшегося места пробелами для выравнивания
				{
				    for (int j = 0; j < (30 - char_count); j++) 						            // Добавление пробелов (пробелы занимают 1 байт в UTF-8)
				    {
				        *(buffer + buffer_len++) = ' ';
				    }
				    *(buffer + buffer_len) = '\0'; 												        // Добавление завершающего нулевого символа
				}

			strncpy(destination, buffer, MAX_NAME_SIZE);	// копирование буффера в поле структуры
			valid = 1; 																            // Ввод корректен, выход из цикла
		}
}

//* ======================== OUTPUT ========================
void output_student_full_name(Full_name* students, int number_of_students)         // функция вывода данных о фирмах
{
	printf("Данные о студентах:\n");
	printf("+-------------------------------------------------------------------------------------------------------+\n");
	printf("| № |             Имя                |            Фамилия             |            Отчество             |\n");
	printf("+-------------------------------------------------------------------------------------------------------+\n");
	for (int i = 0; i < number_of_students; i++) 
	{
		printf("| %d | %s | %s | %s |\n", i, (*(students + i)).name, (*(students + i)).surname, (*(students + i)).patronymic);
		printf("+----------------------------------------------------------------------------------------------------+\n");
	}
}

void output_student_with_family(Student_with_family* students, int number_of_students, int family_choise, int function_choice)
{
	switch (function_choice) 
	{
		case 1:
		printf("Данные о студентах:\n");
		printf("+---------------------------------------------------------------------------------------------------+\n");
		printf("| № |             Имя                |            Фамилия             |          Отчество           |\n");
		printf("+---------------------------------------------------------------------------------------------------+\n");
		for (int i = 0; i < number_of_students; i++) 
		{
			printf("| %d | %s | %s | %s |\n", i, (*(students + i)).full_name.name, (*(students + i)).full_name.surname, (*(students + i)).full_name.patronymic);
			printf("+----------------------------------------------------------------------------------------------------+\n");
		}
		break;
		case 2:
		printf("Данные о семьях студентов:\n");
		printf("+----------------------------------------------------------------------------------------------------------------+\n");
		printf("| № |             Имя                |            Фамилия             |          Отчество            | Член семьи |\n");
		printf("+----------------------------------------------------------------------------------------------------------------+\n");
		for (int i = 0; i < number_of_students; i++) 
		{
			switch (family_choise) 
			{
				case 1:
				printf("| %d | %s | %s | %s | отец |\n", i, (*(students + i)).family.optionA.father.name, (*(students + i)).family.optionA.father.surname, (*(students + i)).family.optionA.father.patronymic);
				printf("+----------------------------------------------------------------------------------------------------------------+\n");
				printf("| %d | %s | %s | %s | мать |\n", i, (*(students + i)).family.optionA.mother.name, (*(students + i)).family.optionA.mother.surname, (*(students + i)).family.optionA.mother.patronymic);
				printf("+----------------------------------------------------------------------------------------------------------------+\n");
				printf("| %d | %s | %s | %s | брат |\n", i, (*(students + i)).family.optionA.brother.name, (*(students + i)).family.optionA.brother.surname, (*(students + i)).family.optionA.brother.patronymic);
				printf("+----------------------------------------------------------------------------------------------------------------+\n");
				break;

				case 2:
				printf("| %d | %s | %s | %s | отец |\n", i, (*(students + i)).family.optionB.father.name, (*(students + i)).family.optionB.father.surname, (*(students + i)).family.optionB.father.patronymic);
				printf("+----------------------------------------------------------------------------------------------------------------+\n");
				printf("| %d | %s | %s | %s | мать |\n", i, (*(students + i)).family.optionB.mother.name, (*(students + i)).family.optionB.mother.surname, (*(students + i)).family.optionB.mother.patronymic);
				printf("+----------------------------------------------------------------------------------------------------------------+\n");
				printf("| %d | %s | %s | %s | брат |\n", i, (*(students + i)).family.optionB.brother.name, (*(students + i)).family.optionB.brother.surname, (*(students + i)).family.optionB.brother.patronymic);
				printf("+----------------------------------------------------------------------------------------------------------------+\n");
				printf("| %d | %s | %s | %s | сестра |\n", i, (*(students + i)).family.optionB.sister.name, (*(students + i)).family.optionB.sister.surname, (*(students + i)).family.optionB.sister.patronymic);
				printf("+----------------------------------------------------------------------------------------------------------------+\n");
				break;

				case 3:
				printf("| %d | %s | %s | %s | мать |\n", i, (*(students + i)).family.optionC.mother.name, (*(students + i)).family.optionC.mother.surname, (*(students + i)).family.optionC.mother.patronymic);
				printf("+----------------------------------------------------------------------------------------------------------------+\n");
				printf("| %d | %s | %s | %s | сестра |\n", i, (*(students + i)).family.optionC.sister.name, (*(students + i)).family.optionC.sister.surname, (*(students + i)).family.optionC.sister.patronymic);
				printf("+----------------------------------------------------------------------------------------------------------------+\n");
				printf("| %d | %s | %s | %s | брат |\n", i, (*(students + i)).family.optionC.brother.name, (*(students + i)).family.optionC.brother.surname, (*(students + i)).family.optionC.brother.patronymic);
				printf("+----------------------------------------------------------------------------------------------------------------+\n");
				break;
			}

		}
		break;
		case 3:
		printf("Данные о студентах с семьями:\n");
		printf("+----------------------------------------------------------------------------------------------------------------+\n");
		printf("| № |             Имя               |            Фамилия             |          Отчество            | Член семьи |\n");
		printf("+----------------------------------------------------------------------------------------------------------------+\n");
		for (int i = 0; i < number_of_students; i++) 
		{
			printf("| %d | %s | %s | %s | студент |\n", i, (*(students + i)).full_name.name, (*(students + i)).full_name.surname, (*(students + i)).full_name.patronymic);
			printf("+----------------------------------------------------------------------------------------------------------------+\n");
			switch (family_choise) 
			{
				case 1:
				printf("| %d | %s | %s | %s | отец |\n", i, (*(students + i)).family.optionA.father.name, (*(students + i)).family.optionA.father.surname, (*(students + i)).family.optionA.father.patronymic);
				printf("+----------------------------------------------------------------------------------------------------------------+\n");
				printf("| %d | %s | %s | %s | мать |\n", i, (*(students + i)).family.optionA.mother.name, (*(students + i)).family.optionA.mother.surname, (*(students + i)).family.optionA.mother.patronymic);
				printf("+----------------------------------------------------------------------------------------------------------------+\n");
				printf("| %d | %s | %s | %s | брат |\n", i, (*(students + i)).family.optionA.brother.name, (*(students + i)).family.optionA.brother.surname, (*(students + i)).family.optionA.brother.patronymic);
				printf("+----------------------------------------------------------------------------------------------------------------+\n");
				break;

				case 2:
				printf("| %d | %s | %s | %s | отец |\n", i, (*(students + i)).family.optionB.father.name, (*(students + i)).family.optionB.father.surname, (*(students + i)).family.optionB.father.patronymic);
				printf("+----------------------------------------------------------------------------------------------------------------+\n");
				printf("| %d | %s | %s | %s | мать |\n", i, (*(students + i)).family.optionB.mother.name, (*(students + i)).family.optionB.mother.surname, (*(students + i)).family.optionB.mother.patronymic);
				printf("+----------------------------------------------------------------------------------------------------------------+\n");
				printf("| %d | %s | %s | %s | брат |\n", i, (*(students + i)).family.optionB.brother.name, (*(students + i)).family.optionB.brother.surname, (*(students + i)).family.optionB.brother.patronymic);
				printf("+----------------------------------------------------------------------------------------------------------------+\n");
				printf("| %d | %s | %s | %s | сестра |\n", i, (*(students + i)).family.optionB.sister.name, (*(students + i)).family.optionB.sister.surname, (*(students + i)).family.optionB.sister.patronymic);
				printf("+----------------------------------------------------------------------------------------------------------------+\n");
				break;

				case 3:
				printf("| %d | %s | %s | %s | мать |\n", i, (*(students + i)).family.optionC.mother.name, (*(students + i)).family.optionC.mother.surname, (*(students + i)).family.optionC.mother.patronymic);
				printf("+----------------------------------------------------------------------------------------------------------------+\n");
				printf("| %d | %s | %s | %s | сестра |\n", i, (*(students + i)).family.optionC.sister.name, (*(students + i)).family.optionC.sister.surname, (*(students + i)).family.optionC.sister.patronymic);
				printf("+----------------------------------------------------------------------------------------------------------------+\n");
				printf("| %d | %s | %s | %s | брат |\n", i, (*(students + i)).family.optionC.brother.name, (*(students + i)).family.optionC.brother.surname, (*(students + i)).family.optionC.brother.patronymic);
				printf("+----------------------------------------------------------------------------------------------------------------+\n");
				break;
			}

		}
		break;
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