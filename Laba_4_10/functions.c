#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"

//* ======================== QUEUE ===========================

Queue* create_queue() {
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    if (queue == NULL) {
         fprintf(stderr, "Критическая ошибка: не удалось выделить память для очереди.\n");
         return NULL;
    }
    queue->front = queue->rear = NULL;
    return queue;
}

int isEmpty(Queue* queue) 
{
	return (queue == NULL) ? ((queue->front == NULL && queue->rear == NULL) ? 1 : 0) : 0;

}

int enqueue(Queue* queue, int clinic_number, int total_beds, int free_beds, const char* location)
{
    // 0. Проверка указателя на саму очередь
    if (queue == NULL) {
        fprintf(stderr, "Ошибка: Попытка добавить элемент в NULL очередь.\n");
        return 0; // Ошибка: очередь не существует
    }

    // 1. Выделяем память для нового узла
    q_node* new_node = (q_node*)malloc(sizeof(q_node));

    // 2. Проверяем результат malloc
    if (new_node == NULL) {
        fprintf(stderr, "Ошибка: Не удалось выделить память для нового узла очереди.\n");
        // В реальном приложении здесь может быть более сложная обработка ошибки
        return 0; // Сообщаем об ошибке
    }

    // 3. Инициализируем ВСЕ поля нового узла
    new_node->clinic_number = clinic_number;
    new_node->total_beds = total_beds;
    new_node->free_beds = free_beds;
    new_node->patients = NULL; // Инициализируем указатель на массив пациентов
    new_node->next = NULL;     // Новый узел всегда будет последним, его next = NULL

    // Инициализируем указатель на функцию (ВАЖНО!)
    new_node->calculate_distance = NULL; // Или присвоить указатель на реальную функцию, если нужно
                                         // например: new_node->calculate_distance = some_distance_function;

    // Копируем строку местоположения (безопасно)
    if (location != NULL) {
        strncpy(new_node->location, location, MAX_STR_SIZE - 1);
        new_node->location[MAX_STR_SIZE - 1] = '\0'; // Гарантируем null-терминацию
    } else {
        new_node->location[0] = '\0'; // Пустая строка, если location == NULL
    }

    // 4. Добавляем узел в очередь
    if (queue->front == NULL) {
        // Если очередь пуста (front == NULL), то и rear должен быть NULL.
        // Новый узел становится и началом, и концом очереди.
        queue->front = new_node;
        queue->rear = new_node;
    }
    else {
        // Если очередь не пуста:
        // а) Текущий последний узел (на который указывает rear)
        //    должен указывать своим next на новый узел.
        queue->rear->next = new_node; // <<< ИСПРАВЛЕНО

        // б) Указатель на конец очереди (rear) теперь должен указывать
        //    на новый добавленный узел.
        queue->rear = new_node;       // <<< ИСПРАВЛЕНО
    }

    // Успешное добавление элемента
    return 1;
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
    if (!queue) return;

    q_node* current = queue->front;
    while (current != NULL) 
	{
        q_node* temp = current;
        current = current->next; // Переходим к следующему ДО освобождения temp

        // Освобождаем память, выделенную для имен пациентов
        if (temp->patients != NULL) 
		{
            // Проходим по всем возможным местам, т.к. память выделялась под total_beds
            // Но освобождаем только те указатели, которые не NULL (т.е. были реально выделены и/или заполнены)
            for (int i = 0; i < temp->total_beds; ++i) 
			{
                if (temp->patients[i] != NULL) 
				{ // Освобождаем только выделенную память
                    free(temp->patients[i]);
                }
            }
            // Освобождаем сам массив указателей
            free(temp->patients);
        }
        // Освобождаем узел очереди
        free(temp);
    }
    // Освобождаем структуру самой очереди
    free(queue);
}

