#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"

//* ======================== Memory ===========================
firm_info* firm_info_alloc(int number_of_firms)     // функция выделения памяти для структуры
{
	firm_info* list_of_firms = (firm_info*)calloc(number_of_firms, sizeof(firm_info));
	return list_of_firms;
}

void firm_info_free(firm_info* list_of_firms)       // функция освобождения памяти структуры
{
	free(list_of_firms);
	list_of_firms = NULL;
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

int compare(const void* a, const void* b)			// функция сравнения
{
    return (*(int*)b - *(int*)a); 
}

int what_month(const char* str) 					// функция определения порядкогого номера месяца
{ 
    const char* en_months[12] =						// английские месяцы
    { 
        "January", "February", "March", "April", "May", "June",
        "July", "August", "September", "October", "November", "December" 
    };
    const char* ru_months[12] = 					// русские месяцы
    { 
        "Январь", "Февраль", "Март", "Апрель", "Май", "Июнь",
        "Июль", "Август", "Сентябрь", "Октябрь", "Ноябрь", "Декобрь" 
    };
    for (int i = 0; i < 12; i++)					// цикл по месяцам
    { 
        if (str_compare(str, (*(en_months + i))) || str_compare(str, (*(ru_months + i)))) return i; 
    } 
    return 0; 
}

int* find_top_5(firm_info* list_of_firms, int number_of_firms, const char* month) 			// функция нахождения топ-5 фирм по налогам
{
    int* list_of_taxes = (int*)malloc(number_of_firms * sizeof(int));						// массив налогов фирм
    int* top_5 = (int*)calloc(5, sizeof(int));												// массив индексов топ-5 фирм
    int count = 0;																			// счётчик для прохода по top_5

    for (int i = 0; i < number_of_firms; i++)												// преобразование налогов в целые числа
	{
        *(list_of_taxes + i) = atoi((*(list_of_firms + i)).taxes);
    }

    qsort(list_of_taxes, number_of_firms, sizeof(int), compare);							// сортировка налогов по убыванию

    for (int i = 0; i < number_of_firms && count < 5; i++)									// цикл по фирмам пока не найден топ-5
	{
        for (int j = 0; j < number_of_firms; j++) 											// цикл по фирмам
		{
            if (*(list_of_taxes + i) == atoi((*(list_of_firms + j)).taxes) &&				// сравнение налога фирмы из топ-5 налогов и проверка оплаты 
                what_month((*(list_of_firms + j)).taxes_payed) < what_month(month)) 
			{
                *(top_5 + count) = j;														// запись индекса топ-5 фирмы в top_5
                count++;
                break;
            }
        }
    }
    sort_top_5_by_name(list_of_firms, top_5);
    free(list_of_taxes);
    return top_5;
}

void sort_top_5_by_name(firm_info* list_of_firms, int* top_5)                               // функция сортировки в алфавитном порядке
{
    for (int i = 0; i < 4; i++)                                                             // сортировка пузырьком
    {
        for (int j = 0; j < 4 - i; j++) 
        {
            if (strcmp(list_of_firms[top_5[j]].name, list_of_firms[top_5[j + 1]].name) > 0) // сравнение строк
            {
                // Обмен индексов в top_5
                int temp = top_5[j];
                top_5[j] = top_5[j + 1];
                top_5[j + 1] = temp;
            }
        }
    }
}
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
			printf("Введите количество фирм(целое положительное десятичное число): \n");
			break;
        case 2:
			printf("Ввберите месяц (1-12): \n");
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

const char* input_month()												// функция ввода месяца
{
    const char* en_months[12] = 										// английские месяцы
    {
        "January", "February", "March", "April", "May", "June",
        "July", "August", "September", "October", "November", "December"
    };
    const char* ru_months[12] = 										// русские месяцы
    { 
        "Январь", "Февраль", "Март", "Апрель", "Май", "Июнь",
        "Июль", "Август", "Сентябрь", "Октябрь", "Ноябрь", "Декобрь" 
    };
    int choice = 0;														// переменная выбора номера месяца
    input_int_var(&choice, 2, 1, 12);									// ввод номера месяца
    return en_months[choice - 1];
}

void input_firm_name(firm_info* list_of_firms, int number_of_firms)		// функция ввода названия фирмы
{
    char name[64];														// буфер для ввода строки
	char c;																// переменная для очистки буфера ввода

    for (int i = 0; i < number_of_firms; i++) 							// цикл по фирмам
    {
        int valid = 0;													// переменная для проверки ввода
        while (!valid) 													// цикл ввода и проверки
        {
            printf("Введите название %d-й фирмы (не более 30 символов): ", i + 1);
            if (fgets(name, sizeof(name), stdin) == NULL) 				// ввод строки
            {
                printf("\033[1;31m Ошибка ввода. Пожалуйста, попробуйте ещё раз.\033[0m\n");
                continue;
            }

            name[strcspn(name, "\n")] = '\0';							// Удаление символа новой строки, если он есть

            int char_count = 0;											// счётчик длины UTF-8 строки
            for (char* p = name; *p != '\0';) 							// Вычисление количества UTF-8 символов
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
			
            int name_len = strlen(name); 											            // получение длины строки	

            if (char_count == 0) 													            // проверка на пустую строку
            {
                printf("\033[1;31m Название не может быть пустым. Пожалуйста, попробуйте ещё раз.\033[0m\n");
                continue;
            }

            if (char_count > 30)													            // проверка на слишком большую строку
            {   
                printf("\033[1;31mНазвание слишком длинное. Пожалуйста, введите название не более 30 символов.\033[0m\n");
                continue;
            }
			else if (name_len >= 61)
				while ((c = getchar()) != '\n' && c != EOF);						

            if (char_count < 30)													            // Заполнение оставшегося места пробелами для выравнивания
            {
                for (int j = 0; j < (30 - char_count); j++) 						            // Добавление пробелов (пробелы занимают 1 байт в UTF-8)
                {
                    *(name + name_len++) = ' ';
                }
                *(name + name_len) = '\0'; 												        // Добавление завершающего нулевого символа
            }

            strncpy((*(list_of_firms + i)).name, name, sizeof((*(list_of_firms + i)).name));	// копирование буффера в поле структуры
            valid = 1; 																            // Ввод корректен, выход из цикла
        }
    }
}

void input_firm_taxes(firm_info* list_of_firms, int number_of_firms) 				// функция ввода налогов
{					
    char taxes[9]; 																	// буфер для строки ввода
	int c;																			// переменная для очистки буфера ввода

    for (int i = 0; i < number_of_firms; i++) 										// цикл по фирмам
	{
        int valid = 0;																// переменная для проверки ввода
        while (!valid) 																// цикл ввода и проверки
		{
            printf("Введите величину налогообложения фирмы %s в рублях (не более 1000000): ", (*(list_of_firms + i)).name);
            if (fgets(taxes, sizeof(taxes), stdin) == NULL) 						// ввод строки
			{
                printf("\033[1;31m Ошибка ввода. Пожалуйста, попробуйте ещё раз.\033[0m\n");
                continue;
            }

            unsigned int len = strlen(taxes);                                       // получение длины строки

            if (*(taxes + len - 1) != '\n')                                         // проверка на слишком большую строку
			{
                while ((c = getchar()) != '\n' && c != EOF);
                printf("\033[1;31m Введено слишком много символов. Пожалуйста, введите не более 7 символов.\033[0m\n");
                continue;
            }

            *(taxes + len - 1) = '\0';                                              // Удаление символа новой строки
            len--;                                                                  // Уменьшаем длину после удаления '\n'

            if (len == 0)                                                           // Проверка на пустую строку
			{
                printf("\033[1;31m Величина налогообложения не может быть пустой (введите 0, если её нет). Попробуйте ещё раз.\033[0m\n");
                continue;
            }

			if (!is_numeric(taxes))                                                 // проверка на бытиё чистом
			{
                printf("\033[1;31m Введено не числовое значение. Пожалуйста, повторите ввод.\033[0m\n");
                continue;
			}

            if (!is_valid_num(taxes))                                               // проверка на слишком большое число
            {
                printf("\033[1;31m Число должно быть в пределе от 0 до 1000000. Пожалуйста, повторите ввод.\033[0m\n");
                continue;
            }

            if (len < 7)                                                            // Заполнение оставшегося места пробелами для выравнивания
			{
                for (int j = len; j < 7; j++) 
				{
                    *(taxes + j) = ' ';
                }
                *(taxes + 7) = '\0';
            }

            strcpy((*(list_of_firms + i)).taxes, taxes);                            // копирование буфера в поле структуры
            valid = 1;                                                              // Ввод корректен, выход из цикла
        }
    }
}

void input_firm_dates(firm_info* list_of_firms, int number_of_firms)                // функция ввода налогов
{
    char date[32];                                                                  // буфер для ввода строки
	int c;                                                                          // переменная для очистки буфера ввода

    for (int i = 0; i < number_of_firms; i++)                                       // цикл по фирмам
    {
        int valid_deadline = 0;                                                     // переменная для проверки ввода
        while (!valid_deadline)                                                     // цикл ввода и проверки
        {
            printf("Введите дату последнего срока внесения налога для фирмы %s (месяц): ", (*(list_of_firms + i)).name);
            if (fgets(date, sizeof(date), stdin) == NULL)                           // ввод строки
            {
                printf("\033[1;31m Ошибка ввода. Пожалуйста, попробуйте ещё раз.\033[0m\n");
                continue;
            }

            *(date + strcspn(date, "\n")) = '\0';                                   // Удаление символа новой строки, если он есть

            int char_count = 0;                                                     // счётчик символов строки
            char* p = date;                                                         // указатель на символ строки
            while (*p)                                                              // цикл по символам строки
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

			int date_len = strlen(date);                                            // получение длины строки

            if (char_count == -1)                                                   // проверка на неверную кодировку символов
                continue;

            if (char_count == 0)                                                    // проверка на пустую строку
            {
                printf("\033[1;31m Дата не может быть пустой. Попробуйте ещё раз.\033[0m\n");
                continue;
            }

            if (char_count > 10)                                                    // проверка на слишком большую строку
            {
                printf("\033[1;31m Введено слишком много символов. Пожалуйста, введите дату в формате месяц (не более 10 символов).\033[0m\n");
                continue;
            }
			else if (date_len >= 21)
				while ((c = getchar()) != '\n' && c != EOF);                        // очистка буфера ввода

            if (!is_valid_date(date))                                               // проверка на правильную дату
            {
                printf("\033[1;31m Введено некорректное название месяца или 0. Пожалуйста, повторите ввод.\033[0m\n");
                continue;
            }

            if (char_count < 10)                                                    // Заполнение оставшегося места пробелами
            {
                for (int j = 0; j < (10 - char_count); j++) 
                {
                    *(date + date_len++) = ' ';
                }
                *(date + date_len) = '\0';
            }

            strncpy((*(list_of_firms + i)).taxes_deadline, date,\
            sizeof((*(list_of_firms + i)).taxes_deadline));                         // копирование буфера в поле структуры
            valid_deadline = 1;
        }

        // Ввод даты фактического внесения налога
        int valid_payed = 0;                                                        // переменная для проверки ввода
        while (!valid_payed)                                                        // цикл ввода и проверки
        {
            printf("Введите дату фактического внесения налога для фирмы %s (месяц или 0, если не внесён): ", (*(list_of_firms + i)).name);
            if (fgets(date, sizeof(date), stdin) == NULL)                           // ввод строки
            {
                printf("\033[1;31m Ошибка ввода. Пожалуйста, попробуйте ещё раз.\033[0m\n");
                continue;
            }

            *(date + strcspn(date, "\n")) = '\0';                                   // Удаление символа новой строки, если он есть

            int char_count = 0;                                                     // счётчик UTF-8 символов в строке
            char* p = date;                                                         // указатель на символ строки
            while (*p)                                                              // цикл по строке
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

			int date_len = strlen(date);                                            // получение длины строки

            if (char_count == -1)                                                   // проверка на неправильную кодировку
                continue;

            if (char_count == 0)                                                    // проверка на пустую строку
            {
                printf("\033[1;31m Дата не может быть пустой. Попробуйте ещё раз.\033[0m\n");
                continue;
            }

            if (char_count > 10)                                                    // проверка на слишком длинную строку
            {
                printf("\033[1;31m Введено слишком много символов. Пожалуйста, введите дату в формате месяц (не более 10 символов).\033[0m\n");
                continue;
            }
			else if (date_len >= 21)
				while ((c = getchar()) != '\n' && c != EOF);

            if (!is_valid_date(date))                                               // проверка на правильный формат даты
            {
                printf("\033[1;31m Введено некорректное название месяца или 0. Пожалуйста, повторите ввод.\033[0m\n");
                continue;
            }
            
            if (char_count < 10)                                                    // Заполнение оставшегося места пробелами
            {
                for (int j = 0; j < (10 - char_count); j++) 
                {
                    *(date + date_len++) = ' ';
                }
                *(date + date_len) = '\0';
            }

            strncpy((*(list_of_firms + i)).taxes_payed, date, \
            sizeof((*(list_of_firms + i)).taxes_payed));                            // копирование буфера в поле структуры
            valid_payed = 1;                                                        // ввод корректен, выход из цикла
        }
    }
}
//* ======================== OUTPUT ========================
void output_firm_info(firm_info* list_of_firms, int number_of_firms, int t)         // функция вывода данных о фирмах
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
			printf("| %d | %s |  NULL   |    NULL     |    NULL    |\n", i, (*(list_of_firms + i)).name);
			printf("+-------------------------------------------------------------------------+\n");
			break;
			case 2:
			printf("| %d | %s | %s |    NULL     |    NULL    |\n", i, (*(list_of_firms + i)).name, (*(list_of_firms + i)).taxes);
			printf("+-------------------------------------------------------------------------+\n");
			break;
			case 3:
			printf("| %d | %s | %s | %s  | %s |\n", i, (*(list_of_firms + i)).name, (*(list_of_firms + i)).taxes, (*(list_of_firms + i)).taxes_deadline, (*(list_of_firms + i)).taxes_payed);
			printf("+-------------------------------------------------------------------------+\n");
			break;
		}
	}
}

void output_top_5(firm_info* list_of_firms, int number_of_firms, const char* month)	// функция вывода топ-5 фирм по налогам
{
    int* top_5 = find_top_5(list_of_firms, number_of_firms, month);
	printf("Топ-5 фирм с самыми большими непогашенными налогами в %s:\n", month);
	printf("+-------------------------------------------------------------------------+\n");
	printf("| № |          Название фирмы        | налоги  |   дедлайн   | дата оплат |\n");
	printf("+-------------------------------------------------------------------------+\n");
	for (int i = 0; i < 5; i++) 
	{
		printf("| %d | %s | %s | %s  | %s |\n", i, (*(list_of_firms + *(top_5 + i))).name, (*(list_of_firms + *(top_5 + i))).taxes, (*(list_of_firms + *(top_5 + i))).taxes_deadline, (*(list_of_firms + *(top_5 + i))).taxes_payed);
		printf("+-------------------------------------------------------------------------+\n");
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