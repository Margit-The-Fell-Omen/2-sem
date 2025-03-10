#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"

//* ======================== Memory ===========================
Full_name* students_alloc(int number_of_students)   						// функция выделения памяти для структуры ФИО
{
	Full_name* student = (Full_name*)malloc(number_of_students * sizeof(Full_name));
	return student;
}

Student_with_family* students_with_families_alloc(int number_of_students)   // функция выделения памяти для структуры студента с семьёй
{
	Student_with_family* student = (Student_with_family*)malloc(number_of_students * sizeof(Student_with_family));
	return student;
}

void students_free(Full_name* students)                                   	// функция освобождения памяти структуры ФИО
{
	free(students);
	students = NULL;
}

void students_with_families_free(Student_with_family* students)       		// функция освобождения памяти структуры студента с семьёй
{
	free(students);
	students = NULL;
}

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

void input_students_names(Full_name* students, int number_of_students)		// функция ввода ФИО студента без семьи
{
	for (int i = 0; i < number_of_students; i++)							// цикл по студентам
	{
		printf("Введите имя %d студента: \n", i+1);
		input_str((*(students + i)).name);									// ввод имени студента

		printf("Введите фамилию %d студента: \n", i+1);
		input_str((*(students + i)).surname);								// ввод фамилии студента

		printf("Введите отчество %d студента: \n", i+1);
		input_str((*(students + i)).patronymic);							// вод отчества студента
	}
}