//* ======================== FUNCTIONAL ========================
void menu(Queue* queue)
{
	int user_choice = 0;
	int user_output_choice = 0;
	int hospital_num = 0;
	q_node* current_node = queue->front;
	int h_num_flag = 1;
	int menu_flag = 1;
	char location[MAX_STR_SIZE];
	while (menu_flag) 
	{
		printf("Выберите действие: \n\t1 - ввод информации о больницах\n\t2 - вывод информации о больницах\n\t\
3 - Запись пациента в больницу\n\t4 - Выписка пациента из больницы\n\t5 - рассчёт расстояния от пациента до ближайшей больницы\n\t\
6 - выход из меню\n");
		input_int_var(&user_choice, 1, 6);
		switch (user_choice) 
		{
			case 6:
            free_queue(queue);
			menu_flag = 0;
			break;

			case 1:
			input_queue(queue);
			break;

			case 2:
			if (queue != NULL) 
			{
				printf("Выберите информацию для вывода:\n\t1 - вывод информации о свободных местах в больницах\n\t\
2 - вывод информации о пациентах в конкретной больнице");
				input_int_var(&user_output_choice, 1, 2);
				switch (user_output_choice) 
				{
					case 1:
					output_beds(queue);
					break;
					case 2:
					printf("Введите номер больницы");
                    input_int_var(&hospital_num, 1, 1000);
					output_patients(queue, hospital_num);
                    break;
                }
            }
			else
			{
				printf("Информация о больницах не введена! Введите информацию о больницах");
			}
			break;

			case 3:
			if (queue != NULL) 
			{
				add_patient(queue);
			}
			else
			{
				printf("Информация о больницах не введена! Введите информацию о больницах");
			}
			break;

			case 4:
			if (queue != NULL) 
			{
				delete_patient(queue);
			}
			else
			{
				printf("Информация о больницах не введена! Введите информацию о больницах");
			}
			break;

			case 5:
			if (queue != NULL) 
			{
				printf("Введите координаты больного:");
				input_str(location, 1);
				find_nearest_hospital(queue, location);
			}
			else
			{
				printf("Информация о больницах не введена! Введите информацию о больницах");
			}
			break;
		}
	}
}

