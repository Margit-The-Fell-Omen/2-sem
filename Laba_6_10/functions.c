#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"


//* ======================== Menu ===========================
void menu(firm_info** list_of_firms_ptr, int* number_of_firms_ptr) 
{
    // Разыменовываем указатели для удобства работы с переменными
    firm_info* current_list_of_firms = *list_of_firms_ptr;
    int current_number_of_firms = *number_of_firms_ptr;

    const char* month;      // название месяца для поиска топ-5
    char choice_buffer[10]; // Буфер для ввода пользовательского выбора (да/нет)
    int user_choice;        // Переменная для хранения выбора пользователя

    // Освобождаем память от предыдущей итерации, если она была выделена
    if (current_list_of_firms != NULL) {
        firm_info_free(current_list_of_firms);
        current_list_of_firms = NULL;
        current_number_of_firms = 0;
    }

    printf("\nХотите загрузить данные из файла '%s'? (1 - Да, 0 - Нет): ", FILENAME);
    user_choice = 0; // Значение по умолчанию
    if (fgets(choice_buffer, sizeof(choice_buffer), stdin) != NULL) {
        // Очистка буфера stdin, если fgets не считал \n
        if (strchr(choice_buffer, '\n') == NULL) {
            int c_temp;
            while ((c_temp = getchar()) != '\n' && c_temp != EOF);
        }
        sscanf(choice_buffer, "%d", &user_choice);
    } else {
        printf("Ошибка чтения ввода. Попробуйте ввести данные вручную.\n");
        if (feof(stdin)) clearerr(stdin); // Сброс EOF для дальнейшей работы
    }

    if (user_choice == 1) {
        current_list_of_firms = load_firms_from_file(&current_number_of_firms, FILENAME);
        if (current_list_of_firms != NULL && current_number_of_firms > 0) {
            printf("Данные успешно загружены из файла. Загружено %d фирм(ы).\n", current_number_of_firms);
        } else {
            printf("Не удалось загрузить данные из файла, или файл пуст/поврежден.\n");
            printf("Пожалуйста, введите данные вручную.\n");
            if (current_list_of_firms != NULL) { // Если load_firms_from_file вернул что-то (например, при ошибке парсинга)
                firm_info_free(current_list_of_firms);
                current_list_of_firms = NULL;
            }
            current_number_of_firms = 0;
            user_choice = 0; // Указываем, что загрузка не удалась / не выбрана
        }
    }

    // Если данные не были загружены (или пользователь отказался), переходим к ручному вводу
    if (user_choice == 0) {
        input_int_var(&current_number_of_firms, 1, 1, 999);
        current_list_of_firms = firm_info_alloc(current_number_of_firms);
        if (current_list_of_firms == NULL) {
            printf("\033[1;31mКритическая ошибка: не удалось выделить память для %d фирм.\033[0m\n", current_number_of_firms);
            // Обновляем указатели в main перед выходом из menu, если это критическая ошибка
            *list_of_firms_ptr = NULL;
            *number_of_firms_ptr = 0;
            // Здесь можно было бы вернуть код ошибки, чтобы main мог решить, что делать дальше (например, завершить программу)
            return; // Выходим из menu, restart_program будет вызван в main
        }
        input_all_firm_data(current_list_of_firms, current_number_of_firms);
    }

    // Обработка и вывод данных, если они есть
    if (current_list_of_firms != NULL && current_number_of_firms > 0) {
        printf("\n--- Текущие данные о фирмах ---\n");
        output_firm_info(current_list_of_firms, current_number_of_firms, 3);

        printf("\n--- Определение топ-5 фирм по неуплаченным налогам ---\n");
        month = input_month();
        output_top_5(current_list_of_firms, current_number_of_firms, month);

        printf("\nХотите сохранить текущие данные в файл '%s'? (1 - Да, 0 - Нет): ", FILENAME);
        user_choice = 0; // Сброс для нового выбора
        if (fgets(choice_buffer, sizeof(choice_buffer), stdin) != NULL) {
            if (strchr(choice_buffer, '\n') == NULL) {
                int c_temp;
                while ((c_temp = getchar()) != '\n' && c_temp != EOF);
            }
            sscanf(choice_buffer, "%d", &user_choice);
        } else {
            printf("Ошибка чтения ввода для сохранения. Данные не будут сохранены.\n");
            if (feof(stdin)) clearerr(stdin);
        }

        if (user_choice == 1) {
            save_firms_to_file(current_list_of_firms, current_number_of_firms, FILENAME);
        }
    } else {
        printf("\nНет данных для обработки или вывода.\n");
    }

    // Обновляем значения указателей в main перед выходом из функции menu
    *list_of_firms_ptr = current_list_of_firms;
    *number_of_firms_ptr = current_number_of_firms;
}


// Вспомогательная функция для удаления пробелов в конце строки
void trim_trailing_spaces(char* str) 
{
    if (str == NULL) return;
    int i = strlen(str) - 1;
    while (i >= 0 && isspace((unsigned char)str[i])) 
    {
        str[i] = '\0';
        i--;
    }
}

// Вспомогательная функция для подсчета UTF-8 символов (из input_firm_name)
int count_utf8_chars_internal(const char* str) 
{
    int char_count = 0;
    const char* p = str;
    while (*p != '\0') 
    {
        if ((*p & 0x80) == 0) p += 1; // 1-байтовый символ (ASCII)
        else if ((*p & 0xE0) == 0xC0) p += 2; // 2-байтовый символ
        else if ((*p & 0xF0) == 0xE0) p += 3; // 3-байтовый символ
        else if ((*p & 0xF8) == 0xF0) p += 4; // 4-байтовый символ
        else return -1; // Неверная кодировка UTF-8
        char_count++;
    }
    return char_count;
}


//* ======================== Memory ===========================
firm_info* firm_info_alloc(int number_of_firms)     // функция выделения памяти для структуры
{
	firm_info* list_of_firms = (firm_info*)calloc(number_of_firms, sizeof(firm_info));
	if (list_of_firms == NULL) {
		perror("Ошибка выделения памяти для списка фирм");
	}
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
    if (str == NULL || *str == '\0') return 0; // Пустая строка не число
    while (*str)                                    // цикл по элементам строки
    {
        // Разрешаем только цифры. Точка и запятая обрабатываются отдельно или не допускаются здесь,
        // в зависимости от того, как atoi и другие функции их интерпретируют.
        // atoi игнорирует всё после нецифровых символов (кроме начальных пробелов).
        // Для простоты, эта функция ожидает строку, состоящую только из цифр после trim.
        if (!isdigit(*str)) return 0;
        str++;
    }
    return 1;
}