int input_students_with_families_names( Student_with_family* students, int number_of_students, int family_choice)	// функция ввода ФИО студентов с семьёй
{
	int function_choice;
	for (int i = 0; i < number_of_students; i++)																	// цикл по студентам
	{
		printf("Введите имя %d студента: \n", i+1);
		input_str((*(students + i)).full_name.name);																// ввод имени студента
		printf("Введите фамилию %d студента: \n", i+1);
		input_str((*(students + i)).full_name.surname);																// ввод фамилии студента
		printf("Введите отчество %d студента: \n", i+1);
		input_str((*(students + i)).full_name.patronymic);															// ввод отчества студента

		switch (family_choice)																						// проверка опции состава семьи
		{
		case 1:																										// а)
			printf("Введите имя отца %d студента: \n", i+1);
			input_str((*(students + i)).family.optionA.father.name);												// ввод имени отца
			printf("Введите фамилию отца %d студента: \n", i+1);
			input_str((*(students + i)).family.optionA.father.surname);												// ввод фамилии отца
			printf("Введите отчество отца %d студента: \n", i+1);
			input_str((*(students + i)).family.optionA.father.patronymic);											// ввод отчеаства отца

			printf("Введите имя матери %d студента: \n", i+1);
			input_str((*(students + i)).family.optionA.mother.name);												// ввод имени матери
			printf("Введите фамилию матери %d студента: \n", i+1);
			input_str((*(students + i)).family.optionA.mother.surname);												// ввод фамилии матери
			printf("Введите отчество матери %d студента: \n", i+1);
			input_str((*(students + i)).family.optionA.mother.patronymic);											// ввод отчества матери

			printf("Введите имя брата %d студента: \n", i+1);
			input_str((*(students + i)).family.optionA.brother.name);												// ввод имени брата
			printf("Введите фамилию брата %d студента: \n", i+1);
			input_str((*(students + i)).family.optionA.brother.surname);											// ввод фамилии брата
			printf("Введите отчество брата %d студента: \n", i+1);
			input_str((*(students + i)).family.optionA.brother.patronymic);											// ввод отчества брата
		break;
		case 2:																										// б)
			printf("Введите имя отца %d студента: \n", i+1);
			input_str((*(students + i)).family.optionB.father.name);												// ввод имени отца
			printf("Введите фамилию отца %d студента: \n", i+1);
			input_str((*(students + i)).family.optionB.father.surname);												// ввод фамилии отца
			printf("Введите отчество отца %d студента: \n", i+1);
			input_str((*(students + i)).family.optionB.father.patronymic);											// ввод отчества отца

			printf("Введите имя матери %d студента: \n", i+1);
			input_str((*(students + i)).family.optionB.mother.name);												// ввод имени матери
			printf("Введите фамилию матери %d студента: \n", i+1);
			input_str((*(students + i)).family.optionB.mother.surname);												// ввод фамилии матери
			printf("Введите отчество матери %d студента: \n", i+1);
			input_str((*(students + i)).family.optionB.mother.patronymic);											// ввод отчества матери

			printf("Введите имя брата %d студента: \n", i+1);
			input_str((*(students + i)).family.optionB.brother.name);												// ввод имени брата
			printf("Введите фамилию брата %d студента: \n", i+1);
			input_str((*(students + i)).family.optionB.brother.surname);											// ввод фамилии брата
			printf("Введите отчество брата %d студента: \n", i+1);
			input_str((*(students + i)).family.optionB.brother.patronymic);											// ввод отчества брата

			printf("Введите имя сестры %d студента: \n", i+1);
			input_str((*(students + i)).family.optionB.sister.name);												// ввод имени сестры
			printf("Введите фамилию сестры %d студента: \n", i+1);
			input_str((*(students + i)).family.optionB.sister.surname);												// ввод фамилии сестры
			printf("Введите отчество сестры %d студента: \n", i+1);
			input_str((*(students + i)).family.optionB.sister.patronymic);											// ввод отчества сестры
		break;
		case 3:																										// в)
			printf("Введите имя матери %d студента: \n", i+1);
			input_str((*(students + i)).family.optionC.mother.name);												// ввод имени матери
			printf("Введите фамилию матери %d студента: \n", i+1);
			input_str((*(students + i)).family.optionC.mother.surname);												// ввод фамилии матери
			printf("Введите отчество матери %d студента: \n", i+1);
			input_str((*(students + i)).family.optionC.mother.patronymic);											// ввод отчества матери

			printf("Введите имя брата %d студента: \n", i+1);
			input_str((*(students + i)).family.optionC.brother.name);												// ввод имени брата
			printf("Введите фамилию брата %d студента: \n", i+1);
			input_str((*(students + i)).family.optionC.brother.surname);											// ввод фамилии брата
			printf("Введите отчество брата %d студента: \n", i+1);
			input_str((*(students + i)).family.optionC.brother.patronymic);											// ввод отчества брата

			printf("Введите имя сестры %d студента: \n", i+1);
			input_str((*(students + i)).family.optionC.sister.name);												// ввод имени сестры
			printf("Введите фамилию сестры %d студента: \n", i+1);
			input_str((*(students + i)).family.optionC.sister.surname);												// ввод фамилии сестры
			printf("Введите отчество сестры %d студента: \n", i+1);
			input_str((*(students + i)).family.optionC.sister.patronymic);											// ввод отчества сестры
		break;
		}
	}
	printf("Введите что вывести на экран: \n\t1 - вывод ФИО студента\n\t2 - вывод ФИО семьи студента\n\t3 - вывод ФИО студента и его семьи\n");
	input_int_var(&function_choice, 4, 1, 3);																		// выбор функции вывода
	return function_choice;
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

			if (char_count > MAX_NAME_SIZE/2-1)							// проверка на слишком большую строку
			{   
				printf("\033[1;31mНазвание слишком длинное. Пожалуйста, введите название не более 30 символов.\033[0m\n");
				continue;
			}
			else if (buffer_len >= MAX_NAME_SIZE)
				while ((c = getchar()) != '\n' && c != EOF);			// очистка буфера ввода			

				if (char_count < MAX_NAME_SIZE/2-1)						// Заполнение оставшегося места пробелами для выравнивания
				{
				    for (int j = 0; j < (30 - char_count); j++) 		// Добавление пробелов (пробелы занимают 1 байт в UTF-8)
				    {
				        *(buffer + buffer_len++) = ' ';
				    }
				    *(buffer + buffer_len) = '\0'; 						// Добавление завершающего нулевого символа
				}

			strncpy(destination, buffer, MAX_NAME_SIZE);				// копирование буффера в поле структуры
			valid = 1; 													// Ввод корректен, выход из цикла
		}
}

//* ======================== OUTPUT ========================
void output_student_full_name(Full_name* students, int number_of_students)         			// функция вывода ФИО студентов без семей
{
	printf("Данные о студентах:\n");
	printf("+--------------------------------------------------------------------------------------------------------+\n");
	printf("| № |             Имя                 |            Фамилия             |            Отчество             |\n");
	printf("+--------------------------------------------------------------------------------------------------------+\n");
	for (int i = 0; i < number_of_students; i++) 
	{
		printf("| %d | %s | %s | %s |\n", i, (*(students + i)).name, (*(students + i)).surname, (*(students + i)).patronymic);
		printf("+----------------------------------------------------------------------------------------------------+\n");
	}
}