void add_patient(Queue* queue) 
{
    int hospital_num;
    q_node* target_hospital = NULL; // Указатель на найденный узел больницы

    // 1. Цикл поиска больницы
    while (target_hospital == NULL) 
	{ // Повторяем, пока не найдем подходящую больницу
        printf("Введите номер принимающей больницы (или 0 для отмены):");
        input_int_var(&hospital_num, 0, 1000); // Позволяем ввести 0 для выхода

        if (hospital_num == 0) 
		{
            printf("Добавление пациента отменено.\n");
            return; // Выход из функции
        }

        // Итератор для поиска, НЕ изменяем queue->front
        q_node* current_node = queue->front;
        while (current_node != NULL) 
		{
            if (current_node->clinic_number == hospital_num) 
			{
                // Больница найдена! Проверяем места
                if (current_node->free_beds > 0) 
				{
                    target_hospital = current_node; // Сохраняем указатель
                } 
				else 
				{
                    printf("В больнице №%d нет свободных мест. Попробуйте другую.\n", hospital_num);
                    // target_hospital остается NULL, цикл поиска продолжится
                }
                break; // Выходим из внутреннего цикла поиска (while current_node)
            }
            current_node = current_node->next; // Переходим к следующему узлу
        }

        // Если внутренний цикл завершился, а target_hospital все еще NULL
        // (и мы не вышли из-за отсутствия мест), значит больница не найдена
        if (target_hospital == NULL && current_node == NULL) 
		{ // Проверяем, что дошли до конца списка
             printf("Больница с номером %d не найдена. Повторите ввод.\n", hospital_num);
        }
         // Если вышли из-за отсутствия мест, сообщение уже было выведено,
         // target_hospital == NULL, и внешний цикл продолжится.
    }

    // Если мы здесь, значит target_hospital указывает на больницу с местами

    char patient_surname[MAX_STR_SIZE];
    int surname_ok = 0; // Флаг, что фамилия подходит

    // 2. Цикл ввода и проверки фамилии
    while (!surname_ok) 
	{
        printf("Введите фамилию пациента (для больницы №%d): ", target_hospital->clinic_number);
        input_str(patient_surname, 0); // Получаем фамилию

        int duplicate_found = 0;
        int current_patients_num = target_hospital->total_beds - target_hospital->free_beds;

        // Проверяем на дубликаты
        for (int i = 0; i < current_patients_num; i++) 
		{
            // Сравниваем строки с помощью strcmp, проверяя на NULL
            if (target_hospital->patients[i] != NULL &&
                strcmp(target_hospital->patients[i], patient_surname) == 0)
            {
                printf("Пациент с фамилией '%s' уже находится в больнице №%d. Повторите ввод.\n",
                       patient_surname, target_hospital->clinic_number);
                duplicate_found = 1;
                break; // Выходим из цикла for
            }
        }

        if (!duplicate_found) 
		{
            surname_ok = 1; // Фамилия подходит, выходим из цикла while (!surname_ok)
        }
        // Если дубликат найден, цикл while (!surname_ok) повторится
    }

    // 3. Добавление пациента (если фамилия подошла)
    int insert_index = target_hospital->total_beds - target_hospital->free_beds;

    // Дополнительная проверка (хотя логика выше должна это гарантировать)
    if (insert_index >= target_hospital->total_beds || target_hospital->patients == NULL) 
	{
         fprintf(stderr, "Критическая ошибка: нет места для вставки пациента, хотя проверка коек пройдена.\n");
         return;
    }

    // Выделяем НОВУЮ память для строки с фамилией
    char* new_patient_name = (char*)malloc((strlen(patient_surname) + 1) * sizeof(char));
    if (new_patient_name == NULL) 
	{
        fprintf(stderr, "Ошибка: Не удалось выделить память для имени пациента.\n");
        return; // Не можем добавить пациента
    }

    // Копируем введенную фамилию в новую память
    strcpy(new_patient_name, patient_surname);

    // Сохраняем указатель на новую строку в массиве пациентов
    target_hospital->patients[insert_index] = new_patient_name;

    // Уменьшаем количество свободных мест
    target_hospital->free_beds--;

    printf("Пациент '%s' успешно добавлен в больницу №%d.\n",
           patient_surname, target_hospital->clinic_number);
}