int is_valid_num(const char* str)                   // функция проверки числа
{
    // atoi корректно обработает строку с числом, даже если там есть пробелы до или цифры после.
    // Например, atoi("  123  ") -> 123. atoi("123xyz") -> 123.
    // Важно, чтобы is_numeric уже проверила, что там только цифры (если это требование).
    // Однако, существующая is_numeric не пропускает пробелы.
    // Для чисел типа "123.45" is_numeric вернет 0. atoi("123.45") вернет 123.
    // В данном контексте налоги - целые числа.
    long long num_check = atoll(str); // Используем atoll для проверки более широкого диапазона перед преобразованием в int
    if (num_check > 1000000 || num_check < 0) return 0;

    int num = atoi(str);
    // Дополнительная проверка, если atoi вернула 0, но строка не "0"
    if (num == 0 && strcmp(str, "0") != 0 && strcmp(str, "0 ") !=0) { // strcmp(str, "0 ") для случая с одним пробелом после trim
        // Если строка не была "0", но atoi вернула 0, это может быть ошибка или нечисловое значение.
        // Но is_numeric должна была это отловить.
        // Эта проверка больше для чисел, которые могут быть невалидны для atoi, но пройдут is_numeric (маловероятно с текущей is_numeric).
    }
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
        "Июль", "Август", "Сентябрь", "Октябрь", "Ноябрь", "Декобрь" // Декабрь с ошибкой в оригинале "Декобрь"
    };
    // Исправим "Декобрь" на "Декабрь" для корректности, если это опечатка
    const char* ru_months_corrected[12] =
    {
        "Январь", "Февраль", "Март", "Апрель", "Май", "Июнь",
        "Июль", "Август", "Сентябрь", "Октябрь", "Ноябрь", "Декабрь"
    };


    int count = 0;									// счётчик совпадений

    // Проверка "0" отдельно, так как он есть только в en_months[0]
    if (str_compare(str, en_months[0])) return 1;

    for (int i = 0; i < 12; i++)					// цикл по месяцам (en_months[0] уже проверен)
    {
        if (str_compare(en_months[i+1], str) || str_compare(str, ru_months_corrected[i])) count++;
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
    // Создадим временную копию строки для trim_trailing_spaces, если нужно
    char temp_str[MAX_DATE_SIZE]; // Предполагаем DATE_FIELD_SIZE из header.h
    strncpy(temp_str, str, sizeof(temp_str) - 1);
    temp_str[sizeof(temp_str) - 1] = '\0';
    trim_trailing_spaces(temp_str); // Удаляем пробелы, чтобы сравнение было точным

    const char* en_months[12] =						// английские месяцы
    { 
        "January", "February", "March", "April", "May", "June",
        "July", "August", "September", "October", "November", "December" 
    };
    const char* ru_months[12] = 					// русские месяцы
    { 
        "Январь", "Февраль", "Март", "Апрель", "Май", "Июнь",
        "Июль", "Август", "Сентябрь", "Октябрь", "Ноябрь", "Декабрь" // Исправлено "Декобрь"
    };

    if (str_compare(temp_str, "0")) return -1; // Специальное значение для "0", чтобы отличать от января (индекс 0)

    for (int i = 0; i < 12; i++)					// цикл по месяцам
    { 
        if (str_compare(temp_str, en_months[i]) || str_compare(temp_str, ru_months[i])) return i; 
    } 
    // Если месяц не найден (например, пустая строка после trim или невалидное имя),
    // можно вернуть специальное значение, например, -2 или продолжить считать его как "не оплачено вовремя".
    // Оригинальная функция возвращала 0 (Январь) если ничего не найдено. Это может быть нежелательно.
    // Для сохранения принципа работы, если месяц не распознан, но не "0", вернем большое число, чтобы он считался "позже" любого месяца.
    // Или, если требуется строгое соответствие, нужно решить, как обрабатывать нераспознанные месяцы.
    // В контексте find_top_5, если what_month(month) вернет большое значение для невалидного месяца,
    // то условие `what_month((*(list_of_firms + j)).taxes_payed) < what_month(month)` будет чаще истинным.
    // Безопаснее вернуть значение, которое не пройдет сравнение, если месяц не распознан.
    // Например, -2 (если -1 уже занят под "0").
    // Исходная функция возвращала 0 (Январь) при неудаче. Оставим так для совместимости, но это потенциальная проблема.
    return 12; // Вернем значение больше любого месяца (0-11), чтобы просроченные платежи с неверной датой считались просроченными.
               // Или можно вернуть -1, как для "0", если это означает "не оплачено".
               // Оригинальная логика find_top_5: `what_month(payed) < what_month(target_month)`
               // Если `taxes_payed` это "0", `what_month` вернет -1.
               // Если `taxes_payed` это "НевалидныйМесяц", оригинальный `what_month` вернет 0 (Январь).
               // Это означает, что невалидный месяц будет считаться как Январь.
               // Для find_top_5: ищем фирмы, где `what_month(payed) < what_month(current_search_month)`.
               // Если `payed` = "0" (не оплачено), `what_month` -> -1. Это всегда `< what_month(current_search_month)` (если месяц не "0").
}