void output_1(Student_with_family* students, int number_of_students, int family_choise)		// функция вывода ФИО студентов с семьями
{
	printf("Данные о студентах:\n");
	printf("+----------------------------------------------------------------------------------------------------+\n");
	printf("| № |             Имя                 |            Фамилия             |          Отчество           |\n");
	printf("+----------------------------------------------------------------------------------------------------+\n");
	for (int i = 0; i < number_of_students; i++) 
	{
		printf("| %d | %s | %s | %s |\n", i, (*(students + i)).full_name.name, (*(students + i)).full_name.surname, (*(students + i)).full_name.patronymic);
		printf("+----------------------------------------------------------------------------------------------------+\n");
	}
}

void output_2(Student_with_family* students, int number_of_students, int family_choise)		// функция вывода ФИО семей
{
	printf("Данные о семьях студентов:\n");
	printf("+-----------------------------------------------------------------------------------------------------------------+\n");
	printf("| № |             Имя                 |            Фамилия             |          Отчество            | Член семьи |\n");
	printf("+-----------------------------------------------------------------------------------------------------------------+\n");
	for (int i = 0; i < number_of_students; i++) 
	{
		switch (family_choise) 
		{
			case 1:
			printf("| %d | %s | %s | %s | отец    |\n", i, (*(students + i)).family.optionA.father.name, (*(students + i)).family.optionA.father.surname, (*(students + i)).family.optionA.father.patronymic);
			printf("+-----------------------------------------------------------------------------------------------------------------+\n");
			printf("| %d | %s | %s | %s | мать    |\n", i, (*(students + i)).family.optionA.mother.name, (*(students + i)).family.optionA.mother.surname, (*(students + i)).family.optionA.mother.patronymic);
			printf("+-----------------------------------------------------------------------------------------------------------------+\n");
			printf("| %d | %s | %s | %s | брат    |\n", i, (*(students + i)).family.optionA.brother.name, (*(students + i)).family.optionA.brother.surname, (*(students + i)).family.optionA.brother.patronymic);
			printf("+-----------------------------------------------------------------------------------------------------------------+\n");
			break;

			case 2:
			printf("| %d | %s | %s | %s | отец    |\n", i, (*(students + i)).family.optionB.father.name, (*(students + i)).family.optionB.father.surname, (*(students + i)).family.optionB.father.patronymic);
			printf("+-----------------------------------------------------------------------------------------------------------------+\n");
			printf("| %d | %s | %s | %s | мать    |\n", i, (*(students + i)).family.optionB.mother.name, (*(students + i)).family.optionB.mother.surname, (*(students + i)).family.optionB.mother.patronymic);
			printf("+-----------------------------------------------------------------------------------------------------------------+\n");
			printf("| %d | %s | %s | %s | брат    |\n", i, (*(students + i)).family.optionB.brother.name, (*(students + i)).family.optionB.brother.surname, (*(students + i)).family.optionB.brother.patronymic);
			printf("+-----------------------------------------------------------------------------------------------------------------+\n");
			printf("| %d | %s | %s | %s | сестра  |\n", i, (*(students + i)).family.optionB.sister.name, (*(students + i)).family.optionB.sister.surname, (*(students + i)).family.optionB.sister.patronymic);
			printf("+-----------------------------------------------------------------------------------------------------------------+\n");
			break;

			case 3:
			printf("| %d | %s | %s | %s | мать    |\n", i, (*(students + i)).family.optionC.mother.name, (*(students + i)).family.optionC.mother.surname, (*(students + i)).family.optionC.mother.patronymic);
			printf("+-----------------------------------------------------------------------------------------------------------------+\n");
			printf("| %d | %s | %s | %s | сестра  |\n", i, (*(students + i)).family.optionC.sister.name, (*(students + i)).family.optionC.sister.surname, (*(students + i)).family.optionC.sister.patronymic);
			printf("+-----------------------------------------------------------------------------------------------------------------+\n");
			printf("| %d | %s | %s | %s | брат    |\n", i, (*(students + i)).family.optionC.brother.name, (*(students + i)).family.optionC.brother.surname, (*(students + i)).family.optionC.brother.patronymic);
			printf("+-----------------------------------------------------------------------------------------------------------------+\n");
			break;
		}

	}
}