void delete_patient(Queue* queue) 
{
    int hospital_num;
    q_node* target_hospital = NULL; // Указатель на найденный узел больницы

    // 1. Цикл поиска больницы
    while (target_hospital == NULL) 
	{
        printf("Введите номер больницы для выписки пациента (или 0 для отмены):");
        input_int_var(&hospital_num, 0, 1000); // Позволяем 0 для выхода

        if (hospital_num == 0) 
		{
            printf("Выписка пациента отменена.\n");
            return; // Выход из функции
        }

        // Итератор для поиска, НЕ изменяем queue->front
        q_node* current_node = queue->front;
        while (current_node != NULL) 
		{
            if (current_node->clinic_number == hospital_num) 
			{
                // Больница найдена! Проверяем, есть ли там пациенты
                if (current_node->free_beds < current_node->total_beds) 
				{
                    target_hospital = current_node; // Сохраняем указатель
                } 
				else 
				{
                    printf("В больнице №%d нет пациентов для выписки. Попробуйте другую.\n", hospital_num);
                    // target_hospital остается NULL, цикл поиска продолжится
                }
                break; // Выходим из внутреннего цикла поиска (while current_node)
            }
            current_node = current_node->next; // Переходим к следующему узлу
        }

        // Если внутренний цикл завершился, а target_hospital все еще NULL
        // (и мы не вышли из-за отсутствия пациентов), значит больница не найдена
        if (target_hospital == NULL && current_node == NULL) 
		{
             printf("Больница с номером %d не найдена. Повторите ввод.\n", hospital_num);
        }
         // Если вышли из-за отсутствия пациентов, сообщение уже было выведено,
         // target_hospital == NULL, и внешний цикл продолжится.
    }

    // Если мы здесь, значит target_hospital указывает на больницу с пациентами

    char patient_surname[MAX_STR_SIZE];
    int patient_index = -1; // Индекс найденного пациента в массиве patients, -1 = не найден

    // 2. Цикл ввода и поиска фамилии
    while (patient_index == -1) 
	{
        printf("Введите фамилию пациента для выписки из больницы №%d (или 'cancel' для отмены): ", target_hospital->clinic_number);
        input_str(patient_surname, 0); // Получаем фамилию

        if (strcmp(patient_surname, "cancel") == 0) 
		{
             printf("Выписка пациента отменена.\n");
             return;
        }


        int current_patients_num = target_hospital->total_beds - target_hospital->free_beds;

        // Ищем пациента по фамилии
        for (int i = 0; i < current_patients_num; i++) 
		{
            // Сравниваем строки с помощью strcmp, проверяя на NULL
            if (target_hospital->patients[i] != NULL &&
                strcmp(target_hospital->patients[i], patient_surname) == 0)
            {
                patient_index = i; // Пациент найден! Сохраняем индекс
                break; // Выходим из цикла for
            }
        }

        // Если после цикла for patient_index все еще -1, пациент не найден
        if (patient_index == -1) 
		{
            printf("Пациент с фамилией '%s' не найден в больнице №%d. Повторите ввод или введите 'cancel'.\n",
                   patient_surname, target_hospital->clinic_number);
            // Цикл while(patient_index == -1) повторится
        }
        // Если пациент найден (patient_index != -1), цикл while завершится
    }

    // 3. Удаление пациента (если найден)
    printf("Найден пациент '%s' для выписки...\n", target_hospital->patients[patient_index]);

    // а) Освобождаем память, выделенную под фамилию
    free(target_hospital->patients[patient_index]);
    target_hospital->patients[patient_index] = NULL; // Делаем указатель недействительным (на всякий случай)

    // б) Сдвигаем указатели в массиве, чтобы заполнить пробел
    int current_patient_count = target_hospital->total_beds - target_hospital->free_beds;
    // Сдвигаем элементы, начиная с удаленного, до предпоследнего пациента
    for (int i = patient_index; i < current_patient_count - 1; i++) {
        target_hospital->patients[i] = target_hospital->patients[i + 1];
    }
    // Последний "занятый" слот теперь должен быть NULL
    if (current_patient_count > 0) { // Убедимся, что был хотя бы 1 пациент
        target_hospital->patients[current_patient_count - 1] = NULL;
    }


    // в) Увеличиваем количество свободных мест
    target_hospital->free_beds++;

    // г) Удаляем неверный realloc
    // temp->front->patients = realloc(temp->front->patients, --patients_num); // НЕПРАВИЛЬНО, УДАЛИТЬ

    printf("Пациент '%s' успешно выписан из больницы №%d.\n",
           patient_surname, target_hospital->clinic_number);
}

// Вспомогательная функция для парсинга строки координат
// Возвращает 1 при успехе, 0 при ошибке
int parse_coordinates(const char *coord_str, double *latitude, double *longitude) 
{
    // Находим запятую - разделитель широты и долготы
    char *comma_ptr = strchr(coord_str, ',');
    if (comma_ptr == NULL) 
	{
        fprintf(stderr, "Ошибка: Запятая не найдена в строке координат: %s\n", coord_str);
        return 0; // Ошибка формата
    }

    char *endptr_lat;
    char *endptr_lon;

    // Преобразуем часть строки до запятой в широту (double)
    // strtod пропускает начальные пробелы
    *latitude = strtod(coord_str, &endptr_lat);

    // Проверяем, что strtod остановился на пробеле или на запятой
    char *temp_ptr = endptr_lat;
    while (isspace((unsigned char)*temp_ptr)) 
	{
        temp_ptr++;
    }
    if (temp_ptr != comma_ptr) 
	{
         fprintf(stderr, "Ошибка: Неверный формат перед запятой в строке: %s\n", coord_str);
         return 0; // Ошибка формата
    }

    // Преобразуем часть строки после запятой в долготу (double)
    // strtod пропускает начальные пробелы
    *longitude = strtod(comma_ptr + 1, &endptr_lon);

     // Проверяем, что после числа остались только пробельные символы (или конец строки)
    while (isspace((unsigned char)*endptr_lon)) 
	{
        endptr_lon++;
    }
    if (*endptr_lon != '\0') 
	{
         fprintf(stderr, "Ошибка: Недопустимые символы после долготы в строке: %s\n", coord_str);
         return 0; // Ошибка формата
    }

    // Простая проверка на случай, если strtod не смог ничего преобразовать (вернул 0)
    // и сама строка не начиналась с '0'. Для большей надежности можно проверять errno.
    if (*latitude == 0 && endptr_lat == coord_str) 
	{
        fprintf(stderr, "Ошибка: Не удалось преобразовать широту из строки: %s\n", coord_str);
        return 0;
    }
    if (*longitude == 0 && endptr_lon == comma_ptr + 1) 
	{
        fprintf(stderr, "Ошибка: Не удалось преобразовать долготу из строки: %s\n", coord_str);
        return 0;
    }


    return 1; // Успешный парсинг
}