int* find_top_5(firm_info* list_of_firms, int number_of_firms, const char* month) 			// функция нахождения топ-5 фирм по налогам
{
    int* list_of_taxes = (int*)malloc(number_of_firms * sizeof(int));						// массив налогов фирм
    if (list_of_taxes == NULL) {
        perror("Ошибка выделения памяти для списка налогов в find_top_5");
        return NULL;
    }
    int* top_5 = (int*)calloc(5, sizeof(int));												// массив индексов топ-5 фирм
    if (top_5 == NULL) {
        perror("Ошибка выделения памяти для top_5 в find_top_5");
        free(list_of_taxes);
        return NULL;
    }
    for(int i=0; i<5; ++i) top_5[i] = -1; // Инициализируем индексы как -1 (не найдено)


    int count = 0;																			// счётчик для прохода по top_5
    char temp_taxes_str[MAX_TAX_SIZE]; // Для безопасного использования atoi

    for (int i = 0; i < number_of_firms; i++)												// преобразование налогов в целые числа
	{
        strncpy(temp_taxes_str, (*(list_of_firms + i)).taxes, MAX_TAX_SIZE -1);
        temp_taxes_str[MAX_TAX_SIZE-1] = '\0';
        trim_trailing_spaces(temp_taxes_str); // atoi лучше работает с чистыми числами
        *(list_of_taxes + i) = atoi(temp_taxes_str);
    }

    qsort(list_of_taxes, number_of_firms, sizeof(int), compare);							// сортировка налогов по убыванию

    // Логика для выбора уникальных фирм для топ-5
    // Необходимо избегать добавления одной и той же фирмы несколько раз, если у них одинаковые налоги
    int* used_firm_indices = (int*)calloc(number_of_firms, sizeof(int));
    if (!used_firm_indices) {
        perror("Ошибка выделения памяти для used_firm_indices");
        free(list_of_taxes);
        free(top_5);
        return NULL;
    }


    int target_month_numeric = what_month(month);

    for (int i = 0; i < number_of_firms && count < 5; i++)									// цикл по отсортированным налогам
	{
        for (int j = 0; j < number_of_firms; j++) 											// цикл по фирмам
		{
            if (used_firm_indices[j]) continue; // Эту фирму уже добавили

            strncpy(temp_taxes_str, (*(list_of_firms + j)).taxes, MAX_TAX_SIZE-1);
            temp_taxes_str[MAX_TAX_SIZE-1] = '\0';
            trim_trailing_spaces(temp_taxes_str);
            int current_firm_tax = atoi(temp_taxes_str);

            if (*(list_of_taxes + i) == current_firm_tax)
			{
                // Проверка условия оплаты: налог не погашен до указанного месяца 'month'
                // what_month((*(list_of_firms + j)).taxes_payed) < what_month(month)
                // Это означает: месяц оплаты < целевого месяца.
                // Если налог не оплачен ("0"), what_month вернет -1, что всегда меньше любого месяца.
                // Если оплачен в срок или позже, условие не выполнится.
                // Нам нужны те, кто НЕ ОПЛАТИЛ ВОВРЕМЯ.
                // Т.е. дата оплаты >= целевого месяца ИЛИ дата оплаты == "0" (не оплачено)
                int payed_month_numeric = what_month((*(list_of_firms + j)).taxes_payed);

                // Условие: налог считается непогашенным, если:
                // 1. Дата оплаты "0" (т.е. payed_month_numeric == -1)
                // 2. Дата оплаты >= целевого месяца (т.е. оплатили в целевом месяце или позже, что считается просрочкой для предыдущих)
                // Оригинальное условие: `what_month(payed) < what_month(month)` выбирало тех, кто оплатил ДОсрочно.
                // Для "непогашенных на момент месяца month" нужно:
                // payed_month_numeric == -1 (не оплачено) ИЛИ payed_month_numeric >= target_month_numeric
                // Однако, если мы ищем "непогашенные НАЛОГИ В month", это значит, что дедлайн был ДО month, а оплата не произведена или произведена в month или позже.
                // Логика find_top_5 ищет "фирмы с самыми большими НЕПОГАШЕННЫМИ налогами в month".
                // Это означает, что к началу `month` налог еще не был оплачен, или оплачен в `month` или позже.
                // То есть, `taxes_payed` это "0" ИЛИ `what_month(taxes_payed) >= what_month(month)`.

                // Корректное условие для "непогашенных к месяцу `month`":
                // Либо не оплачено (payed_month_numeric == -1),
                // Либо оплачено в `month` или позже (payed_month_numeric >= target_month_numeric).
                // При этом, сам `month` (целевой) должен быть после дедлайна.
                // int deadline_month_numeric = what_month((*(list_of_firms + j)).taxes_deadline);
                // if (target_month_numeric > deadline_month_numeric) { // Проверяем только если месяц `month` уже после дедлайна

                if (payed_month_numeric == -1 || payed_month_numeric >= target_month_numeric) {
                    *(top_5 + count) = j;														// запись индекса топ-5 фирмы в top_5
                    used_firm_indices[j] = 1; // Помечаем фирму как использованную
                    count++;
                    if (count == 5) break; // Набрали топ-5 по этому уровню налога
                }
            }
        }
    }
    sort_top_5_by_name(list_of_firms, top_5, count); // Передаем count, т.к. фирм может быть < 5
    free(list_of_taxes);
    free(used_firm_indices);
    return top_5;
}


