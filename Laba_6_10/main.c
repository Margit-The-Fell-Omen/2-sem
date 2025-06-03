#include <stdio.h>  // Для printf, fgets, sscanf и др. (хотя часть может быть в header.h)
#include <string.h> // Для strchr (если используется для очистки буфера)
#include "header.h" // Ваш заголовочный файл

// Имя файла для сохранения и загрузки данных
#define FILENAME "firms_data.txt"

int main() {
    int number_of_firms = 0;    // количество фирм
    int flag = 1;               // флаг главного цикла программы
    firm_info* list_of_firms = NULL; // массив структур фирм
    const char* month;          // название месяца для поиска топ-5

    char choice_buffer[10];     // Буфер для ввода пользовательского выбора (да/нет)
    int user_choice;            // Переменная для хранения выбора пользователя

    printf("Добро пожаловать в программу учета налогов фирм!\n");

    while (flag) {
        // Освобождаем память от предыдущей итерации, если она была выделена
        if (list_of_firms != NULL) {
            firm_info_free(list_of_firms);
            list_of_firms = NULL;
            number_of_firms = 0;
        }

        printf("\nХотите загрузить данные из файла '%s'? (1 - Да, 0 - Нет): ", FILENAME);
        user_choice = 0; // Значение по умолчанию
        if (fgets(choice_buffer, sizeof(choice_buffer), stdin) != NULL) {
            sscanf(choice_buffer, "%d", &user_choice);
        } else {
            printf("Ошибка чтения ввода. Попробуйте ввести данные вручную.\n");
        }

        if (user_choice == 1) {
            list_of_firms = load_firms_from_file(&number_of_firms, FILENAME);
            if (list_of_firms != NULL && number_of_firms > 0) {
                printf("Данные успешно загружены из файла. Загружено %d фирм(ы).\n", number_of_firms);
            } else {
                printf("Не удалось загрузить данные из файла, или файл пуст/поврежден.\n");
                printf("Пожалуйста, введите данные вручную.\n");
                // Сбрасываем, чтобы перейти к ручному вводу
                if (list_of_firms != NULL) firm_info_free(list_of_firms); // Если load_firms_from_file что-то вернул, но с 0 фирм
                list_of_firms = NULL;
                number_of_firms = 0; 
                user_choice = 0; // Указываем, что загрузка не удалась / не выбрана
            }
        }
        
        // Если данные не были загружены (или пользователь отказался), переходим к ручному вводу
        if (user_choice == 0) { // user_choice будет 0, если не хотели грузить или загрузка не удалась
            // Запрос количества фирм для ручного ввода
            // Используем min=1, max=999, как указано в сообщении input_int_var case 1
            input_int_var(&number_of_firms, 1, 1, 999); 
            list_of_firms = firm_info_alloc(number_of_firms);
            if (list_of_firms == NULL) {
                printf("\033[1;31mКритическая ошибка: не удалось выделить память для %d фирм.\033[0m\n", number_of_firms);
                restart_program(&flag); // Предложить перезапуск или выход
                continue; // Пропустить остаток цикла
            }
            // Ввод всех данных для фирм
            input_all_firm_data(list_of_firms, number_of_firms);
        }

        // Обработка и вывод данных, если они есть
        if (list_of_firms != NULL && number_of_firms > 0) {
            printf("\n--- Текущие данные о фирмах ---\n");
            output_firm_info(list_of_firms, number_of_firms, 3); // Вывод всех данных (тип 3)

            printf("\n--- Определение топ-5 фирм по неуплаченным налогам ---\n");
            month = input_month(); // функция ввода месяца
            output_top_5(list_of_firms, number_of_firms, month); // функция вывода топ-5 фирм

            // Запрос на сохранение данных перед перезапуском/выходом
            printf("\nХотите сохранить текущие данные в файл '%s'? (1 - Да, 0 - Нет): ", FILENAME);
            user_choice = 0; // Сброс для нового выбора
            if (fgets(choice_buffer, sizeof(choice_buffer), stdin) != NULL) {
                // Очистка буфера stdin, если fgets не считал \n (важно перед restart_program)
                if (strchr(choice_buffer, '\n') == NULL) {
                    int c;
                    while ((c = getchar()) != '\n' && c != EOF);
                }
                sscanf(choice_buffer, "%d", &user_choice);
            } else {
                 printf("Ошибка чтения ввода для сохранения. Данные не будут сохранены.\n");
            }


            if (user_choice == 1) {
                save_firms_to_file(list_of_firms, number_of_firms, FILENAME);
            }
        } else {
            printf("\nНет данных для обработки или вывода.\n");
        }

        // Освобождение памяти (если еще не освобождена в начале цикла)
        // Это дублирование, если цикл перезапускается. Лучше одна точка освобождения.
        // Убрано отсюда, так как есть в начале цикла.

        restart_program(&flag); // функция перезапуска или завершения программы
    }

    // Финальное освобождение памяти, если программа завершается без перезапуска и память была выделена
    if (list_of_firms != NULL) {
        firm_info_free(list_of_firms);
        list_of_firms = NULL;
    }

    printf("Программа завершена.\n");
    return 0;
}