// Функция для перевода градусов в радианы
double degrees_to_radians(double degrees) 
{
    // M_PI определена в math.h (может потребоваться #define _USE_MATH_DEFINES)
    return degrees * M_PI / 180.0;
}

/**
 * @brief Рассчитывает расстояние между двумя географическими точками по их координатам.
 *
 * Функция использует формулу гаверсинуса для расчета расстояния по большому кругу
 * на сфере, аппроксимирующей Землю.
 *
 * @param coord1_str Строка с координатами первой точки в формате "<широта>[пробелы]","[пробелы]<долгота>".
 * @param coord2_str Строка с координатами второй точки в том же формате.
 * @return Расстояние между точками в километрах. Возвращает -1.0 в случае ошибки
 *         парсинга входных строк координат.
 */
 double calculate_distance(const char *coord1_str, const char *coord2_str)
 {
	 double lat1_deg, lon1_deg, lat2_deg, lon2_deg;
 
	 // Проверка входных указателей
	 if (coord1_str == NULL || coord2_str == NULL) 
	 {
		 fprintf(stderr, "Ошибка: Передан NULL указатель на строку координат.\n");
		 return -1.0;
	 }
 
	 // Парсинг координат из строк
	 if (!parse_coordinates(coord1_str, &lat1_deg, &lon1_deg))
	 {
		 // Сообщение об ошибке уже выводится внутри parse_coordinates
		 // fprintf(stderr, "Ошибка парсинга координат точки 1: '%s'\n", coord1_str);
		 return -1.0; // Возвращаем значение ошибки
	 }
	 if (!parse_coordinates(coord2_str, &lat2_deg, &lon2_deg))
	 {
		 // Сообщение об ошибке уже выводится внутри parse_coordinates
		 // fprintf(stderr, "Ошибка парсинга координат точки 2: '%s'\n", coord2_str);
		 return -1.0; // Возвращаем значение ошибки
	 }
 
	 // Перевод градусов в радианы
	 double lat1_rad = degrees_to_radians(lat1_deg);
	 double lon1_rad = degrees_to_radians(lon1_deg);
	 double lat2_rad = degrees_to_radians(lat2_deg);
	 double lon2_rad = degrees_to_radians(lon2_deg);
 
	 // Средний радиус Земли в километрах
	 const double R = 6371.0;
 
	 // Разница долгот и широт
	 double dlon = lon2_rad - lon1_rad;
	 double dlat = lat2_rad - lat1_rad;
 
	 // Формула гаверсинуса
	 double a = pow(sin(dlat / 2.0), 2) + cos(lat1_rad) * cos(lat2_rad) * pow(sin(dlon / 2.0), 2);
	 // Защита от ошибок округления, приводящих к sqrt(отрицательное_число) или sqrt(>1)
	 if (a < 0.0) a = 0.0;
	 if (a > 1.0) a = 1.0;
	 double c = 2.0 * atan2(sqrt(a), sqrt(1.0 - a));
	 double distance = R * c;
	//  printf("%lf", distance);
	 return distance; // Возвращаем расстояние в километрах
 }
 
 // --- Исправленная функция find_nearest_hospital ---
  double find_nearest_hospital(Queue* queue, const char* location)
 {
	 // Проверка входных данных
	 if (queue == NULL || location == NULL) 
	 {
		  fprintf(stderr, "Ошибка: Передан NULL указатель на очередь или локацию.\n");
		  return -2.0; // Код ошибки для NULL аргументов
	 }
	 if (queue->front == NULL) 
	 {
		 fprintf(stderr, "Предупреждение: Очередь больниц пуста.\n");
		 return -1; // Возвращаем "бесконечность" - нет больниц для сравнения
	 }
 
	 // Попытка распарсить координаты целевой локации один раз перед циклом
	 double target_lat_deg, target_lon_deg;
	 if (!parse_coordinates(location, &target_lat_deg, &target_lon_deg)) 
	 {
		 // Сообщение уже выведено в parse_coordinates
		 return -1.0; // Код ошибки для неверной целевой локации
	 }
 
 
	 double min_distance = 9999; // Инициализируем максимальным значением double
	 int min_dist_hospital_num = -1; // Инициализируем невалидным номером
	 int found_valid = 0; // Флаг, что найдена хотя бы одна валидная больница
 
	 // Используем временный указатель для итерации, НЕ изменяя queue->front
	 q_node* current = queue->front;
 
	 while (current != NULL) // Стандартная итерация по связному списку
	 {
 
		 // Вычисляем расстояние (используем уже распарсенную целевую локацию)
		 // Передаем current->location, а не location (ошибка в оригинале)
		 double distance = calculate_distance(current->location, location);
 
		 // Вывод отладочной информации (можно закомментировать в релизе)
		 // printf("Отладка: Расстояние до больницы #%d (%s): ", current->clinic_number, current->location);
		 if (distance >= 0.0) 
		 { // Проверяем, что calculate_distance не вернул ошибку (-1.0)
			 // printf("%lf км\n", distance); // Выводим только если расчет успешен
			 if (distance < min_distance)
			 {
				 min_distance = distance; // Обновляем минимальное расстояние
				 min_dist_hospital_num = current->clinic_number; // Обновляем номер ближайшей
				 found_valid = 1; // Отмечаем, что нашли хотя бы одну
			 }
		 }
 
		 // Переходим к следующему узлу
		 current = current->next;
	 }
 
	 // Вывод результата после цикла
	 if (found_valid) 
	 {
		 printf("Ближайшая больница: №%d, Расстояние: %lf км\n", min_dist_hospital_num, min_distance);
		 // Записываем номер больницы, если передан указатель
	 } 
	 else 
	 {
		 printf("Не удалось найти ближайшую больницу (возможно, все координаты больниц некорректны или очередь пуста).\n");
		 // Возвращаем DBL_MAX, так как сравнения не производились или все были с ошибками
		  return -1;
	 }
 
	 return min_distance; // Возвращаем найденное минимальное расстояние
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

			if (check_num == 1 && !validateLatLon(buffer))
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
    printf("Введите информацию о больницах:\n");
    printf("Введите количество больниц:");
    input_int_var(&hospital_num, 1, 100); // Получаем общее количество больниц

    for (int i = 0; i < hospital_num; i++) 
	{
        printf("\n--- Ввод данных для больницы %d ---\n", i + 1);

        // 1. Создаем НОВЫЙ узел (пока без данных) и добавляем его в очередь
        //    Функция enqueue должна правильно обновить queue->rear
        enqueue(queue, 0, 0, 0, NULL); // Используем field=0 для создания узла с дефолтными значениями
        // 2. Получаем указатель на ТОЛЬКО ЧТО добавленный узел (он теперь в хвосте)
        q_node* current_node = queue->rear; // Вот ключевое изменение! Работаем с rear.

        // 3. Заполняем данные для current_node (нового узла)
        printf("Введите номер больницы:");
        input_int_var(&(current_node->clinic_number), 1, 100); // Пример диапазона

        printf("Введите координаты больницы (например, '55.75,37.61'):");
        input_str(current_node->location, 1); // Используем ваш input_str

        printf("Введите общее количество мест в больнице:");
        input_int_var(&(current_node->total_beds), 1, 1000); // Пример диапазона

        // 4. Выделяем память под массив указателей на имена пациентов
        //    Размер массива равен ОБЩЕМУ числу мест
        current_node->patients = (char**)malloc(current_node->total_beds * sizeof(char*));
        if (current_node->patients == NULL) 
		{
            fprintf(stderr, "Ошибка: Не удалось выделить память для списка пациентов больницы %d\n", current_node->clinic_number);
            // В реальном приложении здесь нужна более надежная обработка ошибок
            // (освободить уже выделенную память для этой больницы? прервать ввод?)
            // Пока просто инициализируем нулем, чтобы избежать падения дальше
             current_node->total_beds = 0; // Сбрасываем, чтобы не было доступа к patients
             current_node->free_beds = 0;
             current_node->patients = NULL; // Явно указываем
             continue; // Переходим к следующей больнице (или можно вернуть код ошибки)
        }
         // ВАЖНО: Инициализируем все указатели в массиве NULL
         // Это поможет при освобождении памяти и при проверке, занято ли место
         for (int k = 0; k < current_node->total_beds; ++k) 
		 {
             current_node->patients[k] = NULL;
         }


        printf("Введите количество СВОБОДНЫХ мест в больнице:");
        // Свободных мест не может быть больше, чем всего мест
        input_int_var(&(current_node->free_beds), 0, current_node->total_beds);

        // 5. Вводим имена пациентов (если есть занятые места)
        int patient_num = current_node->total_beds - current_node->free_beds;
        if (patient_num < 0) 
		{ // Дополнительная проверка
             fprintf(stderr, "Предупреждение: Количество свободных мест (%d) больше общего (%d) для больницы %d. Количество пациентов будет 0.\n",
                     current_node->free_beds, current_node->total_beds, current_node->clinic_number);
             patient_num = 0;
        }


        if (patient_num > 0) 
		{
            printf("Введите фамилии %d пациентов, находящихся в больнице %d:\n", patient_num, current_node->clinic_number);
            for (int j = 0; j < patient_num; j++) { // Цикл по количеству ПАЦИЕНТОВ
                // Выделяем память под строку КАЖДОГО пациента
                current_node->patients[j] = (char*)malloc(MAX_STR_SIZE * sizeof(char));
                if (current_node->patients[j] == NULL) 
				{
                     fprintf(stderr, "Ошибка: Не удалось выделить память для имени пациента %d в больнице %d\n", j + 1, current_node->clinic_number);
                     // Обработка ошибки: можно пропустить ввод этого пациента или прервать все
                     // Пока просто пропустим ввод остальных для этой больницы
                     break;
                }
                printf("Введите фамилию %d-го пациента:", j + 1);
                // Читаем строку в выделенную память. Используем ИНДЕКС j.
                input_str(current_node->patients[j], 0);
            }
        } 
		else 
		{
             printf("В больнице %d нет пациентов.\n", current_node->clinic_number);
        }
    }
    return hospital_num; // Возвращаем количество успешно обработанных (или запрошенных) больниц
}