void sort_top_5_by_name(firm_info* list_of_firms, int* top_5, int count) // Принимаем count
{
    // Сортируем только действительные записи в top_5 (индексы которых не -1)
    // Сначала соберем действительные индексы, если их меньше 5
    int actual_count = 0;
    for(int i=0; i<count; ++i) {
        if(top_5[i] != -1) actual_count++;
        else break; // Если встретили -1, дальше нет валидных индексов (после инициализации и заполнения)
    }
    if (actual_count <=1) return; // Нечего сортировать

    for (int i = 0; i < actual_count - 1; i++)                                                             // сортировка пузырьком
    {
        for (int j = 0; j < actual_count - 1 - i; j++) 
        {
            // Сравниваем имена фирм по их индексам в top_5
            // Нужны фактические имена для strcmp, а не указатели на char в структуре напрямую, если они не null-terminated правильно
            // Но предполагается, что list_of_firms[index].name это корректная C-строка.
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
			printf("Введите количество фирм(целое положительное десятичное число, не более 999): \n");
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
			check = 0; // scanf вернул 1, но значение невалидно
		}
		else if (*a >= 1000) // Ограничение изменено в сообщении case 1
		{
			printf("\033[1;31m Ошибка ввода: Слишком большое число (макс 999), попробуйте ввести еще раз.\033[0m\n");		// вывод сообщения об ошибке
			check = 0;
		}
        
        if (check == 1 && min != 0 && max != 0) // Проверяем диапазон только если scanf успешен и диапазон задан
        {
            if (*a < min || *a > max) 
            {
                printf("\033[1;31m Ошибка ввода: Число не в пределе %d-%d, попробуйте ввести еще раз.\033[0m\n", min, max);// вывод сообщения об ошибке
                check = 0;
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
    // const char* ru_months[12] = ... // Не используется для возврата значения в этой функции
    int choice = 0;														// переменная выбора номера месяца
    input_int_var(&choice, 2, 1, 12);									// ввод номера месяца
    return en_months[choice - 1];
}

void input_firm_name(firm_info* list_of_firms, int firm_index) // Параметр i переименован в firm_index для ясности
{
    // Буфер для ввода строки. Используем MAX_NAME_SIZE из header.h
    char name_buffer[MAX_NAME_SIZE];
    char c; // переменная для очистки буфера ввода

    int valid = 0; // переменная для проверки ввода
    while (!valid) // цикл ввода и проверки для ОДНОЙ фирмы с индексом firm_index
    {
        printf("Введите название %d-й фирмы (не более 30 UTF-8 символов): ", firm_index + 1);
        if (fgets(name_buffer, sizeof(name_buffer), stdin) == NULL) // ввод строки
        {
            printf("\033[1;31m Ошибка ввода (fgets вернул NULL). Пожалуйста, попробуйте ещё раз.\033[0m\n");
            if (feof(stdin)) {
                clearerr(stdin);
                printf("\033[1;33m Обнаружен конец файла. Ввод прерван.\033[0m\n");
                return; 
            }
            if (ferror(stdin)) {
                clearerr(stdin);
                printf("\033[1;31m Ошибка потока ввода. Пожалуйста, попробуйте ещё раз.\033[0m\n");
            }
            continue;
        }

        // Проверка, была ли считана вся строка или буфер переполнился
        if (strchr(name_buffer, '\n') == NULL && strlen(name_buffer) == sizeof(name_buffer) - 1) {
            // printf("\033[1;33m Введено слишком много символов (превышен размер буфера %zu). Очистка буфера...\033[0m\n", sizeof(name_buffer) -1);
            while ((c = getchar()) != '\n' && c != EOF); // Очищаем остаток строки из stdin
        }
        
        name_buffer[strcspn(name_buffer, "\n")] = '\0'; // Удаление символа новой строки, если он есть

        // === Начало встроенной логики подсчета UTF-8 символов ===
        int char_count = 0;             // счётчик длины UTF-8 строки
        char* p_utf = name_buffer;      // указатель для итерации по строке name_buffer
        while (*p_utf != '\0')          // Вычисление количества UTF-8 символов
        {
            if ((*p_utf & 0x80) == 0)
                p_utf += 1; // 1-байтовый символ (ASCII)
            else if ((*p_utf & 0xE0) == 0xC0)
                p_utf += 2; // 2-байтовый символ
            else if ((*p_utf & 0xF0) == 0xE0)
                p_utf += 3; // 3-байтовый символ
            else if ((*p_utf & 0xF8) == 0xF0)
                p_utf += 4; // 4-байтовый символ
            else 
            {
                printf("\033[1;31m Неверная кодировка символов. Пожалуйста, используйте UTF-8.\033[0m\n");
                char_count = -1; // Признак ошибки
                break;
            }
            char_count++;
        }
        // === Конец встроенной логики подсчета UTF-8 символов ===

        if (char_count == -1) // проверка на неверную кодировку
        {
            continue;
        }
			
        int name_len_bytes = strlen(name_buffer); // получение длины строки в байтах	

        if (char_count == 0) // проверка на пустую строку
        {
            printf("\033[1;31m Название не может быть пустым. Пожалуйста, попробуйте ещё раз.\033[0m\n");
            continue;
        }

        if (char_count > 30) // проверка на слишком большую строку (в UTF-8 символах)
        {   
            printf("\033[1;31mНазвание слишком длинное (введено %d UTF-8 символов, макс. 30). Пожалуйста, введите название не более 30 символов.\033[0m\n", char_count);
            continue;
        }
        // Оригинальная строка: else if (name_len >= 61) while ((c = getchar()) != '\n' && c != EOF);
        // Эта логика очистки буфера теперь покрывается более общей проверкой после fgets,
        // если строка была слишком длинной для name_buffer.
        // Если name_len_bytes (после удаления \n) было бы >= (MAX_NAME_SIZE - некоторая дельта),
        // это значит, что fgets, возможно, не считал \n, и буфер был очищен.
        // Поэтому этот специфичный `else if` для 61 символа здесь не нужен в прежнем виде.

        if (char_count < 30) // Заполнение оставшегося места пробелами для выравнивания
        {
            int spaces_to_add = 30 - char_count;
            for (int j = 0; j < spaces_to_add; j++) 
            {
                if (name_len_bytes < sizeof(name_buffer) - 1) { // Проверка на переполнение буфера name_buffer
                    name_buffer[name_len_bytes++] = ' ';
                } else {
                    // printf("\033[1;31m Ошибка: недостаточно места в буфере name_buffer для добавления пробелов выравнивания.\033[0m\n");
                    break; 
                }
            }
            name_buffer[name_len_bytes] = '\0'; // Добавление завершающего нулевого символа
        }

        // Копирование буфера в поле структуры
        strncpy(list_of_firms[firm_index].name, name_buffer, MAX_NAME_SIZE - 1);
        list_of_firms[firm_index].name[MAX_NAME_SIZE - 1] = '\0'; // Гарантируем null-termination
        
        valid = 1; // Ввод корректен, выход из цикла
    }
}
void input_firm_taxes(firm_info* list_of_firms, int i) // Изменено: принимает индекс
{					
    char taxes_buffer[MAX_TAX_SIZE]; 																	
	int c;																			

    int valid = 0;																
    while (!valid) 																
	{
        // Для вывода имени фирмы, оно должно быть уже введено.
        // Если это первый ввод налогов, имя может быть еще не установлено или быть мусором.
        // Лучше передавать имя фирмы как const char* или убедиться, что оно уже корректно.
        // В данном случае, предполагаем, что имя уже в list_of_firms[i].name
        printf("Введите величину налогообложения фирмы %s в рублях (не более 1000000): ", (*(list_of_firms + i)).name);
        if (fgets(taxes_buffer, sizeof(taxes_buffer), stdin) == NULL) 						
		{
            printf("\033[1;31m Ошибка ввода. Пожалуйста, попробуйте ещё раз.\033[0m\n");
            if (feof(stdin)) clearerr(stdin);
            if (ferror(stdin)) clearerr(stdin);
            continue;
        }

        unsigned int len = strlen(taxes_buffer);                                       
        int newline_found = 0;
        if (len > 0 && taxes_buffer[len-1] == '\n') {
            taxes_buffer[len-1] = '\0';
            len--;
            newline_found = 1;
        } else if (len == sizeof(taxes_buffer) - 1) { // Буфер полон, '\n' не считан
            // Очищаем остаток строки из stdin
            // printf("DEBUG: Clearing stdin buffer for taxes input.\n");
            while ((c = getchar()) != '\n' && c != EOF);
        }


        if (len == 0)                                                           
		{
            printf("\033[1;31m Величина налогообложения не может быть пустой (введите 0, если её нет). Попробуйте ещё раз.\033[0m\n");
            continue;
        }
        
        // Создаем копию для is_numeric и is_valid_num, так как они могут ожидать строку без паддинга
        char temp_taxes_for_validation[MAX_TAX_SIZE];
        strncpy(temp_taxes_for_validation, taxes_buffer, sizeof(temp_taxes_for_validation)-1);
        temp_taxes_for_validation[sizeof(temp_taxes_for_validation)-1] = '\0';
        // trim_trailing_spaces(temp_taxes_for_validation); // is_numeric ожидает чистые цифры

		if (!is_numeric(temp_taxes_for_validation)) // is_numeric должна работать с непаддированной строкой
		{
            printf("\033[1;31m Введено не числовое значение. Пожалуйста, повторите ввод.\033[0m\n");
            continue;
		}

        if (!is_valid_num(temp_taxes_for_validation)) // is_valid_num также с непаддированной
        {
            printf("\033[1;31m Число должно быть в пределе от 0 до 1000000. Пожалуйста, повторите ввод.\033[0m\n");
            continue;
        }

        // Паддинг до 7 символов
        int current_byte_len = strlen(taxes_buffer);
        if (current_byte_len < 7)                                                            
		{
            for (int j = current_byte_len; j < 7; j++) 
			{
                if (j < sizeof(taxes_buffer) -1) {
                    taxes_buffer[j] = ' ';
                } else break;
            }
            if (7 < sizeof(taxes_buffer)) taxes_buffer[7] = '\0';
            else taxes_buffer[sizeof(taxes_buffer)-1] = '\0';
        } else { // Если длина уже 7 или больше, просто обрезаем до 7 и ставим \0
            if (7 < sizeof(taxes_buffer)) taxes_buffer[7] = '\0';
            else taxes_buffer[sizeof(taxes_buffer)-1] = '\0';
        }


        strncpy((*(list_of_firms + i)).taxes, taxes_buffer, sizeof((*(list_of_firms + i)).taxes) -1);
        (*(list_of_firms + i)).taxes[sizeof((*(list_of_firms + i)).taxes) -1] = '\0';
        valid = 1;                                                              
    }
}

void input_firm_dates(firm_info* list_of_firms, int i) // Изменено: принимает индекс
{
    char date_buffer[MAX_DATE_SIZE];                                                                  
	int c;                                                                          

    // Ввод даты последнего срока внесения налога
    int valid_deadline = 0;                                                     
    while (!valid_deadline)                                                     
    {
        printf("Введите дату последнего срока внесения налога для фирмы %s (месяц): ", (*(list_of_firms + i)).name);
        if (fgets(date_buffer, sizeof(date_buffer), stdin) == NULL)                           
        {
            printf("\033[1;31m Ошибка ввода. Пожалуйста, попробуйте ещё раз.\033[0m\n");
            if (feof(stdin)) clearerr(stdin);
            if (ferror(stdin)) clearerr(stdin);
            continue;
        }

        unsigned int len = strlen(date_buffer);
        if (len > 0 && date_buffer[len-1] == '\n') {
            date_buffer[len-1] = '\0';
            len--;
        } else if (len == sizeof(date_buffer) - 1) {
            // printf("DEBUG: Clearing stdin buffer for deadline date input.\n");
            while ((c = getchar()) != '\n' && c != EOF);
        }

        int char_count = count_utf8_chars_internal(date_buffer);
        int date_len_bytes = strlen(date_buffer); // Длина в байтах после удаления \n

        if (char_count == -1) {
            printf("\033[1;31m Неверная кодировка символов. Пожалуйста, используйте UTF-8.\033[0m\n");
            continue;
        }
        if (char_count == 0) {
            printf("\033[1;31m Дата не может быть пустой. Попробуйте ещё раз.\033[0m\n");
            continue;
        }
        if (char_count > 10) { // Макс 10 UTF-8 символов для месяца
            printf("\033[1;31m Введено слишком много символов (макс 10). Пожалуйста, введите дату в формате месяц.\033[0m\n");
            continue;
        }
        
        char temp_date_for_validation[MAX_DATE_SIZE];
        strncpy(temp_date_for_validation, date_buffer, sizeof(temp_date_for_validation)-1);
        temp_date_for_validation[sizeof(temp_date_for_validation)-1] = '\0';
        // trim_trailing_spaces(temp_date_for_validation); // is_valid_date ожидает точное совпадение

        if (!is_valid_date(temp_date_for_validation)) { // is_valid_date должна работать с непаддированной строкой
            printf("\033[1;31m Введено некорректное название месяца или 0. Пожалуйста, повторите ввод.\033[0m\n");
            continue;
        }
        
        // Паддинг до 10 UTF-8 символов (или байт, если все ASCII)
        // Паддинг добавляет пробелы. Общая длина в байтах может увеличиться.
        int current_byte_len = strlen(date_buffer);
        if (char_count < 10) {
            for (int j = 0; j < (10 - char_count); j++) {
                if (current_byte_len < sizeof(date_buffer) - 1) {
                    date_buffer[current_byte_len++] = ' ';
                } else break;
            }
            date_buffer[current_byte_len] = '\0';
        }
        // Если char_count >= 10, строка уже достаточно длинная (или слишком, но это отсеялось выше)
        // Убедимся, что строка не превышает буфер после паддинга и null-terminated
        date_buffer[sizeof(date_buffer)-1] = '\0';


        strncpy((*(list_of_firms + i)).taxes_deadline, date_buffer, sizeof((*(list_of_firms + i)).taxes_deadline) -1);
        (*(list_of_firms + i)).taxes_deadline[sizeof((*(list_of_firms + i)).taxes_deadline) -1] = '\0';
        valid_deadline = 1;
    }

    // Ввод даты фактического внесения налога
    int valid_payed = 0;                                                        
    while (!valid_payed)                                                        
    {
        printf("Введите дату фактического внесения налога для фирмы %s (месяц или 0, если не внесён): ", (*(list_of_firms + i)).name);
        if (fgets(date_buffer, sizeof(date_buffer), stdin) == NULL)                           
        {
            printf("\033[1;31m Ошибка ввода. Пожалуйста, попробуйте ещё раз.\033[0m\n");
            if (feof(stdin)) clearerr(stdin);
            if (ferror(stdin)) clearerr(stdin);
            continue;
        }
        
        unsigned int len = strlen(date_buffer);
        if (len > 0 && date_buffer[len-1] == '\n') {
            date_buffer[len-1] = '\0';
            len--;
        } else if (len == sizeof(date_buffer) - 1) {
            // printf("DEBUG: Clearing stdin buffer for payed date input.\n");
            while ((c = getchar()) != '\n' && c != EOF);
        }

        int char_count = count_utf8_chars_internal(date_buffer);
        int date_len_bytes = strlen(date_buffer);

        if (char_count == -1) {
            printf("\033[1;31m Неверная кодировка символов. Пожалуйста, используйте UTF-8.\033[0m\n");
            continue;
        }
        if (char_count == 0) {
            printf("\033[1;31m Дата не может быть пустой. Попробуйте ещё раз.\033[0m\n");
            continue;
        }
        if (char_count > 10) {
            printf("\033[1;31m Введено слишком много символов (макс 10). Пожалуйста, введите дату в формате месяц.\033[0m\n");
            continue;
        }
        
        char temp_date_for_validation[MAX_DATE_SIZE];
        strncpy(temp_date_for_validation, date_buffer, sizeof(temp_date_for_validation)-1);
        temp_date_for_validation[sizeof(temp_date_for_validation)-1] = '\0';
        // trim_trailing_spaces(temp_date_for_validation); // is_valid_date ожидает точное совпадение

        if (!is_valid_date(temp_date_for_validation)) {
            printf("\033[1;31m Введено некорректное название месяца или 0. Пожалуйста, повторите ввод.\033[0m\n");
            continue;
        }
        
        int current_byte_len = strlen(date_buffer);
        if (char_count < 10) {
            for (int j = 0; j < (10 - char_count); j++) {
                 if (current_byte_len < sizeof(date_buffer) - 1) {
                    date_buffer[current_byte_len++] = ' ';
                } else break;
            }
            date_buffer[current_byte_len] = '\0';
        }
        date_buffer[sizeof(date_buffer)-1] = '\0';


        strncpy((*(list_of_firms + i)).taxes_payed, date_buffer, sizeof((*(list_of_firms + i)).taxes_payed) -1);
        (*(list_of_firms + i)).taxes_payed[sizeof((*(list_of_firms + i)).taxes_payed) -1] = '\0';
        valid_payed = 1;                                                        
    }
}

void input_all_firm_data(firm_info* list_of_firms, int number_of_firms) {
    for (int i = 0; i < number_of_firms; i++) {
        printf("\nВвод данных для фирмы %d из %d:\n", i + 1, number_of_firms);
        input_firm_name(list_of_firms, i);
        input_firm_taxes(list_of_firms, i);
        input_firm_dates(list_of_firms, i);
    }
}


//* ======================== OUTPUT ========================
void output_firm_info(firm_info* list_of_firms, int number_of_firms, int t)         // функция вывода данных о фирмах
{
    // Вывод полных данных всегда (t=3), если не указано иное или если t невалидно.
    // Оригинальная функция имела разные форматы вывода в зависимости от t.
    // Для простоты и полноты, будем выводить все данные.
    // Если нужно сохранить поведение с t, можно раскомментировать switch.
	printf("Данные о фирмах:\n");
	printf("+------------------------------------------------------------------------------------+\n");
	printf("| № |          Название фирмы        |   налоги   |   дедлайн   |     дата оплаты    |\n"); // Скорректировал ширину
	printf("+------------------------------------------------------------------------------------+\n");
	for (int i = 0; i < number_of_firms; i++) 
	{
        // Используем %-Ns для выравнивания строк с пробелами, где N - ширина поля.
        // Предполагаем, что строки в структуре уже паддированы пробелами до нужной длины отображения.
        // name: 30, taxes: 7, deadline: 10, payed: 10 (символов отображения)
        // Однако, printf %s выведет до первого \0. Паддинг должен быть частью строки.
        // Размеры полей в printf: name (30), taxes (8), deadline (11), payed (11) - с учетом пробелов вокруг
		printf("| %-2d| %-30.30s | %-8.8s | %-11.11s | %-18.18s |\n", // Увеличил поле для даты оплаты
            i + 1, 
            (*(list_of_firms + i)).name, 
            (*(list_of_firms + i)).taxes, 
            (*(list_of_firms + i)).taxes_deadline, 
            (*(list_of_firms + i)).taxes_payed);
		printf("+------------------------------------------------------------------------------------+\n");
		/* // Оригинальный switch, если нужно сохранить его логику
		switch (t) 
		{
			case 1: // Только имя
			    printf("| %-2d| %-30.30s | %-8s | %-11s | %-18s |\n", i + 1, (*(list_of_firms + i)).name, "NULL", "NULL", "NULL");
			    break;
			case 2: // Имя и налоги
			    printf("| %-2d| %-30.30s | %-8.8s | %-11s | %-18s |\n", i + 1, (*(list_of_firms + i)).name, (*(list_of_firms + i)).taxes, "NULL", "NULL");
			    break;
			case 3: // Все данные
			default: // По умолчанию выводим все
			    printf("| %-2d| %-30.30s | %-8.8s | %-11.11s | %-18.18s |\n", 
                    i + 1, 
                    (*(list_of_firms + i)).name, 
                    (*(list_of_firms + i)).taxes, 
                    (*(list_of_firms + i)).taxes_deadline, 
                    (*(list_of_firms + i)).taxes_payed);
			    break;
		}
        printf("+------------------------------------------------------------------------------------+\n");
        */
	}
}


void output_top_5(firm_info* list_of_firms, int number_of_firms, const char* month)	// функция вывода топ-5 фирм по налогам
{
    if (number_of_firms == 0) {
        printf("Нет данных о фирмах для отображения топ-5.\n");
        return;
    }
    int* top_5_indices = find_top_5(list_of_firms, number_of_firms, month);
    if (top_5_indices == NULL) {
        printf("Не удалось сформировать топ-5 (возможно, ошибка памяти или нет подходящих фирм).\n");
        return;
    }

    char temp_month_str[MAX_DATE_SIZE];
    strncpy(temp_month_str, month, sizeof(temp_month_str)-1);
    temp_month_str[sizeof(temp_month_str)-1] = '\0';
    trim_trailing_spaces(temp_month_str);


	printf("Топ-5 фирм с самыми большими непогашенными налогами в %s:\n", temp_month_str);
	printf("+------------------------------------------------------------------------------------+\n");
	printf("| № |          Название фирмы        |   налоги   |   дедлайн   |     дата оплаты    |\n");
	printf("+------------------------------------------------------------------------------------+\n");
    int displayed_count = 0;
	for (int i = 0; i < 5; i++) 
	{
        if (*(top_5_indices + i) == -1) break; // Если индекс -1, значит фирм в топе меньше 5
        int firm_idx = *(top_5_indices + i);
		printf("| %-2d| %-30.30s | %-8.8s | %-11.11s | %-18.18s |\n", 
            i + 1, 
            (*(list_of_firms + firm_idx)).name, 
            (*(list_of_firms + firm_idx)).taxes, 
            (*(list_of_firms + firm_idx)).taxes_deadline, 
            (*(list_of_firms + firm_idx)).taxes_payed);
		printf("+------------------------------------------------------------------------------------+\n");
        displayed_count++;
	}
    if (displayed_count == 0) {
        printf("| Нет фирм, соответствующих критериям для отображения в топ-5.                     |\n");
        printf("+------------------------------------------------------------------------------------+\n");
    }
    free(top_5_indices);
}

//* =================== File Operations =====================

// Функция сохранения данных фирм в текстовый файл
int save_firms_to_file(firm_info* list_of_firms, int number_of_firms, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        perror("Ошибка открытия файла для записи");
        return 0; // Неудача
    }

    // Опционально: записать количество фирм в первую строку
    // fprintf(file, "%d\n", number_of_firms);

    for (int i = 0; i < number_of_firms; i++) {
        // Записываем поля как есть (они уже паддированы и null-terminated в структуре)
        // Используем "|" как разделитель. Убедимся, что имена и даты не содержат "|"
        // (стандартные имена месяцев и цифры не содержат).
        // Важно: fprintf %s выведет строку до первого \0.
        // Если поля содержат пробелы для выравнивания, они будут сохранены.
        fprintf(file, "%s|%s|%s|%s\n",
                list_of_firms[i].name,
                list_of_firms[i].taxes,
                list_of_firms[i].taxes_deadline,
                list_of_firms[i].taxes_payed);
    }

    if (fclose(file) != 0) {
        perror("Ошибка закрытия файла после записи");
        return 0; // Неудача
    }
    printf("Данные успешно сохранены в файл: %s\n", filename);
    return 1; // Успех
}

// Функция загрузки данных фирм из текстового файла
firm_info* load_firms_from_file(int* number_of_firms_ptr, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Ошибка открытия файла для чтения");
        *number_of_firms_ptr = 0;
        return NULL;
    }

    firm_info* list_of_firms = NULL;
    int capacity = 0;
    *number_of_firms_ptr = 0;

    // Буфер для чтения строки. Размер должен быть достаточным.
    // NAME_FIELD_SIZE + TAXES_FIELD_SIZE + 2*DATE_FIELD_SIZE + 3 разделителя + \n + \0
    // Примерно: 64 + 9 + 32 + 32 + 3 + 1 + 1 = 142. Возьмем с запасом.
    char line_buffer[256]; 
    int line_number = 0;

    // Опционально: если первая строка - количество фирм
    // if (fgets(line_buffer, sizeof(line_buffer), file) != NULL) {
    //     int n = atoi(line_buffer);
    //     // Можно предварительно выделить память, если это число надежно
    // } else { /* Файл пуст или ошибка чтения */ }


    while (fgets(line_buffer, sizeof(line_buffer), file) != NULL) {
        line_number++;
        // Удаляем символ новой строки, если он есть
        line_buffer[strcspn(line_buffer, "\n")] = '\0';

        if (*number_of_firms_ptr >= capacity) {
            capacity = (capacity == 0) ? 10 : capacity * 2; // Начальная емкость или удваиваем
            firm_info* temp = (firm_info*)realloc(list_of_firms, capacity * sizeof(firm_info));
            if (temp == NULL) {
                perror("Ошибка перераспределения памяти для списка фирм при загрузке");
                free(list_of_firms); // Освобождаем то, что уже было выделено
                fclose(file);
                *number_of_firms_ptr = 0;
                return NULL;
            }
            list_of_firms = temp;
        }

        firm_info current_firm; // Временная структура для заполнения и валидации

        char* token;
        char* rest = line_buffer;

        // 1. Имя фирмы
        token = strtok_r(rest, "|", &rest);
        if (token == NULL) {
            fprintf(stderr, "Ошибка: неверный формат строки %d в файле (отсутствует имя).\n", line_number);
            // Решение: пропустить строку или прервать загрузку. Прервем для безопасности.
            free(list_of_firms); fclose(file); *number_of_firms_ptr = 0; return NULL;
        }
        // Валидация имени (длина UTF-8, не пустое)
        char temp_name_val[MAX_DATE_SIZE];
        strncpy(temp_name_val, token, sizeof(temp_name_val)-1);
        temp_name_val[sizeof(temp_name_val)-1] = '\0';
        trim_trailing_spaces(temp_name_val); // Для count_utf8_chars и проверки на пустоту
        int name_char_count = count_utf8_chars_internal(temp_name_val);
        if (name_char_count == 0 || name_char_count > 30 || name_char_count == -1) {
            fprintf(stderr, "Ошибка: неверное имя ('%s', %d символов) в строке %d файла.\n", temp_name_val, name_char_count, line_number);
            free(list_of_firms); fclose(file); *number_of_firms_ptr = 0; return NULL;
        }
        strncpy(current_firm.name, token, sizeof(current_firm.name) - 1);
        current_firm.name[sizeof(current_firm.name) - 1] = '\0';


        // 2. Налоги
        token = strtok_r(rest, "|", &rest);
        if (token == NULL) {
            fprintf(stderr, "Ошибка: неверный формат строки %d в файле (отсутствуют налоги).\n", line_number);
            free(list_of_firms); fclose(file); *number_of_firms_ptr = 0; return NULL;
        }
        char temp_taxes_val[MAX_DATE_SIZE];
        strncpy(temp_taxes_val, token, sizeof(temp_taxes_val)-1);
        temp_taxes_val[sizeof(temp_taxes_val)-1] = '\0';
        trim_trailing_spaces(temp_taxes_val); // Для is_numeric и is_valid_num
        if (!is_numeric(temp_taxes_val) || !is_valid_num(temp_taxes_val)) {
            fprintf(stderr, "Ошибка: неверное значение налогов ('%s') в строке %d файла.\n", temp_taxes_val, line_number);
            free(list_of_firms); fclose(file); *number_of_firms_ptr = 0; return NULL;
        }
        strncpy(current_firm.taxes, token, sizeof(current_firm.taxes) - 1);
        current_firm.taxes[sizeof(current_firm.taxes) - 1] = '\0';

        // 3. Дедлайн налога
        token = strtok_r(rest, "|", &rest);
        if (token == NULL) {
            fprintf(stderr, "Ошибка: неверный формат строки %d в файле (отсутствует дедлайн).\n", line_number);
            free(list_of_firms); fclose(file); *number_of_firms_ptr = 0; return NULL;
        }
        char temp_deadline_val[MAX_DATE_SIZE];
        strncpy(temp_deadline_val, token, sizeof(temp_deadline_val)-1);
        temp_deadline_val[sizeof(temp_deadline_val)-1] = '\0';
        trim_trailing_spaces(temp_deadline_val); // Для is_valid_date
        if (!is_valid_date(temp_deadline_val)) {
            fprintf(stderr, "Ошибка: неверная дата дедлайна ('%s') в строке %d файла.\n", temp_deadline_val, line_number);
            free(list_of_firms); fclose(file); *number_of_firms_ptr = 0; return NULL;
        }
        strncpy(current_firm.taxes_deadline, token, sizeof(current_firm.taxes_deadline) - 1);
        current_firm.taxes_deadline[sizeof(current_firm.taxes_deadline) - 1] = '\0';

        // 4. Дата оплаты налога
        token = strtok_r(rest, "|", &rest); // rest здесь должен содержать последний токен
        if (token == NULL) { // strtok_r вернет NULL, если больше нет токенов
             fprintf(stderr, "Ошибка: неверный формат строки %d в файле (отсутствует дата оплаты).\n", line_number);
            free(list_of_firms); fclose(file); *number_of_firms_ptr = 0; return NULL;
        }
        char temp_payed_val[MAX_DATE_SIZE];
        strncpy(temp_payed_val, token, sizeof(temp_payed_val)-1);
        temp_payed_val[sizeof(temp_payed_val)-1] = '\0';
        trim_trailing_spaces(temp_payed_val);
        if (!is_valid_date(temp_payed_val)) {
            fprintf(stderr, "Ошибка: неверная дата оплаты ('%s') в строке %d файла.\n", temp_payed_val, line_number);
            free(list_of_firms); fclose(file); *number_of_firms_ptr = 0; return NULL;
        }
        strncpy(current_firm.taxes_payed, token, sizeof(current_firm.taxes_payed) - 1);
        current_firm.taxes_payed[sizeof(current_firm.taxes_payed) - 1] = '\0';

        // Если все поля валидны, добавляем фирму в список
        list_of_firms[*number_of_firms_ptr] = current_firm;
        (*number_of_firms_ptr)++;
    }

    if (ferror(file)) {
        perror("Ошибка чтения из файла");
        free(list_of_firms);
        fclose(file);
        *number_of_firms_ptr = 0;
        return NULL;
    }

    fclose(file);
    printf("Данные успешно загружены из файла: %s. Загружено %d фирм(ы).\n", filename, *number_of_firms_ptr);
    
    // Опционально: уменьшить размер выделенной памяти до фактически использованного
    if (*number_of_firms_ptr > 0 && *number_of_firms_ptr < capacity) {
        firm_info* final_list = (firm_info*)realloc(list_of_firms, (*number_of_firms_ptr) * sizeof(firm_info));
        if (final_list != NULL) {
            list_of_firms = final_list;
        } // Если realloc не удался, остаемся с чуть большим буфером, это не критично
    } else if (*number_of_firms_ptr == 0) { // Если не загружено ни одной фирмы
        free(list_of_firms);
        list_of_firms = NULL;
    }


    return list_of_firms;
}


//* ======================== Restart ========================
void restart_program(int* flag)																				    // функция перезапуска программы
{
	printf("\nДля завершения работы программы введите \033[1;32m0\033[0m, иначе перезапуск программы.\n");	    // запрос на перезапуск программы 
    char buffer[10];
    if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        if (sscanf(buffer, "%d", flag) != 1) {
            *flag = 1; // По умолчанию перезапуск, если ввод некорректен
            printf("Некорректный ввод, программа будет перезапущена.\n");
        }
    } else {
         *flag = 0; // Ошибка ввода, завершаем программу
         printf("Ошибка чтения ввода для перезапуска, программа будет завершена.\n");
    }
    // Очистка остатка буфера stdin, если был введен не только флаг
    // Это не всегда нужно после fgets + sscanf, но может быть полезно
    // if (strchr(buffer, '\n') == NULL) { // Если fgets не нашел \n
    //    int c;
    //    while ((c = getchar()) != '\n' && c != EOF);
    // }


	if (*flag != 0)																								// проверка значения флага
		printf("Перезапуск программы...\n\n");																	// вывод сообщения о перезапуске программы
	else
		printf("Завершение работы...\n");																		// вывод сообщения о завершении работы программы
}