void output_3(Student_with_family* students, int number_of_students, int family_choise) 	// функция вывода ФИО студентов и семьями
{
	printf("Данные о студентах с семьями:\n");
	printf("+-----------------------------------------------------------------------------------------------------------------+\n");
	printf("| № |             Имя                |            Фамилия             |          Отчество            | Член семьи |\n");
	printf("+-----------------------------------------------------------------------------------------------------------------+\n");
	for (int i = 0; i < number_of_students; i++) 
	{
		printf("| %d | %s | %s | %s | студент |\n", i, (*(students + i)).full_name.name, (*(students + i)).full_name.surname, (*(students + i)).full_name.patronymic);
		printf("+-----------------------------------------------------------------------------------------------------------------+\n");
		switch (family_choise) 
		{
			case 1:
			printf("| %d | %s | %s | %s | отец    |\n", i, (*(students + i)).family.optionA.father.name, (*(students + i)).family.optionA.father.surname, (*(students + i)).family.optionA.father.patronymic);
			printf("+-----------------------------------------------------------------------------------------------------------------+\n");
			printf("| %d | %s | %s | %s | мать    |\n", i, (*(students + i)).family.optionA.mother.name, (*(students + i)).family.optionA.mother.surname, (*(students + i)).family.optionA.mother.patronymic);
			printf("+-----------------------------------------------------------------------------------------------------------------+\n");
			printf("| %d | %s | %s | %s | брат    |\n", i, (*(students + i)).family.optionA.brother.name, (*(students + i)).family.optionA.brother.surname, (*(students + i)).family.optionA.brother.patronymic);
			printf("+-----------------------------------------------------------------------------------------------------------------+\n");
			break;

			case 2:
			printf("| %d | %s | %s | %s | отец    |\n", i, (*(students + i)).family.optionB.father.name, (*(students + i)).family.optionB.father.surname, (*(students + i)).family.optionB.father.patronymic);
			printf("+-----------------------------------------------------------------------------------------------------------------+\n");
			printf("| %d | %s | %s | %s | мать    |\n", i, (*(students + i)).family.optionB.mother.name, (*(students + i)).family.optionB.mother.surname, (*(students + i)).family.optionB.mother.patronymic);
			printf("+-----------------------------------------------------------------------------------------------------------------+\n");
			printf("| %d | %s | %s | %s | брат    |\n", i, (*(students + i)).family.optionB.brother.name, (*(students + i)).family.optionB.brother.surname, (*(students + i)).family.optionB.brother.patronymic);
			printf("+-----------------------------------------------------------------------------------------------------------------+\n");
			printf("| %d | %s | %s | %s | сестра  |\n", i, (*(students + i)).family.optionB.sister.name, (*(students + i)).family.optionB.sister.surname, (*(students + i)).family.optionB.sister.patronymic);
			printf("+-----------------------------------------------------------------------------------------------------------------+\n");
			break;

			case 3:
			printf("| %d | %s | %s | %s | мать    |\n", i, (*(students + i)).family.optionC.mother.name, (*(students + i)).family.optionC.mother.surname, (*(students + i)).family.optionC.mother.patronymic);
			printf("+-----------------------------------------------------------------------------------------------------------------+\n");
			printf("| %d | %s | %s | %s | сестра  |\n", i, (*(students + i)).family.optionC.sister.name, (*(students + i)).family.optionC.sister.surname, (*(students + i)).family.optionC.sister.patronymic);
			printf("+-----------------------------------------------------------------------------------------------------------------+\n");
			printf("| %d | %s | %s | %s | брат    |\n", i, (*(students + i)).family.optionC.brother.name, (*(students + i)).family.optionC.brother.surname, (*(students + i)).family.optionC.brother.patronymic);
			printf("+-----------------------------------------------------------------------------------------------------------------+\n");
			break;
		}

	}
}

void output_student_with_family(Student_with_family* students, int number_of_students, int family_choise, int function_choice)	// функция вызова выбранной функции вывода
{
	switch (function_choice) 
	{
		case 1:
		students->output_1 = output_1;
		students->output_1(students, number_of_students, family_choise);

		break;

		case 2:
		students->output_2 = output_2;
		students->output_2(students, number_of_students, family_choise);

		break;

		case 3:
		students->output_3 = output_3;
		students->output_3(students, number_of_students, family_choise);
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