//* ======================== OUTPUT ========================
void output_beds(Queue* queue) 
{
    // 1. Проверяем, существует ли очередь и не пуста ли она
    if (queue == NULL) 
	{
        printf("Ошибка: Указатель на очередь равен NULL.\n");
        return;
    }
    if (queue->front == NULL) 
	{
        printf("Очередь пуста. Нет данных для вывода.\n");
        return;
    }

    // 2. Создаем указатель для ПЕРЕМЕЩЕНИЯ по узлам (типа q_node*)
    //    и инициализируем его НАЧАЛОМ очереди (front).
    q_node* current_node = queue->front;

    printf("Данные о больницах в очереди:\n");
    // 3. Используем более аккуратное форматирование таблицы
    printf("+-------+-------------------+---------------------------+\n");
    printf("|   №   |  Количество мест  | Количество свободных мест |\n");
    printf("+-------+-------------------+---------------------------+\n");

    // 4. Цикл продолжается, пока current_node не станет NULL (конец списка)
    while (current_node != NULL) 
	{
        // 5. Печатаем данные ТЕКУЩЕГО узла (current_node)
        //    Используем ширину полей для выравнивания
        printf("| %5d | %17d | %25d |\n",
               current_node->clinic_number,
               current_node->total_beds,
               current_node->free_beds);

        // 6. Перемещаем указатель на СЛЕДУЮЩИЙ узел.
        //    НЕ изменяем оригинальную структуру queue!
        current_node = current_node->next;
    }

    // 7. Печатаем нижнюю границу таблицы после цикла
    printf("+-------+-------------------+---------------------------+\n");
}

void output_patients(Queue* queue, int hospital_num) {
    // 1. Проверка на пустую очередь или невалидный указатель queue
    if (queue == NULL || queue->front == NULL) {
        printf("Очередь пуста или не инициализирована.\n");
        return; // Выходим, если очередь пуста
    }

    // 2. Используем временный указатель для *перебора* узлов, НЕ изменяя очередь
    q_node* current_node = queue->front;

    // 3. Ищем нужную больницу в очереди
    while (current_node != NULL && current_node->clinic_number != hospital_num) {
        current_node = current_node->next; // Переходим к следующему узлу
    }

    // 4. Проверяем, нашли ли мы больницу
    if (current_node == NULL) {
        printf("Больница с номером %d не найдена в очереди.\n", hospital_num);
        return; // Выходим, если больница не найдена
    }

    // 5. Больница найдена (current_node указывает на нее). Рассчитываем кол-во пациентов.
    //    Добавляем проверку на отрицательное значение, хотя это маловероятно при корректных данных
    int patient_num = current_node->total_beds - current_node->free_beds;
    if (patient_num < 0) {
        patient_num = 0; // Кол-во пациентов не может быть отрицательным
    }

    // 6. Печатаем информацию о больнице и шапку таблицы
    printf("Данные о больнице №%d (Местоположение: %s):\n",
           current_node->clinic_number, current_node->location);
    printf("Всего мест: %d, Свободно мест: %d, Занято мест (пациентов): %d\n",
           current_node->total_beds, current_node->free_beds, patient_num);

    // 7. Проверяем, есть ли вообще данные о пациентах
    if (current_node->patients == NULL) {
        if (patient_num > 0) {
             printf("Информация о пациентах отсутствует (массив patients == NULL), хотя места заняты.\n");
        } else {
             printf("Пациентов нет.\n");
        }
        return; // Выходим, если нет массива пациентов
    }

    // 8. Печатаем таблицу пациентов, если они есть
    if (patient_num > 0) {
        printf("Список пациентов:\n");
        // Выравниваем ширину столбцов (примерные значения, можно подстроить)
        printf("+------------+------------+-------------------------+\n");
        printf("| № больницы | № пациента |   Фамилия пациента      |\n");
        printf("+------------+------------+-------------------------+\n");
        for (int i = 0; i < patient_num; i++) {
            // Дополнительная проверка: убедимся, что строка пациента не NULL
            const char* patient_name = *(current_node->patients + i); // или current_node->patients[i]
            if (patient_name == NULL) {
                 printf("| %-10d | %-10d | %-23s |\n", current_node->clinic_number, i + 1, "(ошибка: NULL имя)");
            } else {
                 // Используем форматирование для выравнивания столбцов
                 // %-Nd означает выравнивание по левому краю в поле шириной N
                 printf("| %-10d | %-10d | %-23s |\n", current_node->clinic_number, i + 1, patient_name);
            }
        }
        // Печатаем нижнюю границу таблицы один раз после цикла
        printf("+------------+------------+-------------------------+\n");
    } else {
        printf("В больнице №%d нет пациентов.\n", hospital_num);
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